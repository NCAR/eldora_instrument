#!/local/bin/python
#
# Module:          radar.py
# Original Author: Rich Neitzel
# Date:            $Date$
# Copywrited by the National Center for Atmospheric Research
# Tk based GUI for ELDORA displays.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.1  1996/09/10  15:04:03  thor
# Added ts stuff.
#
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.6  1995/11/28  18:20:50  thor
# Changed to new IconWindow format.
#
# Revision 1.5  1995/02/14  17:06:45  thor
# Made comm failure msg clearer.
#
# Revision 1.4  1994/10/21  16:20:29  thor
# Fixed labels to match others.
#
# Revision 1.3  1994/10/14  21:47:04  thor
# Fixed some typos.
#
# Revision 1.2  1994/09/09  17:30:05  thor
# New GNU readline eliminated need for --.
#
# Revision 1.1  1994/09/06  16:43:35  thor
# Initial revision
#
#

from Tkinter import *
from Dialog import *

from StaticEntry import *
from IconWindow import *

import TsGate
import radarClnt
import string
import FifoGbl

class RadarGui(Tk):
        def __init__(self, name = 'ELDORA Radar Status'):
                #
                # First let's set up the GUI stuff.
                #
                # We'll call ourself 'DisplayControl'.
                Tk.__init__(self,None,None,'DisplayControl')
                #
                # Create the menubar & its pulldowns.
                self.menubar = Frame(self, {
                          'relief' : 'raised',
                          'bd': '2',
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
                # Setup callback.
                ccb = self.register(self.cmdCB)
                #
                # Add menu items.
		self.pulldown.add('command', {
			  'label': 'Reconnect',
			  'command': (ccb, 'reconnect')
			  })
                self.pulldown.add('command', {
			  'label': 'Clear status',
			  'command': (ccb, 'clear')
			  })
                self.pulldown.add('command', {
			  'label': 'Quit',
			  'command': (ccb, 'exit')
			  })
		#
		# Add the frame for the status area.
		self.statframe = Frame(self,{
                          'relief': 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
                self.foreframe = Frame(self.statframe,{
                          Pack: {
                          'side': 'left',
                          'fill': 'both'
                          }})
                self.aftframe = Frame(self.statframe,{
                          Pack: {
                          'side': 'left',
                          'fill': 'both'
                          }})
		self.bottom = Frame(self,{
                          Pack: {
                          'side': 'bottom',
                          'fill': 'both'
                          }}) 
		#
		# Now add the status items themselves.
		self.foretitle = Label(self.foreframe,{
			  'text': 'Fore',
			  Pack: {
			  'side': 'top',
			  'fill': 'both',
			  'pady': 5
			  }})
		self.foreRp7 = StaticEntry(self.foreframe,'top')
		self.foreMcpl = StaticEntry(self.foreframe,'top')
		self.foreStatus = StaticEntry(self.foreframe,'top')
		self.foreMailbox =  StaticEntry(self.foreframe,'top')

		self.afttitle = Label(self.aftframe,{
			  'text': 'Aft',
			  Pack: {
			  'side': 'top',
			  'fill': 'both',
			  'pady': 5
			  }})
		self.aftRp7 = StaticEntry(self.aftframe,'top')
		self.aftMcpl = StaticEntry(self.aftframe,'top')
		self.aftStatus = StaticEntry(self.aftframe,'top')
		self.aftMailbox =  StaticEntry(self.aftframe,'top')

		self.foreRp7.setLabel('Processor:')
		self.foreMcpl.setLabel('Mcpl:')
		self.foreStatus.setLabel('Status:')
		self.foreMailbox.setLabel('Mailbox:')
		self.aftRp7.setLabel('Processor:')
		self.aftMcpl.setLabel('Mcpl:')
		self.aftStatus.setLabel('Status:')
		self.aftMailbox.setLabel('Mailbox:')

		self.entries = [ self.foreRp7,
			  self.foreMcpl, self.foreStatus,
			  self.foreMailbox,self.aftRp7,
			  self.aftMcpl, self.aftStatus,
			  self.aftMailbox ]

		self.foreTbl = { 'rp7': self.foreRp7, 'mcpl': self.foreMcpl,
			  'status': self.foreStatus,
			  'mailbox': self.foreMailbox }

		self.aftTbl = { 'rp7': self.aftRp7, 'mcpl': self.aftMcpl,
			  'status': self.aftStatus,
			  'mailbox': self.aftMailbox }

		for i in self.entries:
			i.hex()
			i.setData(0)

		self.popupbutton = Button(self.bottom,{
			  'text': 'Set time series location',
			  'command': (ccb, 'ts'),
			  Pack: { 'side': 'bottom' }
			  })
		#
		# Label self.
		self.title(name)
		self.iconname(name)
		self.icon = None
		self.goodicon()
		#
		# Do other local initializations.
		self.statusSize = 8
		self.tmo = 0
		self.test = 1

		self.tsgate = TsGate.TsGate(self)

		#
		# Open fifos.
		self.noFifo = 0
		self.Fifo1 = open(FifoGbl.FifoName.one,'w')
		self.Fifo2 = open(FifoGbl.FifoName.two,'w')

		#
		# Set inital DC state.
		self.foreDC = 0
		self.aftDC = 0
		
	#
	# Callbacks.
	#
	def cmdCB(self,caller):
		if caller == 'exit':
			self.destroy()
			self.quit()
			import sys
			sys.exit(0)
		elif caller == 'reconnect':
			if self.cmdobj.connect():
				self.badComm()
		elif caller == 'ts':
			self.tsgate.popup()
		else:
			self.clrStatus()
	#
	# Get Status.
	def getStatus(self):
		fore_status = self.cmdobj.foreStatus()
		aft_status = self.cmdobj.aftStatus()
		if fore_status['status'] == 0x8000:
			self.badComm()
		elif aft_status['status'] == 0x8000:
			self.badComm()
		else:
			self.clrStatus()
			for i in self.foreTbl.keys():
				entry = self.foreTbl[i]
				x = fore_status[i]
				entry.setData(x)
				if x:
					entry.setLabelBackground('red')
					self.badicon()
			for i in self.aftTbl.keys():
				entry = self.aftTbl[i]
				x = aft_status[i]
				entry.setData(x)
				if x:
					entry.setLabelBackground('red')
					self.badicon()

			self.dcCheck(fore_status['status'],
				     aft_status['status'])

	#
	# Check DC state & send message if needed.
	def dcCheck(self,fore,aft):
		if self.noFifo:
			return
		
		busy = fore | RadarGbl.RadarStatus.dc_busy

		if busy != 0:
			return

		busy = aft | RadarGbl.RadarStatus.dc_busy

		if busy != 0:
			return

		if self.foreDC != 1:
			self.foreDC = fore | RadarGbl.RadarStatus.dc_stat
		if self.AftDC != 1:
			self.AftDC = fore | RadarGbl.RadarStatus.dc_stat

		if self.foreDC == 1 and self.aftDC == 1:
			self.Fifo1.write('1\n')
			self.Fifo2.write('1\n')

		self.cmdobj.clrDC()
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

	#
	# Clear status.
	def clrStatus(self):
		for i in self.entries:
			i.setLabelBackground('aquamarine1')
		self.goodicon()
	#
	# Periodic function to get status.
	def periodic(self, junk=None):
		self.getStatus()
		self.after(self.tmo,self.periodic,None)
	#
	# Icon handlers.
	def goodicon(self):
		if self.icon == None:
			self.okimage = BitmapImage('radar.xbm',
				  file='radar.xbm')
			self.errimage = BitmapImage('radarerr.xbm',
				  file='radarerr.xbm')
                        self.icon = IconWindow(self,image=self.okimage)
		else:
			self.icon.bitmap(image=self.okimage)
                self.icon.background('aquamarine')

        def badicon(self):
                self.icon.bitmap(image=self.errimage)
                self.icon.background('red')
	#
	# Various setting functions.
	def setTmo(self, tmo):
		self.tmo = tmo * 1000	# Tk uses milleseconds.
	#
	# Connect to 2 targets.
	def connect(self,):
		self.cmdobj = radarClnt.radarClnt('fore','aft')
                if self.cmdobj == None:
                        print 'radar.py: Unable to find object, exiting.'
                        sys.exit(1)

		if self.cmdobj.connect():
			self.badComm()
                else:
			self.tsgate.object(self.cmdobj)
			if self.tmo > 0:
				self.after(self.tmo,self.periodic,None)

	def nofifo(self):
		self.noFifo = 1
		
#
# This function is the default if the program is invoked as a runable script.
#
def go():
        global radar

        tmo = 0
        fore = ''
	aft = ''
        test = 0
	fifo = 0

        import sys, getopt, pidfile

        i = len(sys.argv)
        opts, args = getopt.getopt(sys.argv[1:],'s:bn')

        for i in range(len(opts)):
                tmp = opts[i]
                if tmp[0] == '-s':
                        tmo = string.atoi(tmp[1])
                elif tmp[0] == '-b':
                        test = 1
		elif tmp[0] == '-n':
			fifo = 1

	pidfile.create('radar.pid')

        radar = RadarGui('Eldora Processors')

        radar.setTmo(tmo)

	if not test:
		radar.connect()

	if fifo:
		radar.nofifo()

        radar.mainloop()
        
if __name__ == '__main__':
        go()
