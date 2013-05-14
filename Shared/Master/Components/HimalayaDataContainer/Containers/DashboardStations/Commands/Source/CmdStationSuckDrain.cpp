/****************************************************************************/
/*! \file CmdStationSuckDrain.cpp
 *
 *  \brief CmdStationSuckDrain command implementation.
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

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"
namespace MsgClasses {

QString CmdStationSuckDrain::NAME = "MsgClasses::CmdStationSuckDrain";


CmdStationSuckDrain::CmdStationSuckDrain(int Timeout, const QString& StationID, bool IsStart, bool IsSuck) :
    Command(Timeout),
    m_StationID(StationID),
    m_IsStart(IsStart),
    m_IsSuck(IsSuck)
{
}

CmdStationSuckDrain::CmdStationSuckDrain()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdStationSuckDrain::~CmdStationSuckDrain()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdStationSuckDrain::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
