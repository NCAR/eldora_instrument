from time import *

from PyQt4.QtCore import *
from PyQt4.QtGui  import *

from Ui_EldoraMain import *

import subprocess

true = 1
false = 0

# 
# DiskStats will privide disk statistics
#
class DiskStats:
	def __init__(self):
		stats = self.stats()
		self.num = len(stats)
		
	def stats(self):
	    cmd = ['df', '-l', '-h']
	    p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, 
	    	stderr=subprocess.PIPE, close_fds=1)
	    dfout = p.stdout.read().split()
	    dfout = dfout[7:]
	    result = []
	    for i in range(0, len(dfout), 6):
	    	percent = dfout[i+4]
	    	percent = percent.strip('%')
	    	prcnt = int(percent)
	    	result.append((dfout[i+5],prcnt ))
	    return result
	
	def numDisks(self):
		return self.num
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
        
        # save a palette that we can use for widget configuration
        self.createPalette()
        
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
        
        # initialize disk stats
        self.initDiskStats()
        
        # connect components
        # The run/stop button
        self.connect(self.runButton, SIGNAL("toggled(bool)"), self.toggleRun)
        # The shutdown button
        self.connect(self.shutdownButton, SIGNAL("released()"), self.shutdownFunction)
        
        # use our timer for a clock
        self.startTimer(1000)
        
    def createPalette(self):
        self.palette = self.palette()
        self.palette.setColor(QPalette.Active, QPalette.Foreground, QColor('red'))
        self.palette.setColor(QPalette.Active, QPalette.Button, QColor('lightblue'))
        self.palette.setColor(QPalette.Inactive, QPalette.Foreground, QColor('red'))
        self.palette.setColor(QPalette.Inactive, QPalette.Button, QColor('lightblue'))
    	
        
    def initDiskStats(self):
        # create our disk status object
        self.diskStats = DiskStats()
        # create the disk usage dials
        hlayout = QHBoxLayout(self.diskStatsBox)
        self.diskDials = []
        stats = self.diskStats.stats()
        for i in range(len(stats)):
        	# create a group box and a layout for the dial and label
	        box = QGroupBox()
	        vlayout = QVBoxLayout()
	        
	        # create dial and add to layout
	        d = QDial(self.diskStatsBox)
	        d.setMinimum(0)
	        d.setMaximum(100)
	        d.setPalette(self.palette)
	        d.setSingleStep(10)
	        d.setNotchesVisible(1)
	    	vlayout.addWidget(d)
	    	
	    	# set the dial value from the disk stats
	    	d.setValue(stats[i][1])
	    	
	    	# create label and add to layout
	    	l = QLabel(stats[i][0])
	    	l.setPalette(self.palette)
	    	vlayout.addWidget(l)
	    	
	    	# assign the layout to the box
	    	box.setLayout(vlayout)
	    	
	    	# add the box to the horizontal layout
	    	hlayout.addWidget(box)
	    	
	    	# save a reference to the disk dials
	    	self.diskDials.append(d)
     
        
    def initDials(self):
        # configure the pulse rate displays
        progs = [self.forwardPulsesProgress, self.aftPulsesProgress]
        for p in progs:
            p.setMinimum(0)
            p.setMaximum(8000)
            p.setPalette(self.palette)
            p.setValue(0)
            p.setFormat("%v")
        # configure the agregate BW dials
        dials = [self.forwardBWdial, self.aftBWdial]
        for d in dials:
              d.setMinimum(0)
              d.setMaximum(8000)
              d.setNotchesVisible(1)
              d.setValue(0)
              d.setSingleStep(200)
              d.setPalette(self.palette)
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
              dial.setPalette(self.palette)

    def timerEvent(self, event):
        self.dateTimeLabel.setText(asctime(gmtime()))
        self.statusCount = self.statusCount+1
        if self.statusCount >= self.statusPeriod:
            self.statusCount = 0
            # execute the status function
            if (self.statusFunction != None):
                self.statusFunction()
            # do the disk status
            stats = self.diskStats.stats()
            for i in range(len(stats)):
            	dial = self.diskDials[i]
            	dial.setValue(stats[i][1])
        
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
           
               
           
    
            