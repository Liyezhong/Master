/****************************************************************************/
/*! \file DlgWaitingStatus.cpp
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

#include "ServiceWidget/Include/DlgWaitingStatus.h"

namespace MainMenu {

CDlgWaitingStatus::CDlgWaitingStatus(QWidget *parent)
    : CDlgWaitingBase(parent)
{
    mp_GroupStatus = new QGroupBox(this);
    mp_LayoutStatus = new QHBoxLayout(mp_GroupStatus);
    mp_Status = new QLabel(mp_GroupStatus);

    mp_LayoutStatus->addWidget(mp_Status);
    mp_GroupStatus->setLayout(mp_LayoutStatus);

    this->AddWidget(mp_GroupStatus);
}

CDlgWaitingStatus::~CDlgWaitingStatus(void)
{
    try
    {
        delete mp_Status;
        delete mp_LayoutStatus;
        delete mp_GroupStatus;
    }
    catch (...)
    { }
}

void CDlgWaitingStatus::SetStatus(const QString &Status)
{
    mp_Status->setText(Status);
}

} // namespace MainMenu
