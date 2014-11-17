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

    //CONNECTSIGNALSLOTGUI(mp_Ui->parametersPanel, CurrentRowChanged(int), mp_Ui->stackedWidget,
                         //setCurrentIndex(int));

    CONNECTSIGNALSLOTGUI(mp_Ui->parametersPanel, CurrentRowChanged(int), this, OnCurrentTabChanged(int));

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
        ServiceParameters->SetProxyIPAddress(IPAddress);
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

    QString IPAddress("XXX.XXX.XXX.XXX");
    if (mp_ServiceDataConnector->GetServiceParameters()) {
        IPAddress = mp_ServiceDataConnector->GetServiceParameters()->GetProxyIPAddress();
    }

    mp_Ui->pageNetworkSettings->SetPtrToMainWindow(mp_MainWindow);
    mp_Ui->pageDateTimeSettings->SetPtrToMainWindow(mp_MainWindow);
    mp_Ui->pageLanguageSettings->SetPtrToMainWindow(mp_MainWindow);

    mp_Ui->pageNetworkSettings->UpdateIpAddress(IPAddress);
}

/****************************************************************************/
/*!
 *  \brief Slot called when current tab is changed
 *  \iparam CurrentTabIndex = Current tab changed
 */
/****************************************************************************/
void CSettings::OnCurrentTabChanged(int CurrentTabIndex)
{
    mp_Ui->stackedWidget->setCurrentIndex(CurrentTabIndex);

    if (mp_MainWindow->GetSaMUserMode() == QString("Manufacturing") && mp_Ui->stackedWidget->currentWidget() == mp_Ui->pageNetworkSettings) {
        if (mp_ServiceDataConnector->GetServiceParameters()) {
            mp_Ui->pageNetworkSettings->UpdateIpAddress(mp_ServiceDataConnector->GetServiceParameters()->GetProxyIPAddress());
        }
        qDebug()<<"check network...";
        mp_Ui->pageNetworkSettings->reset();
        emit PerformNetworkTests();
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
void CSettings::SetInformationText(QString Text, QString Color)
{
    mp_Ui->pageNetworkSettings->SetInformationText(Text, Color);
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
    if (Result) {
        emit RefreshLatestVersion();
    }
}

} // end namespace ServiceUpdates
