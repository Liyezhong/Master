/****************************************************************************/
/*! \file FillingNDrainingTest.cpp
 *
 *  \brief Implementation of System filling and draining test.
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

#include "Diagnostics/Include/System/FillingNDrainingTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWizardInputBottlePosition.h"
#include "ServiceWidget/Include/DlgWizardText.h"


namespace Diagnostics {

namespace System {

CFillingNDrainingTest::CFillingNDrainingTest(void)
    : CTestBase()
{
}

CFillingNDrainingTest::~CFillingNDrainingTest(void)
{
}

int CFillingNDrainingTest::Run(void)
{
    qDebug() << "Filling and Draining Test starts!";

    this->FirstInputBottlePosition();
}

void CFillingNDrainingTest::FirstInputBottlePosition(void)
{
    qDebug() << "Filling and Draining Test: first input bottle position!";

    // inform the customer to input the bottle position
    MainMenu::CDlgWizardInputBottlePosition *dlg =
            new MainMenu::CDlgWizardInputBottlePosition;
    dlg->SetDialogTitle(tr("Filling and Draining Test"));
    dlg->SetText(tr("Please input the bottle position "
                    "you wish to suck the reagent!"));
    dlg->SetButtonLabel(tr("Bottle position"));

    CONNECTSIGNALSLOT(dlg, AcceptPosition(qint32), this, SecondCheckBottle(qint32));
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CFillingNDrainingTest::SecondCheckBottle(int Position)
{
    qDebug() << "Filling and Draining Test: second check bottle!";

    // save position
    m_Position = Position;
    /// \todo: check position ***************************************/
    QString Station("reagent bottle");
    /// \todo: change Station to "wax bath" according to the position */

    // inform the customer to connect bottle at Position
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Filling and Draining Test"));
    dlg->SetText(tr("Please check the %1 in position %2 is full").
                 arg(Station).arg(Position));

    CONNECTSIGNALSLOT(dlg, accepted(), this, ThirdFill());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CFillingNDrainingTest::ThirdFill(void)
{
    qDebug() << "Filling and Draining Test: third start filling!";

    /// \todo: start filling function in another thread **********/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Filling and Draining Test"));
    dlg->SetText(tr("Filling..."));
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, ForthPressToDrain() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CFillingNDrainingTest::ForthPressToDrain(void)
{
    qDebug() << "Filling and Draining Test: forth press to drain!";

    // inform the customer to connect bottle at Position
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Filling and Draining Test"));
    dlg->SetText(tr("Please press to \"Next\" to start draining test!"));

    dlg->HideCancel();

    CONNECTSIGNALSLOT(dlg, accepted(), this, FifthDrain());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CFillingNDrainingTest::FifthDrain(void)
{
    qDebug() << "Filling and Draining Test: fifth start draining!";

    /// \todo: start draining function in another thread **********/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Filling and Draining Test"));
    dlg->SetText(tr("Draining..."));
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

void CFillingNDrainingTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST_SUCCESS);
    qDebug() << "Filling and Draining Test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Filling and Draining Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Filling and draining test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CFillingNDrainingTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST_FAILURE);
    qDebug() << "Filling and Draining Test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Filling and Draining Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Filling and draining test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CFillingNDrainingTest::Cancel(void)
{
    qDebug() << "Filling and Draining Test canceled!";
}

} // namespace System

} // namespace Diagnostics
