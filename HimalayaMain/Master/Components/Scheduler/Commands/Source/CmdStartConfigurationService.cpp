/****************************************************************************/
/*! \file CmdStartConfigurationService.cpp
 *
 *  \brief CmdStartConfigurationService command definition.
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


#include "Scheduler/Commands/Include/CmdStartConfigurationService.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdStartConfigurationService::NAME = "Scheduler::StartConfigurationService";

CmdStartConfigurationService::CmdStartConfigurationService(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout,IDP, controller)
{
}

CmdStartConfigurationService::~CmdStartConfigurationService(){}


void CmdStartConfigurationService::Execute()
{
    m_result = m_IDeviceProcessing->StartConfigurationService();
}

}

