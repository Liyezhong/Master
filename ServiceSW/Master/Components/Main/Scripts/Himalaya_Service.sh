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
SSH_DIR=/home/root/.ssh
PRIV_KEY=/home/root/.ssh/id_rsa
SSH_CONFIG=/home/root/.ssh/config
cd $BIN_DIR
diff id_rsa $PRIV_KEY 1>/tmp/.log 2>&1
IS_SAME=$?
if [ ! -f $PRIV_KEY ] || [ $IS_SAME -ne 0 ] ; then
	mkdir -p $SSH_DIR
	chmod 0700 $SSH_DIR
	cp -f id_rsa $PRIV_KEY
	chmod 0600 $PRIV_KEY

	echo "UserKnownHostsFile /dev/null" > $SSH_CONFIG
	echo "ConnectTimeout 15" >> $SSH_CONFIG
	echo "StrictHostKeyChecking no" >> $SSH_CONFIG
fi

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

