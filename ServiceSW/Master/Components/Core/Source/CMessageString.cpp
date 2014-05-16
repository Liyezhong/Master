/****************************************************************************/
/*! \file Core/Source/CMessageString.cpp
 *
 *  \brief CMessageString class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-03-26
 *   $Author:  $ Srivathsa HH, Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <Core/Include/CMessageString.h>
#include <QApplication>

namespace Service {


QString CMessageString::MSG_X_REFERENCERUN_SUCCESS;                  //!< X-Axis reference run success
QString CMessageString::MSG_X_REFERENCERUN_FAILURE;                  //!< X-Axis reference run fail message
QString CMessageString::MSG_YZ_MOVETOSAFEPOS_SUCCESS;                //!< YZ arm move to safe position successful
QString CMessageString::MSG_YZ_MOVETOSAFEPOS_FAILURE;                //!< YZ arm move to safe position fail message
QString CMessageString::MSG_YZ_REFERENCE_RUN_SUCCESS;                //!< YZ arm reference run successful
QString CMessageString::MSG_YZ_REFERENCE_RUN_FAILURE;                //!< YZ arm reference run fail message
QString CMessageString::MSG_YZ_RFIDREAD_FAILURE;                     //!< YZ arm RFID read failed
QString CMessageString::MSG_TRANSFERSTN_REFERENCE_RUN_SUCCESS;       //!< Transfer station reference run successful
QString CMessageString::MSG_TRANSFERSTN_REFERENCE_RUN_FAILURE;       //!< Transfer station reference run fail message
QString CMessageString::MSG_TRANSFERSTN_OPEN_SUCCESS;                //!< Transfer station reference run successful
QString CMessageString::MSG_TRANSFERSTN_OPEN_FAILURE;                //!< Transfer station reference run fail message
QString CMessageString::MSG_TRANSFERSTN_CLOSE_SUCCESS;               //!< Transfer station reference run successful
QString CMessageString::MSG_TRANSFERSTN_CLOSE_FAILURE;               //!< Transfer station reference run fail message

QString CMessageString::MSG_DRAWER_REFERENCE_RUN_SUCCESS;             //!< Drawer reference run successful
QString CMessageString::MSG_DRAWER_REFERENCE_RUN_FAILURE;             //!< Drawer reference run fail message
QString CMessageString::MSG_DRAWER_INIT_SUCCESS;                      //!< Drawer initialize successful message
QString CMessageString::MSG_DRAWER_OPEN_SUCCESS;                      //!< Drawer open successful message
QString CMessageString::MSG_DRAWER_CLOSE_SUCCESS;                     //!< Drawer closed successful message
QString CMessageString::MSG_DRAWER_RFID_SUCCESS;                      ///< Drawer RFID read successful message
QString CMessageString::MSG_DRAWER_INIT_FAILURE;                      //!< Drawer initialization failed message
QString CMessageString::MSG_DRAWER_OPEN_FAILURE;                      //!< Drawer open failed message
QString CMessageString::MSG_DRAWER_CLOSE_FAILURE;                     //!< Drawer close failed message
QString CMessageString::MSG_DRAWER_INVALID_COMMAND;                   //!< Drawer invalid command

QString CMessageString::MSG_AGITATION_INIT_SUCCESS;                   //!< Agitation initialization successful
QString CMessageString::MSG_AGITATION_INIT_FAILURE;                   //!< Agitation initialization failed
QString CMessageString::MSG_AGITATION_SETSPEED_SUCCESS;               //!< Agitation set speed successful
QString CMessageString::MSG_AGITATION_SETSPEED_FAILURE;               //!< Agitation set speed failed
QString CMessageString::MSG_AGITATION_START_SUCCESS;                  //!< Agitation start rotation successful
QString CMessageString::MSG_AGITATION_START_FAILURE;                  ///< Agitation start rotation failed

QString CMessageString::MSG_OVENLID_REFERENCE_RUN_SUCCESS;            //!< Oven lid reference run successful
QString CMessageString::MSG_OVENLID_REFERENCE_RUN_FAILURE;            //!< Oven lid reference run failed
QString CMessageString::MSG_OVEN_INIT_SUCCESS;                        //!< Oven initialization successful
QString CMessageString::MSG_OVEN_INIT_FAILURE;                        //!< Oven initialization failed
QString CMessageString::MSG_OVEN_LIDOPEN_SUCCESS;                     //!< Oven lid/cover open successful
QString CMessageString::MSG_OVEN_LIDOPEN_FAILURE;                     //!< Oven lid/cover open failed
QString CMessageString::MSG_OVEN_LIDCLOSE_SUCCESS;                    //!< Oven lid/cover close successful
QString CMessageString::MSG_OVEN_LIDCLOSE_FAILURE;                    //!< Oven lid/cover close failed
QString CMessageString::MSG_OVEN_INVALID_COMMAND;                     //!< Oven Invalid command

QString CMessageString::MSG_SLIDEID_INIT_SUCCESS;                     //!< Photo sensor test successful
QString CMessageString::MSG_SLIDEID_INIT_FAILURE;                     //!< Photo sensor test failed
QString CMessageString::MSG_SLIDEID_STARTCOUNT_FAILURE;               //!< Start count failed
QString CMessageString::MSG_SLIDEID_STRING;                           //!< Level Sensor calibration failure information messgae

QString CMessageString::MSG_XYZ_INIT_FAILURE;                          //!< XYZ init module failed
QString CMessageString::MSG_XYZ_RACKRFID_SUCCESS;                      //!< Rack RFID
QString CMessageString::MSG_XYZ_MOVETOSTN_FAILURE;                     //!< Move to station failed
QString CMessageString::MSG_XYZ_CARRYTOSTN_FAILURE;                    //!< Carry Rack to station failed
QString CMessageString::MSG_XYZ_LOWERRACK_FAILURE;                     //!< Lower rack failed
QString CMessageString::MSG_XYZ_ATTACHRACK_FAILURE;                    //!< Attach rack failed
QString CMessageString::MSG_XYZ_LIFTRACK_FAILURE;                      //!< Lift rack failed
QString CMessageString::MSG_XYZ_DETACHRACK_FAILURE;                    //!< Detach rack failed
QString CMessageString::MSG_XYZ_COUNTSLIDES_SUCCESS;                   //!< Slide count Successful
QString CMessageString::MSG_XYZ_COUNTSLIDES_FAILURE;                   //!< Slide count failed

QString CMessageString::MSG_EXHAUST_INIT_SUCCESS;                      //!< Exhaust device init successful
QString CMessageString::MSG_EXHAUST_INIT_FAILURE;                      //!< Exhaust device init failed
QString CMessageString::MSG_EXHAUST_FANON_SUCCESS;                     //!< Exhaust device FAN ON successful
QString CMessageString::MSG_EXHAUST_FANON_FAILURE;                     //!< Exhaust device FAN ON failed
QString CMessageString::MSG_EXHAUST_FANOFF_SUCCESS;                    //!< Exhaust device FAN OFF successful
QString CMessageString::MSG_EXHAUST_FANOFF_FAILURE;                    //!< Exhaust device FAN OFF failed
QString CMessageString::MSG_EXHAUST_AIRFLOWSTATUS_OK;                  //!< Exhaust device air flow OK status
QString CMessageString::MSG_EXHAUST_AIRFLOWSTATUS_NOK;                 //!< Exhaust device air flow NOK status
QString CMessageString::MSG_EXHAUST_FANSTATUS_OK;                      //!< Exhaust device Fan current OK status
QString CMessageString::MSG_EXHAUST_FANSTATUS_NOK;                     //!< Exhaust device Fan current NOK status
QString CMessageString::MSG_EXHAUST_INVALID_COMMAND;                   //!< Invalid command

QString CMessageString::MSG_WATERSTN_INIT_FAILURE;                     //!< Water device init failed
QString CMessageString::MSG_WATERSTN_OPEN_SUCCESS;                     //!< Water valve open successful
QString CMessageString::MSG_WATERSTN_OPEN_FAILURE;                     //!< Water valve open falied
QString CMessageString::MSG_WATERSTN_CLOSE_SUCCESS;                    //!< Water valve close successful
QString CMessageString::MSG_WATERSTN_CLOSE_FAILURE;                    //!< Water valve close failed
QString CMessageString::MSG_WATERSTN_BLOCKDETECTED;                    //!< Water valve close successful
QString CMessageString::MSG_WATERSTN_BLOCKREMOVED;                     //!< Water valve close failed
QString CMessageString::MSG_WATERSTN_INVALID_COMMAND;                  //!< Invalid command

QString CMessageString::MSG_LIGHTDEVICE_INIT_FAILURE;                  //!< Light device init failed
QString CMessageString::MSG_LIGHTDEVICE_ON_SUCCESS;                    //!< Light device switch on successful
QString CMessageString::MSG_LIGHTDEVICE_OFF_SUCCESS;                   //!< Light device switch off successful
QString CMessageString::MSG_LIGHTDEVICE_ON_FAILURE;                    //!< Light device switch on failed
QString CMessageString::MSG_LIGHTDEVICE_OFF_FAILURE;                   //!< Light device switch off failed
QString CMessageString::MSG_LIGHTDEVICE_INVALID_COMMAND;               //!< Invalid command

QString CMessageString::MSG_HOODDEVICE_INIT_FAILURE;                   //!< Coverhood init failed
QString CMessageString::MSG_HOODDEVICE_OPEN;                           //!< Cover/Hood is open
QString CMessageString::MSG_HOODDEVICE_CLOSE;                          //!< Cover/Hood is closed

QString CMessageString::MSG_XYZ1_CANID;                                //!< X Arm left CANID
QString CMessageString::MSG_XYZ2_CANID;                                //!< X Arm right CANID
QString CMessageString::MSG_LOADER_CANID;                              //!< Loader CANID
QString CMessageString::MSG_UNLOADER_CANID;                            //!< Unloader CANID
QString CMessageString::MSG_OVEN_CANID;                                //!< Oven CANID
QString CMessageString::MSG_HCUVETTES_CANID;                           //!< Heated cuvettes CANID
QString CMessageString::MSG_AGITATION_CANID;                           //!< Agitation CANID
QString CMessageString::MSG_RACKTRANSFER_CANID;                        //!< Rack transfer CANID
QString CMessageString::MSG_EXHAUST_CANID;                             //!< Exhaust device CANID
QString CMessageString::MSG_WATER_CANID;                               //!< Water device CANID
QString CMessageString::MSG_HOOD_CANID;                                //!< Cover/Hood CANID
QString CMessageString::MSG_LIGHT_CANID;                               //!< Light device CANID
QString CMessageString::MSG_SLIDEID_CANID;                             //!< Slide ID CANID
QString CMessageString::MSG_CONSUMABLES_CANID;                         //!< Consumables CANID
QString CMessageString::MSG_INVALID_CANID_COMMAND;                     //!< Unknown CANID command

QString CMessageString::CANID_XYZ1;                                    //!< CANID XYZ1
QString CMessageString::CANID_XYZ2;                                    //!< CANID XYZ2
QString CMessageString::CANID_LOADER;                                  //!< Loader CANID
QString CMessageString::CANID_UNLOADER;                                //!< Unloader CANID
QString CMessageString::CANID_OVEN;                                    //!< Oven CANID
QString CMessageString::CANID_HCUVETTES;                               //!< Heated Cuvettes CANID
QString CMessageString::CANID_RACKTRANSFER;                            //!< Rack transfer CANID
QString CMessageString::CANID_SLIDEID;                                 //!< SlideID CANID
QString CMessageString::CANID_AGITATION;                               //!< Agitation CANID
QString CMessageString::CANID_EXHAUST;                                 //!< Exhaust CANID
QString CMessageString::CANID_WATER;                                   //!< Water station CANID
QString CMessageString::CANID_HOOD;                                    //!< Hood CANID
QString CMessageString::CANID_LIGHT;                                   //!< Light CANID
QString CMessageString::CANID_CONSUMABLES;                             //!< Consumables CANID

QString CMessageString::MSG_XYZ1_FIRMWAREINFO;                         //!< YZ Firmware Information
QString CMessageString::MSG_XYZ2_FIRMWAREINFO;                         //!< YZ Firmware Information
QString CMessageString::MSG_LOADER_FIRMWAREINFO;                       //!< Loader Firmware Information
QString CMessageString::MSG_UNLOADER_FIRMWAREINFO;                     //!< Unloader Firmware Information
QString CMessageString::MSG_OVEN_FIRMWAREINFO;                         //!< Oven Firmware Information
QString CMessageString::MSG_HCUVETTES_FIRMWAREINFO;                    //!< Heated Cuvettes Firmware Information
QString CMessageString::MSG_AGITATION_FIRMWAREINFO;                    //!< Agitation Firmware Information
QString CMessageString::MSG_RACKTRANSFER_FIRMWAREINFO;                 //!< Rack transfer Firmware Information
QString CMessageString::MSG_EXHAUST_FIRMWAREINFO;                      //!< Exhaust Firmware Information
QString CMessageString::MSG_WATER_FIRMWAREINFO;                        //!< water station Firmware Information
QString CMessageString::MSG_HOOD_FIRMWAREINFO;                         //!< Cover/Hood Firmware Information
QString CMessageString::MSG_LIGHT_FIRMWAREINFO;                        //!< Light device Firmware Information
QString CMessageString::MSG_SLIDEID_FIRMWAREINFO;                      //!< Slide ID Firmware Information
QString CMessageString::MSG_CONSUMABLES_FIRMWAREINFO;                  //!< Consumables Firmware Information
QString CMessageString::MSG_INVALID_FIRMWAREINFO_COMMAND;              //!< Unknown Firmware Info command

QString CMessageString::MSG_DEVICEDATACONTAINERS_MISSING;              //!< Device data containers missing
QString CMessageString::MSG_DEVICECOMMAND_TIMEOUT;                     //!< Device command timeout

QString CMessageString::MSG_ENDTEST_RESULT;                            //!< End test result message
QString CMessageString::MSG_HEATINGTEST_RESULT;                        //!< Heating test result message
QString CMessageString::MSG_RFIDCONSUMABLES_RESULT;                    //!< RFID Consumables information message
QString CMessageString::MSG_ENDTEST_COMPLETED;                         //!< End test completed message

QString CMessageString::MSG_DEVICE_CALIBRATION_SUCCESS;                //!< Device calibration success
QString CMessageString::MSG_DEVICE_CALIBRATION_FAILURE;                //!< Device calibration failure

QString CMessageString::MSG_LSENSOR_CALIBRATION_SUCCESS;               //!< Level Sensor calibration success information message
QString CMessageString::MSG_LSENSOR_CALIBRATION_FAILURE;               //!< Level Sensor calibration failure information messgae

QString CMessageString::MSG_EXHAUST_FAN1_CURRENTOK;                    //!< Fan 1 current OK message
QString CMessageString::MSG_EXHAUST_FAN1_CURRENTNOK;                   //!< Fan 2 current OK message
QString CMessageString::MSG_EXHAUST_FAN2_CURRENTOK;                    //!< Fan 1 current OK message
QString CMessageString::MSG_EXHAUST_FAN2_CURRENTNOK;                   //!< Fan 2 current OK message

QString CMessageString::MSG_RFIDCONSUMABLES_INIT_FAILURE;              //!< RFID consumables initialization failed
QString CMessageString::MSG_CALIBRATION_FAILURE;                       //!< Calibration failure
QString CMessageString::MSG_CALIBRATION_SUCCESS;                       //!< Calibration success
QString CMessageString::MSG_CALIBRATION_UNKNOWNCOMMAND;                //!< Unknown Calibration Command
QString CMessageString::MSG_MANUFACTURINGTEST_RESULT;                  //!< Manufacturing test result
QString CMessageString::MSG_HEATINGTEST_FAILURE;                       //!< Heating test failed message

QString CMessageString::MSG_ASB1_0_FWUPDATE_FAILURE;                   //!< ASB1_0_Firmware update failed
QString CMessageString::MSG_ASB1_1_FWUPDATE_FAILURE;                   //!< ASB1_1_Firmware update failed
QString CMessageString::MSG_ASB2_0_FWUPDATE_FAILURE;                   //!< ASB2_0_Firmware update failed
QString CMessageString::MSG_ASB2_1_FWUPDATE_FAILURE;                   //!< ASB2_1_Firmware update failed
QString CMessageString::MSG_ASB2_2_FWUPDATE_FAILURE;                   //!< ASB2_2_Firmware update failed
QString CMessageString::MSG_ASB3_0_FWUPDATE_FAILURE;                   //!< ASB3_0_Firmware update failed
QString CMessageString::MSG_ASB4_0_FWUPDATE_FAILURE;                   //!< ASB4_0_Firmware update failed
QString CMessageString::MSG_ASB4_1_FWUPDATE_FAILURE;                   //!< ASB4_1_Firmware update failed
QString CMessageString::MSG_ASB5_0_FWUPDATE_FAILURE;                   //!< ASB5_0_Firmware update failed
QString CMessageString::MSG_ASB6_0_FWUPDATE_FAILURE;                   //!< ASB6_0_Firmware update failed
QString CMessageString::MSG_ASB7_0_FWUPDATE_FAILURE;                   //!< ASB7_0_Firmware update failed
QString CMessageString::MSG_ASB12_0_FWUPDATE_FAILURE;                  //!< ASB12_0_Firmware update failed
QString CMessageString::MSG_ASB13_0_FWUPDATE_FAILURE;                  //!< ASB13_0_Firmware update failed

QString CMessageString::MSG_CRITICAL_DEVICE_FAILURE;                   //!< Critical device error
QString CMessageString::MSG_DEVICE_NOTYET_READY;                       //!< Devices not ready yet
QString CMessageString::MSG_DEVICE_SHUTDOWN;                           //!< Devices not ready yet
QString CMessageString::MSG_SERIALNUMBER_ERROR;                        //!< Set module serial number error message
QString CMessageString::MSG_DEVICE_RELEASE_PRESSURE_FAILED;            //!< Release pressure failed.
QString CMessageString::MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_FAILED; //!< Pressure sensor calibration failed.
QString CMessageString::MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_SUCCESS; //!< Pressure sensor calibration successfull.

void CMessageString::RetranslateUI()
{
    /****************************************************************************/
    MSG_X_REFERENCERUN_SUCCESS = QApplication::translate("Service", "X Axis Reference Run Successful", 0, QApplication::UnicodeUTF8);                            //!< X-Axis reference run success
    MSG_X_REFERENCERUN_FAILURE = (QApplication::translate("Service", "ERROR: X Axis reference run Failed!", 0, QApplication::UnicodeUTF8));                      //!< X-Axis reference run fail message
    MSG_YZ_MOVETOSAFEPOS_SUCCESS = (QApplication::translate("Service", "YZ Arm Move to Safe position Successful", 0, QApplication::UnicodeUTF8));                //!< YZ arm move to safe position successful
    MSG_YZ_MOVETOSAFEPOS_FAILURE = (QApplication::translate("Service", "ERROR: YZ Arm Move to Safe position Failed!", 0, QApplication::UnicodeUTF8));            //!< YZ arm move to safe position fail message
    MSG_YZ_REFERENCE_RUN_SUCCESS = (QApplication::translate("Service", "YZ Reference Run Successful", 0, QApplication::UnicodeUTF8));                            //!< YZ arm reference run successful
    MSG_YZ_REFERENCE_RUN_FAILURE = (QApplication::translate("Service", "ERROR: YZ reference run Failed!", 0, QApplication::UnicodeUTF8));                        //!< YZ arm reference run fail message
    MSG_YZ_RFIDREAD_FAILURE = (QApplication::translate("Service", "ERROR: RFID read Failed!", 0, QApplication::UnicodeUTF8));                                    //!< YZ arm RFID read failed
    MSG_TRANSFERSTN_REFERENCE_RUN_SUCCESS = (QApplication::translate("Service", "Rack Transfer Reference Run Successful", 0, QApplication::UnicodeUTF8));        //!< Transfer station reference run successful
    MSG_TRANSFERSTN_REFERENCE_RUN_FAILURE = (QApplication::translate("Service", "ERROR: Rack Transfer Reference Run Failed!", 0, QApplication::UnicodeUTF8));    //!< Transfer station reference run fail message
    MSG_TRANSFERSTN_OPEN_SUCCESS = (QApplication::translate("Service", "Rack Transfer Open successful", 0, QApplication::UnicodeUTF8));                          //!< Transfer station open successful
    MSG_TRANSFERSTN_OPEN_FAILURE = (QApplication::translate("Service", "ERROR: Rack Transfer Open Failed!", 0, QApplication::UnicodeUTF8));                      //!< Transfer station open fail message
    MSG_TRANSFERSTN_CLOSE_SUCCESS = (QApplication::translate("Service", "Rack Transfer Close successful", 0, QApplication::UnicodeUTF8));                        //!< Transfer station close successful
    MSG_TRANSFERSTN_CLOSE_FAILURE = (QApplication::translate("Service", "ERROR: Rack Transfer Close Failed!", 0, QApplication::UnicodeUTF8));                    //!< Transfer station close fail message

    /****************************************************************************/
    MSG_DRAWER_REFERENCE_RUN_SUCCESS = (QApplication::translate("Service", "Drawer Reference Run Successful", 0, QApplication::UnicodeUTF8));                    //!< Drawer reference run successful
    MSG_DRAWER_REFERENCE_RUN_FAILURE = (QApplication::translate("Service", "ERROR: Drawer Reference Run Failed!", 0, QApplication::UnicodeUTF8));                //!< Drawer reference run fail message
    MSG_DRAWER_INIT_SUCCESS = (QApplication::translate("Service", "Drawer Initialized successfuly", 0, QApplication::UnicodeUTF8));                              //!< Drawer initialize successful message
    MSG_DRAWER_OPEN_SUCCESS = (QApplication::translate("Service", "Drawer Opened successfully", 0, QApplication::UnicodeUTF8));                                  //!< Drawer open successful message
    MSG_DRAWER_CLOSE_SUCCESS = (QApplication::translate("Service", "Drawer Closed successfully", 0, QApplication::UnicodeUTF8));                                 //!< Drawer closed successful message
    MSG_DRAWER_RFID_SUCCESS = (QApplication::translate("Service", "Drawer RFID read successful", 0, QApplication::UnicodeUTF8));                                 ///< Drawer RFID read successful message
    MSG_DRAWER_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Drawer Initialization Failed !", 0, QApplication::UnicodeUTF8));                       //!< Drawer initialization failed message
    MSG_DRAWER_OPEN_FAILURE = (QApplication::translate("Service", "ERROR: Drawer Open Failed !", 0, QApplication::UnicodeUTF8));                                 //!< Drawer open failed message
    MSG_DRAWER_CLOSE_FAILURE = (QApplication::translate("Service", "ERROR: Drawer Close Failed !", 0, QApplication::UnicodeUTF8));                               //!< Drawer close failed message
    MSG_DRAWER_INVALID_COMMAND = (QApplication::translate("Service", "ERROR: Drawer Invalid Command !", 0, QApplication::UnicodeUTF8));                          //!< Drawer invalid command

    /****************************************************************************/
    MSG_AGITATION_INIT_SUCCESS = (QApplication::translate("Service", "Agitation Initialization successful", 0, QApplication::UnicodeUTF8));                      //!< Agitation initialization successful
    MSG_AGITATION_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Agitation Initialization Failed!", 0, QApplication::UnicodeUTF8));                  //!< Agitation initialization failed
    MSG_AGITATION_SETSPEED_SUCCESS = (QApplication::translate("Service", "Agitation SetSpeed successful", 0, QApplication::UnicodeUTF8));                        //!< Agitation set speed successful
    MSG_AGITATION_SETSPEED_FAILURE = (QApplication::translate("Service", "ERROR: Set Speed Failed!", 0, QApplication::UnicodeUTF8));                             //!< Agitation set speed failed
    MSG_AGITATION_START_SUCCESS = (QApplication::translate("Service", "Agitation Start rotation Successful", 0, QApplication::UnicodeUTF8));                     //!< Agitation start rotation successful
    MSG_AGITATION_START_FAILURE = (QApplication::translate("Service", "ERROR: Start rotation Failed!", 0, QApplication::UnicodeUTF8));                           ///< Agitation start rotation failed

    /****************************************************************************/
    MSG_OVENLID_REFERENCE_RUN_SUCCESS = (QApplication::translate("Service", "Oven Lid Reference Run Successful", 0, QApplication::UnicodeUTF8));                 //!< Oven lid reference run successful
    MSG_OVENLID_REFERENCE_RUN_FAILURE = (QApplication::translate("Service", "ERROR: Oven Lid Reference Run Failed!", 0, QApplication::UnicodeUTF8));             //!< Oven lid reference run failed
    MSG_OVEN_INIT_SUCCESS = (QApplication::translate("Service", "Oven Initialized successfully", 0, QApplication::UnicodeUTF8));                                 //!< Oven initialization successful
    MSG_OVEN_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Oven Initialization failed!", 0, QApplication::UnicodeUTF8));                            //!< Oven initialization failed
    MSG_OVEN_LIDOPEN_SUCCESS = (QApplication::translate("Service", "Oven Lid/Cover Opened successfully", 0, QApplication::UnicodeUTF8));                         //!< Oven lid/cover open successful
    MSG_OVEN_LIDOPEN_FAILURE = (QApplication::translate("Service", "ERROR: Oven Lid/Cover Open Failed!", 0, QApplication::UnicodeUTF8));                         //!< Oven lid/cover open failed
    MSG_OVEN_LIDCLOSE_SUCCESS = (QApplication::translate("Service", "Oven Lid/Cover Closed successfully", 0, QApplication::UnicodeUTF8));                        //!< Oven lid/cover close successful
    MSG_OVEN_LIDCLOSE_FAILURE = (QApplication::translate("Service", "ERROR: Oven Lid/Cover Close Failed!", 0, QApplication::UnicodeUTF8));                       //!< Oven lid/cover close failed
    MSG_OVEN_INVALID_COMMAND = (QApplication::translate("Service", "ERROR: Oven Invalid Command !", 0, QApplication::UnicodeUTF8));                              //!< Oven Invalid command

    /****************************************************************************/
    MSG_SLIDEID_INIT_SUCCESS = (QApplication::translate("Service", "Photo Sensor Test successful", 0, QApplication::UnicodeUTF8));                               //!< Photo sensor test successful
    MSG_SLIDEID_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Photo sensor test Failed!", 0, QApplication::UnicodeUTF8));                           //!< Photo sensor test failed
    MSG_SLIDEID_STARTCOUNT_FAILURE = (QApplication::translate("Service", "ERROR: Start Count Failed!", 0, QApplication::UnicodeUTF8));                           //!< Start count failed
    MSG_SLIDEID_STRING = (QApplication::translate("Service", "Total Number of Slides = ", 0, QApplication::UnicodeUTF8));                                        //!< Level Sensor calibration failure information messgae

    /****************************************************************************/
    MSG_XYZ_INIT_FAILURE = (QApplication::translate("Service", "ERROR: XYZ Module Init Failed!", 0, QApplication::UnicodeUTF8));                                 //!< XYZ init module failed
    MSG_XYZ_RACKRFID_SUCCESS = (QApplication::translate("Service", "Rack RFID Value = ", 0, QApplication::UnicodeUTF8));                                         //!< Rack RFID
    MSG_XYZ_MOVETOSTN_FAILURE = (QApplication::translate("Service", "ERROR: Move to Station Failed !", 0, QApplication::UnicodeUTF8));                           //!< Move to station failed
    MSG_XYZ_CARRYTOSTN_FAILURE = (QApplication::translate("Service", "ERROR: Carry Rack to Station Failed !", 0, QApplication::UnicodeUTF8));                    //!< Carry Rack to station failed
    MSG_XYZ_LOWERRACK_FAILURE = (QApplication::translate("Service", "ERROR: Lower Rack Failed !", 0, QApplication::UnicodeUTF8));                                //!< Lower rack failed
    MSG_XYZ_ATTACHRACK_FAILURE = (QApplication::translate("Service", "ERROR: Attach Rack Failed !", 0, QApplication::UnicodeUTF8));                              //!< Attach rack failed
    MSG_XYZ_LIFTRACK_FAILURE = (QApplication::translate("Service", "ERROR: Lift Rack Failed !", 0, QApplication::UnicodeUTF8));                                  //!< Lift rack failed
    MSG_XYZ_DETACHRACK_FAILURE = (QApplication::translate("Service", "ERROR: Detach Rack Failed !", 0, QApplication::UnicodeUTF8));                              //!< Detach rack failed
    MSG_XYZ_COUNTSLIDES_SUCCESS = (QApplication::translate("Service", "Slide Count Successful. Count = ", 0, QApplication::UnicodeUTF8));                        //!< Slide count Successful
    MSG_XYZ_COUNTSLIDES_FAILURE = (QApplication::translate("Service", "ERROR: Slide Count Failed !", 0, QApplication::UnicodeUTF8));                             //!< Slide count failed

    /****************************************************************************/
    MSG_EXHAUST_INIT_SUCCESS = (QApplication::translate("Service", "Exhaust Device Initialization Successful", 0, QApplication::UnicodeUTF8));                   //!< Exhaust device init successful
    MSG_EXHAUST_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Exhaust Device Initialization Failed!", 0, QApplication::UnicodeUTF8));               //!< Exhaust device init failed
    MSG_EXHAUST_FANON_SUCCESS = (QApplication::translate("Service", "Exhaust Device FAN ON Successful", 0, QApplication::UnicodeUTF8));                          //!< Exhaust device FAN ON successful
    MSG_EXHAUST_FANON_FAILURE = (QApplication::translate("Service", "ERROR: Exhaust Device FAN ON Failed!", 0, QApplication::UnicodeUTF8));                      //!< Exhaust device FAN ON failed
    MSG_EXHAUST_FANOFF_SUCCESS = (QApplication::translate("Service", "Exhaust Device FAN OFF Successful", 0, QApplication::UnicodeUTF8));                        //!< Exhaust device FAN OFF successful
    MSG_EXHAUST_FANOFF_FAILURE = (QApplication::translate("Service", "ERROR: Exhaust Device FAN OFF Failed!", 0, QApplication::UnicodeUTF8));                    //!< Exhaust device FAN OFF failed
    MSG_EXHAUST_AIRFLOWSTATUS_OK = (QApplication::translate("Service", "Air Flow Status : OK", 0, QApplication::UnicodeUTF8));                                   //!< Exhaust device air flow OK status
    MSG_EXHAUST_AIRFLOWSTATUS_NOK = (QApplication::translate("Service", "ERROR: Air Flow Status : NOK", 0, QApplication::UnicodeUTF8));                          //!< Exhaust device air flow NOK status
    MSG_EXHAUST_FANSTATUS_OK = (QApplication::translate("Service", "Fan Current Status : OK", 0, QApplication::UnicodeUTF8));                                    //!< Exhaust device Fan current OK status
    MSG_EXHAUST_FANSTATUS_NOK = (QApplication::translate("Service", "ERROR: Fan Current Status : NOK", 0, QApplication::UnicodeUTF8));                           //!< Exhaust device Fan current NOK status
    MSG_EXHAUST_INVALID_COMMAND = (QApplication::translate("Service", "ERROR: Exhaust Invalid Command!", 0, QApplication::UnicodeUTF8));                         //!< Invalid command

    /****************************************************************************/
    MSG_WATERSTN_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Water Device Initialization Failed!", 0, QApplication::UnicodeUTF8));                 //!< Water device init failed
    MSG_WATERSTN_OPEN_SUCCESS = (QApplication::translate("Service", "Water Valve Opened successfully", 0, QApplication::UnicodeUTF8));                            //!< Water valve open successful
    MSG_WATERSTN_OPEN_FAILURE = (QApplication::translate("Service", "ERROR: Water Valve open Failed!", 0, QApplication::UnicodeUTF8));                            //!< Water valve open falied
    MSG_WATERSTN_CLOSE_SUCCESS = (QApplication::translate("Service", "Water Valve Closed successfully", 0, QApplication::UnicodeUTF8));                           //!< Water valve close successful
    MSG_WATERSTN_CLOSE_FAILURE = (QApplication::translate("Service", "ERROR: Water Valve Close Failed!", 0, QApplication::UnicodeUTF8));                          //!< Water valve close failed
    MSG_WATERSTN_BLOCKDETECTED = (QApplication::translate("Service", "Block Detected", 0, QApplication::UnicodeUTF8));                                            //!< Water valve close successful
    MSG_WATERSTN_BLOCKREMOVED = (QApplication::translate("Service", "Block Removed", 0, QApplication::UnicodeUTF8));                                              //!< Water valve close failed
    MSG_WATERSTN_INVALID_COMMAND = (QApplication::translate("Service", "ERROR: Water Valve Invalid Command!", 0, QApplication::UnicodeUTF8));                     //!< Invalid command

    /****************************************************************************/
    MSG_LIGHTDEVICE_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Light Device Initialization Failed!", 0, QApplication::UnicodeUTF8));             //!< Light device init failed
    MSG_LIGHTDEVICE_ON_SUCCESS = (QApplication::translate("Service", "Light Device Switch ON Successful", 0, QApplication::UnicodeUTF8));                        //!< Light device switch on successful
    MSG_LIGHTDEVICE_OFF_SUCCESS = (QApplication::translate("Service", "Light Device Switch OFF Successful", 0, QApplication::UnicodeUTF8));                      //!< Light device switch off successful
    MSG_LIGHTDEVICE_ON_FAILURE = (QApplication::translate("Service", "ERROR: Light Device Switch ON Failed!", 0, QApplication::UnicodeUTF8));                    //!< Light device switch on failed
    MSG_LIGHTDEVICE_OFF_FAILURE = (QApplication::translate("Service", "ERROR: Light Device Switch OFF Failed!", 0, QApplication::UnicodeUTF8));                  //!< Light device switch off failed
    MSG_LIGHTDEVICE_INVALID_COMMAND = (QApplication::translate("Service", "ERROR: Light Device Invalid Command!", 0, QApplication::UnicodeUTF8));                //!< Invalid command

    /****************************************************************************/
    MSG_HOODDEVICE_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Hood Device Initialization Failed!", 0, QApplication::UnicodeUTF8));               //!< Coverhood init failed
    MSG_HOODDEVICE_OPEN = (QApplication::translate("Service", "Cover/Hood is Open", 0, QApplication::UnicodeUTF8));                                              //!< Cover/Hood is open
    MSG_HOODDEVICE_CLOSE = (QApplication::translate("Service", "Cover/Hood is Closed", 0, QApplication::UnicodeUTF8));                                           //!< Cover/Hood is closed

    /****************************************************************************/
    MSG_XYZ1_CANID = (QApplication::translate("Service", "X Arm Left CAN ID : ", 0, QApplication::UnicodeUTF8));                                                 //!< X Arm left CANID
    MSG_XYZ2_CANID = (QApplication::translate("Service", "X Arm Right CAN ID : ", 0, QApplication::UnicodeUTF8));                                                //!< X Arm right CANID
    MSG_LOADER_CANID = (QApplication::translate("Service", "Loader CAN ID : ", 0, QApplication::UnicodeUTF8));                                                   //!< Loader CANID
    MSG_UNLOADER_CANID = (QApplication::translate("Service", "UnLoader CAN ID : ", 0, QApplication::UnicodeUTF8));                                               //!< Unloader CANID
    MSG_OVEN_CANID = (QApplication::translate("Service", "Oven CAN ID : ", 0, QApplication::UnicodeUTF8));                                                       //!< Oven CANID
    MSG_HCUVETTES_CANID = (QApplication::translate("Service", "Heated Cuvettes CAN ID : ", 0, QApplication::UnicodeUTF8));                                       //!< Heated cuvettes CANID
    MSG_AGITATION_CANID = (QApplication::translate("Service", "Agitation CAN ID : ", 0, QApplication::UnicodeUTF8));                                             //!< Agitation CANID
    MSG_RACKTRANSFER_CANID = (QApplication::translate("Service", "Rack Transfer CAN ID : ", 0, QApplication::UnicodeUTF8));                                      //!< Rack transfer CANID
    MSG_EXHAUST_CANID = (QApplication::translate("Service", "Exhaust Device CAN ID : ", 0, QApplication::UnicodeUTF8));                                          //!< Exhaust device CANID
    MSG_WATER_CANID = (QApplication::translate("Service", "Water Device CAN ID : ", 0, QApplication::UnicodeUTF8));                                              //!< Water device CANID
    MSG_HOOD_CANID = (QApplication::translate("Service", "Cover Hood CAN ID : ", 0, QApplication::UnicodeUTF8));                                                 //!< Cover/Hood CANID
    MSG_LIGHT_CANID = (QApplication::translate("Service", "Light Device CAN ID : ", 0, QApplication::UnicodeUTF8));                                              //!< Light device CANID
    MSG_SLIDEID_CANID = (QApplication::translate("Service", "Slide ID CAN ID : ", 0, QApplication::UnicodeUTF8));                                                //!< Slide ID CANID
    MSG_CONSUMABLES_CANID = (QApplication::translate("Service", "Consumables CAN ID : ", 0, QApplication::UnicodeUTF8));                                         //!< Consumables CANID
    MSG_INVALID_CANID_COMMAND = (QApplication::translate("Service", "ERROR: Unknown CANID Command!", 0, QApplication::UnicodeUTF8));                             //!< Unknown CANID command

    /****************************************************************************/
    CANID_XYZ1 = (QApplication::translate("Service", "0x0002 0x0001", 0, QApplication::UnicodeUTF8));                                                            //!< CANID XYZ1
    CANID_XYZ2 = (QApplication::translate("Service", "0x0102 0x0101", 0, QApplication::UnicodeUTF8));                                                            //!< CANID XYZ2
    CANID_LOADER = (QApplication::translate("Service", "0x0004", 0, QApplication::UnicodeUTF8));                                                                 //!< Loader CANID
    CANID_UNLOADER = (QApplication::translate("Service", "0x0104", 0, QApplication::UnicodeUTF8));                                                               //!< Unloader CANID
    CANID_OVEN = (QApplication::translate("Service", "0x0003", 0, QApplication::UnicodeUTF8));                                                                   //!< Oven CANID
    CANID_HCUVETTES = (QApplication::translate("Service", "0x0005", 0, QApplication::UnicodeUTF8));                                                              //!< Heated Cuvettes CANID
    CANID_RACKTRANSFER = (QApplication::translate("Service", "0x0202", 0, QApplication::UnicodeUTF8));                                                           //!< Rack transfer CANID
    CANID_SLIDEID = (QApplication::translate("Service", "0x0000", 0, QApplication::UnicodeUTF8));                                                                //!< SlideID CANID
    CANID_AGITATION = (QApplication::translate("Service", "0x0000", 0, QApplication::UnicodeUTF8));                                                              //!< Agitation CANID
    CANID_EXHAUST = (QApplication::translate("Service", "0x0000", 0, QApplication::UnicodeUTF8));                                                                //!< Exhaust CANID
    CANID_WATER = (QApplication::translate("Service", "0x0000", 0, QApplication::UnicodeUTF8));                                                                  //!< Water station CANID
    CANID_HOOD = (QApplication::translate("Service", "0x0000", 0, QApplication::UnicodeUTF8));                                                                   //!< Hood CANID
    CANID_LIGHT = (QApplication::translate("Service", "0x0000", 0, QApplication::UnicodeUTF8));                                                                  //!< Light CANID
    CANID_CONSUMABLES = (QApplication::translate("Service", "0x000D", 0, QApplication::UnicodeUTF8));                                                            //!< Consumables CANID

    /****************************************************************************/
    MSG_XYZ1_FIRMWAREINFO = (QApplication::translate("Service", "YZ Firmware Info : ", 0, QApplication::UnicodeUTF8));                                           //!< YZ Firmware Information
    MSG_XYZ2_FIRMWAREINFO = (QApplication::translate("Service", "YZ Firmware Info : ", 0, QApplication::UnicodeUTF8));                                           //!< YZ Firmware Information
    MSG_LOADER_FIRMWAREINFO = (QApplication::translate("Service", "Loader Firmware Info : ", 0, QApplication::UnicodeUTF8));                                     //!< Loader Firmware Information
    MSG_UNLOADER_FIRMWAREINFO = (QApplication::translate("Service", "UnLoader Firmware Info : ", 0, QApplication::UnicodeUTF8));                                 //!< Unloader Firmware Information
    MSG_OVEN_FIRMWAREINFO = (QApplication::translate("Service", "Oven Firmware Info : ", 0, QApplication::UnicodeUTF8));                                         //!< Oven Firmware Information
    MSG_HCUVETTES_FIRMWAREINFO = (QApplication::translate("Service", "Heated Cuvettes Firmware Info : ", 0, QApplication::UnicodeUTF8));                         //!< Heated Cuvettes Firmware Information
    MSG_AGITATION_FIRMWAREINFO = (QApplication::translate("Service", "Agitation Firmware Info : ", 0, QApplication::UnicodeUTF8));                               //!< Agitation Firmware Information
    MSG_RACKTRANSFER_FIRMWAREINFO = (QApplication::translate("Service", "Rack Transfer Firmware Info : ", 0, QApplication::UnicodeUTF8));                        //!< Rack transfer Firmware Information
    MSG_EXHAUST_FIRMWAREINFO = (QApplication::translate("Service", "Exhaust Firmware Info : ", 0, QApplication::UnicodeUTF8));                                   //!< Exhaust Firmware Information
    MSG_WATER_FIRMWAREINFO = (QApplication::translate("Service", "Water Firmware Info : ", 0, QApplication::UnicodeUTF8));                                       //!< water station Firmware Information
    MSG_HOOD_FIRMWAREINFO = (QApplication::translate("Service", "Hood Firmware Info : ", 0, QApplication::UnicodeUTF8));                                         //!< Cover/Hood Firmware Information
    MSG_LIGHT_FIRMWAREINFO = (QApplication::translate("Service", "Light Firmware Info : ", 0, QApplication::UnicodeUTF8));                                       //!< Light device Firmware Information
    MSG_SLIDEID_FIRMWAREINFO = (QApplication::translate("Service", "Slide ID Firmware Info : ", 0, QApplication::UnicodeUTF8));                                  //!< Slide ID Firmware Information
    MSG_CONSUMABLES_FIRMWAREINFO = (QApplication::translate("Service", "Consumables Firmware Info : ", 0, QApplication::UnicodeUTF8));                           //!< Consumables Firmware Information
    MSG_INVALID_FIRMWAREINFO_COMMAND = (QApplication::translate("Service", "ERROR: Unknown FirmwareInfo Command!", 0, QApplication::UnicodeUTF8));               //!< Unknown Firmware Info command

    /****************************************************************************/
    MSG_DEVICEDATACONTAINERS_MISSING = (QApplication::translate("Service", "ERROR: Missing data container XML files !", 0, QApplication::UnicodeUTF8));  //!< Device data containers missing
    MSG_DEVICECOMMAND_TIMEOUT = (QApplication::translate("Service", "ERROR: Device is not responding restart the system!", 0, QApplication::UnicodeUTF8));                     //!< Device command timeout

    /****************************************************************************/
    MSG_ENDTEST_RESULT = (QApplication::translate("Service", "End Test Result", 0, QApplication::UnicodeUTF8));                                                                   //!< End test result message
    MSG_HEATINGTEST_RESULT = (QApplication::translate("Service", "Heating Test Result", 0, QApplication::UnicodeUTF8));                                                           //!< Heating test result message
    MSG_RFIDCONSUMABLES_RESULT = (QApplication::translate("Service", "RFID Consumables Result", 0, QApplication::UnicodeUTF8));                                                   //!< RFID Consumables information message
    MSG_ENDTEST_COMPLETED = (QApplication::translate("Service", "End Test Completed", 0, QApplication::UnicodeUTF8));                                                             //!< End test completed message

    /****************************************************************************/
    MSG_DEVICE_CALIBRATION_SUCCESS = (QApplication::translate("Service", "Device Calibration successful", 0, QApplication::UnicodeUTF8));                                      //!< Device calibration success
    MSG_DEVICE_CALIBRATION_FAILURE = (QApplication::translate("Service", "ERROR: Device Calibration Failed!!!", 0, QApplication::UnicodeUTF8));                                //!< Device calibration failure

    /****************************************************************************/
    MSG_LSENSOR_CALIBRATION_SUCCESS = (QApplication::translate("Service", "Level Sensor Calibration successful", 0, QApplication::UnicodeUTF8));                                  //!< Level Sensor calibration success information message
    MSG_LSENSOR_CALIBRATION_FAILURE = (QApplication::translate("Service", "ERROR: Level Sensor Calibration Failed!!!", 0, QApplication::UnicodeUTF8));                            //!< Level Sensor calibration failure information messgae

    /****************************************************************************/
    MSG_EXHAUST_FAN1_CURRENTOK = (QApplication::translate("Service", "Fan 1 Current OK", 0, QApplication::UnicodeUTF8));                                          //!< Fan 1 current OK message
    MSG_EXHAUST_FAN1_CURRENTNOK = (QApplication::translate("Service", "ERROR: Fan 1 Current Not OK", 0, QApplication::UnicodeUTF8));                              //!< Fan 2 current OK message
    MSG_EXHAUST_FAN2_CURRENTOK = (QApplication::translate("Service", "Fan 2 Current OK", 0, QApplication::UnicodeUTF8));                                          //!< Fan 1 current OK message
    MSG_EXHAUST_FAN2_CURRENTNOK = (QApplication::translate("Service", "ERROR: Fan 2 Current Not OK", 0, QApplication::UnicodeUTF8));                              //!< Fan 1 current OK message

    /****************************************************************************/
    MSG_RFIDCONSUMABLES_INIT_FAILURE = (QApplication::translate("Service", "ERROR: Unable to Initialize RFID Consumables Module!!!", 0, QApplication::UnicodeUTF8)); //!< RFID consumables initialization failed
    MSG_CALIBRATION_FAILURE = (QApplication::translate("Service", "ERROR: Requested Calibration Failed", 0, QApplication::UnicodeUTF8));                             //!< Calibration failure
    MSG_CALIBRATION_SUCCESS = (QApplication::translate("Service", "Calibration successful", 0, QApplication::UnicodeUTF8));                                          //!< Calibration success
    MSG_CALIBRATION_UNKNOWNCOMMAND = (QApplication::translate("Service", "ERROR: Unknown Calibration Command", 0, QApplication::UnicodeUTF8));                       //!< Unknown Calibration Command
    MSG_MANUFACTURINGTEST_RESULT = (QApplication::translate("Service", "Manufacturing Test Result", 0, QApplication::UnicodeUTF8));                                  //!< Manufacturing test result
    MSG_HEATINGTEST_FAILURE = (QApplication::translate("Service", "ERROR: Heating Tests Failed!!!", 0, QApplication::UnicodeUTF8));                                  //!< Heating test failed message

    /****************************************************************************/
    MSG_ASB1_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB1_0_Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB1_0_Firmware update failed
    MSG_ASB1_1_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB1_1 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB1_1_Firmware update failed
    MSG_ASB2_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB2_0 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB2_0_Firmware update failed
    MSG_ASB2_1_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB2_1 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB2_1_Firmware update failed
    MSG_ASB2_2_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB2_2 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB2_2_Firmware update failed
    MSG_ASB3_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB3_0 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB3_0_Firmware update failed
    MSG_ASB4_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB4_0 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB4_0_Firmware update failed
    MSG_ASB4_1_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB4_1 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB4_1_Firmware update failed
    MSG_ASB5_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB5_0 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB5_0_Firmware update failed
    MSG_ASB6_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB6_0 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB6_0_Firmware update failed
    MSG_ASB7_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB7_0 Firmware update failed!", 0, QApplication::UnicodeUTF8));         //!< ASB7_0_Firmware update failed
    MSG_ASB12_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB12_0 Firmware update failed!", 0, QApplication::UnicodeUTF8));       //!< ASB12_0_Firmware update failed
    MSG_ASB13_0_FWUPDATE_FAILURE = (QApplication::translate("Service", "ERROR:ASB13_0 Firmware update failed!", 0, QApplication::UnicodeUTF8));       //!< ASB13_0_Firmware update failed

    /****************************************************************************/
    MSG_CRITICAL_DEVICE_FAILURE = (QApplication::translate("Service", "ERROR:Critical Device Error. Please Restart system to continue.", 0, QApplication::UnicodeUTF8));       //!< Critical device error
    MSG_DEVICE_NOTYET_READY = (QApplication::translate("Service", "Error: Device Layer not yet Initialized. Please retry after some time...", 0, QApplication::UnicodeUTF8));  //!< Devices not ready yet
    MSG_DEVICE_SHUTDOWN = (QApplication::translate("Service", "Power Failure received... Application is shutting down...", 0, QApplication::UnicodeUTF8));                     //!< Devices not ready yet
    MSG_SERIALNUMBER_ERROR = (QApplication::translate("Service", "ERROR: Set Module SerialNumber Failure.", 0, QApplication::UnicodeUTF8));                                    //!< Set module serial number error message
    MSG_DEVICE_RELEASE_PRESSURE_FAILED = (QApplication::translate("Service", "ERROR: Release pressure failure.", 0, QApplication::UnicodeUTF8));                       //!< release pressure failed.
    MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_FAILED = (QApplication::translate("Service", "ERROR: Pressure sensor calibration failure, Please confirm the air system is not blocked and open the retort lid do calibrate again!", 0, QApplication::UnicodeUTF8));            //!< Pressure sensor calibration failed.
    MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_SUCCESS = (QApplication::translate("Service", " Pressure sensor calibration is successful! Pressure drift value(Kpa):", 0, QApplication::UnicodeUTF8));           //!< Pressure sensor calibration successfull.

}

}   // end of namespace Service
