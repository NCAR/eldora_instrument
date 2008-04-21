#!/usr/bin/python

import sys
import time
import os

from PyQt4.QtCore   import *
from PyQt4.QtGui    import *

from EldoraMain     import *
from EldoraRPC      import *
from QtConfig       import *
from EmitterProc    import *

####################################################################################

# Global variiables

global eldoraDir    # used to find eldora application binaries
global ddsRoot      # used to locate DCPSInforepo
global ddsConfigDir # location of the DDS configuration files (for DDS apps)
global ourConfig    # the EldoraGui.ini configuration
global drxrpc       # RPC server for eldoradrx
global hskprpc      # RPC server for the housekeeper
global prodrpc      # RPC server for the products generator
global ourProcesses # Process that we have started and are managing

####################################################################################
def restart():
    try:
        # send a stop and a start comand, although these may not
        # not be used in the final implementation
        r = drxrpc.radarStop()
        main.statusLabel.setText(QString(r))
        r = drxrpc.radarStart()
        main.statusLabel.setText(QString(r))
    except Exception, e:
        print "Error trying to contact ", drxrpc, e
        
    # stop the eldora apps
    stopEldoraApps()
    
    # start the eldora apps
    startEldoraApps()
    
####################################################################################
def stop():
    try:
        # send a stop comand, although this may not
        # not be used in the final implementation
        r = drxrpc.radarStop()
        main.statusLabel.setText(QString(r))
    except Exception, e:
        print "Error trying to contact ", drxrpc, e
        
    # stop the eldora apps
    stopEldoraApps()
    
####################################################################################
def status():
    ''' query the eldora applications for status, and transmit this
    status back to the main application.
    '''
    global drxrpc
    global prodrpc

    try:
        r = prodrpc.status()
        print r
    except Exception, e:
        print "Error trying to contact ", prodrpc.appName, '(', prodrpc.URI, '): ', e
        
    try:
       r = drxrpc.status()
       keys = sorted(r.keys())
       i = 0
       rates = []
       for k in keys:
          # convert rate from MB/s to KB/s and save
          rates.append(r[k]*1000.0)
          i = i + 1
    except Exception, e:
        print "Error trying to contact ", drxrpc.appName, '(', drxrpc.URI, '): ', e
        rates = []
        for i in range(16):
            rates.append(0)
    #
    # set the for and aft bw dials
    forRate = 0.0
    aftRate = 0.0
    for i in range(16):
      if i < 8:
          forRate = forRate + rates[i]
      else:
          aftRate = aftRate + rates[i]
    main.forwardBWdial.setValue(forRate)
    main.aftBWdial.setValue(aftRate)
    # set the for and aft pulse progress bars
    main.forwardPulsesProgress.setValue(forRate)
    main.aftPulsesProgress.setValue(aftRate)
    # set the individual channel dials
    forwardDialsList = main.forwardDials.children()
    aftDialsList = main.aftDials.children()
    # remove the first child, who will be the layout manager
    forwardDialsList = forwardDialsList[1:]
    aftDialsList = aftDialsList[1:]
    for i in range(16):
          if i < 8:
              forwardDialsList[i].setValue(rates[i])
          else:
              aftDialsList[i-8].setValue(rates[i])
              
####################################################################################
def startEldoraApps():
    ''' Run eldora applications which have been selected in the configuration.
    '''
    # drx
    runDrx()
    
    # products
    runProducts()
    
    # start the drx rpc server
    drxrpc.start()
    
    # start the products rpc server
    prodrpc.start()
        
####################################################################################
def stopEldoraApps():
    ''' Stop the standard Eldora processing apps
    '''
    
    pkill('eldoraprod')
    pkill ('eldoradrx')
    # at the moment, kill eldoraprod twice, becasue for some reason it needs this
    pkill ('eldoraprod')
    
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

    runDcps = ourConfig.getBool('Dcps/RunDcps', True)
    if  not runDcps:
        return
    
    restart = ourConfig.getBool('Dcps/AutoRestartDcps', False)
    # see if it is already running
    isRunning = not pgrep('DCPSInfoRepo')
    if isRunning:
        if not restart:
            return
        else:
            pkill('DCPSInfoRepo')
            
    # start a new instance
    dcpscmd = [
        ddsRoot + '/bin/DCPSInfoRepo',
        '-NOBITS',
        '-DCPSConfigFile', ddsConfigDir + '/DCPSInfoRepo.ini' ,
        '-ORBSvcConf', ddsConfigDir + '/ORBSvc.conf',
        '-ORBListenEndpoints iiop://dcpsrepo:50000',
        '-d', ddsConfigDir + '/DDSDomainIds.conf'
        ]
    ourProcesses['DCPSInfoRepo'] = EmitterProc(dcpscmd, emitText=False, textColor='purple')
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

    doDrx = ourConfig.getBool('Drx/RunDrx', True)
    if not doDrx:
        return
    
    # see if it is already running
    isRunning = not pgrep('eldoradrx')
    if isRunning:
        return
    
    # start a new instance
    drxcmd = [
           eldoraDir + '/eldoradrx/eldoradrx',
           '--ORB', ddsConfigDir + '/ORBSvc.conf',
           '--DCPS', ddsConfigDir + '/DDSClient.ini',
           '--start0',
           '--start1',
           '--pub',
           ]
    drxSimMode = ourConfig.getBool('Drx/DrxSimMode', False)
    if drxSimMode:
        drxcmd.append('--sim')
    drxInternalTimer = ourConfig.getBool('Drx/DrxInternalTimer', False)
    if drxInternalTimer:
        drxcmd.append('--int')
    ourProcesses['eldoradrx'] = EmitterProc(command=drxcmd, emitText=True, textColor='blue')
    s = ourProcesses['eldoradrx']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.start()
    time.sleep(1)
    
####################################################################################
def runProducts():
    '''
    Run the products generator, if called for by the configuration. 
    
    The configuration is checked to see if Products/RunProducts is true.
    '''
    global ourConfig
    global ourProcesses
    
    doProducts = ourConfig.getBool('Products/RunProducts', True)
    if  not doProducts:
        return
    
    # see if it is already running
    isRunning = not pgrep('eldoraprod')
    if isRunning:
        return
    
    # start a new instance
    productscmd = [eldoraDir + '/eldoraprod/eldoraprod',]
    ourProcesses['eldoraprod'] = EmitterProc(productscmd, emitText=True, textColor='red')
    s = ourProcesses['eldoraprod']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.start()
    
####################################################################################
def pkill(name):
    ''' Execute pkill <name> via QProcess. Return
    the pkill exit status
    '''
    p = QProcess()
    p.start('/usr/bin/pkill', [name,])
    p.waitForFinished(-1)
    return p.exitCode()

####################################################################################
def pgrep(name):
    ''' Execute pgrep <name> via QProcess. Return
    the pgrep exit status.
    '''
    p = QProcess()
    p.start('/usr/bin/pgrep', [name,])
    p.waitForFinished(-1)
    return p.exitCode()

####################################################################################
def startUs():
    '''
    Call this to start our apps and perform other activities
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
    
    # where do the apps live?
    global eldoraDir
    eldoraDir = os.environ['ELDORADIR']
    
    # where is DCPSInfoRepo
    global ddsRoot
    ddsRoot = os.environ['DDS_ROOT']
    
    # where are the DDS configuration files?
    global ddsConfigDir
    ddsConfigDir = eldoraDir + '/conf'

####################################################################################
def createRpcServers():
    global ourConfig
    
    # create the rpc for the drx
    drxrpchost = ourConfig.getString('Drx/DrxHost', 'drx')
    drxrpcport = ourConfig.getInt('Drx/DrxRpcPort', 60000)
    drxrpcurl = 'http://' + drxrpchost + ':' + str(drxrpcport)
    global drxrpc
    drxrpc = EldoraRPC('drx', drxrpcurl)
    
    # create the rpc for the housekeeper
    hskprpchost = ourConfig.getString('Hksp/HousekeeperHost', 'hskp')
    hskprpcport = ourConfig.getInt('Hksp/HousekeeperRpcPort', 60001)
    hskprpcurl = 'http://' + hskprpchost + ':' + str(hskprpcport)
    global hskprpc
    hskprpc = EldoraRPC('hskp', hskprpcurl)
    
    # create the rpc for the products generator
    prodrpchost = ourConfig.getString('Products/ProductsHost', 'archiver')
    prodrpcport = ourConfig.getInt('Products/ProductsRpcPort', 60002)
    prodrpcurl = 'http://' + prodrpchost + ':' + str(prodrpcport)
    global prodrpc
    prodrpc = EldoraRPC('products', prodrpcurl)

    
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

# instantiate an Edora controller
main = EldoraMain(restartFunction=restart, stopFunction=stop, statusFunction=status, startUp=startUs)
main.show()

# start the event loop
app.exec_()


