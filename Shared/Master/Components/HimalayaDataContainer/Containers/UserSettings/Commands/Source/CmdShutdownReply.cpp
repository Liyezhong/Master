/****************************************************************************/
/*! \file CmdShutdownReply.cpp
 *
 *  \brief After Slave board shutdown, scheduler will notify GUI by this command.
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

#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdShutdownReply.h"



namespace MsgClasses {

QString CmdShutdownReply::NAME = "MsgClasses::CmdShutdownReply";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   Timeout     Timeout for command.
 */
/****************************************************************************/
CmdShutdownReply::CmdShutdownReply(int TimeOut)
    : Command(TimeOut)
{
}

CmdShutdownReply::CmdShutdownReply(void)
    : Command(0)
{
}

CmdShutdownReply::~CmdShutdownReply(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdShutdownReply::GetName(void) const
{
    return NAME;
}

} // end namespace MsgClasses
