/****************************************************************************/
/*! \file DashboardDateTimeWidget.h
 *
 *  \brief DashboardDateTimeWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 27-Mar-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DASHBOARD_DATETIMEWIDGET_H
#define DASHBOARD_DATETIMEWIDGET_H


#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/DateTime.h"
#include "Application/Include/LeicaStyle.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/WheelPanel.h"
#include "MainMenu/Include/MessageDlg.h"

namespace Dashboard {

namespace Ui {
    class CDashboardDateTimeWidget;
}

/****************************************************************************/
/**
 * \brief Frame for the date and time widget
 */
/****************************************************************************/
class CDashboardDateTimeWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

public:
    explicit CDashboardDateTimeWidget(QWidget *p_Parent = 0);
    virtual ~CDashboardDateTimeWidget();
    void RefreshDateTime(Global::TimeFormat TimeFormat = Global::TIME_24);
    void SetASAPDateTime(const QDateTime& DateTime);
    void show();
    static QString SELECTED_PROGRAM_NAME;


private:

    Ui::CDashboardDateTimeWidget *mp_Ui;            //!< User interface
    MainMenu::CScrollWheel *mp_DayWheel;    //!< Day scroll wheel
    MainMenu::CScrollWheel *mp_MonthWheel;  //!< Month scroll wheel
    MainMenu::CScrollWheel *mp_YearWheel;   //!< Year scroll wheel
    MainMenu::CScrollWheel *mp_HourWheel;   //!< Hour scroll wheel
    MainMenu::CScrollWheel *mp_MinWheel;    //!< Minute scroll wheel
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    //Flags
    bool m_ProcessRunning;                  //!< Process running state
    QDateTime m_selDateTime;                //!< Selected Date and Time
    QDateTime m_ASAPDateTime;
    QString m_strEndTimeForProgram;
    void RetranslateUI();

protected:
    void changeEvent(QEvent *p_Event);

public slots:
    void OnOK();
    void OnCancel();
    void UpdateProgramName();
    void OnSetASAPDateTime();

signals:
    void OnSelectDateTime(const QDateTime& selDateTime);

};

} // end namespace Dashboard

#endif // DASHBOARD_DATETIMEWIDGET_H
