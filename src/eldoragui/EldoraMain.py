from time import *

import os
from PyQt4.QtCore  import *
from PyQt4.QtGui   import *

from Ui_EldoraMain import *
from EldoraUtil    import *
from StatusGauge   import *
from ProgressStrip import *
from EldoraHeaderGUI import *


######################################################################################
class EldoraMain(QDialog, Ui_EldoraMain):
    '''
    EldoraMain provides the connection between the user 
    interface and the 'business' logic. It builds the Eldora
    GUI, based on the designer generated Ui_EldoraMain main window.
    User actions are connected to the class in the form of callback
    functions specified to the constructor.
    
    The following signals are emitted:
       start:   start has been requested
       stop:    stop has been requested
       status:  time to do a status update
       ready:   the GUI is ready. This will be emitted once, on the first timer tick.
       scope:   scope has been requested
       ppi(for):ppi has been requested, for==true for forward scope
    '''
    ###############################################################################
    def __init__(self, 
                 headersDir,
                 hdrDumpApp,
                 parent=None):
        # initialize
        super(EldoraMain, self).__init__(parent)
        self.setupUi(self)
        
        # save the location of the headers
        self.headersDir = headersDir
        
        # create a eldoraHeaderGUI to manage header interaction
        self.headerGui = EldoraHeaderGUI(self.hdrCombo, [headersDir,], hdrDumpApp)
        # save the current header
        self.selectedHeader = self.headerGui.selectedHeader
            
        # Several useful palettes will be created
        self.createPalettes()
        
        # set the stop button to red:
        self.stopButton.setPalette(self.redButtonPalette)
        self.stopButton.setDown(True)
        self.startOn = False
        
        # prime the ready signal, which will be emitted
        # on the first timer tick
        self.ready = False 

        # the status function will be called on multiples
        # of the main timer.
        self.statusCount = 0
        self.statusPeriod = 1
        
        # initialize the gauges
        self.initGauges()
        
        # initialize bandwidth displays
        self.initBwDisplays()
        
        # initialize disk stats
        self.initDiskStats()
        
        # configure log browser
        self.logBrowser.setLineWrapMode(QTextEdit.NoWrap)
        self.logCursor = QTextCursor(self.logBrowser.document())
        self.logBrowser.setReadOnly(True)
        
        # connect components
        # The stop button
        self.connect(self.stopButton, SIGNAL("released()"), self.stop)
        # the start button
        self.connect(self.startButton, SIGNAL("released()"), self.start)
        # the scope button
        self.connect(self.scopeButton, SIGNAL('released()'), self.scope)
        # the forward ppi button
        self.connect(self.forwardPpiButton, SIGNAL('released()'), self.forPpi)
        # the aft ppi button
        self.connect(self.aftPpiButton, SIGNAL('released()'), self.aftPpi)
        # The view header button
        self.connect(self.viewHdrBtn, SIGNAL('released()'), self.headerGui.viewHeader)
        # the expand button
        self.connect(self.expandButton, SIGNAL('released()'), self.showHide)
        # The 'header' signal indicates a new header has been chosen
        self.connect(self.headerGui, SIGNAL("header"), self.headerSelected)
        
        # use our timer for a clock. Remember that the first tick will 
        # casue a ready signal to be emitted.
        self.startTimer(1000)
    
    ###############################################################################
    def showStatus(self, 
                   ABPrate, 
                   productRate, 
                   productStatus, 
                   rates):

        #
        # set the for and aft bw dials
        forRate = 0.0
        aftRate = 0.0
        for i in range(16):
          if i < 8:
              forRate = forRate + rates[i]
          else:
              aftRate = aftRate + rates[i]
              
        self.forABP.setValue(ABPrate/2)
        self.aftABP.setValue(ABPrate/2)
        self.forProducts.setValue(productRate/2)
        self.aftProducts.setValue(productRate/2)
        self.setGauge('Products', productStatus)
         
        # set the for and aft pulse progress strips
        self.forBytes.setValue(forRate)
        self.aftBytes.setValue(aftRate)
        # set the individual channel dials
        forwardDialsList = self.forwardDials.children()
        aftDialsList = self.aftDials.children()
        # remove the first child, who will be the layout manager
        forwardDialsList = forwardDialsList[1:]
        aftDialsList = aftDialsList[1:]
        for i in range(16):
              if i < 8:
                  forwardDialsList[i].setValue(rates[i])
              else:
                  aftDialsList[i-8].setValue(rates[i])
                  
        self.forwardBWdial.setValue(forRate)
        self.aftBWdial.setValue(aftRate)
        if (forRate < 400 or aftRate < 400):
            self.setGauge('DRX', 2)
        else:
            if (forRate < 800 or aftRate < 800):
                self.setGauge('DRX', 1)
            else:
                self.setGauge('DRX', 0)

    ###############################################################################
    def headerSelected(self):
        ''' Emit a signal saying that a new header has been chosen,
        and send the selected header with it
        '''
        self.emit(SIGNAL('header'), self.headerGui.selectedHeader)
        if self.startOn and self.selectedHeader != self.headerGui.selectedHeader:
            self.controlMsg.setPalette(self.redTextPalette)
            self.controlMsg.setText('Header changed, but Eldora was not restarted')
        self.selectedHeader = self.headerGui.selectedHeader
        
    ###############################################################################
    def start(self):
        ''' A start() signal will be emitted.
        '''
        self.emit(SIGNAL('start'))
        self.stopButton.setChecked(False)
        self.stopButton.setPalette(self.stdPalette)
        self.stopButton.setDown(False)
        self.startButton.setPalette(self.greenButtonPalette)
        self.startButton.setDown(True)
        self.controlMsg.setText('')
        self.startOn = True

    ###############################################################################
    def stop(self):
        ''' A stop() signal will be emitted.
        '''
        self.emit(SIGNAL('stop'))
        self.startButton.setChecked(False)
        self.stopButton.setPalette(self.redButtonPalette)
        self.stopButton.setDown(True)
        self.startButton.setPalette(self.stdPalette)
        self.startButton.setDown(False)
        self.controlMsg.setText('')
        self.startOn = False

    ###############################################################################
    def scope(self):
        self.emit(SIGNAL('scope'))
                  
    ###############################################################################
    def forPpi(self):
        self.emit(SIGNAL('ppi'), True)
    	
    ###############################################################################
    def aftPpi(self):
        self.emit(SIGNAL('ppi'), False)

    ###############################################################################
    def showHide(self):
        # I had to fiddle around a bunch before hitting on the 
        # sequence below to get the dilog to resize correctly.
        # I honestly don't know why it works.
        if self.expandButton.isChecked():
            self.statusTab.hide()
            self.expandButton.setText('Show')
            QCoreApplication.sendPostedEvents()
            self.updateGeometry()
            self.resize(1,1)
        else:
            self.statusTab.show()
            self.expandButton.setText('Hide')
            QCoreApplication.sendPostedEvents()
            self.updateGeometry()
            self.resize(1,1)
           
    ###############################################################################
    def createPalettes(self):
        self.stdPalette = self.palette()
        
        self.greenButtonPalette = QPalette(self.stdPalette)
        self.setPaletteColors(self.greenButtonPalette, button='lime')
        
        self.redButtonPalette = QPalette(self.stdPalette)
        self.setPaletteColors(self.redButtonPalette, button='red')
        
        self.redTextPalette = QPalette(self.stdPalette)
        self.setPaletteColors(self.redTextPalette, text='red')
        

    ###############################################################################
    def setPaletteColors(self, palette, window=None, button=None, text=None):
        if window != None:
            palette.setColor(QPalette.Active, QPalette.Window, QColor(window))
            palette.setColor(QPalette.Inactive, QPalette.Window, QColor(window))
            palette.setColor(QPalette.Disabled, QPalette.Window, QColor(window))
        if text != None:
            palette.setColor(QPalette.Active, QPalette.WindowText, QColor(text))
            palette.setColor(QPalette.Inactive, QPalette.WindowText, QColor(text))
            palette.setColor(QPalette.Disabled, QPalette.WindowText, QColor(text))
        if button != None:
            palette.setColor(QPalette.Active, QPalette.Button, QColor(button))
            palette.setColor(QPalette.Inactive, QPalette.Button, QColor(button))
            palette.setColor(QPalette.Disabled, QPalette.Button, QColor(button))
    	
    ###############################################################################
    def initGauges(self):
        ''' Create and initialize the status gauges. Reference to the 
        gauges are stored in the gauges dictionary. The gauges are
        placed in the main gaugeBox.
        '''
        self.gauges = {}
        l = QHBoxLayout()
        self.gaugeBox.setLayout(l)
        self.gauges['DRX']         = self.addGauge(title='DRX',
                                                   layout=l,
                                                   callback=self.gaugeReleased)
        self.gauges['Housekeeper'] = self.addGauge(title='Housekeeper',
                                                   layout=l,
                                                   callback=self.gaugeReleased)
        self.gauges['Products']    = self.addGauge(title='Products',
                                                   layout=l,
                                                   callback=self.gaugeReleased)
        self.gauges['Archiver']    = self.addGauge(title='Archiver',
                                                   layout=l,
                                                   callback=self.gaugeReleased)
         
    ###############################################################################
    def addGauge(self, title, layout, initialOn=2, callback=None):
        ''' Create a single StatusGauge. The gauge is placed
        in a QGroupBox, becasue that provides nice titling.
        A callback can be specified, which wil be connected to
        the gauge's released signal. The payload for the callback
        will be the gauge title.
        '''
        g = QGroupBox()
        g.setTitle(title)
        g.setAlignment(Qt.AlignHCenter)
        layout.addWidget(g)
        vl = QVBoxLayout()
        g.setLayout(vl)
        s = StatusGauge(payload=title)
        s.on(initialOn, True)
        vl.addWidget(s)
        if callback != None:
            QObject.connect(s, SIGNAL('released'), callback)
        return s
        
    ###############################################################################
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
	        d.setPalette(self.stdPalette)
	        d.setSingleStep(10)
	        d.setNotchesVisible(1)
	        d.setEnabled(0)
	    	vlayout.addWidget(d)
	    	
	    	# set the dial value from the disk stats
	    	d.setValue(stats[i][1])
	    	
	    	# create label and add to layout
	    	l = QLabel(stats[i][0])
	    	l.setPalette(self.stdPalette)
	    	vlayout.addWidget(l)
	    	
	    	# assign the layout to the box
	    	box.setLayout(vlayout)
	    	
	    	# add the box to the horizontal layout
	    	hlayout.addWidget(box)
	    	
	    	# save a reference to the disk dials
	    	self.diskDials.append(d)
     
        
    ###############################################################################
    def layoutRateBox(self,
                    box,
                    rateStrips):
        l = QHBoxLayout()
        box.setLayout(l)
        for r in rateStrips:
            l.addWidget(r)
                       
    ###############################################################################
    def initBwDisplays(self):
        '''
        configure the rate displays
        '''
        sumMode = False
        self.forBytes = ProgressStrip(max=4000,title='For',sumMode=sumMode)
        self.aftBytes = ProgressStrip(max=4000,title='Aft',sumMode=sumMode)
        self.layoutRateBox(self.DRXRateBox,[self.forBytes,self.aftBytes])
        # 
        self.forABP = ProgressStrip(max=2000,title='For',sumMode=sumMode)
        self.aftABP = ProgressStrip(max=2000,title='Aft',sumMode=sumMode)
        self.layoutRateBox(self.ABPRateBox,[self.forABP,self.aftABP])
        #
        self.forProducts = ProgressStrip(max=1000,title='For',sumMode=sumMode)
        self.aftProducts = ProgressStrip(max=1000,title='Aft',sumMode=sumMode)
        self.layoutRateBox(self.ProductRateBox,[self.forProducts,self.aftProducts])
        
         #
        self.forArchive = ProgressStrip(max=1000,title='For',sumMode=sumMode)
        self.aftArchive = ProgressStrip(max=1000,title='Aft',sumMode=sumMode)
        self.layoutRateBox(self.ArchiveRateBox,[self.forArchive,self.aftArchive])
        
        # configure the agregate BW dials
        dials = [self.forwardBWdial, self.aftBWdial]
        for d in dials:
              d.setMinimum(0)
              d.setMaximum(4000)
              d.setNotchesVisible(1)
              d.setValue(0)
              d.setSingleStep(200)
              d.setPalette(self.stdPalette)
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
              dial.setPalette(self.stdPalette)
              dial.setEnabled(0)

    ###############################################################################
    def timerEvent(self, event):
        ''' Called at regular intervals to handle periodic
        tasks, such as emiiting the startup signal and the 
        status signal.
        '''
        
        # execute the startup function
        if not self.ready:
            self.emit(SIGNAL('ready'))
            # disable startup signal
            self.ready = True
            
        self.statusCount = self.statusCount+1
        if self.statusCount >= self.statusPeriod:
            self.statusCount = 0
            # execute the status function
            self.emit(SIGNAL('status'))
            # do the disk status
            stats = self.diskStats.stats()
            for i in range(len(stats)):
            	dial = self.diskDials[i]
            	dial.setValue(stats[i][1])
                   
    ###############################################################################
    def logText(self, text, color='black'):
        ''' Send the text to the log display.
        
        text = The text to be displayed
        color = The text color
        '''
        html = '<font color="' + color + '">' + text + '</font>'
        self.logBrowser.append(html)

    ###############################################################################
    def gaugeReleased(self, name):
        msg = 'Gauge ' + name + ' was pressed!'
        QMessageBox.information(self, 'Alert!', msg)
        
    ###############################################################################
    def setGauge(self, name, index):     
        ''' Turn on gauge[name] for indicator[index], and turn off all
        other indicators.
        '''
        g = self.gauges[name]
        g.allOn(False)
        g.on(index, True) 
           
    
            