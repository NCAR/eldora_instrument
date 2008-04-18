import os
import sys
import time
import subprocess
import re
import PyQt4.QtCore

class EmitterProc(PyQt4.QtCore.QThread):
    '''
    Spawn a process in a QThread, and emit a signal that
    contains the text from stdout.
    A new thread is created, and the specified command is 
    spawnwed as a subprocess. stdout is monitored, and the
    returned text is emitted as a SIGNAL(text(), data, color),
    which can be connected to other Qt slots.
    Using a QThread allows us to utilize the Qt signal/slot
    mechanism between threads, and Qt will handle the 
    thread access control.
    '''
    def __init__(self, command, emitText=False, textColor='black'):
        PyQt4.QtCore.QThread.__init__(self)
        self.emitText = emitText
        self.textColor = textColor
        self.command = command
        
    def run(self):
        try:
            stdout = subprocess.Popen(self.command, bufsize=1, stdout=subprocess.PIPE).stdout
            while 1:
                line = stdout.readline()
                if not line:
                    break
                line = re.sub('\n','',line)
                if self.emitText:
                    self.emit(PyQt4.QtCore.SIGNAL("text"), line, self.textColor)
        except Exception, e:
            print 'got an exception in ', self, ': ', e
            
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
        # print 'your command is ', cmd
        s = SubThread(command=cmd, callback=SubThread.testCallback)
        s.start()
        while 1:
            time.sleep(1)
            print 'sleeping...'
            
        