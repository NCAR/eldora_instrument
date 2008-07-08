#!/usr/bin/python

import os
import sys
import time
import re
import subprocess

class EldoraBlock(list):
    ''' 
    Represent a single header block. Derived from list.
    Consolidates all fields within a block. The list
    contains entries of the tuple (key, comment, value)
    '''
    def __init__(self, type, description):
        ''' Constructor. The type is the block type,
        such as RYIB, VOLD, etc.
        '''
        self.type = type
        self.description = description
        
    def addField(self, key, comment, value):
        ''' Add a field to this block. 
        '''
        self.append((key, comment, value))
        
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
        self.projectName = 'unnamed'
        self.verifyBlocks = []
        # collect the header blocks
        self.readHeader()
        # collect the verify blocks
        self.readVerify()
        # calculate checksum
        self.doCheckSum()
        
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
        self[:] = list()

        cmd = self.hdrDumpCmd + [self.headerFile,]
        try:
            c = subprocess.Popen(args=cmd, stdout=subprocess.PIPE)
        except Exception, e:
            print 'EldoraHeader.readHeader: error running command:'
            print '\t', ' '.join(cmd)
            print 'Error:', e
            sys.exit(1)
            
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
                description = (line.replace('*','')).strip()
            else:
                mainkey, subkey, comment, value = self.parseLine(line)
                if mainkey != '':
                    if blockstart:
                        self.append(EldoraBlock(mainkey, description))
                        description = 'unknown'
                        blockstart = False
                        
                    block = self[-1]
                    block.addField(subkey, comment, value)
                    
                    if mainkey == 'VOLD' and subkey == 'PROJ':
                        self.projectName = value
                            
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
                description = (line.replace('*','')).strip()
            else:
                mainkey, subkey, comment, value = self.parseLine(line)
                if mainkey != '':
                    if blockstart:
                        block = EldoraBlock(mainkey, description)
                        self.verifyBlocks.append(block)
                        blockstart = False
                        description = 'unknown'

                    block = self.verifyBlocks[-1]
                    block.addField(subkey, comment, value)
                    
           
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

        # drop comments immediately
        if line.startswith('***'):
            return noval
        
        # first break the line at the first space into the key and the remainder
        if not ' ' in line:
            return noval
        key, remainder = line.split(' ', 1)
        
        # break the remainder into description and value at the first colon
        if not ':' in remainder:
            return noval
        comment, value = remainder.split(':', 1)
        
        retval = ['','','','']
        # get the main and sub keys
        if len(key) < 5:
            return noval
        mainkey = key[0:4]
        subkey = key[4:]
        
        retval[0] = mainkey
        retval[1] = subkey
        retval[2] = comment.strip()
        retval[3] = value.strip()

        # return the result
        return retval

###############################################################
    def doCheckSum(self):
        cmd = ['/usr/bin/cksum',] + [self.headerFile,]
        c = subprocess.Popen(args=cmd, stdout=subprocess.PIPE)
        self.checksum = 0
        while 1:
            line = c.stdout.readline()
            if len(line) == 0:
                break
            self.checksum = int(line.split()[0])
       
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
            for f in b:
                print f
        for b in eldoraHdr.verifyBlocks:
            print '*************************** ', b.type, '*************************** '
            for f in b:
                print f
        print 'checksum is ',eldoraHdr.checksum
            
            
# Run the test when invoked standalone
if __name__ == '__main__':
    EldoraHeader.test()
