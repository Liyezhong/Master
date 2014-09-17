#ifndef SVCDASHBOARDWIDGET_H
#define SVCDASHBOARDWIDGET_H

#include <QWidget>
#include <QTimer>

class QGraphicsScene;
class CGraphicsItemPart;

namespace SVCDiagnostics {

namespace Ui {
    class CSVCDashboardWidget;
}

class CSVCDashboardWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CSVCDashboardWidget(QWidget *parent = 0);
    ~CSVCDashboardWidget();
    
private:
    CGraphicsItemPart* CreatePart(const QString& partResName, const QPoint& pos);
    void paintEvent(QPaintEvent *p_PaintEvent);
    Ui::CSVCDashboardWidget *ui;
    QGraphicsScene *m_pScene;
    CGraphicsItemPart * m_pRetort;
    CGraphicsItemPart * m_pOven;
    CGraphicsItemPart * m_pRotaryValve;
    CGraphicsItemPart * m_pHeatingTube;
    CGraphicsItemPart * m_pAirHeatingTube;

private slots:

};

}//end of namespace

#endif // SVCDASHBOARDWIDGET_H
