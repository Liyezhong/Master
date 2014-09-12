/****************************************************************************/
/*! \file CmdALForceDraining.cpp
 *
 *  \brief CmdALForceDraining command definition.
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


#include "Scheduler/Commands/Include/CmdALForceDraining.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdALForceDraining::NAME = "Scheduler::ALForceDraining";

CmdALForceDraining::CmdALForceDraining(int Timeout, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout, controller),m_TargetPressure(AL_TARGET_PRESSURE_POSITIVE)
{
}

CmdALForceDraining::~CmdALForceDraining(){}

}

