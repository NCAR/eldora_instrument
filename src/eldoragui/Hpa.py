#!/usr/bin/python --
import time
import serial

class HPA_BITS:
    # combine all four status bytes to
    # match the following codes

    # states
    on          = 0x00000001
    cooldown  = 0x00000002
    warmup      = 0x00000004
    standby   = 0x00000008
    operate   = 0x00000010  
    # faults
    faulted   = 0x00000020
    interlock = 0x00000100
    rev_power = 0x00000200
    twt          = 0x00000400
    hvps      = 0x00000800
    pulse      = 0x00001000
    filament  = 0x00002000
    # position
    antenna   = 0x00010000
    load      = 0x00020000
    remote      = 0x00080000
    # more faults
    lowpress  = 0x00100000
    rf_arc      = 0x01000000
    hotrun      = 0x02000000
    crowbar   = 0x04000000

###################################################################            
class HPA:
    # command identifiers
        off = 0
        on = 1
        operate = 2
        standby = 3
        antenna = 4
        load = 5 
        status = 6
        commands = {
          on :       '\002\120\003\055',
          off :      '\002\160\003\015',
          operate :  '\002\117\003\056',
          standby :  '\002\123\003\052',
          antenna :  '\002\101\003\074',
          load :     '\002\114\003\061',
          status :   '\002\127\003\046' 
        }
        stateCodes = {
                      # HPA_BITS.on: 'On',
                      HPA_BITS.cooldown: 'Cooldown',
                      HPA_BITS.warmup: 'Warmup',
                      HPA_BITS.standby: 'Standby',
                      HPA_BITS.operate: 'Operate'
        }
        faultCodes = {
        HPA_BITS.faulted: 'Fault',
        HPA_BITS.interlock: 'Interlock',
        HPA_BITS.rev_power: 'Reverse Power',
        HPA_BITS.twt: 'TWT Overtemp',
        HPA_BITS.hvps: 'HVPS Overtemp',
        HPA_BITS.pulse: 'Pulse Input',
        HPA_BITS.filament: 'Filament',
        HPA_BITS.lowpress: 'Low Waveguide Pressure',
        HPA_BITS.rf_arc: 'RF Arc',
        HPA_BITS.hotrun: 'Hotrun Active',
        HPA_BITS.crowbar: 'Crowbar Fired'
        }
        positionCodes = {
          HPA_BITS.antenna: 'Antenna',
          HPA_BITS.load: 'Load'
        }

###################################################################
class Hpa:
    def __init__(self, portname):
        ''' This class sends control info to the selected
        HPA & receives status info back. 

        The antenna/dummy load switch is slow, so if one 
        asks for status right away it will still indicate 
        the old position. A delay of ~2 seconds seems to be
        enough for the switch to actually move.
        '''

        if portname != None:
            # the open may raise SerialException if it fails
            self.serialport = serial.Serial(port=portname,
                    bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    timeout=0.1,
                    xonxoff=0,
                    rtscts=0)
        else:
            self.serialport = None

        self.comms = 'Unknown'
        self.state = 'Unknown'
        self.faults = 'Unknown'
        self.position = 'Unknown'


###################################################################
    def status(self):
        commandResult = self.command(HPA.status)
        if commandResult != '':
            return commandResult
        try:
            s = ''
            while True:
                # get the next character
                try:
                     s = s + self.serialport.read(1)
                except Exception, e:
                    print 'exception reading HPA:', e
                    s = ''
                if len(s) == 0:
                    return 'HPA read failed'
                if (len(s)) > 10:
                    for c in s:
                        print hex(ord(c)), ' ',
                    print ' '
                    return 'Missing EOT character on HPA read'
                # watch for the EOT character
                if s[len(s) - 1] == '\003':
                    break

            # read the checksum   
            try:
                s = s + self.serialport.read(1)
            except Exception, e:
                s = ''
        except serial.SerialException, e:
            return str(e)

        if len(s) < 5:
            return 'Read after write only returned '+len(s)+' bytes, expecting 5'

        as = s[1:5] # We want the 4 status bytes.

        #for c in s:
        #    print hex(ord(c)),' ',
        #print 

        self.update_status(as)

        return ''

###################################################################
    def command(self, op):
        ''' Send a command to the hpa.
        Return a result message if there was a problem. If
        no problem, return an empty string.
        '''
        if self.serialport is None:
            return 'No serial port specified. Unable to send command'

        s = ''
        try:
            writeStatus = self.serialport.write(HPA.commands[op])            

        except serial.SerialException, e:
            # if an error writing or reading, return the exception message
            return str(e)

        return ''

###################################################################
    def update_status(self, stat):
        ''' stat is a list of four characters corresponding
        to bytes 1-4 of the status message
        '''

        status = 0
        for i in range(3, -1, -1):
            status = status*256 + ord(stat[i])

        # print 'status is ', hex(status)

        if status & HPA_BITS.remote:
            self.comms = 'Remote'
        else:
            self.comms = 'Local'

        self.state = ''
        for c in HPA.stateCodes.keys():
            if c & status:
                self.state = self.state + ' ' + HPA.stateCodes[c]

        self.position = ''
        for c in HPA.positionCodes.keys():
            if c & status:
                self.position = self.position + ' ' + HPA.positionCodes[c]

        self.faults = ''
        for c in HPA.faultCodes.keys():
            if c & status:
                if len(self.faults) == 0:
                    self.faults = HPA.faultCodes[c]
                else:
                    self.faults = self.faults + ', ' + HPA.faultCodes[c]

