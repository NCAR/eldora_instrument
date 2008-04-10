from xmlrpclib import *

class EldoraRPC:
    def __init__(self, URI):
        self.server = ServerProxy(URI)
        
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
        
    def radarStart(self):
        # can throw an exception if the server 
        # is not available
        try:
            param = {'Config':'convective', 
                     'Mode':'simulate', 
                     'PRF':'1000',
                     'Gates':'500',
                     'TSGates':'5',
                     'TSFirst':'100'}
            return self.server.start(param)
        except Exception, e:
            raise e
        
    def radarStop(self):
        # can throw an exception if the server 
        # is not available
        try:
            return self.server.stop()
        except Exception, e:
            raise e
        
    def shutdown(self):
        # can throw an exception if the server 
        # is not available
        try:
            return self.server.shutdown()
        except Exception, e:
            raise e
        
    def status(self):
        # can throw an exception if the server 
        # is not available
        try:
            return self.server.status()
        except Exception, e:
            raise e
        
        
            
    