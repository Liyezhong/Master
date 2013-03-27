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

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QFormLayout>
#include <QGraphicsItem>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QLineEdit>
//#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Dashboard {

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


private:
    QPushButton     *mp_btnEndTime;
    QLabel          *mp_lblReagent;
    QLabel          *mp_lblReagentName;
    QLabel          *mp_lblTime;
    QLabel          *mp_lblTimeValue;

    QPixmap         m_btnPixmap;
    QPixmap         m_progBarPixmap;


    void InitializeEndTimeWidgetItems();
};

} // end namespace Dashboard

#endif // DASHBOARD_DASHBOARDENDTIMEWIDGET_H
