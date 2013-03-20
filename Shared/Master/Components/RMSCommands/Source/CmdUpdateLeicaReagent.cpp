/****************************************************************************/
/*! \file CmdUpdateLeicaReagent.cpp
 *
 *  \brief CmdUpdateLeicaReagent command implementation.
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

#include "RMSCommands/Include/CmdUpdateLeicaReagent.h"

namespace MsgClasses {

QString CmdUpdateLeicaReagent::NAME = "MsgClasses::CmdUpdateLeicaReagent";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   LeicaConsumables      Bottle or kit.
 */
/****************************************************************************/
CmdUpdateLeicaReagent::CmdUpdateLeicaReagent(int Timeout, const QString &LeicaConsumables) :
    Command(Timeout),
    m_LeicaConsumables(LeicaConsumables)
{
}
CmdUpdateLeicaReagent::CmdUpdateLeicaReagent() : Command(0)
{
}
/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdUpdateLeicaReagent::~CmdUpdateLeicaReagent()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name.
 *
 *  \return  Command name as string.
 */
/****************************************************************************/
QString CmdUpdateLeicaReagent::GetName() const
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
QString CmdUpdateLeicaReagent::GetLeicaConsumablesStatus() const
{
    return m_LeicaConsumables;
}

} // end namespace MsgClasses
