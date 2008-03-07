#!/usr/bin/python

import sys
import time

from PyQt4.QtCore import *
from PyQt4.QtGui  import *

from EldoraMain import *

def runStop(runswitch):
    if (runswitch):
        print 'Running!'
    else:
        print 'Stopped'
        
app = QApplication(sys.argv)

main = EldoraMain(runStopFunction=runStop)
main.show()

app.exec_()


