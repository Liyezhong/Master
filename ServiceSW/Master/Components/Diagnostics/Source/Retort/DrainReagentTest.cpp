/****************************************************************************/
/*! \file DrainReagentTest.cpp
 *
 *  \brief Implementation of Retort drain reagent test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-25
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

#include "Diagnostics/Include/Retort/DrainReagentTest.h"

#include <QDebug>
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "Diagnostics/Include/SelectBottleNReagentDialog.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace Diagnostics {

namespace Retort {

CDrainReagentTest::CDrainReagentTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_MessageDlg(p_MessageDlg),
      m_MessageTitle("Retort Drain Reagent")
{
}

CDrainReagentTest::~CDrainReagentTest(void)
{
}

int CDrainReagentTest::Run(void)
{
    qDebug() << "Drain reagent test starts!";

    bool CurrentTubeFlag;
    int CurrentPosition;
    if (!SelectPosition(1, CurrentTubeFlag, CurrentPosition)) {
        return RETURN_ERR_FAIL;
    }

    QString Text = "Select the content/condition within the retor.";
    QString Button1 = "Other Reagent";
    QString Button2 = "Paraffin";
    CSelectBottleNReagentDialog* p_SelectDlg = new CSelectBottleNReagentDialog(16, mp_Parent);
    p_SelectDlg->SetScrollWheelVisible(false);
    p_SelectDlg->SetTitle(m_MessageTitle);
    p_SelectDlg->SetLableText(Text);
    p_SelectDlg->SetRadioButtonText(Button1, Button2);

    if (p_SelectDlg->exec() == 0) {
        delete p_SelectDlg;
        return RETURN_ERR_FAIL;
    }

    bool TargetTubeFlag;
    int  TargetPosition;
    if (!SelectPosition(2, TargetTubeFlag, TargetPosition)) {
        return RETURN_ERR_FAIL;
    }

    int ParaffinOption = p_SelectDlg->GetOption();

    delete p_SelectDlg;
    qDebug()<<"Select ParaffinOption option :"<<ParaffinOption;
    if (ParaffinOption == 1) {
        if (ShowConfirmDlg(1) == 0) {
            return RETURN_ERR_FAIL;
        }

        if (ShowConfirmDlg(2) == 0) {
            ShowFinishDlg(1);
            return RETURN_ERR_FAIL;
        }

        if (!CheckLTubeTemp()) {
            ShowFinishDlg(2);
            return RETURN_ERR_FAIL;
        }

        if (!CheckOvenTemp()) {
            ShowFinishDlg(3);
            return RETURN_ERR_FAIL;
        }

        if (!CheckRVTemp(true)) {
            ShowFinishDlg(4);
            return RETURN_ERR_FAIL;
        }
    }
    else {
        if (!CheckRVTemp(false)) {
            ShowFinishDlg(4);
            return RETURN_ERR_FAIL;
        }
    }

    if (!BuildVacuum()) {
        ShowFinishDlg(5);
        return RETURN_ERR_FAIL;
    }

    ServiceDeviceProcess* p_Dev = ServiceDeviceProcess::Instance();
    Text = "Rotary valve is initializing...";
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    int Ret = p_Dev->RVInitialize(CurrentTubeFlag, CurrentPosition);
    mp_MessageDlg->HideWaitingDialog();

    if (Ret != RETURN_OK) {
        qDebug()<<"Retort drain reagent initializing rotary valve failed.";
    }

    for (int i = 0; i < 2; ++i) {
        Ret = p_Dev->RVMovePosition(true, TargetPosition);
        if (Ret == RETURN_OK) {
            break;
        }

        if (Ret == 1100) {//Time out
            ShowFinishDlg(6);
            return RETURN_ERR_FAIL;
        }

        if (Ret == -2) {// Upper limiter
            if (i == 1) {
                ShowFinishDlg(7);
                p_Dev->PumpStopCompressor();
                p_Dev->PumpSetValve(0, 0);
                p_Dev->PumpSetValve(1, 0);
                ShowFinishDlg(8);
                return RETURN_ERR_FAIL;
            }

            if (!SelectPosition(3, TargetTubeFlag, TargetPosition)) {
                return RETURN_ERR_FAIL;
            }
        }
    }

    Text = "Releasing pressure...";
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    Ret = p_Dev->PumpReleasePressure();
    mp_MessageDlg->HideWaitingDialog();

    if (Ret != RETURN_OK) {
        qDebug()<<"Retort DrainReagent : release pressure failed.";
    }

    Text = "Begin Draining...";
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    Ret = p_Dev->PumpDraining();
    mp_MessageDlg->HideWaitingDialog();

    if (Ret != RETURN_OK) {
        ShowFinishDlg(9);
        return Ret;
    }

    ShowFinishDlg(10);
    return RETURN_OK;
}

int CDrainReagentTest::ShowConfirmDlg(int StepNum)
{
    QString Text;
    CDiagnosticMessageDlg::BUTTON_TYPE BtnType = CDiagnosticMessageDlg::OK_ABORT;
    switch (StepNum) {
    case 1:
        Text = "Please visually observe the paraffin melting status in retort. As "
                "soon the sides of paraffin block are molten, remove remaining "
                "paraffin manually and ensure that existing specimen are provided to "
                "the customer.";
        break;
    case 2:
        BtnType = CDiagnosticMessageDlg::YES_NO;
        Text = "Can you verify that all the paraffin in the retort is completely molten and liquid.";
        break;

    default:
        qDebug()<<"Retort: Drain Reagent show confirm dialog error step numbuer.";

    }

    return mp_MessageDlg->ShowConfirmMessage(m_MessageTitle, Text, BtnType);
}

void CDrainReagentTest::ShowFinishDlg(int RetNum)
{
    QString Text;
    ErrorCode_t Ret = RETURN_ERR_FAIL;

    switch (RetNum) {
    case 1:
        Text = "Paraffin must be completely liquid to be drained. "
                "Please wait until it is completely molten and retry draining.";
        break;
    case 2:
        Text = "Liquid heating tube below minimum temperature. It might work in some "
                "minutes when minimum temperature is reached. Retry draining in about 15 mins.";
        break;
    case 3:
        Text = "Oven below minimum temperature. It might work in some minutes when "
                "minimum temperature is reached. Retry draining in about 15 mins.";
        break;
    case 4:
        Text = "Draining of retort failed. Rotary Valve cannot rotate, due to the "
                "minimum temperature has not been reached. "
                "It might work in some minutes when solidified paraffin in the rotary valve "
                "is molten. Retry draining in about 15mins.";
        break;
    case 5:
        Text = "Building Vacuum failed.<br>"
                "Please go to the interactive schematic and check proper "
                "function of pump and valves. Also check status of relieve valve. "
                "Check all affected tubes and connectors between pump and "
                "retort for any leakage. Check the tightness of the waxtrap. "
                "Exchange the according part or resolve the leakage and repeat this test.";
        break;
    case 6:
        Text = "Rotary Valve cannot reach target position.Please exchange rotary valve module.";
        break;
    case 7:
        Text = "Rotary Valve cannot reach target position. "
                "In case current RV position is at a sealing position , switch "
                "instrument off and exchange the Rotary valve. "
                "In case current RV position is at a tube position, please remove "
                "all bottles and wax baths now. Then place an empty vessel below "
                "the according connector. If possible , disconnect the liquid tube "
                "closest to the rotary valve to avoid contamination. If not "
                "possible, clean tube afterwards. Press the OK button and catch "
                "the liquid flowing out.";
        break;
    case 8:
        Text = "Please open the retort lid and wait until the "
                "liquid drained completely. Then press OK, switch "
                "the instrument off and exchange the rotary valve.";
        break;
    case 9:
        Text = "Draining liquid to original bottle was failed.<br>"
                "There is a blockage between the retort and the target "
                "position. Reason might be solid paraffin. Please wait for "
                "about 15 mins. Check for any blockage. Check function of "
                "liquid heating tube and oven. Please resolve the blockage, "
                "and repeat this retort drain function.";
        break;
    case 10:
        Text = "Draining liquid to original bottle was "
                "successful. Please flush the retort now";
        Ret = RETURN_OK;
        break;
    default:
        qDebug()<<"Retort: Drain Reagent show finish dialog error return numbuer.";
    }

    mp_MessageDlg->ShowMessage(m_MessageTitle, Text, Ret);
}

bool CDrainReagentTest::SelectPosition(int StepNum, bool& TubeFlag, int& Position)
{
    QString Text;
    CSelectBottleNReagentDialog* p_SelectDlg = new CSelectBottleNReagentDialog(16, mp_Parent);
    p_SelectDlg->SetTitle(m_MessageTitle);
    if (StepNum == 1) {
        Text = "If the retort is empty, press the \"Abort\" button now. If you can see any reagent remaining<br>"
                "in the retort, the current rotary valve position has to be typed in manually. If the back<br>"
                "panel was removed already, please select the current RV position by using the scroll wheel<br>"
                "and press \"OK\" to continue. If you do not know the current RV position, the instrument needs<br>"
                "first to be shut down to remove the back panel.Therefor, press the \"Shutdown\" button, remove<br>"
                "the back panel, read the rotary valve position and switch the instrument back on. Then enter<br>"
                "the \"Retort Drain Reagent\" function again and key in the current RV position.";

        QString Button1 = "Sealing";
        QString Button2 = "Tube";
        p_SelectDlg->SetRadioButtonText(Button1, Button2);
    }
    else if (StepNum == 2) {
        Text = "Please select the target position of the rotary valve<br>"
                "where the liquid should be drained to. ";

        p_SelectDlg->SetRadioButtonVisible(false);
    }
    else if (StepNum == 3) {
        Text = "Current position of rotary valve has to be typed in manually.<br>"
                "Read the rotary valve position and enter it by using the scroll wheel.<br>"
                "Then press OK to continue.";

        p_SelectDlg->SetRadioButtonVisible(false);
        p_SelectDlg->HideAbort();
    }

    p_SelectDlg->SetLableText(Text);
    if (p_SelectDlg->exec() == 0) {
        delete p_SelectDlg;
        return false;
    }

    Position = p_SelectDlg->GetBottleNumber();
    TubeFlag = (p_SelectDlg->GetOption() == 1);

    delete p_SelectDlg;
    return true;
}

bool CDrainReagentTest::BuildVacuum()
{
    float TargetPressure;
    int WaitTime;
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortDrainReagent");
    TargetPressure = p_TestCase->GetParameter("TargetPressure").toFloat();
    WaitTime = p_TestCase->GetParameter("TimeOut").toInt();

    QString Text = QString("Creating vacuum to %1Kpa...").arg(TargetPressure);
    ServiceDeviceProcess* p_Dev = ServiceDeviceProcess::Instance();

    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    int Ret = p_Dev->PumpBuildPressure(TargetPressure);

    if (Ret != RETURN_OK) {
        return false;
    }

    float CurrentPressure(0);
    while (WaitTime) {
        QTime EndTime = QTime().currentTime().addSecs(1);
        (void)p_Dev->PumpGetPressure(&CurrentPressure);
        if (CurrentPressure < TargetPressure) {
            mp_MessageDlg->HideWaitingDialog();
            return true;
        }

        WaitTime--;
        int MSec = QTime().currentTime().msecsTo(EndTime);
        p_Dev->Pause(MSec);
    }

    mp_MessageDlg->HideWaitingDialog();
    return false;
}

bool CDrainReagentTest::CheckRVTemp(bool IsParaffin)
{
    qreal RVTargetTemp1(0);
    qreal RVTargetTemp2(0);
    qreal RVCurrentTemp1(0);
    qreal RVCurrentTemp2(0);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortDrainReagent");
    if (IsParaffin) {
        RVTargetTemp1 = p_TestCase->GetParameter("RVPTargetTemp1").toInt();
        RVTargetTemp2 = p_TestCase->GetParameter("RVPTargetTemp2").toInt();
    }
    else {
        RVTargetTemp1 = p_TestCase->GetParameter("RVTargetTemp1").toInt();
        RVTargetTemp2 = p_TestCase->GetParameter("RVTargetTemp2").toInt();
    }

    int Ret = ServiceDeviceProcess::Instance()->RVGetTemp(&RVCurrentTemp1, &RVCurrentTemp2);
    if (Ret != RETURN_OK) {
        qDebug()<<"Retort Drain Reagent get rotary valve temperature failed.";
        return false;
    }

    return (RVCurrentTemp1 >= RVTargetTemp1) && (RVCurrentTemp2 >= RVTargetTemp2);
}

bool CDrainReagentTest::CheckLTubeTemp()
{
    qreal TargetTemp(0);
    qreal LTubeCurrentTemp(0);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SGlobal");
    TargetTemp = p_TestCase->GetParameter("PMeltingPoint").toInt();

    int Ret = ServiceDeviceProcess::Instance()->LiquidTubeGetTemp(&LTubeCurrentTemp);
    if (Ret != RETURN_OK) {
        qDebug()<<"Retort Drain Reagent get Liquid heating tube temperature failed.";
        return false;
    }

    return LTubeCurrentTemp >= TargetTemp;
}

bool CDrainReagentTest::CheckOvenTemp()
{
    qreal OvenTempT(0);
    qreal OvenTempB1(0);
    qreal OvenTempB2(0);

    qreal MeltingPointTemp(0);
    qreal TargetTemp(0);
    qreal DiffTemp(0);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SGlobal");
    DataManager::CTestCase* p_TestCase2 = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortDrainReagent");
    DiffTemp = p_TestCase2->GetParameter("OvenDiffTemp").toInt();
    MeltingPointTemp = p_TestCase->GetParameter("PMeltingPoint").toInt();
    TargetTemp = MeltingPointTemp - DiffTemp;

    int Ret = ServiceDeviceProcess::Instance()->OvenGetTemp(&OvenTempT, &OvenTempB1, &OvenTempB2);

    if (Ret != RETURN_OK) {
        qDebug()<<"Retort Drain Reagent get Oven temperature failed.";
        return false;
    }

    qreal MinTemp = OvenTempB1 < OvenTempB2 ? OvenTempB1 : OvenTempB2;
    MinTemp = MinTemp < OvenTempT ? MinTemp : OvenTempT;
    return MinTemp >= TargetTemp;
}

} // namespace Retort

} // namespace Diagnostics

