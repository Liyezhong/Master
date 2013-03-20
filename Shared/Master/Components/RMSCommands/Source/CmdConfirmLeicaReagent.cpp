/****************************************************************************/
/*! \file CmdConfirmLeicaReagent.cpp
 *
 *  \brief CmdConfirmLeicaReagent command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 21.08.2012
 *   $Author:  $ Vikram MK
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

#include "RMSCommands/Include/CmdConfirmLeicaReagent.h"

namespace MsgClasses {

QString CmdConfirmLeicaReagent::NAME = "MsgClasses::CmdConfirmLeicaReagent";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   Status      Confirmation status.
 */
/****************************************************************************/
CmdConfirmLeicaReagent::CmdConfirmLeicaReagent(int Timeout, const QString &Status) :
    Command(Timeout),
    m_Status(Status)
{
}
CmdConfirmLeicaReagent::CmdConfirmLeicaReagent() : Command(0)
{
}
/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdConfirmLeicaReagent::~CmdConfirmLeicaReagent()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name.
 *
 *  \return  Command name as string.
 */
/****************************************************************************/
QString CmdConfirmLeicaReagent::GetName() const
{
    return NAME;
}

/****************************************************************************/
/*!
 *  \brief   This function returns the confirmation status
 *
 *  \return  Confirmation status.
 */
/****************************************************************************/
QString CmdConfirmLeicaReagent::GetConfirmationStatus() const
{
    return m_Status;
}

} // end namespace MsgClasses
