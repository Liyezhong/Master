#include "SVCDiagnostics/Include/SVCDashboardWidget.h"
#include "ui_SVCDashboardWidget.h"
#include <QPainter>
#include <Global/Include/Utils.h>
#include <QGraphicsScene>
#include "SVCDiagnostics/Include/GraphicsItemPart.h"

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

    InitLabel();

    mp_Retort = CreatePart("Retort", QPoint(406, 30));
    mp_Oven = CreatePart("Oven", QPoint(7, 70));
    mp_RotaryValve = CreatePart("RotaryValve", QPoint(220, 152));

    mp_AirHeatingTube = CreatePart("AirHeatingTube", QPoint(489, 50));
    mp_HeatingTube = CreatePart("HeatingTube", QPoint(250, 116));

    mp_Fan = CreatePart("Fan", QPoint(266, 20));
    mp_WaxTrap = CreatePart("WaxTrap", QPoint(666, 109));
    mp_Pump = CreatePart("Pump", QPoint(404, 288));
    mp_GV1 = CreatePart("GV1", QPoint(485, 192));
    mp_GV2 = CreatePart("GV2", QPoint(640, 245));

    mp_Pressure = CreatePart("Pressure", QPoint(615,75));
    mp_RFV1 = CreatePart("RFV1", QPoint(606, 160));

    mp_Filter = CreatePart("Filter", QPoint(269, 80), false);
    mp_Connect1 = CreatePart("ConnectPart", QPoint(500, 256), false);
    mp_Connect2 = CreatePart("ConnectPart", QPoint(569, 256), false);
    mp_Connect3 = CreatePart("ConnectPart", QPoint(517, 360), false);
    mp_Line1    = CreatePart("Line1", QPoint(10, 100), false);

    mp_SelectBtn    = new SVCButton(false, mp_Ui->graphicsView);
    mp_ValveInfoBtn = new SVCButton(true, mp_Ui->graphicsView);
    mp_SelectBtn->setText("Select Position");
    mp_SelectBtn->setPos(137,225);
    mp_ValveInfoBtn->setText("Valve State Info");
    mp_ValveInfoBtn->setPos(380, 420);


    CONNECTSIGNALSLOT(mp_Retort, PartSelected(), this, RetortSelected());
}

CSVCDashboardWidget::~CSVCDashboardWidget()
{
    try {
        delete mp_Ui;
        delete mp_Scene;
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
        delete mp_RotaryValveTemp;
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
    mp_HeatingTubeTemp->setText("   Temp : 46");
    mp_HeatingTubeCurrent->setText("  Current : 1A");

    mp_AirHeatingTubeTemp    = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTubeCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_AirHeatingTubeTemp->setPos(596, 31);
    mp_AirHeatingTubeCurrent->setPos(596, 44);
    mp_AirHeatingTubeTemp->setText("   Temp : 46");
    mp_AirHeatingTubeCurrent->setText("  Current : 1A");

    mp_OvenTemp1   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp2   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp3   = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_OvenTemp1->setPos(17, 183);
    mp_OvenTemp2->setPos(17, 196);
    mp_OvenTemp3->setPos(17, 209);
    mp_OvenCurrent->setPos(17, 222);
    mp_OvenTemp1->setText("  Oven1 : 90");
    mp_OvenTemp2->setText("  Oven2 : 30");
    mp_OvenTemp3->setText("  Oven3 : 60");
    mp_OvenCurrent->setText("  Current : 5A");

    mp_RetortTemp1 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp2 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp3 = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortCurrent = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RetortTemp1->setPos(500, 68);
    mp_RetortTemp2->setPos(500, 81);
    mp_RetortTemp3->setPos(500, 94);
    mp_RetortCurrent->setPos(500, 120);
    mp_RetortTemp1->setText("Retort_Temperature1");
    mp_RetortTemp2->setText("Retort_Temperature2");
    mp_RetortTemp3->setText("Retort_Temperature3");
    mp_RetortCurrent->setText("Current : 2A");

    mp_RotaryValvePosition = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveTemp     = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValveCurrent  = new SVCLabel(true, mp_Ui->graphicsView);
    mp_RotaryValvePosition->setPos(137, 184);
    mp_RotaryValveTemp->setPos(137, 197);
    mp_RotaryValveCurrent->setPos(137, 210);
    mp_RotaryValvePosition->setText("  Position : 10");
    mp_RotaryValveTemp->setText("  Temp : 50");
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

    qDebug()<<"get retort status:"<<Status;

}

void CSVCDashboardWidget::UpdateOvenLabel(qreal OvenTemp1, qreal OvenTemp2, qreal OvenTemp3, qreal Current)
{
    mp_OvenTemp1->setText(QString("  Oven1 : %1").arg(OvenTemp1));
    mp_OvenTemp2->setText(QString("  Oven2 : %1").arg(OvenTemp2));
    mp_OvenTemp3->setText(QString("  Oven3 : %1").arg(OvenTemp3));
    mp_OvenCurrent->setText(QString("  Current : %1A").arg(Current));
}

void CSVCDashboardWidget::UpdateRetortLabel(qreal RetortTemp1, qreal RetortTemp2, qreal RetortTemp3, qreal Current)
{
    mp_RetortTemp1->setText(QString("  Retort1 : %1").arg(RetortTemp1));
    mp_RetortTemp2->setText(QString("  Retort2 : %1").arg(RetortTemp2));
    mp_RetortTemp3->setText(QString("  Retort3 : %1").arg(RetortTemp3));
    mp_RetortCurrent->setText(QString("  Current : %1A").arg(Current));
}

void CSVCDashboardWidget::UpdateRotaryValveLabel(qreal RVPosition, qreal RVTemp, qreal RVCurrent)
{
    mp_RotaryValvePosition->setText(QString("  Position : %1").arg(RVPosition));
    mp_RotaryValveTemp->setText(QString("  Temp : %1").arg(RVTemp));
    mp_RotaryValveCurrent->setText(QString("  Current : %1A").arg(RVCurrent));
}

void CSVCDashboardWidget::UpdateAirHeatingTubeLabel(qreal Temp, qreal Current)
{
    mp_AirHeatingTubeTemp->setText(QString("   Temp : %1").arg(Temp));
    mp_AirHeatingTubeCurrent->setText(QString("  Current : %1A").arg(Current));
}

void CSVCDashboardWidget::UpdateLiquidHeatingTubeLabel(qreal Temp, qreal Current)
{
    mp_HeatingTubeTemp->setText(QString("   Temp : %1").arg(Temp));
    mp_HeatingTubeCurrent->setText(QString("  Current : %1A").arg(Current));
}

void CSVCDashboardWidget::UpdatePressureLabel(qreal Pressure)
{
    mp_PressureLabel->setText(QString(" %1kPa").arg(Pressure));
}
