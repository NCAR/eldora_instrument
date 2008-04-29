import os
import sys
import random
from PyQt4.QtCore  import *
from PyQt4.QtGui   import *

class ProgressStrip(QWidget):
####################################################
    def __init__(self, parent=None, max=100, title='', format='%v', sumMode=False):
        '''
        If sumMode is true, then the LCD presents a running total
        '''
        QWidget.__init__(self, parent)
        self.current = 0
        self.max = max
        self.format = format
        self.sumMode = sumMode

        # create the layout
        vl = QVBoxLayout()
        self.setLayout(vl)
        # add the title
        self.title = QLabel(title)
        self.title.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        self.title.setAlignment(Qt.AlignHCenter)
        vl.addWidget(self.title)
        # add the progress bar
        self.bar = QProgressBar()
        self.bar.setMaximum(self.max)
        self.bar.setOrientation(Qt.Vertical)
        self.bar.setFormat(self.format)
        self.bar.setMinimumWidth(30)
        self.bar.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.MinimumExpanding)
        vl.addWidget(self.bar)
        # add the LCD
        self.lcd = QLCDNumber()
        self.lcd.setSegmentStyle(QLCDNumber.Flat)
        self.lcd.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        vl.addWidget(self.lcd)
        
####################################################
    def setMaximum(self, max):
        self.bar.setMaximum(max)
        self.max = max
        
####################################################
    def setValue(self, value):
    	v = value
    	if v > self.max:
    		v = self.max
        self.bar.setValue(v)
        if not self.sumMode:
            self.lcd.display(value)
        else:
            self.lcd.display(self.lcd.value() + value)            
        self.current = value

####################################################
    def addValue(self, value):
        v = value + self.current
        if v > self.max:
            v = v - self.max
        self.bar.setValue(v)
        if not self.sumMode:
            self.lcd.display(value)
        else:
            self.lcd.display(self.lcd.value() + value)            
        self.current = v
####################################################
    def value(self):
        return self.bar.value()
    
####################################################
    def maximum(self):
        return self.bar.maximum()
    
####################################################
    @staticmethod
    def testtimeout():
        pstrip1.setValue(random.randint(0, 1000))
        pstrip2.addValue(40+random.randint(-7,7))
        
####################################################
    @staticmethod
    def test():
        # create the qt application               
        app = QApplication(sys.argv)
        
        d = QDialog()
        d.show()
        d.resize(100,400)
        
        l1 = QHBoxLayout()
        d.setLayout(l1)
        
        global pstrip1
        pstrip1 = ProgressStrip(parent=d, title='Bitrate', max=1000)
        l1.addWidget(pstrip1)
        
        global pstrip2
        pstrip2 = ProgressStrip(parent=d, title='Packets', max=1000)
        l1.addWidget(pstrip2)

        timer = QTimer()
        timer.setSingleShot(False)
        timer.setInterval(200)
        timer.start()
        QObject.connect(timer, SIGNAL('timeout()'), ProgressStrip.testtimeout)

        # run the event loop
        app.exec_()
        
# To run the test, uncoment the following line and run 'python ProgressStrip.pf'
#ProgressStrip.test()
        
        
        