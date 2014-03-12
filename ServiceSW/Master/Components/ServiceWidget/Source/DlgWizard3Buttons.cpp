/****************************************************************************/
/*! \file DlgWizard3Buttons.cpp
 *
 *  \brief Implementation of wizard dialog with 3 buttons.
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

#include "ServiceWidget/Include/DlgWizard3Buttons.h"

namespace MainMenu {

CDlgWizard3Buttons::CDlgWizard3Buttons(QWidget *parent)
    : CDlgWizardText(parent)
{
    mp_PushThird = new QPushButton(this);

    this->AddButton(mp_PushThird);

    (void)connect(mp_PushThird,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ThirdSelect()) );
}

CDlgWizard3Buttons::~CDlgWizard3Buttons(void)
{
    try
    {
        delete mp_PushThird;
    }
    catch (...)
    { }
}

void CDlgWizard3Buttons::SetButtonText(const QString &Text)
{
    mp_PushThird->setText(Text);
}

void CDlgWizard3Buttons::ThirdSelect(void)
{
    emit ThirdSelected();
}

} // namespace MainMenu
