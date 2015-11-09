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
#include "Main/Include/HimalayaServiceEventCodes.h"

//lint -e539
using namespace SVCDiagnostics;

CSVCDashboardWidget::CSVCDashboardWidget(QWidget *p_Parent) :
    QWidget(p_Parent),
    mp_Ui(new Ui::CSVCDashboardWidget),
    m_OvenTargetTempValue(-1),
    m_RetortTargetTempValue(-1),
    m_RVTargetTempValue(-1),
    m_LiquidTubeTempValue(-1),
    m_AirTubeTempValue(-1),
    m_PumpTestStatus(0)
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

    mp_Retort = CreatePart("Retort", QPoint(404, 30));
    mp_Oven = CreatePart("Oven", QPoint(7, 70));
    mp_RotaryValve = CreatePart("RotaryValve", QPoint(220, 152));

    mp_AirHeatingTube = CreatePart("AirHeatingTube", QPoint(490, 50));
    mp_HeatingTube = CreatePart("HeatingTube", QPoint(250, 116));

    mp_Fan = CreatePart("Fan", QPoint(266, 20));
    mp_WaxTrap = CreatePart("WaxTrap", QPoint(666, 109), false);
    mp_Pump = CreatePart("Pump", QPoint(404, 288));
    mp_GV1 = CreatePart("GV1", QPoint(485, 192));
    mp_GV2 = CreatePart("GV2", QPoint(640, 245));

    mp_Pressure = CreatePart("Pressure", QPoint(610,83), false);
    mp_RFV1 = CreatePart("RFV1", QPoint(596, 154), false);

    mp_Filter = CreatePart("Filter", QPoint(269, 80), false);
    mp_Connect1 = CreatePart("ConnectPart", QPoint(500, 256), false);
    mp_Connect2 = CreatePart("ConnectPart", QPoint(569, 256), false);
    mp_Connect3 = CreatePart("ConnectPart", QPoint(517, 360), false);
    mp_Line1    = CreatePart("Line1", QPoint(10, 100), false);
    mp_Line2    = CreatePart("Line2", QPoint(432, 78), false);
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
    mp_SelectBtn->setPos(133,238);
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
        delete mp_HeatingTubeCurrent;
        delete mp_HeatingTargetTemp;

        delete mp_AirHeatingTubeCurrent;
        delete mp_AirHeatingTubeTemp;
        delete mp_AirHeatingTargetTemp;

        delete mp_OvenTemp1;
        delete mp_OvenTemp2;
        delete mp_OvenTemp3;
        delete mp_OvenCurrent;
        delete mp_OvenTargetTemp;

        delete mp_RetortTemp1;
        delete mp_RetortTemp2;
        delete mp_RetortTemp3;
        delete mp_RetortCurrent;
        delete mp_RetortTargetTemp;

        delete mp_RotaryValvePosition;
        delete mp_RotaryValveTemp1;
        delete mp_RotaryValveTemp2;
        delete mp_RotaryValveCurrent;
        delete mp_RotaryValveTargetTemp;

        delete mp_PressureLabel;
    }
    catch (...) {

    }
}

void CSVCDashboardWidget::InitLabel()
{
    mp_HeatingTubeTemp    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_HeatingTubeCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_HeatingTargetTemp  = new SVCLabel(true, mp_Ui->graphicsView);
    mp_HeatingTubeTemp->setPos(358, 122);
    mp_HeatingTubeCurrent->setPos(358, 150);
    mp_HeatingTargetTemp->setPos(358, 163);
    mp_HeatingTubeTemp->setText("Temp: 0\260C");
    mp_HeatingTubeCurrent->setText("Current: 0A");
    mp_HeatingTargetTemp->setText("Target: N/A");

    mp_AirHeatingTubeTemp    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTubeCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTargetTemp  = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTubeTemp->setPos(596, 18);
    mp_AirHeatingTubeCurrent->setPos(596, 31);
    mp_AirHeatingTargetTemp->setPos(596, 44);
    mp_AirHeatingTubeTemp->setText("Temp: 0\260C");
    mp_AirHeatingTubeCurrent->setText("Current : 0A");
    mp_AirHeatingTargetTemp->setText("Target: N/A");

    mp_OvenTemp1   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp2   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp3   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTargetTemp = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp1->setPos(17, 183);
    mp_OvenTemp2->setPos(17, 196);
    mp_OvenTemp3->setPos(17, 209);
    mp_OvenCurrent->setPos(17, 222);
    mp_OvenTargetTemp->setPos(17, 235);
    mp_OvenTemp1->setText("Oven1: 0\260C");
    mp_OvenTemp2->setText("Oven2: 0\260C");
    mp_OvenTemp3->setText("Oven3: 0\260C");
    mp_OvenCurrent->setText("Current: 0A");
    mp_OvenTargetTemp->setText("Target: N/A");

    mp_RetortTemp1 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp2 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp3 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTargetTemp = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp1->setPos(504, 72);
    mp_RetortTemp2->setPos(504, 85);
    mp_RetortTemp3->setPos(504, 98);
    mp_RetortCurrent->setPos(504, 116);
    mp_RetortTargetTemp->setPos(504, 129);
    mp_RetortTemp1->setText("Retort_Temperature1");
    mp_RetortTemp2->setText("Retort_Temperature2");
    mp_RetortTemp3->setText("Retort_Temperature3");
    mp_RetortCurrent->setText("Current: 0A");
    mp_RetortTargetTemp->setText("Target: N/A");

    mp_RotaryValvePosition = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveTemp1    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveTemp2    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveCurrent  = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveTargetTemp = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValvePosition->setPos(133, 170);
    mp_RotaryValveTemp1->setPos(133, 183);
    mp_RotaryValveTemp2->setPos(133, 196);
    mp_RotaryValveCurrent->setPos(133, 209);
    mp_RotaryValveTargetTemp->setPos(133, 222);
    mp_RotaryValvePosition->setText("Position: Unknown");
    mp_RotaryValveTemp1->setText("Temp1: 0\260C");
    mp_RotaryValveTemp2->setText("Temp1: 0\260C");
    mp_RotaryValveCurrent->setText("Current: 0A");
    mp_RotaryValveTargetTemp->setText("Target: N/A");

    mp_PressureLabel = new SVCLabel(false, mp_Ui->graphicsView);
    mp_PressureLabel->setPos(608, 78);
    mp_PressureLabel->setText(" 0kPa");
}

CGraphicsItemPart* CSVCDashboardWidget::CreatePart(const QString& PartResName, const QPoint& Pos, bool Clickable)
{
    QString normalImg   = ":/Images/" + PartResName + ".png";
    QString disabledImg = ":/Images/" + PartResName + "Disabled.png";
    QString glowImg     = ":/Images/" + PartResName + "Glow.png";
    CGraphicsItemPart* pGraphicsItemPart = new CGraphicsItemPart(QPixmap(normalImg), QPixmap(disabledImg),
                                                                 QPixmap(glowImg), Clickable);
    mp_Scene->addItem(pGraphicsItemPart);
    pGraphicsItemPart->setPos(Pos);
    return pGraphicsItemPart;
}

void CSVCDashboardWidget::paintEvent(QPaintEvent* p_PaintEvent)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    //style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(p_PaintEvent);
}

void CSVCDashboardWidget::RetortSelected()
{
    qDebug()<<"retort selected.";

    quint32 EventId(0);
    Global::tTranslatableStringList StrList;
    StrList<<"Retort";
    CGraphicsItemPart::PartStatus Status = mp_Retort->Status();

    if (Status == CGraphicsItemPart::Working) {
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(60, 10, 100, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Retort Target Temperature");
        if (p_TempSelectionDlg->exec() == 0) {
            mp_Retort->SetStatus(CGraphicsItemPart::Normal);
            delete p_TempSelectionDlg;
            return;
        }

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        m_RetortTargetTempValue = TargetTemp;
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->RetortStartHeating(TargetTemp+7, TargetTemp+2);
        delete p_TempSelectionDlg;

        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Retort", "Retort start heating failed.");
            mp_Retort->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        mp_RetortTargetTemp->setText(QString("Target: %1\260C").arg(TargetTemp));
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_HEATING;
        StrList<<QString::number(TargetTemp);
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->RetortStopHeating() !=(int) Diagnostics::RETURN_OK) {
            ShowFailedDlg("Retort", "Retort stop heating failed.");
            mp_Retort->SetStatus(CGraphicsItemPart::Working);
            return;
        }
        mp_RetortTargetTemp->setText("Target: N/A");
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
    }

    Global::EventObject::Instance().RaiseEvent(EventId, StrList);
    //qDebug()<<"get Retort status:"<<Status;
}

void CSVCDashboardWidget::OvenSelected()
{
    qDebug()<<"Oven selected.";

    quint32 EventId(0);
    Global::tTranslatableStringList StrList;
    StrList<<"Oven";

    CGraphicsItemPart::PartStatus Status = mp_Oven->Status();

    if (Status == CGraphicsItemPart::Working) {
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(70, 10, 85, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Paraffin Oven Target Temperature");
        if (p_TempSelectionDlg->exec() == 0) {
            mp_Oven->SetStatus(CGraphicsItemPart::Normal);
            delete p_TempSelectionDlg;
            return;
        }

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        m_OvenTargetTempValue = TargetTemp;
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->OvenStartHeating(TargetTemp, TargetTemp);
        delete p_TempSelectionDlg;
        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Oven", "Oven start heating failed.");
            mp_Oven->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        mp_OvenTargetTemp->setText(QString("Target: %1\260C").arg(TargetTemp));
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_HEATING;
        StrList<<QString::number(TargetTemp);
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->OvenStopHeating() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Oven", "Oven stop heating failed.");
            mp_Oven->SetStatus(CGraphicsItemPart::Working);
            return;
        }
        mp_OvenTargetTemp->setText("Target: N/A");
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
    }
    Global::EventObject::Instance().RaiseEvent(EventId, StrList);
    //qDebug()<<"get Oven status:"<<Status;
}

void CSVCDashboardWidget::RotaryValveSelected()
{
    qDebug()<<"Rotary Valve selected.";

    quint32 EventId(0);
    Global::tTranslatableStringList StrList;
    StrList<<"Rotary Valve";

    CGraphicsItemPart::PartStatus Status = mp_RotaryValve->Status();

    if (Status == CGraphicsItemPart::Working) {
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(115, 10, 115, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Rotary Valve Target Temperature");
        if (p_TempSelectionDlg->exec() == 0) {
            mp_RotaryValve->SetStatus(CGraphicsItemPart::Normal);
            delete p_TempSelectionDlg;
            return;
        }

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        m_RVTargetTempValue = TargetTemp;
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->RVStartHeating(TargetTemp);
        delete p_TempSelectionDlg;
        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Rotary Valve", "Rotary valve start heating failed.");
            mp_RotaryValve->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        mp_RotaryValveTargetTemp->setText(QString("Target: %1\260C").arg(TargetTemp));
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_HEATING;
        StrList<<QString::number(TargetTemp);
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->RVStopHeating() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Rotary Valve", "Rotary valve stop heating failed.");
            mp_RotaryValve->SetStatus(CGraphicsItemPart::Working);
            return;
        }
        mp_RotaryValveTargetTemp->setText("Target: N/A");
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
    }

    Global::EventObject::Instance().RaiseEvent(EventId, StrList);
    //qDebug()<<"get RotaryValve status:"<<Status;
}

void CSVCDashboardWidget::AirTubeSelected()
{
    qDebug()<<"Air Heating Tube selected.";

    quint32 EventId(0);
    Global::tTranslatableStringList StrList;
    StrList<<"Air Tube";

    CGraphicsItemPart::PartStatus Status = mp_AirHeatingTube->Status();

    if (Status == CGraphicsItemPart::Working) {
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(80, 10, 80, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Air Heating Tube Target Temperature");
        if (p_TempSelectionDlg->exec() == 0) {
            mp_AirHeatingTube->SetStatus(CGraphicsItemPart::Normal);
            delete p_TempSelectionDlg;
            return;
        }

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        m_AirTubeTempValue = TargetTemp;
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->AirTubeStartHeating(TargetTemp);
        delete p_TempSelectionDlg;

        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Air heating tube", "Air heating tube start heating failed.");
            mp_AirHeatingTube->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        mp_AirHeatingTargetTemp->setText(QString("Target: %1\260C").arg(TargetTemp));
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_HEATING;
        StrList<<QString::number(TargetTemp);
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->AirTubeStopHeating() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Air Heating Tube", "Air heating tube stop heating failed.");
            mp_AirHeatingTube->SetStatus(CGraphicsItemPart::Working);
            return;
        }
        mp_AirHeatingTargetTemp->setText("Target: N/A");
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
    }

    Global::EventObject::Instance().RaiseEvent(EventId, StrList);
    //qDebug()<<"get AirHeatingTube status:"<<Status;
}

void CSVCDashboardWidget::LiquidTubeSelected()
{
    qDebug()<<"Liquid Heating Tube selected.";

    quint32 EventId(0);
    Global::tTranslatableStringList StrList;
    StrList<<"Liquid Tube";

    CGraphicsItemPart::PartStatus Status = mp_HeatingTube->Status();

    if (Status == CGraphicsItemPart::Working) {
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(80, 10, 80, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Liquid Heating Tube Target Temperature");
        if (p_TempSelectionDlg->exec() == 0) {
            mp_HeatingTube->SetStatus(CGraphicsItemPart::Normal);
            delete p_TempSelectionDlg;
            return;
        }

        int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        m_LiquidTubeTempValue = TargetTemp;
        int Ret = Diagnostics::ServiceDeviceProcess::Instance()->LiquidTubeStartHeating(TargetTemp);
        delete p_TempSelectionDlg;
        qDebug()<<"Liquid tube heating result:"<<Ret;
        if (Ret != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Liquid Heating Tube", "Liquid heating tube start heating failed.");
            mp_HeatingTube->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        mp_HeatingTargetTemp->setText(QString("Target: %1\260C").arg(TargetTemp));
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_HEATING;
        StrList<<QString::number(TargetTemp);
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->LiquidTubeStopHeating() != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Liquid Heating Tube", "Liquid heating tube stop heating failed.");
            mp_HeatingTube->SetStatus(CGraphicsItemPart::Working);
            return;
        }
        mp_HeatingTargetTemp->setText("Target: N/A");
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
    }

    Global::EventObject::Instance().RaiseEvent(EventId, StrList);
    //qDebug()<<"get LiquidHeatingTube status:"<<Status;
}

void CSVCDashboardWidget::FanSelected()
{
    qDebug()<<"Fan selected.";

    quint32 EventId(0);

    CGraphicsItemPart::PartStatus Status = mp_Fan->Status();

    if (Status == CGraphicsItemPart::Working) {
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetFan(1) != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Exhaust Fan", "Exhaust fan open failed.");
            mp_Fan->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_ACTIVATE;
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetFan(0) != (int)Diagnostics::RETURN_OK) {
            ShowFailedDlg("Exhaust Fan", "Exhaust fan close failed.");
            mp_Fan->SetStatus(CGraphicsItemPart::Working);
            return;
        }
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
    }

    Global::EventObject::Instance().RaiseEvent(EventId, Global::tTranslatableStringList()<<"Exhaust Fan");
}

void CSVCDashboardWidget::Valve1Selected()
{

    qDebug()<<"Valve1 selected.";
    quint32 EventId(0);
    CGraphicsItemPart::PartStatus Status = mp_GV1->Status();
    if (Status == CGraphicsItemPart::Working) {
        if (CheckIfPressureInRange() == false){
            mp_GV1->SetStatus(CGraphicsItemPart::Normal);
            return;
        }

        if(!SetValveStatus(1, 0)) {
            ShowFailedDlg("Error", "Failed to close valve 2");
            mp_GV1->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        if (!SetValveStatus(0, 1)) {
            ShowFailedDlg("Error", "Failedd to open valve 1");
            mp_GV1->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetPressure(9, -40) == -1) {
            ShowFailedDlg("Error", "Failed to set target vacuum -40kPa");
            mp_GV1->SetStatus(CGraphicsItemPart::Normal);
            return;
        }

        mp_GV2->SetStatus(CGraphicsItemPart::Disabled);
        mp_Pump->SetStatus(CGraphicsItemPart::Disabled);

        Diagnostics::ServiceDeviceProcess::Instance()->Pause(500);
        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_ACTIVATE;
        m_PumpTestStatus = 1;
    }
    else if (Status == CGraphicsItemPart::Normal){
        if(Diagnostics::ServiceDeviceProcess::Instance()->PumpStopCompressor() == -1) {
            ShowFailedDlg("Error", "Failed to stop pump");
        }
        if (!SetValveStatus(0, 0)) {
            ShowFailedDlg("Error", "Failed to close valve 1");
            mp_GV1->SetStatus(CGraphicsItemPart::Working);
            return;
        }

        mp_GV2->SetStatus(CGraphicsItemPart::Normal);
        mp_Pump->SetStatus(CGraphicsItemPart::Normal);

        Diagnostics::ServiceDeviceProcess::Instance()->Pause(500);

        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
        m_PumpTestStatus = 0;
    }

    Global::EventObject::Instance().RaiseEvent(EventId, Global::tTranslatableStringList()<<"Valve1");
}

void CSVCDashboardWidget::Valve2Selected()
{
    qDebug()<<"Valve1 selected.";

    quint32 EventId(0);
    CGraphicsItemPart::PartStatus Status = mp_GV2->Status();
    if (Status == CGraphicsItemPart::Working) {
        if (CheckIfPressureInRange() == false){
            mp_GV2->SetStatus(CGraphicsItemPart::Normal);
            return;
        }

        if (!SetValveStatus(0, 0)) {
            ShowFailedDlg("Error", "Failed to close valve 1");
            mp_GV2->SetStatus(CGraphicsItemPart::Normal);
            return ;
        }
        if (!SetValveStatus(1, 1)) {
            ShowFailedDlg("Error", "Failed to open valve 2");
            mp_GV2->SetStatus(CGraphicsItemPart::Normal);
            return;
        }

        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetPressure(1, 40) == -1) {
            ShowFailedDlg("Error", "Failed to set target pressure");
            return ;
        }
        mp_GV1->SetStatus(CGraphicsItemPart::Disabled);
        mp_Pump->SetStatus(CGraphicsItemPart::Disabled);

        Diagnostics::ServiceDeviceProcess::Instance()->Pause(500);

        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_ACTIVATE;
        m_PumpTestStatus = 2;
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpStopCompressor() == -1)
        {
            ShowFailedDlg("Error", "Failed to stop pump");
        }
        if (!SetValveStatus(1, 0)) {
            ShowFailedDlg("Error", "Failed to close valve 2");
            mp_GV2->SetStatus(CGraphicsItemPart::Working);
            return;
        }
        Diagnostics::ServiceDeviceProcess::Instance()->Pause(500);
        mp_GV1->SetStatus(CGraphicsItemPart::Normal);
        mp_Pump->SetStatus(CGraphicsItemPart::Normal);

        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
        m_PumpTestStatus = 0;
    }

    Global::EventObject::Instance().RaiseEvent(EventId, Global::tTranslatableStringList()<<"Valve2");
}

bool CSVCDashboardWidget::SetValveStatus(quint8 ValveIndex, quint8 Status)
{
    if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetValve(ValveIndex, Status) !=
            (int)Diagnostics::RETURN_OK) {
        return false;
    }
    if (ValveIndex == 0) {
        if (Status == 0) {
            mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Disabled);
            mp_GV1StateRight->SetStatus(CGraphicsItemPart::Normal);
        }
        else {         
            mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Normal);
            mp_GV1StateRight->SetStatus(CGraphicsItemPart::Disabled);
        }
    }
    else {
        if (Status == 0) {
            mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Disabled);
            mp_GV2StateRight->SetStatus(CGraphicsItemPart::Normal);
        }
        else {            
            mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Normal);
            mp_GV2StateRight->SetStatus(CGraphicsItemPart::Disabled);
        }
    }
    return true;
}

bool CSVCDashboardWidget::CheckIfPressureInRange()
{
    float Pressure(0.0);
    int Ret(0);
    Ret = Diagnostics::ServiceDeviceProcess::Instance()->PumpGetPressure(&Pressure);
    if (Ret == -1) {
        ShowFailedDlg("Error", "Can't get pressure value.");
        return false;
    }

    if (qAbs(Pressure) <= 2.0) {
        return true;
    }

    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(this);
    QString Title = "Warning!";
    QString Message = QString().sprintf("The current pressure of %.2f kPa is not in the range of (-2kPa~+2kPa)."
                              "Please retry after 5 seconds. If you see this message again, "
                              "please calibrate the pressure sensor manually.", Pressure);
    dlg->SetTitle(Title);
    dlg->SetText(Message);
    dlg->SetIcon(QMessageBox::Warning);

    dlg->HideButtons();
    dlg->SetButtonText(1, "OK");

    dlg->setModal(true);

    (void)dlg->exec();
    delete dlg;

    return false;
}

void CSVCDashboardWidget::PumpSelected()
{
    qDebug()<<"Pump selected.";


    quint32 EventId(0);

    CGraphicsItemPart::PartStatus Status = mp_Pump->Status();

    if (Status == CGraphicsItemPart::Working) {
        if (CheckIfPressureInRange() == false){
            mp_Pump->SetStatus(CGraphicsItemPart::Normal);
            return;
        }

        if (!SetValveStatus(0, 0)) {
            ShowFailedDlg("Error", "Failed to close valve 1.");
            mp_Pump->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        if (!SetValveStatus(1, 0)) {
            ShowFailedDlg("Error", "Failed to close valve 2.");
            mp_Pump->SetStatus(CGraphicsItemPart::Normal);
            return;
        }
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpSetPressure(1, 40) == -1) {
            ShowFailedDlg("Error", "Failed to set target pressure.");
            mp_Pump->SetStatus(CGraphicsItemPart::Normal);
            return ;
        }

        mp_GV1->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV2->SetStatus(CGraphicsItemPart::Disabled);
        Diagnostics::ServiceDeviceProcess::Instance()->Pause(500);

        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_ACTIVATE;
        m_PumpTestStatus = 3;
    }
    else if (Status == CGraphicsItemPart::Normal){
        if (Diagnostics::ServiceDeviceProcess::Instance()->PumpStopCompressor() == -1){
            mp_Pump->SetStatus(CGraphicsItemPart::Working);
            ShowFailedDlg("Error", "Failed to stop pump");
            return ;
        }

        mp_GV1->SetStatus(CGraphicsItemPart::Normal);
        mp_GV2->SetStatus(CGraphicsItemPart::Normal);
        Diagnostics::ServiceDeviceProcess::Instance()->Pause(500);

        EventId = EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE;
        m_PumpTestStatus = 0;
    }
    Global::EventObject::Instance().RaiseEvent(EventId, Global::tTranslatableStringList()<<"Pump");
}

void CSVCDashboardWidget::OnSelectPosition()
{
    if (!CheckRVTemperature()) {
        return;
    }
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

        Text = "Please verify status of the retort. "\
                "If there is any reagent or paraffin in the retort, "\
                "abort and change to the \"Diagnostic_Retort_Reagent Drain\" function "
                "to drain the liquid back to the original position. Thereafter flush the retort if necessary.";
        Ret = mp_MsgDlg->ShowConfirmMessage("Warning!", Text, Diagnostics::CDiagnosticMessageDlg::OK_ABORT);
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

    if (Postion == 14 || Postion == 15 || Postion == 16) {
        Text = "Do you really want to use paraffin?";
        int Ret = mp_MsgDlg->ShowConfirmMessage("WARNING!", Text, Diagnostics::CDiagnosticMessageDlg::YES_CANCEL);

        if (Ret == (int)Diagnostics::CDiagnosticMessageDlg::CANCEL) {
            return;
        }
    }

    QString PositionStr = PostionToStr(Flag, Postion);
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_SVCDIAGNOSTICS_SELECTED_POSITION, Global::tTranslatableStringList()<<PositionStr);

    Text = QString("Rotating Rotary Valve to position %1").arg(PositionStr);
    mp_MsgDlg->ShowWaitingDialog(Title, Text);
    int Ret = Diagnostics::ServiceDeviceProcess::Instance()->RVMovePosition(Flag, Postion);
    mp_MsgDlg->HideWaitingDialog();

    if (Ret != (int)Diagnostics::RETURN_OK) {
        mp_MsgDlg->ShowRVMoveFailedDlg(Title);
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_SVCDIAGNOSTICS_MOVE_POSITION_FAILED);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_SVCDIAGNOSTICS_MOVE_POSITION_SUCCESS);
    }
    qDebug()<<"move position result :"<<Ret;
}

void CSVCDashboardWidget::OnValveStateInfo()
{  
    CSVCValveStateInfoDlg* p_Dlg = new CSVCValveStateInfoDlg(this);
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
    if (StatusIsOn) {
        if (m_RetortTargetTempValue == -1) {
           DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortPreTest");
           m_RetortTargetTempValue = p_TestCase->GetParameter("TargetTemp").toInt();
        }
        mp_RetortTargetTemp->setText(QString("Target: %1\260C").arg(m_RetortTargetTempValue));
    }
    else {
        mp_RetortTargetTemp->setText("Target: N/A");
    }

    Ret = p_DevProc->OvenTempControlIsOn(&StatusIsOn);
    qDebug()<<"Oven temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_Oven->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));
    if (StatusIsOn) {
        if (m_OvenTargetTempValue == -1) {
            DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SOvenPreTest");
            m_OvenTargetTempValue = p_TestCase->GetParameter("TargetTemp").toInt();
        }
        mp_OvenTargetTemp->setText(QString("Target: %1\260C").arg(m_OvenTargetTempValue));
    }
    else {
        mp_OvenTargetTemp->setText("Target: N/A");
    }

    Ret = p_DevProc->RVTempControlIsOn(&StatusIsOn);
    qDebug()<<"rotary valve temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_RotaryValve->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));
    if (StatusIsOn) {
        if (m_RVTargetTempValue == -1) {
            DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRVPreTest");
            m_RVTargetTempValue = p_TestCase->GetParameter("PreHeatingTargetTemp").toInt();
        }
        mp_RotaryValveTargetTemp->setText(QString("Target: %1\260C").arg(m_RVTargetTempValue));
    }
    else {
        mp_RotaryValveTargetTemp->setText("Target: N/A");
    }

    Ret = p_DevProc->AirTubeTempControlIsOn(&StatusIsOn);
    qDebug()<<"air tube temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_AirHeatingTube->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));
    if (StatusIsOn) {
        if (m_AirTubeTempValue != -1) {
            mp_AirHeatingTubeTemp->setText(QString("Target: %1\260C").arg(m_AirTubeTempValue));
        }
    }
    else {
        mp_AirHeatingTubeTemp->setText("Target: N/A");
    }

    Ret = p_DevProc->LiquidTubeTempControlIsOn(&StatusIsOn);
    qDebug()<<"liquid tube temp control state:"<<StatusIsOn<<" ret = "<<Ret;
    mp_HeatingTube->SetStatus(StatusIsOn ? (CGraphicsItemPart::Working) : (CGraphicsItemPart::Normal));
    if (StatusIsOn) {
        if (m_LiquidTubeTempValue == -1) {
            DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SLTubePreTest");
            m_LiquidTubeTempValue = p_TestCase->GetParameter("PreHeatingTargetTemp").toInt();
        }
        mp_HeatingTargetTemp->setText(QString("Target: %1\260C").arg(m_LiquidTubeTempValue));
    }
    else {
        mp_HeatingTargetTemp->setText("Target: N/A");
    }

    mp_Fan->SetStatus(p_DevProc->PumpGetFan() ? CGraphicsItemPart::Working : CGraphicsItemPart::Normal);

    if (m_PumpTestStatus == 0) {
        mp_GV1->SetStatus(CGraphicsItemPart::Normal);
        mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV1StateRight->SetStatus(CGraphicsItemPart::Normal);

        mp_GV2->SetStatus(CGraphicsItemPart::Normal);
        mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV2StateRight->SetStatus(CGraphicsItemPart::Normal);

        mp_Pump->SetStatus(CGraphicsItemPart::Normal);
    }
    else if (m_PumpTestStatus == 1) {
        mp_GV1->SetStatus(CGraphicsItemPart::Working);
        mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Normal);
        mp_GV1StateRight->SetStatus(CGraphicsItemPart::Disabled);

        mp_GV2->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV2StateRight->SetStatus(CGraphicsItemPart::Normal);

        mp_Pump->SetStatus(CGraphicsItemPart::Disabled);
    } else if (m_PumpTestStatus == 1) {
        mp_GV2->SetStatus(CGraphicsItemPart::Working);
        mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Normal);
        mp_GV2StateRight->SetStatus(CGraphicsItemPart::Disabled);

        mp_GV1->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV1StateRight->SetStatus(CGraphicsItemPart::Normal);

        mp_Pump->SetStatus(CGraphicsItemPart::Disabled);
    }
    else if (m_PumpTestStatus == 3) {
        mp_Pump->SetStatus(CGraphicsItemPart::Working);

        mp_GV1->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV1StateLeft->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV1StateRight->SetStatus(CGraphicsItemPart::Normal);

        mp_GV2->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV2StateLeft->SetStatus(CGraphicsItemPart::Disabled);
        mp_GV2StateRight->SetStatus(CGraphicsItemPart::Normal);
    }
}

void CSVCDashboardWidget::UpdateOvenLabel(qreal OvenTemp1, qreal OvenTemp2, qreal OvenTemp3, quint16 Current)
{
    mp_OvenTemp1->setText(QString().sprintf("Oven1:%.2f\260C", OvenTemp1));
    mp_OvenTemp2->setText(QString().sprintf("Oven2:%.2f\260C", OvenTemp2));
    mp_OvenTemp3->setText(QString().sprintf("Oven3:%.2f\260C", OvenTemp3));
    mp_OvenCurrent->setText(QString().sprintf("Current:%dmA", Current));
}

void CSVCDashboardWidget::UpdateRetortLabel(qreal RetortTemp1, qreal RetortTemp2, qreal RetortTemp3, quint16 Current)
{
    mp_RetortTemp1->setText(QString().sprintf("Retort1:%.2f\260C", RetortTemp1));
    mp_RetortTemp2->setText(QString().sprintf("Retort2:%.2f\260C", RetortTemp2));
    mp_RetortTemp3->setText(QString().sprintf("Retort3:%.2f\260C", RetortTemp3));
    mp_RetortCurrent->setText(QString().sprintf("Current:%dmA", Current));
}

void CSVCDashboardWidget::UpdateRotaryValveLabel(bool RVTubeFlag, qreal RVPosition, qreal RVTemp1, qreal RVTemp2, quint16 RVCurrent)
{
    QString PositionStr = PostionToStr(RVTubeFlag, RVPosition);

    mp_RotaryValvePosition->setText(QString("Position:%1").arg(PositionStr));
    mp_RotaryValveTemp1->setText(QString().sprintf("Temp1:%.2f\260C", RVTemp1));
    mp_RotaryValveTemp2->setText(QString().sprintf("Temp2:%.2f\260C", RVTemp2));
    mp_RotaryValveCurrent->setText(QString().sprintf("Current:%dmA", RVCurrent));
}

bool CSVCDashboardWidget::CheckRVTemperature()
{
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("MovementTest");
    qreal RVSensor1TempMin = p_TestCase->GetParameter("RVSensor1TempMin").toFloat();
    qreal RVSensor2TempMin = p_TestCase->GetParameter("RVSensor2TempMin").toFloat();
    qreal RVSensor1TempCurrent;
    qreal RVSensor2TempCurrent;

    int ret = Diagnostics::ServiceDeviceProcess::Instance()->RVGetTemp(&RVSensor1TempCurrent, &RVSensor2TempCurrent);
    if (ret != Diagnostics::RETURN_OK || RVSensor1TempCurrent < RVSensor1TempMin
            || RVSensor2TempCurrent < RVSensor2TempMin) {
        QString text = tr("Rotary Valve cannot rotate, due to the minimum "
                  "temperature has not been reached. Please check "
                  "resistance of temperature sensors, current of heating "
                  "element and function of ASB3. If no root cause "
                  "found, check main relay on ASB15 and cable "
                  "connections in addition. Exchange part accordingly.");
        mp_MsgDlg->ShowMessage("Select Position ", text, Diagnostics::RETURN_ERR_FAIL);
        return false;
    }

    return true;
}

QString CSVCDashboardWidget::PostionToStr(bool TubeFlag, qreal Position)
{   
    if (Position == 0) {
        return QString("Unknown");
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

void CSVCDashboardWidget::UpdateAirHeatingTubeLabel(qreal Temp, quint16 Current)
{
    mp_AirHeatingTubeTemp->setText(QString().sprintf("Temp:%.2f\260C", Temp));
    mp_AirHeatingTubeCurrent->setText(QString().sprintf("Current:%dmA", Current));
}

void CSVCDashboardWidget::UpdateLiquidHeatingTubeLabel(qreal Temp, quint16 Current)
{
    mp_HeatingTubeTemp->setText(QString().sprintf("Temp:%.2f\260C", Temp));
    mp_HeatingTubeCurrent->setText(QString().sprintf("Current:%dmA", Current));
}

void CSVCDashboardWidget::UpdatePressureLabel(qreal Pressure)
{
    mp_PressureLabel->setText(QString().sprintf(" %.2fkPa", Pressure));
}
