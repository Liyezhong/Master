/****************************************************************************/
/*! \file MovementTest.cpp
 *
 *  \brief Implementation of Rotary valve movement test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-8
 *   $Author:  $ Arthur Li
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

#include "Diagnostics/Include/RotaryValve/MovementTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include <QCoreApplication>
#include <QTimer>
#include <QObject>

#define QCOREAPPLICATION_EXEC(TIMEOUT) \
{ \
    QTimer timer; \
    timer.setSingleShot(true); \
    timer.setInterval(TIMEOUT); \
    timer.start(); \
    QObject::connect(&timer, SIGNAL(timeout()), QCoreApplication::instance(), SLOT(quit())); \
    QCoreApplication::exec(); \
}

namespace Diagnostics {

namespace RotaryValve {

CMovementTest::CMovementTest(CDiagnosticMessageDlg *dlg)
    : CTestBase()
    , dlg(dlg)
{
}

CMovementTest::~CMovementTest(void)
{
}

int CMovementTest::Run()
{
    qDebug() << "Rotary Valve movement test starts!";

    QString title((tr("Paraffin Oven Heating Test (Empty)")));
    QString text;

    int ret;

    text = tr(" Please verify status of the retort. If there is any "
                    "reagent or paraffin in the retort, abort this test and "
                    "change to the “Diagnostic_Retort_Reagent Drain "
                    "function to drain the liquid back to the original"
                    "position. Thereafter flush the retort if necessary.");

    ret = dlg->ShowConfirmMessage(title, text, true);
    if (ret == QMessageBox::RejectRole)
        return RETURN_ERR_FAIL;

    qreal RVSensor1TempCurrent(0);
    qreal RVSensor2TempCurrent(0);

    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("MovementTest");
    qreal RVSensor1TempMin = p_TestCase->GetParameter("RVSensor1TempMin").toFloat();
    qreal RVSensor2TempMin = p_TestCase->GetParameter("RVSensor2TempMin").toFloat();

    int t = p_TestCase->GetParameter("t").toInt();
    for (int i = 0; i < t; i++) {
        QCOREAPPLICATION_EXEC(5);
        ret = dev->RVGetTemp(&RVSensor1TempCurrent, &RVSensor2TempCurrent);
        qDebug()<<"RVGetTemp1 --- " << RVSensor1TempMin;
        qDebug()<<"RVGetTemp2 --- " <<RVSensor2TempMin;
        if (ret != RETURN_OK)
            break;
        if (RVSensor1TempCurrent >= RVSensor1TempMin && RVSensor2TempCurrent >= RVSensor2TempMin)
            break;
    }

    if (ret != RETURN_OK) {
        text = tr("Rotary Valve Movement Test failed."
                  "Rotary Valve cannot rotate, due to the minimum "
                  "temperature has not been reached. Please check "
                  "resistance of temperature sensors, current of heating "
                  "element and function of ASB3. If no root cause "
                  "found, check main relay on ASB15 and cable "
                  "connections in addition. Exchange part accordingly.");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    // RV initialize
    if (dev->RVInitialize() != RETURN_OK) {
        text = tr("Rotary Valve Movement Test failed. "
                  "It might work in some minutes when "
                  "solidified paraffin in the rotary valve is "
                  "molten. Repeat initializing test again in "
                  "about 15mins. If it still fails in the second "
                  "try, exchange rotary valve, reboot the "
                  "Service Software and repeat this test.");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr(" Rotating Rotary Valve to tube position7");
    dlg->ShowWaitingDialog(title, text);
    // prompt RV 7
    if (dev->RVMovePosition(true, 7) != RETURN_OK) {
        text = tr("Rotary Valve Movement Test failed."
                  "It might work in some minutes when solidified paraffin in the rotary valve is molten."
                  "Repeat initializing test again in about 15mins. If it still fails in the second try, "
                  "exchange rotary valve, reboot the Service Software and repeat this test.");
        dlg->HideWaitingDialog();
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr(" Rotating Rotary Valve to tube position5");
    dlg->ShowWaitingDialog(title, text);
    // prompt RV 5
    if (dev->RVMovePosition(true, 5) != RETURN_OK) {
        text = tr("Rotary Valve Movement Test failed."
                  "It might work in some minutes when solidified paraffin in the rotary valve is molten."
                  "Repeat initializing test again in about 15mins. If it still fails in the second try, "
                  "exchange rotary valve, reboot the Service Software and repeat this test.");
        dlg->HideWaitingDialog();
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr(" Rotating Rotary Valve to tube position12");
    dlg->ShowWaitingDialog(title, text);
    // prompt RV 12
    if (dev->RVMovePosition(true, 12) != RETURN_OK) {
        text = tr("Rotary Valve Movement Test failed."
                  "It might work in some minutes when solidified paraffin in the rotary valve is molten."
                  "Repeat initializing test again in about 15mins. If it still fails in the second try, "
                  "exchange rotary valve, reboot the Service Software and repeat this test.");
        dlg->HideWaitingDialog();
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr(" Rotating Rotary Valve to tube position2");
    dlg->ShowWaitingDialog(title, text);
    // prompt RV 2
    if (dev->RVMovePosition(true, 2) != RETURN_OK) {
        text = tr("Rotary Valve Movement Test failed."
                  "It might work in some minutes when solidified paraffin in the rotary valve is molten."
                  "Repeat initializing test again in about 15mins. If it still fails in the second try, "
                  "exchange rotary valve, reboot the Service Software and repeat this test.");
        dlg->HideWaitingDialog();
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }
    dlg->HideWaitingDialog();
    text = tr("Rotary Valve Movement Test successful.");
    dlg->ShowMessage(title, text, RETURN_OK);
    return RETURN_OK;
}

void CMovementTest::Start(void)
{
    qDebug() << "Rotary Valve Initializing: start initializing!";

#if 0
    /// \todo: start getting initial position another thread *****************************/

    // inform the customer of the test running status
    MainMenu::CWaitDialog *dlg = new MainMenu::CWaitDialog;
    dlg->SetDialogTitle(tr("Rotary Valve Initializing"));
    dlg->SetText(tr("Getting initial position..."));
    dlg->HideAbort();
    dlg->SetTimeout(5000); /// \todo: change time out

    /// \todo: connect the test success signal with waiting dialog accept slot */
    // (void)connect(TEST, SIGNAL(SUCCESS_SIGNAL()), dlg, SLOT(accept()) );
    /// \todo: connect the test failure signal with waiting dialog reject slot */
    // (void)connect(TEST, SIGNAL(FAILURE_SIGNAL()), dlg, SLOT(reject()) );

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed() );
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail() );

    dlg->exec();

    delete dlg;
#endif

//    emit StartRVInitTest();
}

void CMovementTest::Succeed(void)
{
//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST_SUCCESS);
//    qDebug() << "Rotary Valve Initializing succeeded!";
//
//    // display success message
//    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
//    dlg->SetTitle(tr("Rotary Valve Initializing"));
//    dlg->SetIcon(QMessageBox::Information);
//    dlg->SetText(tr("Succeed to get the initial position!"));
//    dlg->HideButtons();
//    dlg->SetButtonText(1, tr("OK"));
//
//    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept());
//
//    dlg->exec();
//
//    delete dlg;

    /// \todo: log here **************************************/
}

void CMovementTest::Fail(void)
{
//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST_FAILURE);
//    qDebug() << "Rotary Valve Initializing failed!";
//
    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Rotary Valve Movement Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr(" Rotary Valve Movement Test failed. "
                    "Rotary Valve cannot rotate, due to the minimum "
                    "temperature has not been reached. Please check "
                    "resistance of temperature sensors, current of heating "
                    "element and function of ASB3. If no root cause "
                    "found, check main relay on ASB15 and cable "
                    "connections in addition. Exchange part accordingly."));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept());

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

} // namespace RotaryValve

} // namespace Diagnostics
