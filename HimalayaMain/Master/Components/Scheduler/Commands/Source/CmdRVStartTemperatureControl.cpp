/****************************************************************************/
/*! \file CmdRVStartTemperatureControl.cpp
 *
 *  \brief CmdRVStartTemperatureControl command definition.
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


#include "Scheduler/Commands/Include/CmdRVStartTemperatureControl.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdRVStartTemperatureControl::NAME = "Scheduler::RVStartTemperatureControl";

CmdRVStartTemperatureControl::CmdRVStartTemperatureControl(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout,IDP, controller)
{
}

CmdRVStartTemperatureControl::~CmdRVStartTemperatureControl(){}


void CmdRVStartTemperatureControl::Execute()
{
    m_result = m_IDeviceProcessing->RVStartTemperatureControl(m_NominalTemperature, m_SlopeTempChange);
}

}

