#!/usr/bin/python

import sys
from time import *
from socket import *

from SimpleXMLRPCServer import *

########################################################
#
# These are our rpc callable functions
#
class RPCfunctions:
	def __init__(self, name):
		self.name = name

	def id(self):
		return self.name

	def status(self):
		return "I'm alive at " + asctime(localtime())
	
	def radarStart(self):
		return "radar start command received"
		
	def radarStop(self):
		return "radar stop command received"

########################################################
#
# Create a class derived from an RPC server.
#
class XMLRPCServer(SimpleXMLRPCServer):
	def __init__(self, *args):
		# turn off message logging
		SimpleXMLRPCServer.__init__(self, args, logRequests=0)
		# enable introspection
		SimpleXMLRPCServer.register_introspection_functions(self)

	def server_bind(self):
		# overide the server_bind function so that we can set 
		# some socket parameters.
		self.socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
		SimpleXMLRPCServer.server_bind(self)

########################################################
#

# Instantiate a server
#
server = XMLRPCServer("localhost", 60000)

server.register_instance(RPCfunctions("RPC"))

try:
	server.serve_forever()
except Exception, e:
	# catch the server intteruption so that we
	# can shutdown without ugly diagnostics.
	print "Server exception: ", e



