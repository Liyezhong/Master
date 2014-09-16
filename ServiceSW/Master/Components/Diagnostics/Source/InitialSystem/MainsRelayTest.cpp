/****************************************************************************/
/*! \file MainsRelayTest.cpp
 *
 *  \brief Implementation of Mains relay test..
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-15
 *   $Author:  $ Sunny.Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Diagnostics/Include/InitialSystem/MainsRelayTest.h"
#include "MainMenu/Include/MessageDlg.h"

#include <QDebug>
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"

namespace Diagnostics {

namespace InitialSystem {

CMainsRelayTest::CMainsRelayTest(QWidget *parent)
    : CTestBase(),
    mp_Parent(parent)
{
}

CMainsRelayTest::~CMainsRelayTest(void)
{
}

int CMainsRelayTest::Run(void)
{

}

} // namespace InitialSystem

} // namespace Diagnostics
