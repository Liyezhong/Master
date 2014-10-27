/****************************************************************************/
/*! \file SystemSealing.cpp
 *
 *  \brief Implementation of System System sealing test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-10-20
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

#include "Diagnostics/Include/System/SystemSealingTest.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include <QDebug>
namespace Diagnostics {

namespace System {

CSystemSealingTest::CSystemSealingTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_MessageDlg(p_MessageDlg),
      m_MessageTitle("System Sealing Test")
{
}

CSystemSealingTest::~CSystemSealingTest(void)
{
}

int CSystemSealingTest::Run(void)
{
    qDebug() << "System sealing Test starts!";

    int Ret(0);

    if (ShowConfirmDlg() == 0) {
        return Ret;
    }

    qreal RVTempSensor1(0);
    qreal RVTempSensor2(0);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SSystemSealing");
    qreal TargetTemp1    = p_TestCase->GetParameter("RVTargetTemp1").toFloat();
    qreal TargetTemp2    = p_TestCase->GetParameter("RVTargetTemp2").toFloat();
    float TargetPressure = p_TestCase->GetParameter("TargetPressure1").toFloat();
    float PressureDiff   = p_TestCase->GetParameter("PressureDiff").toFloat();
    int KeepDuration     = p_TestCase->GetParameter("KeepDuration").toInt();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    (void)p_DevProc->RVGetTemp(&RVTempSensor1, &RVTempSensor2);

    if (RVTempSensor1 < TargetTemp1 || RVTempSensor2 < TargetTemp2) {
        ShowFinishDlg(1);
        return Ret;
    }

    QString Text = "Rotary valve is initializing...";
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    Ret = p_DevProc->RVInitialize();
    mp_MessageDlg->HideWaitingDialog();
    if (Ret != RETURN_OK) {
        ShowFinishDlg(2);
        return Ret;
    }

    Text = "Rotary Valve is moving to sealing position 1";
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    (void)p_DevProc->RVMovePosition(false, 1);
    mp_MessageDlg->HideWaitingDialog();

Create_Pressure_Test:

    Ret = TestCreatePressure(TargetPressure);

    if (Ret != RETURN_OK) {
        TestReleasePressure();
        ShowFinishDlg(3);

        Ret = TestCreatePressure(TargetPressure);

        TestReleasePressure();
        if (Ret == RETURN_OK) {
            ShowFinishDlg(4);
        }
        else {
            ShowFinishDlg(5);
        }

        return Ret;
    }

    p_DevProc->Pause(1000);
    (void)p_DevProc->PumpStopCompressor();

    if (TestKeepPressure(TargetPressure, PressureDiff, KeepDuration)) {
        ShowFinishDlg(3);
        if (!TestCreatePressure(TargetPressure)) {
            TestReleasePressure();

            ShowFinishDlg(6);
            return Ret;
        }

        p_DevProc->Pause(1000);
        (void)p_DevProc->PumpStopCompressor();
        if (TestKeepPressure(TargetPressure, PressureDiff, KeepDuration)) {
            ShowFinishDlg(5);
        }
        else {
            ShowFinishDlg(4);
        }

        return Ret;
    }

    if (TargetPressure > 0) {
        TargetPressure = p_TestCase->GetParameter("TargetPressure2").toFloat();
        goto Create_Pressure_Test;
    }

    Text = "System Sealing Test successful.";
    mp_MessageDlg->ShowMessage(m_MessageTitle, Text, RETURN_OK);

    return RETURN_OK;
}

bool CSystemSealingTest::TestCreatePressure(float TargetPressure)
{
    bool Ret = false;
    int WaitSec = 60;
    QString Text = QString("Creating pressure to %1Kpa...").arg(TargetPressure);
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    (void)p_DevProc->PumpSetFan(1);
    int PreRet = p_DevProc->PumpBuildPressure(TargetPressure);
    if (PreRet == RETURN_OK) {
        float CurrentPressure(0);
        while (WaitSec) {
            (void)p_DevProc->PumpGetPressure(&CurrentPressure);
            if ((TargetPressure > 0 && CurrentPressure >= TargetPressure) ||
                    (TargetPressure < 0 && CurrentPressure <= TargetPressure)) {
                Ret = true;
                break;
            }
            p_DevProc->Pause(1000);
            WaitSec--;
        }
    }

    mp_MessageDlg->HideWaitingDialog();

    return Ret;
}

void CSystemSealingTest::TestReleasePressure(bool StopCompressorFlag)
{
    QString Text = "Releasing pressure...";
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    if (StopCompressorFlag) {
        (void)p_DevProc->PumpStopCompressor();
    }
    (void)p_DevProc->PumpSetFan(0);
    (void)p_DevProc->PumpSetValve(1, 0);
    (void)p_DevProc->PumpSetValve(2, 0);
    p_DevProc->Pause(20*1000);
    mp_MessageDlg->HideWaitingDialog();
}

bool CSystemSealingTest::TestKeepPressure(float TargetPressure, float DiffPressure, int KeepDuration)
{
    bool Ret = false;
    QString Text = QString("Keep pressure for %1 seconds").arg(KeepDuration);
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    float RetPressure;
    p_DevProc->Pause(KeepDuration*1000);
    (void)p_DevProc->PumpGetPressure(&RetPressure);
    if (qAbs(TargetPressure - RetPressure) >= DiffPressure) {
        Ret = true;
    }
    mp_MessageDlg->HideWaitingDialog();
    TestReleasePressure(false);
    return Ret;
}

int CSystemSealingTest::ShowConfirmDlg(int StepNum)
{
    Q_UNUSED(StepNum);
    QString Text = "Please confirm the retort is empty and dry. Please check sealing surfaces on retort "
                "and lid. Close retort lid and rotate lock to closed position.";

    return mp_MessageDlg->ShowConfirmMessage(m_MessageTitle, Text, CDiagnosticMessageDlg::OK_ABORT);
}

void CSystemSealingTest::ShowFinishDlg(int RetNum)
{
    QString Text = "System Sealing Test failed.<br>";
    ErrorCode_t Ret = RETURN_ERR_FAIL;

    switch (RetNum) {
    case 1:
        Text += "Rotary Valve cannot rotate, due to the minimum "
                "temperature has not been reached. Please check "
                "resistance of temperature sensors, current of heating "
                "element and function of ASB3. If no root cause "
                "found, check main relay on ASB15 and cable "
                "connections in addition. Exchange part accordingly.";
        break;
    case 2:
        Text += "It might work in some minutes when solidified "
                "paraffin in the rotary valve is molten. Repeat "
                "initializing test again in about 15mins. If it still "
                "fails in the second try, exchange rotary valve, "
                "reboot the Service Software and repeat this test.";
        break;
    case 3:
        Text = "Please rotate lock to open position "
                "and open the retort lid . Completely seal "
                "the air inlet opening from inside the retort "
                "and keep it tightly sealed for about 1 "
                "minute during the next test step.";
        Ret = RETURN_OK;
        break;
    case 4:
        Text += "Please go to the interactive schematic and check "
                "all affected tubes and connectors between retort "
                "and rotary valve for any leakage. Also check all "
                "sealings at the retort ( including level sensor seal). "
                "Exchange the according part or resolve the "
                "leakage and repeat this test.";
        break;
    case 5:
        Text += "Please go to the interactive schematic and check "
                "proper function of pump and valves. Also check "
                "status of relieve valve. Check all affected tubes "
                "and connectors between pump and retort for any "
                "leakage. Check the tightness of the waxtrap. "
                "Exchange the according part or resolve the "
                "leakage and repeat this test.";
        break;

    case 6:
        Text += "Target pressure could not be reached. Probably the air "
                "inlet in the retort was not tightly sealed. Please repeat this test.";
        break;
    default:
        qDebug()<<"CSystemSealingTest:ShowFinishDlg error RetNum:"<<RetNum;
    }


    mp_MessageDlg->ShowMessage(m_MessageTitle, Text, Ret);
}

} // namespace System

} // namespace Diagnostics


