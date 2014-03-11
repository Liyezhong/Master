/****************************************************************************/
/*! \file DlgConfirmationStatus.cpp
 *
 *  \brief Implementation of confirmation dialog with status.
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

#include "ServiceWidget/Include/DlgConfirmationStatus.h"

namespace MainMenu {

CDlgConfirmationStatus::CDlgConfirmationStatus(QWidget *parent)
    : CDlgConfirmationBase(parent)
{
    mp_Text = new QLabel(this);
    mp_GroupStatus = new QGroupBox(this);
    mp_LayoutStatus = new QHBoxLayout(mp_GroupStatus);
    mp_Status = new QLabel(mp_GroupStatus);

    mp_LayoutStatus->addWidget(mp_Status);
    mp_GroupStatus->setLayout(mp_LayoutStatus);

    this->AddWidget(mp_Text);
    this->AddWidget(mp_GroupStatus);
}

CDlgConfirmationStatus::~CDlgConfirmationStatus(void)
{
    try
    {
        delete mp_Status;
        delete mp_LayoutStatus;
        delete mp_GroupStatus;
        delete mp_Text;
    }
    catch (...)
    { }
}

void CDlgConfirmationStatus::SetText(const QString &Text)
{
    mp_Text->setText(Text);
}

void CDlgConfirmationStatus::SetStatus(const QString &Status)
{
    mp_Status->setText(Status);
}

} // namespace MainMenu
