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

#include "Dashboard/Include/DashboardEndTimeWidget.h"

namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/

CDashboardEndTimeWidget::CDashboardEndTimeWidget(QWidget *p_Parent) :QWidget(p_Parent)

{
    m_btnPixmap.load(":/HimalayaImages/LongButton/LongButton_Enabled.png");
    m_progBarPixmap.load(":/HimalayaImages/Icons/Dashboard/ProgressLine/ProgressLine_Background.png");



    setMinimumSize(QSize(250, 170));
    InitializeEndTimeWidgetItems();

}

CDashboardEndTimeWidget::~CDashboardEndTimeWidget()
{
    try {

        delete mp_btnEndTime;
        delete mp_lblReagent;
        delete mp_lblReagentName;
        delete mp_lblTime;
        delete mp_lblTimeValue;

    } catch(...) {

    }

}

void CDashboardEndTimeWidget::InitializeEndTimeWidgetItems()
{
    mp_btnEndTime = new QPushButton(this);
    mp_btnEndTime->setGeometry(QRect(5, 2, 225, 70));
    mp_btnEndTime->setFlat(true);
    mp_btnEndTime->setIconSize(QSize(225, 70));
    mp_btnEndTime->setIcon(QIcon(m_btnPixmap));


    mp_lblReagent = new QLabel("Reagent :", this);
    mp_lblReagent->setGeometry(QRect(10, 60, 100, 30));

    mp_lblReagentName = new QLabel("<Reagent Name>", this);
    mp_lblReagentName->setGeometry(QRect(110, 60, 140, 20));

    mp_lblTime = new QLabel("Step Time :", this);
    mp_lblTime->setGeometry((QRect(10, 100, 100, 20)));

    mp_lblTimeValue = new QLabel("00:00", this);
    mp_lblTimeValue->setGeometry(QRect(110, 100, 110, 20));

    update();

}

void CDashboardEndTimeWidget::paintEvent(QPaintEvent *)
{
    QPainter        m_Painter(this);    
    m_Painter.drawPixmap(rect(), QPixmap(":/HimalayaImages/Icons/Dashboard/EndTime/EndTime_Background.png"));

    m_Painter.drawPixmap(10, 140, m_progBarPixmap);
    //m_Painter.fillRect(QRect(10, 140, m_progBarPixmap.width(), m_progBarPixmap.height()), QBrush(Qt::green));


}

}    // end of namespace Dashboard
