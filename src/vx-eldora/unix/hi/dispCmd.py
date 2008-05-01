#!/local/bin/python --
#
# Module:          dispCmd.py
# Original Author: Rich Neitzel
# Date:            $Date$
# Copywrited by the National Center for Atmospheric Research
# Tk based GUI for ELDORA displays.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
#

import socket
import select
import struct
import DispGbl
import Common

class DispCmd:
    def __init__(self,target):
		
	self.target = target
	self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	self.sock.bind('',0)
	self.funcdict1 = {
	    'type': 0,
	    'radar': 0,
	    'nparams': 0,
	    'p1': 0,
	    'p2': 0,
	    'p3': 0,
	    'side': 0,
	    'radius': 0,
	    'tmo': 0,
	    'colortbl': 0,
	    'max1': 0.0,
	    'max2': 0.0,
	    'max3': 0.0,
	    'min1': 0.0,
	    'min2': 0.0,
	    'min3': 0.0,
	    'distance': 0.0,
	    'size': 0.0,
	    'top': 0.0,
	    'bottom': 0.0,
	    }
	self.funcdict2 = {
	    'type': 0,
	    'radar': 0,
	    'nparams': 0,
	    'p1': 0,
	    'p2': 0,
	    'p3': 0,
	    'side': 0,
	    'radius': 0,
	    'tmo': 0,
	    'colortbl': 0,
	    'max1': 0.0,
	    'max2': 0.0,
	    'max3': 0.0,
	    'min1': 0.0,
	    'min2': 0.0,
	    'min3': 0.0,
	    'distance': 0.0,
	    'size': 0.0,
	    'top': 0.0,
	    'bottom': 0.0,
	    }

    def send(self,cmd):
	c1 = 0

	if self.funcdict1['type'] == DispGbl.DispType.radial:
	    if self.funcdict1['radar'] == DispGbl.RadarType.fore:
		c1 = DispGbl.RealCmds.forward_radial
	    else:
		c1 = DispGbl.RealCmds.aft_radial
	elif self.funcdict1['type'] == DispGbl.DispType.vertical:
	    if self.funcdict1['radar'] == DispGbl.RadarType.fore:
		c1 = DispGbl.RealCmds.forward_vert
	    else:
		c1 = DispGbl.RealCmds.aft_vert
	elif self.funcdict1['type'] == DispGbl.DispType.dual:
	    if self.funcdict1['radar'] == DispGbl.RadarType.fore:
		c1 = DispGbl.RealCmds.forward_dual
	    else:
		c1 = DispGbl.RealCmds.aft_dual
	elif self.funcdict1['type'] == DispGbl.DispType.raw:
	    if self.funcdict1['radar'] == DispGbl.RadarType.fore:
		c1 = DispGbl.RealCmds.forward_raw
	    else:
		c1 = DispGbl.RealCmds.aft_raw
	elif self.funcdict1['type'] == DispGbl.DispType.horiz:
	    if self.funcdict1['radar'] == DispGbl.RadarType.fore:
		c1 = DispGbl.RealCmds.forward_horiz
	    else:
		c1 = DispGbl.RealCmds.aft_horiz

	if cmd == DispGbl.RealCmds.load_only:
	    c1 = cmd | c1

	dist1 = self.funcdict1['distance'] * 1000.0
	size1 = self.funcdict1['size'] * 1000.0
	top1 = self.funcdict1['top'] * 1000.0
	bottom1 = self.funcdict1['bottom'] * 1000.0

	c2 = 0

	if self.funcdict2['type'] == DispGbl.DispType.radial:
	    if self.funcdict2['radar'] == DispGbl.RadarType.fore:
		c2 = DispGbl.RealCmds.forward_radial
	    else:
		c2 = DispGbl.RealCmds.aft_radial
	elif self.funcdict2['type'] == DispGbl.DispType.vertical:
	    if self.funcdict2['radar'] == DispGbl.RadarType.fore:
		c2 = DispGbl.RealCmds.forward_vert
	    else:
		c2 = DispGbl.RealCmds.aft_vert
	elif self.funcdict2['type'] == DispGbl.DispType.dual:
	    if self.funcdict2['radar'] == DispGbl.RadarType.fore:
		c2 = DispGbl.RealCmds.forward_dual
	    else:
		c2 = DispGbl.RealCmds.aft_dual
	elif self.funcdict2['type'] == DispGbl.DispType.raw:
	    if self.funcdict2['radar'] == DispGbl.RadarType.fore:
		c2 = DispGbl.RealCmds.forward_raw
	    else:
		c2 = DispGbl.RealCmds.aft_raw
	elif self.funcdict2['type'] == DispGbl.DispType.horiz:
	    if self.funcdict2['radar'] == DispGbl.RadarType.fore:
		c2 = DispGbl.RealCmds.forward_horiz
	    else:
		c2 = DispGbl.RealCmds.aft_horiz

	dist2 = self.funcdict2['distance'] * 1000.0
	size2 = self.funcdict2['size'] * 1000.0
	top2 = self.funcdict2['top'] * 1000.0
	bottom2 = self.funcdict2['bottom'] * 1000.0

	s = struct.pack('>IIIIIIIIIddddddddddIIIIIIIIIdddddddddd',c,
                        self.funcdict1['nparams'],
			self.funcdict1['p1'],self.funcdict1['p2'],
			self.funcdict1['p3'],self.funcdict1['side'],
			self.funcdict1['radius'],self.funcdict1['tmo'],
#			self.funcdict1['colortbl'],self.funcdict1['max1'],
			0,self.funcdict1['max1'],
			self.funcdict1['max2'],self.funcdict1['max3'],
			self.funcdict1['min1'],self.funcdict1['min2'],
			self.funcdict1['min3'],dist1,size1,top1,bottom1,
                        self.funcdict2['nparams'],
			self.funcdict2['p1'],self.funcdict2['p2'],
			self.funcdict2['p3'],self.funcdict2['side'],
			self.funcdict2['radius'],self.funcdict2['tmo'],
#			self.funcdict2['colortbl'],self.funcdict2['max1'],
			0,self.funcdict2['max1'],
			self.funcdict2['max2'],self.funcdict2['max3'],
			self.funcdict2['min1'],self.funcdict2['min2'],
			self.funcdict2['min3'],dist2,size2,top2,bottom2)
			
	try:
	    self.sock.sendto(s,(self.target,Common.Port.cmdport))
	    i,o,e = select.select([self.sock],[],[],10.0)
	    
	    if i == []:
		return 1

	    stat = self.sock.recv(4)

	except socket.error:
	    return 1

    def status(self):
	return self.send(Common.Cmds.status)

    def set(self,dict1,dict2):
	for i in dict1.keys():
	    if i != 'colortbl':
		self.funcdict1[i] = dict1[i]
                self.funcdict2[i] = dict2[i]
				
