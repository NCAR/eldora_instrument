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
    def __init__(self, runStopFunction=None, parent=None):
        # initialize
        super(EldoraMain, self).__init__(parent)
        self.setupUi(self)
        self.runcallback = runStopFunction
        
        # connect components
        # The run/stop button
        self.connect(self.runButton, SIGNAL("toggled(bool)"), self.toggleRun)
        
        # use our timer for a clock
        self.startTimer(1000)
        
    def timerEvent(self, event):
        self.dateTimeLabel.setText(asctime(gmtime()))
        
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

               
           
    
            