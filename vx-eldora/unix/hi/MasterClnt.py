import socket
import struct
import select
import time
import Common
import os

class MasterClnt:
    def __init__(self,targets):
	self.targets = targets
	self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	self.sock.bind('',0)
        self.msock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.msock.bind('',0)
	self.loadNeeded = 1
	self.minirims = 0
	self.dispCount = 2

    def sendCmd(self,cmd):
	scmd = struct.pack('>I',cmd)
	r = scmd

	if self.loadNeeded:
	    if cmd == Common.Cmds.start:
		self.loadNeeded = 0
		r = struct.pack('>I',cmd | Common.Cmds.load)

	index = 0
	if self.dispCount == 1:
	    index = 1

	try:
	    for t in self.targets:
		if t == 'fore' or t == 'aft':
		    self.sock.sendto(r,(t,Common.Port.cmdport))
		elif t == 'mini' and cmd == Common.Cmds.header:
		    index = index + 1
		    continue
		else:
		    self.sock.sendto(scmd,(t,Common.Port.cmdport))

		i,o,e = select.select([self.sock],[],[],10.0)
	    
		if i == []:
		    print 'no input'
		    return t

		stat = self.sock.recv(4)

		s = struct.unpack('>L',stat)

		if s[0] == -1:
		    return t

		if t == 'disp1' and cmd == Common.Cmds.start:
		    time.sleep(5)

		index = index + 1

	    return 'OK'

	except socket.error:
	    print 'socket error'
	    return self.targets[index]

    def start(self):
	status = self.sendCmd(Common.Cmds.start)
	if status != 'OK':
	    return status

	if self.minirims:
	    if self.align() == -1:
		return 'minirims'

	return 'OK'

    def stop(self):
	return self.sendCmd(Common.Cmds.stop)

    def align(self):
	s = struct.pack('>I',Common.Cmds.align_mini)

	try:
	    self.sock.sendto(t,('hskp',Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return -1

	    stat = self.sock.recv(4)

	    return struct.unpack('>I',stat)

	except socket.error:
	    return -1

    def connect(self):
	self.loadNeeded = 1
	return 0

    def numDisplays(self,v):
	self.dispCount = v

    def miniOff(self):
	self.minirims = 0
	
    def miniOn(self):
	self.minirims = 1

    def setFlight(self,f):
	self.flight = f

    def sendHeader(self,name):
	args = ('writeHeader',self.flight,name)
	pid = os.fork()
	if pid == 0:
	    file = '/scr/ymir/udp/hi/writeHeader'
	    os.execv(file,args)
	elif pid == -1:
	    return 1
	else:
	    i = os.waitpid(pid,0)
	    if i[1]:
		return 1

            # Send cappi version.
            self.msock.sendto('/vxbin/headers/current.hdr',("224.0.1.20",3101))

	    return self.sendCmd(Common.Cmds.header)

	
