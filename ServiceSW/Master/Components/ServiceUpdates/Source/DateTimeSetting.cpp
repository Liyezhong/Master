/****************************************************************************/
/*! \file DateTimeSetting.cpp
 *
 *  \brief Implementation of Date/Time setting class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-18
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

#include "ServiceUpdates/Include/DateTimeSetting.h"

#include "Global/Include/Utils.h"

#include "ui_DateTimeSetting.h"

namespace ServiceUpdates {

CDateTimeSetting::CDateTimeSetting(QWidget *parent)
    : MainMenu::CPanelFrame(parent)
    , ui(new Ui::CDateTimeSetting)
    , mp_UserSettings(0)
{
    ui->setupUi(GetContentFrame());

    SetPanelTitle(tr("Date/Time"));

    CONNECTSIGNALSIGNAL(ui->widgetDateTime,
                        ApplyData(QDateTime),
                        this,
                        DateTimeChanged(QDateTime));
}

CDateTimeSetting::~CDateTimeSetting(void)
{
    try
    {
        delete ui;
    }
    catch (...) { };
}

/****************************************************************************/
/*!
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void CDateTimeSetting::SetUserSettings(
        DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;

    if (0 != mp_UserSettings)
    {
        ui->widgetDateTime->RefreshDateTime(mp_UserSettings->GetTimeFormat());
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CDateTimeSetting::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            RetranslateUI();
            break;
        default:
            break;
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CDateTimeSetting::RetranslateUI()
{
   SetPanelTitle(tr("Date/Time"));
}

} // namespace ServiceUpdates
