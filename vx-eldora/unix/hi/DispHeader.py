#!/local/bin/python --
#
# Module:          DispHeader.py
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

import os


class DispHeader(Toplevel):
	basecmd = '/scr/ymir/udp/hi/disphead ' #'/vxbin/unix/disphead '

	def __init__(self, master = None, cnf = {}):
		Toplevel.__init__(self,master,cnf)
		#
		# Build layout.
		self.top = Frame(self)
		self.top['relief'] = RAISED
		self.top['bd'] = 2
		self.top.pack(side=TOP, fill=BOTH)

		self.bottom = Frame(self)
		self.bottom['relief'] = RAISED
		self.bottom['bd'] = 2
		self.bottom.pack(side=TOP, fill=BOTH)

		# Text area for display of header.
		self.text = Text(self.top)
		self.textvsb = Scrollbar(self.top)
		self.textvsb['relief'] = FLAT
		self.textvsb.pack(side=LEFT, fill=Y, expand=YES)
		self.texthsb = Scrollbar(self.top)
		self.texthsb['relief'] = FLAT
		self.texthsb['orient'] = HORIZONTAL
		self.texthsb.pack(side=BOTTOM, fill=X, expand=YES)
		self.text['yscrollcommand'] = (self.textvsb, 'set')
		self.text['xscrollcommand'] = (self.texthsb, 'set')
		self.text['setgrid'] = 1
		self.textvsb['command'] = (self.text, 'yview')
		self.texthsb['command'] = (self.text, 'xview')
		self.text.pack(side=TOP, expand=YES, fill=BOTH)

		# Done button
		self.done = Button(self.bottom,text='Done',command=self.donecb)
		self.done.pack(side=TOP)
		self.popdown()

	def donecb(self):
		self.popdown()

	def popdown(self):
		self.withdraw()

	def popup(self,file):
		self.text.delete(1.0,'end')
		cmd = self.basecmd + file
		pfd = os.popen(cmd,'r')
		l = pfd.readlines()
		for i in l:
			self.text.insert('end',i)
		self.deiconify()


def go():
	import sys

	i = len(sys.argv)
	
	if i < 2:
		print 'Need file name'
		sys.exit(1)

	d = DispHeader()

	d.popup(sys.argv[1])

	d.mainloop()

        
if __name__ == '__main__':
        go()

