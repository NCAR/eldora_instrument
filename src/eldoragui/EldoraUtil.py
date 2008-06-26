from PyQt4.QtCore import *

# 
# DiskStats will privide disk statistics
#
class DiskStats:
    def __init__(self):
        stats = self.stats()
        self.num = len(stats)
        
    def stats(self):
        cmd = ['df', '-l', '-h']
        p = QProcess()
        p.start(cmd[0], cmd[1:])
        p.waitForFinished()
        # read all of the output
        dfout = ''
        while 1:
            line = p.readLine()
            if not line:
                break
            dfout = dfout + str(line)
        dfout = dfout.split()
        dfout = dfout[7:]
        result = []
        for i in range(0, len(dfout), 6):
            percent = dfout[i+4]
            percent = percent.strip('%')
            prcnt = int(percent)
            result.append((dfout[i+5],prcnt ))
        return result
    
    def numDisks(self):
        return self.num
       
def RadarFreqs(header):
	''' Extract the forward and aft radar frequencies from the header
	param header - An EldoraHeader
	returns - A dictionary containing the radar frequencies, in GHz:
	   'forward': [f1,f2,f3,f4,f5]
	   'aft': [f1,f2,f3,f4,f5]
	'''
	result = dict()
	# go through the blocks and find the radd blocks
	radarChoice = None
	for block in header:
		# examine the RADD blocks
		if block.type == 'RADD':
			freqs = list(range(0,5))
			# go through all of the fields. Find the
			# NAME field so that we can select forward or aft
			# Find the FREQn fields and extract the frequencies.
			for field in block:
				if field[0] == 'NAME':
					if field[2] == 'FORE':
						radarChoice = 'forward'
					else:
						radarChoice = 'aft'
				if field[0] == 'FREQ1':
					freqs[0] = field[2]
				elif field[0] == 'FREQ2':
					freqs[1] = field[2]
				elif field[0] == 'FREQ3':
					freqs[2] = field[2]
				elif field[0] == 'FREQ4':
					freqs[3] = field[2]
				elif field[0] == 'FREQ5':
					freqs[4] = field[2]
			# save the frquencies for the selected radar
			result[radarChoice] = freqs
				
	return result

	
