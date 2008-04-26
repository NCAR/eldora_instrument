import os
from PyQt4.QtCore  import *
from PyQt4.QtGui   import *

class statusGauge(QWidget):
    def __init__(self, parent) :
        QWidget.__init__(self, parent)
        
        self._cellPercent = 90.0
        self._orient = GaugeVertical
    
        self.setMinimumSize(30, 100)
        self.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
    
        # set the default inidicators
        colors = []
        colors.append("green")
        colors.append("yellow")
        colors.append("red")
    
        reconfigure(colors, _cellPercent);

#################################################
#################################################
#################################################
#################################################
#################################################
#################################################
#################################################
#################################################
    def reconfigure(colors,
                    cellPercent,
                    orient):
        self.colors = colors;
        self.cellPercent = cellPercent
        self.orient = orient
    
        if (self.cellPercent < 0):
            self.cellPercent = 0.0
    
        # initialize on/off status
        self.ison = []
        for i in range(len(self.colors)):
            self.ison[i] = False
        
#################################################
    @staticmethod
    def test():
        print 'testing StatusGauge'
        return 
        # create the qt application               
        app = QApplication(sys.argv)
        
        d = QDialog
        v = QVBoxLayout
        d.setLayout(v)
        d.show()
        
        l = QLabel("My Label")
        v.addWidget(l)
        
        app.exec_()
        

