#!/local/bin/python
#
# Module:          master.py
# Original Author: Richard Neitzel
# Date:            $Date$
#
# Tk based GUI for master control window on ELDORA.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.8  1995/11/28  18:20:50  thor
# Changed to new IconWindow format.
#
# Revision 1.7  1995/05/26  19:35:38  thor
# Added code to allow for only one display being available.
#
# Revision 1.6  1995/02/08  15:50:20  thor
# Enabled second display. Fixed all references to self.headername.
#
# Revision 1.5  1994/12/29  15:48:35  thor
# Added support for a default header & checking for a vaild path name
# as well.
#
# Revision 1.4  1994/11/29  20:28:47  thor
# Added check & code to handle bad header files.
#
# Revision 1.3  1994/10/28  15:08:36  thor
# Fixed header stuff, added time setting.
#
# Revision 1.2  1994/10/21  16:18:08  thor
# Changed some labels, fixed broken exit, corrected write out of header
# and flight info.
#
# Revision 1.1  1994/10/14  21:46:06  thor
# Initial revision
#
#
from Tkinter import *
from Dialog import *

from EmacsEntry import *
from Browser import *
from Prompter import *
from IconWindow import *
from DispHeader import *

import sys
import Common
import MasterClnt
import FifoGbl

_default_header_dir = '/vxbin/radar/headers'

class MasterGui(Tk):
	def __init__(self, name = 'ELDORA Master Control'):
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
		ccb = self.register(self.cmdCB)
		cb = self.register(self.flightCB)
		self.tk.createcommand(cb,self.flightCB)

		self.pulldown.add('command', {
			  'label': 'Start',
			  'command': (ccb,'Start')
			  })
		self.pulldown.add('command', {
			  'label': 'Stop',
			  'command': (ccb,'Stop')
			  })
		self.pulldown.add('command', {
			  'label': 'Reconnect',
			  'command': (ccb, 'Reconnect')
			  })
		self.pulldown.add('command', {
			  'label': 'Change Flight Info',
			  'command': (cb,'Flight')
			  })
		ecb = self.register(self.exitCB)
		self.pulldown.add('command', {
			  'label': 'Quit',
			  'command': (ecb, 0)
			  })
		#
		# Add commonly used commands as buttons.
		self.buttonarea = Frame(self, {
			  'relief' : 'raised',
			  'bd': 2,
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})

		self.startbutton = Button(self.buttonarea, {
			  'text': 'Start',
			  'command': (ccb, 'Start'),
			  Pack: {
			  'side': 'left',
			  'padx': '20m',
			  'pady': '2m'
			  }})
		self.stopbutton = Button(self.buttonarea, {
			  'text': 'Stop',
			  'command': (ccb, 'Stop'),
			   Pack: {
			  'side': 'left',
			  'padx': '20m',
			  'pady': '2m'
			  }})
		# For when the minirims work!
		#self.alignbutton = Button(self.buttonarea, {
		#	  'text': 'Align',
		#	  'command': (ccb, 'Align'),
		#	  Pack: {
		#	  'side': 'left',
		#	  'padx': '20m',
		#	  'pady': '2m'
		#	  }})
		#
		# Add the header file interaction items.
		self.interact = Frame(self, {
			  'relief' : 'raised',
                          'bd': 2,
                          Pack: {
                          'side': 'top',
			  'fill': 'both'
			  }})

		fcb = self.register(self.fileCB)

		self.filebutton = Button(self.interact, {
			  'text': 'Select a new header file',
			  'command': (fcb, 'Select'),
			  Pack: {
			  'side': 'top',
			  'padx': '2m',
			  'pady': '2m'
			  }})

		self.headername = StringVar(self)
		self.headername.set(_default_header_dir)

		self.filetext = EmacsEntry(self.interact, {
			  'relief': 'sunken',
			  'width': '80',
			  'textvariable': self.headername.__str__(),
			  Pack: {
			  'side': 'top',
			  'padx': '2m',
			  'pady': '2m'
			  }})

		self.filetext.bind('<Return>',self._handleCR)
		#
		# Someday! # Add switch for minirims.
		#self.miniframe = Frame(self)
		#self.miniframe['relief'] = 'raised'
		#self.miniframe['bd'] = 2
		#self.miniframe.pack(side=TOP,fill=BOTH)

		#self.ministate = IntVar(self.miniframe)
		#self.ministate.set(1)

		#self.minion = Radiobutton(self.miniframe)
		#self.minion['releif'] = 'flat'
		#self.minion['text'] = 'Wait for Minirims'
		#self.minion['variable'] = self.ministate.__str__()
		#self.minion['value'] = 1
		#self.minion['command'] = self.setmini
		#self.minion.pack(side=Left,fill=BOTH)

		#self.minioff = Radiobutton(self.miniframe)
		#self.minioff['releif'] = 'flat'
		#self.minioff['text'] = 'Don\'t wait for Minirims'
		#self.minioff['variable'] = self.ministate.__str__()
		#self.minioff['value'] = 1
		#self.minioff['command'] = self.setmini
		#self.minioff.pack(side=Left,fill=BOTH)

		# Add the message area at bottom.
		self.messageframe = Frame(self, {
                          'relief' : 'raised',
                          'bd': 2,
                          Pack: {
                          'side': 'top',
			  'fill': 'both'
			  }})

		self.msg = Label(self.messageframe, {
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
		#
		# Look at me!
		self.filetext.focus()

		# Set up the popup file browser.
		self.filebrowser = FileBrowser(self)
		self.filebrowser.setBanner('Select a Header File')
		self.filebrowser.setLabel('Header File:')
		self.filebrowser.setOkFunc(self.myOk)
		self.filebrowser.setCRFunc(self.myCR)
		self.filebrowser.setDcFunc(self.myDC)
		#
		# Set up header viewer.
		self.viewer = DispHeader()
		#
		# Initialize the flight stuff.
		self.prompter = Prompter()
		self.prompter.setItem('ELD_TEST')
		self.prompter.setWidth('8')
		self.prompter.setMsg('Enter up to 8 characters (no spaces!):')
		#
		# Now do the icon.
		self.goodIcon()
		#
		# Use two displays.
		self.displayCount = 2
		#
		# Set this so it works for the first connect() call.
		self.cmdobj = None

		#
		# Open fifos.
		self.noFifo = 0
		self.Fifo1 = open(FifoGbl.FifoName.one,'w')
		self.Fifo2 = open(FifoGbl.FifoName.two,'w')
	#
	# These are the action callbacks.
	#
	def cmdCB(self, data):
		self.sendCmd(data)

	def flightCB(self, data):
		self.prompter.popup()

	def fileCB(self, name):
		dir = self.headername.get()
		if not posixpath.isdir(dir):
			dir = posixpath.dirname(dir)
		if not posixpath.isdir(dir):
			dir = _default_header_dir
		self.filebrowser.setDirectory(dir)
		self.filebrowser.popup()

	def exitCB(self, junk):
		self.destroy()
		self.quit()
		import sys
		sys.exit()
	#
	# The next two methods are attached as user specified
	# callbacks in the FileBrowser. Note that they also directly
	# call the private default FileBrowser callbacks.
	#
	def myOk(self):
		self.filebrowser._defaultOk()
		self.headername.set(self.filebrowser.getItem())
		# Reclaim the focus.
		self.filetext.focus()

	def myCR(self):
		self.filebrowser._defCR()
		self.headername.set(self.filebrowser.getItem())
		self.filetext.focus()

	def myDC(self):
		self.filebrowser._defaultOk()
		self.headername.set(self.filebrowser.getItem())
		self.filebrowser.deiconify()
		self.viewer.popup(self.headername.get())
	#
	# This handles a return in the text entry. If the name is a directory
	# the file browser is popuped up.
	#
	def _handleCR(self,junk):
		dir = self.headername.get()
		if not posixpath.isdir(dir):
			dir = posixpath.dirname(dir)
		if not posixpath.isdir(dir):
			dir = _default_header_dir
		self.filebrowser.setDirectory(dir)
		self.filebrowser.popup()
	#
	# These are user callable functions.
	# 
	def setHeaderFile(self,name):
		self.headername.set(name)

	def setFlight(self,flt):
		self.prompter.setItem(flt)

	def oneDisp(self):
		self.displayCount = 1

	def sendCmd(self, cmd):
		if cmd == 'Start':
			f = self.prompter.getItem()
			# Note that we only want 8 chars of flight annotation.
			flight = ''
			if len(f) > 8:
				flight = f[0:8]
			else:
				flight = f
			self.sendStart(flight)
			return
		elif cmd == 'Stop':
			status = self.cmdobj.stop()

			if not self.noFifo:
				self.Fifo1.write('0\n')
				self.Fifo2.write('0\n')
			
			if status == 'OK':
				self.msg['text'] = 'System stopped.'
			else:
				status = 'Stop to ' + status + 'failed.'
				self.msg['text'] = status
				Dialog(self,{
					  'title': 'Warning',
					  'text': status,
					  'bitmap': 'warning',
					  'default': 0,
					  'strings': ('Noted',)
					  })
				self.badIcon()
				return
		elif cmd == 'Align':
			self.cursorSet('watch')
			status = self.cmdobj.align()
			self.cursorSet('top_left_arrow')
			if (status < 0):
				self.msg['text'] = 'Align failed.'
				Dialog(self,{
					  'title': 'Warning',
					  'text': 'Align failed.',
					  'bitmap': 'warning',
					  'default': 0,
					  'strings': ('Noted',)
					  })
				self.badIcon()
				return
			else:
			    self.msg['text'] = 'Aligned OK.'
		else:
			self.connect()
		self.goodIcon()
	#
	# This got too complex to do in line. Note that to keep it
	# readable I have used !=, else the indenting would be
	# unbearable.
	def sendStart(self,fltinfo):
		# First let's check for a valid header file. If it's not
		# popup the browser.
		hname = self.headername.get()
		if not posixpath.isfile(hname):
			Dialog(self,{
                                  'title': 'Warning',
                                  'text':
				  'Sorry, the header file does not exist',
                                  'bitmap': 'warning',
                                  'default': 0,
                                  'strings': ('Noted',)
                                  })
                        self.fileCB('junk')
			return
		self.cmdobj.setFlight(fltinfo)
		hstatus = self.cmdobj.sendHeader(hname)
		if hstatus != 'OK':
			self.msg['text'] = 'Sending the header file to ' + hstatus + ' failed.'
			Dialog(self,{
                                  'title': 'Warning',
                                  'text': self.msg['text'],
                                  'bitmap': 'warning',
                                  'default': 0,

                                  'strings': ('Noted',)
                                  })
			self.badIcon()
			return
		status = self.cmdobj.start()
		if status == 'OK':
			self.msg['text'] = 'System running...'
		else:
			status = 'Start to ' + status + 'failed.'
			self.msg['text'] = status
			Dialog(self,{
                                  'title': 'Warning',
                                  'text': status,
                                  'bitmap': 'warning',
                                  'default': 0,
                                  'strings': ('Noted',)
                                  })
			self.badIcon()
			return

		self.goodIcon()

	def setmini(self):
		if self.ministate.get():
			self.cmdobj.miniOn()
		else:
			self.cmdobj.miniOff()

	def warning(self,msg):
		self.badIcon()
		self.msg['text'] = msg
		Dialog(self,{
			  'title': 'Warning!',
			  'text': msg,
			  'bitmap': 'warning',
			  'default': 0,
			  'strings': ('Noted',)
			  })

	def goodIcon(self):
		if self.icon == None:
			self.icon = IconWindow(self,bitmap='master.xbm')
		self.icon.background('aquamarine')

	def badIcon(self):
                self.icon.background('red')

	def connect(self):
	    # Target order is:
	    # display2, display1, record, hskp, fore, aft
	    # Display2 is first so it can be dropped if there is only one
	    # display.
	    targets = ['disp2', 'disp1', 'tape', 'hskp', 'mini', 'fore', 'aft']
	    self.cmdobj = MasterClnt.MasterClnt(targets)
	    if self.cmdobj == None:
		print 'master.py: Unable to find object, exiting.'
		sys.exit(1)
	    self.cmdobj.numDisplays(self.displayCount)

	    self.cmdobj.connect()
	    #self.setmini()
	    self.cmdobj.miniOff() # Remove later when minirims work.
	    self.msg['text'] = 'Connected'


	def cursorSet(self,cursor):
		for c in self.children.values():
			print 'here'
			c.config({'cursor':cursor})

	def nofifo(self):
		self.noFifo = 1
			
#
# This function is the default if the program is invoked as a runable script.
#
def go():
	global master
	master = MasterGui('Eldora Master Control')
	import getopt, string, pidfile

	i = len(sys.argv)

	opts, args = getopt.getopt(sys.argv[1:],'f:h:on')

	for i in range(len(opts)):
                tmp = opts[i]
                if tmp[0] == '-f':
			master.setFlight(tmp[1])
		elif tmp[0] == '-h':
			master.setHeaderFile(tmp[1])
		elif tmp[0] == '-o':
			master.oneDisp()
		elif tmp[0] == '-n':
			master.nofifo()
			
	pidfile.create('master.pid')

	master.connect()
	master.mainloop()
	
if __name__ == '__main__':
	go()
