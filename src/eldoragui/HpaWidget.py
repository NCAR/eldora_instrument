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
        
        self.parse_dict = { 0: HPA.off, 1: HPA.on, 2: HPA.operate,
              3: HPA.standby, 4: HPA.antenna, 5: HPA.dummyload,
              6: HPA.status, 7: HPA.warmup, 8: HPA.cooldown }

        # save our serial device. None is acceptable, and
        # means that we are running in a test mode
        self.serialDevice = serialDevice
        
        # a vertical layout
        self.topLayout = QVBoxLayout(self)
        
        # command combobox
        self.commandCombo = QComboBox(self)
        self.topLayout.addWidget(self.commandCombo)
        self.commandCombo.setToolTip('Select the HPA operating state')
        
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
        self.loadCombo = self.loadCombo = QComboBox(self)
        self.topLayout.addWidget(self.loadCombo)
        self.loadCombo.setToolTip('Select the HPA output load')

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
       
        # some empty space
        self.topLayout.insertStretch(-1)
        
        # The status text area
        self.statusText = QLabel(self)
        self.statusText.setText('unknown')
        self.topLayout.addWidget(self.statusText)
        self.statusText.setToolTip('Current status')
        
        # The warning text area
        self.warningText = QLabel(self)
        self.warningText.setText('-')
        self.topLayout.addWidget(self.warningText)       
        
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

######################################################################
    def doHpaCommand(self, commandIndex):
        # call the selected command
        self.hpaCommands[commandIndex]()
        
######################################################################
    def powerOn(self):
        print 'powerOn'
        self.sendcmd(HPA.on)

######################################################################
    def powerOff(self):
        print 'powerOff'
        self.sendcmd(HPA.off)

######################################################################
    def operate(self):
        print 'operate'
        self.sendcmd(HPA.operate)

######################################################################
    def standby(self):
        print 'standby'
        self.sendcmd(HPA.standby)

######################################################################
    def doLoadCommand(self, commandIndex):
        # call the selected command
        self.loadCommands[commandIndex]()
        
######################################################################
    def dummyLoad(self):
        print 'dummyLoad'
        self.sendcmd(HPA.dummyload)
        
######################################################################
    def antennaLoad(self):
        print 'anntenaLoad'
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
            if self.faults == 0:
                self.warningText.setText('Unknown fault')
            else:
                faultCode = QString('%x').arg(self.hpa.faults)
                self.warningText.setText(faultCode)
        else:
            self.statusText.setText(self.hpa.status())

######################################################################
    def warning(self, warningText):
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

 

        
