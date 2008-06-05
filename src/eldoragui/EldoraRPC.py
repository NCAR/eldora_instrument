from xmlrpclib import *
import PyQt4.QtCore

class EldoraRPC(PyQt4.QtCore.QThread):
    def __init__(self, appName, URI):
        PyQt4.QtCore.QThread.__init__(self)
        self.appName = appName
        self.URI = URI
        
    def run(self):
        self.server = ServerProxy(self.URI)
        
    def listMethods(self):
        # can throw an exception if the server 
        # is not available
        try:
            return self.server.system.listMethods()
        except Exception, e:
            raise e
        
    def methodSignature(self, name):
        # can throw an exception if the server 
        # is not available
        try:
            return self.server.system.methodSignature(name)
        except Exception, e:
            raise e
            
    