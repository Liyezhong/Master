/****************************************************************************/
/*! \file CmdALAllStop.cpp
 *
 *  \brief CmdALAllStop command definition.
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


#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdALAllStop::NAME = "Scheduler::ALAllStop";

CmdALAllStop::CmdALAllStop(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout,IDP, controller)
{
}

CmdALAllStop::~CmdALAllStop(){}


void CmdALAllStop::Execute()
{
    m_result = m_IDeviceProcessing->ALAllStop();
}

}

