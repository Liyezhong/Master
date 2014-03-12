/****************************************************************************/
/*! \file CmdLSensorDetectingTest.cpp
 *
 *  \brief Implementation file for class CmdLSensorDetectingTest.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-05-18
 *  $Author:    $ Brandon Shao
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai R&D Center.
 *
 *  (C) Copyright 2013 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <ServiceDeviceController/Include/Commands/CmdLSensorDetectingTest.h>

namespace DeviceCommandProcessor {

QString CmdLSensorDetectingTest::NAME             = "DeviceCommandProcessor::CmdLSensorDetectingTest";
int     CmdLSensorDetectingTest::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdLSensorDetectingTest::CmdLSensorDetectingTest(DeviceControl::DevInstanceID_t DevInstanceID, qint32 Position)
    : Global::Command(m_CmdTimeout)
    , m_DevInstanceID(DevInstanceID)
    , m_Position(Position)
{

}

/****************************************************************************/
CmdLSensorDetectingTest::~CmdLSensorDetectingTest() {
}

} // end namespace DeviceCommandProcessor

