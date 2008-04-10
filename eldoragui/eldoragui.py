#!/usr/bin/python

import sys
import time

from PyQt4.QtCore import *
from PyQt4.QtGui  import *

from EldoraMain import *
from EldoraRPC import *

rpc = EldoraRPC("http://localhost:60000")

def runStop(runswitch):
    try:
        if (runswitch):
            r = rpc.radarStart()
            main.statusLabel.setText(QString(r))
        else:
            r = rpc.radarStop()
            main.statusLabel.setText(QString(r))
    except Exception, e:
        print "Error trying to contact ", rpc, e
        
def shutdown():
    try:
       r = rpc.shutdown()
       main.statusLabel.setText(QString(r))
    except Exception, e:
        print "Error trying to contact ", rpc, e
        

def status():
    try:
       r = rpc.status()
       keys = sorted(r.keys())
       i = 0
       rates = []
       for k in keys:
          # convert rate from MB/s to KB/s and save
          rates.append(r[k]*1000.0)
          i = i + 1
    except Exception, e:
        print "Error trying to contact ", rpc, e
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
                    
app = QApplication(sys.argv)

main = EldoraMain(runStopFunction=runStop, shutdownFunction=shutdown, statusFunction=status)
main.show()

app.exec_()


