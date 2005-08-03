import socket
import struct
import select
import Common
import RadarGbl

class radarClnt:
    def __init__(self,fore,aft):
	self.fore = fore
	self.aft = aft
	self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	self.sock.bind('',0)
	self.foredict = {
	    'rp7': 0, 'mcpl': 0, 'status': 0x8000, 'mailbox': 0x0
	    }
	self.aftdict = {
	    'rp7': 0, 'mcpl': 0, 'status': 0x8000, 'mailbox': 0x0
	    }

    def foreStatus(self):
	s = struct.pack('>I',Common.Cmds.status)

	try:
	    self.sock.sendto(s,(self.fore,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return self.foredict

	    stat = self.sock.recv(6)

	    st = struct.unpack('>bbhh',stat)

	    self.foredict['rp7'] = st[0]
	    self.foredict['mcpl'] = st[1]
	    self.foredict['status'] = st[2]
	    self.foredict['mailbox'] = st[3]

	    return self.foredict

	except socket.error:
	    return self.foredict

    def aftStatus(self):
	s = struct.pack('>I',Common.Cmds.status)

	try:
	    self.sock.sendto(s,(self.aft,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return self.aftdict

	    stat = self.sock.recv(6)

	    st = struct.unpack('>bbhh',stat)

	    self.aftdict['rp7'] = int(st[0])
	    self.aftdict['mcpl'] = int(st[1])
	    self.aftdict['status'] = st[2]
	    self.aftdict['mailbox'] = st[3]

	    return self.aftdict

	except socket.error:
	    return self.aftdict

    def connect(self):
	fs = self.aftStatus()
	if fs['status'] == 0x8000:
	    return 1
	as = self.foreStatus()
	if as['status'] == 0x8000:
	    return 1
	return 0

    def tsgate(self,gate):
	s = struct.pack('>Idi',RadarGbl.RadarCmds.tsgate,gate,0)

	try:
	    self.sock.sendto(s,(self.aft,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return 1

	    stat = self.sock.recv(6)

	    self.sock.sendto(s,(self.fore,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return 1

	    stat = self.sock.recv(6)

	    return 0

	except socket.error:
	    return 1

    def clrDC(self):
        s = struct.pack('>I',RadarGbl.RadarCmds.clr_dc)

        	try:
	    self.sock.sendto(s,(self.aft,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return 1

	    stat = self.sock.recv(6)

	    self.sock.sendto(s,(self.fore,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return 1

	    stat = self.sock.recv(6)

	    return 0

	except socket.error:
	    return 1
        
