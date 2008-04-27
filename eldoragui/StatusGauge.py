import os
import sys
from PyQt4.QtCore  import *
from PyQt4.QtGui   import *

class StatusGauge(QWidget):
    def __init__(self, parent=None, 
                 colors=None, 
                 cellPercent=90.0, 
                 orient='vertical', 
                 payload=None) :
        QWidget.__init__(self, parent)
        
        self.cellPercent = cellPercent
        self.orient = orient
    
        self.setMinimumSize(30, 30)
        self.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
    
        # set the default inidicators
        if colors == None:
            colors = []
            colors.append("green")
            colors.append("yellow")
            colors.append("red")
        
        # configure ourself
        self.reconfigure(colors=colors, cellPercent=cellPercent, orient=orient, payload=payload)


#################################################
    def paintEvent(self, e):

        h = self.height();
        w = self.width();
    
        n = len(self.ison)
        # what is the height or width of our boxes, and the
        # indicator diameter?
        delta = 0.0
        diameter = 0.0
        fraction = self.cellPercent/100.0
        if (self.orient == 'vertical'):
            # divide vertical column into boxes
            delta = h/n
            diameter = delta*fraction
            if diameter > w*fraction:
                diameter = w*fraction
        else:
            # divide horizontal strip into boxes
            delta = w/n;
            diameter = delta*fraction
            if diameter > h*fraction:
                diameter = h*fraction
        
        p = QPainter()
        p.setPen(Qt.SolidLine)
        p.begin(self)
        for i in range(len(self.ison)):
            # if no color is specified, then the client didn't want an
            # indicator in this position
            if self.colors[i] != '':
                if (self.orient == 'vertical'):
                    leftX = ((w-diameter)/2)
                    topY = (h-(i+1)*delta + (delta-diameter)/2)
                else:
                    leftX = (i*delta + delta/2-diameter/2)
                    topY = (h-diameter)/2
    
                color = QColor (self.colors[i])
                if self.ison[i] == False:
                    color.setAlpha(30)
                else:
                    color.setAlpha(255)
                brush = QBrush(color)
                p.setBrush(brush)
                p.drawEllipse(leftX, topY, diameter, diameter)
        p.end()

#################################################
    def on(self, index, ison):
        if index < 0 or index >= len(self.ison):
            return
        
        self.ison[index] = ison
    
        self.repaint()
                
#################################################
    def allOn(self, ison):
        for i in range(len(self.ison)):
            self.ison[i] = ison
        self.repaint()
        
#################################################
    def mouseReleaseEvent(self, e):
        if self.payload != None:
            self.emit(SIGNAL('released'),self.payload)
        else:
            self.emit(SIGNAL('released'))
        
#################################################
    def reconfigure(self, colors,
                    cellPercent=90.0,
                    orient='vertical',
                    payload=None):
        self.colors = colors;
        self.cellPercent = cellPercent
        self.orient = orient
        self.payload = payload
    
        if (self.cellPercent < 0):
            self.cellPercent = 0.0
    
        # initialize on/off status
        self.ison = []
        for i in range(len(self.colors)):
            self.ison.append(False)
            
        # redraw with the new configuration
        self.repaint()
        
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
        d.show()
        d.resize(100,400)
        
        l1 = QVBoxLayout()
        d.setLayout(l1)
        l = QLabel("Test StatusGauge")
        l1.addWidget(l)
        
        # create horizontal and vertical layouts
        l2 = QHBoxLayout()
        l1.addLayout(l2)
        l3 = QVBoxLayout()
        l1.addLayout(l3)
        
        gauges = []
        # put vertical gauges in the h layout
        s = StatusGauge(d, payload=d)
        s.on(1, True)
        l2.addWidget(s)
        gauges.append(s)
        
        s = StatusGauge(d, payload=d)
        s.on(0, True)
        l2.addWidget(s)
        gauges.append(s)
        
        colors=['gold','', 'blue']
        s = StatusGauge(colors=colors, payload=d)
        s.on(2, True)
        l2.addWidget(s)
        gauges.append(s)
        
        # put horizontal gauges in the v layout
        colors=['gold','red', 'blue', 'violet','maroon','wheat','grey']
        s = StatusGauge(orient='horizontal', colors=colors, payload=d)
        s.on(4, True)
        l3.addWidget(s)
        gauges.append(s)
        
        s = StatusGauge(orient='horizontal', colors=colors, payload=d)
        s.allOn(True)
        l3.addWidget(s)
        gauges.append(s)
        
        for g in gauges:
            QObject.connect(g,SIGNAL("released"), StatusGauge.gaugePressed)

        # now start the event loop
        app.exec_()

# Uncomment the following line in order to run the test
# simply by invoking 'python StatusGauge.py'
#StatusGauge.test()
