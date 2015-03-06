/****************************************************************************/
/*! \file CmdReturnMessage.cpp
 *
 *  \brief Implementation file for class CmdReturnMessage.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-04
 *  $Author:    $ Srivathsa HH
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

#include <ServiceDeviceController/Include/Commands/CmdReturnMessage.h>

namespace DeviceCommandProcessor {

QString CmdReturnMessage::NAME             = "DeviceCommandProcessor::CmdReturnMessage";
int     CmdReturnMessage::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdReturnMessage::CmdReturnMessage(QString ReturnMessage)
    : Global::Command(m_CmdTimeout)
    , m_ReturnMessage(ReturnMessage)
    , m_DeviceId(0)
    , m_MessageType(Service::GUIMSGTYPE_UNDEFINED)
    , m_AirFlowStatus(false)
    , m_HoodStatus(false)
    , m_CalibStatus(false)
    , m_ModuleTestResult(false)
    , m_ServErrorCode(0)
{
}

/****************************************************************************/
CmdReturnMessage::~CmdReturnMessage() {
}

} // end namespace DeviceCommandProcessor

