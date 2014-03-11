/****************************************************************************/
/*! \file CmdGetDataContainers.cpp
 *
 *  \brief Implementation file for class CmdGetDataContainers.
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

#include <ServiceDeviceController/Include/Commands/CmdGetDataContainers.h>

namespace DeviceCommandProcessor {

QString CmdGetDataContainers::NAME             = "DeviceCommandProcessor::CmdGetDataContainers";
int     CmdGetDataContainers::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdGetDataContainers::CmdGetDataContainers()
    : Global::Command(m_CmdTimeout)
    , m_ReturnMessage("ReturnMessage")
    , m_DeviceId(0)
{
}

/****************************************************************************/
CmdGetDataContainers::~CmdGetDataContainers() {
}

} // end namespace DeviceCommandProcessor

