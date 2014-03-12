/****************************************************************************/
/*! \file NetworkSetting.h
 *
 *  \brief NetworkSetting definition.
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
 *  does not evidence any actual or intended publicatlion.
 *
 */
/****************************************************************************/

#ifndef SERVICEUPDATES_NETWORKSETTING_H
#define SERVICEUPDATES_NETWORKSETTING_H

#include "MainMenu/Include/PanelFrame.h"

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"

namespace ServiceUpdates {

namespace Ui {
    class CNetworkSetting;
}

/****************************************************************************/
/**
 * \brief This class contains the proxy ServiceUpdates for remote care
 */
/****************************************************************************/
class CNetworkSetting : public MainMenu::CPanelFrame
{
    Q_OBJECT

public:
    explicit CNetworkSetting(QWidget *p_Parent = 0);

    ~CNetworkSetting(void);

     void SetUserSettings(DataManager::CUserSettings *p_UserSettings);

Q_SIGNALS:
     void UserSettingsChanged(DataManager::CUserSettings &Settings);

protected:
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void OnRemoteCareStateChanged(int State);

    void OnDirectConnectionStateChanged(int State);

    void OnProxyUserName(void);

    void OnProxyPassword(void);

    void OnProxyIPAddress(void);

    void OnProxyPort(void);

    void OnSave(void);

    void ChangeProxyUserName(QString UserName);

    void ChangeProxyPassword(QString Password);

    void ChangeProxyIPAddress(QString Address);

    void ChangeProxyPort(QString Port);

private:
     void InitDialog(void);

     void RetranslateUI(void);

    Ui::CNetworkSetting        *mp_Ui;

    DataManager::CUserSettings *mp_UserSetting;

    QString                     m_Password;
};

} // end namespace ServiceUpdates

#endif // SERVICEUPDATES_NETWORKSETTING_H
