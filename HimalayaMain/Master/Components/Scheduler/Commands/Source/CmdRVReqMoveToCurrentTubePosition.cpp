/****************************************************************************/
/*! \file CmdRVReqMoveToCurrentTubePosition.cpp
 *
 *  \brief CmdRVReqMoveToCurrentTubePosition command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 10.12.2015
 *   $Author:  $ Songtao Yu
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2015 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/


#include "Scheduler/Commands/Include/CmdRVReqMoveToCurrentTubePosition.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdRVReqMoveToCurrentTubePosition::NAME = "Scheduler::RVReqMoveToCurrentTubePosition";

CmdRVReqMoveToCurrentTubePosition::CmdRVReqMoveToCurrentTubePosition(int Timeout, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout, controller)
{
}

CmdRVReqMoveToCurrentTubePosition::~CmdRVReqMoveToCurrentTubePosition(){}

}

