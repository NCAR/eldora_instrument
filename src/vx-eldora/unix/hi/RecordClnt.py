import socket
import struct
import select
import Common
import RecordGbl

class RecordClnt:
    def __init__(self,target):
	self.target = target
	self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	self.sock.bind('odin',0)

    def sendCmd(self,cmd):
	if cmd == RecordGbl.Cmds.reconnect:
	    cmd = Common.Cmds.status

	s = struct.pack('>I',cmd)

	try:
	    self.sock.sendto(s,(self.target,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return (-1,)

	    stat = self.sock.recv(56)

	    st = struct.unpack('>LLLLLLLLLLLLii',stat)

	    return st

	except socket.error:
	    return (-1,)


    def status(self):
	return self.sendCmd(Common.Cmds.status)
