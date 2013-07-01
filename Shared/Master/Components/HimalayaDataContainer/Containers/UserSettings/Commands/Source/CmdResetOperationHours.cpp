/****************************************************************************/
/*! \file CmdResetOperationHours.cpp
 *
 *  \brief this command is used to disappeare the warning icon of maintainance reminder.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-06-27
 *  $Author:    $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdResetOperationHours.h"


namespace MsgClasses {

QString CmdResetOperationHours::NAME = "MsgClasses::CmdResetOperationHours";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   Timeout     Timeout for command.
 */
/****************************************************************************/
CmdResetOperationHours::CmdResetOperationHours(int TimeOut, DataManager::ResetOperationHoursType_t ResetOperationHoursType)
    : Command(TimeOut), m_ResetOperationHoursType(ResetOperationHoursType)
{
}

CmdResetOperationHours::CmdResetOperationHours(void)
    : Command(0)
{
}

CmdResetOperationHours::~CmdResetOperationHours(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdResetOperationHours::GetName(void) const
{
    return NAME;
}

} // end namespace MsgClasses
