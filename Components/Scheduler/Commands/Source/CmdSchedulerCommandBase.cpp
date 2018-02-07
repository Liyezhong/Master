/****************************************************************************/
/*! \file CmdSchedulerCommandBase.cpp
 *
 *  \brief CmdSchedulerCommandBase command definition.
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

#include <QMetaObject>
#include <QMetaMethod>
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"



namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 */
/****************************************************************************/
CmdSchedulerCommandBase::CmdSchedulerCommandBase(int Timeout, SchedulerMainThreadController *controller):
    Command(Timeout),
    m_Response(true),
    mp_SchedulerThreadController(controller)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdSchedulerCommandBase::~CmdSchedulerCommandBase()
{
    /*lint -e1540 */
}


bool CmdSchedulerCommandBase::GetResult(DeviceControl::ReturnCode_t& result) const
{
    Q_UNUSED(result);
    return false;
}

bool CmdSchedulerCommandBase::GetResult(DeviceControl::CBaseDevice* result) const
{
    Q_UNUSED(result);
    return false;
}

bool CmdSchedulerCommandBase::GetResult(DeviceControl::TempCtrlState_t& result) const
{
    Q_UNUSED(result);
    return false;
}

bool CmdSchedulerCommandBase::GetResult(qreal& result) const
{
    Q_UNUSED(result);
    return false;
}

void CmdSchedulerCommandBase::SetResult(DeviceControl::ReturnCode_t result)
{
    Q_UNUSED(result);
}

void CmdSchedulerCommandBase::SetResult(qreal result)
{
    Q_UNUSED(result);
}

void CmdSchedulerCommandBase::SetResult( DeviceControl::TempCtrlState_t result)
{
    Q_UNUSED(result);
}

} // end of namespace Scheduler
