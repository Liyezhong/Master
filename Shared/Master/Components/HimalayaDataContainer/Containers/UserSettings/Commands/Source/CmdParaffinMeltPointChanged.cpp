/****************************************************************************/
/*! \file CmdParaffinMeltPointChanged.cpp
 *
 *  \brief this command is used to disappeare the warning icon of maintainance reminder.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-09-18
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

#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdParaffinMeltPointChanged.h"


namespace MsgClasses {

QString CmdParaffinMeltPointChanged::NAME = "MsgClasses::CmdParaffinMeltPointChanged";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   timeOut     Timeout for command.
 *
 */
/****************************************************************************/
CmdParaffinMeltPointChanged::CmdParaffinMeltPointChanged(int timeOut)
    : Command(timeOut)
{
}

CmdParaffinMeltPointChanged::CmdParaffinMeltPointChanged(void)
    : Command(0)
{
}

CmdParaffinMeltPointChanged::~CmdParaffinMeltPointChanged(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdParaffinMeltPointChanged::GetName(void) const
{
    return NAME;
}

} // end namespace MsgClasses
