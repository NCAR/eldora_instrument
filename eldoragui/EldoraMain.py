from time import *

import os
from PyQt4.QtCore  import *
from PyQt4.QtGui   import *

from Ui_EldoraMain import *
from EldoraUtil    import *
from QtConfig      import *


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
    def __init__(self, stopFunction=None, restartFunction=None, statusFunction=None, parent=None):
        # initialize
        super(EldoraMain, self).__init__(parent)
        self.setupUi(self)
        
   	# make sure that ELDORADIR is defined. This will only be done once,
    	# until 
        if 'ELDORADIR' not in os.environ:
        	m = QMessageBox.critical(self, 'Error', 'The ELDORADIR environment variable must be set')
        	# Post a close event. Note that post, rether than send, must be used
        	# so that the event will be queued if the event loop is not running yet.
        	QApplication.postEvent(QApplication.instance(), QCloseEvent())
        	return
        self.eldoraDir = os.environ['ELDORADIR']
        
        # get our configuration
        config = QtConfig('NCAR', 'EldoraGui')
  
        # save the callback function definitions
        self.runcallback = None
        self.statusFunction = statusFunction
        
        # save a palette that we can use for widget configuration
        self.createPalette()
        
        # the status function will be called on multiples
        # of the main timer.
        self.statusCount = 0
        self.statusPeriod = 5
        
        # initialize bandwidth displays
        self.initBwDisplays()
        
        # initialize disk stats
        self.initDiskStats()
        
        # connect components
        # The stop button
        if (stopFunction != None):
            self.connect(self.stopButton, SIGNAL("released()"), stopFunction)
        # the restart button
        if (restartFunction != None):
            self.connect(self.restartButton, SIGNAL("released()"), restartFunction)
        # the scope button
        self.connect(self.scopeButton, SIGNAL('released()'), self.runScope)
        # the forward ppi button
        self.connect(self.forwardPpiButton, SIGNAL('released()'), self.runForPpi)
        # the aft ppi button
        self.connect(self.aftPpiButton, SIGNAL('released()'), self.runAftPpi)
        
        # use our timer for a clock
        self.startTimer(1000)
        
    def runScope(self):
    	cmd = self.eldoraDir + '/eldorascope/eldorascope'
    	p = subprocess.Popen(cmd, close_fds=1)
    	        
    def runForPpi(self):
    	cmd = [self.eldoraDir + '/eldorappi/eldorappi', '--forward']
    	p = subprocess.Popen(cmd, close_fds=1)
    	
    def runAftPpi(self):
    	cmd = [self.eldoraDir + '/eldorappi/eldorappi', '--aft']
    	p = subprocess.Popen(cmd, close_fds=1)
    	        
    def createPalette(self):
    	fgcolor = 'blue'
    	buttoncolor = 'red'
        self.palette = self.palette()
        self.palette.setColor(QPalette.Active, QPalette.Foreground, QColor(fgcolor))
        self.palette.setColor(QPalette.Active, QPalette.Button, QColor(buttoncolor))
        self.palette.setColor(QPalette.Inactive, QPalette.Foreground, QColor(fgcolor))
        self.palette.setColor(QPalette.Inactive, QPalette.Button, QColor(buttoncolor))
        self.palette.setColor(QPalette.Disabled, QPalette.Foreground, QColor(fgcolor))
        self.palette.setColor(QPalette.Disabled, QPalette.Button, QColor(buttoncolor))
    	
        
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
	        d.setEnabled(0)
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
     
        
    def initBwDisplays(self):
        # configure the pulse rate displays
        progs = [self.forwardPulsesProgress, self.aftPulsesProgress]
        for p in progs:
            p.setMinimum(0)
            p.setMaximum(8000)
            p.setPalette(self.palette)
            p.setValue(0)
            p.setFormat("%v")
            p.setEnabled(0)
        # configure the agregate BW dials
        dials = [self.forwardBWdial, self.aftBWdial]
        for d in dials:
              d.setMinimum(0)
              d.setMaximum(8000)
              d.setNotchesVisible(1)
              d.setValue(0)
              d.setSingleStep(200)
              d.setPalette(self.palette)
              d.setEnabled(0)
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
              dial.setEnabled(0)

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
                   
               
           
    
            