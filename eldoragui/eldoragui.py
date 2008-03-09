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
        
        
app = QApplication(sys.argv)

main = EldoraMain(runStopFunction=runStop)
main.show()

app.exec_()


