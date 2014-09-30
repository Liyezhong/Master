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


namespace Diagnostics {

namespace System {

CAlarmTest::CAlarmTest(void)
    : CTestBase()
{
}

CAlarmTest::~CAlarmTest(void)
{
}

int CAlarmTest::Run(void)
{
    qDebug() << "Alarm Test starts!";

    this->FirstOpenDialog();
}

void CAlarmTest::FirstOpenDialog(void)
{
    qDebug() << "Alarm Test: first open alarm test dialog!";

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
}

void CAlarmTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_SUCCESS);
    qDebug() << "Alarm Test succeeded!";

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
