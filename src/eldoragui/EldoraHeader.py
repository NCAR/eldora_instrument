#!/usr/bin/python

import os
import sys
import time
import re
import subprocess

class EldoraHeaderBlock(dict):
    ''' 
    Represent a single header block.
    The fields within the block are dictionary entries
    withing this class (note dict as superclass).
    '''
    def __init__(self, type):
        self.type = type
        
    def addField(self, key, comment,value):
        payload = [comment, value]
        self[key] = payload
        
class EldoraHeader(list):
    ''' A class for reading and decoding the information in 
    an eldora header file. An external header dump command
    is used to dump a specified header, and the output from
    that command is parsed.
    '''
    def __init__(self, headerDumpCmd, headerFile):
        self.headerDumpCmd = headerDumpCmd
        self.headerFile = headerFile
        self.blocks = list()
        self.readHeader()
        
###############################################################
    def readHeader(self):
        ''' 
        Read and parse the header file, and save it in self.blocks
        self.blocks is a list of EldoraBlock
        '''
        cmd = self.headerDumpCmd + [self.headerFile,]
        c = subprocess.Popen(args=cmd, stdout=subprocess.PIPE)
        self[:] = list()
        blockstart = False
        while 1:
            line = c.stdout.readline()
            if len(line) == 0:
                break
            if line.find('****') != -1:
                # the next non-comment line will be the start of a block
                blockstart = True
            else:
                mainkey, subkey, comment, value = self.parseLine(line)
                if mainkey != '':
                    if blockstart:
                        block = EldoraHeaderBlock(mainkey)
                        block.addField(subkey, comment, value)
                        self.append(block)
                        blockstart = False
                    else:
                        block = self[-1]
                        block.addField(subkey, comment, value)
                        
           
###############################################################
    def parseLine(self, line):
        ''' 
        Decode a header data line. This line must have
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
        #
        print 'enter the header file name: '
        line = sys.stdin.readline()
        file = re.sub('\n', '', line)
        cmd = ['../headermaker/dumpheader','-h']
        eldoraHdr = EldoraHeader(headerDumpCmd=cmd, headerFile=file)
        for b in eldoraHdr:
            print '*************************** ', b.type, '*************************** '
            for f in b.items():
                print f
            
            
# Run the test when invoked standalone
if __name__ == '__main__':
    EldoraHeader.test()

    