/****************************************************************************/
/*! \file DlgConfirmationText.cpp
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

#include "ServiceWidget/Include/DlgConfirmationText.h"

namespace MainMenu {

CDlgConfirmationText::CDlgConfirmationText(QWidget *parent)
    : CDlgConfirmationBase(parent)
{
    mp_Text = new QLabel(this);

    this->AddWidget(mp_Text);
}

CDlgConfirmationText::~CDlgConfirmationText(void)
{
    try
    {
        delete mp_Text;
    }
    catch (...)
    { }
}

void CDlgConfirmationText::SetText(const QString &Text)
{
    mp_Text->setText(Text);
}

} // namespace MainMenu
