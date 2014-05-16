/****************************************************************************/
/*! \file SelectingTest.cpp
 *
 *  \brief Implementation of Rotary valve selecting test.
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

#include "Diagnostics/Include/RotaryValve/SelectingTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWizardInputBottlePosition.h"
#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgConfirmationText.h"


namespace Diagnostics {

namespace RotaryValve {

CSelectingTest::CSelectingTest(void)
    : CTestBase()
{
}

CSelectingTest::~CSelectingTest(void)
{
}

void CSelectingTest::Run(void)
{
    qDebug() << "Rotary valve selecting test starts!";

    //this->FirstInitializing();
    this->SecondInputBottlePosition();
}

void CSelectingTest::FirstInitializing(void)
{
    qDebug() << "Rotary Valve selecting: first initializing!";

    /// \todo: start getting initial position in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Rotary Valve Selecting"));
    dlg->SetText(tr("Getting initial position..."));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondInputBottlePosition() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CSelectingTest::SecondInputBottlePosition(void)
{
    qDebug() << "Rotary Valve selecting: second input bottle position!";

    // inform the customer to input the bottle position
    MainMenu::CDlgWizardInputBottlePosition *dlg =
            new MainMenu::CDlgWizardInputBottlePosition;
    dlg->SetDialogTitle(tr("Rotary Valve Selecting"));
    dlg->SetText(tr("Please input the bottle position "
                    "you wish to select!"));
    dlg->SetButtonLabel(tr("Bottle position"));

    CONNECTSIGNALSLOT(dlg, AcceptPosition(qint32), this, ThirdCheckBottle(qint32));
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CSelectingTest::ThirdCheckBottle(qint32 Position)
{
    qDebug() << "Rotary Valve selecting: Third connect bottle!";

    // save position
    m_Position = Position;
    /// \todo: check position ***************************************/
    QString Station("reagent bottle");
    /// \todo: change Station to "wax bath" according to the position */

    // inform the customer to connect bottle at Position
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Rotary Valve Selecting"));
    dlg->SetText(tr("Please check the %1 in position %2 is full").
                 arg(Station).arg(Position));

    CONNECTSIGNALSLOT(dlg, accepted(), this, ForthGetPosition());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CSelectingTest::ForthGetPosition(void)
{
    qDebug() << "Rotary Valve selecting: forth getting position!";

#if 0
    /// \todo: start getting position %1 in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Rotary Valve Selecting"));
    dlg->SetText(tr("Getting position %1...").arg(m_Position));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, FifthCheckBubbles() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
#endif

    emit StartRVSelectTest(m_Position);
}

void CSelectingTest::FifthCheckBubbles(void)
{
    qDebug() << "Rotary Valve selecting: check bubbles!";

    // ask the customer to confirm if there's bubbles in reagent bottle
    MainMenu::CDlgConfirmationText *dlg = new MainMenu::CDlgConfirmationText;
    dlg->SetDialogTitle(tr("Rotary Valve Selecting"));
    dlg->SetText(tr("Can you see bubbles in reagent bottle %1?").arg(m_Position));

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CSelectingTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST_SUCCESS);
    qDebug() << "Rotary Valve selecting test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Rotary Valve Selecting"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Rotary Valve selecting test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CSelectingTest::Fail(void)
{
     Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST_FAILURE);
    qDebug() << "Rotary Valve selecting test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Rotary Valve Selecting"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Rotary valve selecting test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CSelectingTest::Cancel(void)
{
    qDebug() << "Rotary Valve selecting test canceled!";
}

} // namespace RotaryValve

} // namespace Diagnostics
