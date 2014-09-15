/****************************************************************************/
/*! \file LevelSensorDetectingTest.cpp
 *
 *  \brief Implementation of Retort Level Sensor Detecting test.
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

#include "Diagnostics/Include/Retort/LevelSensorDetectingTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"

#include "ServiceWidget/Include/DlgWizardInputBottlePosition.h"
#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgWizard3Buttons.h"
#include "ServiceWidget/Include/DlgConfirmationText.h"


namespace Diagnostics {

namespace Retort {

CLevelSensorDetectingTest::CLevelSensorDetectingTest(void)
    : CTestBase()
{
}

CLevelSensorDetectingTest::~CLevelSensorDetectingTest(void)
{
}

int CLevelSensorDetectingTest::Run(void)
{
    qDebug() << "Level sensor detecting test starts!";

    this->FirstInputBottlePosition();
}

void CLevelSensorDetectingTest::FirstInputBottlePosition(void)
{
    qDebug() << "Level sensor detecting test: first input bottle position!";

    // inform the customer to input the bottle position
    MainMenu::CDlgWizardInputBottlePosition *dlg =
            new MainMenu::CDlgWizardInputBottlePosition;
    dlg->SetDialogTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetText(tr("Please input the bottle position "
                    "you wish to suck the liquid!"));
    dlg->SetButtonLabel(tr("Bottle position"));

    CONNECTSIGNALSLOT(dlg, AcceptPosition(qint32), this, SecondCheckBottle(qint32));
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CLevelSensorDetectingTest::SecondCheckBottle(qint32 Position)
{
    qDebug() << "Level sensor detecting test: second connect bottle!";

    // save position
    m_Position = Position;
    /// \todo: check position ***************************************/
    QString Station("reagent bottle");
    /// \todo: change Station to "wax bath" according to the position */

    // inform the customer to connect bottle at Position
    MainMenu::CDlgWizardText *dlg = new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetText(tr("Please check the %1 in position %2 is full").
                 arg(Station).arg(Position));

    CONNECTSIGNALSLOT(dlg, accepted(), this, ThirdSelfTest());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CLevelSensorDetectingTest::ThirdSelfTest()
{
    qDebug() << "Level sensor detecting test: start detecting!";


#if 0
    /// \todo: start detecting function in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetText(tr("Testing..."));
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, SelfTestSucceed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, SelfTestFail() );

    dlg->exec();

    delete dlg;
#endif

    emit StartLevelSensorDetectTest(m_Position);
}

void CLevelSensorDetectingTest::SelfTestSucceed(void)
{
    qDebug() << "Level sensor detecting test: self test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Level sensor self test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    CONNECTSIGNALSLOT(dlg, accepted(), this, ForthCheckLiquid() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CLevelSensorDetectingTest::SelfTestFail(void)
{
    qDebug() << "Level sensor detecting test: self test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Level sensor self test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), this, ForthCheckLiquid() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CLevelSensorDetectingTest::ForthCheckLiquid(void)
{
    qDebug() << "Level sensor detecting test: forth check liquid level!";

    // inform the customer to check liquid level in retort
    MainMenu::CDlgWizard3Buttons *dlg = new MainMenu::CDlgWizard3Buttons;
    dlg->SetDialogTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetText(tr("Please press to open the Retort lid to check "
                    "if the liquid is over the level sensor!"));
    dlg->SetButtonText(tr("Open"));

    dlg->DisableNext();
    //dlg->HideNext();

    CONNECTSIGNALSLOT(dlg, ThirdSelected(), this, OpenLidLock() );
    CONNECTSIGNALSLOT(dlg, ThirdSelected(), dlg, EnableNext() );
    CONNECTSIGNALSLOT(dlg, accepted(),this, ForthConfirmResult() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel() );

    dlg->exec();

    delete dlg;
}

void CLevelSensorDetectingTest::OpenLidLock(void)
{
    qDebug() << "Level sensor detecting test: open lid lock from software!";

    /// \todo: open lid lock here **************************/
}

void CLevelSensorDetectingTest::ForthConfirmResult(void)
{
    qDebug() << "Level sensor detecting test: forth confirm liquid level status!";

    // ask the customer to confirm the lid lock status
    MainMenu::CDlgConfirmationText *dlg = new MainMenu::CDlgConfirmationText;
    dlg->SetDialogTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetText(tr("Does the liquid cover the level sensor?"));

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CLevelSensorDetectingTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST_SUCCESS);
    qDebug() << "Level sensor detecting test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Level sensor detecting test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), this, FifthDrain() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CLevelSensorDetectingTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST_FAILURE);
    qDebug() << "Level sensor detecting test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Level sensor detecting test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), this, FifthDrain() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CLevelSensorDetectingTest::Cancel(void)
{
    qDebug() << "Level sensor detecting test canceled!";
}

void CLevelSensorDetectingTest::FifthDrain(void)
{
    qDebug() << "Level sensor detecting test: start draining!";

    /// \todo: start draining function in another thread *****************************/

    // inform the customer of the draining status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Level Sensor - Detecting Test"));
    dlg->SetText(tr("Draining..."));
    dlg->SetTimeout(5000); /// \todo: change time out
    dlg->HideAbort();

    /// \todo: connect the draining end signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, SixthDisconnectBottle() );
    //CONNECTSIGNALSLOT(dlg, rejected(), this, SixthDisconnectBottle() );

    dlg->exec();

    delete dlg;
}

void CLevelSensorDetectingTest::SixthDisconnectBottle(void)
{
    qDebug() << "Level sensor detecting test: disconnect reagent bottle!";

    // display information
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Level Sensor - Detecting Test"));
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
