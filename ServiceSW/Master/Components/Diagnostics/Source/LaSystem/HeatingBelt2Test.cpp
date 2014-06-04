/****************************************************************************/
/*! \file HeatingBelt2Test.cpp
 *
 *  \brief Implementation of L&A System heating belt 2 test.
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

#include "Diagnostics/Include/LaSystem/HeatingBelt2Test.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWaitingBase.h"

namespace Diagnostics {

namespace LaSystem {

CHeatingBelt2Test::CHeatingBelt2Test(void)
    : CTestBase()
{
}

CHeatingBelt2Test::~CHeatingBelt2Test(void)
{
}

void CHeatingBelt2Test::Run(void)
{
    qDebug() << "Heating belt 2 test starts!";

    this->StartHeating();
}

void CHeatingBelt2Test::StartHeating(void)
{
    qDebug() << "Heating belt 2 test: start heating !";

    /// \todo: start heating belt 2 test in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Heating Belt 2 Test"));
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

void CHeatingBelt2Test::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST_SUCCESS);
    qDebug() << "Heating belt 2 test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Heating Belt 2 Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Heating belt 2 test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingBelt2Test::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST_FAILURE);
    qDebug() << "Heating belt 2 test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Heating Belt 2 Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Heating belt 2 test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

} // namespace LaSystem

} // namespace Diagnostics
