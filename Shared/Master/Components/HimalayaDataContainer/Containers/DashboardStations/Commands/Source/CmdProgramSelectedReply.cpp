/****************************************************************************/
/*! \file CmdProgramSelectedReply.cpp
 *
 *  \brief CmdProgramSelectedReply command implementation.
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

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
namespace MsgClasses {

QString CmdProgramSelectedReply::NAME = "MsgClasses::CmdProgramSelectedReply";


CmdProgramSelectedReply::CmdProgramSelectedReply(int timeout, int timeProposed,
                                             int paraffinMeltCostedtime,
                                             int costedTimeBeforeParaffin,
                                             QList<QString>& stationList) :
    Command(timeout),
    m_TimeProposed(timeProposed),
    m_CostedTimeBeforeParaffin(costedTimeBeforeParaffin),
    m_ParaffinMeltCostedtime(paraffinMeltCostedtime),
    m_StationList(stationList)
{
}

CmdProgramSelectedReply::CmdProgramSelectedReply():
    Command(0),
    m_TimeProposed(0),
    m_CostedTimeBeforeParaffin(0),
    m_ParaffinMeltCostedtime(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdProgramSelectedReply::~CmdProgramSelectedReply()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdProgramSelectedReply::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
