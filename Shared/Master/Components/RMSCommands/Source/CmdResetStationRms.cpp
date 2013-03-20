/****************************************************************************/
/*! \file CmdResetStationRms.cpp
 *
 *  \brief CmdResetStationRms command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 18.03.2010, 21.08.2012
 *   $Author:  $ Y.Novak, Vikram MK
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

#include "RMSCommands/Include/CmdResetStationRms.h"

namespace MsgClasses {

QString CmdResetStationRms::NAME = "MsgClasses::CmdResetStationRms";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   StationId   Station ID.
 */
/****************************************************************************/
CmdResetStationRms::CmdResetStationRms(int Timeout, const QString &StationId) :
    Command(Timeout),
    m_StationId(StationId)
{
}
CmdResetStationRms::CmdResetStationRms() : Command(0)
{
}
/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdResetStationRms::~CmdResetStationRms()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdResetStationRms::GetName() const
{
    return NAME;
}

/****************************************************************************/
/*!
 *  \brief   This function returns station ID to reset RMS for
 *
 *  \return  station id
 */
/****************************************************************************/
QString CmdResetStationRms::GetStationId() const
{
    return m_StationId;
}
} // end namespace MsgClasses
