/****************************************************************************/
/*! \file AirHeatingTubeTest.cpp
 *
 *  \brief Implementation of L&A lir heating tube test.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-20
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Diagnostics/Include/LaSystem/AirHeatingTubeTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWizardText.h"

namespace Diagnostics {

namespace LaSystem {

CAirHeatingTubeTest::CAirHeatingTubeTest(void)
    : CTestBase()
{
}

CAirHeatingTubeTest::~CAirHeatingTubeTest(void)
{
}

int CAirHeatingTubeTest::Run(void)
{
    qDebug() << "Air System test starts!";

    this->FirstBuildPressure();
}

void CAirHeatingTubeTest::FirstBuildPressure(void)
{
    qDebug() << "Air System test: first build pressure !";

    /// \todo: start building pressure in another thread **********************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Air System Test"));
    dlg->SetText(tr("Building Pressure..."));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondReleasePressure() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CAirHeatingTubeTest::SecondReleasePressure(void)
{
    qDebug() << "Air System test: second release pressure!";

    /// \todo: start releasing pressure in another thread *********************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Air System Test"));
    dlg->SetText(tr("Releasing Pressure..."));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, ThirdBuildVaccuum() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CAirHeatingTubeTest::ThirdBuildVaccuum(void)
{
    qDebug() << "Air System test: third build vaccuum!";

    /// \todo: start building vaccuum in another thread ***********************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Air System Test"));
    dlg->SetText(tr("Building Vaccuum..."));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, ForthReleaseVaccuum() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CAirHeatingTubeTest::ForthReleaseVaccuum(void)
{
    qDebug() << "Air System test: second release vaccuum!";

    /// \todo: start releasing vaccuum in another thread *********************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Air System Test"));
    dlg->SetText(tr("Releasing Vaccuum..."));
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

void CAirHeatingTubeTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_AIR_SYSTEM_TEST_SUCCESS);
    qDebug() << "Air System test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Air System Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Air tystem test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CAirHeatingTubeTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_AIR_SYSTEM_TEST_FAILURE);
    qDebug() << "Air System test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Air System Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Air system test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

} // namespace LaSystem

} // namespace Diagnostics
