/****************************************************************************/
/*! \file CmdModuleManufacturingTest.cpp
 *
 *  \brief Implementation file for class CmdModuleManufacturingTest.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-19
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

#include <ServiceDeviceController/Include/Commands/CmdModuleManufacturingTest.h>

namespace DeviceCommandProcessor {

QString CmdModuleManufacturingTest::NAME             = "DeviceCommandProcessor::CmdModuleManufacturingTest";
int     CmdModuleManufacturingTest::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdModuleManufacturingTest::CmdModuleManufacturingTest(Service::ModuleTestCaseID CmdType, Service::ModuleTestCaseID AbortId)
    : Global::Command(m_CmdTimeout)
    , m_CommandType(CmdType)
    , m_AbortTestCaseId(AbortId)
{
}

/****************************************************************************/
CmdModuleManufacturingTest::~CmdModuleManufacturingTest() {
}

} // end namespace DeviceCommandProcessor

