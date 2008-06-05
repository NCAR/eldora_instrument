#!/usr/bin/python

import sys
import time
import os
import subprocess

from PyQt4.QtCore    import *
from PyQt4.QtGui     import *

from EldoraMain      import *
from EldoraRPC       import *
from QtConfig        import *
from EmitterProc     import *

####################################################################################

# Basically a dictionary which will return either the value set for a key,
# or the key itself if no value has been set.
class ApplicationDict:
    def __init__(self):
        self.dict = {}
        
    def __getitem__(self, key):
        # return the appropriate entry from our dictionary, or return the
        # key itself if we have no entry for it
        return (self.dict.has_key(key) and self.dict[key]) or key
    
    def __setitem__(self, key, value):
        self.dict[key] = value
    
# Global variables. These probably don't need to be declared global here, but
# we do this in order to keep global objects identified.

global ourConfig    # the EldoraGui.ini configuration

global eldoraDir    # used to find eldora application binaries
global ddsRoot      # used to locate DCPSInforepo
global ddsConfigDir # location of the DDS configuration files (for DDS apps)
global headerDirs   # where the headers are stored

global drxrpc       # RPC server for eldoradrx
global hskprpc      # RPC server for the housekeeper
global prodrpc      # RPC server for the products generator

global ourProcesses # Processes that we have started and are managing


####################################################################################
def start():
    try:
        # send a stop and a start command, although these may not
        # not be used in the final implementation
        r = drxrpc.radarStop()
        main.statusLabel.setText(QString(r))
        r = drxrpc.radarStart()
        main.statusLabel.setText(QString(r))
    except Exception, e:
        print "Error contacting drx RPC for start:", e

    try:
        r = hskprpc.server.Start()
        if (r != 0):
            main.statusLabel.setText("Failed to start housekeeper")
    except Exception, e:
        print "Error contacting housekeeper RPC for start:", e

    try:
        print "starting housekeeper"
        r = hskprpc.server.Start()
        if (r != 0):
            main.statusLabel.setText("Failed to start housekeeper")
    except Exception, e:
        print "Error contacting housekeeper RPC for start:", e
        pass
        
    # stop the eldora apps
    stopEldoraApps()
    
    # start the eldora apps
    startEldoraApps()
    
####################################################################################
def stop():
    try:
        # send a stop command, although this may not
        # not be used in the final implementation
        r = drxrpc.radarStop()
        main.statusLabel.setText(QString(r))
    except Exception, e:
        print "Error contacting drx RPC for stop:", e
        pass
    
    try:
        r = hskprpc.server.Stop()
        if (r != 0):
            main.statusLabel.setText("Failed to stop housekeeper")
    except Exception, e:
        print "Error contacting housekeeper RPC for stop:", e
        
    # stop the eldora apps
    stopEldoraApps()
    
####################################################################################
def status():
    ''' Query the eldora applications for status, and transmit this
    information back to the main display.
    '''
    global drxrpc
    global prodrpc

    # determine the product status and product rates
    productRate = 0
    productStatus = 2
    try:
        r = prodrpc.status()
        ABPrate = r['numAbpBeams']
        productRate = r['numProductBeams']
        numDiscards = r['discardBeamsAft'] + r['discardBeamsFor']
        productStatus = 0
        if numDiscards > 0 or ABPrate < 2000 or productRate < 400:
            productStatus = 1
        if numDiscards > 4 or ABPrate < 1000 or productRate < 300:
            productStatus = 2
    except Exception, e:
        print "Error contacting products RPC for status:", e
        ABPrate = 0
        productStatus = 2
    
    # determine the DRX status and rates.
    try:
       r = drxrpc.status()
       keys = sorted(r.keys())
       rates = []
       for k in keys:
          # convert rate from MB/s to KB/s and save
          rates.append(r[k]*1000.0)
          
    except Exception, e:
        print "Error contacting drx RPC for status:", e
        rates = []
        for i in range(16):
            rates.append(0)
            
    main.showStatus(ABPrate=ABPrate, 
                    productRate=productRate, 
                    productStatus=productStatus, 
                    rates=rates)
            
       
####################################################################################
def startEldoraApps():
    ''' Run eldora applications which have been selected in the configuration.
    '''
    # @todo must add logic to insure that the processes are not still running.
    # it is possible that previous terminate() calls did not work.
    
    # drx
    runDrx()
    
    # products
    runProducts()
        
####################################################################################
def stopEldoraApps():
    ''' Stop the standard Eldora processing apps. Try first with a SIGTERM. If
    that fails, try a SIGKILL. Remove them from ourProcesses. Note that 
    removing a an entry from ourProcesses removes its reference, and it
    will go out of scope. This causes the QProcess to kill the job.
    '''
    global ourProcesses
    for key in ('eldoradrx', 'eldoraprod'):
        if key in ourProcesses.keys():
            msg = 'Terminating ' + key
            main.logText(msg)
            proc = ourProcesses[key]
            proc.terminate() 
            terminated = proc.waitForFinished(1500)
            if not terminated:
                msg = 'Killing ' + key
                main.logText(msg)
                proc.kill()
                killed = proc.waitForFinished(1500)
                if not killed:
                    msg = 'Could not kill ' + key
                    main.logText(msg)
            # remove reference to the processes.
            del ourProcesses[key]
    
####################################################################################
def runDcps():
    '''
    Run the DCPSInfoRepo, but only if the configuration has
    called for that. Automatic restart of a currently running
    DCPSInfoRepo can be specified.
    
    The configuration is checked to see if Dcps/RunDcps is true. 
    
    If Dcps/AutoRestartDcps is false, and the process is already
    running, nothing will happen.
    
    If Dcps/AutoRestartDcps is true, a running copy will be killed before 
    a new one is started.
    
    If Dcps/AutoRestartDcps
    '''
    global ourConfig
    global ourProcesses

    runDcps = ourConfig.getBool('Dcps/Run', True)
    if  not runDcps:
        return
    
    restart = ourConfig.getBool('Dcps/AutoRestart', False)
    # see if it is already running
    isRunning = not pgrep('DCPSInfoRepo')
    if isRunning:
        if not restart:
            return
        else:
            pkill('DCPSInfoRepo')
            
    # start a new instance
    dcpsConfigPath = ourConfig.getString('Dcps/DcpsConfig', ddsConfigDir+'/DCPSInfoRepo.ini')
    orbConfigPath = ourConfig.getString('Dcps/OrbConfig', ddsConfigDir+'/ORBSvc.conf')
    domainConfigPath = ourConfig.getString('Dcps/DomainConfig', ddsConfigDir+'/DDSDomainIds.conf')
    dcpscmd = [
        os.path.join(ddsRoot, 'bin', 'DCPSInfoRepo'), 
        '-NOBITS', 
        '-DCPSConfigFile', dcpsConfigPath, 
        '-ORBSvcConf', orbConfigPath, 
        '-ORBListenEndpoints iiop://dcpsrepo:50000', 
        '-d', domainConfigPath, 
        ]
    ourProcesses['DCPSInfoRepo'] = EmitterProc(dcpscmd, emitText=False, payload=nextTaskColor())
    s = ourProcesses['DCPSInfoRepo']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.startDetached()
    time.sleep(1)

####################################################################################
def runDrx():
    '''
    Run the drx if called for by the configuration. 
    
    The configuration is hecked to see if Products/RunDrx is true.
    '''
    global ourConfig
    global ourProcesses

    doDrx = ourConfig.getBool('Drx/Run', True)
    if not doDrx:
        return
    
    # see if it is already running
    isRunning = not pgrep('eldoradrx')
    if isRunning:
        return
    
    # start a new instance with standard options
    drxcmd = [
           appDict['eldoradrx'], 
           '--start0', 
           '--start1', 
           '--pub', 
           ]
    
    # get the rpc port number
    rpcport = ourConfig.getInt('Drx/RpcPort', 60000)
    drxcmd.append('--rpcport')
    drxcmd.append(str(rpcport))
    
    # see if hskp should be simulated
    drxSimHskpMode = ourConfig.getBool('Mode/SimulateHskp', False)
    if drxSimHskpMode:
        drxcmd.append('--simHskp')
        
    # see if RR314 should be simulated
    drxSimRRMode = ourConfig.getBool('Mode/SimulateRR314', False)
    if drxSimRRMode:
        drxcmd.append('--simRR314')
        # get the usleep rate
        usleep = ourConfig.getInt('Mode/Usleep', 9000)
        drxcmd.append('--usleep')
        drxcmd.append(str(usleep))
        
    # create the process
    s = EmitterProc(command=drxcmd, emitText=True, payload=nextTaskColor())
    ourProcesses['eldoradrx'] = s
    
    # send text output to the text logger in the main gui
    QObject.connect(s, SIGNAL("text"), main.logText)
    
    # start eldoradrx
    s.start()
    
    # and puase to catch our breath
    time.sleep(1)
    
####################################################################################
def runProducts():
    '''
    Run the products generator, if called for by the configuration. 
    
    The configuration is checked to see if Products/RunProducts is true.
    '''
    global ourConfig
    global ourProcesses
    
    doProducts = ourConfig.getBool('Products/Run', True)
    if  not doProducts:
        return
    
    # see if it is already running
    isRunning = not pgrep('eldoraprod')
    if isRunning:
        return
    
    # start a new instance
    productscmd = [appDict['eldoraprod'], ]
    ourProcesses['eldoraprod'] = EmitterProc(productscmd, emitText=True, payload=nextTaskColor())
    s = ourProcesses['eldoraprod']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.start()
    
####################################################################################
def scope():
    ''' Start the scope display
    '''
    cmd = [appDict['eldorascope'],]
    ourProcesses['eldorascope'] = EmitterProc(cmd, 
                                              emitText=True, 
                                              payload=nextTaskColor())
    s = ourProcesses['eldorascope']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.start()
####################################################################################
def ppi(forradar=True):
    ''' Start the ppi display. forradar == True for the
    forward radar, False for the aft radar.
    '''
    cmd = [appDict['eldorappi'],]
    if forradar:
        cmd.append('--forward')
        cmdkey = 'eldorappifor'
    else:
        cmd.append('--aft')
        cmdkey = 'eldorappiaft'
    ourProcesses[cmdkey] = EmitterProc(cmd, 
                                       emitText=True, 
                                       payload=nextTaskColor())
    s = ourProcesses[cmdkey]
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.start()
####################################################################################
def pkill(name):
    ''' Execute pkill <name> via QProcess. Return
    the pkill exit status
    '''
    p = QProcess()
    p.start('/usr/bin/kill', [name,])
    p.waitForFinished(-1)
    return p.exitCode()

####################################################################################
def pgrep(name):
    ''' Execute pgrep <name> via QProcess. Return
    the pgrep exit status.
    '''
    p = QProcess()
    p.start('/usr/bin/pgrep', [name, ])
    p.waitForFinished(-1)
    return p.exitCode()

####################################################################################
def mainIsReady():
    '''
    Call this to perform any activities
    which should take place after the main Qt app has been started.
    For instance, any object that are created from QThread should be
    constructed after the main app is running.
    '''
    # start up DCPS
    runDcps()
    
    # stop any running eldora applications
    stopEldoraApps()

####################################################################################
def fixLdLibraryPath():
    ldLibraryPath = os.getenv('LD_LIBRARY_PATH')
    if ldLibraryPath == None:
        # it has been stripped by kde!
        try:
            ldLibraryPath = ''
            ldLibraryPath = ldLibraryPath + os.getenv('ACE_ROOT') + '/lib' + ':'
            ldLibraryPath = ldLibraryPath + os.getenv('TAO_ROOT') + '/lib' + ':'
            ldLibraryPath = ldLibraryPath + os.getenv('DDS_ROOT') + '/lib' + ':'
            ldLibraryPath = ldLibraryPath + os.getenv('QTDIR') + '/plugins/designer'
            os.putenv('LD_LIBRARY_PATH', ldLibraryPath)
            os.environ['LD_LIBRARY_PATH'] = ldLibraryPath
        except Exception, e:
            print 'Exception ', e
    
####################################################################################
def initConfig():
    ''' Create the configuratiuon and set the path variables
    '''
    global ourConfig
    # get our configuration
    ourConfig = QtConfig('NCAR', 'EldoraGui')
    
    # top (source) directory for finding our apps
    global eldoraDir
    try:
        eldoraDir = os.environ['ELDORADIR']
    except:
        # If the environment variable is not set, use the parent directory
        # of the location of this script
        mydir, myname = os.path.split(sys.argv[0])
        eldoraDir = os.path.abspath(os.path.join(mydir, '..'))
        os.environ['ELDORADIR'] = eldoraDir
    # where is DCPSInfoRepo
    global ddsRoot
    ddsRoot = os.environ['DDS_ROOT']
    
    # where are the DDS configuration files?
    global ddsConfigDir
    ddsConfigDir = os.path.join(eldoraDir, 'conf')
    
   # where are the Eldora header files?
    global headerDirs
    headerDirs = ourConfig.getString('Headers/HeaderDir', 
                                     os.path.join(eldoraDir, 'headermaker', 'headers'))

    # build a special dictionary of apps which returns a specific path for
    # an app, if set, otherwise just the app name
    global appDict
    appDict = ApplicationDict()

    # add apps that are found in eldoraDir/<appName>/<appName>
    for app in ['eldoradrx', 'eldoraprod', 'eldorappi', 'eldorascope']:
        appDict[app] = os.path.join(eldoraDir, app, app)
    # dumpheader lives in the headermaker dir
    appDict['dumpheader'] = os.path.join(eldoraDir, 'headermaker', 'dumpheader')
    # DcpsInfoRepo location
    appDict['DCPSInfoRepo'] = os.path.join(ddsRoot, "bin", "DCPSInfoRepo")
    

####################################################################################
def createRpcServers():
    global ourConfig
    
    # create the rpc for the drx
    drxrpchost = ourConfig.getString('Drx/Host', 'drx')
    drxrpcport = ourConfig.getInt('Drx/RpcPort', 60000)
    drxrpcurl = 'http://' + drxrpchost + ':' + str(drxrpcport)
    global drxrpc
    drxrpc = EldoraRPC('drx', drxrpcurl)
    drxrpc.start()
    
    # create the rpc for the housekeeper
    hskprpchost = ourConfig.getString('Hksp/Host', 'hskp')
    hskprpcport = ourConfig.getInt('Hksp/RpcPort', 60001)
    hskprpcurl = 'http://' + hskprpchost + ':' + str(hskprpcport)
    global hskprpc
    hskprpc = EldoraRPC('hskp', hskprpcurl)
    hskprpc.start()
    
    # create the rpc for the products generator
    prodrpchost = ourConfig.getString('Products/Host', 'archiver')
    prodrpcport = ourConfig.getInt('Products/RpcPort', 60002)
    prodrpcurl = 'http://' + prodrpchost + ':' + str(prodrpcport)
    global prodrpc
    prodrpc = EldoraRPC('products', prodrpcurl)
    prodrpc.start()

####################################################################################
def headerSelected(selectedHeader):
    ''' Called to indicate that a new header has been selected.
    The header is provided as a parameter.
    '''
    print 'Selected new header', selectedHeader.headerFile,
    print '(project ' + selectedHeader.projectName + ')'

    # Copy the selected header to drx:/vxroot/headers/current.hdr, so that the
    # housekeeper can find it when we execute its Header() method.
    try:
        cmd = ['ping', '-c', '1', 'drx']
        status = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).wait()
        if status:
            print 'Unable to locate drx in order to change headers'
        else:    
            try:
                src = selectedHeader.headerFile
                dest = 'drx:/vxroot/headers/current.hdr'
                cmd = ['scp', '-B', src, dest]
                status = subprocess.Popen(cmd, stdout=subprocess.PIPE).wait()
                if (status != 0):
                    print 'Could not scp', selectedHeader.headerFile, 'to', dest
            except OSError, e:
                print 'subprocess.Popen() execution failed for: "' + ' '.join(cmd) + '"'
    except OSError, e:
        print 'subprocess.Popen() execution failed for: "' + ' '.join(cmd) + '"'
            
    # hskprpc.server.Header() generates an unsigned POSIX CRC-32 checksum 
    # for the header file, but can only return a signed value.  Adjust if 
    # necessary to interpret as unsigned.  Zero is returned on error.
    try:
        r = hskprpc.server.Header()
        if (r < 0):
            r = (1 << 32) + r
        if (r != selectedHeader.checksum):
            print('Bad checksum from housekeeper for ' + src + ': ', r, '!=', 
                  selectedHeader.checksum)
    except Exception, e:
        print 'Exception ',e, 'while calling housekeeper Header()'
    
####################################################################################
def nextTaskColor():
    global taskColors
    global currentColor
    # define taskColors on the first call
    try:
        taskColors
    except NameError:
        taskColors = ['red', 'blue', 'green', 'darksalmon', 'purple', 'mediumslateblue',
                      'firebrick', 'darkviolet','darkturquoise']
        currentColor = 0
    
    if currentColor < len(taskColors)-1:
        currentColor = currentColor+1
    else:
        currentColor = 0

    return taskColors[currentColor]
        
####################################################################################
def lastWindowClosed():
    # Terminate processes by sending them SIGTERM. Otherwise
    # it appears that they are killed with SIGKILL when PyQt
    # exists. In the case of eldoradrx, it's use of the 
    # rr314 Jungo driver can cause Linux to crash if eldoradrx
    # is not able to run it's signal cleaup code.
    for key in ourProcesses.keys():
        ourProcesses[key].terminate()
        print 'waiting for ' + key + ' to finish...'
        terminated = ourProcesses[key].waitForFinished(2000)
        if terminated:
            print '...done'
        else:
            print 'could not terminate, trying a kill...'
            ourProcesses[key].kill()
            killed = ourProcesses[key].waitForFinished(2000)
            if killed:
                print '...done'
            else:
                print '...kill did not work either'
            

####################################################################################
#
# This is where it all happens
#

# make sure that LD_LIBRARY_PATH is available (kde sometimes strips it)
fixLdLibraryPath()

# save a list of our processes. 
ourProcesses = {}

# initialialize path variables and our configuration
initConfig()

# create the RPC servers for all applications
createRpcServers()

# create the qt application               
app = QApplication(sys.argv)

# instantiate an Eldora controller gui
main = EldoraMain(headerDirs, appDict['dumpheader'])

main.show()

# connect signals from main back to us
QObject.connect(main, SIGNAL('start'),              start)
QObject.connect(main, SIGNAL('stop'),               stop)
QObject.connect(main, SIGNAL('status'),             status)
QObject.connect(main, SIGNAL('ready'),              mainIsReady)
QObject.connect(main, SIGNAL('ppi'),                ppi)
QObject.connect(main, SIGNAL('scope'),              scope)
QObject.connect(main, SIGNAL('header'),             headerSelected)
QObject.connect(app,  SIGNAL('lastWindowClosed()'), lastWindowClosed)

# start the event loop
app.exec_()

