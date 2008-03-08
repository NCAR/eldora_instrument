#!/usr/bin/python

import sys
import time

from PyQt4.QtCore import *
from PyQt4.QtGui  import *

from EldoraMain import *
from EldoraRPC import *

rpc = EldoraRPC("http://localhost:60000")
try:
    methods =  rpc.listMethods()
except Exception, e:
    print "Error trying to contact ", rpc
    methods = ()
print "there are ", len(methods), " methods"
    

def runStop(runswitch):
    try:
        if (runswitch):
            print rpc.radarStart()
        else:
            print rpc.radarStop()
    except Exception, e:
        print "Error trying to contact ", rpc
        
        
app = QApplication(sys.argv)

main = EldoraMain(runStopFunction=runStop)
main.show()

app.exec_()


