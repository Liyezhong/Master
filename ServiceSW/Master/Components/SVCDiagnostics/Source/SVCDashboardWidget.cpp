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

#include "Diagnostics/Include/SelectBottleNReagentDialog.h"


using namespace SVCDiagnostics;

CSVCDashboardWidget::CSVCDashboardWidget(QWidget *p_Parent) :
    QWidget(p_Parent),
    mp_Ui(new Ui::CSVCDashboardWidget),
    m_IsFristMoveRV(true)
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

    mp_Pressure = CreatePart("Pressure", QPoint(610,70));
    mp_RFV1 = CreatePart("RFV1", QPoint(596, 154), false);

    mp_Filter = CreatePart("Filter", QPoint(269, 80), false);
    mp_Connect1 = CreatePart("ConnectPart", QPoint(500, 256), false);
    mp_Connect2 = CreatePart("ConnectPart", QPoint(569, 256), false);
    mp_Connect3 = CreatePart("ConnectPart", QPoint(517, 360), false);
    mp_Line1    = CreatePart("Line1", QPoint(10, 100), false);
    mp_Line2    = CreatePart("Line2", QPoint(434, 76), false);
    mp_Line3    = CreatePart("Line3", QPoint(227, 274), false);

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
    CONNECTSIGNALSLOT(mp_Pressure, PartSelected(), this, PressureSelected());

    CONNECTSIGNALSLOT(mp_SelectBtn, clicked(), this, OnSelectPosition());
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
    mp_HeatingTubeTemp->setText("   Temp : 46\260C");
    mp_HeatingTubeCurrent->setText("  Current : 1A");

    mp_AirHeatingTubeTemp    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTubeCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTubeTemp->setPos(596, 31);
    mp_AirHeatingTubeCurrent->setPos(596, 44);
    mp_AirHeatingTubeTemp->setText("   Temp : 46\260C");
    mp_AirHeatingTubeCurrent->setText("  Current : 1A");

    mp_OvenTemp1   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp2   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp3   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp1->setPos(17, 183);
    mp_OvenTemp2->setPos(17, 196);
    mp_OvenTemp3->setPos(17, 209);
    mp_OvenCurrent->setPos(17, 222);
    mp_OvenTemp1->setText("  Oven1 : 90\260C");
    mp_OvenTemp2->setText("  Oven2 : 30\260C");
    mp_OvenTemp3->setText("  Oven3 : 60\260C");
    mp_OvenCurrent->setText("  Current : 5A");

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
    mp_RetortCurrent->setText("Current : 2A");

    mp_RotaryValvePosition = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveTemp1    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveTemp2    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveCurrent  = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValvePosition->setPos(137, 183);
    mp_RotaryValveTemp1->setPos(137, 196);
    mp_RotaryValveTemp2->setPos(137, 209);
    mp_RotaryValveCurrent->setPos(137, 222);
    mp_RotaryValvePosition->setText("  Position : 10");
    mp_RotaryValveTemp1->setText("  Temp1 : 50\260C");
    mp_RotaryValveTemp2->setText("  Temp1 : 50\260C");
    mp_RotaryValveCurrent->setText("  Current : 2A");

    mp_PressureLabel = new SVCLabel(false, mp_Ui->graphicsView);
    mp_PressureLabel->setPos(636, 100);
    mp_PressureLabel->setText(" 20kPa");
}

CGraphicsItemPart* CSVCDashboardWidget::CreatePart(const QString& partResName, const QPoint& pos, bool Clickable)
{
    QString normalImg = ":/Images/" + partResName + ".png";
    CGraphicsItemPart* pGraphicsItemPart = NULL;
    if (Clickable) {
        QString disabledImg = ":/Images/" + partResName + "Disabled.png";
        QString glowImg = ":/Images/" + partResName + "Glow.png";
        pGraphicsItemPart = new CGraphicsItemPart(QPixmap(normalImg),
                                                QPixmap(disabledImg),
                                                QPixmap(glowImg));
    }
    else {
        pGraphicsItemPart = new CGraphicsItemPart(QPixmap(normalImg));
    }
    mp_Scene->addItem(pGraphicsItemPart);
    pGraphicsItemPart->setPos(pos);
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
        Diagnostics::ServiceDeviceProcess::Instance()->RetortStartHeating(TargetTemp+7, TargetTemp+2);
        delete p_TempSelectionDlg;
    }
    else if (Status == CGraphicsItemPart::Normal){
        Diagnostics::ServiceDeviceProcess::Instance()->RetortStopHeating();
    }

    qDebug()<<"get Retort status:"<<Status;
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
        Diagnostics::ServiceDeviceProcess::Instance()->OvenStartHeating(TargetTemp, TargetTemp);
        delete p_TempSelectionDlg;
    }
    else if (Status == CGraphicsItemPart::Normal){
        Diagnostics::ServiceDeviceProcess::Instance()->OvenStopHeating();
    }

    qDebug()<<"get Oven status:"<<Status;
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
        (void)Diagnostics::ServiceDeviceProcess::Instance()->RVStartHeating(TargetTemp);
        delete p_TempSelectionDlg;
    }
    else if (Status == CGraphicsItemPart::Normal){
        Diagnostics::ServiceDeviceProcess::Instance()->RVStopHeating();
    }

    qDebug()<<"get RotaryValve status:"<<Status;
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
        Diagnostics::ServiceDeviceProcess::Instance()->AirTubeStartHeating(TargetTemp);
        delete p_TempSelectionDlg;
    }
    else if (Status == CGraphicsItemPart::Normal){
        Diagnostics::ServiceDeviceProcess::Instance()->AirTubeStopHeating();
    }

    qDebug()<<"get AirHeatingTube status:"<<Status;
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
        //if (Ret == 0)
    }
    else if (Status == CGraphicsItemPart::Normal){
        Diagnostics::ServiceDeviceProcess::Instance()->LiquidTubeStopHeating();
    }

    qDebug()<<"get LiquidHeatingTube status:"<<Status;
}

void CSVCDashboardWidget::PressureSelected()
{
    qDebug()<<"Pressure selected.";

    CGraphicsItemPart::PartStatus Status = mp_Pressure->Status();

    if (Status == CGraphicsItemPart::Working) {
        CSVCTargetTempSelectionDlg* p_TempSelectionDlg = new CSVCTargetTempSelectionDlg(20, 40, 120, mp_Ui->graphicsView);
        p_TempSelectionDlg->SetDialogTitle("Pressure");
        (void)p_TempSelectionDlg->exec();
        //int TargetTemp = p_TempSelectionDlg->GetTargetTemp();
        Diagnostics::ServiceDeviceProcess::Instance()->PumpBuildPressure(40);
        delete p_TempSelectionDlg;
    }
    else if (Status == CGraphicsItemPart::Normal){
        Diagnostics::ServiceDeviceProcess::Instance()->PumpReleasePressure();
    }

    qDebug()<<"get Pressure status:"<<Status;
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

        if (Ret == Diagnostics::CDiagnosticMessageDlg::ABORT) {
            return;
        }

        Text = "Rotary valve is initializing...";
        mp_MsgDlg->ShowWaitingDialog(Title, Text);
        Ret = Diagnostics::ServiceDeviceProcess::Instance()->RVInitialize();
        mp_MsgDlg->HideWaitingDialog();

        if (Ret != Diagnostics::RETURN_OK) {
            Text = "Rotary valve initialize failed.";
            mp_MsgDlg->ShowMessage(Title, Text, Diagnostics::RETURN_ERR_FAIL);
            return;
        }

        CurrentTubeFlag = true;
        CurrentPosition = 1;
    }

    SVCDiagnostics::CSVCRotaryValveDlg rotaryValveDlg(this);
    rotaryValveDlg.SetDialogTitle(tr("Rotary valve dialog"));
    rotaryValveDlg.SetPos(CurrentTubeFlag, CurrentPosition);
    if (rotaryValveDlg.exec() == 0)
        return;

    bool flag;
    qint32 pos;
    rotaryValveDlg.GetPos(flag, pos);

    if ( flag == CurrentTubeFlag && pos == CurrentPosition)
        return;

    Text = QString("Rotating Rotary Valve to position %1").arg(PostionToStr(flag, pos));
    mp_MsgDlg->ShowWaitingDialog(Title, Text);
    int Ret = Diagnostics::ServiceDeviceProcess::Instance()->RVMovePosition(flag, pos);
    mp_MsgDlg->HideWaitingDialog();

    qDebug()<<"move position result :"<<Ret;
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
}

void CSVCDashboardWidget::UpdateOvenLabel(qreal OvenTemp1, qreal OvenTemp2, qreal OvenTemp3, qreal Current)
{
    mp_OvenTemp1->setText(QString("  Oven1 : %1\260C").arg(OvenTemp1));
    mp_OvenTemp2->setText(QString("  Oven2 : %1\260C").arg(OvenTemp2));
    mp_OvenTemp3->setText(QString("  Oven3 : %1\260C").arg(OvenTemp3));
    mp_OvenCurrent->setText(QString("  Current : %1mA").arg(Current));
}

void CSVCDashboardWidget::UpdateRetortLabel(qreal RetortTemp1, qreal RetortTemp2, qreal RetortTemp3, qreal Current)
{
    mp_RetortTemp1->setText(QString("  Retort1 : %1\260C").arg(RetortTemp1));
    mp_RetortTemp2->setText(QString("  Retort2 : %1\260C").arg(RetortTemp2));
    mp_RetortTemp3->setText(QString("  Retort3 : %1\260C").arg(RetortTemp3));
    mp_RetortCurrent->setText(QString("  Current : %1mA").arg(Current));
}

void CSVCDashboardWidget::UpdateRotaryValveLabel(bool RVTubeFlag, qreal RVPosition, qreal RVTemp1, qreal RVTemp2, qreal RVCurrent)
{
    QString PositionStr = PostionToStr(RVTubeFlag, RVPosition);

    mp_RotaryValvePosition->setText(QString("  Position : %1").arg(PositionStr));
    mp_RotaryValveTemp1->setText(QString("  Temp1 : %1\260C").arg(RVTemp1));
    mp_RotaryValveTemp2->setText(QString("  Temp2 : %1\260C").arg(RVTemp2));
    mp_RotaryValveCurrent->setText(QString("  Current : %1mA").arg(RVCurrent));
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

void CSVCDashboardWidget::UpdateAirHeatingTubeLabel(qreal Temp, qreal Current)
{
    mp_AirHeatingTubeTemp->setText(QString("   Temp : %1\260C").arg(Temp));
    mp_AirHeatingTubeCurrent->setText(QString("  Current : %1mA").arg(Current));
}

void CSVCDashboardWidget::UpdateLiquidHeatingTubeLabel(qreal Temp, qreal Current)
{
    mp_HeatingTubeTemp->setText(QString("   Temp : %1\260C").arg(Temp));
    mp_HeatingTubeCurrent->setText(QString("  Current : %1mA").arg(Current));
}

void CSVCDashboardWidget::UpdatePressureLabel(qreal Pressure)
{
    mp_PressureLabel->setText(QString(" %1kPa").arg(Pressure));
}
