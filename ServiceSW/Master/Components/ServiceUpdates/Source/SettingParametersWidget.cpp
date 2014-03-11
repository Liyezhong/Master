/****************************************************************************/
/*! \file SettingParametersWidget.cpp
 *
 *  \brief Implementation file for class CSettingParametersWidget.
 *
 *  \b Description:
 *          This class implements widgets for displaying main selection panel for
 *          ServiceUpdates screen.
 *
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer, C.Adaragunchi
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

#include "ServiceUpdates/Include/SettingParametersWidget.h"

#include "Global/Include/Utils.h"

#include "ui_SettingParametersWidget.h"

namespace ServiceUpdates {
CSettingParametersWidget::CSettingParametersWidget(QWidget *p_Parent)
    : MainMenu::CPanelFrame(p_Parent)
    , mp_Ui(new Ui::CSettingParametersWidget)
{
    mp_Ui->setupUi(GetContentFrame());

    SetPanelTitle(tr("Parameter"));

    QPalette Palette = mp_Ui->parameterList->palette();

    Palette.setColor(QPalette::Base, Qt::transparent);
    Palette.setColor(QPalette::Highlight, Qt::transparent);
    Palette.setColor(QPalette::HighlightedText, Qt::black);

    mp_Ui->parameterList->setPalette(Palette);

    CONNECTSIGNALSIGNAL(mp_Ui->parameterList,
                        currentRowChanged(int),
                        this,
                        CurrentRowChanged(int));
}

CSettingParametersWidget::~CSettingParametersWidget(void)
{
    try
    {
        delete mp_Ui;
    }
    catch (...) { };
}

void CSettingParametersWidget::SetCurrentRow(int CurrentRow)
{
    mp_Ui->parameterList->setCurrentRow(CurrentRow);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CSettingParametersWidget::changeEvent(QEvent *p_Event)
{
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            mp_Ui->retranslateUi(this);
            RetranslateUI();
            break;
        default:
            break;
    }
    QWidget::changeEvent(p_Event);
}


/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CSettingParametersWidget::RetranslateUI(void)
{
   SetPanelTitle(tr("Parameter"));
}

} // end namespace ServiceUpdates
