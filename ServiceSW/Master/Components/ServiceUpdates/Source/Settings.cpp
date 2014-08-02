/****************************************************************************/
/*! \file Settings.cpp
 *
 *  \brief CSettings implementation file
 *
 *  \b Description:
 *          This class provides options for Language, Date/Time and network
 *          settings.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-08
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

#include "ServiceUpdates/Include/Settings.h"
#include "Settings/Include/NetworkSettingsWidget.h"
#include "ui_Settings.h"

namespace ServiceUpdates {

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_ServiceDataConnector = Service GUI connector object
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSettings::CSettings(Core::CServiceGUIConnector *p_ServiceDataConnector, MainMenu::CMainWindow *p_Parent) :
    mp_Ui(new Ui::CSettings),
    mp_MainWindow(p_Parent),
    mp_ServiceDataConnector(p_ServiceDataConnector)
{
    mp_Ui->setupUi(this);

    mp_Ui->pageLanguageSettings->setContentsMargins(0, 0, 0, 0);

    CONNECTSIGNALSLOTGUI(mp_Ui->parametersPanel, CurrentRowChanged(int), mp_Ui->stackedWidget,
                         setCurrentIndex(int));

    CONNECTSIGNALSLOTGUI(mp_Ui->pageDateTimeSettings->GetContent(), ApplyData(QDateTime), mp_ServiceDataConnector,
                         SetDateTime(QDateTime));

    CONNECTSIGNALSLOTGUI(mp_Ui->pageNetworkSettings, SaveIPAddress(QString), this, SaveIPAddress(QString));

    CONNECTSIGNALSLOTGUI(this, ServiceParametersChanged(DataManager::CServiceParameters *),
                         mp_ServiceDataConnector, ServiceParametersUpdates(DataManager::CServiceParameters *));

    CONNECTSIGNALSIGNALGUI(mp_Ui->pageLanguageSettings, SetLanguage(PlatformService::Languages_t), this, SetLanguage(PlatformService::Languages_t));

    CONNECTSIGNALSIGNALGUI(mp_Ui->pageNetworkSettings, DownloadFirmware(), this, DownloadFirmware());

    mp_Ui->stackedWidget->setCurrentIndex(0);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSettings::~CSettings()
{
    try {        
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

/*******************************************************************************/
/*!
 *  \brief Slot called to update Service Parameters object
 *  \iparam IPAddress = Proxy IP address to set
 */
/*******************************************************************************/
void CSettings::SaveIPAddress(QString IPAddress) {
    DataManager::CServiceParameters *ServiceParameters = mp_ServiceDataConnector->GetServiceParameters();
    if (ServiceParameters != NULL) {
        ServiceParameters->SetServerIPAddress(IPAddress);
        emit ServiceParametersChanged(ServiceParameters);
    }

    mp_Ui->pageNetworkSettings->reset();
    emit PerformNetworkTests();
}

/*******************************************************************************/
/*!
 *  \brief Slot called to update data connector object
 *  \iparam DataConnector = Data Connector object
 *  \iparam MainWindow = Main window object
 */
/*******************************************************************************/
void CSettings::UpdateGUIConnector(Core::CServiceGUIConnector *DataConnector, MainMenu::CMainWindow *MainWindow)
{
    mp_ServiceDataConnector = DataConnector;
    mp_MainWindow = MainWindow;
    /*
    if (mp_MainWindow->GetSaMUserMode() == QString("Service")) {
        mp_Ui->stackedWidget->removeWidget(mp_Ui->pageMNetworkSettings);
        mp_Ui->pageSNetworkSettings->SetPtrToMainWindow(mp_MainWindow);
    }
    else {
        mp_Ui->stackedWidget->removeWidget(mp_Ui->pageSNetworkSettings);
        mp_Ui->pageMNetworkSettings->SetPtrToMainWindow(mp_MainWindow);
    }
    */
    mp_Ui->pageNetworkSettings->SetPtrToMainWindow(mp_MainWindow);
    mp_Ui->pageDateTimeSettings->SetPtrToMainWindow(mp_MainWindow);
    mp_Ui->pageLanguageSettings->SetPtrToMainWindow(mp_MainWindow);
}

/****************************************************************************/
/*!
 *  \brief Slot called when tab index changed
 *  \iparam Index = Index of the current tab
 */
/****************************************************************************/
void CSettings::TabIndexChanged(int Index)
{
    mp_Ui->stackedWidget->setVisible(true);
    mp_Ui->parametersPanel->setVisible(false);
    mp_Ui->stackedWidget->setCurrentIndex(Index);
}

/****************************************************************************/
/*!
 *  \brief Slot called when current tab is changed
 *  \iparam CurrentTabIndex = Current tab changed
 */
/****************************************************************************/
void CSettings::OnCurrentTabChanged(int CurrentTabIndex)
{
    if (CurrentTabIndex == 5 ) {
        emit TabChanged();
    }
}

/****************************************************************************/
/*!
 *  \brief Slot called to Enable/Disable the button
 */
/****************************************************************************/
void CSettings::ResetButtonStatus()
{
    mp_Ui->pageDateTimeSettings->SetPtrToMainWindow(mp_MainWindow);
    mp_Ui->pageDateTimeSettings->SetButtonStatus();
    mp_Ui->pageNetworkSettings->SetSaveButtonStatus();
}

/****************************************************************************/
/*!
 *  \brief Called for firmware download
 *  \iparam Text = Information text
 *  \iparam Color = color
 */
/****************************************************************************/
void CSettings::SetInformwationText(QString Text, QString Color)
{
    mp_Ui->pageNetworkSettings->SetInformwationText(Text, Color);
}

/****************************************************************************/
/*!
 *  \brief Called for firmware download
 *  \iparam NtService = Network service
 *  \iparam Result = Result of the test
 */
/****************************************************************************/
void CSettings::SetNetworkSettingsResult(PlatformService::NetworkSettings_t NtService, bool Result)
{
    mp_Ui->pageNetworkSettings->SetNetworkSettingsResult(NtService, Result);
}

} // end namespace ServiceUpdates
