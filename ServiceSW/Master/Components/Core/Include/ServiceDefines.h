/****************************************************************************/
/*! \file Include/ServiceDefines.h
 *
 *  \brief Some Service Software definitions.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2010-07-12
 *  $Author:    $ Srivathsa HH
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

#ifndef GLOBAL_SERVICEDEFINES_H
#define GLOBAL_SERVICEDEFINES_H

#include "../Include/PlatformServiceDefines.h"
#include <QtGlobal>
#include <QString>
#include <QHash>

/****************************************************************************/
/**
 * \brief In this namespace live globally used classes and defines.
 */
/****************************************************************************/
namespace Service {

typedef quint16 gSourceType;            ///< typedef for HeartBeat source: source type.
typedef quint16 gSubComponentType;      ///< typedef for logging source: subcomponent type.
typedef quint64 tRefType;               ///< typedef for tRefType.

const QString NO_NUMERIC_DATA = "";     ///< Self explaining

/****************************************************************************/
/**
 * \brief Enum containing Service or Manufacturing Mode.
 */
/****************************************************************************/
enum SoftwareModeType {
    SERVICE_MODE,                ///< Service Mode.
    MANUFACTURING_MODE           ///< Manufacturing Mode.
};

/****************************************************************************/
/**
 * \brief Enum containing all known GUI Return Message Types.
 */
/****************************************************************************/
enum GUIReturnMessageType {
    GUIMSGTYPE_UNDEFINED,       ///< Undefined. Used for initialisation.
    GUIMSGTYPE_ERROR,           ///< Error.
    GUIMSGTYPE_WARNING,         ///< Warning.
    GUIMSGTYPE_INFO,            ///< Info.
    GUIMSGTYPE_CANID,           ///< CAN ID Message.
    GUIMSGTYPE_FIRMWAREINFO,    ///< Firmware Info Message.
    GUIMSGTYPE_RACK_RFID,       ///< Rack RFID Message.
    GUIMSGTYPE_DRAWER_RFID,     ///< Drawer RFID Message.
    GUIMSGTYPE_FANCURRENT,      ///< FAN Current/Status Message.
    GUIMSGTYPE_AIRFLOWSENSOR,   ///< Airflow Sensor Message.
    GUIMSGTYPE_HOODSTATUS,      ///< Hood Status Message.
    GUIMSGTYPE_KITINFO,         ///< KIT Info Message.
    GUIMSGTYPE_BOTTLEINFO,      ///< Bottle Info Message.
    GUIMSGTYPE_INITCALIBRATION,  ///< Init for Calibration Message.
    GUIMSGTYPE_HEATINGSTATUS,  ///< Refresh Heating Status Message.
    GUIMSGTYPE_MANUFMSGMODE     ///< Manufacturing report Message.
};

/****************************************************************************/
/**
 * \brief Enum containing Exhaust device command Types.
 */
/****************************************************************************/
enum ExhaustCommandType {
    EXHAUST_FANON,              ///< FAN ON.
    EXHAUST_FANFF,              ///< FAN OFF.
    EXHAUST_AIRFLOWSTATUS,      ///< AIRFLOWSTATUS
    EXHAUST_FANCURRENT          ///< FAN STATUS
};

/****************************************************************************/
/**
 * \brief Enum containing Device Light command Types.
 */
/****************************************************************************/
enum DeviceLightCommandType {
    DEVICELIGHT_ON,              ///< DeviceLight ON.
    DEVICELIGHT_OFF              ///< DeviceLight OFF.
};

/****************************************************************************/
/**
 * \brief Enum containing Heating command Types.
 */
/****************************************************************************/
enum HeatingTestType {
    HEATING_LEVEL_SENSOR,             ///<
    HEATING_RETORT_EMMPTY,
    HEATING_RETORT_LIQUID,
    HEATING_OVEN_EMPTY,
    HEATING_ROTARY_VALVE,             ///<
    HEATING_TUBE1,
    HEATING_TUBE2
};

/****************************************************************************/
/**
 * \brief Enum containing Heater Types.
 */
/****************************************************************************/
enum HeaterType {
    HEATER_ROTARY_VALVE,           ///< Heater of rotary valve
    HEATER_RETORT,
    HEATER_RETORT_SIDE,
    HEATER_RETORT_BOTTOM,
    HEATER_OVEN,
    HEATER_OVEN_TOP,
    HEATER_OVEN_BOTTOM,
    HEATER_LEVEL_SENSOR,
    HEATER_TUBE1,
    HEATER_TUBE2
};

/****************************************************************************/
/**
 * \brief Enum containing Rotary Valve command Types.
 */
/****************************************************************************/
enum RVTestType {
    RV_MOVE_TO_INIT_POS,             ///<
    RV_MOVE_TO_TUBE_POS,
    RV_MOVE_TO_SEAL_POS
};

/****************************************************************************/
/**
 * \brief Enum containing Calibration Command Type.
 */
/****************************************************************************/
enum DeviceCalibrationCmdType {
    DEVICE_XYZ1_LEVELSENSOR,        ///< Level Sensor XYZ1
    DEVICE_XYZ2_LEVELSENSOR,        ///< Level Sensor XYZ2
    DEVICE_OVENLID,                 ///< Oven LID
    DEVICE_DRAWERLOADER,            ///< Loader Calibration
    DEVICE_DRAWERUNLOADER,          ///< UnLoader Calibration
    DEVICE_XYZ1_LEVELSENSOR_INIT,   ///< Level Sensor XYZ1
    DEVICE_XYZ2_LEVELSENSOR_INIT,   ///< Level Sensor XYZ2
    DEVICE_OVENLID_INIT,            ///< Oven LID
    DEVICE_DRAWERLOADER_INIT,       ///< Loader Calibration
    DEVICE_DRAWERUNLOADER_INIT,      ///< UnLoader Calibration
    DEVICE_PRESSURE_SENSOR          ///< pressure sensor Calibration
};

/****************************************************************************/
/**
 * \brief Enum containing module Test Names
 */
/****************************************************************************/
typedef enum ModuleTestCaseID {
    TEST_CASE_ID_UNUSED = 0,
    MAINCONTROL_ASB3,                          ///< ASB3 test
    MAINCONTROL_ASB5,                          ///< ASB5 test
    MAINCONTROL_ASB15,                         ///< ASB15 test
    RETORT_LID_LOCK,                    ///< Retort lid lock test
    RETORT_LEVEL_SENSOR_HEATING,        ///< Retort level sensor heating test
    RETORT_LEVEL_SENSOR_DETECTING,      ///< Retort level sensor detecting test
    RETORT_HEATING_WITH_WATER,          ///< Retort heating test with water
    RETORT_HEATING_EMPTY,               ///< Retort heating test (empty)
    DISPLAY_BASIC_COLOR,                ///< Display basic color test
    OVEN_COVER_SENSOR,                  ///< Oven cover sensor test
    OVEN_HEATING_EMPTY,                 ///< Oven heating test (empty)
    OVEN_HEATING_WITH_WATER,            ///< Oven heating test with water
    ROTARY_VALVE_INITIALIZING,          ///< Rotary valve initializing test
    ROTARY_VALVE_SELECTION_FUNCTION,    ///< Rotary valve selection function test
    ROTARY_VALVE_SEALING_FUNCTION,      ///< Rotary valve sealing function test
    ROTARY_VALVE_HEATING_END,           ///< Rotary valve heating property end test
    ROTARY_VALVE_HEATING_STATION,       ///< Rotary valve heating property station test
    LA_SYSTEM_PUMP_VALVE_CONTROL,       ///< LA System pump and valve control test
    LA_SYSTEM_HEATING_LIQUID_TUBE,      ///< LA System heating test for liquid tube
    LA_SYSTEM_HEATING_AIR_TUBE,         ///< LA System heating test for air tube
    SYSTEM_SEALING_TEST,                ///< System sealing test
    SYSTEM_PRESSURE_VACUUM_ON_GENERATION, ///< System pressure vacuum on generation test
    SYSTEM_FILL_DRAINING,                ///< System filling and draining test
    SYSTEM_OVERFLOW,                     ///< System overflow test
    SYSTEM_SPEARKER,                     ///< System speaker test
    SYSTEM_REMOTE_LOCAL_ALARM,           ///< System remote and local alarm test
    SYSTEM_USB,                          ///< System usb test
    SYSTEM_MAINS_RELAY,                  ///< System Mains relay test
    SYSTEM_EXHAUST_FAN,                  ///< System exhaust fan test
    SYSTEM_VENTILATION_FAN,              ///< System ventilation fan test
    SYSTEM_110V_220V_SWITCH,             ///< System 110V/220V switch test
    TEST_ABORT                           ///< This is to abort a heating test case.
} ModuleTestCaseID_t;

/****************************************************************************/
/**
 * \brief Enum containing module Names
 */
/****************************************************************************/
typedef enum ModuleNames {
    MAIN_CONTROL,                     ///< E Box
    RETORT,                 ///< Retort Module
    DISPLAY,              ///< Display Module
    OVEN,                ///< Oven Module
    ROTARY_VALVE,                   ///< Rotary Valve Module
    LA_SYSTEM,              ///< L&A System Module
    SYSTEM            ///< System
} ModuleNames_t;

typedef QMap<QString, QString>  ModuleTestStatus;

/****************************************************************************/
/**
 * \brief Parameters for service tests.
 */
/****************************************************************************/
const qint32 TEST_RV_HEATING_TEMP = 100;
const qint32 TEST_RV_HEATING_MIN_TEMP = 90;
const qint32 TEST_RV_HEATING_MAX_TEMP = 110;
const qint32 TEST_RV_HEATING_TIME = 100;

const qint32 TEST_LS_HEATING_TEMP = 100;
const qint32 TEST_LS_HEATING_MIN_TEMP = 90;
const qint32 TEST_LS_HEATING_MAX_TEMP = 110;
const qint32 TEST_LS_HEATING_TIME = 100;

const qint32 TEST_OVEN_HEATING_TEMP = 100;
const qint32 TEST_OVEN_HEATING_MIN_TEMP = 50;
const qint32 TEST_OVEN_HEATING_MAX_TEMP = 60;
const qint32 TEST_OVEN_HEATING_TIME_EMPTY = 1*60;
const qint32 TEST_OVEN_HEATING_TIME_WITH_WATER = 40*60;

const qint32 TEST_TUBE_HEATING_TEMP = 80;
const qint32 TEST_TUBE_HEATING_MIN_TEMP = 75;
const qint32 TEST_TUBE_HEATING_MAX_TEMP = 85;
const qint32 TEST_TUBE_HEATING_TIME = 1200;

const qint32 TEST_RV_TUBEPOS_PRESS = 30;
const qint32 TEST_RV_TUBEPOS_PRESS_DURATION = 60*1000;
const qint32 TEST_RV_SEALPOS_PRESS = 30;
const qint32 TEST_RV_SEALPOS_PRESS_DROP = 1;
const qint32 TEST_RV_SEALPOS_PRESS_DURATION = 60*1000;

/****************************************************************************/
const QString MSG_X_REFERENCERUN_SUCCESS("X Axis Reference Run Successful");
const QString MSG_X_REFERENCERUN_FAILURE("ERROR: X Axis reference run Failed!!!");
const QString MSG_YZ_MOVETOSAFEPOS_SUCCESS("YZ Arm Move to Safe position Successful");
const QString MSG_YZ_MOVETOSAFEPOS_FAILURE("ERROR: YZ Arm Move to Safe position Failed!!!");
const QString MSG_YZ_REFERENCE_RUN_SUCCESS("YZ Reference Run Successful");
const QString MSG_YZ_REFERENCE_RUN_FAILURE("ERROR: YZ reference run Failed!!!");
const QString MSG_TRANSFERSTN_REFERENCE_RUN_SUCCESS("Rack Transfer Reference Run Successful");
const QString MSG_TRANSFERSTN_REFERENCE_RUN_FAILURE("ERROR: Rack Transfer Reference Run Failed!!!");

/****************************************************************************/
const QString MSG_DRAWER_REFERENCE_RUN_SUCCESS("Drawer Reference Run Successful");
const QString MSG_DRAWER_REFERENCE_RUN_FAILURE("ERROR: Drawer Reference Run Failed!!!");
const QString MSG_DRAWER_INIT_SUCCESS("Drawer Initialized successfuly");
const QString MSG_DRAWER_OPEN_SUCCESS("Drawer Opened successfully");
const QString MSG_DRAWER_CLOSE_SUCCESS("Drawer Closed successfully");
const QString MSG_DRAWER_RFID_SUCCESS("Drawer RFID read successful");
const QString MSG_DRAWER_INIT_FAILURE("ERROR: Drawer Initialization Failed !!!");
const QString MSG_DRAWER_OPEN_FAILURE("ERROR: Drawer Open Failed !!!");
const QString MSG_DRAWER_CLOSE_FAILURE("ERROR: Drawer Close Failed !!!");
const QString MSG_DRAWER_INVALID_COMMAND("ERROR: Drawer Invalid Command !!!");

/****************************************************************************/
const QString MSG_AGITATION_INIT_SUCCESS("Agitation Initialization successful");
const QString MSG_AGITATION_INIT_FAILURE("ERROR: Agitation Initialization Failed!!!");
const QString MSG_AGITATION_SETSPEED_SUCCESS("Agitation SetSpeed successful");
const QString MSG_AGITATION_SETSPEED_FAILURE("ERROR: Set Speed Failed!!!");
const QString MSG_AGITATION_START_SUCCESS("Agitation Start rotation Successful");
const QString MSG_AGITATION_START_FAILURE("ERROR: Start rotation Failed!!!");

/****************************************************************************/
const QString MSG_OVENLID_REFERENCE_RUN_SUCCESS("Oven Lid Reference Run Successful");
const QString MSG_OVENLID_REFERENCE_RUN_FAILURE("ERROR: Oven Lid Reference Run Failed!!!");
const QString MSG_OVEN_INIT_SUCCESS("Oven Initialized successfully");
const QString MSG_OVEN_INIT_FAILURE("ERROR: Oven Initialization failed!!!");
const QString MSG_OVEN_LIDOPEN_SUCCESS("Oven Lid/Cover Opened successfully");
const QString MSG_OVEN_LIDOPEN_FAILURE("ERROR: Oven Lid/Cover Open Failed!!!");
const QString MSG_OVEN_LIDCLOSE_SUCCESS("Oven Lid/Cover Closed successfully");
const QString MSG_OVEN_LIDCLOSE_FAILURE("ERROR: Oven Lid/Cover Close Failed!!!");
const QString MSG_OVEN_INVALID_COMMAND("ERROR: Oven Invalid Command !!!");

/****************************************************************************/
const QString MSG_SLIDEID_INIT_SUCCESS("Photo Sensor Test successful");
const QString MSG_SLIDEID_INIT_FAILURE("ERROR: Photo sensor test Failed!!!");
const QString MSG_SLIDEID_STARTCOUNT_FAILURE("ERROR: Start Count Failed!!!");

/****************************************************************************/
const QString MSG_XYZ_INIT_FAILURE("ERROR: XYZ Module Init Failed!!!");
const QString MSG_XYZ_RACKRFID_SUCCESS("Rack RFID Value = ");
const QString MSG_XYZ_MOVETOSTN_FAILURE("ERROR: Move to Station Failed !!!");
const QString MSG_XYZ_CARRYTOSTN_FAILURE("ERROR: Carry Rack to Station Failed !!!");
const QString MSG_XYZ_LOWERRACK_FAILURE("ERROR: Lower Rack Failed !!!");
const QString MSG_XYZ_ATTACHRACK_FAILURE("ERROR: Attach Rack Failed !!!");
const QString MSG_XYZ_LIFTRACK_FAILURE("ERROR: Lift Rack Failed !!!");
const QString MSG_XYZ_DETACHRACK_FAILURE("ERROR: Detach Rack Failed !!!");
const QString MSG_XYZ_COUNTSLIDES_SUCCESS("Slide Count Successful. Count = ");
const QString MSG_XYZ_COUNTSLIDES_FAILURE("ERROR: Slide Count Failed !!!");

/****************************************************************************/
const QString MSG_EXHAUST_INIT_SUCCESS("Exhaust Device Initialization Successful");
const QString MSG_EXHAUST_INIT_FAILURE("ERROR: Exhaust Device Initialization Failed!!!");
const QString MSG_EXHAUST_FANON_SUCCESS("Exhaust Device FAN ON Successful");
const QString MSG_EXHAUST_FANON_FAILURE("ERROR: Exhaust Device FAN ON Failed!!!");
const QString MSG_EXHAUST_FANOFF_SUCCESS("Exhaust Device FAN OFF Successful");
const QString MSG_EXHAUST_FANOFF_FAILURE("ERROR: Exhaust Device FAN OFF Failed!!!");
const QString MSG_EXHAUST_AIRFLOWSTATUS_OK("Air Flow Status : OK");
const QString MSG_EXHAUST_AIRFLOWSTATUS_NOK("ERROR: Air Flow Status : NOK");
const QString MSG_EXHAUST_FANSTATUS_OK("Fan Current Status : OK");
const QString MSG_EXHAUST_FANSTATUS_NOK("ERROR: Fan Current Status : NOK");
const QString MSG_EXHAUST_INVALID_COMMAND("ERROR: Exhaust Invalid Command!!!");

/****************************************************************************/
const QString MSG_WATERSTN_INIT_FAILURE("ERROR: Water Device Initialization Failed!!!");
const QString MSG_WATERSTN_OPEN_SUCCESS("Water Valve Opened successfully");
const QString MSG_WATERSTN_OPEN_FAILURE("ERROR: Water Valve open Failed!!!");
const QString MSG_WATERSTN_CLOSE_SUCCESS("Water Valve Closed successfully");
const QString MSG_WATERSTN_CLOSE_FAILURE("ERROR: Water Valve Close Failed!!!");
const QString MSG_WATERSTN_INVALID_COMMAND("ERROR: Water Valve Invalid Command!!!");

/****************************************************************************/
const QString MSG_LIGHTDEVICE_INIT_FAILURE("ERROR: Light Device Initialization Failed!!!");
const QString MSG_LIGHTDEVICE_ON_SUCCESS("Light Device Switch ON Successful");
const QString MSG_LIGHTDEVICE_OFF_SUCCESS("Light Device Switch OFF Successful");
const QString MSG_LIGHTDEVICE_ON_FAILURE("ERROR: Light Device Switch ON Failed!!!");
const QString MSG_LIGHTDEVICE_OFF_FAILURE("ERROR: Light Device Switch OFF Failed!!!");
const QString MSG_LIGHTDEVICE_INVALID_COMMAND("ERROR: Light Device Invalid Command!!!");

/****************************************************************************/
const QString MSG_HOODDEVICE_INIT_FAILURE("ERROR: Hood Device Initialization Failed!!!");
const QString MSG_HOODDEVICE_OPEN("Cover/Hood is Open");
const QString MSG_HOODDEVICE_CLOSE("Cover/Hood is Closed");

/****************************************************************************/
const QString MSG_XYZ1_CANID("YZ Left CAN ID : ");
const QString MSG_XYZ2_CANID("YZ Right CAN ID : ");
const QString MSG_LOADER_CANID("Loader CAN ID : ");
const QString MSG_UNLOADER_CANID("UnLoader CAN ID : ");
const QString MSG_OVEN_CANID("Oven CAN ID : ");
const QString MSG_HCUVETTES_CANID("Heated Cuvettes CAN ID : ");
const QString MSG_AGITATION_CANID("Agitation CAN ID : ");
const QString MSG_RACKTRANSFER_CANID("Rack Transfer CAN ID : ");
const QString MSG_EXHAUST_CANID("Exhaust Device CAN ID : ");
const QString MSG_WATER_CANID("Water Device CAN ID : ");
const QString MSG_HOOD_CANID("Cover Hood CAN ID : ");
const QString MSG_LIGHT_CANID("Light Device CAN ID : ");
const QString MSG_SLIDEID_CANID("Slide ID CAN ID : ");
const QString MSG_INVALID_CANID_COMMAND("ERROR: Unknown CANID Command!!!");

/****************************************************************************/
const QString CANID_XYZ1("0x0002 0x0001");
const QString CANID_XYZ2("0x0102 0x0101");
const QString CANID_LOADER("0x0004");
const QString CANID_UNLOADER("0x0104");
const QString CANID_OVEN("0x0003");
const QString CANID_HCUVETTES("0x0005");
const QString CANID_RACKTRANSFER("0x0202");
const QString CANID_SLIDEID("0x?\?\?");
const QString CANID_AGITATION("0x?\?\?\?");
const QString CANID_EXHAUST("0x?\?\?\?");
const QString CANID_WATER("0x?\?\?\?");
const QString CANID_HOOD("0x?\?\?\?");
const QString CANID_LIGHT("0x?\?\?\?");

/****************************************************************************/
const QString MSG_XYZ1_FIRMWAREINFO("YZ Firmware Info : ");
const QString MSG_XYZ2_FIRMWAREINFO("YZ Firmware Info : ");
const QString MSG_LOADER_FIRMWAREINFO("Loader Firmware Info : ");
const QString MSG_UNLOADER_FIRMWAREINFO("UnLoader Firmware Info : ");
const QString MSG_OVEN_FIRMWAREINFO("Oven Firmware Info : ");
const QString MSG_HCUVETTES_FIRMWAREINFO("Heated Cuvettes Firmware Info : ");
const QString MSG_AGITATION_FIRMWAREINFO("Agitation Firmware Info : ");
const QString MSG_RACKTRANSFER_FIRMWAREINFO("Rack Transfer Firmware Info : ");
const QString MSG_EXHAUST_FIRMWAREINFO("Exhaust Firmware Info : ");
const QString MSG_WATER_FIRMWAREINFO("Water Firmware Info : ");
const QString MSG_HOOD_FIRMWAREINFO("Hood Firmware Info : ");
const QString MSG_LIGHT_FIRMWAREINFO("Light Firmware Info : ");
const QString MSG_SLIDEID_FIRMWAREINFO("Slide ID Firmware Info : ");
const QString MSG_INVALID_FIRMWAREINFO_COMMAND("ERROR: Unknown FirmwareInfo Command!!!");
/****************************************************************************/

const QString MSG_DEVICEDATACONTAINERS_MISSING("ERROR: ProcessSettings.xml or Adjustment.xml files are missing!!!");


/****************************************************************************/
const QString MSG_DEVICE_NOT_INITIALIZED("ERROR: Device Not Initialized!!!");
const QString MSG_TEST_ABORT_FAILUE("ERROR: Test Aborted!!!");
const QString MSG_LSENSOR_DETECTING_FAILURE("ERROR: Level Sensor Detecting Test Failed!!!");
const QString MSG_LSENSOR_DETECTING_TIMEOUT("ERROR: Level Sensor Detecting Test Failed [Timeout]!!!");
const QString MSG_LSENSOR_DETECTING_OVERFLOW("ERROR: Level Sensor Detecting Test Failed [Overflow]!!!");
const QString MSG_LSENSOR_DETECTING_INSUFFICIENT("ERROR: Level Sensor Detecting Test Failed [Insufficient]!!!");
const QString MSG_LSENSOR_DETECTING_SUCCESS("Level Sensor Detecting Test successful");
const QString MSG_LSENSOR_HEATING_FAILURE("ERROR: Level Sensor Heating Test Failed!!!");
const QString MSG_LSENSOR_HEATING_SUCCESS("Level Sensor Heating Test successful");
const QString MSG_RV_HEATING_FAILURE("ERROR: Rotary Valve Heating Test Failed!!!");
const QString MSG_RV_HEATING_SUCCESS("Rotary Valve Heating Test successful");
const QString MSG_RV_MOVETO_INITPOS_FAILURE("ERROR: Moving Rotary Valve to Initial Position Test Failed!!!");
const QString MSG_RV_MOVETO_INITPOS_SUCCESS("Moving Rotary Valve to Initial Position Test successful");
const QString MSG_RV_MOVETO_TUBEPOS_FAILURE("ERROR: Moving Rotary Valve to Tube Position Test Failed!!!");
const QString MSG_RV_MOVETO_TUBEPOS_SUCCESS("Moving Rotary Valve to Tube Position Test successful");
const QString MSG_RV_MOVETO_SEALPOS_FAILURE("ERROR: Moving Rotary Valve to Seal Position Test Failed!!!");
const QString MSG_RV_MOVETO_SEALPOS_SUCCESS("Moving Rotary Valve to Seal Position Test successful");
const QString MSG_OVEN_HEATING_FAILURE("ERROR: Oven Heating Test Failed!!!");
const QString MSG_OVEN_HEATING_SUCCESS("Oven Heating Test successful");
const QString MSG_OVEN_COVER_SENSOR_SUCCESS("Oven Cover Sensor Test successful");
const QString MSG_TUBE1_HEATING_FAILURE("ERROR: Belt1 Heating Test Failed!!!");
const QString MSG_TUBE1_HEATING_SUCCESS("Belt1 Heating Test successful");
const QString MSG_TUBE2_HEATING_FAILURE("ERROR: Belt2 Heating Test Failed!!!");
const QString MSG_TUBE2_HEATING_SUCCESS("Belt2 Heating Test successful");
/****************************************************************************/

} // end namespace Service

#endif // GLOBAL_SERVICEDEFINES_H
