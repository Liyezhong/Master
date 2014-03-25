/****************************************************************************/
/*! \file CmdRotaryValveTest.cpp
 *
 *  \brief Implementation file for class CmdRotaryValveTest.
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

#include <ServiceDeviceController/Include/Commands/CmdRotaryValveTest.h>

namespace DeviceCommandProcessor {

QString CmdRotaryValveTest::NAME             = "DeviceCommandProcessor::CmdRotaryValveTest";
int     CmdRotaryValveTest::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdRotaryValveTest::CmdRotaryValveTest(quint32 DevInstanceID, qint32 Position,
               quint8 CmdType)
    : Global::Command(m_CmdTimeout)
    , m_DevInstanceID(DevInstanceID)
    , m_Position(Position)
    , m_CmdType(CmdType)
{

}

/****************************************************************************/
CmdRotaryValveTest::~CmdRotaryValveTest() {
}

} // end namespace DeviceCommandProcessor

