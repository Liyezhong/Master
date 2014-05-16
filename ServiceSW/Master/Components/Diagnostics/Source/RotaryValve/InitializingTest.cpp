/****************************************************************************/
/*! \file InitializingTest.cpp
 *
 *  \brief Implementation of Rotary valve initializing test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
 *   $Author:  $ R.Wu
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

#include "Diagnostics/Include/RotaryValve/InitializingTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"


namespace Diagnostics {

namespace RotaryValve {

CInitializingTest::CInitializingTest(void)
    : CTestBase()
{
}

CInitializingTest::~CInitializingTest(void)
{
}

void CInitializingTest::Run(void)
{
    qDebug() << "Rotary Valve Initializing test starts!";

    this->StartInitializing();
}

void CInitializingTest::StartInitializing(void)
{
    qDebug() << "Rotary Valve Initializing: start initializing!";

#if 0
    /// \todo: start getting initial position another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Rotary Valve Initializing"));
    dlg->SetText(tr("Getting initial position..."));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
#endif

    emit StartRVInitTest();
}

void CInitializingTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST_SUCCESS);
    qDebug() << "Rotary Valve Initializing succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Rotary Valve Initializing"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Succeed to get the initial position!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CInitializingTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST_FAILURE);
    qDebug() << "Rotary Valve Initializing failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Rotary Valve Initializing"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Fail to get the initial positioin!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

} // namespace RotaryValve

} // namespace Diagnostics
