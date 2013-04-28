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


CmdProgramSelectedReply::CmdProgramSelectedReply(int Timeout, int TimeProposed,
                                             int ParaffinWeltCostedtime,
                                             int CostedTimeBeforeParaffin,
                                             QList<QString>& StationList) :
    Command(Timeout),
    m_TimeProposed(TimeProposed),
    m_ParaffinWeltCostedtime(ParaffinWeltCostedtime),
    m_CostedTimeBeforeParaffin(CostedTimeBeforeParaffin),
    m_StationList(StationList)
{
}

CmdProgramSelectedReply::CmdProgramSelectedReply()
    : Command(0)
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
