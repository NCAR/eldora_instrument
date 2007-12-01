#!/bin/bash

echo -n "Enter the RR314 device number (0-1]: "
read dev

echo -n "Enter the bitstream file name: "
read -e bitstream

echo " "
echo "Programming device $dev with bisteram $bitstream"
echo " "
echo "*** Warning, the prom programming may appear to hang the system for 2 minutes."
echo " "

../redrapids/DSK-320-002-R05\ \(CA\ C\ API\ and\ Sample\ Code\)/loadxsvf/bin/loadxsvf -dev $dev \
   -prom -xsvf $bitstream
   
echo " "
echo "*** Note - you must reboot the system in order to load the new PROM bitstream into the FPGA."


