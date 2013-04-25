/****************************************************************************/
/*! \file CmdProgramEndTime.cpp
 *
 *  \brief CmdProgramEndTime command implementation.
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

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramEndTime.h"
namespace MsgClasses {

QString CmdProgramEndTime::NAME = "MsgClasses::CmdProgramEndTime";


CmdProgramEndTime::CmdProgramEndTime(int Timeout, int TimeProposed,
                                             int ParaffinWeltCostedtime,
                                             int CostedTimeBeforeParaffin) :
    Command(Timeout),
    m_TimeProposed(TimeProposed),
    m_ParaffinWeltCostedtime(ParaffinWeltCostedtime),
    m_CostedTimeBeforeParaffin(CostedTimeBeforeParaffin)
{
}

CmdProgramEndTime::CmdProgramEndTime()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdProgramEndTime::~CmdProgramEndTime()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdProgramEndTime::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
