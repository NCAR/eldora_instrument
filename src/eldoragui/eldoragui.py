#!/usr/bin/python

import sys
import time
import os
import subprocess

from PyQt4.QtCore    import *
from PyQt4.QtGui     import *

if not os.environ.has_key('ELDORADIR'):
    # If the ELDORADIR environment variable is not set, set it to the parent 
    # directory of where this script was found.
    mypath = os.path.dirname(os.path.abspath(sys.argv[0]))
    parentpath = os.path.abspath(os.path.join(mypath, '..'))
    print 'ELDORADIR is not set; using', parentpath
    os.environ['ELDORADIR'] = parentpath

sys.path.append(os.path.join(os.environ['ELDORADIR'],'lib','python'))

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
global Verbose
Verbose=False
if len(sys.argv) > 1 and sys.argv[1] == '-v':
   Verbose = True

global ourConfig    # the EldoraGui.ini configuration

global eldoraDir    # used to find eldora application binaries
global ddsRoot      # used to locate DCPSInforepo
global ddsConfigDir # location of the DDS configuration files (for DDS apps)
global headerDirs   # where the headers are stored
global drxSimHskpMode   # are we generating simulated housekeeping?
drxSimHskpMode = False

global drxrpc       # RPC server for eldoradrx
global hskprpc      # RPC server for the housekeeper
global prodrpc      # RPC server for the products generator

global ourProcesses # Processes that we have started and are managing


####################################################################################
def start():
    ''' start the radar running:
    1. Send the current header to the housekeepr and eldoradrx
    2. Send an RPC start() to eldoradrx
    3. Send an RPC start() to the housekeepr
    '''
    # send out the header to interested clients
    sendHeader()
    
    # send start command to eldoradrx
    try:
        r = drxrpc.server.start()
        main.logText(str(r))
    except Exception, e:
        print ("Error contacting drx RPC (%s) for start : %s"% (drxrpc.URI, str(e)) )

    # Send start command to the housekeeper
    try:
        r = hskprpc.server.Start()
        if (r != 0):
                main.logText(str(r))
    except Exception, e:
        print ("Error contacting housekeeper RPC (%s) for start: %s"% (hskprpc.URI,str(e)))
        
    # stop  (i.e. kill) products
    stopProducts()
    
    # run products
    startProducts()

####################################################################################
def stop():
    try:
        r = drxrpc.server.stop()
        main.logText(str(r))
    except Exception, e:
        print("Error contacting drx RPC for stop:"+str(e))
        pass
    
    try:
        r = hskprpc.server.Stop()
        if (r != 0):
            main.logText(str(r))
    except Exception, e:
        print("Error contacting housekeeper RPC for stop:"+str(e))
        
    # stop the products
    stopProducts()
    
####################################################################################
def status():
    ''' Query the eldora applications for status, and transmit this
    information back to the main display.
    '''
    global drxrpc
    global hskprpc
    global prodrpc
    global drxSimHskpMode

    # determine the product status and product rates
    productRate = 0
    productStatus = 2
    ABPrate = 0
    # only try to get products status if we have run it, since
    # it will always be under our control
    if 'eldoraprod' in ourProcesses.keys():
        try:
            r = prodrpc.server.status()
            ABPrate = r['numAbpBeams']
            productRate = r['numProductBeams']
            numDiscards = r['discardBeamsAft'] + r['discardBeamsFor']
            productStatus = 0
            if numDiscards > 0 or ABPrate < 2000 or productRate < 400:
                productStatus = 1
            if numDiscards > 4 or ABPrate < 1000 or productRate < 300:
                productStatus = 2
        except Exception, e:
            print ("Error contacting products RPC for status:"+ str(e))
    
    # determine the DRX status and rates.
    try:
       r = drxrpc.server.status()
       keys = sorted(r.keys())
       rates = []
       for k in keys:
          # convert rate from MB/s to KB/s and save
          rates.append(r[k]*1000.0)
          
    except Exception, e:
        print ("Error contacting drx RPC (%s) for status: %s"%(drxrpc.URI,str(e)))
        rates = []
        for i in range(16):
            rates.append(0)
    
    # get status from the housekeeper if we aren't running in fake housekeeping
    # mode        
    if drxSimHskpMode:
        hskpStatus = 0
        hskpForRate = 1.0
        hskpAftRate = 1.0    
    else:
        try:
            r = hskprpc.server.status()
            hskpForRate = r['foreRate']
            hskpAftRate = r['aftRate']
            if hskpForRate > 0:
                hskpStatus = 0
            else:
                hskpStatus = 2
        except Exception, e:
            print("Error contacting housekeeper RPC (%s) for status: %s" %
     (hskprpc.URI, str(e)))
	    hskpForRate = 0.0  # set default value, so Python doesn't whine 
	    hskpAftRate = 0.0    
            hskpStatus = 2
            
    main.showStatus(ABPrate=ABPrate, 
                    productRate=productRate, 
                    productStatus=productStatus, 
                    hskpForRate=hskpForRate,
                    hskpAftRate=hskpAftRate,
                    hskpStatus=hskpStatus,
                    rates=rates)
            
####################################################################################
def stopProducts():
    ''' Stop the standard Eldora processing apps. Try first with a SIGTERM. If
    that fails, try a SIGKILL. Remove them from ourProcesses. Note that 
    removing a an entry from ourProcesses removes its reference, and it
    will go out of scope. This causes the QProcess to kill the job.
    '''
    eldoraProcesses = ['eldoraprod',]
    global ourProcesses
    for key in eldoraProcesses:
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
def startDcps():
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

    dcpsConfigPath = ourConfig.getString('Dcps/DcpsConfig', ddsConfigDir+'/DCPSInfoRepo.ini')
    orbConfigPath = ourConfig.getString('Dcps/OrbConfig', ddsConfigDir+'/ORBSvc.conf')
    domainConfigPath = ourConfig.getString('Dcps/DomainConfig', ddsConfigDir+'/DDSDomainIds.conf')
    dcpsURL = ourConfig.getString('Dcps/URL','iiop://archiver:50000')

    if Verbose: print 'start_DCPS - checking if we should run DCPS'
    runDcps = ourConfig.getBool('Dcps/Run', True)
    restart = ourConfig.getBool('Dcps/AutoRestart', False)
    if  not runDcps:
	if Verbose: print 'start_DCPS - we are not configured to start DCPS'
        return
    
    # see if it is already running
    isRunning = not pgrep('DCPSInfoRepo')
    if isRunning:
        if not restart:
	    if Verbose: print 'start_DCPS - DCPS is running'
            return
        else:
	    if Verbose: print 'start_DCPS - killing DCPS '
            pkill('DCPSInfoRepo')
            
    # start a new instance
    if Verbose: print 'start_DCPS - starting new instance'
    dcpscmd = [
        os.path.join(ddsRoot, 'bin', 'DCPSInfoRepo'), 
        '-NOBITS', 
        '-DCPSConfigFile', dcpsConfigPath, 
        '-ORBSvcConf', orbConfigPath, 
        '-ORBListenEndpoints', dcpsURL,
        '-d', domainConfigPath, 
        ]
    ourProcesses['DCPSInfoRepo'] = EmitterProc(dcpscmd, emitText=False,
    payload=nextTaskColor(),verbose=Verbose)
    s = ourProcesses['DCPSInfoRepo']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.startDetached()
    time.sleep(1)

####################################################################################
def startDrx():
    '''
    Run the drx if called for by the configuration. 
    
    The configuration is checked to see if Products/RunDrx is true.
    '''
    global ourConfig
    global ourProcesses
    global drxSimHskpMode

    doDrx = ourConfig.getBool('Drx/Run', True)
    rpcport = ourConfig.getInt('Drx/RpcPort', 60000)
    drxSimHskpMode = ourConfig.getBool('Mode/SimulateHskp', False)
    drxSimRRMode = ourConfig.getBool('Mode/SimulateRR314', False)
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
    drxcmd.append('--rpcport')
    drxcmd.append(str(rpcport))
    
    # see if hskp should be simulated
    if drxSimHskpMode:
        drxcmd.append('--simHskp')
        
    # see if RR314 should be simulated
    if drxSimRRMode:
        drxcmd.append('--simRR314')
        
    # create the process
    s = EmitterProc(command=drxcmd, emitText=True, payload=nextTaskColor(),verbose=Verbose)
    ourProcesses['eldoradrx'] = s
    
    # send text output to the text logger in the main gui
    QObject.connect(s, SIGNAL("text"), main.logText)
    
    # start eldoradrx
    s.start()
    
    # and puase to catch our breath
    time.sleep(1)
    
####################################################################################
def startProducts():
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
    
    # find out if the current header specified single or dual prt
    isDual = dualPrt()
    print 'dualPrt is', isDual
    
    # start a new instance
    productscmd = [appDict['eldoraprod'], ]
    if isDual:
        productscmd.append('--dualprt')
    ourProcesses['eldoraprod'] = EmitterProc(productscmd, emitText=True,
    payload=nextTaskColor(),verbose=Verbose)
    s = ourProcesses['eldoraprod']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.start()
    
####################################################################################
def sendHeader():
    ''' Send the currently selected header to the housekeeper and eldoradrx.
    '''
    
    # Copy the selected header to drx:/vxroot/headers/current.hdr, so that the
    # housekeeper can find it when we execute its Header() method.
    try:
        cmd = ['ping', '-c', '1', 'drx']
        status = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).wait()
        if status:
            main.logText('Unable to locate drx in order to write new header file')
        else:    
            try:
                src = main.selectedHeader.headerFile
                dest = 'drx:/vxroot/headers/current.hdr'
                cmd = ['scp', '-B', src, dest]
                status = subprocess.Popen(cmd, stdout=subprocess.PIPE).wait()
                if (status != 0):
                    main.logText('Could not scp '+main.selectedHeader.headerFile+' to '+dest)
            except OSError, e:
                main.logText('subprocess.Popen() execution failed for: "' + ' '.join(cmd) + '"')
    except OSError, e:
        main.logText('subprocess.Popen() execution failed for: "' + ' '.join(cmd) + '"')
            
    # tell the housekeeper to load a new header
    # hskprpc.server.Header() generates an unsigned POSIX CRC-32 checksum 
    # for the header file, but can only return a signed value.  Adjust if 
    # necessary to interpret as unsigned.  Zero is returned on error.
    try:
        r = hskprpc.server.Header()
        if (r < 0):
            r = (1 << 32) + r
        if (r != main.selectedHeader.checksum):
            main.logText('Bad checksum from housekeeper for ' + src + ': ', r, '!=', 
                  main.selectedHeader.checksum)
    except Exception, e:
        main.logText('Exception '+ str(e) +'while calling housekeeper Header()')
        
    # tell eldoradrx to reconfigure with current radar parameters
    headerToDrx(main.selectedHeader)
    
####################################################################################
def dualPrt():
    ''' check the current header and return true if dual 
     prt is specified, false otherwise'''
    retval = False
    for block in main.selectedHeader:
        if block.type == 'RADD':
            for field in block:
                if field[0] == 'NIPP':
                    if field[2] != '1':
                        retval = True
    return retval
    
def scope():
    ''' Start the scope display
    '''
    cmd = [appDict['eldorascope'],]
    ourProcesses['eldorascope'] = EmitterProc(cmd, 
                                              emitText=True, 
                                              payload=nextTaskColor(),verbose=Verbose)
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
                                       payload=nextTaskColor(),verbose=Verbose)
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
    if Verbose: print 'mainIsReady called'
    # start up DCPS
    startDcps()
    
    # start drx
    startDrx()
    
    # stop any running products
    stopProducts()

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
            ldLibraryPath = ldLibraryPath + os.getenv('ELDORADIR') + '/lib' + ':'
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
    except KeyError :
 
        # If the environment variable is not set, use the parent directory
        # of the location of this script
        mydir, myname = os.path.split(sys.argv[0])
        eldoraDir = os.path.abspath(os.path.join(mydir, '..'))
        os.environ['ELDORADIR'] = eldoraDir
	print 'eldoraDir = ', eldoraDir
    # where is DCPSInfoRepo
    global ddsRoot
    ddsRoot = os.environ['DDS_ROOT']
    
    # where are the DDS configuration files?
    global ddsConfigDir
    ddsConfigDir = os.path.join(eldoraDir, 'conf')
    
   # where are the Eldora header files?
    global headerDirs
    headerDirs = ourConfig.getString('Headers/HeaderDir', 
                                     os.path.join(eldoraDir, 'conf'))

    # build a special dictionary of apps which returns a specific path for
    # an app, if set, otherwise just the app name
    global appDict
    appDict = ApplicationDict()

    # add apps that are found in eldoraDir/<appName>/<appName>
    for app in ['eldoradrx', 'eldoraprod', 'eldorappi', 'eldorascope',
	'dumpheader']:
        appDict[app] = os.path.join(eldoraDir, 'bin', app)
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
    if Verbose: print 'drxrpcurl = ', drxrpcurl
    drxrpc = EldoraRPC('drx', drxrpcurl)
    drxrpc.start()
    
    # create the rpc for the housekeeper
    hskprpchost = ourConfig.getString('Hksp/Host', 'hskp')
    hskprpcport = ourConfig.getInt('Hksp/RpcPort', 60001)
    hskprpcurl = 'http://' + hskprpchost + ':' + str(hskprpcport)
    global hskprpc
    if Verbose: print 'hskprpcurl = ', hskprpcurl
    hskprpc = EldoraRPC('hskp', hskprpcurl)
    hskprpc.start()
    
    # create the rpc for the products generator
    prodrpchost = ourConfig.getString('Products/Host', 'archiver')
    prodrpcport = ourConfig.getInt('Products/RpcPort', 60002)
    prodrpcurl = 'http://' + prodrpchost + ':' + str(prodrpcport)
    global prodrpc
    if Verbose: print 'prodrpcurl =',prodrpcurl
    prodrpc = EldoraRPC('products', prodrpcurl)
    prodrpc.start()
    if Verbose: print 'CreateRpcServers finished'

####################################################################################
def headerSelected(selectedHeader):
    ''' Called to indicate that a new header has been selected.
    The header is provided as a parameter.
    '''
    main.logText('Selected new header ' + selectedHeader.headerFile + ' (project ' + selectedHeader.projectName + ')')

####################################################################################
def headerToDrx(header):
    ''' Send header values to eldoradrx. The values will
be sent to the param() RPC function in eldoradrx. 

They will arrive as an array of two element tuples. The first
elemnt in the tuple is the key. The second element is an array
of associated values. Most keys only have one element in the value 
array, but some, such as FRIBRXGAIN, will have multiple items

The order they are sent to, and received by eldoradrx is
important, in order to identify the FORE or AFT radar that succeeding values
belong to. Thus the RADD block must appear before associated PARM blocks, etc.
'''

    # these are the fields that we will capture from the 
    # header. All others will br ignored.
    fieldtypes = set( [
        'WAVENCHIPS',
        'WAVECHPOFF',
        'WAVECHPWID',
        'WAVENGATES',
        'WAVEGATE1',
        'WAVEGATE2',
        'WAVEGATE3',
        'WAVEGATE4',
        'WAVEGATE5',
        'WAVEMSREP',
        'WAVESEQREP',
        'RADDNAME',
        'RADDCONST',
        'RADDNOIPOW',
        'RADDUNVEL',
        'RADDUNRNG',
        'RADDNFREQ',
        'RADDNIPP',
        'RADDFREQ1',
        'RADDFREQ2',
        'RADDFREQ3',
        'RADDFREQ4',
        'RADDFREQ5',
        'RADDIPP1',
        'RADDIPP2',
        'RADDIPP3',
        'RADDIPP4',
        'RADDIPP5',
        'FRIBNAME',
        'FRIBRXGAIN',
        'FRIBLNALOS',
        'FRIBXGAIN',
        'FRIBTSGAT',
        'FRIBFRQGA',
        'CSPDWIDTH',
        'PARMNAME',
        'PARMBIAS',
        'PARMSCALE'
        ])

    # this rpc parameter list will be an array rather than 
    # a dictionary, since we can have entries with the same name.
    params = []
    # collect all keys and associated values of interest in the header.
    for block in header:
        fields = {}
        for field in block:
            key = block.type+field[0]
            if key in fieldtypes:
                p = [key,]
                p.append(field[2].split())
                params.append(p)
    # send them to eldoradrx
    drxrpc.server.params(params)
    
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
if Verbose: print 'finished creating QApplication'

# instantiate an Eldora controller gui
main = EldoraMain(headerDirs, appDict['dumpheader'])
if Verbose: print 'finished instantiating EldoraMain'

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
if Verbose: print 'starting the event loop'
app.exec_()

