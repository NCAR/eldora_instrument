from EmitterProc import *
from EldoraHeader import *
from EldoraUtil import *

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
