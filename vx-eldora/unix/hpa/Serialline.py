#!/local/bin/python --
#
# Module:          Serialline.py
# Original Author: Rich Neitzel
# Date:            $Date$
# Copywrited by the National Center for Atmospheric Research
# Tk based GUI for ELDORA displays.
#
# $Id$
# revision history
# ----------------
# $Log$
#
#   This class controls a serial line for comm to an HPA.

import os
import termios
import TERMIOS

class Serialline:
	def __init__(self,name):
		self.fd = os.open(name,2,0700)

		self.original = termios.tcgetattr(self.fd)

		tmp = []

		tmp.append(TERMIOS.IGNBRK | TERMIOS.IGNPAR)
		tmp.append(0)
		tmp.append(TERMIOS.B9600 | TERMIOS.CS8 | TERMIOS.CREAD |
			  TERMIOS.CLOCAL)
		tmp.append(TERMIOS.NOFLSH)
		tmp.append(TERMIOS.B0)
		tmp.append(TERMIOS.B9600)
		tmp.append(self.original[6])

		termios.tcsetattr(self.fd,TERMIOS.TCSANOW,tmp)
		
		
	def __del__(self):
		termios.tcsetattr(self.fd,TERMIOS.TCSANOW,self.original)
		self.fp.close()

	def read(self,bytes=1):
		return os.read(self.fd,bytes)

	def write(self,output):
		return os.write(self.fd,output)
		
	def tell(self):
		settings = termios.tcgetattr(self.fd)
		print '%o' %settings[0]
		print '%o' %settings[1]
		print '%o' %settings[2]
		print '%o' %settings[3]
		print settings[4]
		print settings[5]
		print settings[6]
