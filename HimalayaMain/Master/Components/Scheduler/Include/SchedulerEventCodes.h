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
const quint32 EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_SUCCESSFULLY = SCHEDULER_BASE_ID + 1; ///< Slave boards initialized successfully.
const quint32 EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_FAILURE = SCHEDULER_BASE_ID + 2; ///< Error happend during Slaves initializing %1.
const quint32 EVENT_SCHEDULER_OPEN_MAIN_RELAY_SUCCESSFULLY = SCHEDULER_BASE_ID + 3; ///< Main Relay opened successfully.
const quint32 EVENT_SCHEDULER_OPEN_MAIN_RELAY_FAILURE = SCHEDULER_BASE_ID + 4; ///< Fail to open the Main Relay %1.

// Action command from GUI or Event Handler
const quint32 EVENT_SCHEDULER_REC_ACTION_SHUTDOWN = SCHEDULER_BASE_ID + 10; ///< User shutdown instrument.
const quint32 EVENT_SCHEDULER_REC_ERROR_ACTION = SCHEDULER_BASE_ID + 11; ///<  Do recovery action %1.
const quint32 EVENT_SCHEDULER_REC_START_PROGRAM = SCHEDULER_BASE_ID + 12; ///< User start/continue the program %1.
const quint32 EVENT_SCHEDULER_REC_PAUSE_PROGRAM = SCHEDULER_BASE_ID + 13; ///< User pause the program %1.
const quint32 EVENT_SCHEDULER_REC_ABORT_PROGRAM = SCHEDULER_BASE_ID + 14; ///< User abort the program %1.
const quint32 EVENT_SCHEDULER_REC_DRAIN_PROGRAM = SCHEDULER_BASE_ID + 15; ///< User drain the last reagent of the program %1.

// Self test
const quint32 EVENT_SCHEDULER_START_SELFTEST = SCHEDULER_BASE_ID + 0x0100; ///< Start the Selftest.
const quint32 EVENT_SCHEDULER_DELAY_3S = SCHEDULER_BASE_ID + 0x0101; ///< Delay 2 second.
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

// running program
// 1. Power failudre
const quint32 EVENT_SCHEDULER_POWER_FAILURE = SCHEDULER_BASE_ID + 0x0300; ///< Detect an unfinished program %1 at step %2.
// 2. IDLE state
const quint32 EVENT_SCHEDULER_IN_IDLE_STATE = SCHEDULER_BASE_ID + 0x0310; ///< Instrument enters the idle state.
// 3. runing program
const quint32 EVENT_SCHEDULER_START_PROGRAM = SCHEDULER_BASE_ID + 0x0320; ///< start the program %1
const quint32 EVENT_SCHEDULER_SET_RV_POSITION = SCHEDULER_BASE_ID + 0x0321; ///< Set the rotary valve position to %1.

} //end of Scheduler namespace
#endif // SCHEDULEREVENTCODES_H
