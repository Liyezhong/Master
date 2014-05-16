/****************************************************************************/
/*! \file AlarmTest.cpp
 *
 *  \brief Implementation of System alarm test.
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

#include "Diagnostics/Include/System/AlarmTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgConfirmationText.h"

#include "Diagnostics/Include/System/DlgAlarm.h"

namespace Diagnostics {

namespace System {

CAlarmTest::CAlarmTest(void)
    : CTestBase()
{
}

CAlarmTest::~CAlarmTest(void)
{
}

void CAlarmTest::Run(void)
{
    qDebug() << "Alarm Test starts!";

    this->FirstOpenDialog();
}

void CAlarmTest::FirstOpenDialog(void)
{
    qDebug() << "Alarm Test: first open alarm test dialog!";

    // open the Alarm Test dialog
    CDlgAlarm *dlg = new CDlgAlarm;
    dlg->SetDialogTitle(tr("Alarm Test"));


    /// \todo: test Socket connection status here **************************/
    /// \todo: update soceket connection status through slot here **********/
    // CONNECTSIGNALSLOT(TEST,
    //                   SOCKET_STATUS_SIGNAL(const QString &),
    //                   dlg,
    //                   SetStatus(const QString &));
    dlg->SetStatus(tr("Socket connection status: disconnected"));

    CONNECTSIGNALSLOT(dlg, TestAlarm(bool), this, TestAlarm(bool));
    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondConfirmResult() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CAlarmTest::TestAlarm(bool On)
{
    qDebug() << "Alarm Test: to turn on alarm? " << On;

    /// \todo: turn On/Off alarm here **************************/
}

void CAlarmTest::SecondConfirmResult(void)
{
    qDebug() << "Alarm Test: second confirm result!";

    // ask the customer to confirm the test result
    MainMenu::CDlgConfirmationText *dlg = new MainMenu::CDlgConfirmationText;
    dlg->SetDialogTitle(tr("Alarm Test"));
    dlg->SetText(tr("Does the alarm test pass?"));

    CONNECTSIGNALSLOT(dlg, accepted(),this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CAlarmTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_SUCCESS);
    qDebug() << "Alarm Test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Alarm Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Alarm test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CAlarmTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_FAILURE);
    qDebug() << "Alarm Test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Alarm Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Alarm test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CAlarmTest::Cancel(void)
{
    qDebug() << "Alarm Test canceled!";
}

} // namespace System

} // namespace Diagnostics
