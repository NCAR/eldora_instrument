from EmitterProc   import *
from EldoraHeader  import *
from EldoraUtil    import *
from PyQt4.QtCore  import *

####################################################################################
####################################################################################
class DDS:
	''' Manage the direct digital synthesizers for a single radar.
	A DDS programming application will be run in order to 
	program the syntehsizer. The frequency parameters are
	extracted from a supplied header.
	'''
	def __init__(self, ip, port, radar, ddsProgram, textFunction=None, verbose=False):
		''' Constructor
		ip (string) - the ip address of the dds controller
		port (string) - the port number 
		radar (string) - the radar choice, either 'forward' or 'aft'
		ddsProgram (string) - path to the dds programmig program.
		textFunction (function) - function to call to log text. The function
		   signature is (text, paylod)
		'''
		self.ip = ip
		self.port = str(port)
		self.radar = radar
		self.ddsapp = ddsProgram
		self.verbose = verbose
		self.textFunction = textFunction
 		
####################################################################################
	def runDDS(self, chan, freq):
		''' Run the progdds command.
		radar (string) - either forward or aft
		chan (int) - the channel
		freq (string) - the frequency in ghz
		'''
		cmd = [self.ddsapp,]
		cmd.append('--ip')
		cmd.append(self.ip)
		cmd.append('--port')
		cmd.append(self.port)
		cmd.append('--radar')
		cmd.append(self.radar)
		cmd.append('--chan')
		cmd.append(str(chan))
		cmd.append('--freq')
		cmd.append(freq)
		p = EmitterProc(cmd, emitText=False, 
					payload='red',verbose=self.verbose)
		if self.textFunction != None:
			QObject.connect(p, SIGNAL("text"), self.textFunction)
		p.startDetached()
	    
####################################################################################	
	def programDDS(self, header):
		''' Program the dds with frequencies from the header.
		header - the selected header
		'''
		# get the frequencies from the header. They are
		# returned as a diction of lists. The dictionary 
		# keys are 'forward' and 'aft'
		freqs = RadarFreqs(header)
		for i in range(0, 4):
			self.runDDS(i, freqs[self.radar][i])
			
####################################################################################
####################################################################################
class SA:
	''' Manage the stepped attenuators  (SA)for both radars.
	A SA programming application will be run in order to 
	program them.
	'''
	def __init__(self, ip, port, saProgram, textFunction=None, verbose=False):
		''' Constructor
		ip (string) - the ip address of the dds controller
		port (string) - the port number 
		saProgram (string) - path to the sa programming program.
		textFunction (function) - function to call to log text. The function
		   signature is (text, paylod)
		'''
		self.ip = ip
		self.port = str(port)
		self.saapp = saProgram
		self.verbose = verbose
		self.textFunction = textFunction
				
####################################################################################
	def programSA(self, radar, db):
		''' Run the sa programming command.
		radar (string) - either forward or aft
		db (string) - the attenuation in db
		'''
		cmd = [self.saapp,]
		cmd.append('--ip')
		cmd.append(self.ip)
		cmd.append('--port')
		cmd.append(self.port)
		cmd.append('--radar')
		cmd.append(radar)
		cmd.append('--db')
		cmd.append(db)
		p = EmitterProc(cmd, emitText=False, 
					payload='red',verbose=self.verbose)
		if self.textFunction != None:
			QObject.connect(p, SIGNAL("text"), self.textFunction)
		p.startDetached()
		
####################################################################################
####################################################################################
class TestPulseControl(QObject):
	''' Assist with periodic programming of the dds, sa and mux
	for the test pulse generation for both radars.
	The operational characteristics are specified in the constructor.
	
	Periodically, the forward and aft test pulse generators are 
	reconfigured. At each update, the test pulse frequency is changed.
	An associated attenuation is set at the same time. The multiplexer
	is also reprogrammed in order to ... what?.
	
	The start() function is called in order to start the periodic
	programming.
	
	The stop() functon is used to halt it.
	
	At the end of each programming cycle, a specified callback function 
	will be called. The callback function could be used, for example, 
	to update the housekeeper with the current operating characteristics.
	
	TestPulseControl is derived from QObject so that we can use the
	QTimer. It must be created in a Qt context in order to have 
	the Qt event loop available for the timer.
	'''
	def __init__(self, testpulseapp, ipddsfor, ipddsaft, ipsamux, 
				port, periodSecs, header, atten, fOffGhz, parent, 
				textFunction=None, verbose=False, callback=None):
		''' Constructor
		testPulseApp (string) - the test pulse programming application
		ipddsfor (string) - ip address of the forward dds
		ipddsaft (string) - ip address of the aft dds
		ipsamux (string) - ip address of the sa/mux
		port (int) - ip ort number (same for all devices)
		periodSecs - how often (secs) to run the programming cycle
		header - An EldoraHeader. Thebase frequencies will be drawn 
		         from here.
		atten - a dictionary ['forward','aft'] of lists of attenuation values.
		        Each list contains integer values, one per frequency.
		        There must be 4 attenuations per radar, to match the 
		        4 transmit frequencies
		fOffGhz (double) - The frequncy offset to be added to each 
		        frequency specified in the header.
		parent - A QObject, which gives us access to the Qt event loop
		textFunction - A text function passed on to EmitterProc.
		verbose - Set true for diagnostic output.
		callback - Function to call when the test pulse is reconfigured.
		         Call it as: callback(params), where params is a 
		         dictionary('forward','aft'). Each entry in the dictionary
		         is another dictionary containing test pulse parameters.
		'''
		super(QObject, self).__init__(parent)
		
		self.freqs = RadarFreqs(header)
		self.atten = atten
		self.ip = dict()
		self.ip['forward'] = ipddsfor
		self.ip['aft'] = ipddsaft
		self.textFunction = textFunction
		self.verbose = verbose
		self.callback = callback
		
		# add the offset to the frequencies
		for radar in self.freqs:
			for i in range(0, len(self.freqs[radar])):
						fOrig = self.freqs[radar][i]
						fNew = str(float(fOrig)+fOffGhz)
						self.freqs[radar][i] = fNew
						
		# index through the values
		self.index = 0
		
		# create the first part of the command
		self.cmd = list()
		self.cmd.append(testpulseapp)
		self.cmd.append('--ipsamux')
		self.cmd.append(ipsamux)
		self.cmd.append('--port')
		self.cmd.append(str(port))

		# create the timer, but don't start it		
		self.timer = QTimer(self)
		self.timer.setInterval(periodSecs*1000)
		self.connect(self.timer, SIGNAL('timeout()'), self.timeout)


####################################################################################
	def start(self):
		''' Start the timer. Also perform the initial programming
		at the start.
		'''
		# start the timer
		self.timer.start()
	
####################################################################################
	def stop(self):
		self.timer.stop()

####################################################################################
	def programTestPulse(self, radar, muxchan, freq, db):
		''' Run the sa programming command.
		radar (string) - either forward or aft
		db (string) - the attenuation in db
		'''
		# add parameters to the end of the skeleton command
		cmd = list()
		cmd = self.cmd + cmd
		cmd.append('--ipdds')
		cmd.append(self.ip[radar])
		cmd.append('--radar')
		cmd.append(radar)
		cmd.append('--muxchan')
		cmd.append(muxchan)
		cmd.append('--freq')
		cmd.append(freq)
		cmd.append('--db')
		cmd.append(db)
		
		p = EmitterProc(cmd, emitText=False, 
					payload='red',verbose=self.verbose)
		if self.textFunction != None:
			QObject.connect(p, SIGNAL("text"), self.textFunction)
		p.startDetached()

####################################################################################
	def timeout(self):
		callbackParams = dict()
		callbackParams['forward'] = dict()
		callbackParams['aft'] = dict()
		for radar in ('forward','aft'):
			db = self.atten[radar][self.index]
			freq = self.freqs[radar][self.index]
			self.programTestPulse(radar, str(self.index),freq, db)
			if self.callback != None:
				callbackParams[radar]['db'] = float(self.atten[radar][self.index])
				callbackParams[radar]['freq'] = float(self.freqs[radar][self.index])			
			
		# execute the callback
		if self.callback != None:
			self.callback(callbackParams)
			
		# bump to the next set.
		self.index = self.index + 1
		if self.index == 4:
			self.index = 0