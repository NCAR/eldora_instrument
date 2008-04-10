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
        # the status function will be called on multiples
        # of the main timer.
        self.statusCount = 0
        self.statusPeriod = 5
        # initialize the runstop button
        self.toggleRun(false)
        # set the callback _after_ calling toggleRun, so that it does
        # not get called during our construction. Otherwise, the 
        # routine that is constructing us might not yet have everything
        # in place for the callback to work properly.
        self.runcallback = runStopFunction

        # initialize dials
        self.initDials()
        # connect components
        # The run/stop button
        self.connect(self.runButton, SIGNAL("toggled(bool)"), self.toggleRun)
        # The shutdown button
        self.connect(self.shutdownButton, SIGNAL("released()"), self.shutdownFunction)
        
        # use our timer for a clock
        self.startTimer(1000)
        
    def initDials(self):
        # create the palette for the dials
        palette = self.palette()
        palette.setColor(QPalette.Active, QPalette.Foreground, QColor('red'))
        palette.setColor(QPalette.Active, QPalette.Button, QColor('lightgrey'))
        palette.setColor(QPalette.Inactive, QPalette.Foreground, QColor('red'))
        palette.setColor(QPalette.Inactive, QPalette.Button, QColor('lightgrey'))
        # configure the pulse rate displays
        lcds = [self.forwardPulsesLcd, self.aftPulsesLcd]
        for l in lcds:
            l.setPalette(palette)
        progs = [self.forwardPulsesProgress, self.aftPulsesProgress]
        for p in progs:
            p.setMinimum(0)
            p.setMaximum(8000)
            p.setPalette(palette)
            p.setValue(0)
        # configure the agregate BW dials
        dials = [self.forwardBWdial, self.aftBWdial]
        for d in dials:
              d.setMinimum(0)
              d.setMaximum(8000)
              d.setNotchesVisible(1)
              d.setValue(0)
              d.setSingleStep(200)
              d.setPalette(palette)
        # configure the individual dials
        forwardDialsList = self.forwardDials.children()
        aftDialsList = self.aftDials.children()
        # remove the first child, who will be the layout manager
        forwardDialsList = forwardDialsList[1:]
        aftDialsList = aftDialsList[1:]
        dials = forwardDialsList + aftDialsList
        # set the dials
        for dial in dials:
              dial.setMinimum(0)
              dial.setMaximum(2000)
              dial.setNotchesVisible(1)
              dial.setValue(0)
              dial.setSingleStep(50)
              dial.setPalette(palette)

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
           
               
           
    
            