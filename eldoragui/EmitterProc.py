import os
import sys
import time
import re
from PyQt4.QtCore import *

class EmitterProc(QProcess):
    '''
    Spawn a command in a QProcess, and emit a signal that
    contains the text from stdout and stderr.
    
    The specified command is spawnwed as a QProcess. Stdout and
    stderr are read when triggered by the readyReadStandardOutput 
    signal, and the returned text is emitted as a 
    SIGNAL(text(), data, color), which can be connected to 
    other Qt slots.
    
    Using a QProcess allows us to utilize the Qt signal/slot
    mechanism between threads, and Qt will handle the 
    thread access control.
    
    NB: If you are instantiating EmitterProc in a static function, make 
    that there is a global reference to the object when the function is
    exited, if you want the process to continue after the function leaves 
    scope.
    '''
    def __init__(self, command, emitText=False, textColor='black'):
        QProcess.__init__(self)
        self.emitText = emitText
        self.textColor = textColor
        self.command = command
        self.setProcessChannelMode(QProcess.MergedChannels)
        self.connect(self, SIGNAL("readyReadStandardOutput()"), self.readyRead)

        
    def start(self):
        QProcess.start(self, self.command[0], self.command[1:])
        
    def readyRead(self):
        while (1):
            line = self.readLine()
            if not line:
                return
            if self.emitText:
                self.emit(SIGNAL("text"), line, self.textColor)
        
###############################################################
    @staticmethod
    def testCallback(line):
        print line
        
    @staticmethod
    def test():
        '''
        A test routine for SubThread
        '''
        print 'your command is my wish: '
        line = sys.stdin.readline()
        line = re.sub('\n', '', line)
        cmd = re.split(' +',line)
        s = EmiterProc(command=cmd)
        s.start()
        while 1:
            time.sleep(1)
            print 'sleeping...'
            
        