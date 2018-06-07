/****************************************************************************/
/*! \file CmdALPurge.cpp
 *
 *  \brief CmdALPurge command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 07.06.2018
 *   $Author:  $ Dixiong Li
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


#include "Scheduler/Commands/Include/CmdALPurge.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{

QString CmdALPurge::NAME = "Scheduler::ALPurge";

CmdALPurge::CmdALPurge(int Timeout, const QString& sender):
    CmdSchedulerCommandBase(Timeout, sender)
{
}

CmdALPurge::~CmdALPurge(){}

}
