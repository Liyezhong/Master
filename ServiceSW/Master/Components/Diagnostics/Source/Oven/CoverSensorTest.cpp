/****************************************************************************/
/*! \file CoverSensorTest.cpp
 *
 *  \brief Implementation of Oven cover sensor test.
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

#include "Diagnostics/Include/Oven/CoverSensorTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgConfirmationStatus.h"

namespace Diagnostics {

namespace Oven {

CCoverSensorTest::CCoverSensorTest(void)
    : CTestBase()
{
}

CCoverSensorTest::~CCoverSensorTest(void)
{
}

void CCoverSensorTest::Run(void)
{
    qDebug() << "Cover sensor test starts!";

    this->FirstManualOpen();
}

void CCoverSensorTest::FirstManualOpen(void)
{
    qDebug() << "Cover sensor test: first manual open cover!";

    // inform the customer to manually open the cover
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Cover Sensor Test"));
    dlg->SetText(tr("Please Open the Oven cover Manually!"));

    CONNECTSIGNALSLOT(dlg, accepted(), this, FirstCheckOpen() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CCoverSensorTest::FirstCheckOpen(void)
{
    qDebug() << "Cover sensor test: first check Cover sensor status!";

    /// \todo: read cover sensor status here **************************/

    // ask the customer to confirm the Cover sensor status
    MainMenu::CDlgConfirmationStatus *dlg = new MainMenu::CDlgConfirmationStatus;
    dlg->SetDialogTitle(tr("Cover Sensor Test"));
    dlg->SetText(tr("Does the Oven Cover Sensor status show \"Open\"?"));
    dlg->SetStatus(tr("Oven Cover sensor status: \"Open\"")); /// \todo change status here **********/

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondManualClose() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CCoverSensorTest::SecondManualClose(void)
{
    qDebug() << "Cover sensor test: second close oven cover!";

    // inform the customer to close the cover
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Cover sensor Test"));
    dlg->SetText(tr("Please close the Oven Cover manually!"));

    CONNECTSIGNALSLOT(dlg, accepted(),this, SecondCheckClose() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CCoverSensorTest::SecondCheckClose(void)
{
    qDebug() << "Cover sensor test: second check Cover sensor status!";

    /// \todo: read cover sensor status here **************************/

    // ask the customer to confirm the Cover sensor status
    MainMenu::CDlgConfirmationStatus *dlg = new MainMenu::CDlgConfirmationStatus;
    dlg->SetDialogTitle(tr("Cover sensor - Test"));
    dlg->SetText(tr("Does the Oven Cover Sensor status show \"Closed\"?"));
    dlg->SetStatus(tr("Oven Cover Sensor status: \"Closed\"")); /// \todo change status here **********/

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CCoverSensorTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_SUCCESS);
    qDebug() << "Cover sensor test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Cover sensor - Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Cover sensor test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CCoverSensorTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_FAILURE);
    qDebug() << "Cover sensor test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Cover sensor - Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Cover sensor test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CCoverSensorTest::Cancel(void)
{
    qDebug() << "Cover sensor test canceled!";
}

} // namespace Oven

} // namespace Diagnostics
