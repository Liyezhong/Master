/****************************************************************************/
/*! \file SVCDashboardWidget.cpp
 *
 *  \brief Implementation of SVC Diagnostics dashboard widget.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-02
 *   $Author:  $ Abe Yang
 *
 *   \b Company:
 *
 * 		 Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include "SVCDiagnostics/Include/SVCDashboardWidget.h"
#include "ui_SVCDashboardWidget.h"
#include <QPainter>
#include <Global/Include/Utils.h>
#include <QGraphicsScene>
#include "SVCDiagnostics/Include/GraphicsItemPart.h"
#include "SVCDiagnostics/Include/SVCButton.h"
#include "SVCDiagnostics/Include/SVCLabel.h"
#include "SVCDiagnostics/Include/SVCTargetTempSelectionDlg.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "SVCDiagnostics/Include/SVCRotaryValveDlg.h"
#include "SVCDiagnostics/Include/SVCValveStateInfoDlg.h"

#include "Diagnostics/Include/SelectBottleNReagentDialog.h"


using namespace SVCDiagnostics;

CSVCDashboardWidget::CSVCDashboardWidget(QWidget *p_Parent) :
    QWidget(p_Parent),
    mp_Ui(new Ui::CSVCDashboardWidget)
{
    mp_Ui->setupUi(this);
    mp_Ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    mp_Ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    //ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    mp_Ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    mp_Scene = new QGraphicsScene(mp_Ui->graphicsView);
    mp_Scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    mp_Scene->setSceneRect(0, 0, 772, 490);
    mp_Scene->setBackgroundBrush(QImage(":/Images/SVCDiagnosticsBackground.png"));
    mp_Ui->graphicsView->setScene(mp_Scene);

    mp_MsgDlg = new Diagnostics::CDiagnosticMessageDlg(this);
    InitLabel();

    mp_Retort = CreatePart("Retort", QPoint(406, 30));
    mp_Oven = CreatePart("Oven", QPoint(7, 70));
    mp_RotaryValve = CreatePart("RotaryValve", QPoint(220, 152));

    mp_AirHeatingTube = CreatePart("AirHeatingTube", QPoint(489, 50));
    mp_HeatingTube = CreatePart("HeatingTube", QPoint(250, 116));

    mp_Fan = CreatePart("Fan", QPoint(266, 20));
    mp_WaxTrap = CreatePart("WaxTrap", QPoint(666, 109), false);
    mp_Pump = CreatePart("Pump", QPoint(404, 288));
    mp_GV1 = CreatePart("GV1", QPoint(485, 192));
    mp_GV2 = CreatePart("GV2", QPoint(640, 245));

    mp_Pressure = CreatePart("Pressure", QPoint(610,70), false);
    mp_RFV1 = CreatePart("RFV1", QPoint(596, 154), false);

    mp_Filter = CreatePart("Filter", QPoint(269, 80), false);
    mp_Connect1 = CreatePart("ConnectPart", QPoint(500, 256), false);
    mp_Connect2 = CreatePart("ConnectPart", QPoint(569, 256), false);
    mp_Connect3 = CreatePart("ConnectPart", QPoint(517, 360), false);
    mp_Line1    = CreatePart("Line1", QPoint(10, 100), false);
    mp_Line2    = CreatePart("Line2", QPoint(434, 76), false);
    mp_Line3    = CreatePart("Line3", QPoint(227, 274), false);

    mp_GV1StateUp    = CreatePart("ValveState", QPoint(512, 167), false);
    mp_GV1StateLeft  = CreatePart("ValveState", QPoint(462, 220), false);
    mp_GV1StateRight = CreatePart("ValveState", QPoint(564, 200), false);
    mp_GV2StateUp    = CreatePart("ValveState", QPoint(658, 236), false);
    mp_GV2StateLeft  = CreatePart("ValveState", QPoint(637, 291), false);
    mp_GV2StateRight = CreatePart("ValveState", QPoint(720, 251), false);

    mp_SelectBtn    = new SVCButton(false, mp_Ui->graphicsView);
    mp_ValveInfoBtn = new SVCButton(true, mp_Ui->graphicsView);
    mp_SelectBtn->setText("Select Position");
    mp_SelectBtn->setPos(137,238);
    mp_ValveInfoBtn->setText("Valve State Info");
    mp_ValveInfoBtn->setPos(400, 420);

    mp_RefreshTimer = new QTimer;
    mp_RefreshTimer->setSingleShot(false);
    mp_RefreshTimer->setInterval(1000);
    CONNECTSIGNALSLOT(mp_RefreshTimer, timeout(), this, RefreshLabel());

    CONNECTSIGNALSLOT(mp_Retort, PartSelected(), this, RetortSelected());
    CONNECTSIGNALSLOT(mp_Oven, PartSelected(), this, OvenSelected());
    CONNECTSIGNALSLOT(mp_RotaryValve, PartSelected(), this, RotaryValveSelected());
    CONNECTSIGNALSLOT(mp_AirHeatingTube, PartSelected(), this, AirTubeSelected());
    CONNECTSIGNALSLOT(mp_HeatingTube, PartSelected(), this, LiquidTubeSelected());
    CONNECTSIGNALSLOT(mp_Fan, PartSelected(), this, FanSelected());
    CONNECTSIGNALSLOT(mp_GV1, PartSelected(), this, Valve1Selected());
    CONNECTSIGNALSLOT(mp_GV2, PartSelected(), this, Valve2Selected());
    CONNECTSIGNALSLOT(mp_Pump, PartSelected(), this, PumpSelected());

    CONNECTSIGNALSLOT(mp_SelectBtn, clicked(), this, OnSelectPosition());
    CONNECTSIGNALSLOT(mp_ValveInfoBtn, clicked(), this, OnValveStateInfo());
}

CSVCDashboardWidget::~CSVCDashboardWidget()
{
    try {        
        delete mp_RefreshTimer;
        delete mp_Ui;
        delete mp_Scene;
        delete mp_MsgDlg;
        delete mp_SelectBtn;
        delete mp_ValveInfoBtn;
        delete mp_HeatingTubeTemp;
        delete mp_AirHeatingTubeCurrent;
        delete mp_AirHeatingTubeTemp;

        delete mp_OvenTemp1;
        delete mp_OvenTemp2;
        delete mp_OvenTemp3;
        delete mp_OvenCurrent;

        delete mp_RetortTemp1;
        delete mp_RetortTemp2;
        delete mp_RetortTemp3;
        delete mp_RetortCurrent;

        delete mp_RotaryValvePosition;
        delete mp_RotaryValveTemp1;
        delete mp_RotaryValveTemp2;
        delete mp_RotaryValveCurrent;

        delete mp_PressureLabel;
    }
    catch (...) {

    }
}

void CSVCDashboardWidget::InitLabel()
{
    mp_HeatingTubeTemp    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_HeatingTubeCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_HeatingTubeTemp->setPos(358, 126);
    mp_HeatingTubeCurrent->setPos(358, 147);
    mp_HeatingTubeTemp->setText(" Temp: 0\260C");
    mp_HeatingTubeCurrent->setText(" Current: 0A");

    mp_AirHeatingTubeTemp    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTubeCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTubeTemp->setPos(596, 31);
    mp_AirHeatingTubeCurrent->setPos(596, 44);
    mp_AirHeatingTubeTemp->setText(" Temp: 0\260C");
    mp_AirHeatingTubeCurrent->setText(" Current : 0A");

    mp_OvenTemp1   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp2   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp3   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp1->setPos(17, 183);
    mp_OvenTemp2->setPos(17, 196);
    mp_OvenTemp3->setPos(17, 209);
    mp_OvenCurrent->setPos(17, 222);
    mp_OvenTemp1->setText(" Oven1: 0\260C");
    mp_OvenTemp2->setText(" Oven2: 0\260C");
    mp_OvenTemp3->setText(" Oven3: 0\260C");
    mp_OvenCurrent->setText(" Current: 0A");

    mp_RetortTemp1 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp2 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp3 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp1->setPos(504, 68);
    mp_RetortTemp2->setPos(504, 81);
    mp_RetortTemp3->setPos(504, 94);
    mp_RetortCurrent->setPos(504, 116);
    mp_RetortTemp1->setText("Retort_Temperature1");
    mp_RetortTemp2->setText("Retort_Temperature2");
    mp_RetortTemp3->setText("Retort_Temperature3");
    mp_RetortCurrent->setText(" Current: 0A");

    mp_RotaryValvePosition = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveTemp1    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveTemp2    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveCurrent  = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValvePosition->setPos(137, 183);
    mp_RotaryValveTemp1->setPos(137, 196);
    mp_RotaryValveTemp2->setPos(137, 209);
    mp_RotaryValveCurrent->setPos(137, 222);
    mp_RotaryValvePosition->setText(" Position: Unknow");
    mp_RotaryValveTemp1->setText(" Temp1: 0\260C");
    mp_RotaryValveTemp2->setText(" Temp1: 0\260C");
    mp_RotaryValveCurrent->setText(" Current: 0A");

    mp_PressureLabel = new SVCLabel(false, mp_Ui->graphicsView);
    mp_PressureLabel->setPos(636, 100);
    mp_PressureLabel->setText(" 0kPa");
}

CGraphicsItemPart* CSVCDashboardWidget::CreatePart(const QString& partResName, const QPoint& _pos, bool Clickable)
{
    QString normalImg = ":/Images/" + partResName + ".png";
    CGraphicsItemPart* pGraphicsItemPart = NULL;
    //if (Clickable) {
        QString disabledImg = ":/Images/" + partResName + "Disabled.png";
        QString glowImg = ":/Images/" + partResName + "Glow.png";
        pGraphicsItemPart = new CGraphicsItemPart(QPixmap(normalImg),
                                                QPixmap(disabledImg),
                                                QPixmap(glowImg), Clickable);
//    }
//    else {
//        pGraphicsItemPart = new CGraphicsItemPart(QPixmap(normalImg));
//    }
    mp_Scene->addItem(pGraphicsItemPart);
    pGraphicsItemPart->setPos(_pos);
    return pGraphicsItemPart;
}

void CSVCDashboardWidget::paintEvent(QPaintEvent* pPaintEvent)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    //style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(pPaintEvent);
}

void CSVCDashboardWidget::RetortSelected()
{
    qDebug()<<"retort selected.";

    CGraphicsItemPart::PartStatus Status = mp_Retort->Status();

    if (Status == CGraphicsItemPart::Working) {
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortPreTest");
        int DefTarget = p_TestCase->GetParameter("RetortTargetTemp").toInt();
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(DefTarget, 40, 120, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Retort Target Temperature");
        (void)p_TempSelectionDlg->exec();

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->RetortStartHeating(TargetTemp+7, TargetTemp+2);
        delete p_TempSelectionDlg;

        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Retort", "Retort start heating failed.");
            mp_Retort->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->RetortStopHeating() !=(int) Diagnostics::RETURN_OK) {
            ShowFailedDlg("Retort", "Retort stop heating failed.");
            mp_Retort->SetStatus(CGraphicsItemPart::Working);
        }
    }

    //qDebug()<<"get Retort status:"<<Status;
}

void CSVCDashboardWidget::OvenSelected()
{
    qDebug()<<"Oven selected.";

    CGraphicsItemPart::PartStatus Status = mp_Oven->Status();

    if (Status == CGraphicsItemPart::Working) {
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SOvenPreTest");
        int DefTarget = p_TestCase->GetParameter("OvenTopTargetTemp").toInt();
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(DefTarget, 40, 120, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Paraffin Oven Target Temperature");
        (void)p_TempSelectionDlg->exec();

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->OvenStartHeating(TargetTemp, TargetTemp);
        delete p_TempSelectionDlg;
        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Oven", "Oven start heating failed.");
            mp_Oven->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->OvenStopHeating() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Oven", "Oven stop heating failed.");
            mp_Oven->SetStatus(CGraphicsItemPart::Working);
        }
    }

    //qDebug()<<"get Oven status:"<<Status;
}

void CSVCDashboardWidget::RotaryValveSelected()
{
    qDebug()<<"Rotary Valve selected.";

    CGraphicsItemPart::PartStatus Status = mp_RotaryValve->Status();

    if (Status == CGraphicsItemPart::Working) {
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRVPreTest");
        int DefTarget = p_TestCase->GetParameter("RVTargetTemp").toInt();
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(DefTarget, 40, 120, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Rotary Valve Target Temperature");
        (void)p_TempSelectionDlg->exec();

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->RVStartHeating(TargetTemp);
        delete p_TempSelectionDlg;
        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Rotary Valve", "Rotary valve start heating failed.");
            mp_RotaryValve->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->RVStopHeating() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Rotary Valve", "Rotary valve stop heating failed.");
            mp_RotaryValve->SetStatus(CGraphicsItemPart::Working);
        }
    }

    //qDebug()<<"get RotaryValve status:"<<Status;
}

void CSVCDashboardWidget::AirTubeSelected()
{
    qDebug()<<"Air Heating Tube selected.";

    CGraphicsItemPart::PartStatus Status = mp_AirHeatingTube->Status();

    if (Status == CGraphicsItemPart::Working) {
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SLTubePreTest");
        int DefTarget = p_TestCase->GetParameter("LTubeTargetTemp").toInt();
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(DefTarget, 40, 120, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Air Heating Tube Target Temperature");
        (void)p_TempSelectionDlg->exec();

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->AirTubeStartHeating(TargetTemp);
        delete p_TempSelectionDlg;

        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Air heating tube", "Air heating tube start heating failed.");
            mp_AirHeatingTube->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->AirTubeStopHeating() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Air Heating Tube", "Air heating tube stop heating failed.");
            mp_AirHeatingTube->SetStatus(CGraphicsItemPart::Working);
        }
    }

    //qDebug()<<"get AirHeatingTube status:"<<Status;
}

void CSVCDashboardWidget::LiquidTubeSelected()
{
    qDebug()<<"Liquid Heating Tube selected.";

    CGraphicsItemPart::PartStatus Status = mp_HeatingTube->Status();

    if (Status == CGraphicsItemPart::Working) {
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SLTubePreTest");
        int DefTarget = p_TestCase->GetParameter("LTubeTargetTemp").toInt();
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(DefTarget, 40, 120, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Liquid Heating Tube Target Temperature");
        (void)p_TempSelectionDlg->exec();

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->LiquidTubeStartHeating(TargetTemp);
        delete p_TempSelectionDlg;
        qDebug()<<"Liquid tube heating result:"<<Ret;
        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Liquid Heating Tube", "Liquid heating tube start heating failed.");
            mp_HeatingTube->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->LiquidTubeStopHeating() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Liquid Heating Tube", "Liquid heating tube stop heating failed.");
            mp_HeatingTube->SetStatus(CGraphicsItemPart::Working);
        }
    }

    //qDebug()<<"get LiquidHeatingTube status:"<<Status;
}

void CSVCDashboardWidget::FanSelected()
{
    qDebug()<<"Fan selected.";

    CGraphicsItemPart::PartStatus Status = mp_Fan->Status();

    if (Status == CGraphicsItemPart::Working) {
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetFan(1) != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Exhaust Fan", "Exhaust fan open failed.");
            mp_Fan->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetFan(0) != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Exhaust Fan", "Exhaust fan close failed.");
            mp_Fan->SetStatus(CGraphicsItemPart::Working);
        }
    }
}

void CSVCDashboardWidget::Valve1Selected()
{
    qDebug()<<"Valve1 selected.";

    CGraphicsItemPart::PartStatus Status = mp_GV1->Status();

    if (Status == CGraphicsItemPart::Working) {
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetValve(0, 1) == (int)Diagnostics::RETURN_OK) {
            //mp_GV1StateUp->SetStatus(CGraphicsItemPart::Normal);
            mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Normal);
            mp_GV1StateRight->SetStatus(CGraphicsItemPart::Disabled);
        }
        else {
            ShowFailedDlg("Valve1", "Valve1 open failed");
            mp_GV1->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetValve(0, 0) == (int)Diagnostics::RETURN_OK) {
            //mp_GV1StateUp->SetStatus(CGraphicsItemPart::Normal);
            mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Disabled);
            mp_GV1StateRight->SetStatus(CGraphicsItemPart::Normal);
        }
        else {
            ShowFailedDlg("Valve1", "Valve1 close failed");
            mp_GV1->SetStatus(CGraphicsItemPart::Working);
        }
    }
}

void CSVCDashboardWidget::Valve2Selected()
{
    qDebug()<<"Valve1 selected.";

    CGraphicsItemPart::PartStatus Status = mp_GV2->Status();

    if (Status == CGraphicsItemPart::Working) {
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetValve(1, 1) == (int)Diagnostics::RETURN_OK) {
            //mp_GV2StateUp->SetStatus(CGraphicsItemPart::Normal);
            mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Normal);
            mp_GV2StateRight->SetStatus(CGraphicsItemPart::Disabled);
        }
        else {
            ShowFailedDlg("Valve2", "Valve2 open failed");
            mp_GV2->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetValve(1, 0) == (int)Diagnostics::RETURN_OK) {
            //mp_GV2StateUp->SetStatus(CGraphicsItemPart::Normal);
            mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Disabled);
            mp_GV2StateRight->SetStatus(CGraphicsItemPart::Normal);
        }
        else {
            ShowFailedDlg("Valve2", "Valve2 close failed");
            mp_GV2->SetStatus(CGraphicsItemPart::Working);
        }
    }
}

void CSVCDashboardWidget::PumpSelected()
{
    qDebug()<<"Pump selected.";

    CGraphicsItemPart::PartStatus Status = mp_Pump->Status();

    if (Status == CGraphicsItemPart::Working) {
        int Ret = (int)Diagnostics::RETURN_OK;
        if (mp_GV2->Status() == CGraphicsItemPart::Working) {//valve 1 is Open
            qDebug()<<"To create pressure";
            Ret = Diagnostics::ServiceDeviceProcess::Instance()->PumpSetPressure(1, 30);//pressure
        }
        else if (mp_GV1->Status() == CGraphicsItemPart::Working) { //valve 2 is open
            qDebug()<<"To create vaccum";
            Ret = Diagnostics::ServiceDeviceProcess::Instance()->PumpSetPressure(9, -30);//vaccum
        }
        else {
            //To show warning dialog
            QString Title = tr("Pump");
            QString Text  = tr("Valve1&2 is off, cann't to create pressure.");
            mp_MsgDlg->ShowMessage(Title, Text, Diagnostics::RETURN_ERR_FAIL);
            mp_Pump->SetStatus(CGraphicsItemPart::Normal);
        }

        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Pump", "Pump open failed.");
            mp_Pump->SetStatus(CGraphicsItemPart::Normal);
        }
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpStopCompressor() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Pump", "Pump close failed.");
            mp_Pump->SetStatus(CGraphicsItemPart::Working);
        }
    }
}

void CSVCDashboardWidget::OnSelectPosition()
{
    QString Title = tr("Select Position");
    QString Text;
    bool CurrentTubeFlag = true;
    qint32 CurrentPosition(0);
    (void)Diagnostics::ServiceDeviceProcess::Instance()->RVGetPosition(&CurrentTubeFlag, &CurrentPosition);
    if (CurrentPosition == 0) {
        Text = "Can't get current position, Do you want to initialize position?";
        int Ret = mp_MsgDlg->ShowConfirmMessage(Title, Text, Diagnostics::CDiagnosticMessageDlg::OK_ABORT);

        if (Ret == (int)Diagnostics::CDiagnosticMessageDlg::ABORT) {
            return;
        }

        Text = "Rotary valve is initializing...";
        mp_MsgDlg->ShowWaitingDialog(Title, Text);
        Ret = Diagnostics::ServiceDeviceProcess::Instance()->RVInitialize();
        mp_MsgDlg->HideWaitingDialog();

        if (Ret != (int)Diagnostics::RETURN_OK) {
            Text = "Rotary valve initialize failed.";
            mp_MsgDlg->ShowMessage(Title, Text, Diagnostics::RETURN_ERR_FAIL);
            return;
        }

        CurrentTubeFlag = true;
        CurrentPosition = 1;
    }

    SVCDiagnostics::CSVCRotaryValveDlg RotaryValveDlg(this);
    RotaryValveDlg.SetDialogTitle(tr("Rotary valve dialog"));
    RotaryValveDlg.SetPos(CurrentTubeFlag, CurrentPosition);
    if (RotaryValveDlg.exec() == 0) {
        return;
    }

    bool Flag;
    qint32 Postion;
    RotaryValveDlg.GetPos(Flag, Postion);

    if ( Flag == CurrentTubeFlag && Postion == CurrentPosition) {
        return;
    }

    Text = QString("Rotating Rotary Valve to position %1").arg(PostionToStr(Flag, Postion));
    mp_MsgDlg->ShowWaitingDialog(Title, Text);
    int Ret = Diagnostics::ServiceDeviceProcess::Instance()->RVMovePosition(Flag, Postion);
    mp_MsgDlg->HideWaitingDialog();

    if (Ret != (int)Diagnostics::RETURN_OK) {
        mp_MsgDlg->ShowRVMoveFailedDlg(Title);
    }

    qDebug()<<"move position result :"<<Ret;
}

void CSVCDashboardWidget::OnValveStateInfo()
{  
    bool Valve1Flag = mp_GV1->Status() == CGraphicsItemPart::Working;
    bool Valve2Flag = mp_GV2->Status() == CGraphicsItemPart::Working;
    bool PumpFlag   = mp_Pump->Status() == CGraphicsItemPart::Working;
    CSVCValveStateInfoDlg* p_Dlg = new CSVCValveStateInfoDlg(PumpFlag, Valve1Flag, Valve2Flag, this);
    (void)p_Dlg->exec();

    delete p_Dlg;
}

void CSVCDashboardWidget::TimerStart(bool IsStart)
{
    if (IsStart) {
        mp_RefreshTimer->start();
    }
    else {
        mp_RefreshTimer->stop();
    }
}

void CSVCDashboardWidget::RefreshLabel()
{
    Diagnostics::ServiceDeviceProcess* p_DevProc = Diagnostics::ServiceDeviceProcess::Instance();
    qreal OvenTemp1(0);
    qreal OvenTemp2(0);
    qreal OvenTemp3(0);
    quint16 OvenCurrentT(0);
    quint16 OvenCurrentB(0);

    qreal RetortTemp1(0);
    qreal RetortTemp2(0);
    qreal RetortTemp3(0);
    quint16 RetortCurrentS(0);
    quint16 RetortCurrentB(0);

    bool TubeFlag(0);
    qint32 Position(0);
    qreal SensorTemp1(0);
    qreal SensorTemp2(0);
    quint16 RVCurrent(0);

    qreal AirTubeTemp(0);
    quint16 AirTubeCurrent(0);

    qreal LiquidTubeTemp(0);
    quint16 LiquidTubeCurrent(0);

    float Pressure(0);

    (void)p_DevProc->OvenGetTemp(&OvenTemp1, &OvenTemp2, &OvenTemp3);
    (void)p_DevProc->OvenGetCurrent(&OvenCurrentT, &OvenCurrentB);

    (void)p_DevProc->RetortGetTemp(&RetortTemp1, &RetortTemp2, &RetortTemp3);
    (void)p_DevProc->RetortGetCurrent(&RetortCurrentS, &RetortCurrentB);

    (void)p_DevProc->RVGetPosition(&TubeFlag, &Position);
    (void)p_DevProc->RVGetTemp(&SensorTemp1, &SensorTemp2);
    (void)p_DevProc->RVGetCurrent(&RVCurrent);

    (void)p_DevProc->AirTubeGetTemp(&AirTubeTemp);
    (void)p_DevProc->AirTubeGetCurrent(&AirTubeCurrent);

    (void)p_DevProc->LiquidTubeGetTemp(&LiquidTubeTemp);
    (void)p_DevProc->LiquidTubeGetCurrent(&LiquidTubeCurrent);

    (void)p_DevProc->PumpGetPressure(&Pressure);

    UpdateOvenLabel(OvenTemp1, OvenTemp2, OvenTemp3, OvenCurrentT);
    UpdateRetortLabel(RetortTemp1, RetortTemp2, RetortTemp3, RetortCurrentS);
    UpdateRotaryValveLabel(TubeFlag, Position, SensorTemp1, SensorTemp2, RVCurrent);
    UpdateAirHeatingTubeLabel(AirTubeTemp, AirTubeCurrent);
    UpdateLiquidHeatingTubeLabel(LiquidTubeTemp, LiquidTubeCurrent);
    UpdatePressureLabel(Pressure);
}

void CSVCDashboardWidget::UpdatePartStatus()
{
    bool StatusIsOn = false;
    int Ret(0);
    Diagnostics::ServiceDeviceProcess* p_DevProc = Diagnostics::ServiceDeviceProcess::Instance();

    Ret = p_DevProc->RetortTempControlIsOn(&StatusIsOn);
    qDebug()<<"retort temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_Retort->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));

    Ret = p_DevProc->OvenTempControlIsOn(&StatusIsOn);
    qDebug()<<"Oven temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_Oven->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));

    Ret = p_DevProc->RVTempControlIsOn(&StatusIsOn);
    qDebug()<<"rotary valve temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_RotaryValve->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));

    Ret = p_DevProc->AirTubeTempControlIsOn(&StatusIsOn);
    qDebug()<<"air tube temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_AirHeatingTube->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));

    Ret = p_DevProc->LiquidTubeTempControlIsOn(&StatusIsOn);
    qDebug()<<"liquid tube temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_HeatingTube->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));

    mp_Fan->SetStatus(p_DevProc->PumpGetFan() ? CGraphicsItemPart::Working : CGraphicsItemPart::Normal);

    quint8 Value;
    p_DevProc->PumpGetValve(0, Value);
    if (Value == 1) {
        mp_GV1->SetStatus(CGraphicsItemPart::Working);
        mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Normal);
        mp_GV1StateRight->SetStatus(CGraphicsItemPart::Disabled);
    } else if (Value == 0) {
        mp_GV1->SetStatus(CGraphicsItemPart::Normal);
        mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV1StateRight->SetStatus(CGraphicsItemPart::Normal);
    }

    p_DevProc->PumpGetValve(1, Value);
    if (Value == 1) {
        mp_GV2->SetStatus(CGraphicsItemPart::Working);
        mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Normal);
        mp_GV2StateRight->SetStatus(CGraphicsItemPart::Disabled);
    } else if (Value == 0) {
        mp_GV2->SetStatus(CGraphicsItemPart::Normal);
        mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV2StateRight->SetStatus(CGraphicsItemPart::Normal);
    }

    mp_Pump->SetStatus(p_DevProc->PumpGetStatus() ? CGraphicsItemPart::Working : CGraphicsItemPart::Normal);
}

void CSVCDashboardWidget::UpdateOvenLabel(qreal OvenTemp1, qreal OvenTemp2, qreal OvenTemp3, qreal Current)
{
    mp_OvenTemp1->setText(QString(" Oven1: %1\260C").arg(OvenTemp1));
    mp_OvenTemp2->setText(QString(" Oven2: %1\260C").arg(OvenTemp2));
    mp_OvenTemp3->setText(QString(" Oven3: %1\260C").arg(OvenTemp3));
    mp_OvenCurrent->setText(QString(" Current: %1mA").arg(Current));
}

void CSVCDashboardWidget::UpdateRetortLabel(qreal RetortTemp1, qreal RetortTemp2, qreal RetortTemp3, qreal Current)
{
    mp_RetortTemp1->setText(QString(" Retort1: %1\260C").arg(RetortTemp1));
    mp_RetortTemp2->setText(QString(" Retort2: %1\260C").arg(RetortTemp2));
    mp_RetortTemp3->setText(QString(" Retort3: %1\260C").arg(RetortTemp3));
    mp_RetortCurrent->setText(QString(" Current: %1mA").arg(Current));
}

void CSVCDashboardWidget::UpdateRotaryValveLabel(bool RVTubeFlag, qreal RVPosition, qreal RVTemp1, qreal RVTemp2, qreal RVCurrent)
{
    QString PositionStr = PostionToStr(RVTubeFlag, RVPosition);

    mp_RotaryValvePosition->setText(QString(" Position: %1").arg(PositionStr));
    mp_RotaryValveTemp1->setText(QString(" Temp1: %1\260C").arg(RVTemp1));
    mp_RotaryValveTemp2->setText(QString(" Temp2: %1\260C").arg(RVTemp2));
    mp_RotaryValveCurrent->setText(QString(" Current: %1mA").arg(RVCurrent));
}

QString CSVCDashboardWidget::PostionToStr(bool TubeFlag, qreal Position)
{   
    if (Position == 0) {
        return QString("Unknow");
    }

    QString PositionStr;
    if (!TubeFlag) {
        PositionStr = "X";
    }

    if (Position == 14) {
        PositionStr += "P1";
    }
    else if (Position == 15) {
        PositionStr += "P2";
    }
    else if (Position == 16) {
        PositionStr += "P3";
    }
    else {
        PositionStr += QString("%1").arg(Position);
    }

    return PositionStr;
}

void CSVCDashboardWidget::ShowFailedDlg(const QString& Title, const QString& Text)
{
    mp_MsgDlg->ShowMessage(Title, Text, Diagnostics::RETURN_ERR_FAIL);
}

void CSVCDashboardWidget::UpdateAirHeatingTubeLabel(qreal Temp, qreal Current)
{
    mp_AirHeatingTubeTemp->setText(QString(" Temp: %1\260C").arg(Temp));
    mp_AirHeatingTubeCurrent->setText(QString(" Current: %1mA").arg(Current));
}

void CSVCDashboardWidget::UpdateLiquidHeatingTubeLabel(qreal Temp, qreal Current)
{
    mp_HeatingTubeTemp->setText(QString(" Temp: %1\260C").arg(Temp));
    mp_HeatingTubeCurrent->setText(QString(" Current: %1mA").arg(Current));
}

void CSVCDashboardWidget::UpdatePressureLabel(qreal Pressure)
{
    mp_PressureLabel->setText(QString(" %1kPa").arg(Pressure));
}
