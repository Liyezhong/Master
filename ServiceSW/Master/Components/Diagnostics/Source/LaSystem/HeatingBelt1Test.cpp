/****************************************************************************/
/*! \file HeatingBelt1Test.cpp
 *
 *  \brief Implementation of L&A System heating belt 1 test.
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

#include "Diagnostics/Include/LaSystem/HeatingBelt1Test.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWizardText.h"

namespace Diagnostics {

namespace LaSystem {

CHeatingBelt1Test::CHeatingBelt1Test(void)
    : CTestBase()
{
}

CHeatingBelt1Test::~CHeatingBelt1Test(void)
{
}

void CHeatingBelt1Test::Run(void)
{
    qDebug() << "Heating belt 1 test starts!";

    this->FirstCheckBottle();
}

void CHeatingBelt1Test::FirstCheckBottle(void)
{
    qDebug() << "Heating belt 1 test: first connect bottle !";

    // inform the customer to connect the bottle
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Heating Belt 1 Test"));
    dlg->SetText(tr("Please confirm the waste bottle is connected!"));

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondHeating() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CHeatingBelt1Test::SecondHeating(void)
{
    qDebug() << "Heating belt 1 test: second heating!";

    /// \todo: start heating belt 1 test in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Heating Belt 1 Test"));
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

void CHeatingBelt1Test::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_BELT1_TEST_SUCCESS);
    qDebug() << "Heating belt 1 test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Heating Belt 1 Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Heating belt 1 test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingBelt1Test::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_BELT1_TEST_FAILURE);
    qDebug() << "Heating belt 1 test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Heating Belt 1 Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Heating belt 1 test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingBelt1Test::Cancel(void)
{
    qDebug() << "Heating belt 1 test canceled!";
}

} // namespace LaSystem

} // namespace Diagnostics
