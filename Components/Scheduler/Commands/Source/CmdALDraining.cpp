/****************************************************************************/
/*! \file CmdALDraining.cpp
 *
 *  \brief CmdALDraining command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 29.03.2013
 *   $Author:  $ L. Yang
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


#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdALDraining::NAME = "Scheduler::ALDraining";

CmdALDraining::CmdALDraining(int Timeout, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout, controller),m_TargetPressure(AL_TARGET_PRESSURE_POSITIVE), m_IgnorePressure(false)
{
}

CmdALDraining::~CmdALDraining(){}

}

