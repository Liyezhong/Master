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
    explicit CDashboardDateTimeWidget(QString ProgramName, QWidget *p_Parent = 0);
    virtual ~CDashboardDateTimeWidget();
    QDateTime & GetChosenProgamEndTime();


private:

    Ui::CDashboardDateTimeWidget *mp_Ui;            //!< User interface
    //Flags
    bool m_ProcessRunning;                  //!< Process running state
    QDateTime m_selDateTime;

public slots:
    void OnOK();
    void OnCancel();

signals:
    void OnSelectDateTime(QDateTime& selDateTime);

};

} // end namespace Dashboard

#endif // DASHBOARD_DATETIMEWIDGET_H
