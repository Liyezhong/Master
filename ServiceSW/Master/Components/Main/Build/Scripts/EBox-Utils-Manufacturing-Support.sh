#!/bin/bash
#****************************************************************************/
#! \file Scripts/EBox-Utils-Manufacturing-Support.sh
#
#  \brief Script containing utilities for manufacturing support 
#         Features:
#         1) Test connectivity to SaM server
#         2) Send reports to SaM server
#         3) Check for availability of new ASB firmwares in SaM server
#         4) Fetch new ASB firmwares from SaM server
#  \Note: This script makes some assumption, they are:
#         1) Access to SaM server doesn't authentication (courtesy ssh keys)
#         2) SaM server shall contain ASB firmwares which is of same
#            version as that present in /home/Leica/Firmware folder
#            or higher.
#
#   $Version: $ 1.0
#   $Date:    $ 2014-07-25
#   $Author:  $ Nikhil Kamath
#
#  \b Company:
#
#       Leica Biosystems Nussloch GmbH.
#
#  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
#  This is unpublished proprietary source code of Leica. The copyright notice
#  does not evidence any actual or intended publication.
#
#/
#****************************************************************************/


FIRMWARE_PATH=/home/Leica/Firmware
BASE_PATH=/home/Leica
TIME_OUT=10 #second

#######################################################
# Ping test for SaM server
# Arguments: $1 -> SaM server IP
# Returns: 0 test success, everything else is failure
#######################################################
ping_SaM_server()
{
    ping -c 3 $1 >/dev/null 2>&1
    return $?
}

#######################################################
# File copy test -> confirms if SaM software is able
# to copy file and delete file on SaM server
# Arguments:    $1 -> File to copy 
#               $2 -> SaM_Server_User_Name@SaM_Server_IP
#               $3 -> Target path , where file is
#                     supposed to be copied to.
# Returns: 0 test success
#          "CopyingFailed" if copying file failed
#          "DeleteFailed" if delete test file failed
#          "TestFileNotFound" if test file is not found. 
#######################################################
test_file_copy_to_SaM_server()
{
    local CopyingFailed=1
    local DeleteFailed=2
    local TestFileNotFound=3
    [ -f $1 ] || return $TestFileNotFound
    scp -o ConnectTimeout=$TIME_OUT "$1" "$2":"$3" >/dev/null 2>&1
    ScpReturnVal=$?
    [ $ScpReturnVal -ne 0 ] && return $CopyingFailed
    local FileName=$(basename $1)
    #delete test file, we don't want to flood SaM server
    #with test files B-)
    ssh -o ConnectTimeout=$TIME_OUT "$2" "rm $3/$FileName" >/dev/null 2>&1
    [ $? -ne 0 ] && return $DeleteFailed || return 0
}

#######################################################
# Send SaM test reports to SaM server
# Arguments:    $1 -> Report file to copy
#               $2 -> SaM_Server_User_Name@SaM_Server_IP
#               $3 -> Target path , where file is
#                     supposed to be copied to.
# Returns: 0 for success, everything else is failure
#######################################################
send_reports_to_SaM_server()
{
    scp -o ConnectTimeout=$TIME_OUT "$1" "$2":"$3" >/dev/null 2>&1
    return $?
}

#######################################################
# Fetch firmware files from SaM server
# Arguments:   $1 -> SaM_Server_User_Name@SaM_Server_IP
#              $2 -> Firmware folder path on SaM server
#               
# Returns: 0 for success, everything else is failure
#######################################################
copy_firmware_files_from_SaM_server()
{
    local CopyFailed=1
    local Md5sumFailed=2
    scp -o ConnectTimeout=$TIME_OUT -r "$1":"$2" "$BASE_PATH" >/dev/null 2>&1
    [ $? -ne 0 ] && return $CopyFailed
    cd $FIRMWARE_PATH && md5sum -c .md5sum.txt >/dev/null 2>&1
    [ $? -eq 0 ] && return 0 || return $Md5sumFailed
}

#######################################################
# Check for availability of new firmware
# Arguments:   $1 -> SaM_Server_User_Name@SaM_Server_IP
#              $2 -> Firmware folder path on SaM server
#               
# Returns: 0 if new firmware is available, 
#           everything else means new firmware not
#           available.
#######################################################
check_if_SaM_server_has_new_firmware()
{
    #login to SaM server, and compare md5sum of our firmwares against
    #firmwares on SaM server.
    ssh -o ConnectTimeout=$TIME_OUT "$1" "cat $2/.md5sum.txt" | diff - "$FIRMWARE_PATH"/.md5sum.txt > /dev/null 2>&1
    #invert return value
    [ $? -eq 0 ] && return 1 || return 0
}

$@

