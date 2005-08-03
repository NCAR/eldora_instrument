#
# This file should track the C++/C file DisplayStruct.h.
#
#

import struct

class DispStruct:
    def __init__(self,v):
	self.cmd = v
	self.numParams = 0
	self.param0 = 0
	self.param1 = 0
	self.param2 = 0
	self.side = 0
	self.radius = 0
	self.tmo = 0
	self.userColors = 0
	self.max0 = 0.0
	self.max1 = 0.0
	self.max2 = 0.0
	self.min0 = 0.0
	self.min1 = 0.0
	self.min2 = 0.0
	self.distance = 0.0
	self.size = 0.0
	self.top = 0.0
	self.bottom = 0.0

    def setCmd(self,v):
	self.cmd = v

    def setNumparams(self,v):
	self.numParams = v

    def setParam0(self,v):
	self.param0 = v

    def setParam1(self,v):
	self.param1 = v

    def setParam2(self,v):
	self.param2 = v

    def setSide(self,v):
	self.side = v

    def setRadius(self,v):
	self.radius = v

    def setTmo(self,v):
	self.tmo = v

    def setUsercolors(self,v):
	self.userColors = v

    def setMax0(self,v):
	self.max0 = v
    
    def setMax1(self,v):
	self.max1 = v
    
    def setMax2(self,v):
	self.max2 = v
	
    def setMin0(self,v):
	self.min0 = v

    def setMin1(self,v):
	self.min1 = v

    def setMin2(self,v):
	self.min2 = v

    def setDistance(self,v):
	self.distance = v

    def setSize(self,v):
	self.size = v

    def setTop(self,v):
	self.top = v

    def setBottom(self,v):
	self.bottom = v

    def getString(self):
	string = struct.pack('>LLLLLLLLLdddddddddd',self.cmd,self.numParams,
			     self.param0,self.param1,self.param2,self.side,
			     self.radius,self.tmo,self.userColors,self.max0,
			     self.max1,self.max2,self.min0,self.min1,
			     self.min2,self.distance,self.size,self.top,
			     self.bottom)
	return string
