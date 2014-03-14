/****************************************************************************/
/*! \file LidLockTest.cpp
 *
 *  \brief Implementation of Retort Lid Lock test.
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

#include "Diagnostics/Include/Retort/LidLockTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgConfirmationStatus.h"
#include "ServiceWidget/Include/DlgWizard3Buttons.h"

namespace Diagnostics {

namespace Retort {

CLidLockTest::CLidLockTest(void)
    : CTestBase()
{
}

CLidLockTest::~CLidLockTest(void)
{
}

void CLidLockTest::Run(void)
{
    qDebug() << "Lid lock test starts!";

    this->FirstManualClose();
}

void CLidLockTest::FirstManualClose(void)
{
    qDebug() << "Lid lock test: first manual close retort!";

    // inform the customer to manually close the lid
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Lid Lock Test"));
    dlg->SetText(tr("Please Close the Retort Manually!"));

    CONNECTSIGNALSLOT(dlg, accepted(), this, FirstCheckClose() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CLidLockTest::FirstCheckClose(void)
{
    qDebug() << "Lid lock test: first check lid lock status!";

    /// \todo: read lock status here **************************/

    // ask the customer to confirm the lid lock status
    MainMenu::CDlgConfirmationStatus *dlg = new MainMenu::CDlgConfirmationStatus;
    dlg->SetDialogTitle(tr("Lid Lock Test"));
    dlg->SetText(tr("Does the Retort lid lock status show \"Closed\"?"));
    dlg->SetStatus(tr("Retort lid lock status: \"Closed\"")); /// \todo change status here **********/

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondPressOpen() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CLidLockTest::SecondPressOpen(void)
{
    qDebug() << "Lid lock test: second press to open lid lock!";

    // inform the customer to open the lid
    MainMenu::CDlgWizard3Buttons *dlg = new MainMenu::CDlgWizard3Buttons;
    dlg->SetDialogTitle(tr("Lid Lock Test"));
    dlg->SetText(tr("Please press to open the Retort lid!"));
    dlg->SetButtonText(tr("Open"));

    dlg->DisableNext();
    //dlg->HideNext();

    CONNECTSIGNALSLOT(dlg, ThirdSelected(), this, OpenLidLock() );
    CONNECTSIGNALSLOT(dlg, ThirdSelected(), dlg, EnableNext() );
    CONNECTSIGNALSLOT(dlg, accepted(),this, SecondCheckOpen() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CLidLockTest::OpenLidLock(void)
{
    qDebug() << "Lid lock test: open lid lock from software!";

    /// \todo: open lid lock here **************************/
}

void CLidLockTest::SecondCheckOpen(void)
{
    qDebug() << "Lid lock test: second check lid lock status!";

    /// \todo: read lock status here **************************/

    // ask the customer to confirmt the lid lock status
    MainMenu::CDlgConfirmationStatus *dlg = new MainMenu::CDlgConfirmationStatus;
    dlg->SetDialogTitle(tr("Lid Lock - Test"));
    dlg->SetText(tr("Does the Retort lid lock status show \"Open\"?"));
    dlg->SetStatus(tr("Retort lid lock status: \"Open\"")); /// \todo change status here **********/

    CONNECTSIGNALSLOT(dlg, accepted(), this, ThirdManualOpen() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CLidLockTest::ThirdManualOpen(void)
{
    qDebug() << "Lid lock test: third manual open retort!";

    // inform the customer to close and manually open the lid
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Lid Lock - Test"));
    dlg->SetText(tr("Please first Close the Retort Manually!"
                    "\r\n"
                    "\r\n"
                    "Then manually Open the Retort!"));

    CONNECTSIGNALSLOT(dlg, accepted(), this, ThirdCheckOpen() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CLidLockTest::ThirdCheckOpen(void)
{
    qDebug() << "Lid lock test: third check lid lock status!";

    /// \todo: read lock status here **************************/

    // ask the customer to confirm the lid lock status
    MainMenu::CDlgConfirmationStatus *dlg = new MainMenu::CDlgConfirmationStatus;
    dlg->SetDialogTitle(tr("Lid Lock - Test"));
    dlg->SetText(tr("Does the Retort lid lock status show \"Open\"?"));
    dlg->SetStatus(tr("Retort lid lock status: \"Open\"")); /// \todo change status here **********/

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CLidLockTest::Succeed(void)
{
    qDebug() << "Lid lock test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Lid Lock - Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Lid Lock test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CLidLockTest::Fail(void)
{
    qDebug() << "Lid lock test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Lid Lock - Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Lid Lock test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CLidLockTest::Cancel(void)
{
    qDebug() << "Lid lock test canceled!";
}

} // namespace Retort

} // namespace Diagnostics
