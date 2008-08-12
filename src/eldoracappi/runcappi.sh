#!/bin/bash

# get our environment, since kde will trash it
# when run from a taskbar button.
. ~/.bashrc

# mount the cappi data dir
mount /cappi

# find the last cappi file
cappifile=`ls -1 -t -r /cappi/cappi_*.nc | tail -n 1`

echo $cappifile

#  Run the cappi
eldoracappi --input $cappifile

# Wait a couple seconds, in case we have been run 
# in a window from the taskbar, and the hapless SE
# needs to see diagnostic output.
sleep 2