#!/bin/bash

# get our environment, since kde will trash it
# when run from a taskbar button.
. ~/.bashrc

#  Run the gui
eldoragui.py

# Wait a couple seconds, in case we have been run 
# in a window from the taskbar, and the hapless SE
# needs to see diagnostic output.
sleep 2