#!/usr/bin/python

import os
import sys
from PyQt4.QtCore  import *
from PyQt4.QtGui   import *
from Hpa           import *

class HpaWidget(QWidget):
    '''A QWidget for controlling a high power amplifier (i.e. transmitter).
    An instance of an Hpa actually handles interaction with the device. Hpawidget
    provides the user interface with command handling and status polling
    '''
    def __init__(self, parent=None, serialDevice=None, hpaName='HPA'):
        
        super(QWidget, self).__init__(parent)
        
        self.setContentsMargins(0,0,0,0)
        
        # create some useful palettes
        self.createPalettes()

        self.parse_dict = { 0: HPA.off, 1: HPA.on, 2: HPA.operate,
              3: HPA.standby, 4: HPA.antenna, 5: HPA.load,
              6: HPA.status, 7: HPA.warmup, 8: HPA.cooldown }

        # save our serial device. None is acceptable, and
        # means that we are running in a test mode
        self.serialDevice = serialDevice
        
        # a vertical layout
        self.topLayout = QVBoxLayout()
        self.setLayout(self.topLayout)

        # state control
        # The state text area
        self.commandLayout = QHBoxLayout()
        self.topLayout.addLayout(self.commandLayout)
        # state combo box
        self.stateCombo = QComboBox()
        self.stateCombo.setToolTip('Select the HPA operating state')
        # specify hpa modes, and our functions to be called when selected
        self.hpaCommands = []
        self.stateCombo.addItem('Off')
        self.hpaCommands.append(self.powerOff)
        self.stateCombo.addItem('On')
        self.hpaCommands.append(self.powerOn)
        self.stateCombo.addItem('Operate')
        self.hpaCommands.append(self.operate)
        self.stateCombo.addItem('Standby')
        self.hpaCommands.append(self.standby)
        # call doHpaCommand when a new command is chosen
        self.connect(self.stateCombo,
                     SIGNAL('activated(int)'),
                     self.doHpaCommand)
        self.commandLayout.addWidget(self.stateCombo)
        
        # The position text area
        self.positionLayout = QHBoxLayout()
        self.topLayout.addLayout(self.positionLayout)
        # position combobox
        self.positionCombo = QComboBox()
        self.positionCombo.setToolTip('Select the HPA output position')
         # specify antenna/dummy load modes, and our 
        # functions to be called when selected
        self.loadCommands = []
        self.positionCombo.addItem('Load')
        self.loadCommands.append(self.dummyLoad)
        self.positionCombo.addItem('Antenna')
        self.loadCommands.append(self.antennaLoad)
         # call doLoadCommand when a new command is chosen
        self.connect(self.positionCombo,
                     SIGNAL('activated(int)'),
                     self.doLoadCommand)

        self.positionLayout.addWidget(self.positionCombo)
        
        # The remote/local area
        self.commsText = QLabel()
        self.commsText.setText('Unknown')
        self.commsText.setToolTip('Remote/Local')
        self.topLayout.addWidget(self.commsText)

        # The fault text area
        self.faultText = QLabel()
        self.faultText.setText('No Faults')
        self.faultText.setToolTip('Fault codes')        
        self.topLayout.addWidget(self.faultText)    
        
        # some empty space
        self.topLayout.insertStretch(-1)
    

        # some empty space
        self.topLayout.insertStretch(-1)

        # The warning text area
        self.warningText = QLabel()
        self.topLayout.addWidget(self.warningText)       
        self.warningText.setPalette(self.redTextPalette)
        
        # create an Hpa device that we can control
        self.hpa = Hpa(self.serialDevice)
        
        # create a timer for performing status checks
        self.timer = QTimer()
        self.timer.setInterval(1000)
        self.connect(self.timer, SIGNAL('timeout()'), self.status)
        self.timer.start()
        
###############################################################################
    def createPalettes(self):
        self.stdPalette = self.palette()
        
        self.greenButtonPalette = QPalette(self.stdPalette)
        self.setPaletteColors(self.greenButtonPalette, button='lime')
        
        self.redButtonPalette = QPalette(self.stdPalette)
        self.setPaletteColors(self.redButtonPalette, button='red')
        
        self.redTextPalette = QPalette(self.stdPalette)
        self.setPaletteColors(self.redTextPalette, text='red')
        
        self.greenTextPalette = QPalette(self.stdPalette)
        self.setPaletteColors(self.greenTextPalette, text='green')
        
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
        
######################################################################
    def doHpaCommand(self, commandIndex):
        # call the selected command
        print 'sending hpa command ', commandIndex
        self.hpaCommands[commandIndex]()
        self.stateCombo.setPalette(self.stdPalette)
        
######################################################################
    def doLoadCommand(self, commandIndex):
        # call the selected command
        print 'sending position command ', commandIndex
        self.loadCommands[commandIndex]()
        self.positionCombo.setPalette(self.stdPalette)
        
######################################################################
    def powerOn(self):
        self.sendcmd(HPA.on)

######################################################################
    def powerOff(self):
        self.sendcmd(HPA.off)

######################################################################
    def operate(self):
        self.sendcmd(HPA.operate)

######################################################################
    def standby(self):
        self.sendcmd(HPA.standby)

######################################################################
    def dummyLoad(self):
        self.sendcmd(HPA.load)
        
######################################################################
    def antennaLoad(self):
        self.sendcmd(HPA.antenna)
        
######################################################################
    def sendcmd(self,op):
        cmd = self.parse_dict[op]
        s = self.hpa.command(cmd)
        self.warning(s)
        self.update()

######################################################################
    def status(self):
        result = self.hpa.status()
        self.warning(result)
        self.update()
        
######################################################################
    def update(self):
        state = self.hpa.state
        if state.find('Operate') == -1:
            self.stateCombo.setPalette(self.redButtonPalette)
        else:
            self.stateCombo.setPalette(self.greenButtonPalette)    
        for i in range(0, self.stateCombo.count()):
            if state.find(self.stateCombo.itemText(i)) != -1:
                self.stateCombo.setCurrentIndex(i)
                break

        position = self.hpa.position
        if position.find('Antenna') == -1:
            self.positionCombo.setPalette(self.redButtonPalette)
        else:
            self.positionCombo.setPalette(self.greenButtonPalette)
        for i in range(0, self.positionCombo.count()):
            if position.find(self.positionCombo.itemText(i)) != -1:
                self.positionCombo.setCurrentIndex(i)
                break
            
        faults = self.hpa.faults
        if len(faults) > 0:
            self.faultText.setText('Fault')
            self.faultText.setPalette(self.redTextPalette)
            self.faultText.setToolTip(faults)
        else:
            self.faultText.setText('')
            self.faultText.setPalette(self.greenTextPalette)
            self.faultText.setToolTip('')            
        
        comms = self.hpa.comms 
        self.commsText.setText(comms)
        if comms.find('Remote') == -1:
            self.commsText.setPalette(self.redTextPalette)
        else:
            self.commsText.setPalette(self.greenTextPalette)
 
######################################################################
    def warning(self, warningText):
        if warningText == '':
            self.warningText.setText('')
            self.warningText.setToolTip('')
        else:
            self.warningText.setText('Error')
            self.warningText.setToolTip(warningText)

######################################################################
    @staticmethod
    def test(device):
        # create the qt application               
        app = QApplication(sys.argv)
        
        d = QDialog()
        d.resize(200,200)
        d.show()
        d.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        
        l1 = QVBoxLayout()
        d.setLayout(l1)
        text = QString("HpaWidget")
        if device != None:
            text = text + ' for ' + device
        l = QLabel(text)
        l1.addWidget(l)
        
        # create horizontal layouts
        l2 = QHBoxLayout()
        l1.addLayout(l2)
        
        hpa0 = HpaWidget(parent=d, 
                         serialDevice=device,
                         hpaName='HPA')
        l2.addWidget(hpa0)
        
        # now start the event loop
        app.exec_()

######################################################################
# Run the test when invoked standalone
if __name__ == '__main__':
    if len(sys.argv) == 1:
        serialDevice = None
    else:
        serialDevice = sys.argv[1]
    HpaWidget.test(serialDevice)

 

        
