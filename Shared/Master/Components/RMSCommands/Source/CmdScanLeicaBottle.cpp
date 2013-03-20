/****************************************************************************/
/*! \file CmdScanLeicaBottle.cpp
 *
 *  \brief CmdScanLeicaBottle command implementation.
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

#include "RMSCommands/Include/CmdScanLeicaBottle.h"

namespace MsgClasses {

QString CmdScanLeicaBottle::NAME = "MsgClasses::CmdScanLeicaBottle";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   NeedAutoHide      Yes or no.
 */
/****************************************************************************/
CmdScanLeicaBottle::CmdScanLeicaBottle(int Timeout, const QString &NeedAutoHide) :
    Command(Timeout),
    m_AutoHide(NeedAutoHide)
{
}
CmdScanLeicaBottle::CmdScanLeicaBottle() : Command(0)
{
}
/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdScanLeicaBottle::~CmdScanLeicaBottle()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name.
 *
 *  \return  Command name as string.
 */
/****************************************************************************/
QString CmdScanLeicaBottle::GetName() const
{
    return NAME;
}

/****************************************************************************/
/*!
 *  \brief   This function returns the auto hide required
 *
 *  \return  Confirmation status.
 */
/****************************************************************************/
QString CmdScanLeicaBottle::GetAutoHideStatus() const
{
    return m_AutoHide;
}

} // end namespace MsgClasses
