#ifndef SVCDASHBOARDWIDGET_H
#define SVCDASHBOARDWIDGET_H

#include <QWidget>
#include <QPushButton>
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
    CGraphicsItemPart* CreatePart(const QString& partResName, const QPoint& pos, bool Clickable = true);
    void paintEvent(QPaintEvent *p_PaintEvent);
    Ui::CSVCDashboardWidget *ui;
    QGraphicsScene *mp_Scene;
    CGraphicsItemPart* mp_Retort;
    CGraphicsItemPart* mp_Oven;
    CGraphicsItemPart* mp_RotaryValve;
    CGraphicsItemPart* mp_HeatingTube;
    CGraphicsItemPart* mp_AirHeatingTube;
    CGraphicsItemPart* mp_Fan;
    CGraphicsItemPart* mp_WaxTrap;
    CGraphicsItemPart* mp_Pump;
    CGraphicsItemPart* mp_GV1;
    CGraphicsItemPart* mp_GV2;
    CGraphicsItemPart* mp_Pressure;
    CGraphicsItemPart* mp_RFV1;

    CGraphicsItemPart* mp_Filter;
    CGraphicsItemPart* mp_Connect1;
    CGraphicsItemPart* mp_Connect2;
    CGraphicsItemPart* mp_Connect3;

    QPushButton* mp_SelectBtn;
    QPushButton* mp_ValveInfoBtn;

private slots:
    void RetortSelected();
};

}//end of namespace

#endif // SVCDASHBOARDWIDGET_H
