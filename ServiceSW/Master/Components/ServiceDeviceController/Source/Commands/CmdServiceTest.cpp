/****************************************************************************/
/*! \file CmdServiceTest.cpp
 *
 *  \brief Implementation file for class CmdServiceTest.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-09-19
 *  $Author:    $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <ServiceDeviceController/Include/Commands/CmdServiceTest.h>

namespace DeviceCommandProcessor {

QString CmdServiceTest::NAME             = "DeviceCommandProcessor::CmdServiceTest";
int     CmdServiceTest::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdServiceTest::CmdServiceTest(QString ReqName, QStringList Params)
    : Global::Command(m_CmdTimeout)
    , m_ReqName(ReqName)
    , m_Params(Params)
{
}

/****************************************************************************/
CmdServiceTest::~CmdServiceTest() {
}

} // end namespace DeviceCommandProcessor

