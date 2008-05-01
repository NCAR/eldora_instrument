#!/local/bin/python
#
# Module:          cedit.py
# Original Author: Rich Neitzel
# Date:            $Date$
# Copywrited by the National Center for Atmospheric Research
# Tk based GUI for ELDORA displays.
#
# $Id$
# revision history
# ----------------
# $Log$
#

from Tkinter import *
from Dialog import *

from EmacsEntry import *
from Browser import *
from Prompter import *
from Parent import *
from IconWindow import *

class ColorEditor(Tk, Parent):
	def __init__(self, name = 'AGC Color Editor'):
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
                self.filemenubutton = Menubutton(self.menubar, {
                          'text': 'File',
                          'underline': 0,
                          Pack: {'side': 'left'}
                          })
                self.filemenu = Menu(self.filemenubutton)
                self.filemenubutton['menu'] = self.filemenu
                #
                # Setup callback.
                ccb = self.register(self.cmdCB)
                #
                # Add file menu items.
                self.filemenu.add('command', {
                          'label': 'Load',
			  'command': (ccb, 'load')
			  })
                self.filemenu.add('command', {
                          'label': 'Save',
			  'command': (ccb, 'save')
			  })
                self.filemenu.add('command', {
                          'label': 'Save as',
			  'command': (ccb, 'save as')
			  })
                self.filemenu.add('command', {
                          'label': 'Quit',
			  'command': (ccb, 'quit')
			  })
		#
		# Add actions menu.
		self.editmenubutton = Menubutton(self.menubar, {
                          'text': 'Edit',
                          'underline': 0,
                          Pack: {'side': 'left'}
                          })
                self.editmenu = Menu(self.editmenubutton)
                self.editmenubutton['menu'] = self.editmenu
		self.editmenu.add('command', {
			  'label': 'Clear',
			  'command': (ccb, 'clear')
			  })
		self.editmenu.add('command', {
			  'label': 'Undo',
			  'command': (ccb, 'undo')
			  })
		self.editmenu.add('command', {
			  'label': 'Grey',
			  'command': (ccb, 'grey')
			  })
		self.editmenu.add('command', {
			  'label': 'Copy left',
			  'command': (ccb, 'copy left')
			  })
		self.editmenu.add('command', {
			  'label': 'Copy right',
			  'command': (ccb, 'copy right')
			  })
		self.editmenu.add('command', {
			  'label': 'Next',
			  'command': (ccb, 'next')
			  })
		self.editmenu.add('command', {
			  'label': 'Previous',
			  'command': (ccb, 'previous')
			  })
		#
		# Now add the 1st control region.
		self.whichframe = Frame(self, {
			  'relief': 'raised',
			  'bd': 2,
			  Pack: {
			  'side': 'top',
			  'fill': 'both'
			  }})
		self.whichtbl = IntVar(self.whichframe)
		self.whichtbl.set(1)
		self.one = Radiobutton(self.typeframe,{
                          'relief': 'flat',
                          'text': 'Radial',
                          'variable': self.radartype.__str__(),
                          'value': '1',
                          Pack: {
                          'side': 'left',
                          'anchor': 'w'
                          }})
		self.two = Radiobutton(self.typeframe,{
                          'relief': 'flat',
                          'text': 'Radial',
                          'variable': self.radartype.__str__(),
                          'value': '2',
                          Pack: {
                          'side': 'left',
                          'anchor': 'w'
                          }})
		self.three = Radiobutton(self.typeframe,{
                          'relief': 'flat',
                          'text': 'Radial',
                          'variable': self.radartype.__str__(),
                          'value': '3',
                          Pack: {
                          'side': 'left',
                          'anchor': 'w'
                          }})
