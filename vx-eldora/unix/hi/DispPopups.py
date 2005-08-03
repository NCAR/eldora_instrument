#!/local/bin/python
#
# Module:          DispPopups.py
# Original Author: Richard Neitzel
# Date:            $Date$
#
# Popup windows for setting parameters specific to various display modes.
#
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.2  1994/11/29  20:27:42  thor
# Added support for variable centered Dual display.
#
# Revision 1.1  1994/09/02  21:08:16  thor
# Initial revision
#
#
from Tkinter import *
from EmacsEntry import *
from Browser import *

import DispGbl

class Dual(Toplevel):
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
		self.middle = Frame(self,{
			  'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
		self.chkframe = Frame(self,{
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
		self.label1 = Label(self.top,{
			  'text': 'Maximum altitude:',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.dualmax = DoubleVar(self.top)
		self.maxentry = EmacsEntry(self.top,{
			  'relief': 'sunken',
			  'width': '10',
			  'textvariable': self.dualmax.__str__(),
			  Pack: {
			  'side': 'left'
			  }})
		self.label2 = Label(self.middle,{
			  'text': 'Minimum altitude:',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.dualmin = DoubleVar(self.middle)
		self.minentry = EmacsEntry(self.middle,{
			  'relief': 'sunken',
			  'width': '10',
			  'textvariable': self.dualmin.__str__(),
			  Pack: {
			  'side': 'left'
			  }})
		self.label4 = Label(self.chkframe,{
			  'text': 'Which side [looking forward]:',
			  Pack: {
			  'side': 'left',
			  }})
		self.dualside = IntVar(self.top)
		self.farleft = Radiobutton(self.chkframe,{
			  'relief': 'flat',
			  'text': 'Extreme Left',
			  'value': DispGbl.DispSides.farLeft,
			  'variable': self.dualside.__str__(),
			  Pack: {
			  'side': 'left',
			  'anchor': 'w'
			  }})
		self.left = Radiobutton(self.chkframe,{
			  'relief': 'flat',
			  'text': 'Left',
			  'value': DispGbl.DispSides.left,
			  'variable': self.dualside.__str__(),
			  Pack: {
			  'side': 'left',
			  'anchor': 'w'
			  }})
		self.center = Radiobutton(self.chkframe,{
			  'relief': 'flat',
			  'text': 'Center',
			  'value': DispGbl.DispSides.center,
			  'variable': self.dualside.__str__(),
			  Pack: {
			  'side': 'left',
			  'anchor': 'w'
			  }})
		self.right = Radiobutton(self.chkframe,{
			  'relief': 'flat',
			  'text': 'Right',
			  'value': DispGbl.DispSides.right,
			  'variable': self.dualside.__str__(),
			  Pack: {
			  'side': 'left',
			  'anchor': 'w'
			  }})
		self.farright = Radiobutton(self.chkframe,{
			  'relief': 'flat',
			  'text': 'Extreme Right',
			  'value': DispGbl.DispSides.farRight,
			  'variable': self.dualside.__str__(),
			  Pack: {
			  'side': 'left',
			  'anchor': 'w'
			  }})
		cb = self.register(self.doneCB)
		self.done = Button(self.bottom,{
			  'text': 'Done',
			  'command': (cb,'done'),
			  Pack: {
			  'side': 'top'
			  }})
		self.maxentry.bind('<Return>',self.returnBinding)
		self.minentry.bind('<Return>',self.returnBinding)
		self.withdraw()
		#
		# Icon.
		self.iconbitmap('@disp.xbm')
	#
	#
	#
	def doneCB(self,d):
		self.withdraw()

	def returnBinding(self,e):
		pass

	def popup(self):
		self.deiconify()
		self.maxentry.focus()

	def set(self,dict):
		self.paramdict = dict
		self.reset()

	def reset(self):
		self.dualmax.set(self.paramdict['top'])
		self.dualmin.set(self.paramdict['bottom'])
		self.dualside.set(self.paramdict['side'])

	def get(self):
		self.paramdict['top'] = self.dualmax.get()
		self.paramdict['bottom'] = self.dualmin.get()
		self.paramdict['side'] = self.dualside.get()

class Radial(Toplevel):
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
		self.label = Label(self.top,{
			  'text': 'Radius in pixels:',
			  Pack: {
			  'side': 'left'
			  }})
		self.radius = IntVar(self.top)
		self.entry = EmacsEntry(self.top,{
			  'relief': 'sunken',
			  'width': '10',
			  'textvariable': self.radius.__str__(),
			  Pack: {
			  'side': 'left'
			  }})

		cb = self.register(self.updateCB)
		self.scale = Scale(self.middle,{
			  'from': '256',
			  'to': '511',
			  'length': '10c',
			  'orient': 'horizontal',
			  'tickinterval': '50',
			  'command': cb,
			  Pack: {
			  'side': 'top'
			  }})
		cb = self.register(self.doneCB)
		self.done = Button(self.bottom,{
			  'text': 'Done',
			  'command': (cb,'done'),
			  Pack: {
			  'side': 'top'
			  }})
		self.entry.bind('<Return>',self.returnBinding)
		self.withdraw()
		#
		# Icon.
		self.iconbitmap('@disp.xbm')
	#
	#
	#
	def doneCB(self,d):
		self.withdraw()

	def updateCB(self,value):
		self.radius.set(value)

	def returnBinding(self,e):
		self.scale.set(self.radius.get())

	def popup(self):
		self.deiconify()
		self.entry.focus()

	def set(self,dict):
		self.paramdict = dict
		self.reset()

	def reset(self):
		self.radius.set(self.paramdict['radius'])
		self.scale.set(self.paramdict['radius'])

	def get(self):
		self.paramdict['radius'] = self.radius.get()


class Vert(Toplevel):
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
		self.middle3 = Frame(self,{
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
		self.label1 = Label(self.top,{
			  'text': 'Maximum altitude:',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.vertmax = DoubleVar(self.top)
		self.maxentry = EmacsEntry(self.top,{
			  'relief': 'sunken',
			  'width': '10',
			  'textvariable': self.vertmax.__str__(),
			  Pack: {
			  'side': 'left'
			  }})
		self.label2 = Label(self.middle1,{
			  'text': 'Minimum altitude:',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.vertmin = DoubleVar(self.top)
		self.minentry = EmacsEntry(self.middle1,{
			  'relief': 'sunken',
			  'width': '10',
			  'textvariable': self.vertmin.__str__(),
			  Pack: {
			  'side': 'left'
			  }})
		self.label3 = Label(self.middle2,{
			  'text': 'Distance from aircraft:',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.vertdist = DoubleVar(self.top)
		self.distentry = EmacsEntry(self.middle2,{
			  'relief': 'sunken',
			  'width': '10',
			  'textvariable': self.vertdist.__str__(),
			  Pack: {
			  'side': 'left'
			  }})
		self.label4 = Label(self.middle3,{
			  'text': 'Which side [looking forward]:',
			  Pack: {
			  'side': 'left',
			  }})
		self.vertside = IntVar(self.top)
		self.left = Radiobutton(self.middle3,{
			  'relief': 'flat',
			  'text': 'Left',
			  'value': DispGbl.DispSides.left,
			  'variable': self.vertside.__str__(),
			  Pack: {
			  'side': 'left',
			  'anchor': 'w'
			  }})
		self.right = Radiobutton(self.middle3,{
			  'relief': 'flat',
			  'text': 'Right',
			  'value': DispGbl.DispSides.right,
			  'variable': self.vertside.__str__(),
			  Pack: {
			  'side': 'left',
			  'anchor': 'w'
			  }})
		cb = self.register(self.doneCB)
		self.done = Button(self.bottom,{
			  'text': 'Done',
			  'command': (cb,'done'),
			  Pack: {
			  'side': 'top',
			  }})
		self.maxentry.bind('<Return>',self.returnBinding)
		self.minentry.bind('<Return>',self.returnBinding)
		self.distentry.bind('<Return>',self.returnBinding)
		self.withdraw()
		#
		# Icon.
		self.iconbitmap('@disp.xbm')
	#
	#
	#
	def doneCB(self,d):
		self.withdraw()

	def returnBinding(self,e):
		pass

	def popup(self):
		self.deiconify()
		self.maxentry.focus()

	def set(self,dict):
		self.paramdict = dict
		self.reset()

	def reset(self):
		self.vertmax.set(self.paramdict['top'])
		self.vertmin.set(self.paramdict['bottom'])
		self.vertdist.set(self.paramdict['distance'])
		self.vertside.set(self.paramdict['side'])

	def get(self):
		self.paramdict['top'] = self.vertmax.get()
		self.paramdict['bottom'] = self.vertmin.get()
		self.paramdict['distance'] = self.vertdist.get()
		self.paramdict['side'] = self.vertside.get()


class Horiz(Toplevel):
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
		self.label1 = Label(self.top,{
			  'text': 'Altitude:',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.alt = DoubleVar(self.top)
		self.altentry = EmacsEntry(self.top,{
			  'relief': 'sunken',
			  'width': '10',
			  'textvariable': self.alt.__str__(),
			  Pack: {
			  'side': 'left'
			  }})
		self.label2 = Label(self.middle,{
			  'text': 'Size:',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.size = DoubleVar(self.top)
		self.sizeentry = EmacsEntry(self.middle,{
			  'relief': 'sunken',
			  'width': '10',
			  'textvariable': self.size.__str__(),
			  Pack: {
			  'side': 'left'
			  }})
		cb = self.register(self.doneCB)
		self.done = Button(self.bottom,{
			  'text': 'Done',
			  'command': (cb,'done'),
			  Pack: {
			  'side': 'top'
			  }})
		self.altentry.bind('<Return>',self.returnBinding)
		self.sizeentry.bind('<Return>',self.returnBinding)
		self.withdraw()
		#
		# Icon.
		self.iconbitmap('@disp.xbm')
	#
	#
	#
	def doneCB(self,d):
		self.withdraw()

	def returnBinding(self,e):
		pass

	def popup(self):
		self.deiconify()
		self.altentry.focus()

	def set(self,dict):
		self.paramdict = dict
		self.reset()

	def reset(self):
		self.alt.set(self.paramdict['distance'])
		self.size.set(self.paramdict['size'])

	def get(self):
		self.paramdict['distance'] = self.alt.get()
		self.paramdict['size'] = self.size.get()
