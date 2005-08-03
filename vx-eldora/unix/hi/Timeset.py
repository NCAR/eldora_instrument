#
# Module:          TimeSet.py
# Original Author: Richard Neitzel
# Date:            $Date$
#
# The window for setting the time on the housekeeper.
#
# revision history:
# -----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.2  1995/11/28  18:20:50  thor
# Changed to new IconWindow format.
#
# Revision 1.1  1994/09/02  21:08:16  thor
# Initial revision
#
#
from Tkinter import *
from EmacsEntry import *
from IconWindow import *
import string
import time


class TimeSet(Toplevel):
        def __init__(self,master):
		#
		Toplevel.__init__(self,master,{})
                #
		# Build the layout items.
                self.top = Frame(self, {
                          'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
                self.middle = Frame(self,{
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
		# Type buttons.
		self.warning = Label(self.top,{
			  'text':
			  'You must stop the system before statrting the clock!',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		#
		# Add time entries.
		self.left = Frame(self.middle,{
                          Pack: {
                          'side': 'left',
                          'fill': 'both',
			  'expand': 1
                          }})
                self.right = Frame(self.middle,{
                          Pack: {
                          'side': 'left',
                          'fill': 'both',
			  'expand': 1
                          }})
		self.hour = TimeEntry(self.right)
		self.hour.setLabel('Hour')
		self.minute = TimeEntry(self.right)
		self.minute.setLabel('Minute')
		self.second = TimeEntry(self.right)
		self.second.setLabel('Second')
		self.year = TimeEntry(self.left)
		self.year.setLabel('Year')
		self.month = TimeEntry(self.left)
		self.month.setLabel('Month')
		self.day = TimeEntry(self.left)
		self.day.setLabel('Day')
		#
		# Tie them together.
		self.year.setNext(self.month)
		self.month.setNext(self.day)
		self.day.setNext(self.hour)
		self.hour.setNext(self.minute)
		self.minute.setNext(self.second)
		self.second.setNext(self.hour)
		#
		# Action buttons.
		cb = self.register(self.actions)
		self.apply = Button(self.bottom,{
			  'text': 'Apply',
			  'command': (cb,'go'),
			  Pack: {
			  'side': 'left',
			  'padx': '50'
			  }})
		self.done = Button(self.bottom,{
			  'text': 'Done',
			  'command': (cb,'done'),
			  Pack: {
			  'side': 'left',
			  'padx': 50
			  }})
		#
		# Not visibile at first.
		self.withdraw()
		#
		# Save parent for apply.
		self.parent = master

	def actions(self,caller):
		self.withdraw()
		if caller == 'go':
			self.parent.apply('time')

	def popup(self):
		self.deiconify()
		yy,mm,dd,h,m,s,j,k,l = time.gmtime(time.time())
		self.dict['year'] = yy
		self.dict['month'] = mm
		self.dict['day'] = dd
		self.dict['hour'] = h
		self.dict['minute'] = m
		self.dict['second'] = s
		self.set()

	def set(self,dict = {}):
		if dict != {}:
			self.dict = dict
		self.year.setValue(self.dict['year'])
		self.month.setValue(self.dict['month'])
		self.day.setValue(self.dict['day'])
		self.hour.setValue(self.dict['hour'])
		self.minute.setValue(self.dict['minute'])
		self.second.setValue(self.dict['second'])

	def get(self):
		self.dict['year'] = self.year.getValue()
		self.dict['month'] = self.month.getValue()
		self.dict['day'] = self.day.getValue()
		self.dict['hour'] = self.hour.getValue()
		self.dict['minute'] = self.minute.getValue()
		self.dict['second'] = self.second.getValue()

	def icon(self,bitmap):
		self.iconwindow = IconWindow(self,image=bitmap)

class TimeEntry:
	def __init__(self,master):
		self.frame = Frame(master,{
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		self.label = Label(self.frame,{
			  'text': '.......',
			  Pack: {
                          'side': 'left',
                          'expand': '1'
                          }})
		self.var = IntVar(self.frame)
                self.entry = EmacsEntry(self.frame,{
                          'width': '5',
			  'relief': 'sunken',
			  'textvariable': self.var.__str__(),
                          Pack: {
                          'side': 'left'
                          }})
#		self.entry['textvariable'] = self.var

	def setNext(self,next):
		self.entry.set_next(next.entry)

	def getValue(self):
		return self.var.get()

	def setValue(self,value):
		self.var.set(value)
	
	def setLabel(self,label):
		self.label['text'] = label
