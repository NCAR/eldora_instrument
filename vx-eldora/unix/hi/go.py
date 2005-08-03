#!/local/bin/python --
#
# Module:          go.py
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

from Tkinter import *
from Dialog import *
from EmacsEntry import *

import posix

class gui(Tk):
	def __init__(self, file, name = 'ELDORA Startup'):
		self.cleanup()
		Tk.__init__(self,None,None,'Startup')

		self.base = Frame(self)

		self.base.pack(side=TOP, fill=BOTH, expand=YES)

		self.dispcount = IntVar(self.base)
		self.dispframe = Frame(self.base)
		self.displabel = Label(self.dispframe,
			  text='Number of Displays:')
		self.displabel.pack(side=LEFT)

		self.onedisp = Radiobutton(self.dispframe,text='1',
			  value=1,variable=self.dispcount.__str__())
		self.twodisp = Radiobutton(self.dispframe,text='2',
			  value=2,variable=self.dispcount.__str__())

		self.onedisp['relief']=FLAT
		self.onedisp.pack(side=LEFT)
		self.twodisp['relief']=FLAT
		self.twodisp.pack(side=LEFT)

		self.dispframe.pack(side=TOP,fill=BOTH)

		self.hframe = Frame(self.base)

		self.hlabel = Label(self.hframe,text='Header file:')
		self.header = EmacsEntry(self.hframe)
		self.headername = StringVar(self.base)
		self.header['relief'] = FLAT
		self.header['width'] = 60
		self.header['textvariable'] = self.headername.__str__()
		self.hlabel.pack(side=LEFT)
		self.header.pack(side=LEFT)
		self.hframe.pack(side=TOP)

		self.fframe = Frame(self.base)
		self.flabel = Label(self.fframe,text='Flight info:')
		self.flight = EmacsEntry(self.fframe)
		self.flightinfo = StringVar(self.base)
		self.flight['relief'] = FLAT
		self.flight['width'] = 8
		self.flight['textvariable'] = self.flightinfo.__str__()
		self.flabel.pack(side=LEFT)
		self.flight.pack(side=LEFT)
		self.fframe.pack(side=TOP,fill=BOTH)

		self.cmdframe = Frame(self.base)

		self.doit = Button(self.cmdframe,text='Continue startup',
			  command=self.startup)
		self.quit = Button(self.cmdframe,text='Exit',
			  command=self.exit)

		self.doit.pack(side=LEFT)
		self.quit.pack(side=LEFT)

		self.cmdframe.pack(side=TOP)

		self.dispcount.set(2)
		self.headername.set(file)
		self.flightinfo.set('FLT1')

	def startup(self):		
		import time
		time.sleep(5)
		self.fork_master()
		self.fork_disps()
		self.fork_hskp()
		self.fork_record()
		self.fork_radar()
		self.exit()

	def cleanup(self):
		import pidfile
		l = posix.listdir('.')
		for i in l:
			if i[-3:] == 'pid':
				pidfile.kill(i,9)

	def exit(self):
		self.destroy()

	def fork_master(self):
		args = ('master.py','-f')
		args = args + (self.flightinfo.get(),)
		if self.dispcount.get() == 1:
			args = args + ('-o',)
		args = args + ('-h',)
		args = args + (self.headername.get(),)

		pid = posix.fork()
		if pid == 0:
			posix.execv('./master.py',args)

	def fork_disps(self):
		args1 = ('display.py','-t','disp1')
		pid = posix.fork()
		if pid == 0:
			posix.execv('./display.py',args1)

		if self.dispcount.get() == 2:
			args2 = ('display.py','-t','disp2')
			pid = posix.fork()
			if pid == 0:
				posix.execv('./display.py',args2)

	def fork_hskp(self):
		pid = posix.fork()
		if pid == 0:
			posix.execv('./hskp.py',('hskp.py','-s','60'))

	def fork_record(self):
		pid = posix.fork()
		if pid == 0:
			posix.execv('./record.py',('record.py','-s','60'))

	def fork_radar(self):
		pid = posix.fork()
		if pid == 0:
			posix.execv('./radar.py',('radar.py','-s','60'))

if __name__ == '__main__':
    file = '/usr/local/vxbin/radar/headers/default.hd'
    import sys, getopt, string

    i = len(sys.argv)
    opts, args = getopt.getopt(sys.argv[1:],'f:')

    for i in range(len(opts)):
	tmp = opts[i]
	if tmp[0] == '-f':
	    file = tmp[1]

    obj = gui(file)
    obj.mainloop()
