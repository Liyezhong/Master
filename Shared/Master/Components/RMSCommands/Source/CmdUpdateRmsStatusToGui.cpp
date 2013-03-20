/****************************************************************************/
/*! \file CmdUpdateRmsStatusToGui.cpp
 *
 *  \brief CmdUpdateRmsStatusToGui command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 06.11.2012
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

#include "RMSCommands/Include/CmdUpdateRmsStatusToGui.h"

namespace MsgClasses {

QString CmdUpdateRmsStatusToGui::NAME = "MsgClasses::CmdUpdateRmsStatusToGui";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout     Timeout for command.
 * \param[in]   RmsStatus   Message Id.
 * \param[in]   BottleCount Bottle count.
 * \param[in]   LeicaReagentId Leica reagent id.
 */
/****************************************************************************/
CmdUpdateRmsStatusToGui::CmdUpdateRmsStatusToGui(int Timeout,const RmsStatus_t RmsStatus,qint32 BottleCount,QString LeicaReagentId) :
    Command(Timeout),   
    m_RmsStatus(RmsStatus),
    m_BottleCount(BottleCount),
    m_LeicaReagentId(LeicaReagentId)
{
}
CmdUpdateRmsStatusToGui::CmdUpdateRmsStatusToGui():Command(0)
{

}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdUpdateRmsStatusToGui::~CmdUpdateRmsStatusToGui()
{
}
/****************************************************************************/
/*!
 *  \brief   This function returns the scan message id.
 *
 *  \return  Message Id.
 */
/****************************************************************************/
RmsStatus_t CmdUpdateRmsStatusToGui::GetRmsStatusMsgId() const
{
    return m_RmsStatus;
}
/****************************************************************************/
/*!
 *  \brief   This function returns the Kit's total Bottle count.
 *
 *  \return  Bottle count.
 */
/****************************************************************************/
qint32 CmdUpdateRmsStatusToGui::GetRmsBottleCount() const
{
    return m_BottleCount;
}
/****************************************************************************/
/*!
 *  \brief   This function returns the Leica Reagent id of the Leica Bottle.
 *
 *  \return  Lecia Reagent Id.
 */
/****************************************************************************/
QString CmdUpdateRmsStatusToGui::GetRmsLeicaReagentId() const
{
    return m_LeicaReagentId;
}
/****************************************************************************/
/*!
 *  \brief   Get command name.
 *
 *  \return  Command name as string.
 */
/****************************************************************************/
QString CmdUpdateRmsStatusToGui::GetName() const
{
    return NAME;
}
} // end namespace MsgClasses


