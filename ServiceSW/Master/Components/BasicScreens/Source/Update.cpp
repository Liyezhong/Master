/****************************************************************************/
/*! \file Update.cpp
 *
 *  \brief Update implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-22
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

#include "BasicScreens/Include/Update.h"
#include "ui_Update.h"
#include <QDebug>

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CUpdate::CUpdate(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CUpdate)
{
    mp_Ui->setupUi(this);

    mp_Dialog = new MainMenu::CWaitDialog(this);

    mp_Dialog->SetDialogTitle("Update");
    mp_Dialog->SetText("Updating ...");

    if (!connect(mp_Ui->updateButton, SIGNAL(clicked()), this, SLOT(ExecDialog()))) {
        qDebug() << "CUpdate: cannot connect 'Closed' signal";
    }
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CUpdate::~CUpdate()
{
    try {
        delete mp_Dialog;
        delete mp_Ui;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Start the wait dialog
 */
/****************************************************************************/
void CUpdate::ExecDialog()
{
    mp_Dialog->show();
}

} // end namespace BasicScreens
