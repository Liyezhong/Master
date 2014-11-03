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

    m_pScene = new QGraphicsScene(ui->graphicsView);
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_pScene->setSceneRect(0, 0, 772, 490);
    m_pScene->setBackgroundBrush(QImage(":/Images/SVCDiagnosticsBackground.png"));
    ui->graphicsView->setScene(m_pScene);

    m_pRetort = CreatePart("Retort", QPoint(406, 30));
    m_pOven = CreatePart("Oven", QPoint(7, 70));
    m_pRotaryValve = CreatePart("RotaryValve", QPoint(219, 154));

    m_pAirHeatingTube = CreatePart("AirHeatingTube", QPoint(489, 50));
    m_pHeatingTube = CreatePart("HeatingTube", QPoint(250, 116));
    m_pFan = CreatePart("Fan", QPoint(266, 25));
    m_pWaxTrap = CreatePart("WaxTrap", QPoint(666, 109));

    CONNECTSIGNALSLOT(m_pRetort, PartSelected(), this, RetortSelected());
}

CSVCDashboardWidget::~CSVCDashboardWidget()
{
    delete ui;
    delete m_pScene;
}

CGraphicsItemPart* CSVCDashboardWidget::CreatePart(const QString& partResName, const QPoint& pos)
{
    QString normalImg = ":/Images/" + partResName + ".png";
    QString disabledImg = ":/Images/" + partResName + "Disabled.png";
    QString glowImg = ":/Images/" + partResName + "Glow.png";
    CGraphicsItemPart* pGraphicsItemPart = new CGraphicsItemPart(QPixmap(normalImg),
                                             QPixmap(disabledImg),
                                             QPixmap(glowImg));
    m_pScene->addItem(pGraphicsItemPart);
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

    CGraphicsItemPart::PartStatus Status = m_pRetort->Status();

    qDebug()<<"get retort status:"<<Status;

}
