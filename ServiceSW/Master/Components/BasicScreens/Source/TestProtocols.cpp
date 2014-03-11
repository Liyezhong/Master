/****************************************************************************/
/*! \file TestProtocols.cpp
 *
 *  \brief TestProtocols implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-08
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

#include "BasicScreens/Include/TestProtocols.h"
#include "ui_TestProtocols.h"
#include <QDebug>

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CTestProtocols::CTestProtocols(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CTestProtocols)
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

    mp_Dialog = new MainMenu::CTextDialog(this);
    mp_Dialog->SetCaption("Test Protocol:");

    if (!connect(mp_Ui->pushButton, SIGNAL(clicked()), this, SLOT(ExecDialog()))) {
        qDebug() << "CTestProtocols: cannot connect 'clicked' signal";
    }
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTestProtocols::~CTestProtocols()
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
 *  \brief Shows the dialog displaying the log file
 */
/****************************************************************************/
void CTestProtocols::ExecDialog()
{
    //TODO: remove this later
    QString Text;
    for (qint32 i = 0; i < 100; i++) {
        for (qint32 j = 0; j < i; j++) {
            Text.append(m_Model.itemFromIndex(mp_TableWidget->currentIndex())->text() + " ");
        }
        Text.append("\n");
    }
    mp_Dialog->SetText(Text);
    mp_Dialog->SetDialogTitle(m_Model.itemFromIndex(mp_TableWidget->currentIndex())->text());
    mp_Dialog->resize(458, 458);
    mp_Dialog->show();
}

} // end namespace BasicScreens
