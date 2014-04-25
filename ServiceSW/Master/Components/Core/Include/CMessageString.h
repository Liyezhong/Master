/****************************************************************************/
/*! \file Core/Include/CMessageString.h
 *
 *  \brief CMessageString class definition.
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
#ifndef SERVICE_CMESSAGESTRING_H
#define SERVICE_CMESSAGESTRING_H

#include <QString>

namespace Service {

class CMessageString
{
public:
    static QString MSG_X_REFERENCERUN_SUCCESS;                  //!< X-Axis reference run success
    static QString MSG_X_REFERENCERUN_FAILURE;                  //!< X-Axis reference run fail message
    static QString MSG_YZ_MOVETOSAFEPOS_SUCCESS;                //!< YZ arm move to safe position successful
    static QString MSG_YZ_MOVETOSAFEPOS_FAILURE;                //!< YZ arm move to safe position fail message
    static QString MSG_YZ_REFERENCE_RUN_SUCCESS;                //!< YZ arm reference run successful
    static QString MSG_YZ_REFERENCE_RUN_FAILURE;                //!< YZ arm reference run fail message
    static QString MSG_YZ_RFIDREAD_FAILURE;                     //!< YZ arm RFID read failed
    static QString MSG_TRANSFERSTN_REFERENCE_RUN_SUCCESS;       //!< Transfer station reference run successful
    static QString MSG_TRANSFERSTN_REFERENCE_RUN_FAILURE;       //!< Transfer station reference run fail message
    static QString MSG_TRANSFERSTN_OPEN_SUCCESS;                //!< Transfer station reference run successful
    static QString MSG_TRANSFERSTN_OPEN_FAILURE;                //!< Transfer station reference run fail message
    static QString MSG_TRANSFERSTN_CLOSE_SUCCESS;               //!< Transfer station reference run successful
    static QString MSG_TRANSFERSTN_CLOSE_FAILURE;               //!< Transfer station reference run fail message


    static QString MSG_DRAWER_REFERENCE_RUN_SUCCESS;             //!< Drawer reference run successful
    static QString MSG_DRAWER_REFERENCE_RUN_FAILURE;             //!< Drawer reference run fail message
    static QString MSG_DRAWER_INIT_SUCCESS;                      //!< Drawer initialize successful message
    static QString MSG_DRAWER_OPEN_SUCCESS;                      //!< Drawer open successful message
    static QString MSG_DRAWER_CLOSE_SUCCESS;                     //!< Drawer closed successful message
    static QString MSG_DRAWER_RFID_SUCCESS;                      ///< Drawer RFID read successful message
    static QString MSG_DRAWER_INIT_FAILURE;                      //!< Drawer initialization failed message
    static QString MSG_DRAWER_OPEN_FAILURE;                      //!< Drawer open failed message
    static QString MSG_DRAWER_CLOSE_FAILURE;                     //!< Drawer close failed message
    static QString MSG_DRAWER_INVALID_COMMAND;                   //!< Drawer invalid command

    static QString MSG_AGITATION_INIT_SUCCESS;                   //!< Agitation initialization successful
    static QString MSG_AGITATION_INIT_FAILURE;                   //!< Agitation initialization failed
    static QString MSG_AGITATION_SETSPEED_SUCCESS;               //!< Agitation set speed successful
    static QString MSG_AGITATION_SETSPEED_FAILURE;               //!< Agitation set speed failed
    static QString MSG_AGITATION_START_SUCCESS;                  //!< Agitation start rotation successful
    static QString MSG_AGITATION_START_FAILURE;                  ///< Agitation start rotation failed

    static QString MSG_OVENLID_REFERENCE_RUN_SUCCESS;            //!< Oven lid reference run successful
    static QString MSG_OVENLID_REFERENCE_RUN_FAILURE;            //!< Oven lid reference run failed
    static QString MSG_OVEN_INIT_SUCCESS;                        //!< Oven initialization successful
    static QString MSG_OVEN_INIT_FAILURE;                        //!< Oven initialization failed
    static QString MSG_OVEN_LIDOPEN_SUCCESS;                     //!< Oven lid/cover open successful
    static QString MSG_OVEN_LIDOPEN_FAILURE;                     //!< Oven lid/cover open failed
    static QString MSG_OVEN_LIDCLOSE_SUCCESS;                    //!< Oven lid/cover close successful
    static QString MSG_OVEN_LIDCLOSE_FAILURE;                    //!< Oven lid/cover close failed
    static QString MSG_OVEN_INVALID_COMMAND;                     //!< Oven Invalid command

    static QString MSG_SLIDEID_INIT_SUCCESS;                     //!< Photo sensor test successful
    static QString MSG_SLIDEID_INIT_FAILURE;                     //!< Photo sensor test failed
    static QString MSG_SLIDEID_STARTCOUNT_FAILURE;               //!< Start count failed
    static QString MSG_SLIDEID_STRING;                           //!< Level Sensor calibration failure information messgae

    static QString MSG_XYZ_INIT_FAILURE;                          //!< XYZ init module failed
    static QString MSG_XYZ_RACKRFID_SUCCESS;                      //!< Rack RFID
    static QString MSG_XYZ_MOVETOSTN_FAILURE;                     //!< Move to station failed
    static QString MSG_XYZ_CARRYTOSTN_FAILURE;                    //!< Carry Rack to station failed
    static QString MSG_XYZ_LOWERRACK_FAILURE;                     //!< Lower rack failed
    static QString MSG_XYZ_ATTACHRACK_FAILURE;                    //!< Attach rack failed
    static QString MSG_XYZ_LIFTRACK_FAILURE;                      //!< Lift rack failed
    static QString MSG_XYZ_DETACHRACK_FAILURE;                    //!< Detach rack failed
    static QString MSG_XYZ_COUNTSLIDES_SUCCESS;                   //!< Slide count Successful
    static QString MSG_XYZ_COUNTSLIDES_FAILURE;                   //!< Slide count failed

    static QString MSG_EXHAUST_INIT_SUCCESS;                      //!< Exhaust device init successful
    static QString MSG_EXHAUST_INIT_FAILURE;                      //!< Exhaust device init failed
    static QString MSG_EXHAUST_FANON_SUCCESS;                     //!< Exhaust device FAN ON successful
    static QString MSG_EXHAUST_FANON_FAILURE;                     //!< Exhaust device FAN ON failed
    static QString MSG_EXHAUST_FANOFF_SUCCESS;                    //!< Exhaust device FAN OFF successful
    static QString MSG_EXHAUST_FANOFF_FAILURE;                    //!< Exhaust device FAN OFF failed
    static QString MSG_EXHAUST_AIRFLOWSTATUS_OK;                  //!< Exhaust device air flow OK status
    static QString MSG_EXHAUST_AIRFLOWSTATUS_NOK;                 //!< Exhaust device air flow NOK status
    static QString MSG_EXHAUST_FANSTATUS_OK;                      //!< Exhaust device Fan current OK status
    static QString MSG_EXHAUST_FANSTATUS_NOK;                     //!< Exhaust device Fan current NOK status
    static QString MSG_EXHAUST_INVALID_COMMAND;                   //!< Invalid command

    static QString MSG_WATERSTN_INIT_FAILURE;                     //!< Water device init failed
    static QString MSG_WATERSTN_OPEN_SUCCESS;                     //!< Water valve open successful
    static QString MSG_WATERSTN_OPEN_FAILURE;                     //!< Water valve open falied
    static QString MSG_WATERSTN_CLOSE_SUCCESS;                    //!< Water valve close successful
    static QString MSG_WATERSTN_CLOSE_FAILURE;                    //!< Water valve close failed
    static QString MSG_WATERSTN_BLOCKDETECTED;                    //!< Water valve close successful
    static QString MSG_WATERSTN_BLOCKREMOVED;                     //!< Water valve close failed
    static QString MSG_WATERSTN_INVALID_COMMAND;                  //!< Invalid command

    static QString MSG_LIGHTDEVICE_INIT_FAILURE;                  //!< Light device init failed
    static QString MSG_LIGHTDEVICE_ON_SUCCESS;                    //!< Light device switch on successful
    static QString MSG_LIGHTDEVICE_OFF_SUCCESS;                   //!< Light device switch off successful
    static QString MSG_LIGHTDEVICE_ON_FAILURE;                    //!< Light device switch on failed
    static QString MSG_LIGHTDEVICE_OFF_FAILURE;                   //!< Light device switch off failed
    static QString MSG_LIGHTDEVICE_INVALID_COMMAND;               //!< Invalid command

    static QString MSG_HOODDEVICE_INIT_FAILURE;                   //!< Coverhood init failed
    static QString MSG_HOODDEVICE_OPEN;                           //!< Cover/Hood is open
    static QString MSG_HOODDEVICE_CLOSE;                          //!< Cover/Hood is closed

    static QString MSG_XYZ1_CANID;                                //!< X Arm left CANID
    static QString MSG_XYZ2_CANID;                                //!< X Arm right CANID
    static QString MSG_LOADER_CANID;                              //!< Loader CANID
    static QString MSG_UNLOADER_CANID;                            //!< Unloader CANID
    static QString MSG_OVEN_CANID;                                //!< Oven CANID
    static QString MSG_HCUVETTES_CANID;                           //!< Heated cuvettes CANID
    static QString MSG_AGITATION_CANID;                           //!< Agitation CANID
    static QString MSG_RACKTRANSFER_CANID;                        //!< Rack transfer CANID
    static QString MSG_EXHAUST_CANID;                             //!< Exhaust device CANID
    static QString MSG_WATER_CANID;                               //!< Water device CANID
    static QString MSG_HOOD_CANID;                                //!< Cover/Hood CANID
    static QString MSG_LIGHT_CANID;                               //!< Light device CANID
    static QString MSG_SLIDEID_CANID;                             //!< Slide ID CANID
    static QString MSG_CONSUMABLES_CANID;                         //!< Consumables CANID
    static QString MSG_INVALID_CANID_COMMAND;                     //!< Unknown CANID command

    static QString CANID_XYZ1;                                    //!< CANID XYZ1
    static QString CANID_XYZ2;                                    //!< CANID XYZ2
    static QString CANID_LOADER;                                  //!< Loader CANID
    static QString CANID_UNLOADER;                                //!< Unloader CANID
    static QString CANID_OVEN;                                    //!< Oven CANID
    static QString CANID_HCUVETTES;                               //!< Heated Cuvettes CANID
    static QString CANID_RACKTRANSFER;                            //!< Rack transfer CANID
    static QString CANID_SLIDEID;                                 //!< SlideID CANID
    static QString CANID_AGITATION;                               //!< Agitation CANID
    static QString CANID_EXHAUST;                                 //!< Exhaust CANID
    static QString CANID_WATER;                                   //!< Water station CANID
    static QString CANID_HOOD;                                    //!< Hood CANID
    static QString CANID_LIGHT;                                   //!< Light CANID
    static QString CANID_CONSUMABLES;                             //!< Consumables CANID

    static QString MSG_XYZ1_FIRMWAREINFO;                         //!< YZ Firmware Information
    static QString MSG_XYZ2_FIRMWAREINFO;                         //!< YZ Firmware Information
    static QString MSG_LOADER_FIRMWAREINFO;                       //!< Loader Firmware Information
    static QString MSG_UNLOADER_FIRMWAREINFO;                     //!< Unloader Firmware Information
    static QString MSG_OVEN_FIRMWAREINFO;                         //!< Oven Firmware Information
    static QString MSG_HCUVETTES_FIRMWAREINFO;                    //!< Heated Cuvettes Firmware Information
    static QString MSG_AGITATION_FIRMWAREINFO;                    //!< Agitation Firmware Information
    static QString MSG_RACKTRANSFER_FIRMWAREINFO;                 //!< Rack transfer Firmware Information
    static QString MSG_EXHAUST_FIRMWAREINFO;                      //!< Exhaust Firmware Information
    static QString MSG_WATER_FIRMWAREINFO;                        //!< water station Firmware Information
    static QString MSG_HOOD_FIRMWAREINFO;                         //!< Cover/Hood Firmware Information
    static QString MSG_LIGHT_FIRMWAREINFO;                        //!< Light device Firmware Information
    static QString MSG_SLIDEID_FIRMWAREINFO;                      //!< Slide ID Firmware Information
    static QString MSG_CONSUMABLES_FIRMWAREINFO;                  //!< Consumables Firmware Information
    static QString MSG_INVALID_FIRMWAREINFO_COMMAND;              //!< Unknown Firmware Info command

    static QString MSG_DEVICEDATACONTAINERS_MISSING;              //!< Device data containers missing
    static QString MSG_DEVICECOMMAND_TIMEOUT;                     //!< Device command timeout

    static QString MSG_ENDTEST_RESULT;                            //!< End test result message
    static QString MSG_HEATINGTEST_RESULT;                        //!< Heating test result message
    static QString MSG_RFIDCONSUMABLES_RESULT;                    //!< RFID Consumables information message
    static QString MSG_ENDTEST_COMPLETED;                         //!< End test completed message

    static QString MSG_DEVICE_CALIBRATION_SUCCESS;                //!< Device calibration success
    static QString MSG_DEVICE_CALIBRATION_FAILURE;                //!< Device calibration failure

    static QString MSG_LSENSOR_CALIBRATION_SUCCESS;               //!< Level Sensor calibration success information message
    static QString MSG_LSENSOR_CALIBRATION_FAILURE;               //!< Level Sensor calibration failure information messgae

    static QString MSG_EXHAUST_FAN1_CURRENTOK;                    //!< Fan 1 current OK message
    static QString MSG_EXHAUST_FAN1_CURRENTNOK;                   //!< Fan 2 current OK message
    static QString MSG_EXHAUST_FAN2_CURRENTOK;                    //!< Fan 1 current OK message
    static QString MSG_EXHAUST_FAN2_CURRENTNOK;                   //!< Fan 2 current OK message

    static QString MSG_RFIDCONSUMABLES_INIT_FAILURE;              //!< RFID consumables initialization failed
    static QString MSG_CALIBRATION_FAILURE;                       //!< Calibration failure
    static QString MSG_CALIBRATION_SUCCESS;                       //!< Calibration success
    static QString MSG_CALIBRATION_UNKNOWNCOMMAND;                //!< Unknown Calibration Command
    static QString MSG_MANUFACTURINGTEST_RESULT;                  //!< Manufacturing test result
    static QString MSG_HEATINGTEST_FAILURE;                       //!< Heating test failed message

    static QString MSG_ASB1_0_FWUPDATE_FAILURE;                   //!< ASB1_0_Firmware update failed
    static QString MSG_ASB1_1_FWUPDATE_FAILURE;                   //!< ASB1_1_Firmware update failed
    static QString MSG_ASB2_0_FWUPDATE_FAILURE;                   //!< ASB2_0_Firmware update failed
    static QString MSG_ASB2_1_FWUPDATE_FAILURE;                   //!< ASB2_1_Firmware update failed
    static QString MSG_ASB2_2_FWUPDATE_FAILURE;                   //!< ASB2_2_Firmware update failed
    static QString MSG_ASB3_0_FWUPDATE_FAILURE;                   //!< ASB3_0_Firmware update failed
    static QString MSG_ASB4_0_FWUPDATE_FAILURE;                   //!< ASB4_0_Firmware update failed
    static QString MSG_ASB4_1_FWUPDATE_FAILURE;                   //!< ASB4_1_Firmware update failed
    static QString MSG_ASB5_0_FWUPDATE_FAILURE;                   //!< ASB5_0_Firmware update failed
    static QString MSG_ASB6_0_FWUPDATE_FAILURE;                   //!< ASB6_0_Firmware update failed
    static QString MSG_ASB7_0_FWUPDATE_FAILURE;                   //!< ASB7_0_Firmware update failed
    static QString MSG_ASB12_0_FWUPDATE_FAILURE;                  //!< ASB12_0_Firmware update failed
    static QString MSG_ASB13_0_FWUPDATE_FAILURE;                  //!< ASB13_0_Firmware update failed

    static QString MSG_CRITICAL_DEVICE_FAILURE;                   //!< Critical device error
    static QString MSG_DEVICE_NOTYET_READY;                       //!< Devices not ready yet
    static QString MSG_DEVICE_SHUTDOWN;                           //!< Devices not ready yet
    static QString MSG_SERIALNUMBER_ERROR;                        //!< Set module serial number error message

    static void RetranslateUI();
};

}   // end of namespace Service

#endif // SERVICE_CMESSAGESTRING_H
