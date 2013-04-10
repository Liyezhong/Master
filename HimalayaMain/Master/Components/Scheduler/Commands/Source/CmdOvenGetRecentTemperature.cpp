/****************************************************************************/
/*! \file CmdOvenGetRecentTemperature.cpp
 *
 *  \brief CmdOvenGetRecentTemperature command definition.
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


#include "Scheduler/Commands/Include/CmdOvenGetRecentTemperature.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdOvenGetRecentTemperature::NAME = "Scheduler::OvenGetRecentTemperature";

CmdOvenGetRecentTemperature::CmdOvenGetRecentTemperature(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout,IDP, controller)
{
}

CmdOvenGetRecentTemperature::~CmdOvenGetRecentTemperature(){}


void CmdOvenGetRecentTemperature::Execute()
{
    m_result = m_IDeviceProcessing->OvenGetRecentTemperature(m_Type, m_Index);
}

}

