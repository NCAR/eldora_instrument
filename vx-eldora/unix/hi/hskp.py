#!/local/bin/python
#
# Module:          hskp.py
# Original Author: Richard Neitzel
# Date:            $Date$
#
# Tk based GUI for housekeeper control window on ELDORA.
#
# $Id$
# revision history:
# -----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.4  1995/11/28  18:20:50  thor
# Changed to new IconWindow format.
#
# Revision 1.3  1995/02/14  17:06:45  thor
# Made comm failure msg clearer.
#
# Revision 1.2  1994/09/09  17:30:05  thor
# New GNU readline eliminated need for --.
#
# Revision 1.1  1994/09/02  21:08:16  thor
# Initial revision
#

from Tkinter import *
from Dialog import *
import string

from Timeset import *
from StaticEntry import *
from IconWindow import *

import sys
import HskpClnt
import HskpGbl

class HskpGui(Tk):
        def __init__(self, name = 'ELDORA Housekeeper'):
                #
                # First let's set up the GUI stuff.
                #
                # We'll call ourself 'MasterControl'.
                Tk.__init__(self,None,None,'MasterControl')
                #
                # Create the menubar & its pulldowns.
                self.menubar = Frame(self, {
                          'relief' : 'raised',
                          'bd': 2,
                          Pack: {
                          'side': 'top',
                          'fill': 'x'
                          }})

                self.menubutton = Menubutton(self.menubar, {
                          'text': 'File',
                          'underline': 0,
                          Pack: {'side': 'left'}
                          })
                self.pulldown = Menu(self.menubutton)
                self.menubutton['menu'] = self.pulldown
                #
                # Setup callbacks.
                acb = self.register(self.actionCB)
		self.pulldown.add('command', {
			  'label': 'Reconnect',
			  'command': (acb, 'reconnect')
			  })
		self.pulldown.add('command',{
			  'label': 'Clear status',
			  'command': (acb, 'clear')
			  })
		self.pulldown.add('command',{
			  'label': 'Get status',
			  'command': (acb, 'get')
			  })
		self.pulldown.add('command',{
			  'label': 'Set time',
			  'command': (acb, 'time')
			  })
		self.pulldown.add('command', {
                          'label': 'Quit',
                          'command': (acb, 'exit')
			  })
		#
		# Add a bunch of static entries.
		self.entframe = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		self.leftframe = Frame(self.entframe,{
			  Pack: {
			  'side': 'left',
			  'fill': 'both'
			  }})
		self.rightframe = Frame(self.entframe,{
			  Pack: {
			  'side': 'left',
			  'fill': 'both'
			  }})

		self.fvme = StaticEntry(self.leftframe,'top')
		self.avme = StaticEntry(self.rightframe,'top')
		self.transpwr = StaticEntry(self.leftframe,'top')
		self.pulsepwr = StaticEntry(self.rightframe,'top')
		self.iru = StaticEntry(self.leftframe,'top')
		self.gps = StaticEntry(self.rightframe,'top')
		self.ecb = StaticEntry(self.leftframe,'top')
		self.ecbslave = StaticEntry(self.rightframe,'top')
		self.ontemp = StaticEntry(self.leftframe,'top')
		self.offtemp = StaticEntry(self.rightframe,'top')
		self.fdds = StaticEntry(self.leftframe,'top')
		self.adds = StaticEntry(self.rightframe,'top')
		self.ministat = StaticEntry(self.leftframe,'top')
		self.minitemp = StaticEntry(self.rightframe,'top')
		self.mc = StaticEntry(self.leftframe,'top')
		self.clockstat = StaticEntry(self.rightframe,'top')
		self.insitu = StaticEntry(self.rightframe,'top')

		self.fvme.setLabel('Fore VME to VME:')
		self.avme.setLabel('Aft VME to VME:')
		self.transpwr.setLabel('Transmitted Power Meter:')
		self.pulsepwr.setLabel('Test Pulse Power Meter:')
		self.iru.setLabel('IRU:')
		self.gps.setLabel('GPS:')
		self.ecb.setLabel('ECB:')
		self.ecbslave.setLabel('ECB Slave:')
		self.ontemp.setLabel('Onboard Temperature:')
		self.offtemp.setLabel('Offboard Temperature:')
		self.fdds.setLabel('Fore DDS:')
		self.adds.setLabel('Aft DDS:')
		self.ministat.setLabel('MiniRIMS:')
		self.minitemp.setLabel('MiniRIMS Temperature:')
		self.mc.setLabel('Motor Controller:')
		self.clockstat.setLabel('Clock:')
		self.insitu.setLabel('In-Situ:')
		#
		# Build a list of StaticEntry objects.
		self.statusObjects = [self.fvme, self.avme, self.transpwr,
			  self.pulsepwr, self.iru, self.gps, self.ecb,
			  self.ecbslave, self.ontemp, self.offtemp, self.fdds,
			  self.adds, self.ministat, self.minitemp, self.mc,
			  self.clockstat, self.insitu]

		for i in self.statusObjects:
			i.hex()
			i.setData(0)
		#
		# Now the options...
		self.optframe = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		#
		# Initialize variables.
		self.doAlign = IntVar(self)
		self.doClock = IntVar(self)
		self.doIrigb = IntVar(self)
		self.doGps = IntVar(self)
		self.doTime = IntVar(self)
		self.doAlign.set(1)
		self.doClock.set(0)
		self.doIrigb.set(1)
		self.doGps.set(1)
		self.align = Checkbutton(self.optframe,{
			  'text': 'Align',
			  'variable': self.doAlign.__str__(),
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.clkstart = Checkbutton(self.optframe,{
			  'text': 'Manually start clock',
			  'variable': self.doClock.__str__(),
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.syncIRIGB = Checkbutton(self.optframe,{
			  'text': 'Sync to IRIGB',
			  'variable': self.doIrigb.__str__(),
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		self.initGPS = Checkbutton(self.optframe,{
			  'text': 'Init GPS',
			  'variable': self.doGps.__str__(),
			  Pack: {
			  'side': 'left',
			  'expand': '1',
			  'fill': 'x'
			  }})
		#
		# Command frame.
		self.cmdframe = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		self.timebutton = Button(self.cmdframe,{
			  'text': 'Set Clock',
			  'command': (acb,'time'),
			  Pack: {
			  'side': 'left',
			  'padx': '10m'
			  }})
		self.clrbutton = Button(self.cmdframe,{
			  'text': 'Clear status',
			  'command': (acb,'clear'),
			  Pack: {
			  'side': 'left',
			  'padx': '10m'
			  }})
		appcb = self.register(self.apply)
		self.appbutton = Button(self.cmdframe,{
			  'text': 'Apply',
			  'command': (appcb,'x'),
			  Pack: {
			  'side': 'left',
			  'padx': '10m'
			  }})
		#
		# Message frame
		self.msgframe  = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
                self.msg = Label(self.msgframe, {
                          'text': 'Initializing..........',
                          Pack: {
                          'side': 'left',
                          'fill': 'both'
                          }})
		#
                # My name is....
                self.title(name)
                self.iconname(name)
		self.icon = None
		self.goodicon()
		#
		# Initialize the popup.
		self.timeset = TimeSet(self)

		#self.timeset.icon(self.okimage)
		#
		# Misc. variables that need to be set to a rational default.
		self.cmddict = {
			  'cmd': 0,
			  'year': 0,
			  'month': 0,
			  'day': 0,
			  'hour': 0,
			  'minute': 0,
			  'second': 1
			  }
		self.statusdict = {
			  'fore': 0,
			  'aft': 0,
			  'motorCtrl': 0,
			  'transPower': 0,
			  'testPower': 0,
			  'iru': 0,
			  'gps': 0,
			  'ecbgen': 0,
			  'slvdead': 0,
			  'onBoverT': 0,
			  'offBoverT': 0,
			  'forDDSool': 0,
			  'aftDDSool': 0,
			  'minirims': 0,
			  'minirimsTemp': 0,
			  'clock': 0,
			  'insitu': 0
			  }
		self.sobjs = {
			  'fore': self.fvme,
			  'aft': self.avme,
			  'motorCtrl': self.mc,
			  'transPower': self.transpwr,
			  'testPower': self.pulsepwr,
			  'iru': self.iru,
			  'gps': self.gps,
			  'ecbgen': self.ecb,
			  'slvdead': self.ecbslave,
			  'onBoverT': self.ontemp,
			  'offBoverT': self.offtemp,
			  'forDDSool': self.fdds,
			  'aftDDSool': self.adds,
			  'minirims': self.ministat,
			  'minirimsTemp': self.minitemp,
			  'clock': self.clockstat,
			  'insitu': self.insitu
			  }
		self.tmo = 0
		self.timeset.set(self.cmddict)
	#
	# Callbacks.
	#
	def actionCB(self,caller):
		if caller == 'time':
			self.timeset.popup()
		elif caller == 'get':
			self.getStatus()
		elif caller == 'exit':
			self.destroy()
			self.quit()
			sys.exit(0)
		elif caller == 'reconnect':
			self.reconnect()
		else:
			self.clearStatus()
		
	def apply(self,caller):
		if caller == 'time':
			self.doClock.set(1)
			self.doIrigb.set(0)
			self.doTime.set(1)
		else:
			self.doTime.set(0)
		if self.sendCmd():
			self.msg['text'] = 'Started clock.'
	#
	# The status gathering routines.
	#
	def getStatus(self):
		status = self.cmdobj.status()
		self.clearStatus()
		self.setStatus(status)

	def setStatus(self,status):
	    index = 0
	    for i in self.statusdict.keys():
		self.sobjs[i].setData(status[index])
		if status[index]:
		    self.sobjs[i].setLabelBackground('red')
		    self.badicon()
		index = index + 1

	def clearStatus(self):
		self.goodicon()
		for i in self.statusObjects:
			i.setLabelBackground('aquamarine1')

	def periodic(self, junk=None):
		self.getStatus()
		if self.tmo:
			self.after(self.tmo,self.periodic,None)
	#
	# Command routines.
	#
	def sendCmd(self):
		cmd = 0
		if self.doAlign.get():
			cmd = cmd + HskpGbl.Cmds.align
		if self.doClock.get():
			cmd = cmd + HskpGbl.Cmds.start_clock
		if self.doIrigb.get():
			cmd = cmd + HskpGbl.Cmds.irigb_sync
		if self.doGps.get():
			cmd = cmd + HskpGbl.Cmds.gps_start
		if self.doTime.get():
			cmd = cmd + HskpGbl.Cmds.set_time
		self.cmdobj.setYear(self.cmddict['year'] - 1900)
		self.cmdobj.setMonth(self.cmddict['month'])
		self.cmdobj.setDay(self.cmddict['day'])
		self.cmdobj.setHour(self.cmddict['hour'])
		self.cmdobj.setMin(self.cmddict['minute'])
		self.cmdobj.setSec(self.cmddict['second'])

		status = self.cmdobj.sendCmd(cmd)
		self.clearStatus()
		self.setStatus(status)

	def badComm(self):
		self.badicon()
		self.msg['text'] = 'Communication to housekeeper failed.'
		Dialog(self,{
			  'title': 'Warning!',
			  'text': 'Communication to housekeeper failed.',
			  'strings':  ('Noted',),
			  'bitmap': 'warning',
			  'default': 0
			  })
	#
	# Functions to change icons.
	#
	def goodicon(self):
		if self.icon == None:
			self.okimage = BitmapImage('hskp.xbm',file='hskp.xbm')
			self.errimage = BitmapImage('hskperr.xbm',
				  file='hskperr.xbm')
			self.icon = IconWindow(self,image = self.okimage)
		else:
			self.icon.bitmap(image = self.okimage)
		self.icon.background('aquamarine')

	def badicon(self):
		self.icon.bitmap(image = self.errimage)
		self.icon.background('red')
	#
	# Functions to set various things.
	#
	def setTmo(self,tmo):
		self.tmo = tmo * 1000	# Tk uses milleseconds.
	#
	# Connect to target.
	def vxConnect(self):
		self.cmdobj = HskpClnt.HskpClnt('hskp')
		if self.cmdobj == None:
			print 'record.py: Unable to find object, exiting.'
                        sys.exit(1)
		else:
		    if self.cmdobj.connect():
			self.badComm()
		    else:
			self.msg['text'] = 'Connected'
		    if self.tmo > 0:
			self.after(self.tmo,self.periodic,None)

	def reconnect(self):
		if self.cmdobj.connect():
			self.badComm()
		else:
			self.goodicon()
#
# This function is the default if the program is invoked as a runable script.
#
def go():
        global hskp

        tmo = 0

        import getopt, pidfile

        i = len(sys.argv)
        opts, args = getopt.getopt(sys.argv[1:],'s:')

        for i in range(len(opts)):
                tmp = opts[i]
                if tmp[0] == '-s':
                        tmo = string.atoi(tmp[1])

	pidfile.create('hskp.pid')

        hskp = HskpGui('Eldora Housekeeper')

	hskp.setTmo(tmo)

	hskp.vxConnect()

        hskp.mainloop()
        
if __name__ == '__main__':
        go()
