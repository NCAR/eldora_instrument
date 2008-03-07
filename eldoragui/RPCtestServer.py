#!/usr/bin/python

import sys
from time import *
from socket import *

from SimpleXMLRPCServer import *

class XMLRPCServer(SimpleXMLRPCServer):
	def __init__(self, *args):
		SimpleXMLRPCServer.__init__(self, args, logRequests=0)
		SimpleXMLRPCServer.register_introspection_functions(self)

	def server_bind(self):
		self.socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
		SimpleXMLRPCServer.server_bind(self)

# Create server
server = XMLRPCServer("localhost", 60000)

class RPCfunctions:
	def __init__(self, name):
		self.name = name

	def id(self):
		return self.name

	def status(self):
		return "I'm alive at " + asctime(localtime())

server.register_instance(RPCfunctions("RPC"))

try:
	server.serve_forever()
except Exception, e:
	print "Exception is ", e



