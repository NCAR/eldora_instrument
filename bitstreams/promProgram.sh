#!/bin/bash

echo -n "Enter the RR314 device number (0-1]: "
read dev

echo -n "Enter the bitstream file name: "
read -e bitstream

echo " "
echo "*** Warning, the prom programming may appear to hang the system for 2 minutes."
echo " "

~/redrapids/314/loadxsvf/bin/loadxsvf -dev $dev \
   -prom -xsvf $bitstream
   
echo " "
echo "*** Note - you must reboot the system in order to load the new PROM bitstream into the FPGA."


