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
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam p_DataConnector = service Gui connector, parent = parent of widget
     */
    /****************************************************************************/
    explicit CSettings(Core::CServiceGUIConnector *p_ServiceDataConnector, MainMenu::CMainWindow *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
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
     /****************************************************************************/
     /*!
      *  \brief Slot for save Ip address.
      *  \param IPAddress = ip address, IPType = ip type of server or proxy
      */
     /****************************************************************************/
     void SaveIPAddress(QString IPAddress);

     /****************************************************************************/
     /*!
      *  \brief Slot for reset Ui button status
      */
     /****************************************************************************/
     void ResetButtonStatus();

     /****************************************************************************/
     /*!
      *  \brief Slot for update GUi by user model
      *  \DataConnector = service Gui connector, MainWindow = this Ui Main window
      */
     /****************************************************************************/
     void UpdateGUIConnector(Core::CServiceGUIConnector *DataConnector, MainMenu::CMainWindow *MainWindow);

     /****************************************************************************/
     /*!
      *  \brief Slot for set information text to pop' up.
      *  \param Text = information text, Color = color of this information
      */
     /****************************************************************************/
     void SetInformwationText(QString Text, QString Color);

     /****************************************************************************/
     /*!
      *  \brief To Set net work setting result.
      *  \param NtService = Enum for Network settings
      *  \param Result = setting result
      */
     /****************************************************************************/
     void SetNetworkSettingsResult(PlatformService::NetworkSettings_t NtService, bool Result);
};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_HELPFILEIMPORT_H
