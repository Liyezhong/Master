/****************************************************************************/
/*! \file SpeakerTest.cpp
 *
 *  \brief Implementation of System speaker test.
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

#include "Diagnostics/Include/System/SpeakerTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgConfirmationText.h"

#include "Diagnostics/Include/System/DlgSpeaker.h"

namespace Diagnostics {

namespace System {

CSpeakerTest::CSpeakerTest(void)
    : CTestBase()
{
}

CSpeakerTest::~CSpeakerTest(void)
{
}

void CSpeakerTest::Run(void)
{
    qDebug() << "Speaker Test starts!";

    this->FirstOpenDialog();
}

void CSpeakerTest::FirstOpenDialog(void)
{
    qDebug() << "Speaker Test: first open speaker test dialog!";

    // open the speaker test dialog
    CDlgSpeaker *dlg = new CDlgSpeaker;
    dlg->SetDialogTitle(tr("Speaker Test"));

    CONNECTSIGNALSLOT(dlg, TestSpeaker(int, int), this, TestSpeaker(int, int));
    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondConfirmResult() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CSpeakerTest::TestSpeaker(int Sound, int Volume)
{
    qDebug() << "Speaker Test: test speaker with sound: " 
             << Sound << " and Volume: " << Volume << " !";

    /// \todo: play sound here **************************/
}

void CSpeakerTest::SecondConfirmResult(void)
{
    qDebug() << "Speaker Test: second confirm result!";

    // ask the customer to confirm the test result
    MainMenu::CDlgConfirmationText *dlg = new MainMenu::CDlgConfirmationText;
    dlg->SetDialogTitle(tr("Speaker Test"));
    dlg->SetText(tr("Does the speaker test pass?"));

    CONNECTSIGNALSLOT(dlg, accepted(),this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CSpeakerTest::Succeed(void)
{
    qDebug() << "Speaker Test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Speaker Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Speaker test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CSpeakerTest::Fail(void)
{
    qDebug() << "Speaker Test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Speaker Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Speaker test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CSpeakerTest::Cancel(void)
{
    qDebug() << "Speaker Test canceled!";
}

} // namespace System

} // namespace Diagnostics
