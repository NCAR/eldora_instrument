#!/local/bin/python --
#
# Module:          Hpa.py
# Original Author: Rich Neitzel
# Date:            $Date$
# Copywrited by the National Center for Atmospheric Research
# Tk based GUI for ELDORA displays.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 1.1  1996/04/08  21:47:51  thor
# Initial revision
#
#
#   This implements 2 simple classes that are nothing but constants &
# the 'real' Hpa class. The latter sends control info to the selected
# HPA & receives status info back. Two nits to be aware of: first, the
# serial line does NOT need a null modem; second, the anteena/dummy
# load switch is slow, so if one asks for status right away it will
# still indicate the old position. A delay of ~2 seconds seems to be
# enough for the switch to actually move.
#

from Serialline import *
import time

class HPA:
	off = 0
	on = 1
	operate = 2
	standby = 3
	antenna = 4
	dummyload = 5 
	status = 6
	warmup = 7
	cooldown = 8

	badWrite = 1
	badRead = 2

class HPA_STATES:
	off = 0
	on = 1
	cooldown = 2
	warmup = 4
	standby = 8
	operate = 0x10
	faulted = 0x20

	filament = 0x20
	pulse = 0x10
	hvps = 8
	twt = 4
	rev_power = 2
	interlock = 1

	byte2 = filament | pulse | hvps | twt | rev_power | interlock

	low_press = 0x10
	
	byte3 = low_press

	dummyload = 2
	antenna = 1

	position = dummyload | antenna

	crowbar = 4
	hotrun = 2
	rf_arc = 1

	byte4 = crowbar | hotrun | rf_arc


class Hpa:
	def __init__(self,portname):
		self.serialport = Serialline(portname)
		self.cmd_dict = { HPA.on : '\002\120\003\055',
			  HPA.off : '\002\160\003\015',
			  HPA.operate : '\002\117\003\056',
			  HPA.standby : '\002\123\003\052',
			  HPA.antenna : '\002\101\003\074',
			  HPA.dummyload : '\002\114\003\061',
			  HPA.status : '\002\127\003\046' }
		self.state_dict = {
			  HPA_STATES.off: 'Power off',
			  HPA_STATES.on: 'Power on',
			  HPA_STATES.cooldown: 'Cooldown',
			  HPA_STATES.warmup: 'Warmup',
			  HPA_STATES.standby: 'Standby',
			  HPA_STATES.operate: 'Operate'
		  }

	def __del__(self):
		del self.serialport

	def docommand(self,op):
		if self.serialport.write(self.cmd_dict[op]) != 4:
			return badWrite

		if op != HPA.status:
			if (op == HPA.antenna) or (op == HPA.dummyload):
				time.sleep(2)
			self.serialport.write(self.cmd_dict[HPA.status])

		# Yes, this would be efficent if we just read n bytes,
		# but upgrades to the HPAs could change this number.
		s = self.serialport.read(1)

		while s[len(s) - 1] != '\003':
			s = s + self.serialport.read(1)
			
		s = s + self.serialport.read(1)

		as = s[1:5] # We want the 4 status bytes.

		self.update_status(as)

		return 0

	def update_status(self,stat):
		self.faulted = 0
		self.faults = 0

		state = ord(stat[0])

		if (state & HPA_STATES.faulted):
			self.faulted = 1
		if (state & HPA_STATES.operate):
			self.current_status = HPA_STATES.operate
		elif (state & HPA_STATES.standby):
			self.current_status = HPA_STATES.standby
		elif (state & HPA_STATES.warmup):
			self.current_status = HPA_STATES.warmup
		elif (state & HPA_STATES.cooldown):
			self.current_status = HPA_STATES.cooldown
		elif (state & HPA_STATES.on):
			self.current_status = HPA_STATES.on
		else:
			self.current_status = HPA_STATES.off

		state = ord(stat[1])
		state = state & HPA_STATES.byte2
		if (state):
			self.faulted = 1
			self.faults = self.faults | (state << 16)

		state = ord(stat[2])
		self.load_pos = state & HPA_STATES.position
		state = state & HPA_STATES.byte3
		if (state):
			self.faulted = 1
			self.faults = self.faults | (state << 8)
			
		state = ord(stat[3])
		state = state & HPA_STATES.byte4
		if (state):
			self.faulted = 1
			self.faults = self.faults | state

