import os
import sys
import time
import re
import subprocess

class EldoraHeader:
    ''' A class for reading and decoding the information in 
    an eldora header file. An external header dump command
    is used to dump a specified header, and the output from
    that command is parsed.
    '''
    def __init__(self, headerDumpCmd, headerFile):
        self.headerDumpCmd = headerDumpCmd
        self.headerFile = headerFile
        self.dict = dict()
        self.readHeader()
        
###############################################################
    def readHeader(self):
        cmd = [self.headerDumpCmd, self.headerFile]
        c = subprocess.Popen(args=cmd, stdout=subprocess.PIPE)
        while 1:
            line = c.stdout.readline()
            if len(line) == 0:
                break
            mainkey, subkey, comment, value = self.parseLine(line)
            if mainkey != '':
                print mainkey, subkey
           
###############################################################
    def parseLine(self, line):
        ''' Decode a header data line. This line must have
        an initial key, comment text, a colon, and value text. E.g:
        PARMSAMPLS NUM SAMPLES USED IN ESTIMATE: 40
        The first four characters of the key define the main
        key, the following characters are a sub key.
        Return a four element list containing [mainkey, subkey, comment, value]
        Ignore outright lines begining with ***, as these are 
        known to be comment lines
        '''

        noval = ['','','','']
        # break up the line
        tokens = line.split()
        # convnce ourselves that it is valid
        if len(tokens) < 3:
            return noval
        if tokens[1].find('***') != -1:
            return noval
        if not ':' in tokens:
            return noval
        
        retval = ['','','','']
        # get the main and sub keys
        key = tokens[0]
        if len(key) < 5:
            return noval
        mainkey = key[0:4]
        subkey = key[4:]
        retval[0] = mainkey
        retval[1] = subkey
        
        # get the comment
        tokens = tokens[1:]
        for t in tokens:
            tokens = tokens[1:]
            if (t != ':'):
                retval[2] = retval[2] + ' ' + t
            else:
                break
        retval[2] = retval[2].strip()
        
        # get the value
        for t in tokens:
            retval[3] = retval[3] + ' ' + t
        retval[3] = retval[3].strip()
        
        # return the result
        return retval
    
        
###############################################################
###############################################################
###############################################################
###############################################################
    @staticmethod
    def test():
        print 'This test expects to find dumpheader in ../headermaker'
        cmd = '../headermaker/dumpheader'
        file = './testheader.hd'
        eldoraHdr = EldoraHeader(headerDumpCmd=cmd, headerFile=file)
        
EldoraHeader.test()

    