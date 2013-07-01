/****************************************************************************/
/*! \file SettingsWidget.cpp
 *
 *  \brief SettingsWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer
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

#include "Settings/Include/SettingsWidget.h"
#include "Global/Include/Commands/CmdDateAndTime.h"
#include "ui_SettingsWidget.h"

namespace Settings {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Data = Global data connector
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CSettingsWidget::CSettingsWidget(Core::CDataConnector *p_Data, MainMenu::CMainWindow *p_Parent,
                                 KeyBoard::CKeyBoard *p_Keyboard) :QWidget(p_Parent),
                                 mp_Ui(new Ui::CSettingsWidget),mp_Data(p_Data),mp_MainWindow(p_Parent),
                                 mp_KeyBoardWidget(p_Keyboard)
{
    mp_Ui->setupUi(this);
    DataManager::CHimalayaUserSettings *p_Settings = mp_Data->SettingsInterface->GetUserSettings();
    if (p_Settings) {
        mp_Ui->pageSystemSetup->SetUserSettings(p_Settings);
        mp_Ui->pageAlarm->SetUserSettings(p_Settings);
        mp_Ui->pageDateTime->SetUserSettings(p_Settings);
        mp_Ui->pageInstallation->SetUserSettings(p_Settings);
        mp_Ui->pageInstallation->SetKeyBoardInstance(mp_KeyBoardWidget);
        mp_Ui->pageRegionalSettings->SetUserSettings(p_Settings);
        mp_Ui->pageService->SetUserSettings(p_Settings);
        mp_Ui->pageRegionalSettings->SetPtrToMainWindow(mp_MainWindow);
        mp_Ui->pageLanguage->SetPtrToMainWindow(mp_MainWindow,p_Data);
        mp_Ui->pageSystemSetup->SetPtrToMainWindow(mp_MainWindow);
        mp_Ui->pageAlarm->SetPtrToMainWindow(mp_MainWindow);
        mp_Ui->pageAlarm->SetDataConnector(mp_Data);        
        mp_Ui->pageNetwork->SetPtrToMainWindow(mp_MainWindow);
        mp_Ui->pageNetwork->SetKeyBoardInstance(mp_KeyBoardWidget);
        mp_Ui->pageInstallation->SetPtrToMainWindow(mp_MainWindow);
        mp_Ui->pageRegionalSettings->SetPtrToMainWindow(mp_MainWindow);
        mp_Ui->pageService->SetPtrToMainWindow(mp_MainWindow);
        mp_Ui->pageDateTime->SetPtrToMainWindow(mp_MainWindow);
        mp_Ui->pageDataManagement->SetPtrToMainWindow(mp_MainWindow);
    }    
    CONNECTSIGNALSLOT(mp_Ui->parametersPanel, CurrentRowChanged(int), mp_Ui->settingsStack, setCurrentIndex(int));    
    CONNECTSIGNALSLOT(mp_Ui->pageDateTime->GetContent(), ApplyData(QDateTime), mp_Data, SendDateTime(QDateTime));
    CONNECTSIGNALSLOT(mp_Data, DateTimeAcked(), this, PropagateDateTime());
    CONNECTSIGNALSLOT(mp_Ui->pageRegionalSettings, RegionalSettingsChanged(DataManager::CUserSettings &),
                      mp_Data, SendUpdatedSettings(DataManager::CUserSettings &));

    CONNECTSIGNALSLOT(mp_Ui->pageService, ServiceSettingsChanged(DataManager::CUserSettings &),
                      mp_Data, SendUpdatedSettings(DataManager::CUserSettings &));

    CONNECTSIGNALSLOT(mp_Ui->pageService, SystemShutdown(),
                      mp_Data, SendSystemShutdown());

    CONNECTSIGNALSLOT(mp_Ui->pageService, ResetOperationDays(DataManager::ResetOperationHoursType_t),
                      mp_Data, SendResetOperationDays(DataManager::ResetOperationHoursType_t));


    CONNECTSIGNALSLOT(mp_Ui->pageSystemSetup,TemperatureChanged(DataManager::CUserSettings &),
                      mp_Data, SendUpdatedSettings(DataManager::CUserSettings &));
    CONNECTSIGNALSLOT(mp_Ui->pageAlarm, AlarmSettingsChanged(DataManager::CUserSettings &),
            mp_Data, SendUpdatedSettings(DataManager::CUserSettings &));
    CONNECTSIGNALSLOT(mp_Ui->pageAlarm, PlayTestTone(quint8 , quint8 , bool ),
            mp_Data, SendCmdPlayTestToneAlarm(quint8 , quint8 , bool ));
    CONNECTSIGNALSLOT(mp_Data, UserSettingsUpdated(), this, UserSettingsUpdated());
    CONNECTSIGNALSLOT(mp_Ui->pageDataManagement, ExecSending(const QString, const QStringList &), mp_Data, SendDataImportExport(const QString , const QStringList &));

    CONNECTSIGNALSLOT(mp_Ui->settingsStack, currentChanged(int), this, PanelSelected(int));
    CONNECTSIGNALSLOT(mp_Ui->pageEventView, SelectedLogFile(const QString &), mp_Data, SendSelectedDayRunLogFile(const QString &));
    CONNECTSIGNALSIGNAL(mp_Data, UpdateDayRunLogFileContent(const QDataStream &), mp_Ui->pageEventView, DayRunLogFileContent(const QDataStream &));
    CONNECTSIGNALSIGNAL(mp_Data, UpdateDayRunLogFileNames(const QStringList &), mp_Ui->pageEventView, DayRunLogFileNames(const QStringList &));
    // Updated the width and size of the panel size of the event viewr screen
    mp_Ui->pageEventView->SetPanelSize(p_Parent->width(), p_Parent->height());
    CONNECTSIGNALSLOT(mp_Data, DeviceConfigurationUpdated(), this, UpdateLanguages());
    CONNECTSIGNALSLOT(mp_Ui->pageLanguage, UserSettingsChanged(DataManager::CUserSettings &), mp_Data, SendUpdatedSettings(DataManager::CUserSettings &));
    CONNECTSIGNALSLOT(mp_Ui->pageNetwork, SettingsChanged(DataManager::CUserSettings &), mp_Data, SendUpdatedSettings(DataManager::CUserSettings &));
    CONNECTSIGNALSLOT(mp_Data, RevertChangedUserSettings(), this, UserSettingsUpdated());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSettingsWidget::~CSettingsWidget()
{
    delete mp_Ui;
}

/****************************************************************************/
/*!
 *  \brief Sets the global time of the UI
 */
/****************************************************************************/
void CSettingsWidget::PropagateDateTime()
{
    Global::AdjustedTime::Instance().AdjustToDateTime(mp_Ui->pageDateTime->GetContent()->GetDateTime().addSecs(1));
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CSettingsWidget::changeEvent(QEvent *p_Event)
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
void CSettingsWidget::RetranslateUI()
{
    QWidget::setWindowTitle(QApplication::translate("Settings::CSettingsWidget", "Parameter", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Updates the changed user settings
 *  \todo any other widget needs to be informed about settings changed ?
 */
/****************************************************************************/
void CSettingsWidget::UserSettingsUpdated()
{
    DataManager::CHimalayaUserSettings *p_Settings = mp_Data->SettingsInterface->GetUserSettings();
    if (p_Settings) {
        mp_MainWindow->SetDateTimeFormat(p_Settings->GetDateFormat(),
                                     p_Settings->GetTimeFormat());
        mp_Ui->pageNetwork->SetUserSettings(p_Settings);
        mp_Ui->pageLanguage->SetUserSettings(p_Settings);
        mp_Ui->pageAlarm->SetUserSettings(p_Settings);
    }
}

/****************************************************************************/
/*!
 *  \brief Updates the LangauageList
 */
/****************************************************************************/
void CSettingsWidget::UpdateLanguages()
{
    mp_Ui->pageLanguage->SetLanguages(mp_Data->DeviceConfigurationInterface->GetLanguageList());
}

/****************************************************************************/
/*!
 *  \brief Slot for the selected panel
 *  \param[in] Index = Index of the panel
 */
/****************************************************************************/
void CSettingsWidget::PanelSelected(int Index)
{
    // check for the whether panel is event view or not
    int PanelIndex = mp_Ui->settingsStack->indexOf(mp_Ui->pageEventView);
    // if the panel index matches then update it
    if (PanelIndex == Index) {
        mp_Ui->pageEventView->DisableButton();
        // call the daily run log files
        mp_Data->RequestDayRunLogFileNames();
    }
}

} // end namespace Settings
