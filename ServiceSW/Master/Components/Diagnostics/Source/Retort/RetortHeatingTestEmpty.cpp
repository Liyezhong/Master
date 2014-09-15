/****************************************************************************/
/*! \file RetortHeatingTestEmpty.cpp
 *
 *  \brief Implementation of Retort Heating test empty.
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

#include "Diagnostics/Include/Retort/RetortHeatingTestEmpty.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWaitingStatus.h"


namespace Diagnostics {

namespace Retort {

CHeatingTestEmpty::CHeatingTestEmpty(void)
    : CTestBase()
{
}

CHeatingTestEmpty::~CHeatingTestEmpty(void)
{
}

int CHeatingTestEmpty::Run(void)
{
    qDebug() << "Retort Heating test empty starts!";

    this->StartHeating();
}

void CHeatingTestEmpty::StartHeating(void)
{
    qDebug() << "Retort Heating test empty: start heating!";

    /// \todo: start heating function in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CDlgWaitingStatus *dlg = new MainMenu::CDlgWaitingStatus;
    dlg->SetDialogTitle(tr("Retort Heating Test - empty"));
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

void CHeatingTestEmpty::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_EMPTY_TEST_SUCCESS);
    qDebug() << "Retort Heating test empty succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Retort Heating Test - empty"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Heating test empty SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    /// \todo: connect the message dialog accepted signal
    ///        with the test's stop heating slots */
    // (void)connect(dlg, SIGNAL(accept()), TEST, SLOT(STOP_HEATING_SLOT()) );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingTestEmpty::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_EMPTY_TEST_FAILURE);
    qDebug() << "Retort Heating test empty failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Retort Heating Test - empty"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Heating test empty FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    /// \todo: connect the message dialog accepted signal
    ///        with the test's stop heating slots */
    // (void)connect(dlg, SIGNAL(accept()), TEST, SLOT(STOP_HEATING_SLOT()) );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

} // namespace Retort

} // namespace Diagnostics
