/****************************************************************************/
/*! \file CmdStationParaffinBathStatus.cpp
 *
 *  \brief CmdStationParaffinBathStatus command implementation.
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

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationParaffinBathStatus.h"
namespace MsgClasses {

QString CmdStationParaffinBathStatus::NAME = "MsgClasses::CmdStationParaffinBathStatus";


CmdStationParaffinBathStatus::CmdStationParaffinBathStatus(int Timeout, const QString& StationParaffinBathID,
                                    DataManager::ContainerStatusType_t StationParaffinBathStatusType) :
    Command(Timeout),
    m_StationParaffinBathID(StationParaffinBathID),
    m_StationParaffinBathStatusType(StationParaffinBathStatusType)
{
}

CmdStationParaffinBathStatus::CmdStationParaffinBathStatus()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdStationParaffinBathStatus::~CmdStationParaffinBathStatus()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdStationParaffinBathStatus::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
