/****************************************************************************/
/*! \file FillingNDrainingTest.cpp
 *
 *  \brief Implementation of System filling and draining test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-09
 *   $Author:  $ Dixiong Li
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

#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "Diagnostics/Include/LevelSensorHeatingDialog.h"
#include "Diagnostics/Include/SelectBottleNReagentDialog.h"

namespace Diagnostics {

namespace System {

QString FILLINGNDRAINING_TITLE("System Filling & Draining Test");  //!< this dialog title string

CFillingNDrainingTest::CFillingNDrainingTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_MessageDlg(p_MessageDlg)
{
}

CFillingNDrainingTest::~CFillingNDrainingTest(void)
{
}

int CFillingNDrainingTest::Run(void)
{
    qDebug() << "Filling and Draining Test starts!";

    int Ret = 0;

    if (ShowConfirmDlg(1) == 0 || ShowConfirmDlg(2) == 0) {
        return RETURN_ABORT;
    }

    qreal RVTempSensor1(0);
    qreal RVTempSensor2(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SSystemFillingNDraining");
    qreal TargetTemp1 = p_TestCase->GetParameter("RVTargetTemp1").toFloat();
    qreal TargetTemp2 = p_TestCase->GetParameter("RVTargetTemp2").toFloat();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    (void)p_DevProc->RVGetTemp(&RVTempSensor1, &RVTempSensor2);

    if (RVTempSensor1 < TargetTemp1 || RVTempSensor2 < TargetTemp2) {
        ShowFinishDlg(1);
        return RETURN_ERR_FAIL;
    }

    QString Text = "Rotary valve is initializing...";
    mp_MessageDlg->ShowWaitingDialog(FILLINGNDRAINING_TITLE, Text);
    Ret = p_DevProc->RVInitialize();
    mp_MessageDlg->HideWaitingDialog();
    if (Ret != RETURN_OK) {
        ShowFinishDlg(2);
        return RETURN_ERR_FAIL;
    }

    CSelectBottleNReagentDialog* p_SelectDlg = new CSelectBottleNReagentDialog(13, mp_Parent);
    p_SelectDlg->SetTitle(FILLINGNDRAINING_TITLE);

    if ( p_SelectDlg->exec() == 0) {
        delete p_SelectDlg;
        return RETURN_ABORT;
    }

    int ReagentGroup = p_SelectDlg->GetOption();
    int BottleNumber = p_SelectDlg->GetBottleNumber();

    qDebug()<<"Reagent Group:"<<ReagentGroup;
    qDebug()<<"Bottle Number:"<<BottleNumber;

    delete p_SelectDlg;

    Text = QString("Rotary Valve is moving to tube position %1").arg(BottleNumber);
    mp_MessageDlg->ShowWaitingDialog(FILLINGNDRAINING_TITLE, Text);
    Ret = p_DevProc->RVMovePosition(true, BottleNumber);
    mp_MessageDlg->HideWaitingDialog();
    if (Ret != RETURN_OK) {
        ShowFinishDlg(3);
        return RETURN_ERR_FAIL;
    }

    CLevelSensorHeatingDialog* p_HeatingDlg = new CLevelSensorHeatingDialog(mp_Parent);
    p_HeatingDlg->SetTitle(FILLINGNDRAINING_TITLE);

    bool HeatingRet = p_HeatingDlg->StartHeating(!ReagentGroup);

    if (p_HeatingDlg->result() == 0) {
        delete p_HeatingDlg;
        return RETURN_ABORT;
    }

    delete p_HeatingDlg;

    if (!HeatingRet) {
        ShowFinishDlg(4);
        return RETURN_ERR_FAIL;
    }

    Text = QString("Filling retort from the bottle %1").arg(BottleNumber);
    mp_MessageDlg->ShowWaitingDialog(FILLINGNDRAINING_TITLE, Text);
    QTime TimeNow = QTime::currentTime();
    Ret = p_DevProc->PumpSucking(0);
    int SuckingTime = TimeNow.secsTo(QTime::currentTime());
    mp_MessageDlg->HideWaitingDialog();
    (void)p_DevProc->LSStopHeating();
    if (Ret != RETURN_OK) {
        Text = "Draining...";
        mp_MessageDlg->ShowWaitingDialog(FILLINGNDRAINING_TITLE, Text);
        (void)p_DevProc->PumpDraining(0);
        mp_MessageDlg->HideWaitingDialog();
        ShowFinishDlg(5);
        return RETURN_ERR_FAIL;
    }

    Text = QString("Rotary Valve is moving to sealing position %1").arg(BottleNumber);
    mp_MessageDlg->ShowWaitingDialog(FILLINGNDRAINING_TITLE, Text);
    Ret = p_DevProc->RVMovePosition(false, BottleNumber);
    mp_MessageDlg->HideWaitingDialog();
    if (RETURN_OK != Ret)
    {
        mp_MessageDlg->ShowRVMoveFailedDlg(FILLINGNDRAINING_TITLE);
        return RETURN_ERR_FAIL;
    }

    Text = QString("Rotary Valve is moving to tube position %1").arg(BottleNumber);
    mp_MessageDlg->ShowWaitingDialog(FILLINGNDRAINING_TITLE, Text);
    Ret = p_DevProc->RVMovePosition(true, BottleNumber);
    mp_MessageDlg->HideWaitingDialog();
    if (RETURN_OK != Ret)
    {
        mp_MessageDlg->ShowRVMoveFailedDlg(FILLINGNDRAINING_TITLE);
        return RETURN_ERR_FAIL;
    }

    Text = "Retort was successfully filled. Start Draining";
    mp_MessageDlg->ShowWaitingDialog(FILLINGNDRAINING_TITLE, Text);
    TimeNow = QTime::currentTime();
    Ret = p_DevProc->PumpDraining(0);
    int DrainingTime  = TimeNow.secsTo(QTime::currentTime());
    mp_MessageDlg->HideWaitingDialog();

    Text = QString("System Filling & Draining Test finished<br>"\
            "Bottle No.: %1<br>Filling Time: %2<br>Draining Time: %3<br>")
            .arg(QString::number(BottleNumber), QTime().addSecs(SuckingTime).toString("hh:mm:ss"),
                 QTime().addSecs(DrainingTime).toString("hh:mm:ss"));

    mp_MessageDlg->ShowMessage(FILLINGNDRAINING_TITLE, Text, RETURN_OK);

    return RETURN_OK;
}

int CFillingNDrainingTest::ShowConfirmDlg(int StepNum)
{
    QString Text;

    if (StepNum == 1) {
        Text = "Be aware that the result of this test is only valid if the Retort_Level Sensor "\
                "Detection test was successfully performed before! "\
                "Please verify status of the retort. If there is any reagent or paraffin in the retort, "\
                "abort this test and change to the \"Diagnostic_Retort_Reagent Drain\" function to drain the "\
                "liquid back to the original position. Thereafter flush the retort if necessary.";
    }
    else if (StepNum == 2) {
        Text = "If you want to test with different reagents, please keep "\
                "the reagent compatibility in mind and do not cause any cross contamination.";
    }

    return mp_MessageDlg->ShowConfirmMessage(FILLINGNDRAINING_TITLE, Text, CDiagnosticMessageDlg::OK_ABORT);
}

void CFillingNDrainingTest::ShowFinishDlg(int RetNum)
{
    QString Text;

    ErrorCode_t Ret = RETURN_ERR_FAIL;

    if (RetNum == 1) {
        Text = "System Filling & Draining Test failed.<br>"\
                "Rotary Valve cannot rotate, due to the minimum temperature has "\
                "not been reached. Please check resistance of temperature "\
                "sensors, current of heating element and function of ASB3. If no "\
                "root cause found, check main relay on ASB15 and cable "\
                "connections in addition. Exchange part accordingly.";
    }
    else if (RetNum == 2) {
        Text = "System Filling & Draining Test failed.<br>"\
                "It might work in some minutes when solidified paraffin in "\
                "the rotary valve is molten. Repeat initializing test again in "\
                "about 15mins. If it still fails in the second try, exchange "\
                "rotary valve, reboot the Service Software and repeat this test.";
    }
    else if (RetNum == 3) {
        Text = "System Filling & Draining failed.<br>"\
                "It might work in some minutes when solidified paraffin in "\
                "the rotary valve is molten. Repeat initializing test again in "\
                "about 15mins. If it still fails in the second try, exchange "\
                "rotary valve, reboot the Service Software and repeat this test.";
    }
    else if (RetNum == 4) {
        Text = "System Filling & Draining Test failed.<br>"\
                "Level sensor's target temperature was not "\
                "reached in time. Clean level sensor, repeat this "\
                "test. If still failed, exchange the level sensor.";
    }
    else if (RetNum == 5) {
        Text = "System Filling & Draining Test failed.<br>"\
                "Level sensor did not detect any liquid within 2mins. "\
                "Clean level sensor, perform Retort_Level Sensor Dection test.";
    }

    mp_MessageDlg->ShowMessage(FILLINGNDRAINING_TITLE, Text, Ret);
}

} // namespace System

} // namespace Diagnostics
