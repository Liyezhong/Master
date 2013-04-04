/****************************************************************************/
/*! \file DashboardEndTimeWidget.h
 *
 *  \brief DashboardComboBox definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 19-Mar-2013
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

#ifndef DASHBOARD_DASHBOARDENDTIMEWIDGET_H
#define DASHBOARD_DASHBOARDENDTIMEWIDGET_H


#include <QWidget>
#include "Dashboard/Include/DashboardDateTimeWidget.h"

namespace Dashboard {

namespace Ui {
    class CDashboardEndTimeWidget;
}

/****************************************************************************/
/**
 * \brief This is a touchscreen optimized customized widget for showing
 *  end time, reagent name, step time, progress bar of the currently running
 *  program on the dashboard
 */
/****************************************************************************/
class CDashboardEndTimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDashboardEndTimeWidget(QWidget *p_Parent = NULL);
    ~CDashboardEndTimeWidget();


protected:
    void paintEvent(QPaintEvent *);

public slots:
    void OnEndTimeButtonClicked();
    void UpdateDateTime(QDateTime & selDateTime);

private:
    Ui::CDashboardEndTimeWidget *mp_Ui;
    QPixmap         m_btnPixmap;
    QPixmap         m_progBarPixmap;
    QPixmap         m_backgroundPixmap;
    Dashboard::CDashboardDateTimeWidget *mp_wdgtDateTime;

};

} // end namespace Dashboard

#endif // DASHBOARD_DASHBOARDENDTIMEWIDGET_H
