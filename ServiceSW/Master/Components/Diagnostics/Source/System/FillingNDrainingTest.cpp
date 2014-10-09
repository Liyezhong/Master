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

#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "Diagnostics/Include/SelectBottleNReagentDialog.h"

namespace Diagnostics {

namespace System {

QString FILLINGNDRAINING_TITLE("System Filling && Draining Test");

CFillingNDrainingTest::CFillingNDrainingTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_Parent(p_Parent),
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
        return Ret;
    }

    qreal RVTempSensor1(0);
    qreal RVTempSensor2(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SSystemFillingNDraining");
    qreal TargetTemp = p_TestCase->GetParameter("RVTargetTemp").toFloat();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    p_DevProc->RVGetTemp(&RVTempSensor1, &RVTempSensor2);

    if (RVTempSensor1 < TargetTemp || RVTempSensor2 < TargetTemp) {
        ShowFinishDlg(1);
        return Ret;
    }

    //mp_MessageDlg->ShowWaitingDialog(FILLINGNDRAINING_TITLE, "Rotary valve initializing...");
    Ret = p_DevProc->RVInitialize();
    mp_MessageDlg->HideWaitingDialog();
    if (Ret == 0) {
        ShowFinishDlg(2);
        return Ret;
    }

    CSelectBottleNReagentDialog* p_SelectDlg = new CSelectBottleNReagentDialog(mp_Parent);

    if ( p_SelectDlg->exec() == 0) {
        delete p_SelectDlg;
        p_SelectDlg = NULL;
        return Ret;
    }

    qreal HeatingTargetTemp(0);
    //qreal CurrentTemp = ;
    int ReagentGroup = p_SelectDlg->GetReagentGroup();
    int BottleNumber = p_SelectDlg->GetBottleNumber();

    qDebug()<<"Reagent Group:"<<ReagentGroup;
    qDebug()<<"Bottle Number:"<<BottleNumber;

    delete p_SelectDlg;
    p_SelectDlg = NULL;

    if (ReagentGroup == 1) {
        HeatingTargetTemp = p_TestCase->GetParameter("XyleneTargetTemp").toInt();
    }
    else if (ReagentGroup == 2) {
        HeatingTargetTemp = p_TestCase->GetParameter("OtherTargetTemp").toInt();
    }

    p_DevProc->RVStartHeating(HeatingTargetTemp);

    ShowFinishDlg(3);
}

bool CFillingNDrainingTest::CheckRVTemperature()
{
    int Ret = 0;


    //p_DevProc->Pause(60000);


}

int CFillingNDrainingTest::ShowConfirmDlg(int StepNum)
{
    QString Text;

    if (StepNum == 1) {
        Text = "Be aware that the result of this test is only valid if the Retort_Level Sensor "\
                "Detection test was successfully performed before! "\
                "Please verify status of the retort. If there is any reagent or paraffin in the retort, "\
                "abort this test and change to the “Diagnostic_Retort_Reagent Drain” function to drain the "\
                "liquid back to the original position. Thereafter flush the retort if necessary";
    }
    else if (StepNum == 2) {
        Text = "If you want to test with different reagents , please keep "\
                "the reagent compatibility in mind and do not cause any cross contamination";
    }

    return mp_MessageDlg->ShowConfirmMessage(FILLINGNDRAINING_TITLE, Text, true);
}

void CFillingNDrainingTest::ShowFinishDlg(int RetNum)
{
    QString Text;

    ErrorCode_t Ret = RETURN_ERR_FAIL;

    if (RetNum == 1) {
        Text = "System Filling & Draining Test failed. "\
                "Rotary Valve cannot rotate, due to the minimum temperature has "\
                "not been reached. Please check resistance of temperature "\
                "sensors, current of heating element and function of ASB3. If no "\
                "root cause found, check main relay on ASB15 and cable "\
                "connections in addition. Exchange part accordingly";
    }
    else if (RetNum == 2) {
        Text = "System Filling & Draining Test failed. "\
                "It might work in some minutes when solidified paraffin in "\
                "the rotary valve is molten. Repeat initializing test again in "\
                "about 15mins. If it still fails in the second try, exchange "\
                "rotary valve, reboot the Service Software and repeat this test";
    }
    else if (RetNum == 3) {
        Text = "System Filling & Draining Test failed. "\
                "Level sensor’s target temperature was not "\
                "reached in time. Clean level sensor, repeat this "\
                "test. If still failed, exchange the level sensor";
    }
    else if (RetNum == 4) {
        Text = "System Filling & Draining failed. "\
                "It might work in some minutes when solidified paraffin in "\
                "the rotary valve is molten. Repeat initializing test again in "\
                "about 15mins. If it still fails in the second try, exchange "\
                "rotary valve, reboot the Service Software and repeat this test.";
    }
    else if (RetNum == 5) {
        Text = "System Filling & Draining Test failed. Level "\
                "sensor did not detect any liquid within 2mins. "\
                "Clean level sensor, perform Retort_Level Sensor Dection test.";
    }

    mp_MessageDlg->ShowMessage(FILLINGNDRAINING_TITLE, Text, Ret);
}

} // namespace System

} // namespace Diagnostics
