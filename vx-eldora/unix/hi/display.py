#!/local/bin/python
#
# Module:          display.py
# Original Author: Richard Neitzel
# Date:            $Date$
#
# Tk based GUI for ELDORA displays.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.10  1996/03/25  22:37:10  thor
# Added raw support, updated pipe reads.
#
# Revision 1.9  1995/11/28  18:20:50  thor
# Changed to new IconWindow format.
#
# Revision 1.8  1995/02/14  17:06:45  thor
# Made comm failure msg clearer.
#
# Revision 1.7  1995/02/08  15:49:16  thor
# Added error handling if open for write fails.
#
# Revision 1.6  1994/11/29  20:27:42  thor
# Added support for variable centered Dual display.
#
# Revision 1.5  1994/10/28  15:07:45  thor
# Changed another default value.
#
# Revision 1.4  1994/10/24  15:05:29  thor
# Changed default display parameters.
#
# Revision 1.3  1994/09/23  15:16:58  thor
# Now sends correct reconnect msg.
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

from EmacsEntry import *
from Browser import *
from Prompter import *
from DispEditor import *
from IconWindow import *

import posix
import posixpath
from DispGbl import *

import dispCmd

# This is the order in which the C structure is built.
_print_key_order = (
          'type', 'radar', 'nparams', 'p1', 'p2', 'p3', 'side', 'radius', 'tmo',
 'colortbl',
          'max1', 'max2', 'max3', 'min1', 'min2', 'min3', 'distance', 'size',
          'top', 'bottom'
)

class DisplayGui(Tk):
        def __init__(self, name = 'ELDORA Display Control'):
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
			  'command': (ccb, 'Reconnect')
			  })
		self.pulldown.add('command', {
			  'label': 'Load file',
			  'command': (ccb, 'Load')
			  })
		self.pulldown.add('command', {
			  'label': 'Save file',
			  'command': (ccb, 'Save')
			  })
		self.pulldown.add('command', {
			  'label': 'Save as file',
			  'command': (ccb, 'As')
			  })
		self.pulldown.add('command', {
			  'label': 'Edit',
			  'command': (ccb, 'Edit')
			  })
		self.pulldown.add('command', {
			  'label': 'Apply',
			  'command': (ccb, 'Apply')
			  })
		self.pulldown.add('command', {
			  'label': 'Quit',
			  'command': (ccb, 'Exit')
			  })
                #
                # Add commonly used commands as buttons.
                self.buttonarea = Frame(self, {
                          'relief' : 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
		self.load = Button(self.buttonarea,{
			  'command': (ccb, 'Load'),
			  'text': 'Load',
			  Pack: {
                          'side': 'left',
                          'padx': '20m',
                          'pady': '2m'
                          }})
		self.save = Button(self.buttonarea,{
			  'command': (ccb, 'Save'),
			  'text': 'Save',
			  Pack: {
                          'side': 'left',
                          'padx': '20m',
                          'pady': '2m'
                          }})
		self.edit = Button(self.buttonarea,{
			  'command': (ccb, 'Edit'),
			  'text': 'Edit',
			  Pack: {
                          'side': 'left',
                          'padx': '20m',
                          'pady': '2m'
                          }})
		self.apply = Button(self.buttonarea,{
			  'command': (ccb, 'Apply'),
			  'text': 'Apply',
			  Pack: {
                          'side': 'left',
                          'padx': '20m',
                          'pady': '2m'
                          }})
                #
                # Add the parameter file interaction items.
                self.interact = Frame(self, {
                          'relief' : 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }})
                self.paramname = StringVar(self)
		# Give a totally bogus default name to keep the user
		# from grabbing something by mistake.
                self.paramname.set('.')
                self.filetext = EmacsEntry(self.interact, {
                          'relief': 'sunken',
                          'width': '80',
                          'textvariable': self.paramname.__str__(),
                          Pack: {
                          'side': 'top',
                          'padx': '2m',
                          'pady': '2m'
                          }})

                self.filetext.bind('<Return>',self._handleCR)
                #
                # Add the message area at bottom.
                self.messageframe = Frame(self, {
                          'relief' : 'raised',
                          'bd': '2',
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
		self.goodIcon()
                #
                # Look at me!
                self.filetext.focus()
		#
		# Dictionary of action procs. & causes.
		self.actions = {
			  'Reconnect': self.reconnect,
			  'Load': self.loadfile,
			  'Save': self.savefile,
			  'As': self.saveas,
			  'Edit': self.editparams,
			  'Apply': self.applyparams,
			  'Exit': self.bye
			  }
		#
		# Initialize FileBrowser.
		self.fb = FileBrowser(self)
		self.fb.setBanner('Select a Parameter file name:')
		self.fb.setLabel('Parameter file:')
		self.fb.setOkFunc(self.myOk)
		self.fb.setCRFunc(self.myCR)
		self.fb.setDcFunc(self.myOk)
		self.fb.list.bind('<Double-Button-2>',self.double2)
		#
		# Initialize display editors.
		self.editor1 = DispEditor(self,unit='1')
		self.editor2 = DispEditor(self,unit='2')
		#
		# Some rational default values.
		self.paramdict1 = {
			  'type': DispType.dual,
			  'radar': RadarType.fore,
			  'nparams': 2,
			  'p1': DispParameters.dbz,
			  'p2': DispParameters.vr,
			  'p3': DispParameters.none,
			  'side': DispSides.center,
			  'radius': 511,
			  'tmo': 0,
			  'colortbl': 0,
			  'max1': 70.0,
			  'max2': 0.0,
			  'max3': 0.0,
			  'min1': -30.0,
			  'min2': 0.0,
			  'min3': 0.0,
			  'distance': 0.0,
			  'size': 0.0,
			  'top': 30.0,
			  'bottom': 0.0
			  }
		self.paramdict2 = {
			  'type': DispType.dual,
			  'radar': RadarType.aft,
			  'nparams': 2,
			  'p1': DispParameters.dbz,
			  'p2': DispParameters.vr,
			  'p3': DispParameters.none,
			  'side': DispSides.center,
			  'radius': 511,
			  'tmo': 0,
			  'colortbl': 0,
			  'max1': 70.0,
			  'max2': 0.0,
			  'max3': 0.0,
			  'min1': -30.0,
			  'min2': 0.0,
			  'min3': 0.0,
			  'distance': 0.0,
			  'size': 0.0,
			  'top': 30.0,
			  'bottom': 0.0
			  }
		# Propigate downward.
		self.editor1.set(self.paramdict1)
		self.editor2.set(self.paramdict2)
		# Don't send commands.
		self.nosend = 0

		self.editor1.setApplyFunc(self.applyparams)
		self.editor2.setApplyFunc(self.applyparams)
	#
	# Callbacks & bindings.
	#
	def cmdCB(self,cause):
		if self.actions.has_key(cause):
			self.func = self.actions[cause]
			self.func()
		else:
			print 'Unknown callback!'

	def _handleCR(self,e):
		pass
	#
	# Overloaded callbacks for FileBrowser.
	# 

	#
	# Action functions.
	#
	def reconnect(self):
		if not self.nosend:
			self.sendCmd(RealCmds.load_only)

	def loadfile(self):
		self.readfile()

	def savefile(self):
		self.writefile()

	def saveas(self):
		self.browser()
		self.writefile()

	def editparams(self):
		self.editor1.popup()
		self.editor2.popup()

	def applyparams(self):
		self.editor1.get()
		self.editor2.get()
		self.sendCmd(0)

	def bye(self):
		self.destroy()
		self.quit()
		import sys
		sys.exit(0)
	#
	# These are the functions that work with the FileBrowser.
	#
	def browser(self):
		self.fb.setFile(self.paramname.get())
		self.fb.popup()

	def myOk(self):
		self.fb._defaultOk()
		self.paramname.set(self.fb.getItem())

	def myCR(self):
                self.fb._defCR()
		self.paramname.set(self.fb.getItem())

	def double2(self,junk):
		self.viewer.popup(self.fb.getItem())
	#
	# The I/O functions. Readfile will popup a Dialog if the file
	# doesn't exist.
	#
	def writefile(self):
		self.editor1.get()
		self.editor2.get()
		try:
			file = open(self.paramname.get(),'w')
		except IOError:
			Dialog(self,{
				  'title': 'Warning',
				  'text': 'Sorry, but that file cannot be opened',
				  'bitmap': 'warning',
				  'default': 0,
				  'strings': ('Noted',)
				  })
			return
		for i in _print_key_order:
			item = self.paramdict1[i]
			if type(item) != type('a'):
				file.write(str(item) + '\n')
			else:
				file.write(item + '\n')
		for i in _print_key_order:
			item = self.paramdict2[i]
			if type(item) != type('a'):
				file.write(str(item) + '\n')
			else:
				file.write(item + '\n')
		file.close()

	def readfile(self):
		fn = self.paramname.get()
		if posixpath.isfile(fn):
			try:
				file = open(fn,'r')
			except IOError:
				Dialog(self,{
					  'title': 'Warning',
					  'text': 'Sorry, but that file cannot be opened',
					  'bitmap': 'warning',
					  'default': 0,
					  'strings': ('Noted',)
					  })
				return
			for i in _print_key_order:
				tmp = file.readline()
				item = tmp[:len(tmp)-1]
				self.paramdict1[i] = item
			for i in _print_key_order:
				tmp = file.readline()
				item = tmp[:len(tmp)-1]
				self.paramdict2[i] = item
			file.close()
			self.editor1.reset()
			self.editor2.reset()
		else:
			Dialog(self,{
				  'title': 'Warning',
				  'text': 'Sorry, that is not a readable file',
				  'bitmap': 'warning',
				  'default': 0,
				  'strings': ('Noted',)
				  })
			self.browser()
	#
	# Utilities
	#
	def setParamfile(self,file):
		self.paramname.set(file)

	def setTarget(self,target):
		self.target = target

	def setBuildOnly(self):
		self.nosend = 1

	def setTimeout(self,tmo):
		self.tmo = tmo

	def goodIcon(self):
		if self.icon == None:
			self.okimage = BitmapImage('disp.xbm',file='disp.xbm')
			self.errimage = BitmapImage('disperr.xbm',
				  file='disperr.xbm')
                        self.icon = IconWindow(self,image=self.okimage)
		else:
			self.icon.bitmap(image=self.okimage)
                self.icon.background('aquamarine')

        def badIcon(self):
                self.icon.bitmap(image=self.errimage)
                self.icon.background('red')
	#
	# The intercommunication functions.
	#
	def sendCmd(self,cmd):
		if self.nosend:
			return 1
		self.cmdobj.set(self.paramdict1,self.paramdict2)
		s = self.cmdobj.send(cmd)
		if s:
			self.badComm()
			return 0
		self.msg['text'] = 'System OK.'
		return 1

	def badComm(self):
		msg = 'Communication to system ' + self.target + ' failed.'
		self.msg['text'] = msg
		Dialog(self,{
			  'title': 'Warning!',
			  'text': msg,
			  'bitmap': 'warning',
			  'default': 0,
			  'strings': ('Noted',)
			  })

	def getStatus(self, junk = None):
		s = self.cmdobj.status()
		if s:
		    self.badComm()
		#
		# Should we call ourself again?
		if self.tmo > 0:
			self.after(self.tmo,self.getStatus,None)

	def connect(self):
		if not self.nosend:
			self.cmdobj = dispCmd.DispCmd(self.target)
			self.cmdobj.set(self.paramdict)
			if self.sendCmd(RealCmds.load_only) == 1:
				self.msg['text'] = 'Connected.'
				if self.tmo > 0:
					self.after(self.tmo,self.getStatus,
						  None)

#
# This function is the default if the program is invoked as a runable
# script. It assumes that you will connect to a target and a default
# status update rate of 0 seconds.
#
def go():
        global display			# In case we need to smash it directly.

	build_only = 0
	tmo = 0
	target = ''

        import sys, getopt, string

        i = len(sys.argv)
	opts, args = getopt.getopt(sys.argv[1:],'t:bs:')

	for i in range(len(opts)):
		tmp = opts[i]
		if tmp[0] == '-s':
			tmo = string.atoi(tmp[1])
		elif tmp[0] == '-b':
			build_only = 1
		else:
			target = tmp[1]

	if build_only:
		target = 'build only.'
        display = DisplayGui('Eldora Display Control for ' + target)

	if not build_only:
		if target == '':
			print 'Sorry, you must specify a target.'
			sys.exit(1)
		display.setTarget(target)
		import pidfile
		pidfile.create(target + '.pid')
	else:
		display.setBuildOnly()

	if tmo > 0:
		tmo = tmo * 1000
	display.setTimeout(tmo)
	display.connect()
        display.mainloop()
        
if __name__ == '__main__':
        go()
