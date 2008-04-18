#!/usr/bin/python

import sys
import time
import os

from PyQt4.QtCore import *
from PyQt4.QtGui  import *

from EldoraMain   import *
from EldoraRPC    import *
from QtConfig     import *
from SubThread    import *

#-----------------------------------------------------------
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
    
#-----------------------------------------------------------
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
    
#-----------------------------------------------------------
def status():
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
              
#-----------------------------------------------------------
def startEldoraApps():
    '''
    Run eldora applications which have been selected in the configuration.
    If they are already running, they will be restarted only if restart is true.
    '''
    # drx
    runDrx()
    
    # products
    runProducts()
        
#-----------------------------------------------------------
def stopEldoraApps():
    pkill('eldoraprod')
    pkill ('eldoradrx')
    # at the moment, kill eldoraprod twice, becasue for some reason it needs this
    pkill ('eldoraprod')
    
#-----------------------------------------------------------
def runDcps():
    '''
    Run the DCPSInfoRepo, but only if the configuration has
    called for that.
    '''
    runDcps = config.getBool('Dcps/RunDcps', true)
    if  not runDcps:
        return
    
    restart = config.getBool('Dcps/AutoRestartDcps', false)
    # see if it is already running
    isRunning = not subprocess.call(['/usr/bin/pgrep', 'DCPSInfoRepo'])
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
    #spawn(dcpscmd)
    s = SubThread(dcpscmd)
    s.start()
    time.sleep(1)

#-----------------------------------------------------------

def callback(line):
    print 'line: ', line
    
def runDrx():
    '''
    Run the drx if called for by the configuration. 
    '''
    doDrx = config.getBool('Drx/RunDrx', true)
    if not doDrx:
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
    #spawn(drxcmd)
    s = SubThread(drxcmd, callback=callback)
    s.start()
    time.sleep(1)
    
#-----------------------------------------------------------
def runProducts():
    '''
    Run the products generator if called for by the configuration. 
    '''
    doProducts = config.getBool('Products/RunProducts', True)
    if  not doProducts:
        return
    
    # start a new instance
    productscmd = [eldoraDir + '/eldoraprod/eldoraprod',]
    #spawn(productscmd)
    s = SubThread(productscmd, callback)
    s.start()
    
#-----------------------------------------------------------
def pkill(name):
    pkill = '/usr/bin/pkill'
    pkillcmd =  [pkill, name]
    print pkillcmd
    subprocess.Popen(pkillcmd)
    
#-----------------------------------------------------------
def spawn(cmd, sleepSecs=1):
    print  cmd
    pid = subprocess.Popen(cmd)

############################################################
#
# This is where it all happens
#

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

# start up DCPS
runDcps()

# start the eldora applications
startEldoraApps()

# create the qt application               
app = QApplication(sys.argv)

# instantiate an Edora controller
main = EldoraMain(restartFunction=restart, stopFunction=stop, statusFunction=status)
main.show()

# start the event loop
app.exec_()


