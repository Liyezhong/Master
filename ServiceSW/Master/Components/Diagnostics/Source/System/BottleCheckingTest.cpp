/****************************************************************************/
/*! \file BottleCheckingTest.cpp
 *
 *  \brief Implementation of System bottle checking test.
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

#include "Diagnostics/Include/System/BottleCheckingTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWizardInputBottlePosition.h"
#include "ServiceWidget/Include/DlgWizardText.h"


namespace Diagnostics {

namespace System {

CBottleCheckingTest::CBottleCheckingTest(void)
    : CTestBase()
{
}

CBottleCheckingTest::~CBottleCheckingTest(void)
{
}

void CBottleCheckingTest::Run(void)
{
    qDebug() << "Bottle Checking Test starts!";

    this->FirstInputBottlePosition();
}

void CBottleCheckingTest::FirstInputBottlePosition(void)
{
    qDebug() << "Bottle Checking Test: first input bottle position!";

    // inform the customer to input the bottle position
    MainMenu::CDlgWizardInputBottlePosition *dlg =
            new MainMenu::CDlgWizardInputBottlePosition;
    dlg->SetDialogTitle(tr("Bottle Checking Test"));
    dlg->SetText(tr("Please input the bottle position "
                    "you wish to run the test!"));
    dlg->SetButtonLabel(tr("Bottle position"));

    CONNECTSIGNALSLOT(dlg, AcceptPosition(qint32), this, SecondCheckFull(qint32));
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CBottleCheckingTest::SecondCheckFull(int Position)
{
    qDebug() << "Bottle Checking Test: second check bottle is full!";

    // save position
    m_Position = Position;
    /// \todo: check position ***************************************/
    QString Station("reagent bottle");
    /// \todo: change Station to "wax bath" according to the position */

    // inform the customer to connect bottle at Position
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Bottle Checking Test"));
    dlg->SetText(tr("Please check the %1 in position %2 is filled"
                    " at least to the minimum level!").arg(Station).arg(Position));

    CONNECTSIGNALSLOT(dlg, accepted(), this, ThirdBottleCheck());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CBottleCheckingTest::ThirdBottleCheck(void)
{
    qDebug() << "Bottle Checking Test: start bottle checking!";

    /// \todo: start  bottle checking function in another thread **********/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Bottle Checking Test"));
    dlg->SetText(tr("Testing..."));
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CBottleCheckingTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_BOTTLE_CHECKING_TEST_SUCCESS);
    qDebug() << "Bottle Checking Test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Bottle Checking Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Bottle checking test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CBottleCheckingTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_BOTTLE_CHECKING_TEST_FAILURE);
    qDebug() << "Bottle Checking Test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Bottle Checking Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Bottle checking test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CBottleCheckingTest::Cancel(void)
{
    qDebug() << "Bottle Checking Test canceled!";
}

} // namespace System

} // namespace Diagnostics
