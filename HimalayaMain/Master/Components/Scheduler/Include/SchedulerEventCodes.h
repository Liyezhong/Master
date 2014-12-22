/****************************************************************************/
/*! \file SchedulerEventCodes.h
 *
 *  \brief All event codes used by Error handling for Himalaya device .
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-2-27
 *  $Author:    $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SCHEDULEREVENTCODES_H
#define SCHEDULEREVENTCODES_H
#include <QStringList>
#include "Global/Include/EventObject.h"
#include "../../../Include/HimalayaEventCodes.h"
#include "Scheduler/Include/SchedulerStateMachineStates.h"

namespace Scheduler{
const quint32 SCHEDULER_BASE_ID = 0x04010000;  ///<  Base event id of Scheduler

//slave configurations
const quint32 EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_SUCCESSFULLY = SCHEDULER_BASE_ID + 0x0001; ///< Slave boards initialized successfully.
const quint32 EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_FAILURE = SCHEDULER_BASE_ID + 0x0002; ///< Error happend during Slaves initializing %1.
const quint32 EVENT_SCHEDULER_OPEN_MAIN_RELAY_SUCCESSFULLY = SCHEDULER_BASE_ID + 0x0003; ///< Main Relay opened successfully.
const quint32 EVENT_SCHEDULER_OPEN_MAIN_RELAY_FAILURE = SCHEDULER_BASE_ID + 0x0004; ///< Fail to open the Main Relay %1.

// Action command from GUI or Event Handler
const quint32 EVENT_SCHEDULER_REC_ACTION_SHUTDOWN = SCHEDULER_BASE_ID + 0x0010; ///< User shutdown instrument.
const quint32 EVENT_SCHEDULER_REC_ERROR_ACTION = SCHEDULER_BASE_ID + 0x0011; ///<  Do recovery action %1.
const quint32 EVENT_SCHEDULER_REC_START_PROGRAM = SCHEDULER_BASE_ID + 0x0012; ///< User start the program %1 with the finished time %2.
const quint32 EVENT_SCHEDULER_REC_CONTINUE_PROGRAM = SCHEDULER_BASE_ID + 0x0013; ///< User continue the program %1.
const quint32 EVENT_SCHEDULER_REC_PAUSE_PROGRAM = SCHEDULER_BASE_ID + 0x0014; ///< User pause the program %1.
const quint32 EVENT_SCHEDULER_REC_ABORT_PROGRAM = SCHEDULER_BASE_ID + 0x0015; ///< User abort the program %1.
const quint32 EVENT_SCHEDULER_REC_DRAIN_PROGRAM = SCHEDULER_BASE_ID + 0x0016; ///< User drain the last reagent of the program %1.
const quint32 EVENT_SCHEDULER_REC_CHANGE_MELTING_POINT = SCHEDULER_BASE_ID + 0x0017; ///< User change the paraffin melting point from %1 to %2.

// Self test
const quint32 EVENT_SCHEDULER_START_SELFTEST = SCHEDULER_BASE_ID + 0x0100; ///< Start the Selftest.
const quint32 EVENT_SCHEDULER_DELAY_3S = SCHEDULER_BASE_ID + 0x0101; ///< Delay 3 second.
const quint32 EVENT_SCHEDULER_DELAY_5S = SCHEDULER_BASE_ID + 0x0102; ///< Delay 5 second.
const quint32 EVENT_SCHEDULER_ASB3VOLTAGE_SWITCHAUTO = SCHEDULER_BASE_ID + 0x0103; ///< Set ASB3 voltage switch to auto.
const quint32 EVENT_SCHEDULER_ASB3VOLTAGE_SWITCHAUTO_SUCCESS = SCHEDULER_BASE_ID + 0x0104; ///< Set the ASB3 voltage switch to auto successfully.
const quint32 EVENT_SCHEDULER_ASB5VOLTAGE_SWITCHAUTO = SCHEDULER_BASE_ID + 0x0105; ///< Set the ASB5 voltage switch to auto.
const quint32 EVENT_SHCEDULER_ASB5VOLTAGE_SWITCHAUTO_SUCESS = SCHEDULER_BASE_ID + 0x0106; ///< Set the ASB5 voltage switch to auto successfully.
const quint32 EVENT_SCHEDULER_START_ROTARYVALVE_TEMP = SCHEDULER_BASE_ID + 0x0107; ///< Start heating rotary valve.
const quint32 EVENT_SCHEDULER_START_RETORTBOTTOM_TEMP = SCHEDULER_BASE_ID + 0x0108; ///< Start heating retort bottom.
const quint32 EVENT_SCHEDULER_START_ROTARYVALVE_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x0109; ///< Start heating rotary valve successfully.
const quint32 EVENT_SCHEDULER_START_RETORTBOTTOM_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x010A; ///< Start heating retort bottom successfully.
const quint32 EVENT_SCHEDULER_STOP_ROTARYVALVE_TEMP = SCHEDULER_BASE_ID + 0x010B; ///< Stop heating rotary valve.
const quint32 EVENT_SCHEDULER_STOP_RETORTBOTTOM_TEMP = SCHEDULER_BASE_ID + 0x010C; ///< Stop heating retort bottom.
const quint32 EVENT_SCHEDULER_STOP_ROTARYVALVE_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x010D; ///< Stop heating rotary valve successfully.
const quint32 EVENT_SCHEDULER_STOP_RETORTBOTTOM_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x010E; ///< Stop heating retort bottom successfully.
const quint32 EVENT_SCHEDULER_ASB5_ASB3_SWITCHTYPE = SCHEDULER_BASE_ID + 0x010F; ///< Get the ASB5 SwithcType:%1,ASB3 SwitchType:%2.
const quint32 EVENT_SCHEDULER_RETRY_AC_VOLTAGE_TEST = SCHEDULER_BASE_ID + 0x0110; ///< Retry the AC voltage switch test.
const quint32 EVENT_SCHEDULER_UNKNOW_ASB5_SWITCHTYPE = SCHEDULER_BASE_ID + 0x0111; ///< Unknown the ASB5 voltage.
const quint32 EVENT_SCHEDULER_ASB5_SAME_ASB3 = SCHEDULER_BASE_ID + 0x0112; ///< ASB5 voltage type is same the ASB3, and pass.
const quint32 EVENT_SCHEDULER_SET_ASB3VOLTAGE_WITH_ASB5 = SCHEDULER_BASE_ID + 0x0113; ///< Set the ASB3 voltage with ASB5 voltage.
const quint32 EVENT_SCHEDULER_SET_ASB3VOLTAGE_WITH_ASB5_SUCCESS = SCHEDULER_BASE_ID + 0x114; ///< Set the ASB3 voltage with ASB5 voltage successfully.
const quint32 EVENT_SCHEDULER_START_TUBE1_TEMP = SCHEDULER_BASE_ID + 0x115; ///< Start heating liquid tube.
const quint32 EVENT_SCHEDULER_START_TUBE1_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x116; ///< Start heating liquid tube successfully.
const quint32 EVENT_SCHEDULER_STOP_TUBE1_TEMP = SCHEDULER_BASE_ID + 0x117; ///< Stop heating liquid tube.
const quint32 EVENT_SCHEDULER_STOP_TUBE1_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x118; ///< Stop heating liquid tube successfully.
const quint32 EVENT_SCHEDULER_START_TUBE2_TEMP = SCHEDULER_BASE_ID + 0x119; ///< Start heating air tube.
const quint32 EVENT_SCHEDULER_START_TUBE2_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x11A; ///< Start heating air tube successfully.
const quint32 EVENT_SCHEDULER_STOP_TUBE2_TEMP = SCHEDULER_BASE_ID + 0x11B; ///< Stop heating air tube.
const quint32 EVENT_SCHEDULER_STOP_TUBE2_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x11C; ///< Stop heating air tube successfully.
const quint32 EVENT_SCHEDULER_START_LEVELSENSOR_TEMP = SCHEDULER_BASE_ID + 0x11D; ///< Start heating level sensor.
const quint32 EVENT_SCHEDULER_START_LEVELSENSOR_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x11E; ///< Start heating level sensor successfully.
const quint32 EVENT_SCHEDULER_STOP_LEVELSENSOR_TEMP = SCHEDULER_BASE_ID + 0x11F; ///< Stop heating level sensor.
const quint32 EVENT_SCHEDULER_STOP_LEVELSENSOR_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x120; ///< Stop heating level sensor successfully.
const quint32 EVENT_SCHEDULER_START_PUMP = SCHEDULER_BASE_ID + 0x121; ///< Start pump.
const quint32 EVENT_SCHEDULER_START_PUMP_SUCCESS = SCHEDULER_BASE_ID + 0x122; ///< Start pump successfully.
const quint32 EVENT_SCHEDULER_STOP_PUMP = SCHEDULER_BASE_ID + 0x123; ///< Stop pump.
const quint32 EVENT_SCHEDULER_STOP_PUMP_SUCCESS = SCHEDULER_BASE_ID + 0x124; ///< Stop pump successfully.
const quint32 EVENT_SCHEDULER_START_VALVE1 = SCHEDULER_BASE_ID + 0x125; ///< Start valve1.
const quint32 EVENT_SCHEDULER_START_VALVE1_SUCCESS = SCHEDULER_BASE_ID + 0x126; ///< Start valve1 successfully.
const quint32 EVENT_SCHEDULER_STOP_VALVE1 = SCHEDULER_BASE_ID + 0x127; ///< Stop valve1.
const quint32 EVENT_SCHEDULER_STOP_VALVE1_SUCCESS = SCHEDULER_BASE_ID + 0x128; ///< Stop valve1 successfully.
const quint32 EVENT_SCHEDULER_START_VALVE2 = SCHEDULER_BASE_ID + 0x129; ///< Start valve2.
const quint32 EVENT_SCHEDULER_START_VALVE2_SUCCESS = SCHEDULER_BASE_ID + 0x12A; ///< Start valve2 successfully.
const quint32 EVENT_SCHEDULER_STOP_VALVE2 = SCHEDULER_BASE_ID + 0x12B; ///< Stop valve2.
const quint32 EVENT_SCHEDULER_STOP_VALVE2_SUCCESS = SCHEDULER_BASE_ID + 0x12C; ///< Stop valve2 successfully.
const quint32 EVENT_SCHEDULER_START_FAN = SCHEDULER_BASE_ID + 0x12D; ///< Start exhaust fan.
const quint32 EVENT_SCHEDULER_START_FAN_SUCCESS = SCHEDULER_BASE_ID + 0x12E; ///< Start exhaust fan successfully.
const quint32 EVENT_SCHEDULER_STOP_FAN = SCHEDULER_BASE_ID + 0x12F; ///< Stop exhaust fan.
const quint32 EVENT_SCHEDULER_STOP_FAN_SUCCESS = SCHEDULER_BASE_ID + 0x130; ///< Stop exhaust fan successfully.
const quint32 EVENT_SCHEDULER_START_RETORTSIDE_TEMP = SCHEDULER_BASE_ID + 0x131; ///< Start heating retort side.
const quint32 EVENT_SCHEDULER_START_RETORTSIDE_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x132; ///< Start heating retort side successfully.
const quint32 EVENT_SCHEDULER_STOP_RETORTSIDE_TEMP = SCHEDULER_BASE_ID + 0x133; ///< Stop heating retort side.
const quint32 EVENT_SCHEDULER_STOP_RETORTSIDE_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x134; ///< Stop heating retort side successfully.
const quint32 EVENT_SCHEDULER_START_OVENTOP_TEMP = SCHEDULER_BASE_ID + 0x135; ///< Start heating oven top.
const quint32 EVENT_SCHEDULER_START_OVENTOP_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x136; ///< Start heating oven top successfully.
const quint32 EVENT_SCHEDULER_STOP_OVENTOP_TEMP = SCHEDULER_BASE_ID + 0x137; ///< Stop heating oven top.
const quint32 EVENT_SCHEDULER_STOP_OVENTOP_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x138; ///< Stop heating oven top successfully.
const quint32 EVENT_SCHEDULER_START_OVENBOT_TEMP = SCHEDULER_BASE_ID + 0x139; ///< Start heating oven bottom.
const quint32 EVENT_SCHEDULER_START_OVENBOT_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x13A; ///< Start heating oven bottom successfully.
const quint32 EVENT_SCHEDULER_STOP_OVENBOT_TEMP = SCHEDULER_BASE_ID + 0x13B; ///< Stop heating oven bottom.
const quint32 EVENT_SCHEDULER_STOP_OVENBOT_TEMP_SUCCESS = SCHEDULER_BASE_ID + 0x14C; ///< Stop heating oven bottom successfully.

// Pre test
const quint32 EVENT_SCHEDULER_START_PRETEST = SCHEDULER_BASE_ID + 0x0200; ///< Start the pretest.
const quint32 EVENT_SCHEDULER_CHECK_LEVELSENSOR = SCHEDULER_BASE_ID + 0x0201; ///< Check the level sensor temperature.
const quint32 EVENT_SCHEDULER_CHECK_LEVELSENSOR_SUCCESS = SCHEDULER_BASE_ID + 0x202; ///< Check the level sensor temperature passed.
const quint32 EVENT_SCHEDULER_MOVETO_INITIALIZE_POSITION = SCHEDULER_BASE_ID + 0x203; ///< Move RV to initialize position.
const quint32 EVENT_SCHEDULER_MOVETO_INITIALIZE_POSITION_SUCCESS = SCHEDULER_BASE_ID + 0x204; ///< Move RV to initialize position successfully.
const quint32 EVENT_SCHEDULER_PRESSURE_CALIBRATION = SCHEDULER_BASE_ID + 0x205; ///<  Test pressure calibration.
const quint32 EVENT_SCHEDULER_RELEASE_PRESSURE_CALIBRATION = SCHEDULER_BASE_ID + 0x206; ///< Calibrate pressure of release pressure.
const quint32 EVENT_SCHEDULER_RELEASE_PRESSURE_CALIBRATION_SUCCESS = SCHEDULER_BASE_ID + 0x207; ///< Calibrate pressure of release pressure successfully.
const quint32 EVENT_SCHEDULER_WAIT_20S = SCHEDULER_BASE_ID + 0x208; ///< Wait 20 second.
const quint32 EVENT_SCHEDULER_GET_CURRENT_PRESSURE = SCHEDULER_BASE_ID + 0x209; ///< Calibrate pressure the current pressure is :%1.
const quint32 EVENT_SCHEDULER_RETRY_PRESSURE_CALIBRATION = SCHEDULER_BASE_ID + 0x20A; ///< Retry Pressure Calibration the pressure value is too large.
const quint32 EVENT_SCHEDULER_PRESSURE_CALIBRATION_SUCCESS = SCHEDULER_BASE_ID + 0x20B; ///< Calibrate pressure successfully.
const quint32 EVENT_SCHEDULER_OFFSET_CALIBRATION = SCHEDULER_BASE_ID +  0x20C; ///< Offset the pressure calibration.
const quint32 EVENT_SCHEDULER_SEALING_TEST = SCHEDULER_BASE_ID + 0x20D; ///< Sealing test.
const quint32 EVENT_SCHEDULER_SEALING_TEST_SUCCESS = SCHEDULER_BASE_ID + 0x20E; ///< Sealing test successfully.
const quint32 EVENT_SCHEDULER_BOTTLE_CHECK = SCHEDULER_BASE_ID + 0x20F; ///< Bottle test.
const quint32 EVENT_SCHEDULER_BOTTLE_CHECK_SUCCESS = SCHEDULER_BASE_ID + 0x210; ///< Bottle test successfully.
const quint32 EVENT_SCHEDULER_PRETEST_SUCCESS = SCHEDULER_BASE_ID + 0x211; ///< PreTest success.

//heating strategy
const quint32 EVENT_SCHEDULER_HEATING_STRATEGY_INITIALIZE_FAILED = SCHEDULER_BASE_ID + 0x250; ///< Initialize the heatingStrategy failed.
const quint32 EVENT_SCHEDULER_HEATING_RETORTSIDE_PRETEST = SCHEDULER_BASE_ID + 0x251; ///< Start heating retort side in pretest,the target temperature:%1.
const quint32 EVENT_SCHEDULER_HEATING_RETORTBOT_PRETEST = SCHEDULER_BASE_ID + 0x252; ///< Start heating retort bottom in pretest,the target temperature:%1.
const quint32 EVENT_SCHEDULER_HEATING_LEVEL_SENSOR = SCHEDULER_BASE_ID + 0x253; ///< Start heating level sensor heating in scenario:%1,the target temperature:%2.
const quint32 EVENT_SCHEDULER_HEATING_RETORT = SCHEDULER_BASE_ID + 0x254; ///< Start retort(%1) heating in scenario:%2,the target temperature:%3.
const quint32 EVENT_SCHEDULER_HEATING_OVEN = SCHEDULER_BASE_ID + 0x255; ///< Start wax bath(%1) heating in scenario:%2,the target temperature:%3.
const quint32 EVENT_SCHEDULER_HEATING_RV = SCHEDULER_BASE_ID + 0x256; ///< Start rotary valve heating in scenario:%1,the target temperature:%2.
const quint32 EVENT_SCHEDULER_HEATING_LA = SCHEDULER_BASE_ID + 0x257; ///< Start LA system(%1) heating in scenario:%2,the target temperature:%3.

// running program
// 1. Power failudre
const quint32 EVENT_SCHEDULER_POWER_FAILURE = SCHEDULER_BASE_ID + 0x0300; ///< Detect an unfinished program %1 at step %2.
const quint32 EVENT_SCHEDULER_POWER_FAILURE_SPECIAL_STEP = SCHEDULER_BASE_ID + 0x0301; ///< Reheating for 200 and 260 step.
const quint32 EVENT_SCHEDULER_POWER_FAILURE_REAGENT_STEP = SCHEDULER_BASE_ID + 0x0302; ///< Reheating for reagent step.
const quint32 EVENT_SCHEDULER_POWER_FAILURE_PARAFFIN_STEP = SCHEDULER_BASE_ID + 0x0303; ///< Reheating for paraffin step.
const quint32 EVENT_SCHEDULER_POWER_FAILURE_BACK_281 = SCHEDULER_BASE_ID + 0x0304; ///< Power failure back to sensor 281.
const quint32 EVENT_SCHEDULER_POWER_FAILURE_BACK_291 = SCHEDULER_BASE_ID + 0x0305; ///< Power failure back to sensor 291.
const quint32 EVENT_SCHEDULER_POWER_FAILURE_BACK_DRY_STEP = SCHEDULER_BASE_ID + 0x0306; ///< Power failure back to RC_restart.

// 2. IDLE state
const quint32 EVENT_SCHEDULER_IN_IDLE_STATE = SCHEDULER_BASE_ID + 0x0310; ///< Scheduler enters the idle state.
const quint32 EVENT_SCHEDULER_IN_ERROR_STATE = SCHEDULER_BASE_ID + 0x0311; ///< Scheduler enters the Error state.
const quint32 EVENT_SCHEDULER_READ_PROGRAM_STATUS_FILE_FAIL = SCHEDULER_BASE_ID + 0x0312; ///< Not found unfinished program, please confirm.
// 3. runing program
const quint32 EVENT_SCHEDULER_START_PROGRAM = SCHEDULER_BASE_ID + 0x0320; ///< start the program %1
const quint32 EVENT_SCHEDULER_SET_RV_POSITION = SCHEDULER_BASE_ID + 0x0321; ///< Set the rotary valve position to %1.
const quint32 EVENT_SCHEDULER_OVEN_HEATING_TIME = SCHEDULER_BASE_ID + 0x0322; ///< Still need to heat oven for %1 for melting paraffin.
const quint32 EVENT_SCHEDULER_OVEN_ABORTING = SCHEDULER_BASE_ID + 0x0323; ///< Aborting program.
const quint32 EVENT_SCHEDULER_OVEN_ABORTED = SCHEDULER_BASE_ID + 0x0324; ///< Aborted program.
const quint32 EVENT_SCHEDULER_OVEN_PAUSE = SCHEDULER_BASE_ID + 0x0325; ///< Pause program.
const quint32 EVENT_SCHEDULER_RELEASE_PREASURE = SCHEDULER_BASE_ID + 0x0326; ///< Release presure.
const quint32 EVENT_SCHEDULER_DRAINING = SCHEDULER_BASE_ID + 0x0327; ///< Draining.
const quint32 EVENT_SCHEDULER_WAITING_FOR_FILLING_PARAFFIN = SCHEDULER_BASE_ID + 0x0328; ///< Waiting for filling paraffin.
const quint32 EVENT_SCHEDULER_HEATING_LEVEL_SENSOR_FOR_FILLING = SCHEDULER_BASE_ID + 0x0329; ///< Heating the level sensor for filling
const quint32 EVENT_SCHEDULER_START_FILLING = SCHEDULER_BASE_ID + 0x032A; ///< Start filling
const quint32 EVENT_SCHEDULER_MOVE_RV_TUBE_POSITION = SCHEDULER_BASE_ID + 0x032B; ///< Moving the rotary valve to the tube position of station %1.
const quint32 EVENT_SCHEDULER_MOVE_RV_SEALING_POSITION = SCHEDULER_BASE_ID + 0x032C; ///< Moveing the rotary valve to the sealing position of station %1.
const quint32 EVENT_SCHEDULER_START_PROCESSING = SCHEDULER_BASE_ID + 0x032D; ///< Start processing.
const quint32 EVENT_SCHEDULER_SET_PRESSURE = SCHEDULER_BASE_ID + 0x032E; ///< Set pressure to %1.
const quint32 EVENT_SCHEDULER_PROGRAM_STEP_FINISHED = SCHEDULER_BASE_ID + 0x032F; ///< Finished Step %1.
const quint32 EVENT_SCHEDULER_PROGRAM_STEP_START = SCHEDULER_BASE_ID + 0x0330; ///< Start Step %1(Station %2, %3, %4, %5, %6).
const quint32 EVENT_SCHEDULER_PROGRAM_FINISHED = SCHEDULER_BASE_ID + 0x0331; ///< Finished Program %1.
const quint32 EVENT_SCHEDULER_START_DRY_PROCESSING = SCHEDULER_BASE_ID + 0x0332; ///< Start the dry processing.
const quint32 EVENT_SCHEDULER_FINISHED_DRY_PROCESSING = SCHEDULER_BASE_ID + 0x0333; ///< Finished the dry processing.
const quint32 EVENT_SCHEDULER_FOR_WARNING_FLAG = SCHEDULER_BASE_ID + 0x0334; ///< Time between step %1 and step %2 exceed 4 minutes.


/// strings
const quint32 STR_SCHEDULER_PRECHECK = SCHEDULER_BASE_ID + 0x0400; ///< Prechecking
const quint32 STR_SCHEDULER_DRY_PROCESSING = SCHEDULER_BASE_ID + 0x0401; ///< Dry processing


/// Place holder for Service string for Error Handling
/// SCHEDULER_BASE_ID + 0x0600 ~ SCHEDULER_BASE_ID + 0x06ff

/// Place holder for stringID for Error Handling
/// SCHEDULER_BASE_ID + 0x0700 ~ SCHEDULER_BASE_ID + 0x07ff

} //end of Scheduler namespace
#endif // SCHEDULEREVENTCODES_H
