#!/local/bin/python --
import time
import serial

###################################################################
class Hpa:
	def __init__(self, portname):
		''' This class sends control info to the selected
		HPA & receives status info back. 
		
		The antenna/dummy load switch is slow, so if one 
		asks for status right away it will still indicate 
		the old position. A delay of ~2 seconds seems to be
		enough for the switch to actually move.
		'''
		
		if portname != None:
			# the open may raise SerialException if it fails
			self.serialport = serial.Serial(port=portname, 
								    bytesize=serial.EIGHTBITS, 
								    parity=serial.PARITY_NONE, 
								    stopbits=serial.STOPBITS_ONE, 
								    timeout=1, 
								    xonxoff=0, 
								    rtscts=0)
		else:
			self.serialport = None
			
		self.faulted = 0
		self.faults = 0

		self.current_status = HPA_STATES.off
			
		self.cmd_dict = { HPA.on : '\002\120\003\055', 
			  HPA.off : '\002\160\003\015', 
			  HPA.operate : '\002\117\003\056', 
			  HPA.standby : '\002\123\003\052', 
			  HPA.antenna : '\002\101\003\074', 
			  HPA.dummyload : '\002\114\003\061', 
			  HPA.status : '\002\127\003\046' }
		
		self.cmdNames = { HPA.on : 'On', 
			  HPA.off : 'Off', 
			  HPA.operate : 'Operate', 
			  HPA.standby : 'Standby', 
			  HPA.antenna : 'Antenna', 
			  HPA.dummyload : 'Dummy', 
			  HPA.status : 'Status' }
		
		self.stateDescriptions = {
			  HPA_STATES.off: 'Power off', 
			  HPA_STATES.on: 'Power on', 
			  HPA_STATES.cooldown: 'Cooldown', 
			  HPA_STATES.warmup: 'Warmup', 
			  HPA_STATES.standby: 'Standby', 
			  HPA_STATES.operate: 'Operate'
		  }

###################################################################
	def __del__(self):
		pass
	
###################################################################
	def status(self):
		print 'current_status is ',self.stateDescriptions[self.current_status]
		return self.stateDescriptions[self.current_status]

###################################################################
	def command(self, op):
		''' Send a command to the hpa.
		Return a result message if there was a problem. If
		no proble, return an empty string.
		'''
		if self.serialport is None:
			return 'No serial port specified. Unable to send ' + self.cmdNames[op] + ' command'
		
		print 'send', self.cmdNames[op], 'command to', self.serialport.port
		
		writeStatus = self.serialport.write(self.cmd_dict[op])
		if  writeStatus != None:
			return 'Write to the serial port did not succeed. Unable to send ' + self.cmdNames[op] + ' command'

		if op != HPA.status:
			if (op == HPA.antenna) or (op == HPA.dummyload):
				time.sleep(2)
			self.serialport.write(self.cmd_dict[HPA.status])

		s = ''
		while True:
			s = s + self.serialport.read(1)
			if len(s) == 0:
				return 'HPA read failed'
			if (len(s)) > 10:
				for c in s:
					print hex(ord(c)), ' ',
				print ' '
				return 'Missing EOT character on HPA read'
			if s[len(s) - 1] == '\003':
		  		break
			
		s = s + self.serialport.read(1)
		
		if len(s) < 5:
			return 'Read after write only returned '+len(s)+' bytes, expecting at least 5'

		as = s[1:5] # We want the 4 status bytes.

		for c in s:
			print hex(ord(c)),' ',
		print 
		self.update_status(as)

		return ''

###################################################################
	def update_status(self, stat):
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

###################################################################			
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



