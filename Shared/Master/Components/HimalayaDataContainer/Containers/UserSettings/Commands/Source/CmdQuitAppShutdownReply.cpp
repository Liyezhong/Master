/****************************************************************************/
/*! \file CmdQuitAppShutdownReply.cpp
 *
 *  \brief  After some processes have done for "quit Application" or "shutdown", scheduler will notify GUI by this command.
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

#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdQuitAppShutdownReply.h"



namespace MsgClasses {

QString CmdQuitAppShutdownReply::NAME = "MsgClasses::CmdQuitAppShutdownReply";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   Timeout     Timeout for command.
 */
/****************************************************************************/
CmdQuitAppShutdownReply::CmdQuitAppShutdownReply(int TimeOut, DataManager::QuitAppShutdownActionType_t quitAppShutdownActionType)
    : Command(TimeOut), m_quitAppShutdownActionType(quitAppShutdownActionType)
{
}

CmdQuitAppShutdownReply::CmdQuitAppShutdownReply(void)
    : Command(0)
{
}

CmdQuitAppShutdownReply::~CmdQuitAppShutdownReply(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdQuitAppShutdownReply::GetName(void) const
{
    return NAME;
}

} // end namespace MsgClasses
