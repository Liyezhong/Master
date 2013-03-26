/****************************************************************************/
/*! \file CmdRetortLockStatus.cpp
 *
 *  \brief CmdRetortLockStatus command implementation.
 *
 *   $Version: $ 0.1
*   $Date:    $ 26.03.2013
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
  *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortLockStatus.h"
namespace MsgClasses {

QString CmdRetortLockStatus::NAME = "MsgClasses::CmdRetortLockStatus";


CmdRetortLockStatus::CmdRetortLockStatus(int Timeout, bool IsLocked) :
    Command(Timeout),
    m_Locked(IsLocked)
{
}

CmdRetortLockStatus::CmdRetortLockStatus()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdRetortLockStatus::~CmdRetortLockStatus()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdRetortLockStatus::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
