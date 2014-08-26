/****************************************************************************/
/*! \file CmdRmtLocAlarm.cpp
 *
 *  \brief CmdIDBottleCheck command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 21.08.2014
 *   $Author:  $ Jeff Chen
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/


#include "Scheduler/Commands/Include/CmdRmtLocAlarm.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler {

QString CmdRmtLocAlarm::NAME = "Scheduler::RmtLocAlarm";

CmdRmtLocAlarm::CmdRmtLocAlarm(int Timeout, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout, controller),
    opcode(-1)
{
}

CmdRmtLocAlarm::~CmdRmtLocAlarm()
{
}

}
