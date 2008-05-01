#!/local/bin/python --
#
# Module:          TsGate.py
# Original Author: Rich Neitzel
# Date:            $Date$
# Copywrited by the National Center for Atmospheric Research
# Tk based GUI for ELDORA displays.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 1.1  1996/09/10  14:55:28  thor
# Initial revision
#
#

from Tkinter import *
from EmacsEntry import *

class TsGate(Toplevel):
	def __init__(self, master, cnf = {}):
		Toplevel.__init__(self,master,cnf)
                #
                # Build the layout items.
		self.top = Frame(self, {
                          'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
		self.bottom = Frame(self,{
                          'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})

		self.tsgate = DoubleVar(self.top)
		self.tsgate.set(0.0)

		self.label = Label(self.top,{
                          'text': 'Kilometers:',
                          Pack: {
                          'side': 'left',
                          'expand': '1',
                          'fill': 'x'
                          }})

		self.entry = EmacsEntry(self.top,{
                          'relief': 'sunken',
                          'width': '6',
                          'textvariable': self.tsgate.__str__(),
                          Pack: {
                          'side': 'left',
                          'pady': '2m'
                          }})

		cb = self.register(self.callback)
		self.okbutton = Button(self.bottom,{
			  'text': 'Apply',
			  'command' : (cb, 'ok'),
			  Pack: {
			  'side': 'left'
			  }})
		self.cancelbuttom = Button(self.bottom,{
			  'text': 'Cancel',
			  'command' : (cb, 'cancel'),
			  Pack: {
			  'side': 'left'
			  }})

		self.withdraw()

	def callback(self,caller):
		if caller == 'ok':
			if self.cmdobj.tsGate(self.get.tsgate()):
				self.badComm()
		self.withdraw()

	def popup(self):
		self.deiconify()

	def object(self,obj):
		self.cmdobj = obj

	#
	# In case rpc fails.
	def badComm(self):
		Dialog(self,{
                          'title': 'Warning!',
                          'text': 'Communication to radar processors failed.',
                          'bitmap': 'warning',
                          'default': 0,
                          'strings': ('Noted',)
                          })
