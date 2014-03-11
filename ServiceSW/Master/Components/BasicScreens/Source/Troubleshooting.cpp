/****************************************************************************/
/*! \file Troubleshooting.cpp
 *
 *  \brief Troubleshooting implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-05
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "BasicScreens/Include/Troubleshooting.h"
#include "ui_Troubleshooting.h"
#include <QDebug>
#include <QDateTime>

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CTroubleshooting::CTroubleshooting(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CTroubleshooting)
{
    mp_Ui->setupUi(this);

    mp_TableWidget = new MainMenu::CBaseTable;
    //TODO: remove this later
    for (qint32 i = 0; i < 20; i++) {
        m_Model.appendRow(new QStandardItem("Test " + QString::number(i)));
    }
    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->selectRow(0);
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_Dialog = new BasicScreens::CTroublesList(this);

    if (!connect(mp_Ui->pushButton, SIGNAL(clicked()), this, SLOT(ExecDialog()))) {
        qDebug() << "CTroubleshooting: cannot connect 'clicked' signal";
    }
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTroubleshooting::~CTroubleshooting()
{
    try {
        delete mp_Dialog;
        delete mp_TableWidget;
        delete mp_Ui;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Starts the dialog listing the device errors
 */
/****************************************************************************/
void CTroubleshooting::ExecDialog()
{
    QTime Time(0, 0, 0);

    mp_Dialog->SetDialogTitle(m_Model.itemFromIndex(mp_TableWidget->currentIndex())->text());

    //TODO: remove this later
    for (qint32 i = 0; i < 20; i++) {
        mp_Dialog->AddItem(Time.toString(Qt::ISODate), "Device " + QString::number(i), QString::number(i), "Error " + QString::number(i), "");
        Time = Time.addSecs(1);
    }

    mp_Dialog->resize(451, 451);
    mp_Dialog->show();
}

} // end namespace BasicScreens
