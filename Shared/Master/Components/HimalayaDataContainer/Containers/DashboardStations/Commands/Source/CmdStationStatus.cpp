/****************************************************************************/
/*! \file CmdStationStatus.cpp
 *
 *  \brief CmdStationStatus command implementation.
 *
 *   $Version: $ 0.1
*   $Date:    $ 27.03.2013
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
  *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationStatus.h"
namespace MsgClasses {

QString CmdStationStatus::NAME = "MsgClasses::CmdStationStatus";


CmdStationStatus::CmdStationStatus(int Timeout, const QString& StationID,
                                    DataManager::StationStatusType_t StationStatusType) :
    Command(Timeout),
    m_StationID(StationID),
    m_StationStatusType(StationStatusType)
{
}

CmdStationStatus::CmdStationStatus()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdStationStatus::~CmdStationStatus()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdStationStatus::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
