#!/usr/bin/python

import os
import sys
import time
import re
import subprocess

class EldoraBlock(dict):
    ''' 
    Represent a single header block. Derived from dict.
    Consolidates all fields within a block.
    The fields are referenced by the identifier 
    assigned by dumpheader.
    '''
    def __init__(self, type, description):
        ''' Constructor. The type is the block type,
        such as RYIB, VOLD, etc.
        '''
        self.type = type
        self.description = description
        
    def addField(self, key, comment,value):
        ''' Add a field to this block. 
        '''
        payload = [comment, value]
        self[key] = payload
        
class EldoraHeader(list):
    ''' A class for reading and decoding the information in 
    an eldora header file. An external header dump command
    is used to dump a specified header, and the output from
    that command is parsed.
    The header blocks are collected in our list.
    
    This class also runs a header verify command, and collects 
    that output in a list of blocks. This list is the data
    member verifyBlocks.
    '''
    def __init__(self, hdrDumpCmd, hdrVerifyCmd, headerFile):
        self.hdrDumpCmd = hdrDumpCmd
        self.hdrVerifyCmd = hdrVerifyCmd
        self.headerFile = headerFile
        self.projectName = 'unamed'
        self.verifyBlocks = []
        # collect the header blocks
        self.readHeader()
        # collect the verify blocks
        self.readVerify()
        
###############################################################
    def readHeader(self):
        ''' 
        Read and parse the header file, collect the
        header blocks, and save it in our list of
        EldoraBlocks. The header is dumped using
        the self.hdrDumpCmd. This command puts out
        "key comment:value" triples. The key will 
        not begin with V, which is reserved for the 
        verify command.
        '''
        cmd = self.hdrDumpCmd + [self.headerFile,]
        c = subprocess.Popen(args=cmd, stdout=subprocess.PIPE)
        self[:] = list()
        blockstart = False
        description = 'unknown'
        while 1:
            line = c.stdout.readline()
            if len(line) == 0:
                break
            line = line.replace('\n','')
            if line.find('****') != -1:
                # the next non-comment line will be the start of a block
                blockstart = True
                # save the description that appears in the ****  ****** line
                description = (line.replace('*','')).lstrip().rstrip()
            else:
                mainkey, subkey, comment, value = self.parseLine(line)
                if mainkey != '':
                    if blockstart:
                        block = EldoraBlock(mainkey, description)
                        block.addField(subkey, comment, value)
                        self.append(block)
                        blockstart = False
                        if mainkey == 'VOLD' and subkey == 'PROJ':
                            self.projectName = value
                    else:
                        block = self[-1]
                        block.addField(subkey, comment, value)
                        if mainkey == 'VOLD' and subkey == 'PROJ':
                            self.projectName = value
                        description = 'unknown'
                            
###############################################################
    def readVerify(self):
        ''' 
        Read and parse the header file verify output, and save it 
        as eldoraBlocks in self.verifyBlocks
        '''
        cmd = self.hdrVerifyCmd + [self.headerFile,]
        c = subprocess.Popen(args=cmd, stdout=subprocess.PIPE)
        self.verifyBlocks[:] = list()
        blockstart = False
        description = 'unknown'
        while 1:
            line = c.stdout.readline()
            if len(line) == 0:
                break
            line = line.replace('\n','')
            if line.find('****') != -1:
                # the next non-comment line will be the start of a block
                blockstart = True
                description = (line.replace('*','')).lstrip().rstrip()
            else:
                mainkey, subkey, comment, value = self.parseLine(line)
                if mainkey != '':
                    if blockstart:
                        block = EldoraBlock(mainkey, description)
                        block.addField(subkey, comment, value)
                        self.verifyBlocks.append(block)
                        blockstart = False
                    else:
                        block = self.verifyBlocks[-1]
                        block.addField(subkey, comment, value)
                        description = 'unknown'
                    
           
###############################################################
    def parseLine(self, line):
        ''' 
        Decode a header data line. This line must have
        an initial key, comment text, a colon, and value text. E.g:
        PARMSAMPLS NUM SAMPLES USED IN ESTIMATE : 40
        
        The first four characters of the key define the main
        key, the following characters are a sub key.
        
        Return a four element list containing [mainkey, subkey, comment, value]
        
        Ignore outright lines begining with ***, as these are 
        known to be comment lines
        '''

        noval = ['','','','']
        # break up the line
        # make sure that the colon is blank delimited
        newline = line.replace(':', ' : ')
        tokens = newline.split()
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
    @staticmethod
    def test():
        print 'This test expects to find dumpheader in ../headermaker'
        #
        print 'enter the header file name: '
        line = sys.stdin.readline()
        file = re.sub('\n', '', line)
        dumpcmd = ['../headermaker/dumpheader','-h']
        verifycmd = ['../headermaker/dumpheader','-v']
        eldoraHdr = EldoraHeader(hdrDumpCmd=dumpcmd, hdrVerifyCmd=verifycmd, headerFile=file)
        for b in eldoraHdr:
            print '*************************** ', b.type, '*************************** '
            for f in b.items():
                print f
        for b in eldoraHdr.verifyBlocks:
            print '*************************** ', b.type, '*************************** '
            for f in b.items():
                print f
            
            
# Run the test when invoked standalone
if __name__ == '__main__':
    EldoraHeader.test()
