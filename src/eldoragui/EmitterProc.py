#!/usr/bin/python

import os
import sys
import time
import re
from PyQt4.QtCore import *
from PyQt4.QtGui  import *

class EmitterProc(QProcess):
    '''
    Spawn a command in a QProcess, and emit a signal that
    contains the text from stdout and stderr.
    
    The specified command is spawnwed as a QProcess. Stdout and
    stderr are read when triggered by the readyReadStandardOutput 
    signal, and the returned text is emitted as a 
    SIGNAL(text(), text, payload), which can be connected to 
    other Qt slots.
    
    Using a QProcess allows us to utilize the Qt signal/slot
    mechanism between threads, and Qt will handle the 
    thread access control.
    
    NB: If you are instantiating EmitterProc in a static function, make 
    that there is a global reference to the object when the function is
    exited, if you want the process to continue after the function leaves 
    scope.
    '''
    def __init__(self, command, emitText=False, payload=None):
        QProcess.__init__(self)
        self.emitText = emitText
        self.payload = payload
        self.command = command
        # prefix lines that are emmitted on text with the command name
        self.lineprefix = os.path.split(os.path.splitext(command[0])[0])[1]
        print self.lineprefix
        self.lineprefix = self.lineprefix + ': '
        self.setProcessChannelMode(QProcess.MergedChannels)
        self.connect(self, SIGNAL("readyReadStandardOutput()"), self.readyRead)

        
    def start(self):
        QProcess.start(self, self.command[0], self.command[1:])
        line= self.lineprefix
        for p in self.command:
            line = line + ' ' + p
        self.emit(SIGNAL("text"), line, self.payload)
        
    def startDetached(self):
        QProcess.startDetached(self.command[0], self.command[1:])
        
    def readyRead(self):
        while (1):
            line = self.readLine()
            if not line:
                return
            if self.emitText:
                text = self.lineprefix + line
                self.emit(SIGNAL("text"), 
                          text, 
                          self.payload)
        
###############################################################
    @staticmethod
    def testCallback(text, color):
        html = '<font color="' + color + '">' + text + '</font>'
        d.append(html)
        
    @staticmethod
    def test():
        '''
        A test routine for SubThread
        '''
        # create a qt application so that we have an event loop.            
        app = QApplication(sys.argv)
        global d
        d = QTextEdit()
        d.resize(1000, 300)
        d.show()
        
        # request the command
        print 'your command is my wish: '
        line = sys.stdin.readline()
        line = re.sub('\n', '', line)
        cmd = re.split(' +',line)
        
        # create the EmitterProc and start the process
        s = EmitterProc(command=cmd, emitText=True, payload="magenta")
        QObject.connect(s,SIGNAL("text"), EmitterProc.testCallback)
        s.start()
        
        # start our event loop
        app.exec_()
            
# Run the test when invoked standalone
if __name__ == '__main__':
    EmitterProc.test()

        