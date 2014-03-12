/****************************************************************************/
/*! \file DlgWizardText.cpp
 *
 *  \brief Implementation of text wizard dialog.
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

#include "ServiceWidget/Include/DlgWizardText.h"

namespace MainMenu {

CDlgWizardText::CDlgWizardText(QWidget *parent)
    : CDlgWizardBase(parent)
{
    mp_Text = new QLabel(this);

    this->AddWidget(mp_Text);
}

CDlgWizardText::~CDlgWizardText(void)
{
    try
    {
        delete mp_Text;
    }
    catch (...)
    { }
}

void CDlgWizardText::SetText(const QString &Text)
{
    mp_Text->setText(Text);
}

} // namespace MainMenu
