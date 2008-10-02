#!/usr/bin/python

import os
import sys
from PyQt4.QtCore  import *
from PyQt4.QtGui   import *

class StatusGauge(QLabel):
    def __init__(self, parent=None, 
                 payload=None) :
        QLabel.__init__(self, parent)
        
        self.setMinimumSize(30, 30)
        self.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
    
        # Load the per-state images.  Try for our png-s if we can find them,
        # else fall back to boring colored squares...
        pixmapPath = os.path.join(os.environ['ELDORADIR'], 'lib', 'python', '')
        if (os.path.exists(pixmapPath + 'greenLED.png')):
            pixmaps = [QPixmap(pixmapPath + 'greenLED.png'),
                       QPixmap(pixmapPath + 'yellowLED.png'),
                       QPixmap(pixmapPath + 'redLED.png')]
        else:
            print("StatusGauge: Could not find %s.  Using colored squares." %
                  (pixmapPath + 'greenLED.png'))
            pixmaps = []
            width = 24
            height = 24
            img = QImage(width, height, QImage.Format_RGB888)
            # for colorval in [green, yellow, red]:
            for colorval in [0x00ff00, 0x00ffff, 0x0000ff]:
                img.fill(colorval)
                pixmaps.append(QPixmap.fromImage(img))
                
        # configure ourself
        self.reconfigure(pixmaps=pixmaps, payload=payload)

#################################################
# Change to the selected state
    def setState(self, state):
        if (state == self.state):
            return
        
        self.state = state
        self.setPixmap(self.pixmaps[self.state])
        self.update()
        
#################################################
    def mouseReleaseEvent(self, e):
        if self.payload != None:
            self.emit(SIGNAL('released'),self.payload)
        else:
            self.emit(SIGNAL('released'))
        
#################################################
    def reconfigure(self, pixmaps, payload=None):
        self.pixmaps = pixmaps;
        self.payload = payload

        # Use the largest width and largest height from our pixmaps as
        # the preferred size for the QLabel
        maxW = 1
        maxH = 1
        for i in range(len(pixmaps)):
            if (pixmaps[i].width() > maxW):
                maxW = pixmaps[i].width()
            if (pixmaps[i].height() > maxH):
                maxH = pixmaps[i].height()
            
        self.sizeHint = QSize(maxW, maxH)
        
        # Display pixmaps in the center of the QLabel
        self.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
    
        # initialize state
        self.state = -1     # forces pixmap change in setState
        self.setState(0)
        
#################################################
    @staticmethod
    def gaugePressed(parent):
        QMessageBox.information(parent,'Hello!', 'You pressed the gauge!')
        
#################################################
    @staticmethod
    def test():
        # create the qt application               
        app = QApplication(sys.argv)
        
        d = QDialog()
        
        l1 = QVBoxLayout()
        d.setLayout(l1)
        l = QLabel("Test StatusGauge")
        l1.addWidget(l)
        
        gauges = []
        # put vertical gauges in the h layout
        s = StatusGauge(d, payload=d)
        s.setState(1)
        l1.addWidget(s)
        gauges.append(s)
        
        s = StatusGauge(d, payload=d)
        s.setState(0)
        l1.addWidget(s)
        gauges.append(s)
        
        s = StatusGauge(payload=d)
        s.setState(2)
        l1.addWidget(s)
        gauges.append(s)
        
        d.show()

        for g in gauges:
            QObject.connect(g,SIGNAL("released"), StatusGauge.gaugePressed)

        # now start the event loop
        app.exec_()

# Run the test when invoked standalone
if __name__ == '__main__':
    StatusGauge.test()

 
