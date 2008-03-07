from xmlrpclib import *

class RPC(ServerProxy):
    def __init__(self,URI):
        ServerProxy.__init__(self, URI)
        
    def listMethods(self):
        # can throw an exception if the server 
        # is not available
        try:
            return self.system.listMethods()
        except Exception, e:
            raise e
            
    