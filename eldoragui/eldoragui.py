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
        print "Error trying to contact ", drxrpc, '(', drxrpc.URI, '): ', e
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
    runDcps = config.getBool('Dcps/RunDcps', True)
    if  not runDcps:
        return
    
    restart = config.getBool('Dcps/AutoRestartDcps', False)
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
        '-DCPSConfigFile', conf + '/DCPSInfoRepo.ini' ,
        '-ORBSvcConf', conf + '/ORBSvc.conf',
        '-ORBListenEndpoints iiop://dcpsrepo:50000',
        '-d', conf + '/DDSDomainIds.conf'
        ]
    ourThreads['DCPSInfoRepo'] = EmitterProc(dcpscmd, emitText=False, textColor='purple')
    s = ourThreads['DCPSInfoRepo']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.startDetached()
    time.sleep(1)

####################################################################################
def runDrx():
    '''
    Run the drx if called for by the configuration. 
    
    The configuration is hecked to see if Products/RunDrx is true.
    '''
    doDrx = config.getBool('Drx/RunDrx', True)
    if not doDrx:
        return
    
    # see if it is already running
    isRunning = not pgrep('eldoradrx')
    if isRunning:
        return
    
    # start a new instance
    drxcmd = [
           eldoraDir + '/eldoradrx/eldoradrx',
           '--ORB', conf + '/ORBSvc.conf',
           '--DCPS', conf + '/DDSClient.ini',
           '--start0',
           '--start1',
           '--pub',
           ]
    drxSimMode = config.getBool('Drx/DrxSimMode', False)
    if drxSimMode:
        drxcmd.append('--sim')
    drxInternalTimer = config.getBool('Drx/DrxInternalTimer', False)
    if drxInternalTimer:
        drxcmd.append('--int')
    ourThreads['eldoradrx'] = EmitterProc(command=drxcmd, emitText=True, textColor='blue')
    s = ourThreads['eldoradrx']
    QObject.connect(s, SIGNAL("text"), main.logText)
    s.start()
    time.sleep(1)
    
####################################################################################
def runProducts():
    '''
    Run the products generator, if called for by the configuration. 
    
    The configuration is checked to see if Products/RunProducts is true.
    '''
    doProducts = config.getBool('Products/RunProducts', True)
    if  not doProducts:
        return
    
    # see if it is already running
    isRunning = not pgrep('eldoraprod')
    if isRunning:
        return
    
    # start a new instance
    productscmd = [eldoraDir + '/eldoraprod/eldoraprod',]
    ourThreads['eldoraprod'] = EmitterProc(productscmd, emitText=True, textColor='red')
    s = ourThreads['eldoraprod']
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
#
# This is where it all happens
#

# make sure that LD_LIBRARY_PATH is available (kde sometimes strips it)
fixLdLibraryPath()

# get our configuration
config = QtConfig('NCAR', 'EldoraGui')

# where do the apps live?
eldoraDir = os.environ['ELDORADIR']
ddsRoot = os.environ['DDS_ROOT']

# where are the DDS configuration files?
conf = eldoraDir + '/conf'

# create the rpc for the drx
drxrpchost = config.getString('Drx/DrxRpcHost', 'drx')
drxrpcport = config.getInt('Drx/DrxRpcPort', 60000)
drxrpcurl = 'http://' + drxrpchost + ':' + str(drxrpcport)
drxrpc = EldoraRPC(drxrpcurl)

# create the rpc for the housekeeper
hskprpchost = config.getString('Hksp/HousekeeperRpcHost', 'hskp')
hskprpcport = config.getInt('Hksp/HousekeeperRpcPort', 60001)
hskprpcurl = 'http://' + hskprpchost + ':' + str(hskprpcport)
hskprpc = EldoraRPC(hskprpcurl)


# save a list of our threads. Logically, we need to keep a
# global reference to the threads, so that they don't get deleted
# the functions that create them go out of scope.
ourThreads = {}

# create the qt application               
app = QApplication(sys.argv)

# instantiate an Edora controller
main = EldoraMain(restartFunction=restart, stopFunction=stop, statusFunction=status, startUp=startUs)
main.show()

# start the event loop
app.exec_()


