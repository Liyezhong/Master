/****************************************************************************/
/*! \file DashboardEndTimeWidget.cpp
 *
 *  \brief DashboardComboBox implementation.
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

#include "ui_DashboardEndTimeWidget.h"
#include <QPainter>
#include "Dashboard/Include/DashboardEndTimeWidget.h"

namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/

CDashboardEndTimeWidget::CDashboardEndTimeWidget(QString & ProgramName, QWidget *p_Parent) : QWidget(p_Parent),
    mp_Ui(new Ui::CDashboardEndTimeWidget)

{
    mp_Ui->setupUi(this);

    m_backgroundPixmap.load(":/HimalayaImages/Icons/Dashboard/EndTime/EndTime_Background.png");
    m_btnPixmap.load(":/HimalayaImages/LongButton/LongButton_Enabled.png");
    m_progBarPixmap.load(":/HimalayaImages/Icons/Dashboard/ProgressLine/ProgressLine_Background.png");


    mp_wdgtDateTime = new Dashboard::CDashboardDateTimeWidget(ProgramName);
    mp_wdgtDateTime->setWindowFlags(Qt::CustomizeWindowHint);

    CONNECTSIGNALSLOT(mp_Ui->btnEndTime, clicked(), this, OnEndTimeButtonClicked());
}

CDashboardEndTimeWidget::~CDashboardEndTimeWidget()
{
    try {
        delete mp_Ui;
    } catch(...) {

    }

}

void CDashboardEndTimeWidget::paintEvent(QPaintEvent *)
{

    QPainter        Painter(this);
    Painter.drawPixmap(rect(), m_backgroundPixmap);

    //m_Painter.fillRect(QRect(10, 140, m_progBarPixmap.width(), m_progBarPixmap.height()), QBrush(Qt::green));

}

void CDashboardEndTimeWidget::OnEndTimeButtonClicked()
{

    mp_wdgtDateTime->show();

    CONNECTSIGNALSLOT(mp_wdgtDateTime, OnSelectDateTime(QDateTime &), this, UpdateDateTime(QDateTime &));

    update();

}


void CDashboardEndTimeWidget::UpdateDateTime(QDateTime &selDateTime)
{

    QString TimeStr = QString(tr("%1").arg(selDateTime.time().toString()));
    QString DateStr = QString(tr("%1").arg(selDateTime.date().toString()));

    QString DateTimeStr;
    DateTimeStr.append(tr("End Time :"));
    DateTimeStr.append(TimeStr);
    DateTimeStr.append("\n");
    DateTimeStr.append("\t\t\t");
    DateTimeStr.append(DateStr);

    mp_Ui->btnEndTime->setText(DateTimeStr);

}

}    // end of namespace Dashboard
