#!/local/bin/python
#
# Module:          DispGbl.py
# Original Author: Richard Neitzel
# Date:            $Date$
#
# Variables, etc. needed in more then one module.
#
# $Id$
# revision history
# ----------------
# $Log$
# Revision 2.0  1996/06/28  20:51:38  thor
# *** empty log message ***
#
# Revision 1.4  1994/11/29  20:27:42  thor
# Added support for variable centered Dual display.
#
# Revision 1.3  1994/11/03  18:08:57  thor
# Added new items for long/short PRT.
#
# Revision 1.2  1994/09/12  16:09:49  thor
# Added new parameters.
#
# Revision 1.1  1994/09/02  21:08:16  thor
# Initial revision
#
#
class DispParameters:
    dbz = 0x1
    vr = 0x2
    snr = 0x4
    sw = 0x8
    power = 0x200
    vl = 0x80000
    vs = 0x100000
    sws = 0x200000
    swl = 0x400000
    ncpl = 0x800000
    dbzl = 0x1000000
    none = -1

class DispSides:
    left = 0
    right = 1
    farLeft = 2
    farRight = 4
    center = 8

class RealCmds:
    forward_radial = 0x10
    forward_horiz =  0x20
    forward_vert =   0x40
    aft_radial =     0x80
    aft_horiz =      0x100
    aft_vert =       0x200
    restart_disp =   0x400
    tmo_change =     0x800
    forward_dual =   0x1000
    aft_dual =       0x2000
    forward_raw =    0x4000
    aft_raw =        0x8000
    load_only =      0x80000

class RadarType:
    fore = 0
    aft = 1

class DispType:
    radial = 0
    vertical = 1
    dual = 2
    raw = 3
    horiz = 4
