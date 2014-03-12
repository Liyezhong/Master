/****************************************************************************/
/*! \file PressureSensorCalibrate.cpp
 *
 *  \brief Implementation of Pressure Sensor calibration.
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

#include "Calibration/Include/PressureSensorCalibrate.h"

#include <QDebug>

#include "Global/Include/Utils.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"


namespace Calibration {

CPressureSensorCalibrate::CPressureSensorCalibrate(void)
    : QObject()
{
}

CPressureSensorCalibrate::~CPressureSensorCalibrate(void)
{
}

void CPressureSensorCalibrate::Run(void)
{
    qDebug() << "Pressure Sensor Calibration starts!";

    this->StartCalibration();
}

void CPressureSensorCalibrate::StartCalibration(void)
{
    qDebug() << "Pressure Sensor Calibration: start heating!";

    /// \todo: start calibration in another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Pressure Sensor Calibration"));
    dlg->SetText(tr("Calibrating ..."));
    dlg->SetTimeout(5000); /// \todo: change time out, 20000

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
}

void CPressureSensorCalibrate::Succeed(void)
{
    qDebug() << "Pressure Sensor Calibration succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Pressure Sensor Calibration"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Pressure Sensor Calibration SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CPressureSensorCalibrate::Fail(void)
{
    qDebug() << "Pressure Sensor Calibration failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Pressure Sensor Calibration"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Pressure Sensor Calibration FAILED!"
                    "\r\n"
                    "Please confirm the air system is not blocked"
                    " and the retort lid is open."
                    " Then do calibration again!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

} // namespace Calibration
