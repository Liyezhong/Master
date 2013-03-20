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

CDashboardComboBox::CDashboardComboBox(QWidget *p_Parent): QComboBox(p_Parent)
{
    mp_Msg = new QMessageBox();
    //setMinimumSize(0,53);
    //setIconSize(QSize(300, 53));
    //setCurrentIndex(0);
}

CDashboardComboBox::~CDashboardComboBox()
{
    try {
        delete mp_Msg;
    } catch(...) {

    }

}

void CDashboardComboBox::showPopup()
{
    QComboBox::showPopup();
    mp_Popup = this->findChild<QFrame *>();
    mp_Popup->move( mp_Popup->x(), mp_Popup->y() - this->height() - mp_Popup->height() );
}

void CDashboardComboBox::handleSelectionChanged(int index)
{

    mp_Msg->setWindowTitle("Hello !");
    mp_Msg->setText("Selected Index is :"+QString::number(index));
    mp_Msg->show();
}


}

