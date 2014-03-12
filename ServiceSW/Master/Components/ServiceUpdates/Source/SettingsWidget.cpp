/****************************************************************************/
/*! \file SettingsWidget.cpp
 *
 *  \brief Implementation of Settings widget class.
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

#include "ServiceUpdates/Include/SettingsWidget.h"

#include <QDir>

#include "Global/Include/Utils.h"
#include "Global/Include/SystemPaths.h"

#include "ui_SettingsWidget.h"


namespace ServiceUpdates {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CSettingsWidget::CSettingsWidget(Core::CServiceGUIConnector &DataConnector,
                                 QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CSettingsWidget)
    , mp_DateConnector(&DataConnector)
{
    mp_Ui->setupUi(this);

    UpdateSupportedGUILanguages();

    CONNECTSIGNALSLOT(mp_Ui->parametersPanel,
                      CurrentRowChanged(int),
                      mp_Ui->stackedWidget,
                      setCurrentIndex(int));

    mp_Ui->pageLanguage->SetLanguages(m_LanguageList);

    mp_Ui->parametersPanel->SetCurrentRow(0);

    CONNECTSIGNALSIGNAL(mp_Ui->pageLanguage,
                        UserSettingsChanged(DataManager::CUserSettings &),
                        this,
                        UserSettingsChanged(DataManager::CUserSettings &));
    CONNECTSIGNALSIGNAL(mp_Ui->pageDateTime,
                        DateTimeChanged(QDateTime),
                        this,
                        DateTimeChanged(QDateTime));
    CONNECTSIGNALSIGNAL(mp_Ui->pageNetwork,
                        UserSettingsChanged(DataManager::CUserSettings &),
                        this,
                        UserSettingsChanged(DataManager::CUserSettings &));
}

CSettingsWidget::~CSettingsWidget(void)
{
}

void CSettingsWidget::UpdateGUI(void)
{
    DataManager::CUserSettings *pUserSettings =
            mp_DateConnector->GetUserSettingInterface()->GetUserSettings();

    mp_Ui->pageLanguage->SetUserSettings(pUserSettings);

    mp_Ui->pageDateTime->SetUserSettings(pUserSettings);

    mp_Ui->pageNetwork->SetUserSettings(pUserSettings);
}

void CSettingsWidget::TabIndexChanged(int Index)
{
    mp_Ui->stackedWidget->setVisible(true);
    mp_Ui->parametersPanel->setVisible(false);
    mp_Ui->stackedWidget->setCurrentIndex(Index);
}

void CSettingsWidget::UpdateSupportedGUILanguages(void)
{
    QDir TheDir(Global::SystemPaths::Instance().GetTranslationsPath());

    QStringList FileNames = TheDir.entryList(QStringList("Himalaya_*.qm"));

    QStringList LanguageList;
    for (int i = 0; i < FileNames.size(); ++i)
    {
        // get locale extracted by filename
        QString Locale;
        Locale = FileNames[i];                      // "Himalaya_en.qm"
        Locale.truncate(Locale.lastIndexOf('.'));   // "Himalaya_en"
        Locale.remove(0, Locale.indexOf('_') + 1);  // "en"

        LanguageList << QLocale::languageToString(QLocale(Locale).language());
    }

    m_LanguageList = LanguageList;
}


} // end namespace ServiceUpdates
