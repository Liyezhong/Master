/****************************************************************************/
/*! \file NetworkWidget.h
 *
 *  \brief NetworkWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-07-12
 *   $Author:  $ Dixiong li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERVICEUPDATES_NETWORKWIDGET_H
#define SERVICEUPDATES_NETWORKWIDGET_H

#include "../Include/PlatformServiceDefines.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include <KeyBoard/Include/KeyBoard.h>
#include <QRegExpValidator>
#include <MainMenu/Include/BaseTable.h>
#include <QStandardItemModel>

namespace ServiceUpdates {

namespace Ui {
    class CNetworkWidget;
}

//!< Button Type -UserName, Password, IP Address or Port
typedef enum {
    USERNAME_BTN_CLICKED = 1,
    PASSWORD_BTN_CLICKED,
    IP_ADDRESS_BTN_CLICKED,
    PORT_BTN_CLICKED,
    INVALID
}ButtonType_t;

//!< Ip Type - proxy, server
typedef enum {
    UNDEFINE,
    PROXY_IP,
    SERVER_IP
}IPType_t;

/****************************************************************************/
/**
 * \brief This class contains the proxy settings for remote care
 */
/****************************************************************************/
class CNetworkWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam parent = parent of widget
     */
    /****************************************************************************/
    explicit CNetworkWidget(QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CNetworkWidget();

    /****************************************************************************/
    /**
     * \brief Set this Ui to main window.
     *  \param p_MainWindow = MainWindow object
     */
    /****************************************************************************/
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);

    /****************************************************************************/
    /**
     * \brief To set Save Button status.
     */
    /****************************************************************************/
    void SetSaveButtonStatus();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam TestName = Test name
     */
    /****************************************************************************/
    void AddItem(QString TestName);

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Event = QEvent type parameter
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    Ui::CNetworkWidget *mp_Ui;                      //!< User interface
    MainMenu::CMainWindow *mp_MainWindow;                   //!< Pointer to MainWindow
    bool m_ProcessRunning;                                  //!< Process running state
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //!< Current user role
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;                 //!< Reference to Keyboard widget
    ButtonType_t m_ButtonType;                              //!< Button Type
    QString m_Password;                                     //!< String to hold Password
    MainMenu::CMessageDlg *mp_MessageDlg;                   //!< Message dialog object.
    QString m_ProxyIp;                                    //!< To store the proxy Ip address
    QString m_ServerIp;                                    //!< To Store the server Ip address
    MainMenu::CBaseTable *mp_TableWidget;                   //!< Base Table widget
    QStandardItemModel m_Model;                             //!< Model for the table
    IPType_t           m_IpType;                            //!< Ip Type

    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void ConnectKeyBoardSignalSlots();

    /****************************************************************************/
    /*!
     *  \brief Disconnects signals and slots of keyboard.
     */
    /****************************************************************************/
    void DisconnectKeyBoardSignalSlots();

    /****************************************************************************/
    /*!
     *  \brief To enter ip adress.
     *  \param IpType = ip type of server and proxy
     */
    /****************************************************************************/
    void EnterIPAddress(IPType_t IpType);

signals:
    /****************************************************************************/
    /*!
     *  \brief  This signal is emitted when the ServiceParameter is changed.
     *
     *  \iparam ServiceParameters = ServiceParameters reference.
     */
    /****************************************************************************/
    void SaveIPAddress(QString, IPType_t);

    /****************************************************************************/
    /*!
     *  \brief  This signal is emitted when validation of key board is done.
     *
     *  \iparam Value = Flag value.
     */
    /****************************************************************************/
    void KeyBoardStringValidationComplete(const bool Value);

    /****************************************************************************/
    /*!
     *  \brief  This signal is emitted for firmware download
     */
    /****************************************************************************/
    void DownloadFirmware();

private slots:
    /****************************************************************************/
    /*!
     *  \brief Slot for enter Proxy ip address.
     */
    /****************************************************************************/
    void OnProxyIPAddress();

    /****************************************************************************/
    /*!
     *  \brief Slot for enter Server Ip address.
     */
    /****************************************************************************/
    void OnServerIPAddress();

    /****************************************************************************/
    /*!
     *  \brief Slot for save Proxy Ip address.
     */
    /****************************************************************************/
    void OnProxyIpSave();

    /****************************************************************************/
    /*!
     *  \brief Slot for save Server Ip address.
     */
    /****************************************************************************/
    void OnServerIpSave();

    /****************************************************************************/
    /*!
     *  \brief Slot for clicked ok button.
     *  \param EnterendText = entered ip address
     */
    /****************************************************************************/
    void OnOkClicked(QString EnteredText);

    /****************************************************************************/
    /*!
     *  \brief Slot for clicked esc button.
     */
    /****************************************************************************/
    void OnESCClicked();

    /****************************************************************************/
    /*!
     *  \brief To check entered string.
     *  \param Valve = entered string
     */
    /****************************************************************************/
    void KeyBoardValidateEnteredString(QString& Value);

    /****************************************************************************/
    /*!
     *  \brief To retranslate ui.
     */
    /****************************************************************************/
    void RetranslateUI();

public slots:
    /****************************************************************************/
    /*!
     *  \brief Slot for download firmware.
     */
    /****************************************************************************/
    void OnDownloadFirmware();

    /****************************************************************************/
    /*!
     *  \brief Slot for Update Ip address.
     *  \param IpAddress = ip address string
     */
    /****************************************************************************/
    void UpdateIpAddress(QString IpAddress);

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

    /****************************************************************************/
    /*!
     *  \brief Slot for reset this ui.
     */
    /****************************************************************************/
    void reset();

};

} // end namespace ServiceUpdates

#endif // SERVICEUPDATES_NETWORKWIDGET_H
