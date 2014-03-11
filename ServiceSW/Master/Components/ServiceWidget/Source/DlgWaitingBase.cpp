/****************************************************************************/
/*! \file DlgWaitingBase.cpp
 *
 *  \brief Implementation of base class of waiting dialog.
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

#include "ServiceWidget/Include/DlgWaitingBase.h"

#include <QDebug>

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "ui_DlgWaitingBase.h"

namespace MainMenu {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CDlgWaitingBase::CDlgWaitingBase(QWidget *p_Parent)
    : MainMenu::CDialogFrame(p_Parent)
    , ui(new Ui::CDlgWaitingBase)
    , m_Timer(new QTimer)
{
    ui->setupUi(GetContentFrame());

    m_Timer->setSingleShot(true);

    setModal(true);

    CONNECTSIGNALSLOT(ui->abortButton, clicked(), this, reject());
    CONNECTSIGNALSLOT(m_Timer, timeout(), this, reject());
    CONNECTSIGNALSIGNAL(m_Timer, timeout(), this, Timeout());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDlgWaitingBase::~CDlgWaitingBase()
{
    try
    {
        delete m_Timer;
        delete ui;
    }
    catch (...) {}
}

void CDlgWaitingBase::AddWidget(QWidget *pContent)
{
    ui->layoutContent->addWidget(pContent);
}

/****************************************************************************/
/*!
 *  \brief Sets the text displayed in the wait dialog
 *
 *  \iparam Text = Label text
 */
/****************************************************************************/
void CDlgWaitingBase::SetText(const QString &Text)
{
    ui->contentLabel->setText(tr("%1").arg(Text));
}

/****************************************************************************/
/*!
 *  \brief Activates a timeout timer
 *
 *  \iparam Milliseconds = Timeout in milliseconds
 */
/****************************************************************************/
void CDlgWaitingBase::SetTimeout(qint32 Milliseconds)
{
    m_Timer->start(Milliseconds);
}

/****************************************************************************/
/*!
 *  \brief Sets the text of abort button
 *
 *  \iparam Label = Label text
 */
/****************************************************************************/
void CDlgWaitingBase::SetAbortLabel(const QString &Label)
{
    ui->abortButton->setText(tr("%1").arg(Label));
}

/****************************************************************************/
/*!
 *  \brief Show the abort button of the dialog
 */
/****************************************************************************/
void CDlgWaitingBase::ShowAbort(void)
{
    ui->abortButton->show();
}

/****************************************************************************/
/*!
 *  \brief Hides the abort button of the dialog
 */
/****************************************************************************/
void CDlgWaitingBase::HideAbort(void)
{
    ui->abortButton->hide();
}

/****************************************************************************/
/*!
 *  \brief Enable the abort button of the dialog
 */
/****************************************************************************/
void CDlgWaitingBase::EnableAbort(void)
{
    ui->abortButton->setEnabled(true);
}

/****************************************************************************/
/*!
 *  \brief Disable the abort button of the dialog
 */
/****************************************************************************/
void CDlgWaitingBase::DisableAbort(void)
{
    ui->abortButton->setEnabled(false);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CDlgWaitingBase::changeEvent(QEvent *p_Event)
{
    QDialog::changeEvent(p_Event);

    switch (p_Event->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

} // end namespace MainMenu
