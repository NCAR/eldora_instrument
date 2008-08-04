#!/usr/bin/python

import sys
import time
import os
import subprocess
import signal
import serial

# Determine ELDORADIR
if not os.environ.has_key('ELDORADIR'):
    # If the ELDORADIR environment variable is not set, set it to the parent 
    # directory of where this script was found.
    mypath = os.path.dirname(os.path.abspath(sys.argv[0]))
    parentpath = os.path.abspath(os.path.join(mypath, '..'))
    print 'ELDORADIR is not set; using', parentpath
    os.environ['ELDORADIR'] = parentpath
sys.path.append(os.path.join(os.environ['ELDORADIR'],'lib','python'))


from PyQt4.QtCore    import *
from PyQt4.QtGui     import *

from EldoraMain      import *
from EldoraRPC       import *
from QtConfig        import *
from EmitterProc     import *
from HpaWidget       import *
from ECB             import *

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
global main             # The main gui window, from EldoraMain.ui
global dds              # the dds controller - dict('forward', 'aft')
global sa               # the steped attenuator controller, one for both fore and aft
global mux              # The mux programmer
global testPulse        # test pulse periodic controller
global ourConfig        # the EldoraGui.ini configuration
global eldoraDir        # used to find eldora application binaries
global ddsRoot          # used to locate DCPSInforepo
global ddsConfigDir     # location of the DDS configuration files (for DDS apps)
global headerDirs       # where the headers are stored
global drxSimHskpMode   # are we generating simulated housekeeping?
global ldLibraryPath    # The ldLibraryPath
global drxrpc           # RPC server for eldoradrx
global hskprpc          # RPC server for the housekeeper
global prodrpc          # RPC server for the products generator
global ourProcesses     # Processes that we have started and are managing
global Stopped          # are we shutting down?
global Verbose          # Set for diagnostics from eldoragui itself

####################################################################################
def start():
    ''' start the radar running:
    0: Program the ECB devices and start the test pulse controller
    1. Send the current header to the housekeepr and eldoradrx
    2. Send an RPC start() to eldoradrx
    3. Send an RPC start() to the housekeepr
    '''

    # program the ECB devices
    # disable the ecb, since we are not using the eldora syntesizers anymore
    # progECB()

    # start archivers
    startArchivers()
    
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
    ''' Stop eldora processing
    1. stop the test pulse controller
    2. stop the drx process
    3. kill the products generator
    4. terminate rpc threads
    '''

    global testPulse
    global Verbose

    if Verbose: 
        print("stop() called")

    # stop the test pulse controller
    testPulse.stop()

	# send stop sommand to the drx
    try:
        r = drxrpc.server.stop()
        main.logText(str(r))
    except Exception, e:
        print("Error contacting drx RPC for stop:"+str(e))
        pass
	
	# send stop command to the housekeeper
    try:
        r = hskprpc.server.Stop()
        if (r != 0):
            main.logText(str(r))
    except Exception, e:
        print("Error contacting housekeeper RPC for stop:"+str(e))

	# kill the products generator
    if 'eldoraprod' in ourProcesses.keys():
        # stop the products
        stopProducts()

    # force any RPC threads to exit
    drxrpc.terminate()
    hskprpc.terminate()
    prodrpc.terminate()
    archiver1rpc.terminate()
    archiver2rpc.terminate()

####################################################################################
def status():
    ''' Query the eldora applications for status, and transmit this
    information back to the main display.
    '''
    global drxrpc
    global hskprpc
    global prodrpc
    global archiver1rpc
    global archiver2rpc
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
            print ("Error contacting products RPC for status:" + str(e))

    # determine the DRX status and rates.
    try:
        r = drxrpc.server.status()
        # Get the clock offset in dwells, then remove that entry
        # from the dictionary, leaving just the 16 channel rates
        clockOffsetInDwells = r['clockOffsetInDwells']
        del r['clockOffsetInDwells']
        
        keys = sorted(r.keys())
        drxRates = []
        chanCount = 0
        drxForeRate = 0.0
        drxAftRate = 0.0
        for k in keys:
            # convert rate from MB/s to KB/s and save
            rate = r[k] * 1000.0
            drxRates.append(rate)
            if (chanCount < 8):
                drxForeRate += rate
            else:
                drxAftRate += rate
            chanCount += 1

        # Default to green status
        drxStatus = 0
        
        # If either rate is less than 800 KB/s, or if clock offset is
        # more than 0.2 dwells (or is infinite, which means offset is
        # not determined), go to yellow status
        inf = float('inf')
        if (drxForeRate < 800 or drxAftRate < 800 or clockOffsetInDwells > 0.2):
            drxStatus = 1
            
        if (clockOffsetInDwells == inf):
            drxStatus = 1
            main.logText('DRX clock offset is uncertain.  If NTP was just ' +
                         'started on the drx, wait a few minutes.')
            main.logText('If this persists, you may need to restart NTP on ' +
                         'the drx')
        # If either rate is less than 400 KB/s, or if clock offset is known
        # and is more than 0.5 dwells, go to red status
        if (drxForeRate < 400 or drxAftRate < 400):
            drxStatus = 2
        if (clockOffsetInDwells > 0.5 and clockOffsetInDwells != inf):
            drxStatus = 2
            main.logText('DRX clock is offset by ' + str(clockOffsetInDwells) +
                         ' dwells!')
            main.logText('Make sure the time server is locked, and restart ' +
                         'ntpd on the drx.')

    except Exception, e:
        print ("Error contacting drx RPC (%s) for status: %s"%(drxrpc.URI,str(e)))
        drxStatus = 2
        drxRates = []
        for i in range(16):
            drxRates.append(0)

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
            print("Error contacting housekeeper RPC (%s) for status: %s" % (hskprpc.URI, str(e)))
            hskpForRate = 0.0  # set default value, so Python doesn't whine 
            hskpAftRate = 0.0    
            hskpStatus = 2
            
    # determine the archivers' status and rate.
    archiver1Rate, archiver1Status = getArchiverStatus(archiver1rpc)
    archiver2Rate, archiver2Status = getArchiverStatus(archiver2rpc)

    main.showStatus(ABPrate=ABPrate, 
                    productRate=productRate, 
                    productStatus=productStatus, 
                    hskpForRate=hskpForRate,
                    hskpAftRate=hskpAftRate,
                    hskpStatus=hskpStatus,
                    archiver1Rate=archiver1Rate,
                    archiver1Status=archiver1Status,
                    archiver2Rate=archiver2Rate,
                    archiver2Status=archiver2Status,
                    drxStatus=drxStatus,
                    drxRates=drxRates)

####################################################################################
#Return (rate, status) from the given archiver RPC
def getArchiverStatus(rpc):
    try:
        r = rpc.server.status()
        rate = r['rate']
        # convert the signed checksum back into its unsigned form
        cksum = r['headerChecksum']
        if (cksum < 0):
            cksum = (1 << 32) + cksum
        if (cksum == main.selectedHeader.checksum):
            if (rate > 0.0):
                status = 0
            else:
                status = 1
        else:
            status = 2
            main.logText('Bad checksum from ' + rpc.appName + ' (' + 
                         str(cksum) + ' != ' + 
                         str(main.selectedHeader.checksum) + ')')
    except Exception, e:
        print ('Error contacting %s RPC (%s) for status: %s' %
               (rpc.appName, rpc.URI, str(e)))
        rate = 0.0
        status = 2
        
    return (rate, status)
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
            secs = 6
            if Verbose: print 'waiting %f seconds for %s to terminate' % (secs, key)
            terminated = proc.waitForFinished(secs*1000)
            if not terminated:
                msg = 'Killing ' + key
                main.logText(msg)
                if Verbose: print(msg)
                proc.kill()
                killed = proc.waitForFinished(1500)
                if not killed:
                    msg = 'Could not kill ' + key + ', using kill -9'
                    main.logText(msg)
                    if Verbose: print(msg)
                    pkill(key)
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
    os.chdir('/tmp')  # DCPSInfoRepo wants to write a repo.ior in the current
        # directory, which had better be writable!

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

    # just go ahead and kill DCPS. Old running instances have 
    # turned out to cause a lot of heartburn.
    restart = True
    # see if it is already running
    isRunning = not pgrep('DCPSInfoRepo')
    if isRunning:
        if not restart:
	    if Verbose: print 'startDcps() - DCPS is running'
            return
        else:
	    if Verbose: print 'startDcps() - killing DCPS '
            pkill('DCPSInfoRepo')

    # start a new instance
    if Verbose: print 'startDcps() - starting new instance'
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
    #s.startDetached()
    s.start()
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

    # get the number of channels
    nchan = ourConfig.getInt('Products/Channels', 3)
    
    # find out if the current header specified single or dual prt
    isDual = dualPrt()
    # start a new instance
    productscmd = [appDict['eldoraprod'], ]
    if isDual:
        productscmd.append('--dualprt')
    
    productscmd.append('--nchan')
    productscmd.append(str(nchan))
    
    # create the process
    ourProcesses['eldoraprod'] = EmitterProc(productscmd, emitText=True,
       payload=nextTaskColor(),verbose=Verbose)
    
    # connect the output stream
    s = ourProcesses['eldoraprod']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.start()

####################################################################################
def startArchivers():
    '''
    Run the archiver, if called for by the configuration. 

    The configuration is checked to see if Archiver/Run is true.
    '''
    global ourConfig
    global ourProcesses
    
    doArchiver = ourConfig.getBool('Archiver/Run', True)
    if not doArchiver:
        return

    # archive directories and RPC ports
    dir1 = ourConfig.getString('Archiver/Archiver1Dir', '/data_first')
    dir2 = ourConfig.getString('Archiver/Archiver2Dir', '/data_second')
    port1 = ourConfig.getInt('Archiver/Archiver1RpcPort', 60003)
    port2 = ourConfig.getInt('Archiver/Archiver2RpcPort', 60004)

    # start the two servers if they aren't running
    archiverList = [('eldoraarchiver1', dir1, port1),
                    ('eldoraarchiver2', dir2, port2)]
    for entry in archiverList:
        key, dir, port = entry
        if (ourProcesses.has_key(key) and 
            ourProcesses[key].state() == QProcess.Running):
                main.logText(key + ' is already running (PID ' + 
                             str(ourProcesses[key].pid()) + ')')
                continue
            
        cmd = [appDict['eldoraarchiver'], dir, str(port)]
        ourProcesses[key] = EmitterProc(cmd, 
                                        emitText = True, 
                                        payload = nextTaskColor(),
                                        verbose = Verbose)
        proc = ourProcesses[key]
        QObject.connect(proc, SIGNAL("text"), main.logText)
        proc.start()
        # Give them a moment to actually start
        time.sleep(2)

####################################################################################
def sendHeader():
    ''' Send the currently selected header to the housekeeper, eldoradrx,
    and archivers
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
                    main.logText('Could not scp ' + 
                                 main.selectedHeader.headerFile + ' to ' + dest)
            except OSError, e:
                main.logText('subprocess.Popen() execution failed for: "' + 
                             ' '.join(cmd) + '"')
    except OSError, e:
        main.logText('subprocess.Popen() execution failed for: "' + 
                     ' '.join(cmd) + '"')

    # tell the housekeeper to load a new header
    # hskprpc.server.Header() generates an unsigned POSIX CRC-32 checksum 
    # for the header file, but can only return a signed value.  Adjust if 
    # necessary to interpret as unsigned.  Zero is returned on error.
    try:
        r = hskprpc.server.Header()
        if (r < 0):
            r = (1 << 32) + r
        if (r != main.selectedHeader.checksum):
            main.logText('Bad checksum from housekeeper for ' + src + ': ', r, 
                         '!=', main.selectedHeader.checksum)
    except Exception, e:
        main.logText('Exception ' + str(e) + ' while calling housekeeper Header()')

    # tell archivers to load a new header
    # archiver.server.Header() generates an unsigned POSIX CRC-32 checksum 
    # for the header file, but can only return a signed value.  Adjust if 
    # necessary to interpret as unsigned.  Zero is returned on error.
    for rpc in [archiver1rpc, archiver2rpc]:
        try:
            fltname = ourConfig.getString('Flight/FlightName','UNKNOWN')
            # @todo: Change to use a real flight name
            r = rpc.server.Header([os.path.basename(main.selectedHeader.headerFile),
                                   fltname])
            if (r < 0):
                r = (1 << 32) + r
            if (r != main.selectedHeader.checksum):
                main.logText('Bad checksum from ' + rpc.appName + ' for ' + 
                             src + ':', r, '!=', main.selectedHeader.checksum)
        except Exception, e:
            main.logText('Exception ' + str(e) + ' while calling ' + 
                         rpc.appName + ' Header()')

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
    p.start('/usr/bin/kill -s 9', [name,])
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

    # create the ECB controllers
    createECB()

    # start up DCPS
    startDcps()

    # start drx
    startDrx()

    # stop any running products
    stopProducts()

    # display some internal information on the Internal tab
    showInternal()    

####################################################################################
def fixLdLibraryPath():
    global ldLibraryPath
    ldLibraryPath = os.getenv('LD_LIBRARY_PATH')
    if ldLibraryPath == None:
        # it has been stripped by kde! (or maybe it was never set...)
        try:
            libdirs = []
            libdirs += os.environ['ACE_ROOT'] + '/lib'
            libdirs += os.environ['TAO_ROOT'] + '/lib'
            libdirs += os.environ['DDS_ROOT'] + '/lib'
            libdirs += os.environ['ELDORADIR'] + '/lib'
            ldLibraryPath = ':'.join(libdirs)
            os.environ['LD_LIBRARY_PATH'] = ldLibraryPath
        except Exception, e:
            print 'missing environment variable', e
            sys.exit(1)

    ####################################################################################
def initConfig():
    ''' Create the configuration and set the path variables
    '''
    global main
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

    # where is DCPSInfoRepo
    global ddsRoot
    try:
        ddsRoot = os.environ['DDS_ROOT']
    except KeyError:
        print "The DDS_ROOT environment variable is not set!"
        sys.exit(1)

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
    for app in ['eldoraarchiver', 'eldoradrx', 'eldoraprod', 'eldorappi', 
                'eldorascope', 'dumpheader', 'progdds', 'progsa', 'progmux', 
                'progtestpulse']:
        appDict[app] = os.path.join(eldoraDir, 'bin', app)
    # DcpsInfoRepo location
    appDict['DCPSInfoRepo'] = os.path.join(ddsRoot, "bin", "DCPSInfoRepo")


####################################################################################
def createRpcServers():
    global ourConfig

    # create the rpc for the drx
    host = ourConfig.getString('Drx/Host', 'drx')
    port = ourConfig.getInt('Drx/RpcPort', 60000)
    url = 'http://' + host + ':' + str(port)
    global drxrpc
    if Verbose: print 'drxrpcurl = ', url
    drxrpc = EldoraRPC('drx', url)
    drxrpc.start()

    # create the rpc for the housekeeper
    host = ourConfig.getString('Hksp/Host', 'hskp')
    port = ourConfig.getInt('Hksp/RpcPort', 60001)
    url = 'http://' + host + ':' + str(port)
    global hskprpc
    if Verbose: print 'hskprpcurl = ', url
    hskprpc = EldoraRPC('hskp', url)
    hskprpc.start()

    # create the rpc for the products generator
    host = ourConfig.getString('Products/Host', 'archiver')
    port = ourConfig.getInt('Products/RpcPort', 60002)
    url = 'http://' + host + ':' + str(port)
    global prodrpc
    if Verbose: print 'prodrpcurl =', url
    prodrpc = EldoraRPC('products', url)
    prodrpc.start()
    if Verbose: print 'CreateRpcServers finished'
    
    # create the rpc for the primary archiver
    host = ourConfig.getString('Archiver/Archiver1Host', 'archiver')
    port = ourConfig.getInt('Archiver/Archiver1RpcPort', 60003)
    url = 'http://' + host + ':' + str(port)
    global archiver1rpc
    if Verbose: print 'archiver1url =', url
    archiver1rpc = EldoraRPC('eldoraarchiver1', url)
    archiver1rpc.start()
    
    # create the rpc for the secondary archiver
    host = ourConfig.getString('Archiver/Archiver2Host', 'archiver')
    port = ourConfig.getInt('Archiver/Archiver2RpcPort', 60004)
    url = 'http://' + host + ':' + str(port)
    global archiver2rpc
    if Verbose: print 'archiver2url =', url
    archiver2rpc = EldoraRPC('eldoraarchiver2', url)
    archiver2rpc.start()
    
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

Since there are multiple PARM blocks, the scale and bias values
are associated with the PARM name value to create a unique key, of
the form PARMSCALEDBZ or PARMBIASDBZ. The drx rpc handler is expecting 
this.

The order they are sent to, and received by eldoradrx is
important, in order to identify the FORE or AFT radar that succeeding values
belong to. Thus the RADD block must appear before associated PARM blocks, etc.
'''

    # these are the fields that we will capture from the 
    # header. All others will be ignored.
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
        'RADDROTVEL',
        'FRIBNAME',
        'FRIBRXGAIN',
        'FRIBLNALOS',
        'FRIBXGAIN',
        'FRIBTSGAT',
        'FRIBFRQGA',
        'FRIBENCANG',
        'CSPDWIDTH',
        'CSPD1STGAT',
        'PARMNAME',
        'PARMBIAS',
        'PARMSCALE'
        ])

    # this rpc parameter list will be an array rather than 
    # a dictionary, since we can have entries with the same name.
    params = []
    # capture the parameter name, to be used in building a key
    # for scale and bias
    parmname = ''
    # collect all keys and associated values of interest in the header.
    # translate PARMSCALE and PARMBIAS to keys with the variable name 
    # appended.
    for block in header:
        fields = {}
        for field in block:
            key = block.type+field[0]
            if key in fieldtypes:
                if key == 'PARMNAME':
                    parmname = field[2]
                else:
                    # Append the param name to PARMSCALE and PARMBIAS keys,
                    # which become e.g., PARMSCALEDBZ and PARMBIASDBZ
                    if key == 'PARMSCALE' or key == 'PARMBIAS':
                        key = key + parmname
                    # add the [key, keyvalue] pair to our list
                    params.append([key, field[2]])
    # send them to eldoradrx
    try:
    	drxrpc.server.params(params)
    except Exception, e:
    	print 'Error sending params to drx', '(', drxrpc.URI,')',e

####################################################################################
def testPulseChange(params):
    '''Called when the test pulse has changed. Send the information on to 
    the housekeeper.
    params - need to define what they are, and how the housekeeper wants 
    to receive them.
    '''
    
    # don't send the testpulse information to the housekeeper right now,
    # becasue the housekeeper hangs.
    return
    
    try:
        hskprpc.server.testpulse(params)
    except Exception, e:
        print 'Error sending testpulse params to hskp', '(', hskprpc.URI,')',e
    
####################################################################################
def createECB():
    ''' Create the ECB devices: DDS, stepped attenuator, and trigger mux.
    Also creat the test pulse controller
    '''
    global main
    global dds
    global sa
    global mux
    global Verbose
    global ourConfig
    global testPulse
    
    dds = dict()
    # all devices share the same port number
    port = ourConfig.getInt('ECB/IpPort', 2424)

    # create the dds devices
    ipDdsFor = ourConfig.getString('ECB/IpDdsForward', 'etherio-for')
    ipDdsAft = ourConfig.getString('ECB/IpDdsAft', 'etherio-aft')
    dds['forward'] = DDS(ip=ipDdsFor, port=port, 
                         ddsProgram=appDict['progdds'],
                         radar='forward', textFunction=main.logText,
                         verbose=Verbose)
    dds['aft'] = DDS(ip=ipDdsAft, port=port, 
                     ddsProgram=appDict['progdds'],
                     radar='aft', textFunction=main.logText,
                     verbose=Verbose)

    # create the stepped attenuator device
    ipSaMux = ourConfig.getString('ECB/IpSaMux', 'etherio-sa')
    sa = SA(ip=ipSaMux, port=port, 
            saProgram=appDict['progsa'],
            textFunction=main.logText,
            verbose=Verbose)
    
    # create a multiplexor controller
    mux = Mux(ip=ipSaMux,
			port=port,
			muxProgram=appDict['progmux'],
			textFunction=main.logText,
			verbose=Verbose)
			
    # create the test pulse controller
    # get the attenuation values
    atten = dict()
    atten['forward'] = list()
    atten['forward'].append(ourConfig.getString('ECB/AttenForward0','10'))
    atten['forward'].append(ourConfig.getString('ECB/AttenForward1','30'))
    atten['forward'].append(ourConfig.getString('ECB/AttenForward2','50'))
    atten['forward'].append(ourConfig.getString('ECB/AttenForward3','70'))
    atten['aft'] = list()
    atten['aft'].append(ourConfig.getString('ECB/AttenAft0','20'))
    atten['aft'].append(ourConfig.getString('ECB/AttenAft1','40'))
    atten['aft'].append(ourConfig.getString('ECB/AttenAft2','60'))
    atten['aft'].append(ourConfig.getString('ECB/AttenAft3','80'))
    # get the frequncy offset
    fOffGhz = ourConfig.getDouble('ECB/FreqOffsetGhz', 0.0001)
    
    # create the test pulse controller
    testPulse = TestPulseControl(testpulseapp=appDict['progtestpulse'],
								ipddsfor=ipDdsFor,
								ipddsaft=ipDdsAft,
								ipsamux=ipSaMux,
								port=port, 
								periodSecs=10,
								header=main.selectedHeader,
								atten=atten,
								fOffGhz=fOffGhz,
                                callback=testPulseChange,
								parent=main,
								textFunction=main.logText,
								verbose=Verbose)

####################################################################################	
def progECB():
    global dds
    global sa
    global mux
    global testPulse

    # get the header
    header = main.selectedHeader

    # program the for and aft transmit frequencies
    dds['forward'].programDDS(header)
    dds['aft'].programDDS(header)
    
    # program the transmit multiplexors
    mux.programMux(radar='forward', muxchoice='transmit', channel='P')
    mux.programMux(radar='aft',     muxchoice='transmit', channel='P')
    
    # start the testpulse cycle
    testPulse.start()

    return

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
def flightName(fltname):
    ourConfig.setString('Flight/FlightName',fltname)


####################################################################################
def lastWindowClosed():
    if Verbose: print 'lastWindowClosed - calling stop'
    
    # stop everything
    stop()
    
    if Verbose: print 'lastWindowClosed - sending signals to processes'
    # Terminate processes by sending them SIGTERM. Otherwise
    # it appears that they are killed with SIGKILL when PyQt
    # exits. In the case of eldoradrx, it's use of the 
    # rr314 Jungo driver can cause Linux to crash if eldoradrx
    # is not able to run it's signal cleaup code.
    for key in ourProcesses.keys():
        ourProcesses[key].terminate()
        secs = 10
        print 'waiting %d seconds for %s to finish' % (secs, key)
        terminated = ourProcesses[key].waitForFinished(secs*1000)
        if terminated:
            print '...done'
        else:
            print 'could not terminate, trying a kill...'
            ourProcesses[key].kill()
            killed = ourProcesses[key].waitForFinished(8000)
            if killed:
                print '...done'
            else:
                print '...kill did not work either'
                
    # Be sure to kill DCPS. There may have been a rogue instance left
    # running
    pkill('DCPSInforepo')
    global Stopped
    Stopped = True

def my_ExceptHook(type, value, tb):
    global app
    import traceback,exceptions
    print 'eldoragui.py terminated - cleaning up'
    # if this isn't a keyboard interrupt (^C), better print what it was.
    if not isinstance(value,exceptions.KeyboardInterrupt) :
        print 'eldoragui.py : Exception : ', type, value
        print 'Traceback : ', traceback.print_tb(tb)

    app.quit()

def sig_handler(sig_num, frame):
    global app
    print 'caught signal : ', sig_num
    print 'eldoragui.py terminated - cleaning up'
    app.quit()




####################################################################################
def showInternal():
    ''' Display some diagnostic eldoragui configuration information
    on the Internal tab of the gui.
    '''
    main.internalText('eldoraDir: ' + eldoraDir)
    main.internalText('ddsRoot: '+ ddsRoot)
    main.internalText('ddsConfigDir: ' + ddsConfigDir)
    main.internalText('headerDirs: ' + headerDirs)
    for k in appDict.dict.keys():
        main.internalText('appDict['+ k  + ']: '+ appDict[k])
    main.internalText('ldLibraryPath: ' + ldLibraryPath)
    # show the processes that we currently have. Of course processes
    # run later won't be available right now 
    for k in ourProcesses:
        text = k + ' process: '
        # The command is acutally a list
        for a in ourProcesses[k].command:
            text = text + ' ' + a
        main.internalText(text)

####################################################################################
#
# This is where it all happens
#
Stopped = False

Verbose = False
# set the Verbose flag
if len(sys.argv) > 1 and sys.argv[1] == '-v':
    Verbose = True

# default the simulated housekeeping to false
drxSimHskpMode = False

# make sure that LD_LIBRARY_PATH is available (kde sometimes strips it)
fixLdLibraryPath()

# save a list of our processes. 
ourProcesses = {}

# initialialize path variables and our configuration
initConfig()

# create the RPC servers for all applications
createRpcServers()

# create the qt application               
global app
app = QApplication(sys.argv)
if Verbose: print 'finished creating QApplication'

# instantiate an Eldora controller gui

# get device names for the HPA controller
HPA0device = ourConfig.getString('HPAs/Hpa0Device', '/dev/ttydp01')
HPA1device = ourConfig.getString('HPAs/Hpa1Device', '/dev/ttydp02')

fltname = ourConfig.getString('Flight/FlightName','UNKNOWN')

main = EldoraMain(flightName=fltname,
                  headersDir=headerDirs, 
                  hdrDumpApp=appDict['dumpheader'],
                  hpa0Device=HPA0device,
                  hpa1Device=HPA1device)
main.show()
if Verbose: print 'finished instantiating EldoraMain'

# connect signals from main back to us
QObject.connect(main, SIGNAL('start'),              start)
QObject.connect(main, SIGNAL('stop'),               stop)
QObject.connect(main, SIGNAL('status'),             status)
QObject.connect(main, SIGNAL('ready'),              mainIsReady)
QObject.connect(main, SIGNAL('ppi'),                ppi)
QObject.connect(main, SIGNAL('scope'),              scope)
QObject.connect(main, SIGNAL('header'),             headerSelected)
QObject.connect(main, SIGNAL('flightname'),         flightName)
QObject.connect(app,  SIGNAL('lastWindowClosed()'), lastWindowClosed)

# start the event loop
if Verbose: print 'starting the event loop'
# install our exception handler after all initialization is complete
sys.excepthook = my_ExceptHook
signal.signal(signal.SIGHUP, sig_handler)
signal.signal(signal.SIGTERM, sig_handler)
signal.signal(signal.SIGINT, sig_handler)
app.exec_()
if Verbose: print 'exited the event loop'
if not Stopped:  # if we haven't already cleaned up, do it now
    if Verbose: print 'calling lastWindowClosed '
    lastWindowClosed()

if Verbose: print 'exiting now'
