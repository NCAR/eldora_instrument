#!/usr/local/bin/python
#
# Today.py: python cgi program
# Formats output from posix 'cal' and 'fortune' into HTML, 
# with current day in <STRONG><U>nderline 
# 
# Unfortunately, much of the formatting: which Header level,
# number of spaces of padding, etc. is rather ad-hoc, but I 
# didn't know how else to do it. ( "HTML -- Is it a markup
# language or is it a presentation language? ... It's BOTH!" :-)
# 
# Changes:
# changed fortune to <PRE> - sometimes it spits out poems!
#     However - that makes <em> text for fortune too wide, so that is now 
#     wrapped in <h4>. Not good markup, but it works OK on MY client. <sigh>
#     (You may want to change that.) Misc. other changes to the markup output. 
# added spaces around day string - else single digit days highlght all occurances
# of that digit. Didn't catch this 'till the 1st of the month. 
#   (and then had to wrap lines with spaces too.)
# <TITLE> is now Today: date - it looks nicer if it's displayed. 
#
# 
# ToDo: 
#    add 'calendar'  - simple but we don't have a departmental calendar
#
#  			--  Steve Majewski <sdm7g@Virginia.EDU> 

import time,posix,string,sys

FROM_GMT = 14400.0 # sorry this has to be hardwired, but in some non-interactive
		   # context's - like running from a cgi-script, the TZ 
		   # environment variable is not set on AIX. If you have the same
		   # problem, set this to your local value. 


FORTUNE = '/usr/games/bin/fortune'		# you need these programs 
CALENDAR = '/usr/bin/cal' 		# 

if time.timezone == 0 : 
	time.TIME = time.time() - FROM_GMT 
else:	time.TIME = time.time()
day = ' ' + repr(time.localtime( time.TIME )[2]) + ' '  # padding 
calout = posix.popen( CALENDAR, 'r' ).readlines()
date=time.ctime(time.TIME)

print 'Content-type: text/html\n'	# extra new-line REQUIRED
print '<TITLE>Today: ' +date+ ' </TITLE>'
print '<pre><HR><H2>           ', date, '</H2><HR>' 
for line in calout:
	line = ' '*3 + line[:-1] + ' ' + line[-1]
	i = string.find( line, day )
	if i >= 0:
		print line[:i] +'<STRONG><U>'+ line[i:i+len(day)] +'</U></STRONG>'+line[i+len(day):]
	else:	print line


print '<h4>'
for line in posix.popen( FORTUNE , 'r' ).readlines(): 
	print line,
print '</h4></pre>'

# This is really dead code when run from CGI. You can delete it if you like. 
if 'DEBUG' in sys.argv[1:] :
	if posix.environ.has_key('TZ') and posix.environ['TZ']:
		print posix.environ['TZ'] 
	else:  print 'NO TZ' 
