/****************************************************************************/
/*! \file CmdStationUpdate.cpp
 *
 *  \brief CmdStationUpdate command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 14.03.2012
 *   $Author:  $ V.Raghunath
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QBuffer>
#include "HimalayaDataContainer/Containers/Stations/Commands/Include/CmdStationUpdate.h"

namespace MsgClasses {

QString CmdStationUpdate::NAME = "MsgClasses::CmdStationUpdate";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   Station     QByteArray containing station.
 */
/****************************************************************************/
CmdStationUpdate::CmdStationUpdate(int Timeout, const QDataStream &StationUpdateDataStream) :
    Command(Timeout)
{
    m_StationUpdateByteArray = static_cast< QBuffer *>(StationUpdateDataStream.device())->data();
}

/****************************************************************************/
/*!
 * \brief   Constructor for receiving
 */
/****************************************************************************/
CmdStationUpdate::CmdStationUpdate() : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdStationUpdate::~CmdStationUpdate()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdStationUpdate::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
