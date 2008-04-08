from time import *

from PyQt4.QtCore import *
from PyQt4.QtGui  import *

from Ui_EldoraMain import *

true = 1
false = 0

#
# EldoraMain provides the connection between the user 
# interface and the 'business' logic. It builds the Eldora
# GUI, based on the designer generated Ui_EldoraMain main window.
# User actions are connected to the class in the form of callback
# functions specified to the constructor.
#
class EldoraMain(QMainWindow, Ui_EldoraMain):
    def __init__(self, runStopFunction=None, shutdownFunction=None, statusFunction=None, parent=None):
        # initialize
        super(EldoraMain, self).__init__(parent)
        self.setupUi(self)
        self.runcallback = None
        self.shutdownFunction = shutdownFunction
        self.statusFunction = statusFunction
        # the status function will be called every multiple
        # of the timer timeouts.
        self.statusCount = 0
        self.statusPeriod = 10
        # create labels in the table cells
        for i in range(self.statusTable.rowCount()):
            item = QTableWidgetItem("x")
            self.statusTable.setItem(i, 0, item)
            item = QTableWidgetItem("y")
            self.statusTable.setItem(i, 1, item)


        
        # initialize the runstop button
        self.toggleRun(false)
        # set the callback _after_ calling toggleRun, so that it does
        # not get called during our construction. Otherwise, the 
        # routine that is constructing us might not yet have everything
        # in place for the callback to work properly.
        self.runcallback = runStopFunction

        # connect components
        # The run/stop button
        self.connect(self.runButton, SIGNAL("toggled(bool)"), self.toggleRun)
        # The shutdown button
        self.connect(self.shutdownButton, SIGNAL("released()"), self.shutdownFunction)
        
        # use our timer for a clock
        self.startTimer(1000)
        
    def timerEvent(self, event):
        self.dateTimeLabel.setText(asctime(gmtime()))
        self.statusCount = self.statusCount+1
        if self.statusCount >= self.statusPeriod:
            self.statusCount = 0
            # execute the status function
            if (self.statusFunction != None):
                self.statusFunction()
        
    # change the run state
    def toggleRun(self, runChecked):
           
        if (runChecked):
            # we have been put into the run state
            # set the button label to Stop
            self.runButton.setText("Stop")
        else:
            # we have been put into the stop state
            # set the button label to Run
            self.runButton.setText("Run")

        # execute the runStop callback if we have one
        if (self.runcallback != None):
           self.runcallback(runChecked)
           
               
           
    
            