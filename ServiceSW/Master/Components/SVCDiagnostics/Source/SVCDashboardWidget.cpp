#include "SVCDiagnostics/Include/SVCDashboardWidget.h"
#include "ui_SVCDashboardWidget.h"
#include <QPainter>
#include <Global/Include/Utils.h>
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

    mp_Filter = CreatePart("Filter", QPoint(269, 80), false);
    mp_Connect1 = CreatePart("ConnectPart", QPoint(500, 256), false);
    mp_Connect2 = CreatePart("ConnectPart", QPoint(569, 256), false);
    mp_Connect3 = CreatePart("ConnectPart", QPoint(517, 360), false);

    CONNECTSIGNALSLOT(mp_Retort, PartSelected(), this, RetortSelected());

    QFont Font;
    Font.setPointSize(10);
    mp_SelectBtn    = new QPushButton(ui->graphicsView);
    mp_ValveInfoBtn = new QPushButton(ui->graphicsView);
//    QLabel *SelectBtn = new QLabel;
    //SelectBtn->move(QPoint(115, 210));
    mp_SelectBtn->setGeometry(131, 215, 102, 36);
    mp_SelectBtn->setFont(Font);
    //mp_SelectBtn->setObjectName("selectBtn");
    mp_SelectBtn->setText(tr("Select Position"));
    mp_SelectBtn->setBackgroundRole(QPalette::Shadow);

    mp_ValveInfoBtn->setGeometry(380, 420, 180, 40);
    Font.setPointSize(13);
    mp_ValveInfoBtn->setFont(Font);
    mp_ValveInfoBtn->setText(tr("Valve State Info"));
    //QGraphicsProxyWidget* ProBtn = mp_Scene->addWidget(SelectBtn);
    //SelectBtn->
    //ProBtn->setGeometry(0,0, 160, 154);
}

CSVCDashboardWidget::~CSVCDashboardWidget()
{
    try {
        delete ui;
        delete mp_Scene;
        delete mp_SelectBtn;
        delete mp_ValveInfoBtn;
    }
    catch (...) {

    }
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
