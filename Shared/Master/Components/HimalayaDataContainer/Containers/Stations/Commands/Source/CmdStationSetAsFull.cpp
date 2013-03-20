/****************************************************************************/
/*! \file CmdStationSetAsFull.cpp
 *
 *  \brief Set the station as full command implementation.
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

#include "HimalayaDataContainer/Containers/Stations/Commands/Include/CmdStationSetAsFull.h"

#include <QBuffer>

namespace MsgClasses {

QString CmdStationSetAsFull::NAME = "MsgClasses::CmdStationSetAsFull";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   ReagentID   Unique reagent id.
 */
/****************************************************************************/
CmdStationSetAsFull::CmdStationSetAsFull(int timeout,
                                             const QDataStream &dataStream)
    : CommandBase(timeout, dataStream)
{
}

CmdStationSetAsFull::CmdStationSetAsFull(void)
    : CommandBase()
{
}

CmdStationSetAsFull::~CmdStationSetAsFull(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdStationSetAsFull::GetName(void) const
{
    return NAME;
}

} // end namespace MsgClasses
