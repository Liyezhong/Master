/****************************************************************************/
/*! \file CmdProgramEndDateTime.cpp
 *
 *  \brief CmdProgramEndDateTime command implementation.
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

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramEndDateTime.h"
namespace MsgClasses {

QString CmdProgramEndDateTime::NAME = "MsgClasses::CmdProgramEndDateTime";


CmdProgramEndDateTime::CmdProgramEndDateTime(int Timeout, const QDateTime& ProgramEndDateTime, const QTime& ParaffinWeltCostedtime) :
    Command(Timeout),
    m_EndDateTime(ProgramEndDateTime),
    m_ParaffinWeltCostedtime(ParaffinWeltCostedtime)
{
}

CmdProgramEndDateTime::CmdProgramEndDateTime()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdProgramEndDateTime::~CmdProgramEndDateTime()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdProgramEndDateTime::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
