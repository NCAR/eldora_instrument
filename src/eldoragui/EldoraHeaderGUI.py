#!/usr/bin/python

import os
from PyQt4.QtCore  import *
from PyQt4.QtGui   import *
from EldoraHeader  import *

#####################################################################
class EldoraHeaderGUI:
    ''' EldoraHeaderGUI manages the user interface presentation
    for the Eldora system header management. 
    
    headerDirs is a list of directories to search for header files
    
    '''
    def __init__(self, hdrCombo, headerDirs):
        # the combobox used to select a header
        self.hdrCombo = hdrCombo
        # set the combobox to use fixed pitch font,
        # so that we can arrange justification in the entries
        font = self.hdrCombo.font()
        font.setFamily('Courier New')
        font.setBold(True)
        font.setPointSize(12)
        self.hdrCombo.setFont(font)
        # The directories to be searched for header files
        self.headerDirs = headerDirs
        # create a list of our headers
        self.headers = list()
        # load up the headers
        self.scan()

#####################################################################
    def scan(self):
        # find all files with extension '.hd'
        hdrfiles = []
        for d in self.headerDirs:
            files = os.listdir(d)
            for f in files:
                if not os.path.isdir(f):
                    ext = os.path.splitext(f)[-1]
                    if ext == '.hd':
                        fullpath = os.path.join(d, f)
                        hdrfiles.append(fullpath)

        # create header objects from these files
        for hfile in hdrfiles:
            dumpcmd = ['../headermaker/dumpheader','-h']
            verifycmd = ['../headermaker/dumpheader','-v']
            eldoraHdr = EldoraHeader(hdrDumpCmd=dumpcmd, 
                                     hdrVerifyCmd=verifycmd, 
                                     headerFile=hfile)
            if len(eldoraHdr) != 0:
                self.addHeader(eldoraHdr)         
        
#####################################################################
    def addHeader(self, header):
        headername = header.projectName + ''
        path = os.path.split(header.headerFile)[-1]
        while len(headername) < 50 - len(path):
            headername = headername + ' '
        headername = headername + path
        self.headers.append(header)
        self.hdrCombo.addItem(headername, 
                              QVariant(len(self.headers)-1))
        
#####################################################################

#####################################################################
#####################################################################
#####################################################################

# Run the test when invoked standalone
if __name__ == '__main__':
        # create the qt application               
        app = QApplication(sys.argv)
        
        d = QDialog()
        d.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        
        l1 = QHBoxLayout()
        d.setLayout(l1)
        
        combo = QComboBox()
        combo.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        l1.addWidget(combo)
        
        headerDirs = ['./', '../headermaker/headers/']
        
        EldoraHeaderGUI(combo, headerDirs)
    
        # run the event loop
        d.show()
        app.exec_()

           