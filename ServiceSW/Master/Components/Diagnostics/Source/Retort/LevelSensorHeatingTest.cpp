/****************************************************************************/
/*! \file LevelSensorHeatingTest.cpp
 *
 *  \brief Implementation of Retort Level Sensor Heating test.
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

#include "Diagnostics/Include/Retort/LevelSensorHeatingTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"


namespace Diagnostics {

namespace Retort {

CLevelSensorHeatingTest::CLevelSensorHeatingTest(void)
    : CTestBase()
{
}

CLevelSensorHeatingTest::~CLevelSensorHeatingTest(void)
{
}

void CLevelSensorHeatingTest::Run(void)
{
    qDebug() << "Level sensor heating test starts!";

    this->StartHeating();
}

void CLevelSensorHeatingTest::StartHeating(void)
{
    qDebug() << "Level sensor heating test: start heating!";

    /// \todo: start heating function in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Level Sensor - Heating Test"));
    dlg->SetText(tr("Testing..."));
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

void CLevelSensorHeatingTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST_SUCCESS);
    qDebug() << "Level sensor heating test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Level Sensor - Heating Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Level sensor heating test SUCCEEDED!"
                    "\r\n"
                    "Get temperature in time!"));
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

void CLevelSensorHeatingTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST_FAILURE);
    qDebug() << "Level sensor heating test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Level Sensor - Heating Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Level sensor heating test FAILED!"
                    "\r\n"
                    "Cannot get temperature in time!"));
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
