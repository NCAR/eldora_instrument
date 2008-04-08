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
       #
       i = 0
       for k in keys:
          label = main.statusTable.item(i,0)
          label.setText(k)
          val = main.statusTable.item(i,1)
          val.setText(QString("%1").arg(r[k]))
          i = i + 1
    except Exception, e:
        print "Error trying to contact ", rpc, e
        
app = QApplication(sys.argv)

main = EldoraMain(runStopFunction=runStop, shutdownFunction=shutdown, statusFunction=status)
main.show()

app.exec_()


