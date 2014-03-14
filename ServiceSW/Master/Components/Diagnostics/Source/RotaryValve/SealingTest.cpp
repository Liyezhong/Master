/****************************************************************************/
/*! \file SealingTest.cpp
 *
 *  \brief Implementation of Rotary valve sealing test.
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

#include "Diagnostics/Include/RotaryValve/SealingTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWizardInputBottlePosition.h"
#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgConfirmationText.h"


namespace Diagnostics {

namespace RotaryValve {

CSealingTest::CSealingTest(void)
    : CTestBase()
{
}

CSealingTest::~CSealingTest(void)
{
}

void CSealingTest::Run(void)
{
    qDebug() << "Rotary valve sealing test starts!";

    //this->FirstInitializing();

    this->SecondInputBottlePosition();
}

void CSealingTest::FirstInitializing(void)
{
    qDebug() << "Rotary Valve sealing: first initializing!";

    /// \todo: start getting initial position in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Rotary Valve Sealing"));
    dlg->SetText(tr("Getting initial position..."));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondInputBottlePosition() );
    //CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, SecondInputBottlePosition() );

    dlg->exec();

    delete dlg;
}

void CSealingTest::SecondInputBottlePosition(void)
{
    qDebug() << "Rotary Valve sealing: second input bottle position!";

    // inform the customer to input the bottle position
    MainMenu::CDlgWizardInputBottlePosition *dlg =
            new MainMenu::CDlgWizardInputBottlePosition;
    dlg->SetDialogTitle(tr("Rotary Valve Sealing"));
    dlg->SetText(tr("Please input the bottle position "
                    "you wish to select!"));
    dlg->SetButtonLabel(tr("Bottle position"));

    CONNECTSIGNALSLOT(dlg, AcceptPosition(qint32), this, ThirdGetPosition(qint32));
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CSealingTest::ThirdGetPosition(qint32 Position)
{
    qDebug() << "Rotary Valve sealing: Third getting position!";

    // save position
    m_Position = Position;

#if 0
    /// \todo: check position ***************************************/
    /// \todo: start getting position %1 in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Rotary Valve Sealing"));
    dlg->SetText(tr("Getting position %1...").arg(m_Position));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, ForthSealing() );
    //CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, ForthSealing() );

    dlg->exec();

    delete dlg;
#endif

    emit  StartRVSealTest(m_Position);
}

void CSealingTest::ForthSealing(void)
{
    qDebug() << "Rotary Valve sealing: forth sealing!";

    /// \todo: start sealing function in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Rotary Valve Sealing"));
    dlg->SetText(tr("Testing..."));
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
}

void CSealingTest::Succeed(void)
{
    qDebug() << "Rotary Valve sealing test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Rotary Valve Sealing"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Rotary Valve sealing test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CSealingTest::Fail(void)
{
    qDebug() << "Rotary Valve sealing test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Rotary Valve Sealing"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Rotary valve sealing test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CSealingTest::Cancel(void)
{
    qDebug() << "Rotary Valve selecting test canceled!";
}

} // namespace RotaryValve

} // namespace Diagnostics
