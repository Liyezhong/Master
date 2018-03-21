/****************************************************************************/
/*! \file CmdALFilling.cpp
 *
 *  \brief CmdALFilling command definition.
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


#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdALFilling::NAME = "Scheduler::ALFilling";

CmdALFilling::CmdALFilling(int Timeout, const QString& sender):
    CmdSchedulerCommandBase(Timeout, sender), m_SafeReagent4Paraffin(false)
{
}

CmdALFilling::~CmdALFilling(){}

}

