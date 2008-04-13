import subprocess

# 
# DiskStats will privide disk statistics
#
class DiskStats:
    def __init__(self):
        stats = self.stats()
        self.num = len(stats)
        
    def stats(self):
        cmd = ['df', '-l', '-h']
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
        dfout = p.stdout.read().split()
        dfout = dfout[7:]
        result = []
        for i in range(0, len(dfout), 6):
            percent = dfout[i+4]
            percent = percent.strip('%')
            prcnt = int(percent)
            result.append((dfout[i+5],prcnt ))
        return result
    
    def numDisks(self):
        return self.num
