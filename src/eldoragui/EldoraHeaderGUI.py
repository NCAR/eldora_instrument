#!/usr/bin/python

import os
from PyQt4.QtCore     import *
from PyQt4.QtGui      import *
from EldoraHeader     import *
from EldoraHeaderView import *

#####################################################################
class EldoraHeaderGUI(QWidget):
    ''' EldoraHeaderGUI manages the user interface presentation
    for the Eldora system header management. 
    
    headerDirs is a list of directories to search for header files
    
    signals:
    header(selectedHeader) - emitted when a new header is chosen
    
    '''
    def __init__(self, hdrCombo, headerDirs, hdrDumpApp):
        QObject.__init__(self)
        # the combobox used to select a header
        self.hdrCombo = hdrCombo
        self.connect(self.hdrCombo,SIGNAL("activated(int)"), self.hdrChosen)    
        # set the combobox to use fixed pitch font,
        # so that we can arrange justification in the entries
        font = self.hdrCombo.font()
        font.setFamily('Courier')
        font.setBold(True)
        font.setPointSize(12)
        self.hdrCombo.setFont(font)
        # Path to the dumpheader command
        self.hdrDumpApp = hdrDumpApp
        # The directories to be searched for header files
        self.headerDirs = headerDirs
        # create a list of our headers
        self.headers = list()
        # load up the headers
        self.scan()
        # save the chosen one. Use None if there were no headers
        if len(self.headers) == 0:
            self.selectedHeader = None
        else:
            self.selectedHeader = self.headers[0]


#####################################################################
    def scan(self):
        ''' Find all files ending with .hd in the directories
        listed in self.headerDirs. Extract an eldoraHeader
        from each file, and ad that header to our collection 
        of headers. The headers are collected in self.headers.
        '''
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
            dumpcmd = [self.hdrDumpApp,'-h']
            verifycmd = [self.hdrDumpApp,'-v']
            eldoraHdr = EldoraHeader(hdrDumpCmd=dumpcmd, 
                                     hdrVerifyCmd=verifycmd, 
                                     headerFile=hfile)
            if len(eldoraHdr) != 0:
                self.addHeader(eldoraHdr)         
        
#####################################################################
    def addHeader(self, header):
        ''' Append header to self.headers. Add a selection
        for this header to the combo box.
        '''
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
        self.selectedHeader = self.headers[index]
        self.emit(SIGNAL("header"), self.selectedHeader)
        
#####################################################################
    def viewHeader(self):
        ''' Called when the user wants to see what is in the header.
        Popup a display of the header
        '''
       # create an EldoraHeaderView
        e = EldoraHeaderView(header=self.selectedHeader,
                             parent=self)
        e.show()
          
#####################################################################

# Run the test when invoked standalone
if __name__ == '__main__':
        # create the qt application               
        app = QApplication(sys.argv)
        
        d = QDialog()
        d.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        
        l1 = QHBoxLayout()
        d.setLayout(l1)
        
        viewbtn = QPushButton('View')
        l1.addWidget(viewbtn)

        combo = QComboBox()
        combo.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        l1.addWidget(combo)
        
        headerDirs = ['./',]
        
        gui = EldoraHeaderGUI(combo, headerDirs)
        gui.connect(viewbtn, SIGNAL("released()"), gui.viewHeader)
    
        # run the event loop
        d.show()
        app.exec_()

           