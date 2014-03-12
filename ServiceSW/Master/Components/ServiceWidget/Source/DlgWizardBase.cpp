/****************************************************************************/
/*! \file DlgWizardBase.cpp
 *
 *  \brief Implementation of base class for wizard dialog.
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

#include "ServiceWidget/Include/DlgWizardBase.h"
#include "ui_DlgWizardBase.h"

namespace MainMenu {

CDlgWizardBase::CDlgWizardBase(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::CDlgWizardBase)
{
    ui->setupUi(this->GetContentFrame());

    setModal(true);

    (void)connect(ui->pushNext,
                  SIGNAL(clicked()),
                  this,
                  SLOT(accept()) );

    (void)connect(ui->pushCancel,
                  SIGNAL(clicked()),
                  this,
                  SLOT(reject()) );
}

CDlgWizardBase::~CDlgWizardBase()
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
void CDlgWizardBase::AddWidget(QWidget *pContent)
{
    ui->layoutContent->addWidget(pContent);
}

/****************************************************************************/
/*!
 *  \brief Add a third button to the dialog
 *
 *  \iparam pButton = Third button to be added.
 */
/****************************************************************************/
void CDlgWizardBase::AddButton(QPushButton *pButton)
{
    ui->layoutMoreButtons->addWidget(pButton);
}

/****************************************************************************/
/*!
 *  \brief Show button "Next"
 *
 */
/****************************************************************************/
void CDlgWizardBase::ShowNext(void)
{
    ui->pushNext->show();
}

/****************************************************************************/
/*!
 *  \brief Show button "Cancel"
 *
 */
/****************************************************************************/
void CDlgWizardBase::ShowCancel(void)
{
    ui->pushCancel->show();
}

/****************************************************************************/
/*!
 *  \brief Hide button "Next"
 *
 */
/****************************************************************************/
void CDlgWizardBase::HideNext(void)
{
    ui->pushNext->hide();
}

/****************************************************************************/
/*!
 *  \brief Hide button "Cancel"
 *
 */
/****************************************************************************/
void CDlgWizardBase::HideCancel(void)
{
    ui->pushCancel->hide();
}

/****************************************************************************/
/*!
 *  \brief Enable button "Next"
 *
 */
/****************************************************************************/
void CDlgWizardBase::EnableNext(void)
{
    ui->pushNext->setEnabled(true);
}

/****************************************************************************/
/*!
 *  \brief Enable button "Cancel"
 *
 */
/****************************************************************************/
void CDlgWizardBase::EnableCancel(void)
{
    ui->pushCancel->setEnabled(true);
}

/****************************************************************************/
/*!
 *  \brief Disable button "Next"
 *
 */
/****************************************************************************/
void CDlgWizardBase::DisableNext(void)
{
    ui->pushNext->setEnabled(false);
}

/****************************************************************************/
/*!
 *  \brief Disable button "Cancel"
 *
 */
/****************************************************************************/
void CDlgWizardBase::DisableCancel(void)
{
    ui->pushCancel->setEnabled(false);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CDlgWizardBase::changeEvent(QEvent *p_Event)
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
