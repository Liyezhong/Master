#!/bin/bash
####################################################################################
#Author: Soumya.D
#Date: 27-Jan-2014
#Version: 0.1
#Description: This script shall be called from Himalaya S.a.M software for 
#touch screen calibration.
###################################################################################

BIN_DIR=/home/Leica/Bin
killall himalaya_service
ts_calibrate
cd $BIN_DIR
./himalaya_service $1 -qws &
exit 0


