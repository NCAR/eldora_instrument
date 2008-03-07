#!/usr/bin/python

import sys
import time

from PyQt4.QtCore import *
from PyQt4.QtGui  import *

from EldoraMain import *
from RPC import *

rpc = RPC("http://localhost:60000")
try:
    print rpc.listMethods()
except Exception, e:
        print "Error trying to contact ", rpc
    

def runStop(runswitch):
    try:
        if (runswitch):
            print 'Running! ', rpc.listMethods()
        else:
            print 'Stopped! ', rpc.listMethods()
    except Exception, e:
        print "Error trying to contact ", rpc
        
        
app = QApplication(sys.argv)

main = EldoraMain(runStopFunction=runStop)
main.show()

app.exec_()


