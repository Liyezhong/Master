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
#include <QDebug>

namespace Diagnostics {

namespace System {

CLiquidHoseTestDlg::CLiquidHoseTestDlg(QWidget *p_Parent):
    MainMenu::CDialogFrame(p_Parent),
    mp_Ui(new Ui::CLiquidHoseTestDlg),
    m_Abort(false)
{
    mp_Ui->setupUi(GetContentFrame());

    this->setModal(true);
    this->SetDialogTitle("Liquid Hose Test");

    InitLableGroup();

    mp_Ui->okBtn->setEnabled(false);

    CONNECTSIGNALSLOT(mp_Ui->okBtn, clicked(), this, OnOk());
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

void CLiquidHoseTestDlg::RunTest()
{
    int BottleNumber;
    bool CreatePressureRet = true;
    float RecordPressure(0);
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SSystemLiquidHose");
    float TargetPressure = p_TestCase->GetParameter("TargetPressure").toFloat();
    int TimeOut          = p_TestCase->GetParameter("TimeOut").toInt();
    int DurationTime     = p_TestCase->GetParameter("KeepDuration").toInt();
    int RecordTime       = p_TestCase->GetParameter("RecordTime").toInt();

    this->show();
    mp_Ui->labelStatus->setText("Rotary Valve is moving to sealing position 1");
    p_DevProc->RVMovePosition(false, 1);
    (void)p_DevProc->PumpSetFan(1);
    for (int i = 0; i < m_BottleNumberList.count(); ++i) {
        if (!CreatePressure(TargetPressure, TimeOut)) {
            CreatePressureRet = false;
            break;
        }
        (void)p_DevProc->PumpStopCompressor();
        if (m_Abort) {
            break;
        }

        BottleNumber = m_BottleNumberList.at(i);

        mp_Ui->labelStatus->setText(QString("Rotary Valve is moving to tube position %1").arg(BottleNumber));
        (void)p_DevProc->RVMovePosition(true, BottleNumber);

        mp_Ui->labelStatus->setText(QString("Keep pressure for %1 seconds").arg(DurationTime));
        p_DevProc->Pause(DurationTime*1000);

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
        (void)p_DevProc->RVMovePosition(false, BottleNumber);
    }

    mp_Ui->labelStatus->setText("Releasing pressure...");
    (void)p_DevProc->PumpSetFan(0);
    (void)p_DevProc->PumpSetValve(1, 0);
    (void)p_DevProc->PumpSetValve(2, 0);
    p_DevProc->Pause(20*1000);

    mp_Ui->abortBtn->setEnabled(false);
    mp_Ui->okBtn->setEnabled(true);
    m_BottleNumberList.clear();

    mp_Ui->labelStatus->setText("Liquid Hose Test finished.");

    if (m_Abort) {
        delete this;
    }
    else if (!CreatePressureRet) {
        QString Title = "Liquid Hose Test";
        QString Text  = "Liquid Hose Test failed.<br>"
                "Target pressure could not be reached. Please "
                "perform System Sealing Test to diagnose the reason. Then repeat this test";
        CDiagnosticMessageDlg* p_Dlg = new CDiagnosticMessageDlg(this);
        p_Dlg->ShowMessage(Title, Text, RETURN_ERR_FAIL);
        delete p_Dlg;
    }    
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

void CLiquidHoseTestDlg::OnOk()
{
    accept();
    delete this;
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
            (void)p_DevProc->PumpGetPressure(&CurrentPressure);
            qDebug()<<"Liquid hose test current pressure:"<<CurrentPressure;
            if (m_Abort || CurrentPressure >= TargetPressure) {
                Ret = true;
                break;
            }
            p_DevProc->Pause(1000);
            TimeOut--;
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
        ServiceDeviceProcess::Instance()->PumpGetPressure(&Pressure);
        ServiceDeviceProcess::Instance()->Pause(1000);
        PressureSum += Pressure;
        WaitSec--;
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

    QString BottleStr;
    QString ResultStr;
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

    if (Pressure < 0.5) {
        ResultStr = "Leak";
    }
    else if (Pressure > 10) {
        ResultStr = "Restriction";
    }
    else {
        ResultStr = "Ok";
    }

    Labels->bottleLabel->setText(BottleStr);
    Labels->pressureLabel->setText(QString::number(Pressure));
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

} // namespace System

} // namespace Diagnostics
