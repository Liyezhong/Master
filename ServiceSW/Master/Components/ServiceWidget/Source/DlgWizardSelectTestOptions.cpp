/****************************************************************************/
/*! \file DlgWizardSelectTestOptions.cpp
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-12
 *   $Author:  $ Abe Yang
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

#include "ServiceWidget/Include/DlgWizardSelectTestOptions.h"
#include "ServiceWidget/Include/DlgSelectTestOptions.h"
#include "Global/Include/Utils.h"

namespace MainMenu {

CDlgWizardSelectTestOptions::CDlgWizardSelectTestOptions(QWidget *parent, QMainWindow *pMainWindow)
    : CDlgWizardBase(parent, pMainWindow)
{
    m_pDlgSelectTestOptions = new CDlgSelectTestOptions(parent);
    this->AddWidget(m_pDlgSelectTestOptions);
    this->move(35, 100);
    this->DisableNext();
    this->HideCancel();
    CONNECTSIGNALSLOT(m_pDlgSelectTestOptions, RadioBtnSelected(int), this, OnRadioBtnSelected(int));
    CONNECTSIGNALSLOT(this, accepted(), this, OnClickedNextButton());
}

CDlgWizardSelectTestOptions::~CDlgWizardSelectTestOptions(void)
{
    try
    {
        delete m_pDlgSelectTestOptions;
    }
    catch (...)
    {
    }
}

void CDlgWizardSelectTestOptions::OnRadioBtnSelected(int radioBtnIndex)
{
    this->EnableNext();
    m_radioBtnIndex = radioBtnIndex;
}

void CDlgWizardSelectTestOptions::OnClickedNextButton()
{
    emit ClickedNextButton(m_radioBtnIndex);
}

} // namespace MainMenu
