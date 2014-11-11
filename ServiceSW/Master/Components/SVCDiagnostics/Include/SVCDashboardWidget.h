#ifndef SVCDASHBOARDWIDGET_H
#define SVCDASHBOARDWIDGET_H

#include <QWidget>
#include <QTimer>

#include "SVCDiagnostics/Include/SVCTargetTempSelectionDlg.h"

class QGraphicsScene;
class CGraphicsItemPart;
class SVCButton;
class SVCLabel;

namespace SVCDiagnostics {

namespace Ui {
    class CSVCDashboardWidget;
}

class CSVCDashboardWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CSVCDashboardWidget(QWidget *p_Parent = 0);
    ~CSVCDashboardWidget();

private slots:
    void RetortSelected();

public slots:
    void UpdateOvenLabel(qreal OvenTemp1, qreal OvenTemp2, qreal OvenTemp3, qreal Current);
    void UpdateRetortLabel(qreal RetortTemp1, qreal RetortTemp2, qreal RetortTemp3, qreal Current);
    void UpdateRotaryValveLabel(qreal RVPosition, qreal RVTemp, qreal RVCurrent);

    void UpdateAirHeatingTubeLabel(qreal Temp, qreal Current);
    void UpdateLiquidHeatingTubeLabel(qreal Temp, qreal Current);
    void UpdatePressureLabel(qreal Pressure);
    
private:
    void InitLabel();
    CGraphicsItemPart* CreatePart(const QString& partResName, const QPoint& pos, bool Clickable = true);
    void paintEvent(QPaintEvent *p_PaintEvent);

    Ui::CSVCDashboardWidget *mp_Ui;
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
    CGraphicsItemPart* mp_Line1;

    SVCButton* mp_SelectBtn;
    SVCButton* mp_ValveInfoBtn;

    SVCLabel* mp_HeatingTubeTemp;
    SVCLabel* mp_HeatingTubeCurrent;

    SVCLabel* mp_AirHeatingTubeTemp;
    SVCLabel* mp_AirHeatingTubeCurrent;

    SVCLabel* mp_OvenTemp1;
    SVCLabel* mp_OvenTemp2;
    SVCLabel* mp_OvenTemp3;
    SVCLabel* mp_OvenCurrent;

    SVCLabel* mp_RetortTemp1;
    SVCLabel* mp_RetortTemp2;
    SVCLabel* mp_RetortTemp3;
    SVCLabel* mp_RetortCurrent;

    SVCLabel* mp_RotaryValvePosition;
    SVCLabel* mp_RotaryValveTemp;
    SVCLabel* mp_RotaryValveCurrent;

    SVCLabel* mp_PressureLabel;

    CSVCTargetTempSelectionDlg* mp_TempSelectionDlg;
};

}//end of namespace

#endif // SVCDASHBOARDWIDGET_H
