import socket
import struct
import select
import Common
import HskpGbl

class HskpClnt:
    def __init__(self,target):
	self.target = target
	self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	self.sock.bind('',0)
	self.year = 0
	self.month = 1
	self.day = 1
	self.hour = 0
	self.minute = 0
	self.second = 0

    def sendCmd(self,cmd):
	s = struct.pack('>IBBBBBB',cmd,self.year,self.month,self.day,
			self.hour,self.minute,self.second)

	try:
	    self.sock.sendto(s,(self.target,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return (-1,)

	    stat = self.sock.recv(17)

	    st = struct.unpack('>BBBBBBBBBBBBBBBBB',stat)

	    return st

	except socket.error:
	    return (-1,)

    def status(self):
	return self.sendCmd(Common.Cmds.status)

    def connect(self):
	s = self.sendCmd(Common.Cmds.status)
	if s[0] == -1:
	    return 1
	return 0

    def setYear(self,v):
	self.year = v

    def setMonth(self,v):
	self.month = v

    def setDay(self,v):
	self.day = v

    def setHour(self,v):
	self.hour = v

    def setMin(self,v):
	self.minute = v

    def setSec(self,v):
	self.second = v

