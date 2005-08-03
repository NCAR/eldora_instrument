#
# Module:          DispEditor.py
# Original Author: Richard Neitzel
# Date:            $Date$
#
# This class is the editor for ELDORA display control.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.5  1996/03/25  22:37:42  thor
# Added raw support.
#
# Revision 1.4  1994/11/03  18:05:23  thor
# Added new items for long/short PRT.
#
# Revision 1.3  1994/10/24  15:04:51  thor
# Removed redundent velocity menu item.
#
# Revision 1.2  1994/09/12  16:09:49  thor
# Added new parameters.
#
# Revision 1.1  1994/09/02  21:08:16  thor
# Initial revision
#
#
from Tkinter import *
from EmacsEntry import *
from Browser import *
from DispPopups import *

import DispGbl

# Convert to/from expanded string.
_label_lookup_tbl = {
    DispGbl.DispParameters.dbz: 'DBZ',
    DispGbl.DispParameters.vr: 'Velocity',
    DispGbl.DispParameters.sw: 'Spectral Width',
    DispGbl.DispParameters.power: 'Normalized Coherent Power',
    DispGbl.DispParameters.snr: 'Signal/noise Ratio',
    DispGbl.DispParameters.vl: 'Velocity - Long PRT',
    DispGbl.DispParameters.vs: 'Velocity - Short PRT',
    DispGbl.DispParameters.sws: 'Spectral Width - Short PRT',
    DispGbl.DispParameters.swl: 'Spectral Width - Long PRT',
    DispGbl.DispParameters.ncpl: 'Normalized Coherent Power- Long PRT',
    #DispGbl.DispParameters.DBZL: 'DBZ - Long PRT',
    DispGbl.DispParameters.none: 'None',
    }

class DispEditor(Toplevel):
	def __init__(self, master = None, unit):
                Toplevel.__init__(self,master,{})
                #
                # Build the layout items.
		self.title('Display ' + unit)
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
		self.p1frame = Frame(self.top,{
			  'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
		self.p2frame = Frame(self.top,{
			  'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
		self.p3frame = Frame(self.top,{
			  'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
		#
		# Now stick paramter stuff into each subframe.
		self.p1label = Label(self.p1frame,{
			  'text': 'Parameter 1:',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p1menubutton = Menubutton(self.p1frame,{
			  'text': 'Unknown',
			  'relief': 'raised',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.p1maxlabel = Label(self.p1frame,{
			  'text': 'Maximum',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p1maxval = DoubleVar(self.p1frame)
		self.p1maxval.set(0.0)
		self.p1max = EmacsEntry(self.p1frame,{
			  'relief': 'sunken',
			  'width': '6',
			  'textvariable': self.p1maxval.__str__(),
			  Pack: {
			  'side': 'left',
			  'pady': '2m'
			  }})
		self.p1minlabel = Label(self.p1frame,{
			  'text': 'Minimum',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p1minval = DoubleVar(self.p1frame)
		self.p1minval.set(0.0)
		self.p1min = EmacsEntry(self.p1frame,{
			  'relief': 'sunken',
			  'width': '6',
			  'textvariable': self.p1minval.__str__(),
			  Pack: {
			  'side': 'left',
			  'pady': '2m'
			  }})
		self.p1menu = Menu(self.p1menubutton)
		self.p1menubutton['menu'] = self.p1menu
		self.p1type = IntVar(self.p1menu)
		self.p1type.set(DispGbl.DispParameters.dbz)
		self.setvar('p1type',self.p1type)
		vcb = self.register(self.varCB)
		self.p1menu.add('radiobutton',{
			  'label': 'DBZ',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.dbz,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Velocity',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.vr,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Spectral width',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.sw,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Normalized coherent power',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.power,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Signal/noise ratio',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.snr,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Velocity - Long PRT',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.vl,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Velocity - Short PRT',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.vs,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Spectral Width - Short PRT',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.sws,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Spectral Width - Long PRT',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.swl,
			  'command': (vcb,'p1')
			  })
		self.p1menu.add('radiobutton',{
			  'label': 'Normalized Coherent Power- Long PRT',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.ncpl,
			  'command': (vcb,'p1')
			  })
		#self.p1menu.add('radiobutton',{
		#	  'label': 'DBZ - Long PRT',
		#	  'variable': self.p1type.__str__(),
		#	  'value': DispGbl.DispParameters.dbzl,
		#	  'command': (vcb,'p1')
		#	  })
		self.p1menu.add('radiobutton',{
			  'label': 'None',
			  'variable': self.p1type.__str__(),
			  'value': DispGbl.DispParameters.none,
			  'command': (vcb,'p1')
			  })
		#
		self.p2label = Label(self.p2frame,{
			  'text': 'Parameter 2:',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p2menubutton = Menubutton(self.p2frame,{
			  'text': 'Unknown',
			  'relief': 'raised',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.p2maxlabel = Label(self.p2frame,{
			  'text': 'Maximum',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p2maxval = DoubleVar(self.p2frame)
		self.p2maxval.set(0.0)
		self.p2max = EmacsEntry(self.p2frame,{
			  'relief': 'sunken',
			  'width': '6',
			  'textvariable': self.p2maxval.__str__(),
			  Pack: {
			  'side': 'left',
			  'pady': '2m'
			  }})
		self.p2minlabel = Label(self.p2frame,{
			  'text': 'Minimum',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p2minval = DoubleVar(self.p2frame)
		self.p2minval.set(0.0)
		self.p2min = EmacsEntry(self.p2frame,{
			  'relief': 'sunken',
			  'width': '6',
			  'textvariable': self.p2minval.__str__(),
			  Pack: {
			  'side': 'left',
			  'pady': '2m'
			  }})
		self.p2menu = Menu(self.p2menubutton)
		self.p2menubutton['menu'] = self.p2menu
		self.p2type = IntVar(self.p2menu)
		self.p2type.set(DispGbl.DispParameters.vr)
		self.p2menu.add('radiobutton',{
			  'label': 'DBZ',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.dbz,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Velocity',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.vr,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Spectral width',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.sw,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Normalized coherent power',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.power,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Signal/noise ratio',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.snr,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Velocity - Long PRT',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.vl,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Velocity - Short PRT',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.vs,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Spectral Width - Short PRT',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.sws,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Spectral Width - Long PRT',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.swl,
			  'command': (vcb,'p2')
			  })
		self.p2menu.add('radiobutton',{
			  'label': 'Normalized Coherent Power- Long PRT',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.ncpl,
			  'command': (vcb,'p2')
			  })
		#self.p2menu.add('radiobutton',{
		#	  'label': 'DBZ - Long PRT',
		#	  'variable': self.p2type.__str__(),
		#	  'value': DispGbl.DispParameters.dbzl,
		#	  'command': (vcb,'p2')
		#	  })
		self.p2menu.add('radiobutton',{
			  'label': 'None',
			  'variable': self.p2type.__str__(),
			  'value': DispGbl.DispParameters.none,
			  'command': (vcb,'p2')
			  })
		#
		self.p3label = Label(self.p3frame,{
			  'text': 'Parameter 3:',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p3menubutton = Menubutton(self.p3frame,{
			  'text': 'Unknown',
			  'relief': 'raised',
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.p3maxlabel = Label(self.p3frame,{
			  'text': 'Maximum',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p3maxval = DoubleVar(self.p3frame)
		self.p3maxval.set(0.0)
		self.p3max = EmacsEntry(self.p3frame,{
			  'relief': 'sunken',
			  'width': '6',
			  'textvariable': self.p3maxval.__str__(),
			  Pack: {
			  'side': 'left',
			  'pady': '2m'
			  }})
		self.p3minlabel = Label(self.p3frame,{
			  'text': 'Minimum',
			  Pack: {
			  'side': 'left',
			  'padx': '5m',
			  'pady': '2m'
			  }})
		self.p3minval = DoubleVar(self.p3frame)
		self.p3minval.set(0.0)
		self.p3min = EmacsEntry(self.p3frame,{
			  'relief': 'sunken',
			  'width': '6',
			  'textvariable': self.p3minval.__str__(),
			  Pack: {
			  'side': 'left',
			  'pady': '2m'
			  }})
		self.p3menu = Menu(self.p3menubutton)
		self.p3menubutton['menu'] = self.p3menu
		self.p3type = IntVar(self.p3menu)
		self.p3type.set(DispGbl.DispParameters.none)
		self.p3menu.add('radiobutton',{
			  'label': 'DBZ',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.dbz,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Velocity',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.vr,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Spectral width',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.sw,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Normalized coherent power',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.power,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Signal/noise ratio',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.snr,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Velocity - Long PRT',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.vl,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Velocity - Short PRT',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.vs,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Spectral Width - Short PRT',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.sws,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Spectral Width - Long PRT',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.swl,
			  'command': (vcb,'p3')
			  })
		self.p3menu.add('radiobutton',{
			  'label': 'Normalized Coherent Power- Long PRT',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.ncpl,
			  'command': (vcb,'p3')
			  })
		#self.p3menu.add('radiobutton',{
		#	  'label': 'DBZ - Long PRT',
		#	  'variable': self.p3type.__str__(),
		#	  'value': DispGbl.DispParameters.dbzl,
		#	  'command': (vcb,'p3')
		#	  })
		self.p3menu.add('radiobutton',{
			  'label': 'None',
			  'variable': self.p3type.__str__(),
			  'value': DispGbl.DispParameters.none,
			  'command': (vcb,'p3')
			  })
		#
		# Add bindings for all text entries.
		self.p1max.bind('<Return>',self.returnBinding)
		self.p1min.bind('<Return>',self.returnBinding)
		self.p2max.bind('<Return>',self.returnBinding)
		self.p2min.bind('<Return>',self.returnBinding)
		self.p3max.bind('<Return>',self.returnBinding)
		self.p3min.bind('<Return>',self.returnBinding)
		#
		# Now add radiobuton groups for fore/aft & display type.
		self.typeframe = Frame(self.middle,{
			  Pack: {
			  'side': 'left',
			  'fill': 'both',
			  'padx': '2m',
			  'expand': '1'
			  }})
		self.typelabel = Label(self.typeframe,{
			  'text': 'Display type',
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		self.radartype = IntVar(self.typeframe)
		self.radartype.set(DispGbl.DispType.dual)
		tcb = self.register(self.typeCB)
		self.dual = Radiobutton(self.typeframe,{
			  'relief': 'flat',
			  'text': 'Dual',
			  'variable': self.radartype.__str__(),
			  'value': DispGbl.DispType.dual,
			  'command': (tcb, 'x'),
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		self.radial = Radiobutton(self.typeframe,{
			  'relief': 'flat',
			  'text': 'Radial',
			  'variable': self.radartype.__str__(),
			  'value': DispGbl.DispType.radial,
			  'command': (tcb, 'x'),
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		self.horiz = Radiobutton(self.typeframe,{
			  'relief': 'flat',
			  'text': 'Horizontal',
			  'variable': self.radartype.__str__(),
			  'value': DispGbl.DispType.horiz,
			  'command': (tcb, 'x'),
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		self.vert = Radiobutton(self.typeframe,{
			  'relief': 'flat',
			  'text': 'Vertical',
			  'variable': self.radartype.__str__(),
			  'value': DispGbl.DispType.vertical,
			  'command': (tcb, 'x'),
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		self.raw = Radiobutton(self.typeframe,{
			  'relief': 'flat',
			  'text': 'Raw',
			  'variable': self.radartype.__str__(),
			  'value': DispGbl.DispType.raw,
			  'command': (tcb, 'x'),
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		self.directframe = Frame(self.middle,{
			  Pack: {
			  'side': 'left',
			  'fill': 'both',
			  'expand': '1'
			  }})
		self.directlabel = Label(self.directframe,{
			  'text': 'Which Radar:',
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		self.direction = IntVar(self.directframe)
		self.direction.set(DispGbl.RadarType.fore)
		self.fore = Radiobutton(self.directframe,{
			  'relief': 'flat',
			  'text': 'Fore',
			  'variable': self.direction.__str__(),
			  'value': DispGbl.RadarType.fore,
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		self.aft = Radiobutton(self.directframe,{
			  'relief': 'flat',
			  'text': 'Aft',
			  'variable': self.direction.__str__(),
			  'value': DispGbl.RadarType.aft,
			  Pack: {
			  'side': 'top',
			  'anchor': 'w'
			  }})
		#
		# Now add the buttons that do things!
		ccb = self.register(self.colorCB)
		self.cfbutton = Button(self.bottom,{
			  'text': 'Select a new color file',
			  'command': (ccb,'new'),
			  Pack: {
			  'side': 'left',
			  'padx': '10m',
			  'pady': '2m'
			  }})
		self.dfbutton = Button(self.bottom,{
			  'text': 'Use default colors',
			  'command': (ccb,'old'),
			  Pack: {
			  'side': 'left',
			  'padx': '10m',
			  'pady': '2m'
			  }})
		acb = self.register(self.applyCB)
		self.apbutton = Button(self.bottom,{
			  'text': 'Apply',
			  'command': (acb, 'apply'),
			  Pack: {
			  'side': 'left',
			  'padx': '10m',
			  'pady': '2m'
			  }})
		qcb = self.register(self.hideCB)
		self.donebutton = Button(self.bottom,{
			  'text': 'Done',
			  'command': (qcb,''),
			  Pack: {
			  'side': 'left',
			  'padx': '10m',
			  'pady': '2m'
			  }})
		#
		# No peeking!
		self.withdraw()
		#
		# Initialize the various popups.
		self.dualpop = Dual(self)
		self.radpop = Radial(self)
		self.vertpop = Vert(self)
		self.horizpop = Horiz(self)
		self.fb = FileBrowser(self)
		self.fb.setOkFunc(self.ok)
		self.fb.setDcFunc(self.ok)
		self.fb.setCRFunc(self.cr)
		self.fb.setBanner('Select a new color table file:')
		#
		# Keep python happy.
		self.applyfunc = self.donothing
		#
		# Icon.
		self.iconbitmap('@disp.xbm')
	#
	# Visibility functions.
	#
	def popup(self):
		self.deiconify()

	def hide(self):
		self.withdraw()
	#
	# Set/get functions.
	#
	def setApplyFunc(self,func):
		self.applyfunc = func

	def set(self, dict):
		self.dualpop.set(dict)
		self.radpop.set(dict)
		self.vertpop.set(dict)
		self.horizpop.set(dict)
		self.paramdict = dict
		self.reset()

	def reset(self):
		self.p1type.set(self.paramdict['p1'])
		self.resetP1()
		self.p1maxval.set(self.paramdict['max1'])
		self.p1minval.set(self.paramdict['min1'])
		#
		self.p2type.set(self.paramdict['p2'])
		self.resetP2()
		self.p2maxval.set(self.paramdict['max2'])
		self.p2minval.set(self.paramdict['min2'])
		#
		self.p3type.set(self.paramdict['p3'])
		self.resetP3()
		self.p3maxval.set(self.paramdict['max3'])
		self.p3minval.set(self.paramdict['min3'])
		#
		self.radartype.set(self.paramdict['type'])
		self.direction.set(self.paramdict['radar'])

		self.dualpop.reset()
                self.radpop.reset()
                self.vertpop.reset()
                self.horizpop.reset()
		
	def get(self):
		self.paramdict['p1'] = self.p1type.get()
		self.paramdict['max1'] = self.p1maxval.get()
		self.paramdict['min1'] = self.p1minval.get()
		self.paramdict['p2'] = self.p2type.get()
		self.paramdict['max2'] = self.p2maxval.get()
		self.paramdict['min2'] = self.p2minval.get()
		self.paramdict['p3'] = self.p3type.get()
		self.paramdict['max3'] = self.p3maxval.get()
		self.paramdict['min3'] = self.p3minval.get()
		self.paramdict['type'] = self.radartype.get()
		self.paramdict['radar'] = self.direction.get()
		type = self.radartype.get()
		if type == DispGbl.DispType.dual:
			self.dualpop.get()
		elif type == DispGbl.DispType.radial:
			self.radpop.get()
		elif type == DispGbl.DispType.vertical:
			self.vertpop.get()
		elif type == DispGbl.DispType.horiz:
			self.horizpop.get()
		#
		# This a little bit more work...
		np = 1
		if self.paramdict['p3'] != DispGbl.DispParameters.none:
			np = 3
		elif self.paramdict['p2'] != DispGbl.DispParameters.none:
			np = 2
		self.paramdict['nparams'] = np
	#
	# Callbacks & bindings.
	#
	def returnBinding(self,e):
		pass			# Yeah, that's it, just eat return.

	def hideCB(self,j):
		self.withdraw()

	def applyCB(self,j):
		self.applyfunc()

	def colorCB(self,caller):
		if caller == 'old':
			self.paramdict['colortbl'] = 'DEFAULT'
		else:
			self.fb.popup()

	def varCB(self, caller):
		if caller == 'p1':
			self.resetP1()
		elif caller == 'p2':
			self.resetP2()
		elif caller == 'p3':
			self.resetP3()

	def typeCB(self,t):
		radartype = self.radartype.get()
		if radartype == DispGbl.DispType.dual:
			self.dualpop.popup()
		elif radartype == DispGbl.DispType.radial:
			self.radpop.popup()
		elif radartype == DispGbl.DispType.vertical:
			self.vertpop.popup()
		elif radartype == DispGbl.DispType.horiz:
			self.horizpop.popup()
	#
	# These are callbacks that are hooked onto the FileBrowser's.
	#
	def ok(self):
		self.fb._defaultOk()
		self.paramdict['colortbl'] = self.fb.getItem()

	def cr(self):
		self.fb._defCR()
		self.paramdict['colortbl'] = self.fb.getItem()

	# These functions set the labels on the menubuttons.
	#
	def resetP1(self):
		self.p1menubutton['text'] = _label_lookup_tbl[
			  self.p1type.get()]

	def resetP2(self):
		self.p2menubutton['text'] = _label_lookup_tbl[
			  self.p2type.get()]

	def resetP3(self):
		self.p3menubutton['text'] = _label_lookup_tbl[
			  self.p3type.get()]

	def donothing(self):
		pass
