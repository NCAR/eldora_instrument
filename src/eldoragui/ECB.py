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
	def __init__(self, dds, sa, periodSecs, header, atten, fOffGhz, parent):
		''' Constructor
		dds - a dictionary['forward', 'aft'] of DDS controlers
		sa - an SA controller
		periodSecs - how often (secs) to run the programming cycle
		header - An EldoraHeader. Thebase frequencies will be drawn 
		         from here.
		atten - a dictionary ['forward','aft'] of lists of attenuation values.
		        Each list contains integer values, one per frequency.
		        There must be 4 attenuations per radar, to match the 
		        4 transmit frequencies
		fOffGhz (double) - The frequncy offset to be added to each 
		        frequency specified in the header.
		'''
		super(QObject, self).__init__(parent)
		
		self.dds = dds
		self.sa = sa
		self.freqs = RadarFreqs(header)
		self.atten = atten
		
		# add the offset to the frequencies
		for radar in self.freqs:
			for i in range(0, len(self.freqs[radar])):
						fOrig = self.freqs[radar][i]
						fNew = str(float(fOrig)+fOffGhz)
						self.freqs[radar][i] = fNew
						
		# index through the values
		self.index = 0

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
	def timeout(self):
		for radar in ('forward','aft'):
			# set the attenuator
			db = str(self.atten[radar][self.index])
			self.sa.programSA(radar, db)
			# set the frequency
			freq = str(self.freqs[radar][self.index])
			self.dds[radar].runDDS(5, freq)
			
		# bump to the next set.
		self.index = self.index + 1
		if self.index == 4:
			self.index = 0