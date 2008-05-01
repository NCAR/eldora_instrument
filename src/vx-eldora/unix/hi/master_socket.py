#!/local/bin/python --
#
# Module:          master_socket.py
# Original Author: Rich Neitzel
# Date:            $Date$
# Copywrited by the National Center for Atmospheric Research
# Tk based GUI for ELDORA displays.
#
# revision history
# ----------------
# $Log$

import socket
import select
import struct
import Common
from dispCmd import *

class CommonClnts:
    def __init__(self,targets):
		
	for i in targets:
	    if i == 'display1':
		self.d1 = DispCmd(i)
	    elif i == 'display2':
		self.d2 = DispCmd(i)
	    elif i == 'fore':
		self.fore =
	    
	    
    def stop(self):

       	s = struct.pack('>L',Common.Cmds.stop)

	try:
	    self.sock.sendto(s,(self.target,DispGbl.DispPort.port))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return 'No'

	    stat = self.sock.recv(4)

	    return 'OK'

	except socket.error:
	    return 'No'

    def align(self):
	if (target 
