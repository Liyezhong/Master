#include "SVCDiagnostics/Include/SVCDashboardWidget.h"
#include "ui_SVCDashboardWidget.h"
#include <QPainter>
#include <Global/Include/Utils.h>
#include <QPushButton>
#include <QGraphicsScene>
#include "SVCDiagnostics/Include/GraphicsItemPart.h"

using namespace SVCDiagnostics;

CSVCDashboardWidget::CSVCDashboardWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSVCDashboardWidget)
{
    ui->setupUi(this);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    //ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    mp_Scene = new QGraphicsScene(ui->graphicsView);
    mp_Scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    mp_Scene->setSceneRect(0, 0, 772, 490);
    mp_Scene->setBackgroundBrush(QImage(":/Images/SVCDiagnosticsBackground.png"));
    ui->graphicsView->setScene(mp_Scene);

    mp_Retort = CreatePart("Retort", QPoint(406, 30));
    mp_Oven = CreatePart("Oven", QPoint(7, 70));
    mp_RotaryValve = CreatePart("RotaryValve", QPoint(219, 154));

    mp_AirHeatingTube = CreatePart("AirHeatingTube", QPoint(489, 50));
    mp_HeatingTube = CreatePart("HeatingTube", QPoint(250, 116));

    mp_Fan = CreatePart("Fan", QPoint(266, 20));
    mp_WaxTrap = CreatePart("WaxTrap", QPoint(666, 109));
    mp_Pump = CreatePart("Pump", QPoint(404, 288));
    mp_GV1 = CreatePart("GV1", QPoint(485, 192));
    mp_GV2 = CreatePart("GV2", QPoint(640, 245));

    mp_Pressure = CreatePart("Pressure", QPoint(615,75));
    mp_RFV1 = CreatePart("RFV1", QPoint(606, 160));

    mp_Filter = CreatePart("Filter", QPoint(269, 80));

    CONNECTSIGNALSLOT(mp_Retort, PartSelected(), this, RetortSelected());

    //QPushButton* SelectBtn = new QPushButton;
    //SelectBtn->setObjectName("selectBtn");
    //SelectBtn->setText("");
}

CSVCDashboardWidget::~CSVCDashboardWidget()
{
    delete ui;
    delete mp_Scene;
}

CGraphicsItemPart* CSVCDashboardWidget::CreatePart(const QString& partResName, const QPoint& pos)
{
    QString normalImg = ":/Images/" + partResName + ".png";
    QString disabledImg = ":/Images/" + partResName + "Disabled.png";
    QString glowImg = ":/Images/" + partResName + "Glow.png";
    CGraphicsItemPart* pGraphicsItemPart = new CGraphicsItemPart(QPixmap(normalImg),
                                             QPixmap(disabledImg),
                                             QPixmap(glowImg));
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
