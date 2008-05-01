#!/local/bin/python
#
# Module:          record.py
# Original Author: Richard Neitzel
# Date:            $Date$
#
# Tk based GUI for recording control window on ELDORA.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.4  1995/11/28  18:20:50  thor
# Changed to new IconWindow format.
#
# Revision 1.3  1995/02/14  17:06:45  thor
# Made comm failure msg clearer & added load command.
#
# Revision 1.2  1994/09/09  17:30:05  thor
# New GNU readline eliminated need for --.
#
# Revision 1.1  1994/09/02  21:08:16  thor
# Initial revision
#
#
from Tkinter import *
from Dialog import *
import string

from RecordStatus import *
from IconWindow import *

import RecordClnt
import RecordGbl

_status_read_order = (
	  'drvs0','drvs1','numdrv','status','failures','attempts',
	  'percent','unit'
	  )
_one_read = {'percent':1,'unit':1}

class RecordGui(Tk):
        def __init__(self, name = 'ELDORA Recording Control'):
		#
		# Misc. variables that need to be set to a rational default.
		self.percent = '0'
		self.normalcolor = 'aquamarine1'
		self.status = {
			  'drvs0': (0,0),
			  'drvs1': (0,0),
			  'numdrv': (0,0),
			  'status': (0,0),
			  'failures': (0,0),
			  'attempts': (0,0),
			  'percent': 0,
			  'unit': 0
			  }
		#
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
                acb = self.register(self.reconnectCB)
		self.pulldown.add('command', {
			  'label': 'Reconnect',
			  'command': acb
			  })
		acb = self.register(self.onCB)
		self.pulldown.add('command',{
			  'label': 'Recording on',
			  'command': acb
			  })
		acb = self.register(self.offCB)
		self.pulldown.add('command',{
			  'label': 'Recording off',
			  'command': acb
			  })
		acb = self.register(self.logCB)
		self.pulldown.add('command',{
			  'label': 'Record Log File',
			  'command': acb
			  })
		acb = self.register(self.rewindCB)
		self.pulldown.add('command',{
			  'label': 'Rewind',
			  'command': acb
			  })
		acb = self.register(self.ejectCB)
		self.pulldown.add('command',{
			  'label': 'Eject',
			  'command': acb
			  })
		self.pulldown.add('command',{
			  'label': 'Init unit',
			  'command': self.initunit
			  })
		self.pulldown.add('command',{
			  'label': 'Switch units',
			  'command': self.switchunits
			  })
		scb = self.register(self.statusCB)
		self.pulldown.add('command',{
			  'label': 'Status for sys. 0',
			  'command': (scb, '0')
			  })
		self.pulldown.add('command',{
			  'label': 'Status for sys. 1',
			  'command': (scb, '1')
			  })
		ecb = self.register(self.exitCB)
		self.pulldown.add('command', {
                          'label': 'Quit',
                          'command': (ecb, '0')
			  })
		#
		# Add the status frame.
		self.statframe = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		self.statlabel = Label(self.statframe,{
			  'text': 'Currently active system: 0',
			  Pack: {
			  'side': 'left',
			  'padx': '5m'
			  }})
		self.zerobutton = Button(self.statframe,{
			  'text': 'System 0 status',
			  'command': (scb, '0'),
			  Pack: {
			  'side': 'left',
			  'padx': '10m'
			  }})
		self.onebutton = Button(self.statframe,{
			  'text': 'System 1 status',
			  'command': (scb, '1'),
			  Pack: {
			  'side': 'left',
			  'padx': '10m'
			  }})
		#
		# Recording frame.
		self.recframe = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		acb = self.register(self.onCB)
		self.onbutton = Button(self.recframe,{
			  'text': 'Recording on',
			  'command': acb,
			  Pack: {
			  'side': 'left',
			  'expand': '1'
			  }})
		acb = self.register(self.offCB)
		self.offbutton = Button(self.recframe,{
			  'text': 'Recording off',
			  'command': acb,
			  Pack: {
			  'side': 'left',
			  'expand': '1'
			  }})
		#
		# Action frame.
		self.actframe = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		acb = self.register(self.rewindCB)
		self.rewind = Button(self.actframe,{
			  'text': 'Rewind',
			  'command': acb,
			  Pack: {
			  'side': 'left',
			  'expand': '1'
			  }})
		acb = self.register(self.ejectCB)
		self.eject = Button(self.actframe,{
			  'text': 'Eject',
			  'command': acb,
			  Pack: {
			  'side': 'left',
			  'expand': '1'
			  }})
		acb = self.register(self.loadCB)
		self.load = Button(self.actframe,{
			  'text': 'Load',
			  'command': acb,
			  Pack: {
			  'side': 'left',
			  'expand': '1'
			  }})
		self.init = Button(self.actframe)
		self.init['text'] = 'Init unit'
		self.init['command'] = self.initunit
		self.init.pack(side=LEFT,expand='1')

		self.switch = Button(self.actframe)
		self.switch['text'] = 'Switch units'
		self.switch['command'] = self.switchunits
		self.switch.pack(side=LEFT,expand='1')

		acb = self.register(self.logCB)
		self.log = Button(self.actframe,{
			  'text': 'Record log',
			  'command': acb,
			  Pack: {
			  'side': 'left',
			  'expand': '1'
			  }})
		#
		# Usage frame.
		self.useframe = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		slcb = self.register(self.scaleCB)
		self.usescale = Scale(self.useframe,{
			  'label': 'Percent of recording media used:',
			  'from': '0',
			  'to': '100',
			  'tickinterval': '10',
			  'orient': 'horizontal',
			  'length': '10c',
			  'command': slcb,
			  Pack: {
			  'side': 'top',
			  'padx': '10m',
			  'pady': '2m'
			  }})
		#
		# Status frame.
		self.msgframe = Frame(self,{
			  'relief': 'raised',
			  'bd': '2',
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		self.msg = Label(self.msgframe,{
			  'text': 'Initializing...',
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

		# Initialize the two status windows.
		self.sw1 = RecordStatus(self)
		self.sw2 = RecordStatus(self)
		self.sw1.setDict(self.status)
		self.sw2.setDict(self.status)
		self.sw1.setUnit(0)
		self.sw2.setUnit(1)
		self.sw1.bitmap('@tape.xbm')
		self.sw2.bitmap('@tape.xbm')
	#
	# Callbacks.
	#
	def logCB(self):
		self.sendCmd(RecordGbl.Cmds.log)

	def onCB(self):
		self.sendCmd(RecordGbl.Cmds.rec_on)

	def offCB(self):
		self.sendCmd(RecordGbl.Cmds.rec_off)

	def loadCB(self):
		self.sendCmd(RecordGbl.Cmds.load_tape)

	def rewindCB(self):
		self.sendCmd(RecordGbl.Cmds.rewind)

	def ejectCB(self):
		self.sendCmd(RecordGbl.Cmds.eject)

	def reconnectCB(self):
		self.sendCmd(RecordGbl.Cmds.reconnect)

	def initunit(self):
		self.sendCmd(RecordGbl.Cmds.init_unit)

	def switchunits(self):
		self.sendCmd(RecordGbl.Cmds.use_other_unit)
		
	def statusCB(self, caller):
		if caller == '0':
			self.sw1.popup()
		else:
			self.sw2.popup()

	def scaleCB(self, j):		# Resets if user tries to change it.
		self.usescale.set(self.status['percent'])

	def exitCB(self, j):
		self.destroy()
		self.quit()
		import sys
		sys.exit()
	#
	# The status gathering routines.
	#
	def setStatus(self,status):
		index = 0
		for i in _status_read_order:
			if _one_read.has_key(i):
				self.status[i] = status[index]
				index = index + 1
			else:
				t1 = status[index]
				index = index + 1
				t2 = status[index]
				index = index + 1
				self.status[i] = (t1, t2)
		self.setScale()
		hexstatus = self.status['status'][self.status['unit']]
		if hexstatus & 4:
			self.recframe['background'] = self.normalcolor
			self.goodicon()
		else:
			self.recframe['background'] = 'red'
			self.badicon()
		self.statlabel['text'] = 'Currently active system: ' + str(
			  self.status['unit'])
		self.sw1.set()
		self.sw2.set()
		
	def periodic(self, junk=None):
		status = self.cmdobj.status()
		if status[0] == -1:
			self.badComm()
		else:
			self.setStatus(status)
		if self.tmo > 0:
			self.after(self.tmo,self.periodic,None)
	#
	# Command routines.
	#
	def sendCmd(self,cmd):
		status = self.cmdobj.sendCmd(cmd)
		if status[0] == -1:
			self.badComm()
		else:
			self.msg['text'] = 'Command sent.'
			self.setStatus(status)

	def badComm(self):
		errtext = 'Communication to recorder failed.'
		self.msg['text'] =  errtext
		Dialog(self,{
			  'title': 'Warning!',
			  'text': errtext,
			  'bitmap': 'warning',
			  'default': 0,
			  'strings': ('Noted',)
			  })
	#
	# Functions to change icons.
	#
	def goodicon(self):
		if self.icon == None:
			self.okimage = BitmapImage('tape.xbm',file='tape.xbm')
			self.errimage = BitmapImage('tape_err.xbm',
				  file='tape_err.xbm')
			self.icon = IconWindow(self,image=self.okimage)
		else:
			self.icon.bitmap(image=self.okimage)
		self.icon.background('aquamarine')

	def badicon(self):
		self.icon.bitmap(image=self.errimage)
		self.icon.background('red')
	#
	# Functions to set various things.
	#
	def setTmo(self,tmo):
		self.tmo = tmo * 1000	# Tk uses milleseconds.

	def setScale(self):
		p = self.status['percent']
		p = 100 - p
		if p >= 95:
			self.useframe['background'] = 'yellow'
		else:
			self.useframe['background'] = self.normalcolor
		self.percent = str(p)
		self.usescale.set(self.percent)
	#
	# Connect to remote target.
	def connect(self):
		self.cmdobj = RecordClnt.RecordClnt('tape')
		if self.cmdobj == None:
			print 'record.py: Unable to find object, exiting.'
			sys.exit(1)
		self.sendCmd(RecordGbl.Cmds.reconnect)
		self.msg['text'] = 'Connected'
#
# This function is the default if the program is invoked as a runable script.
#
def go():
        global record

        tmo = 0
	test = 0

        import sys, getopt, pidfile

        i = len(sys.argv)
        opts, args = getopt.getopt(sys.argv[1:],'s:b')

        for i in range(len(opts)):
                tmp = opts[i]
                if tmp[0] == '-s':
                        tmo = string.atoi(tmp[1])
		elif tmp[0] == '-b':
			test = 1

	pidfile.create('record.pid')

        record = RecordGui('Eldora Recording Control')

	record.setTmo(tmo)

	if not test:
		record.connect()
	if tmo:
		record.periodic()

        record.mainloop()
        
if __name__ == '__main__':
        go()
