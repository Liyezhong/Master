/****************************************************************************/
/*! \file CmdRetortLock.cpp
 *
 *  \brief CmdRetortLock command implementation.
 *
 *   $Version: $ 0.1
*   $Date:    $ 25.03.2013
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
  *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortLock.h"
namespace MsgClasses {

QString CmdRetortLock::NAME = "MsgClasses::CmdRetortLock";


CmdRetortLock::CmdRetortLock(int Timeout, bool IsLock) :
    Command(Timeout),
    m_Lock(IsLock)
{
}

CmdRetortLock::CmdRetortLock()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdRetortLock::~CmdRetortLock()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdRetortLock::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
