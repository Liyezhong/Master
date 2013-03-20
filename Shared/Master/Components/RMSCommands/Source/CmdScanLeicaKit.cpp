/****************************************************************************/
/*! \file CmdScanLeicaKit.cpp
 *
 *  \brief CmdScanLeicaKit command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.10.2012
 *   $Author:  $ Ningu
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

#include "RMSCommands/Include/CmdScanLeicaKit.h"

namespace MsgClasses {

QString CmdScanLeicaKit::NAME = "MsgClasses::CmdScanLeicaKit";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout Timeout for command.
 * \param[in]   IsKitScanViaBlg   Yes or no.
 * \param[in]   NeedAutoHide Yes or no
 */
/****************************************************************************/
CmdScanLeicaKit::CmdScanLeicaKit(int Timeout,const bool IsKitScanViaBlg,const QString &NeedAutoHide) :
    Command(Timeout),
    m_AutoHide(NeedAutoHide),
    m_IsKitScanViaBlg(IsKitScanViaBlg)
{
}
CmdScanLeicaKit::CmdScanLeicaKit():Command(0)
{

}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdScanLeicaKit::~CmdScanLeicaKit()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name.
 *
 *  \return  Command name as string.
 */
/****************************************************************************/
QString CmdScanLeicaKit::GetName() const
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
QString CmdScanLeicaKit::GetAutoHideStatus() const
{
    return m_AutoHide;
}
/****************************************************************************/
/*!
 *  \brief   This function returns the UpdateReagent fa;.
 *
 *  \return  Bottle count.
 */
/****************************************************************************/
bool CmdScanLeicaKit::GetKitScanFlag() const
{
    return m_IsKitScanViaBlg;
}
} // end namespace MsgClasses

