#!/bin/bash
####################################################################################
#Author: Sunny Qu
#Date: 06-08-2014
#Version: 0.1
#Description: This script shall be called from EBox-StartAndMonitorMainSoftware for himalaya service software
#himalaya_service.sh
###################################################################################

BIN_DIR=/home/Leica/Bin
FLAG_FILE="/tmp/.ts_calibrate"

cd $BIN_DIR
./himalaya_service -qws

while true
do
	if [ ! -f $FLAG_FILE ] ; then
		break;
	else
		ts_calibrate
		PARAM=`cat $FLAG_FILE | tr -d '\n\r'`
		rm $FLAG_FILE
		./himalaya_service $PARAM -qws
	fi
done

exit 0

