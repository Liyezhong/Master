/****************************************************************************/
/*! \file CmdCalibrateDevice.cpp
 *
 *  \brief Implementation file for class CmdCalibrateDevice.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-189
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

#include <ServiceDeviceController/Include/Commands/CmdCalibrateDevice.h>

namespace DeviceCommandProcessor {

QString CmdCalibrateDevice::NAME             = "DeviceCommandProcessor::CmdCalibrateDevice";
int     CmdCalibrateDevice::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdCalibrateDevice::CmdCalibrateDevice(Service::DeviceCalibrationCmdType DeviceCommand)
    : Global::Command(m_CmdTimeout)
    , m_CommandType(DeviceCommand)
    , m_RackId(0)
    , m_RackUserData(0)
{
}

/****************************************************************************/
CmdCalibrateDevice::~CmdCalibrateDevice() {
}

} // end namespace DeviceCommandProcessor

