#!/local/bin/python --
#
# Module:          hpa.py
# Original Author: Rich Neitzel
# Date:            $Date$
# Copywrited by the National Center for Atmospheric Research
# Tk based GUI for ELDORA displays.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 1.1  1996/04/08  21:47:51  thor
# Initial revision
#
#
#   This is the GUI portion of the HPA control program. 
#

from Tkinter import *
from StaticEntry import *
from IconWindow import *
from Dialog import *

from Hpa import *

import posix

class HpaGui(Tk,Hpa):
	def __init__(self,unit,portname,testing=0):
                #
		# First set up serial port.
		#
		self.testing = testing
		if not self.testing:
			Hpa.__init__(self,portname)
                self.msg_dict = { HPA.on : 'Power On',
                          HPA.off : 'Power Off',
                          HPA.operate : 'Operating',
                          HPA.standby : 'Standby',
			  HPA.warmup: 'Warming up',
			  HPA.cooldown: 'Cool down'
			  }

                # Now let's set up the GUI stuff.
                #
                # We'll call ourself HpaUnitx.
		self.unit = unit
                Tk.__init__(self,None,None,'HpaUnit' + self.unit)
		self.title('HPA Control for Unit ' + self.unit)
                #
                # Create the menubar & its pulldowns.
		frame_cnf = {
                          'relief' : 'raised',
                          'bd': '2',
                          Pack: {
                          'side': 'top',
                          'fill': 'both'
                          }}
                self.menubar = Frame(self, frame_cnf)
                self.menubutton = Menubutton(self.menubar)
		self.menubutton['text'] =  'File'
		self.menubutton['underline'] = 0
		self.menubutton.pack(side=LEFT)

                self.pulldown = Menu(self.menubutton)
                self.menubutton['menu'] = self.pulldown
                #
                # Setup callback.
                ccb = self.register(self.cmdCB)
                #
                # Add menu items.
                self.pulldown.add('command',label = 'Exit',
                          command = (ccb, 'Exit'))
		#
		# Add buttons for common controls.
		#
		self.cmdframe = Frame(self, frame_cnf)
		
		self.label = Label(self.cmdframe,text='HPA Commands:')

		poncb = self.register(self.poncb)
		self.poweron = Button(self.cmdframe,relief=FLAT,
			  text='On',command=poncb)
		poffcb = self.register(self.poffcb)
		self.poweroff = Button(self.cmdframe,relief=FLAT,
			  text='Off',command=poffcb)
		oponcb = self.register(self.oponcb)
		self.opon = Button(self.cmdframe,relief=FLAT,
			  text='Operate',command=oponcb)
		opoffcb = self.register(self.opoffcb)
		self.opoff = Button(self.cmdframe,relief=FLAT,
			  text='Standby',command=opoffcb)

		self.label.pack(side=TOP,fill=X)
		self.poweron.pack(side=LEFT,expand=TRUE,fill=X)
		self.poweroff.pack(side=LEFT,expand=TRUE,fill=X)
		self.opon.pack(side=LEFT,expand=TRUE,fill=X)
		self.opoff.pack(side=LEFT,expand=TRUE,fill=X)

		#
		# Add status area.
		#
		self.msgframe = Frame(self,frame_cnf)
		self.msg = StaticEntry(self.msgframe,TOP)
		self.msg.setLabel('Current Status for HPA ' + self.unit)
		self.msg.text()
		self.msg.setData('unknown')

		self.loadframe = Frame(self,frame_cnf)
		self.loadlabel = Label(self.loadframe,text='Load Position:')
		self.antenna_state = IntVar(self)
		self.antenna_button = Radiobutton(self.loadframe,
			  text='Antenna',variable=self.antenna_state,
			  command=self.checkcb,value=HPA_STATES.antenna)
		self.dummy_button = Radiobutton(self.loadframe,
			  text='Dummy Load',variable=self.antenna_state,
			  command=self.checkcb,value=HPA_STATES.dummyload)

		self.loadlabel.pack(side=TOP,fill=X)
		self.antenna_button.pack(side=LEFT,expand=TRUE,fill=X)
		self.dummy_button.pack(side=LEFT,expand=TRUE,fill=X)
		#
		# Set up icons.
		#
		self.okimage = BitmapImage('OkIcon',file='hpa_ok.xbm')
		self.errimage = BitmapImage('ErrIcon',file='hpa_err.xbm')
		self.icon = None
		self.good()
		self.faulted = 0
		self.current_status = HPA_STATES.off
		self.load_pos = HPA_STATES.dummyload
		self.parse_dict = { 0: HPA.off, 1: HPA.on, 2: HPA.operate,
			  3: HPA.standby, 4: HPA.antenna, 5: HPA.dummyload,
			  6: HPA.status, 7: HPA.warmup, 8: HPA.cooldown }

		file = unit + '.pid'
		f = open(file,"w")
		f.write(`posix.getpid()`)
		f.close()
		
	def __del__(self):
		print 'bye'

	def cmdCB(self,junk):
		self.destroy()
		self.quit()

	def poncb(self):
		self.sendcmd(HPA.on)

	def poffcb(self):
		self.sendcmd(HPA.off)

	def oponcb(self):
		self.sendcmd(HPA.operate)

	def opoffcb(self):
		self.sendcmd(HPA.standby)


	def sendcmd(self,op):
		cmd = self.parse_dict[op]
		s = self.docommand(cmd)
		if s != 0:
			self.post_warning()
	       	else:
			self.update()

	def checkcb(self):
		if not self.testing:
			if self.antenna_state.get() == HPA_STATES.antenna:
				self.sendcmd(HPA.antenna)
			else:
				self.sendcmd(HPA.dummyload)
		else:
			self.load_pos = self.antenna_state.get()
		self.check_pos()

	def check_pos(self):
		self.antenna_state.set(self.load_pos)
		if self.load_pos == HPA_STATES.dummyload:
			self.dummy_button['background'] = 'blue'
			self.antenna_button['background'] = 'aquamarine'
		else:
			self.dummy_button['background'] = 'aquamarine'
			self.antenna_button['background'] = 'blue'

	def periodic(self,junk):
		if self.sendcmd(HPA.status):
			self.post_warning()
		else:
			self.update()
		if self.tmo > 0:
			self.after(self.tmo,self.periodic,None)

	def set_tmo(self,tmo = 0):
		self.tmo = tmo * 1000

	def update(self):
		if self.faulted:
			self.msg.setData('Faulted - %#x' % self.faults )
			self.bad()
		else:
			self.msg.setData(self.state_dict[self.current_status])
			self.good()
		self.check_pos()

	def bad(self):
		self.msg.setLabelBackground('red')
		self.msg.setEntryBackground('red')
		self.icon.set_image(image=self.errimage)
		self.icon.background('red')

	def good(self):
		self.msg.setLabelBackground('aquamarine')
		self.msg.setEntryBackground('aquamarine')
		if self.icon == None:
			self.icon = IconWindow(self,image=self.okimage)
		else:
			self.icon.set_image(image=self.okimage)
		self.icon.background('aquamarine')

	def post_warning(self):
		msg = 'Communication to Hpa #' + self.unit
		self.msg.setData(msg)
		#Dialog(self,{
			#  'title': 'Warning!',
			 # 'bitmap': 'warning',
			 # 'default': 0,
			 # 'strings': 'Noted',
			 # 'text': msg
			 # })

def go():

	import sys, getopt, string

	tmo = 0
	port = ''
	testing = 0
	unit = ''

	i = len(sys.argv)
        opts, args = getopt.getopt(sys.argv[1:],'p:s:tu:')

        for i in range(len(opts)):
                tmp = opts[i]
                if tmp[0] == '-s':
                        tmo = string.atoi(tmp[1])
		elif tmp[0] == '-p':
			port = tmp[1]
                elif tmp[0] == '-t':
                        testing = 1
		elif tmp[0] == '-u':
			unit = tmp[1]
	if port == '':
		if args != []:
			port = args[0]

	if port == '' or unit == '':
		print 'Usage: hpa.py -u unit [-s seconds] [-t] [-p port] [port]'
		sys.exit(1)

	hpa = HpaGui(unit,port,testing)

	hpa.set_tmo(tmo)

	if not testing:
		hpa.periodic(None)	# This forces a status check.

	hpa.mainloop()

	del hpa

	print 'all done'


if __name__ == '__main__':
        go()
