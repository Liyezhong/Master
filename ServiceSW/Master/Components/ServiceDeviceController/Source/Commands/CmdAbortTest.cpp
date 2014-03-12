/****************************************************************************/
/*! \file CmdAbortTest.cpp
 *
 *  \brief Implementation file for class CmdAbortTest.
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

#include <ServiceDeviceController/Include/Commands/CmdAbortTest.h>

namespace DeviceCommandProcessor {

QString CmdAbortTest::NAME             = "DeviceCommandProcessor::CmdAbortTest";
int     CmdAbortTest::m_CmdTimeout     = 3000;


/****************************************************************************/
CmdAbortTest::CmdAbortTest(DeviceControl::DevInstanceID_t DevInstanceID)
    : Global::Command(m_CmdTimeout)
    , m_DevInstanceID(DevInstanceID)
{
}
/****************************************************************************/
CmdAbortTest::~CmdAbortTest() {
}

} // end namespace DeviceCommandProcessor

