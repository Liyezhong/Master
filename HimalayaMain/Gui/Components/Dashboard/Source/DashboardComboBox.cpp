/****************************************************************************/
/*! \file DashboardComboBox.cpp
 *
 *  \brief DashboardComboBox implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 08-Feb-2013
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

#include "Dashboard/Include/DashboardComboBox.h"
#include <QDebug>

namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/

CDashboardComboBox::CDashboardComboBox(QWidget *p_Parent):
    QComboBox(p_Parent), m_IsWorkAsButton(false)
{

    //mp_ListView = new QListView(this);
    //mp_ListView->setStyleSheet("QListView::item { border-bottom: 5px solid white; margin:3px; }" \
                               //"QListView::item:selected { border-bottom: 5px solid black; margin:3px;" \
                               //"color: black;}"
                                //);
   // setView(mp_ListView);
    m_SelProgName = tr("Program");
}

CDashboardComboBox::~CDashboardComboBox()
{
    try {
    } catch(...) {

    }

}

void CDashboardComboBox::showPopup()
{
    QComboBox::showPopup();
    mp_Popup = this->findChild<QFrame *>();
    mp_Popup->move( mp_Popup->x(), mp_Popup->y() - this->height() - mp_Popup->height() );
}

void CDashboardComboBox::hidePopup()
{
    QComboBox::hidePopup();
}

void CDashboardComboBox::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter Painter(this);
    QFont TextFont;
    Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/ComboButton/ComboButton_Button_Enable.png"));
    Painter.setRenderHint(QPainter::Antialiasing);
    TextFont.setPointSize(14);
    Painter.setFont(TextFont);
    Painter.setPen(Qt::white);
    Painter.drawText(rect(), Qt::AlignCenter, m_SelProgName);

}

void CDashboardComboBox::mousePressEvent(QMouseEvent *e)
{
    if (!m_IsWorkAsButton)
      QComboBox::mousePressEvent(e);
    else
        emit ButtonPress();

}

void CDashboardComboBox::UpdateSelectedProgramName(QString &SelProgramName)
{
    m_SelProgName = SelProgramName;
    update();
}

void  CDashboardComboBox::WorkAsButton(bool bSet)
{
    m_IsWorkAsButton = bSet;
}

}

