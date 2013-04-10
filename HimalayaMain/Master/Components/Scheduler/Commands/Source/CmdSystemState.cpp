/****************************************************************************/
/*! \file CmdSystemState.cpp
 *
 *  \brief Definition file for class CmdSystemState
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-1-5
 *  $Author:    $ Abe Yang
 *
 *  \b Description:
 *
 *       In Himalaya project, when some events happenning, we should consider 
 *       the current system state in different sates, we have 
 *       different processing.
 *       The command CmdSystemState will be sent from the device control layer 
 *       to HimalayaMasterController thread, mainly it carries the state type information.
 *       
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

#include <Scheduler/Commands/Include/CmdSystemState.h>

namespace NetCommands {

QString CmdSystemState::NAME = "NetCommands::CmdSystemState";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   stateId  state ID.
 */
/****************************************************************************/
CmdSystemState::CmdSystemState(quint32 stateId) :
    Command(5000),
    m_StateId(stateId)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdSystemState::~CmdSystemState()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdSystemState::GetName() const
{
    return NAME;
}

} // end namespace NetCommands
