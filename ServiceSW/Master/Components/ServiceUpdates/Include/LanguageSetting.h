/****************************************************************************/
/*! \file LanguageSetting.h
 *
 *  \brief LanguageSetting definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-12
 *   $Author:  $ Soumya. D
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

#ifndef ServiceUpdates_LanguageSetting_H
#define ServiceUpdates_LanguageSetting_H

#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/BaseTable.h"

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"

#include "ServiceUpdates/Include/LanguageModel.h"

namespace ServiceUpdates {

namespace Ui {
    class CLanguageSetting;
}

/****************************************************************************/
/**
 * \brief This class contains the language selection widget
 */
/****************************************************************************/
class CLanguageSetting : public MainMenu::CPanelFrame
{
    Q_OBJECT
public:
    explicit CLanguageSetting(QWidget *p_Parent = 0);

    ~CLanguageSetting(void);

    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);

    void SetLanguages(QStringList LanguageList);

Q_SIGNALS:
     void UserSettingsChanged(DataManager::CUserSettings &Settings);

private:
    void InitDialog(void);

    void PopulateLanguageList(void);

    void RetranslateUI(void);

protected:
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void OnBtnApplyClicked(void);

    void SelectionChanged(QModelIndex Index);

private:
    Ui::CLanguageSetting           *mp_Ui;

    MainMenu::CBaseTable           *mp_TableWidget;
    DataManager::CUserSettings     *mp_UserSettings;
    ServiceUpdates::CLanguageModel *mp_LanguageModel;

    QStringList                     m_LanguageList;
    QString                         m_SelectedLanguage;

    QStringList                     m_LanguageNativeNameList;

};

} // end namespace ServiceUpdates

#endif // ServiceUpdates_LanguageSetting_H
