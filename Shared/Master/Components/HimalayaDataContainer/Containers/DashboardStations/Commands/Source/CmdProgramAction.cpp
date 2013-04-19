/****************************************************************************/
/*! \file CmdProgramAction.cpp
 *
 *  \brief CmdProgramAction command implementation.
 *
 *   $Version: $ 0.1
*   $Date:    $ 22.03.2013
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

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
namespace MsgClasses {

QString CmdProgramAction::NAME = "MsgClasses::CmdProgramAction";


CmdProgramAction::CmdProgramAction(int Timeout, const QString& ProgramID, DataManager::ProgramActionType_t ActionType,
                                   const QDateTime ProgramEndDateTime) :
    Command(Timeout),
    m_ProgramID(ProgramID),
    m_ActionType(ActionType),
    m_ProgramEndDateTime(ProgramEndDateTime)
{
}

CmdProgramAction::CmdProgramAction()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdProgramAction::~CmdProgramAction()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdProgramAction::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
