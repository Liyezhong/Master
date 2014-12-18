/****************************************************************************/
/*! \file SVCDashboardWidget.h
 *
 *  \brief SVC Diagnostics dashboard widget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-02
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
#ifndef SVCDASHBOARDWIDGET_H
#define SVCDASHBOARDWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QEventLoop>

class QGraphicsScene;
class CGraphicsItemPart;
class SVCButton;
class SVCLabel;

namespace Diagnostics {
    class CDiagnosticMessageDlg;
}

namespace SVCDiagnostics {

namespace Ui {
    class CSVCDashboardWidget;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CSVCDashboardWidget
 */
/****************************************************************************/
class CSVCDashboardWidget : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CSVCDashboardWidget
     *  \param p_Parent = parent widget
     *  \return from CSVCDashboardWidget
     */
    /****************************************************************************/
    explicit CSVCDashboardWidget(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CSVCDashboardWidget();

private slots:
    /****************************************************************************/
    /*!
     *  \brief Slot for refresh label status
     */
    /****************************************************************************/
    void RefreshLabel();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the Retort image
     */
    /****************************************************************************/
    void RetortSelected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the oven image
     */
    /****************************************************************************/
    void OvenSelected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the rotary valve image
     */
    /****************************************************************************/
    void RotaryValveSelected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the air heating tube image
     */
    /****************************************************************************/
    void AirTubeSelected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the liquid heating tube image
     */
    /****************************************************************************/
    void LiquidTubeSelected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the Fan image
     */
    /****************************************************************************/
    void FanSelected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the Valve1 image
     */
    /****************************************************************************/
    void Valve1Selected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the Valve2 image
     */
    /****************************************************************************/
    void Valve2Selected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the Pump image
     */
    /****************************************************************************/
    void PumpSelected();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the Select Position button
     */
    /****************************************************************************/
    void OnSelectPosition();

    /****************************************************************************/
    /*!
     *  \brief This function is called when clicked the Valve State Info button
     */
    /****************************************************************************/
    void OnValveStateInfo();

public slots:
    /****************************************************************************/
    /*!
     *  \brief Slot for update Oven labels
     *  \iparam OvenTemp1 = The value of oven temperature 1
     *  \iparam OvenTemp2 = The value of oven temperature 2
     *  \iparam OvenTemp3 = The value of oven temperature 3
     *  \iparam Current   = The value of oven Current
     */
    /****************************************************************************/
    void UpdateOvenLabel(qreal OvenTemp1, qreal OvenTemp2, qreal OvenTemp3, qreal Current);

    /****************************************************************************/
    /*!
     *  \brief Slot for update Retort labels
     *  \iparam RetortTemp1 = The value of Retort temperature 1
     *  \iparam RetortTemp2 = The value of Retort temperature 2
     *  \iparam RetortTemp3 = The value of Retort temperature 3
     *  \iparam Current     = The value of Retort current
     */
    /****************************************************************************/
    void UpdateRetortLabel(qreal RetortTemp1, qreal RetortTemp2, qreal RetortTemp3, qreal Current);

    /****************************************************************************/
    /*!
     *  \brief Slot for update Rotary valve labels
     *  \iparam RVTubeFlag = rotary valve tube flag
     *  \iparam RVPosition = rotary valve position
     *  \iparam RVTemp1    = rotary valve temperature 1
     *  \iparam RVTemp2    = rotary valve temperature 2
     *  \iparam RVCurrent  = rotary valve Current
     */
    /****************************************************************************/
    void UpdateRotaryValveLabel(bool RVTubeFlag, qreal RVPosition, qreal RVTemp1, qreal RVTemp2, qreal RVCurrent);

    /****************************************************************************/
    /*!
     *  \brief Slot for update Air heating tube labels
     *  \iparam Temp = Air heating tube temperature
     *  \iparam Current = Air heating tube Current
     */
    /****************************************************************************/
    void UpdateAirHeatingTubeLabel(qreal Temp, qreal Current);

    /****************************************************************************/
    /*!
     *  \brief Slot for update liquid heating tube labels
     *  \iparam Temp = Liquid heating tube temperature
     *  \iparam Current = Liquid heating tube Current
     */
    /****************************************************************************/
    void UpdateLiquidHeatingTubeLabel(qreal Temp, qreal Current);

    /****************************************************************************/
    /*!
     *  \brief Slot for update pressure labels
     *  \iparam Pressure = The value of pressure
     */
    /****************************************************************************/
    void UpdatePressureLabel(qreal Pressure);
    
    /****************************************************************************/
    /*!
     *  \brief Slot for update all part status
     */
    /****************************************************************************/
    void UpdatePartStatus();

    /****************************************************************************/
    /*!
     *  \brief Slot for timer start
     *  \iparam IsStart = flag of start
     */
    /****************************************************************************/
    void TimerStart(bool IsStart);
private:

    /****************************************************************************/
    /*!
     *  \brief Make position to string.
     *  \iparam TubeFlag = flag of tube position
     *  \iparam Position = target position
     *  \return String of position
     */
    /****************************************************************************/
    QString PostionToStr(bool TubeFlag, qreal Position);

    /****************************************************************************/
    /*!
     *  \brief To show failed dialog
     *  \iparam Title = Dialog title
     *  \iparam Text = Dialog text
     */
    /****************************************************************************/
    void ShowFailedDlg(const QString& Title, const QString& Text);

    /****************************************************************************/
    /*!
     *  \brief Create all label
     */
    /****************************************************************************/
    void InitLabel();

    /****************************************************************************/
    /*!
     *  \brief Create all part
     *  \iparam partResName = the name of the part
     *  \iparam pos = the pos of the part
     *  \iparam Clickable = the flag the part Clickable
     *  \return the pointer of the CGraphiscsItemPart
     */
    /****************************************************************************/
    CGraphicsItemPart* CreatePart(const QString& partResName, const QPoint& _pos, bool Clickable = true);

    /****************************************************************************/
    /*!
     *  \brief Draws the background widget
     *  \iparam p_PaintEvent = Paint event for drawing
     */
    /****************************************************************************/
    void paintEvent(QPaintEvent *p_PaintEvent);

    QTimer*  mp_RefreshTimer;         //!< Refresh label status timer
    Ui::CSVCDashboardWidget *mp_Ui;   //!< User interface

    QGraphicsScene *mp_Scene;         //!< The graphics scene
    Diagnostics::CDiagnosticMessageDlg* mp_MsgDlg; //!< Show message dialog
    CGraphicsItemPart* mp_Retort;     //!< retort item part
    CGraphicsItemPart* mp_Oven;       //!< oven item part
    CGraphicsItemPart* mp_RotaryValve;  //!< rotary valve item part
    CGraphicsItemPart* mp_HeatingTube;  //!< liquid heating tube item part
    CGraphicsItemPart* mp_AirHeatingTube;   //!< air heating tube item part
    CGraphicsItemPart* mp_Fan;      //!< exhaust fan item part
    CGraphicsItemPart* mp_WaxTrap;  //!< wax trap item part
    CGraphicsItemPart* mp_Pump;     //!< pump item part
    CGraphicsItemPart* mp_GV1;      //!< GV1 item part
    CGraphicsItemPart* mp_GV2;      //!< GV2 item part
    CGraphicsItemPart* mp_Pressure; //!< pressure item part
    CGraphicsItemPart* mp_RFV1;     //!< rfv1 item part

    CGraphicsItemPart* mp_Filter;    //!< filter item part
    CGraphicsItemPart* mp_Connect1;  //!< connect1 item part
    CGraphicsItemPart* mp_Connect2;  //!< connect2 item part
    CGraphicsItemPart* mp_Connect3;  //!< connect3 item part
    CGraphicsItemPart* mp_Line1;     //!< connect line1 item part
    CGraphicsItemPart* mp_Line2;     //!< connect line2 item part
    CGraphicsItemPart* mp_Line3;     //!< connect line3 item part

    CGraphicsItemPart* mp_GV1StateUp;       //!< valve1 up state
    CGraphicsItemPart* mp_GV1StateLeft;     //!< valve1 left state
    CGraphicsItemPart* mp_GV1StateRight;    //!< valve1 right state

    CGraphicsItemPart* mp_GV2StateUp;       //!< valve2 up state
    CGraphicsItemPart* mp_GV2StateLeft;     //!< valve2 left state
    CGraphicsItemPart* mp_GV2StateRight;    //!< valve2 right state

    SVCButton* mp_SelectBtn;         //!< The button of select position
    SVCButton* mp_ValveInfoBtn;      //!< The button of valve state info

    SVCLabel* mp_HeatingTubeTemp;    //!< Liquid heating tube temperature label
    SVCLabel* mp_HeatingTubeCurrent; //!< Liquid heating tube current label

    SVCLabel* mp_AirHeatingTubeTemp;  //!< Air heating tube temperature label
    SVCLabel* mp_AirHeatingTubeCurrent;  //!< Air heating tube current label

    SVCLabel* mp_OvenTemp1;     //!< Oven temperature 1 label
    SVCLabel* mp_OvenTemp2;     //!< Oven temperature 2 label
    SVCLabel* mp_OvenTemp3;     //!< Oven temperature 3 label
    SVCLabel* mp_OvenCurrent;   //!< Oven current label

    SVCLabel* mp_RetortTemp1;   //!< Retort temperature 1 label
    SVCLabel* mp_RetortTemp2;   //!< Retort temperature 2 label
    SVCLabel* mp_RetortTemp3;   //!< Retort temperature 3 label
    SVCLabel* mp_RetortCurrent; //!< Retort current lable

    SVCLabel* mp_RotaryValvePosition;   //!< Rotary valve position label
    SVCLabel* mp_RotaryValveTemp1;      //!< Rotary valve temperature1 label
    SVCLabel* mp_RotaryValveTemp2;      //!< Rotary valve temperature2 label
    SVCLabel* mp_RotaryValveCurrent;    //!< Rotary valve current label

    SVCLabel* mp_PressureLabel;         //!< Pressure label
};

}//end of namespace

#endif // SVCDASHBOARDWIDGET_H
