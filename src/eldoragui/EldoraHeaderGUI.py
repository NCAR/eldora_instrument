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
        QObject.connect(self.hdrCombo,SIGNAL("activated(int)"), self.hdrChosen)    
        # set the combobox to use fixed pitch font,
        # so that we can arrange justification in the entries
        font = self.hdrCombo.font()
        font.setFamily('Courier')
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
        headername = header.projectName + ' '
        path = os.path.split(header.headerFile)[-1]
        # create the combo text, combining the project
        # name and the file name. Justify with the
        # project left algined and file right aligned.
        while len(headername) < 40 - len(path):
            headername = headername + ' '
        headername = headername + path
        self.headers.append(header)
        self.hdrCombo.addItem(headername, 
                              QVariant(len(self.headers)-1))
        
#####################################################################
    def hdrChosen(self, index):
        ''' Called when the selected item in the list of headers
        is changed.
        '''
        text = 'You selected the ' \
            + self.headers[index].projectName \
            + ' header'
        print text
        QMessageBox.information(None,'Header Selection', text)
        
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
        
        headerDirs = ['./',]
        
        EldoraHeaderGUI(combo, headerDirs)
    
        # run the event loop
        d.show()
        app.exec_()

           