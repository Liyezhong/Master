/****************************************************************************/
/*! \file DlgConfirmationBase.cpp
 *
 *  \brief Implementation of base class for confirmation dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "ServiceWidget/Include/DlgConfirmationBase.h"
#include "ui_DlgConfirmationBase.h"

namespace MainMenu {

CDlgConfirmationBase::CDlgConfirmationBase(QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent),
    ui(new Ui::CDlgConfirmationBase)
{
    ui->setupUi(this->GetContentFrame());

    setModal(true);

    (void)connect(ui->pushYes, SIGNAL(clicked()), this, SLOT(accept()) );
    (void)connect(ui->pushNo, SIGNAL(clicked()), this, SLOT(reject()) );
}

CDlgConfirmationBase::~CDlgConfirmationBase()
{
    delete ui;
}

/****************************************************************************/
/*!
 *  \brief Add a child widget to the dialog
 *
 *  \iparam pContent = Child widget to be added.
 */
/****************************************************************************/
void CDlgConfirmationBase::AddWidget(QWidget *pContent)
{
    ui->layoutContent->addWidget(pContent);
}

/****************************************************************************/
/*!
 *  \brief Remove a child widget from the dialog
 *
 *  \iparam pContent = Child widget to be removed.
 */
/****************************************************************************/
void CDlgConfirmationBase::RemoveWidget(QWidget *pContent)
{
    ui->layoutContent->removeWidget(pContent);
}

/****************************************************************************/
/*!
 *  \brief Show button "Yes"
 *
 */
/****************************************************************************/
void CDlgConfirmationBase::ShowYes(void)
{
    ui->pushYes->show();
}

/****************************************************************************/
/*!
 *  \brief Show button "No"
 *
 */
/****************************************************************************/
void CDlgConfirmationBase::ShowNo(void)
{
    ui->pushNo->show();
}

/****************************************************************************/
/*!
 *  \brief Hide button "Yes"
 *
 */
/****************************************************************************/
void CDlgConfirmationBase::HideYes(void)
{
    ui->pushYes->hide();
}

/****************************************************************************/
/*!
 *  \brief Hide button "No"
 *
 */
/****************************************************************************/
void CDlgConfirmationBase::HideNo(void)
{
    ui->pushNo->hide();
}

/****************************************************************************/
/*!
 *  \brief Enable button "Yes"
 *
 */
/****************************************************************************/
void CDlgConfirmationBase::EnableYes(void)
{
    ui->pushYes->setEnabled(true);
}

/****************************************************************************/
/*!
 *  \brief Enable button "No"
 *
 */
/****************************************************************************/
void CDlgConfirmationBase::EnableNo(void)
{
    ui->pushNo->setEnabled(true);
}

/****************************************************************************/
/*!
 *  \brief Disable button "Yes"
 *
 */
/****************************************************************************/
void CDlgConfirmationBase::DisableYes(void)
{
    ui->pushYes->setEnabled(false);
}

/****************************************************************************/
/*!
 *  \brief Disable button "No"
 *
 */
/****************************************************************************/
void CDlgConfirmationBase::DisableNo(void)
{
    ui->pushNo->setEnabled(false);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CDlgConfirmationBase::changeEvent(QEvent *p_Event)
{
    MainMenu::CDialogFrame::changeEvent(p_Event);

    switch (p_Event->type())
    {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

} // namespace MainMenu
