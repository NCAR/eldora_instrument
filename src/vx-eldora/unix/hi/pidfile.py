#!/local/bin/python --
#
# Module:          pidfile.py
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

import posix
import string

def create(file):
	f = open(file,"w")
	f.write(`posix.getpid()`)
	f.close()

def kill(file,signal):
	try:
		f = open(file,'r')
		s = f.read()
		posix.kill(string.atoi(s),signal)
	except:
		pass			# Ignore all errors.

