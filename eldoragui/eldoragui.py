#!/usr/bin/python

import sys
import time
import os

from PyQt4.QtCore import *
from PyQt4.QtGui  import *

from EldoraMain   import *
from EldoraRPC    import *
from QtConfig     import *

def runStop(runswitch):
    try:
        if (runswitch):
            r = drxrpc.radarStart()
            main.statusLabel.setText(QString(r))
        else:
            r = drxrpc.radarStop()
            main.statusLabel.setText(QString(r))
    except Exception, e:
        print "Error trying to contact ", drxrpc, e
    if runswitch:
        startEldoraApps()
    else:
        stopEldoraApps()

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
              
def startEldoraApps():
    '''
    Run eldora applications which have been selected in the configuration.
    But first, stop them if they are already running.
    '''
    # restart DCPS, if configured for this
    restartDCPS()
    # stop currently running apps
    stopEldoraApps()
    # drx
    runDrx = config.getBool('Drx/RunDrx', true)
    if runDrx:
        drxcmd = [
               eldoraDir + '/eldoradrx/eldoradrx',
               '--ORB', conf + '/ORBSvc.conf',
               '--DCPS', conf + '/DDSClient.ini',
               '--start0',
               '--start1',
               '--pub',
               ]
        drxSimMode = config.getBool('Drx/DrxSimMode', false)
        if drxSimMode:
            drxcmd.append('--sim')
        drxInternalTimer = config.getBool('Drx/DrxInternalTimer', false)
        if drxInternalTimer:
            drxcmd.append('--int')
        spawn(drxcmd)
        time.sleep(1)
    # products
    runProducts = config.getBool('Products/RunProducts', true)
    if  runProducts:
        productscmd = [
               eldoraDir + '/eldoraprod/eldoraprod',
               ]
        spawn(productscmd)
        
def stopEldoraApps():
    pkill('eldoraprod')
    pkill ('eldoradrx')
    autoRestartDcps = config.getBool('Dcps/AutoRestartDcps', true)
    if autoRestartDcps:
    	pkill ('DCPS')
    	
def restartDCPS():
    # DCPS
    runDcps = config.getBool('Dcps/RunDcps', true)
    if runDcps:
    	# if we are told to auto start dcps everytime, then
    	# make sure that it is not running already
    	autoRestartDcps = config.getBool('Dcps/AutoRestartDcps', true)
    	if autoRestartDcps:
    		pkill ('DCPS')
		# see if it is already running before starting again
    	dcpsIsRunning = not subprocess.call(['/usr/bin/pgrep', 'DCPS'])
    	print 'dcpsIsRunning is ', dcpsIsRunning
    	if not dcpsIsRunning:
	        dcpscmd = [
	            ddsRoot + '/bin/DCPSInfoRepo',
	            '-NOBITS',
	            '-DCPSConfigFile',conf + '/DCPSInfoRepo.ini' ,
	            '-ORBSvcConf', conf + '/ORBSvc.conf',
	            '-ORBListenEndpoints iiop://dcpsrepo:50000',
	            '-d', conf + '/DDSDomainIds.conf'
	            ]
	        spawn(dcpscmd)
	        time.sleep(1)
    
def pkill(name):
    pkill = '/usr/bin/pkill'
    pkillcmd =  [pkill, name]
    print pkillcmd
    subprocess.call(pkillcmd)
    
def spawn(cmd, sleepSecs=1):
    print 'cmd is: ', cmd
    pid = subprocess.Popen(cmd)
    print 'pid is ', pid

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

# start up DCPS, if configured to be running it here
restartDCPS()

                    
app = QApplication(sys.argv)

main = EldoraMain(runStopFunction=runStop, statusFunction=status)
main.show()

app.exec_()


