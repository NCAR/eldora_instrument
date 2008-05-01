#!/local/bin/python
#
# Module:          RecordStatus.py
# Original Author: Richard Neitzel
# Date:            $Date$
# 
# Status window for recording processor.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.2  1995/01/23  20:50:54  thor
# Changed tape unit numbers from 0-3 to 1-4.
#
# Revision 1.1  1994/09/02  21:08:16  thor
# Initial revision
#
#
from Tkinter import *

class RecordStatus(Toplevel):
        def __init__(self, master = None, cnf = {}):
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
                self.middle1 = Frame(self,{
                          'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
		self.middle2 = Frame(self,{
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
		#
		# Am I active?
		self.activelabel = Label(self.top,{
			  'text': 'Is this the current recording system:',
			  Pack: {
			  'side': 'left'
			  }})
		pcb = self.register(self.phonyCB)
		self.yes = Checkbutton(self.top,{
			  'text': 'Yes',
			  'relief': 'flat',
			  'command': (pcb,'a'),
			  Pack: {
			  'side': 'left'
			  }})
		self.no = Checkbutton(self.top,{
			  'text': 'No',
			  'relief': 'flat',
			  'command': (pcb,'a'),
			  Pack: {
			  'side': 'left'
			  }})
		self.yes.deselect()
		self.no.deselect()
		#
		# With what drives?
		self.drvlabel = Label(self.middle1,{
			  'text': 'Drives in use:',
			  Pack: {
			  'side': 'left'
			  }})
		self.drv0 = Checkbutton(self.middle1,{
			  'text': '1',
			  'relief': 'flat',
			  'command': (pcb,'a'),
			  Pack: {
			  'side': 'left'
			  }})
		self.drv1 = Checkbutton(self.middle1,{
			  'text': '2',
			  'relief': 'flat',
			  'command': (pcb,'a'),
			  Pack: {
			  'side': 'left'
			  }})
		self.drv2 = Checkbutton(self.middle1,{
			  'text': '3',
			  'relief': 'flat',
			  'command': (pcb,'a'),
			  Pack: {
			  'side': 'left'
			  }})
		self.drv3 = Checkbutton(self.middle1,{
			  'text': '4',
			  'relief': 'flat',
			  'command': (pcb,'a'),
			  Pack: {
			  'side': 'left'
			  }})
		self.drvlist = []
		self.drvlist.append(self.drv0)
		self.drvlist.append(self.drv1)
		self.drvlist.append(self.drv2)
		self.drvlist.append(self.drv3)

		for i in self.drvlist:
			i.deselect()
		#
		# Text display.
		self.statlabel = Message(self.middle2,{
			  'text': 'Status word: 0x00000000',
			  'justify': 'center',
			  'width': '8c',
			  Pack: {
			  'side': 'top',
			  'pady': '2m'
			  }})
		self.trylabel = Message(self.middle2,{
			  'text': 'Attempted writes: 0000000',
			  'justify': 'center',
			  'width': '8c',
			  Pack: {
			  'side': 'top',
			  'pady': '2m'
			  }})
		self.faillabel = Message(self.middle2,{
			  'text': 'Soft failures: 000000',
			  'justify': 'center',
			  'width': '8c',
			  Pack: {
			  'side': 'top',
			  'pady': '2m'
			  }})
		#
		# Close button.
		cb = self.register(self.doneCB)
		self.done = Button(self.bottom,{
			  'text': 'Close',
			  'command': (cb, 'd'),
			  'relief': 'sunken',
			  Pack: {
			  'side': 'top',
			  'pady': '2m'
			  }})
		#
		# Not visible until asked.
		self.withdraw()
		self.group(master.__str__())
	#
	# Callbacks.
	#
	def doneCB(self,j):
		self.withdraw()

	def phonyCB(self,j):		# In case users try to play around.
		self.set()
	#
	# Popup.
	#
	def popup(self):
		self.deiconify()
	#
	# Setting routines.
	#
	def setDict(self,dict):
		self.dict = dict

	def set(self):
		if self.dict['unit'] == self.unit:
			self.yes.select()
			self.no.deselect()
		else:
			self.yes.deselect()
			self.no.select()
		d0 = -1
		d1 = -1
		if self.unit == 0:
			if self.dict['numdrv'][0]:
				d0 = self.dict['drvs0'][0]
				if self.dict['numdrv'][0] > 1:
					d1 = self.dict['drvs0'][1]
		else:
			if self.dict['numdrv'][1]:
				d0 = self.dict['drvs1'][0]
				if self.dict['numdrv'][1] > 1:
					d1 = self.dict['drvs1'][1]
		for i in self.drvlist:
			i.deselect()

		index = int(d0)
		if index >= 1:
			index = index - 1
			self.drvlist[index].select()
		index = int(d1)
		if index >= 1:
			index = index - 1
			self.drvlist[index].select()
		t = str(self.dict['attempts'][self.unit])
		self.trylabel['text'] = 'Attempted writes: ' + t
		f = str(self.dict['failures'][self.unit])
		self.faillabel['text'] = 'Soft failures: ' + f
		s = 'Status word: 0x%x' %self.dict['status'][self.unit]
		self.statlabel['text'] = s

	def setUnit(self,unit):
		self.unit = unit

	def bitmap(self,bitmap):
		self.iconbitmap(bitmap)
