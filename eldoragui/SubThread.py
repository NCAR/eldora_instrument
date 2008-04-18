import threading
import os
import sys
import time
import subprocess
import re

class SubThread(threading.Thread):
    '''
    Spawn a process as a thread, and send the output back
    to a callback function
    '''
    def __init__(self, command, callback=None):
        threading.Thread.__init__(self)
        self.callback = callback
        self.command = command
        
    def run(self):
        try:
            stdout = subprocess.Popen(self.command, bufsize=1, stdout=subprocess.PIPE).stdout
            while 1:
                line = stdout.readline()
                if not line:
                    break
                line = re.sub('\n','',line)
                if self.callback != None:
                    self.callback(line)
                
        except Exception, e:
            print 'Exception in SubThread.run(): ', e
            
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
            
        