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

const quint32 SCHEDULER_BASE_ID = 0x04010000;  ///<  Base event id of Scheduler

//slave configurations
const quint32 EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_SUCCESSFULLY = SCHEDULER_BASE_ID + 1; ///< Slave boards initialized successfully.
const quint32 EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_FAILURE = SCHEDULER_BASE_ID + 2; ///< Error happend during Slaves initializing %1.
const quint32 EVENT_SCHEDULER_OPEN_MAIN_RELAY_SUCCESSFULLY = SCHEDULER_BASE_ID + 3; ///< Main Relay opened successfully.
const quint32 EVENT_SCHEDULER_OPEN_MAIN_RELAY_FAILURE = SCHEDULER_BASE_ID + 4; ///< Fail to open the Main Relay %1.

// Action command from GUI or Event Handler
const quint32 EVENT_SCHEDULER_REC_ACTION_SHUTDOWN = SCHEDULER_BASE_ID + 10; ///< Scheduler received shutdown action.
const quint32 EVENT_SCHEDULER_REC_ERROR_ACTION = SCHEDULER_BASE_ID + 11; ///< Scheduler received Error recovery action %1.
const quint32 EVENT_SCHEDULER_REC_START_PROGRAM = SCHEDULER_BASE_ID + 12; ///< Scheduler receive action to start/continue the program %1.
const quint32 EVENT_SCHEDULER_REC_PAUSE_PROGRAM = SCHEDULER_BASE_ID + 13; ///< Scheduler receive action to pause the program %1.
const quint32 EVENT_SCHEDULER_REC_ABORT_PROGRAM = SCHEDULER_BASE_ID + 14; ///< Scheduler receive action to abort the program %1.
const quint32 EVENT_SCHEDULER_REC_DRAIN_PROGRAM = SCHEDULER_BASE_ID + 15; ///< Scheduler receive action to drain the last reagent of the program %1.


#endif // SCHEDULEREVENTCODES_H
