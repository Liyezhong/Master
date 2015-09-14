/****************************************************************************/
/*! \file LiquidHoseTestDlg.cpp
 *
 *  \brief Implementation of System Liquid hose test dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-10-21
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
#include "Diagnostics/Include/System/LiquidHoseTestDlg.h"

#include "ui_LiquidHoseTestDlg.h"

#include "Global/Include/Utils.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include <QDebug>

namespace Diagnostics {

namespace System {

CLiquidHoseTestDlg::CLiquidHoseTestDlg(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent):
    MainMenu::CDialogFrame(p_Parent),
    mp_Ui(new Ui::CLiquidHoseTestDlg),
    mp_MessageDlg(p_MessageDlg),
    m_MessageTitle("Liquid Hose Test"),
    m_Abort(false)
{
    mp_Ui->setupUi(GetContentFrame());

    this->setModal(true);
    this->SetDialogTitle(m_MessageTitle);

    InitLableGroup();

    mp_Ui->okBtn->setEnabled(false);

    CONNECTSIGNALSLOT(mp_Ui->okBtn, clicked(), this, accept());
    CONNECTSIGNALSLOT(mp_Ui->abortBtn, clicked(), this, OnAbort());
}

CLiquidHoseTestDlg::~CLiquidHoseTestDlg()
{
    try {
        delete mp_Ui;
        for(int i = 0; i< m_LabelGroups.count(); ++i) {
            delete m_LabelGroups.at(i);
        }
    }
    catch (...) {}
}

int CLiquidHoseTestDlg::RunTest()
{
    int BottleNumber;
    bool CreatePressureRet = true;
    float RecordPressure(0);
    QString MsgTitle = "Liquid Hose Test";
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SSystemLiquidHose");
    float TargetPressure = p_TestCase->GetParameter("TargetPressure").toFloat();
    int TimeOut          = p_TestCase->GetParameter("TimeOut").toInt();
    int DurationTime     = p_TestCase->GetParameter("KeepDuration").toInt();
    int RecordTime       = p_TestCase->GetParameter("RecordTime").toInt();
    qreal TargetTemp1    = p_TestCase->GetParameter("RVTargetTemp1").toFloat();
    qreal TargetTemp2    = p_TestCase->GetParameter("RVTargetTemp2").toFloat();
    qreal RVTempSensor1(0);
    qreal RVTempSensor2(0);

    p_DevProc->RVGetTemp(&RVTempSensor1, &RVTempSensor2);

    if (RVTempSensor1 < TargetTemp1 || RVTempSensor2 < TargetTemp2) {
        ShowFinishDlg(1);
        return RETURN_ERR_FAIL;
    }

    QString Text = "Rotary valve is initializing...";
    mp_MessageDlg->ShowWaitingDialog(m_MessageTitle, Text);
    int Ret = p_DevProc->RVInitialize();
    mp_MessageDlg->HideWaitingDialog();
    if (Ret != RETURN_OK) {
        ShowFinishDlg(2);
        return RETURN_ERR_FAIL;
    }

    this->show();
    if (m_BottleNumberList.count() > 1) {
        BottleNumber = 1;
    }
    else if(m_BottleNumberList.count() == 1) {
        BottleNumber = m_BottleNumberList.at(0);
    }
    mp_Ui->labelStatus->setText(QString("Rotary Valve is moving to sealing position %1").arg(BottleNumber));
    if (p_DevProc->RVMovePosition(false, BottleNumber) != RETURN_OK) {
        mp_MessageDlg->ShowRVMoveFailedDlg(m_MessageTitle);
        return RETURN_ERR_FAIL;
    }

    (void)p_DevProc->PumpSetFan(1);
    for (int i = 0; i < m_BottleNumberList.count(); ++i) {
        if (!CreatePressure(TargetPressure, TimeOut)) {
            CreatePressureRet = false;
            (void)p_DevProc->PumpStopCompressor();
            break;
        }
        (void)p_DevProc->PumpStopCompressor();
        if (m_Abort) {
            break;
        }

        BottleNumber = m_BottleNumberList.at(i);

        mp_Ui->labelStatus->setText(QString("Rotary Valve is moving to tube position %1").arg(BottleNumber));
        if (p_DevProc->RVMovePosition(true, BottleNumber) != RETURN_OK) {
            mp_MessageDlg->ShowRVMoveFailedDlg(m_MessageTitle);
            return RETURN_ERR_FAIL;
        }

        mp_Ui->labelStatus->setText(QString("Keep pressure for %1 seconds").arg(DurationTime));
        for (int j = 0; j < DurationTime; ++j) {
            p_DevProc->Pause(1000);
            if (m_Abort) {
                break;
            }
        }

        if (m_Abort) {
            break;
        }

        mp_Ui->labelStatus->setText("Record Pressure value");
        RecordPressure = GetRecordPressure(RecordTime);
        if (m_Abort) {
            break;
        }

        UpdateLabel(i, BottleNumber, RecordPressure);

        if (i == m_BottleNumberList.count()) {
            break;
        }
        mp_Ui->labelStatus->setText(QString("Rotary Valve is moving to sealing position %1").arg(BottleNumber));
        if (p_DevProc->RVMovePosition(false, BottleNumber) != RETURN_OK) {
            mp_MessageDlg->ShowRVMoveFailedDlg(m_MessageTitle);
            return RETURN_ERR_FAIL;
        }
    }

    Text = "Releasing pressure...";
    if (m_Abort) {
        mp_MessageDlg->ShowWaitingDialog(MsgTitle, Text);
    }
    else {
        mp_Ui->labelStatus->setText(Text);
    }
    (void)p_DevProc->PumpSetFan(0);
    (void)p_DevProc->PumpSetValve(0, 0);
    (void)p_DevProc->PumpSetValve(1, 0);
    p_DevProc->Pause(20*1000);

    mp_Ui->abortBtn->setEnabled(false);
    mp_Ui->okBtn->setEnabled(true);
    m_BottleNumberList.clear();

    if (m_Abort) {
        mp_MessageDlg->HideWaitingDialog();
        return RETURN_ABORT;
    }

    mp_Ui->labelStatus->setText("Liquid Hose Test finished.");

    if (!CreatePressureRet) {       
        ShowFinishDlg(3);
    }

    (void)this->exec();

    return CreatePressureRet ? RETURN_OK : RETURN_ERR_FAIL;
}

void CLiquidHoseTestDlg::AddBottleNumber(int BottleNumber)
{
    m_BottleNumberList.append(BottleNumber);
}

void CLiquidHoseTestDlg::OnAbort()
{
    m_Abort = true;
    reject();
}

bool CLiquidHoseTestDlg::CreatePressure(float TargetPressure, int TimeOut)
{
    bool Ret = false;
    mp_Ui->labelStatus->setText(QString("Creating pressure to %1Kpa...").arg(TargetPressure));
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    int PreRet = p_DevProc->PumpBuildPressure(TargetPressure);
    if (PreRet == RETURN_OK) {
        float CurrentPressure(0);
        while (TimeOut) {
            QTime EndTime = QTime().currentTime().addSecs(1);
            (void)p_DevProc->PumpGetPressure(&CurrentPressure);
            qDebug()<<"Liquid hose test current pressure:"<<CurrentPressure;
            if (m_Abort || CurrentPressure >= TargetPressure) {
                Ret = true;
                break;
            }

            TimeOut--;
            int MSec = QTime().currentTime().msecsTo(EndTime);

            if (MSec <= 0) {
                MSec = 1000;
            }
            p_DevProc->Pause(MSec);
        }
    }

    return Ret;
}

float CLiquidHoseTestDlg::GetRecordPressure(int RecordTime)
{
    float Pressure(0.0);
    float PressureSum(0.0);

    int WaitSec = RecordTime;
    while (WaitSec) {
        if (m_Abort) {
            break;
        }
        QTime EndTime = QTime().currentTime().addSecs(1);
        (void)ServiceDeviceProcess::Instance()->PumpGetPressure(&Pressure);

        PressureSum += Pressure;
        WaitSec--;
        int MSec = QTime().currentTime().msecsTo(EndTime);

        if (MSec <= 0) {
            MSec = 1000;
        }
        ServiceDeviceProcess::Instance()->Pause(MSec);
    }

    return PressureSum/RecordTime;
}

bool CLiquidHoseTestDlg::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(p_Object, p_Event);
    }
}

void CLiquidHoseTestDlg::UpdateLabel(int Index, int BottleNumber, float Pressure)
{
    LabelGroup* Labels = m_LabelGroups.at(Index);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SSystemLiquidHose");
    qreal PressureLeak(0);
    qreal PressureRestriction(0);

    QString BottleStr;
    QString ResultStr;
    QString PressureStr;
    if (BottleNumber == 14) {
        BottleStr = "P1";
    }
    else if (BottleNumber == 15) {
        BottleStr = "P2";
    }
    else if (BottleNumber == 16) {
        BottleStr = "P3";
    }
    else {
        BottleStr = QString::number(BottleNumber);
    }

    if (BottleStr.at(0) == 'P') {
        PressureLeak = p_TestCase->GetParameter("PressureLeakP").toFloat();
        PressureRestriction = p_TestCase->GetParameter("PressureRestrictionP").toFloat();
    }
    else {
        PressureLeak = p_TestCase->GetParameter("PressureLeak").toFloat();
        PressureRestriction = p_TestCase->GetParameter("PressureRestriction").toFloat();
    }

    if (Pressure < PressureLeak) {
        ResultStr = "Leak";
    }
    else if (Pressure > PressureRestriction) {
        ResultStr = "Restriction";
    }
    else {
        ResultStr = "Ok";
    }
    PressureStr = QString::number(Pressure);

    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_LIQUIDHOSE_TEST_POSITION,
                                               Global::tTranslatableStringList()<<BottleStr<<PressureStr<<ResultStr);

    Labels->bottleLabel->setText(BottleStr);
    Labels->pressureLabel->setText(PressureStr);
    Labels->resultLabel->setText(ResultStr);
}

void CLiquidHoseTestDlg::InitLableGroup()
{
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle1, mp_Ui->pressure1, mp_Ui->result1));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle2, mp_Ui->pressure2, mp_Ui->result2));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle3, mp_Ui->pressure3, mp_Ui->result3));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle4, mp_Ui->pressure4, mp_Ui->result4));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle5, mp_Ui->pressure5, mp_Ui->result5));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle6, mp_Ui->pressure6, mp_Ui->result6));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle7, mp_Ui->pressure7, mp_Ui->result7));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle8, mp_Ui->pressure8, mp_Ui->result8));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle9, mp_Ui->pressure9, mp_Ui->result9));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle10, mp_Ui->pressure10, mp_Ui->result10));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle11, mp_Ui->pressure11, mp_Ui->result11));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle12, mp_Ui->pressure12, mp_Ui->result12));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle13, mp_Ui->pressure13, mp_Ui->result13));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle14, mp_Ui->pressure14, mp_Ui->result14));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle15, mp_Ui->pressure15, mp_Ui->result15));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->bottle16, mp_Ui->pressure16, mp_Ui->result16));
}

void CLiquidHoseTestDlg::ShowFinishDlg(int RetNum)
{
    QString Text;
    if (RetNum == 1) {
        Text = " Liquid Hose Test failed.<br>" \
                "Rotary Valve cannot rotate, due to the minimum " \
                "temperature has not been reached. Please check " \
                "resistance of temperature sensors, current of heating " \
                "element and function of ASB3. If no root cause " \
                "found, check main relay on ASB15 and cable " \
                "connections in addition. Exchange part accordingly";
    }
    else if (RetNum == 2) {
        Text = "Liquid Hose Test failed.<br>" \
                "It might work in some minutes when " \
                "solidified paraffin in the rotary valve is " \
                "molten. Repeat initializing test again in " \
                "about 15mins. If it still fails in the second " \
                "try, exchange rotary valve, reboot the " \
                "Service Software and repeat this test.";
    }
    else if (RetNum == 3) {
        Text  = "Liquid Hose Test failed.<br>" \
                "Target pressure could not be reached. Please " \
                "perform System Sealing Test to diagnose the reason. Then repeat this test.";
    }
    mp_MessageDlg->ShowMessage(m_MessageTitle, Text, RETURN_ERR_FAIL);
}

} // namespace System

} // namespace Diagnostics
