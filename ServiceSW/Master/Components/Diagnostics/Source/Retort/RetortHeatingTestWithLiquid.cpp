/****************************************************************************/
/*! \file RetortHeatingTestWithLiquid.cpp
 *
 *  \brief Implementation of Retort heating test with liquid.
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

#include "Diagnostics/Include/Retort/RetortHeatingTestWithLiquid.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgWaitingStatus.h"


namespace Diagnostics {

namespace Retort {

CHeatingTestWithLiquid::CHeatingTestWithLiquid(void)
    : CTestBase()
{
}

CHeatingTestWithLiquid::~CHeatingTestWithLiquid(void)
{
}

void CHeatingTestWithLiquid::Run(void)
{
    qDebug() << "Retort Heating test with liquid starts!";

    this->FirstCheckBottle();
}

void CHeatingTestWithLiquid::FirstCheckBottle(int Position)
{
    qDebug() << "Retort Heating test with liquid: first connect bottle!";

    QString Station("reagent bottle");
    /// \todo: change Station to "wax bath" according to the position */

    // inform the customer to connect bottle at Position
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Retort Heating Test - with liquid"));
    dlg->SetText(tr("Please check the %1 in position %2 is full").
                 arg(Station).arg(Position));

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondStartHeating());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CHeatingTestWithLiquid::SecondStartHeating(void)
{
    qDebug() << "Retort Heating test with liquid: start heating!";

    /// \todo: start heating function in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CDlgWaitingStatus *dlg = new MainMenu::CDlgWaitingStatus;
    dlg->SetDialogTitle(tr("Heatint Test - with liquid"));
    dlg->SetText(tr("Testing..."));
    dlg->SetAbortLabel(tr("Stop"));
    dlg->SetTimeout(5000); /// \todo: change time out
    dlg->SetStatus(tr("Target Temperature: %1°C"
                      "\r\n"
                      "Current Temperature: %2°C"));

    /// \todo: update the current temperature to the waiting dialog */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(SetStatus(const QString &)) );
    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CHeatingTestWithLiquid::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST_SUCCESS);
    qDebug() << "Retort Heating test with liquid succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Retort Heating Test - with liquid"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Heating test with liquid SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), this, ThirdDrain() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingTestWithLiquid::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST_FAILURE);
    qDebug() << "Retort Heating test with liquid failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Retort Heating Test - with liquid"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Heating test with liquid FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), this, ThirdDrain() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingTestWithLiquid::Cancel(void)
{
    qDebug() << "Level sensor detecting test canceled!";
}

void CHeatingTestWithLiquid::ThirdDrain(void)
{
    qDebug() << "Retort Heating test with liquid: start draining!";

    /// \todo: start draining function in another thread *****************************/

    // inform the customer of the draining status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Retort Heating Test - with liquid"));
    dlg->SetText(tr("Draining..."));
    dlg->SetTimeout(5000); /// \todo: change time out
    dlg->HideAbort();

    /// \todo: connect the draining end signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, ForthDisconnectBottle() );
    // CONNECTSIGNALSLOT(dlg, rejected(), this, ForthDisconnectBottle() );

    dlg->exec();

    delete dlg;
}

void CHeatingTestWithLiquid::ForthDisconnectBottle(void)
{
    qDebug() << "Retort Heating test with liquid: disconnect reagent bottle!";

    // display information
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Retort Heating Test - with liquid"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Please pull out the reagent bottle you have connected!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;
}

} // namespace Retort

} // namespace Diagnostics
