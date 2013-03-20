/****************************************************************************/
/*! \file CmdCancelProgram.cpp
 *
 *  \brief CmdCancelProgram command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17.03.2010
 *   $Author:  $ Y.Novak
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

#include "HimalayaDataContainer/Containers/Programs/Commands/Include/CmdCancelProgram.h"

namespace MsgClasses {

QString CmdCancelProgram::NAME = "MsgClasses::CmdCancelProgram";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   Action      Type of action: how program has to be cancelled.
 */
/****************************************************************************/
CmdCancelProgram::CmdCancelProgram(int Timeout, const QString &Action) :
    Command(Timeout),
     m_Action(Action)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdCancelProgram::~CmdCancelProgram()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdCancelProgram::GetName() const
{
    return NAME;
}

/****************************************************************************/
/*!
 *  \brief   This function returns type of action --> how program shall
 *           be cancelled: immediately or as scheduler finds a free time slot.
 *
 *  \return  action
 */
/****************************************************************************/
QString CmdCancelProgram::GetAction() const
{
    return m_Action;
}

} // end namespace MsgClasses
