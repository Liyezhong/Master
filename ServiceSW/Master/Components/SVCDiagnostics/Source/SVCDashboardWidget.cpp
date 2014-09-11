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
    //m_pScene->setBackgroundBrush(QImage(":/Images/SVCDiagnosticsBackground.png"));
    ui->graphicsView->setScene(m_pScene);

    m_pRetort = new CGraphicsItemPart(QPixmap(":/Images/retort.png"));
    m_pScene->addItem(m_pRetort);
    m_pRetort->setPos(0, 0);
}

CSVCDashboardWidget::~CSVCDashboardWidget()
{
    delete ui;
    delete m_pScene;
}

void CSVCDashboardWidget::paintEvent(QPaintEvent* pPaintEvent)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    //style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(pPaintEvent);
}

