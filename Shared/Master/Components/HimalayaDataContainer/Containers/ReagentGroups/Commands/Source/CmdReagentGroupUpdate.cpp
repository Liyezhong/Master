/****************************************************************************/
/*! \file CmdReagentGroupUpdate.cpp
 *
 *  \brief Update reagent group command implementation.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-02
 *  $Author:    $ R. Wu
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

#include "HimalayaDataContainer/Containers/ReagentGroups/Commands/Include/CmdReagentGroupUpdate.h"

#include <QBuffer>

namespace MsgClasses {

QString CmdReagentGroupUpdate::NAME = "MsgClasses::CmdReagentGroupUpdate";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   ReagentID   Unique reagent id.
 */
/****************************************************************************/
CmdReagentGroupUpdate::CmdReagentGroupUpdate(int timeout,
                                             const QDataStream &dataStream)
    : CommandBase(timeout, dataStream)
{
}

CmdReagentGroupUpdate::CmdReagentGroupUpdate(void)
    : CommandBase()
{
}

CmdReagentGroupUpdate::~CmdReagentGroupUpdate(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdReagentGroupUpdate::GetName(void) const
{
    return NAME;
}

} // end namespace MsgClasses
