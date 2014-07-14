/****************************************************************************/
/*! \file Settings.h
 *
 *  \brief CSettings class definition file
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

#ifndef SERVICEUPDATES_SETTINGS_H
#define SERVICEUPDATES_SETTINGS_H

#include <QWidget>
#include "Core/Include/ServiceGUIConnector.h"
#include "MainMenu/Include/MainWindow.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "MainMenu/Include/MessageDlg.h"
#include "../Include/PlatformServiceDefines.h"
#include "ServiceUpdates/Include/NetworkWidget.h"
namespace ServiceUpdates {

namespace Ui {
    class CSettings;
}
/****************************************************************************/
/**
 * \brief This widget implements User settings
 */
/****************************************************************************/
class CSettings : public QWidget
{
    Q_OBJECT

public:
    explicit CSettings(Core::CServiceGUIConnector *p_ServiceDataConnector, MainMenu::CMainWindow *p_Parent = 0);    
    ~CSettings();

private:
    Ui::CSettings *mp_Ui;                                   //!< User Interface
    MainMenu::CMainWindow *mp_MainWindow;                   //!< Main window object
    Core::CServiceGUIConnector *mp_ServiceDataConnector;    //!< Service GUI connector object
    bool m_CurrentTab;                                      //!< Stores current tab index    

signals:
    /****************************************************************************/
    /**
     * \brief Signal is emitted when current tab is changed
     */
    /****************************************************************************/
    void TabChanged();

    /****************************************************************************/
    /*!
     *  \brief  This signal is emitted for firmware download
     */
    /****************************************************************************/
    void DownloadFirmware();

    /****************************************************************************/
    /*!
     *  \brief Signal emitted for language setting
     */
    /****************************************************************************/
    void SetLanguage(PlatformService::Languages_t);

    /****************************************************************************/
    /*!
     *  \brief  This signal is emitted when the ServiceParameter is changed.
     *
     *  \iparam ServiceParameters = ServiceParameters reference.
     */
    /****************************************************************************/
    void ServiceParametersChanged(DataManager::CServiceParameters *ServiceParameters);

private slots:
     void TabIndexChanged(int);
     void OnCurrentTabChanged(int CurrentTabIndex);

public slots:     
     void SaveIPAddress(QString, IPType_t);
     void ResetButtonStatus();
     void UpdateGUIConnector(Core::CServiceGUIConnector *DataConnector, MainMenu::CMainWindow *MainWindow);
     void SetInformwationText(QString Text, QString Color);
     void SetNetworkSettingsResult(PlatformService::NetworkSettings_t, bool);
};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_HELPFILEIMPORT_H
