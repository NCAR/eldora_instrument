import os
import sys
import time
import re
import subprocess

class EldoraHeaderBlock:
    def __init__(self, type):
        self.type = type
        self.fields = list()
        
    def addField(self, key, comment,value):
        field = [key, comment, value]
        self.fields.append(field)
        
class EldoraHeader:
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
        cmd = [self.headerDumpCmd, self.headerFile]
        c = subprocess.Popen(args=cmd, stdout=subprocess.PIPE)
        self.blocks = list()
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
                        self.blocks.append(block)
                        blockstart = False
                    else:
                        block = self.blocks[-1]
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
        cmd = '../headermaker/dumpheader'
        file = './testheader.hd'
        eldoraHdr = EldoraHeader(headerDumpCmd=cmd, headerFile=file)
        blocks = eldoraHdr.blocks
        for b in blocks:
            print 'Block ', b.type
            for f in b.fields:
                print f
            
# To run the test, uncomment the following line and run
# 'python EldoraHeader.py'        
EldoraHeader.test()

    