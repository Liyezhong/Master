/****************************************************************************/
/*! \file CmdRaiseAlarm.cpp
 *
 *  \brief Definition file for class CmdRaiseAlarm
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-3-4
 *  $Author:    $ Abe Yang
 *
 *  \b Description:
 *
 *  Send this command for local or remore alarm.       
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

#include "../Include/Commands/CmdRaiseAlarm.h"

namespace HimalayaErrorHandler {

QString CmdRaiseAlarm::NAME = "HimalayaErrorHandler::CmdRaiseAlarm";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   stateId  state ID.
 */
/****************************************************************************/
CmdRaiseAlarm::CmdRaiseAlarm(bool isLocalAlarm) :
    Command(5000),
    m_localAlarm(isLocalAlarm)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdRaiseAlarm::~CmdRaiseAlarm()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdRaiseAlarm::GetName() const
{
    return NAME;
}

} // end namespace HimalayaErrorHandler
