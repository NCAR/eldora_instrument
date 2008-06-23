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
        
        # create some useful palettes
        self.createPalettes()

        self.parse_dict = { 0: HPA.off, 1: HPA.on, 2: HPA.operate,
              3: HPA.standby, 4: HPA.antenna, 5: HPA.dummyload,
              6: HPA.status, 7: HPA.warmup, 8: HPA.cooldown }

        # save our serial device. None is acceptable, and
        # means that we are running in a test mode
        self.serialDevice = serialDevice
        
        # a vertical layout
        l = QVBoxLayout(self)
        self.frame = QFrame(self)
        self.frame.setContentsMargins(0,0,0,0)
        l.addWidget(self.frame)
        self.topLayout = QVBoxLayout(self.frame)
        l.insertStretch(-1)
       
        # command combobox
        self.commandCombo = QComboBox(self.frame)
        self.topLayout.addWidget(self.commandCombo)
        self.commandCombo.setToolTip('Select the HPA operating state')
        self.commandCombo.setPalette(self.redButtonPalette)

        
        # specify hpa modes, and our functions to be called when selected
        self.hpaCommands = []
        self.commandCombo.addItem('Off')
        self.hpaCommands.append(self.powerOff)
        self.commandCombo.addItem('On')
        self.hpaCommands.append(self.powerOn)
        self.commandCombo.addItem('Operate')
        self.hpaCommands.append(self.operate)
        self.commandCombo.addItem('Standby')
        self.hpaCommands.append(self.standby)
        # call doHpaCommand when a new command is chosen
        self.connect(self.commandCombo,
                     SIGNAL('currentIndexChanged(int)'),
                     self.doHpaCommand)
        
        # load combobox
        self.loadCombo = QComboBox(self.frame)
        self.topLayout.addWidget(self.loadCombo)
        self.loadCombo.setToolTip('Select the HPA output load')
        self.loadCombo.setPalette(self.redButtonPalette)

        # specify antenna/dummy load modes, and our 
        # functions to be called when selected
        self.loadCommands = []
        self.loadCombo.addItem('Dummy Load')
        self.loadCommands.append(self.dummyLoad)
        self.loadCombo.addItem('Antenna')
        self.loadCommands.append(self.antennaLoad)
         # call doLoadCommand when a new command is chosen
        self.connect(self.loadCombo,
                     SIGNAL('currentIndexChanged(int)'),
                     self.doLoadCommand)
       
        # The status text area
        self.statusText = QLabel(self.frame)
        self.statusText.setText('Status Unknown')
        self.topLayout.addWidget(self.statusText)
        self.statusText.setToolTip('Current status')
        
        # The warning text area
        self.warningText = QLabel(self.frame)
        self.warningText.setText('-')
        self.topLayout.addWidget(self.warningText)       
        self.warningText.setToolTip('Warning Messages')
        
        # The fault text area
        self.faultText = QLabel(self.frame)
        self.faultText.setText('No Faults')
        self.topLayout.addWidget(self.faultText)               
        self.faultText.setToolTip('Fault codes')

        # some empty space
        self.topLayout.insertStretch(-1)
        
        # create an Hpa device that we can control
        self.hpa = Hpa(self.serialDevice)
        
        # create a timer for performing status checks
        self.timer = QTimer()
        self.timer.setInterval(1000)
        self.connect(self.timer, SIGNAL('timeout()'), self.status)
        self.timer.start()
        
        self.msg_dict = { HPA.on : 'Power On',
                          HPA.off : 'Power Off',
                          HPA.operate : 'Operating',
                          HPA.standby : 'Standby',
                          HPA.warmup: 'Warming up',
                          HPA.cooldown: 'Cool down'
                          }
        
        self.load_pos = HPA_STATES.dummyload
        
        # create some useful palettes
        self.createPalettes()

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
        
######################################################################
    def doHpaCommand(self, commandIndex):
        # call the selected command
        self.hpaCommands[commandIndex]()
        
######################################################################
    def powerOn(self):
        self.commandCombo.setPalette(self.redButtonPalette)
        self.sendcmd(HPA.on)

######################################################################
    def powerOff(self):
        self.commandCombo.setPalette(self.redButtonPalette)
        self.sendcmd(HPA.off)

######################################################################
    def operate(self):
        self.commandCombo.setPalette(self.greenButtonPalette)
        self.sendcmd(HPA.operate)

######################################################################
    def standby(self):
        self.commandCombo.setPalette(self.redButtonPalette)
        self.sendcmd(HPA.standby)

######################################################################
    def doLoadCommand(self, commandIndex):
        # call the selected command
        self.loadCommands[commandIndex]()
        
######################################################################
    def dummyLoad(self):
        self.loadCombo.setPalette(self.redButtonPalette)
        self.sendcmd(HPA.dummyload)
        
######################################################################
    def antennaLoad(self):
        self.loadCombo.setPalette(self.greenButtonPalette)
        self.sendcmd(HPA.antenna)
        
######################################################################
    def sendcmd(self,op):
        cmd = self.parse_dict[op]
        s = self.hpa.command(cmd)
        if s != '':
            self.warning(s)
        else:
            self.update()

######################################################################
    def status(self):
        self.sendcmd(HPA.status)
        
######################################################################
    def update(self):
        if self.hpa.faulted:
            self.faultText.setPalette(self.redTextPalette)
            if self.faults == 0:
                self.faultText.setText('Unknown fault')
            else:
                faultCode = QString('Fault %x').arg(self.hpa.faults)
                self.faultText.setText(faultCode)
        else:
            self.faultText.setPalette(self.stdPalette)
            self.faultText.setText('No faults')

######################################################################
    def warning(self, warningText):
        self.warningText.setPalette(self.redTextPalette)
        self.warningText.setText('Warning')
        self.warningText.setToolTip(warningText)

######################################################################
    @staticmethod
    def test(device):
        # create the qt application               
        app = QApplication(sys.argv)
        
        d = QDialog()
        d.show()
        d.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        
        l1 = QVBoxLayout()
        d.setLayout(l1)
        l = QLabel("Test HpaWidget")
        l1.addWidget(l)
        
        # create horizontal layouts
        l2 = QHBoxLayout()
        l1.addLayout(l2)
        
        hpa0 = HpaWidget(parent=d, 
                         serialDevice=device,
                         hpaName='HPA 0')
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

 

        
