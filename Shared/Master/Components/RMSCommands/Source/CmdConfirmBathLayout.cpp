/****************************************************************************/
/*! \file CmdConfirmBathLayout.cpp
 *
 *  \brief CmdConfirmBathLayout command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17.03.2010, 21.08.2012
 *   $Author:  $ Y.Novak, Vikram MK
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

#include "RMSCommands/Include/CmdConfirmBathLayout.h"

namespace MsgClasses {

QString CmdConfirmBathLayout::NAME = "MsgClasses::CmdConfirmBathLayout";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   Status      Confirmation status.
 */
/****************************************************************************/
CmdConfirmBathLayout::CmdConfirmBathLayout(int Timeout, const QString &Status) :
    Command(Timeout),
    m_Status(Status)
{
}
CmdConfirmBathLayout::CmdConfirmBathLayout() : Command(0)
{
}
/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdConfirmBathLayout::~CmdConfirmBathLayout()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name.
 *
 *  \return  Command name as string.
 */
/****************************************************************************/
QString CmdConfirmBathLayout::GetName() const
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
QString CmdConfirmBathLayout::GetConfirmationStatus() const
{
    return m_Status;
}

} // end namespace MsgClasses
