/****************************************************************************/
/*! \file MainsRelayTest.cpp
 *
 *  \brief Implementation of Mains relay test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
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

#include "Diagnostics/Include/System/MainsRelayTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgConfirmationText.h"

#include "Diagnostics/Include/System/DlgMainsRelay.h"

namespace Diagnostics {

namespace System {

CMainsRelayTest::CMainsRelayTest(void)
    : CTestBase()
{
}

CMainsRelayTest::~CMainsRelayTest(void)
{
}

void CMainsRelayTest::Run(void)
{
    qDebug() << "Mains Relay Test starts!";

    this->FirstOpenDialog();
}

void CMainsRelayTest::FirstOpenDialog(void)
{
    qDebug() << "Mains Relay Test: first open Mains Relay Test dialog!";

    // open the Mains Relay Test dialog
    CDlgMainsRelay *dlg = new CDlgMainsRelay;
    dlg->SetDialogTitle(tr("Mains Relay Test"));


    /// \todo: test AC current here **************************/
    /// \todo: update AC current through slot here **********/
    // CONNECTSIGNALSLOT(TEST,
    //                   AC_CURRENT_SIGNAL(const QString &),
    //                   dlg,
    //                   SetStatus(const QString &));
    dlg->SetStatus(tr("AC current: 5A"));

    CONNECTSIGNALSLOT(dlg, TestAcHeating(bool), this, TestAcHeating(bool));
    CONNECTSIGNALSLOT(dlg, TestRelay(bool), this, TestRelay(bool));
    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondConfirmResult() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CMainsRelayTest::TestAcHeating(bool On)
{
    qDebug() << "Mains Relay Test: to turn on AC heating? " << On;

    /// \todo: turn On/Off AC heating here *********************/
}

void CMainsRelayTest::TestRelay(bool On)
{
    qDebug() << "Mains Relay Test: to turn on relay? " << On;

    /// \todo: turn On/Off relay here **************************/
}

void CMainsRelayTest::SecondConfirmResult(void)
{
    qDebug() << "Mains Relay Test: second confirm result!";

    // ask the customer to confirm the test result
    MainMenu::CDlgConfirmationText *dlg = new MainMenu::CDlgConfirmationText;
    dlg->SetDialogTitle(tr("Mains Relay Test"));
    dlg->SetText(tr("Does the Mains relay test pass?"));

    CONNECTSIGNALSLOT(dlg, accepted(),this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CMainsRelayTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST_SUCCESS);
    qDebug() << "Mains Relay Test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Mains Relay Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Mains relay test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CMainsRelayTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST_FAILURE);
    qDebug() << "Mains Relay Test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Mains Relay Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Mains relay test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CMainsRelayTest::Cancel(void)
{
    qDebug() << "Mains relay test canceled!";
}

} // namespace System

} // namespace Diagnostics
