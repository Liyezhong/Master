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

namespace Diagnostics {

namespace InitialSystem {

CMainsRelayTest::CMainsRelayTest(QWidget *parent)
    : CTestBase(parent),
    mp_Parent(parent)
{
}

CMainsRelayTest::~CMainsRelayTest(void)
{
}

int CMainsRelayTest::Run(void)
{
    return RETURN_OK;
}

} // namespace InitialSystem

} // namespace Diagnostics
