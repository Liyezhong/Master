/****************************************************************************/
/*! \file CmdUpdateStationReagentStatus.cpp
 *
 *  \brief when the running Program finishes with 1 Reagent, the Reagent status should be updated.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-20
 *  $Author:    $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2013 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdUpdateStationReagentStatus.h"

#include <QBuffer>

namespace MsgClasses {

QString CmdUpdateStationReagentStatus::NAME = "MsgClasses::CmdUpdateStationReagentStatus";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   ReagentID   Unique Station id.
 * \param[in]   CassetteCount   if RMS os Cassete, we should give CassetteCount
 */
/****************************************************************************/
CmdUpdateStationReagentStatus::CmdUpdateStationReagentStatus(int TimeOut, const QStringList& StationIDs, int CassetteCount)
    : Command(TimeOut)
    , m_CassetteCount(CassetteCount)
    , m_StationIDs(StationIDs)
{
}

CmdUpdateStationReagentStatus::CmdUpdateStationReagentStatus(void)
    : Command(0)
{
}

CmdUpdateStationReagentStatus::~CmdUpdateStationReagentStatus(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdUpdateStationReagentStatus::GetName(void) const
{
    return NAME;
}

} // end namespace MsgClasses
