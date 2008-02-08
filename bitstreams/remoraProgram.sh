#!/bin/bash

dev=0

programEEPROM='n'

echo -n "Program the eeprom [y/N]? "
read -e programEEPROM

programEEPROM="_$programEEPROM"

if [ $programEEPROM = '_y' ]  ||  [ $programEEPROM = '_Y' ]
then
   target='fpgaflash0'
else
   target='fpga0'
fi

echo -n "Enter the bitstream file name: "
read -e bitstream

echo " "
echo "Programming device $dev, target $target,  with $bitstream"
echo " "

$DSP21KSF/bin/bitldr $target -d $dev -f $bitstream

