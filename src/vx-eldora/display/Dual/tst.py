#!/local/bin/python --
#
# Module:          tst.py
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

def ymajor(x1,y1,x2,y2,dx,dy):

	slope = (dx << 16) / dy

	x = x1 << 16

	while y1 < y2:
		z = x >> 16
		print z,y1
		y1 = y1 + 1
		x = x + slope
	print x2,y1

def xmajor(y1,x1,y2,x2,dx,dy):

	slope = (dx << 16) / dy

	x = x1 << 16

	while y1 < y2:
		z = x >> 16
		print y1,z
		y1 = y1 + 1
		x = x + slope
	print y1,x2


def line(x1,y1,x2,y2):
	dx = x2 - x1
	dy = y2 - y1

	ax = dx

	if ax < 0:
		ax = -ax

	ay = dy

	if ay < 0:
		ay = -ay

	if ax > ay:
		if x1 > x2:
			xmajor(x2,y2,x1,y1,-dx,-dy)
		else:
			xmajor(x1,y1,x2,y2,dx,dy)
	else:
		if y1 < y2:
			ymajor(x1,y1,x2,y2,dx,dy)
		else:
			ymajor(x2,y2,x1,y1,-dx,-dy)
