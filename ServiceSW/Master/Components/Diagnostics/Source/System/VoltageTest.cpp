/****************************************************************************/
/*! \file VoltageTest.cpp
 *
 *  \brief Implementation of System 110V/220V switching test.
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

#include "Diagnostics/Include/System/VoltageTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgConfirmationText.h"

namespace Diagnostics {

namespace System {

CVoltageTest::CVoltageTest(void)
    : CTestBase()
{
}

CVoltageTest::~CVoltageTest(void)
{
}

void CVoltageTest::Run(void)
{
    qDebug() << "110V/220V Switching Test starts!";

    this->FirstConfirmVoltage();
}

void CVoltageTest::FirstConfirmVoltage(void)
{
    qDebug() << "110V/220V Switching Test: first confirm voltage!";

    /// \todo: test the system voltage here *************************/
    int Voltage = 220;

    // ask the customer to confirm the test result
    MainMenu::CDlgConfirmationText *dlg = new MainMenu::CDlgConfirmationText;
    dlg->SetDialogTitle(tr("110V/220V Switching Test"));
    dlg->SetText(tr("Is the instrument connected to a %1V power supply?").
                 arg(Voltage));

    CONNECTSIGNALSLOT(dlg, accepted(),this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CVoltageTest::Succeed(void)
{
    qDebug() << "110V/220V Switching Test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("110V/220V Switching Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("110V/220V switching test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CVoltageTest::Fail(void)
{
    qDebug() << "110V/220V Switching Test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("110V/220V Switching Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("110V/220V switching test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

} // namespace System

} // namespace Diagnostics
