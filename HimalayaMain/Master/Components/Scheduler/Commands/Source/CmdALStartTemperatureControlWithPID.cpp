/****************************************************************************/
/*! \file CmdALStartTemperatureControlWithPID.cpp
 *
 *  \brief CmdALStartTemperatureControlWithPID command definition.
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


#include "Scheduler/Commands/Include/CmdALStartTemperatureControlWithPID.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdALStartTemperatureControlWithPID::NAME = "Scheduler::ALStartTemperatureControlWithPID";

CmdALStartTemperatureControlWithPID::CmdALStartTemperatureControlWithPID(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller):
    CmdSchedulerCommandBase(Timeout,IDP, controller)
{
}

CmdALStartTemperatureControlWithPID::~CmdALStartTemperatureControlWithPID(){}


void CmdALStartTemperatureControlWithPID::Execute()
{
    m_result = m_IDeviceProcessing->ALStartTemperatureControlWithPID(m_Type, m_NominalTemperature, m_SlopeTempChange, m_MaxTemperature, m_ControllerGain, m_ResetTime, m_DerivativeTime);
    mp_SchedulerThreadController->PushDeviceControlCmdQueue(this);
}

}

