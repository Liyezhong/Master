/****************************************************************************/
/*! \file TroublesList.cpp
 *
 *  \brief TroublesList implementation.
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

#include "BasicScreens/Include/TroublesList.h"
#include "ui_TroublesList.h"
#include <QDebug>

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CTroublesList::CTroublesList(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CTroublesList)
{
    mp_Ui->setupUi(GetContentFrame());

    mp_TableWidget = new MainMenu::CBaseTable;
    m_Model.setHorizontalHeaderLabels(QStringList() << "Timestamp" << "Origin" << "ID" << "Description");
    mp_TableWidget->horizontalHeader()->show();
    mp_TableWidget->setModel(&m_Model);
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_Dialog = new MainMenu::CTextDialog(this);

    if (!connect(mp_Ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()))) {
        qDebug() << "CTroublesList: cannot connect 'clicked' signal";
    }
    if (!connect(mp_Ui->openButton, SIGNAL(clicked()), this, SLOT(ExecDialog()))) {
        qDebug() << "CTroublesList: cannot connect 'clicked' signal";
    }
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTroublesList::~CTroublesList()
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
 *  \brief Adds an entry to the error list
 *
 *  \iparam Timestamp = Time the error occured
 *  \iparam Origin = Assembly group or module responsible
 *  \iparam Id = Error ID
 *  \iparam Description = Short error description
 *  \iparam PossibleCauses = Possible cause for the error
 */
/****************************************************************************/
void CTroublesList::AddItem(QString Timestamp, QString Origin, QString Id, QString Description, QString PossibleCauses)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(Timestamp);
    ItemList << new QStandardItem(Origin);
    ItemList << new QStandardItem(Id);
    ItemList << new QStandardItem(Description);
    m_Model.appendRow(ItemList);
    mp_TableWidget->selectRow(0);
}

/****************************************************************************/
/*!
 *  \brief Starts the dialog displaying possible causes
 */
/****************************************************************************/
void CTroublesList::ExecDialog()
{
    //TODO: remove this later
    QString Text;
    for (qint32 i = 0; i < 10; i++) {
        for (qint32 j = 0; j < i; j++) {
            Text.append(m_Model.item(mp_TableWidget->currentIndex().row(), 3)->text() + " ");
        }
        Text.append("\n");
    }
    mp_Dialog->SetText(Text);
    mp_Dialog->SetCaption("Possible Causes:");
    mp_Dialog->SetDialogTitle(m_Model.item(mp_TableWidget->currentIndex().row(), 3)->text());
    mp_Dialog->resize(408, 408);
    mp_Dialog->show();
}

} // end namespace BasicScreens
