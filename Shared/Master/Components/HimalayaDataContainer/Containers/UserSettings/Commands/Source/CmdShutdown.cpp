/****************************************************************************/
/*! \file CmdShutdown.cpp
 *
 *  \brief Notice Scheduler that System will shutdown.
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

#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdShutdown.h"



namespace MsgClasses {

QString CmdShutdown::NAME = "MsgClasses::CmdShutdown";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   Timeout     Timeout for command.
 */
/****************************************************************************/
CmdShutdown::CmdShutdown(int TimeOut)
    : Command(TimeOut)
{
}

CmdShutdown::CmdShutdown(void)
    : Command(0)
{
}

CmdShutdown::~CmdShutdown(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdShutdown::GetName(void) const
{
    return NAME;
}

} // end namespace MsgClasses
