/****************************************************************************/
/*! \file CmdFillLeicaReagent.cpp
 *
 *  \brief CmdFillLeicaReagent command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 21.08.2012
 *   $Author:  $ Vikram MK
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

#include "RMSCommands/Include/CmdFillLeicaReagent.h"

namespace MsgClasses {

QString CmdFillLeicaReagent::NAME = "MsgClasses::CmdFillLeicaReagent";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   StationId   Station ID.
 */
/****************************************************************************/
CmdFillLeicaReagent::CmdFillLeicaReagent(int Timeout, const QString &StationId) :
    Command(Timeout),
    m_StationId(StationId)
{
}
CmdFillLeicaReagent::CmdFillLeicaReagent() : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdFillLeicaReagent::~CmdFillLeicaReagent()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdFillLeicaReagent::GetName() const
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
QString CmdFillLeicaReagent::GetStationId() const
{
    return m_StationId;
}
} // end namespace MsgClasses
