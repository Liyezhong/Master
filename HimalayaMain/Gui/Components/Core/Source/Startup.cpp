/****************************************************************************/
/*! \file Startup.cpp
 *
 *  \brief Startup implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-31 - 29-Nov-2012
 *   $Author:  $ M.Scherer,Ningu
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

#include "Core/Include/Startup.h"
#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsInterface.h"
#include "MainMenu/Include/StatusBarManager.h"
#include "Application/Include/LeicaStyle.h"

namespace Core {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CStartup::CStartup() : QObject()
{
    m_MainWindow.setWindowTitle("ST8200 Software");
    m_MainWindow.setFixedSize(800, 600);
    m_MainWindow.SetVerticalOrientation(true);
    m_MainWindow.SetUserIcon(MainMenu::CMainWindow::Operator);

    #ifdef Q_WS_QWS
        m_MainWindow.showFullScreen();
    #else
        m_MainWindow.show();
    #endif

    mp_DataConnector = new Core::CDataConnector(&m_MainWindow);

    mp_Dashboard = new Dashboard::CDashboardWidget(mp_DataConnector, &m_MainWindow);
    mp_KeyBoardWidget = new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);
    mp_KeyBoardWidget->setModal(true);
    mp_Programs = new Programs::CProgramWidget(mp_DataConnector, &m_MainWindow, mp_KeyBoardWidget);
    mp_Reagents = new Reagents::CReagentWidget(mp_DataConnector, &m_MainWindow, mp_KeyBoardWidget);
    mp_Settings = new Settings::CSettingsWidget(mp_DataConnector, &m_MainWindow, mp_KeyBoardWidget);
    mp_Users = new Users::CUserPrivilegeWidget(&m_MainWindow, mp_KeyBoardWidget);
    MainMenu::StatusBarManager::CreateInstance(&m_MainWindow,mp_DataConnector->SettingsInterface);

    // Dashboard Signals & Slots
    CONNECTSIGNALSLOT(mp_Reagents, RMSValueChanged(Global::RMSOptions_t), mp_Dashboard, OnRMSValueChanged(Global::RMSOptions_t));

//    CONNECTSIGNALSLOT(mp_Reagents, UpdateReagent(DataManager::CReagent &),
//                      mp_DataConnector, SendReagentUpdate(DataManager::CReagent &));
//    CONNECTSIGNALSLOT(mp_Reagents, AddReagent(DataManager::CReagent &),
//                      mp_DataConnector, SendReagentAdd(DataManager::CReagent &));
//    CONNECTSIGNALSLOT(mp_Reagents, RemoveReagent(QString), mp_DataConnector, SendReagentRemove(QString));

//    CONNECTSIGNALSIGNAL(mp_DataConnector, ProgramsUpdated(), mp_Programs, UpdateProgramList());

    CONNECTSIGNALSLOT(mp_Programs, UpdateProgram(DataManager::CProgram &),
                      mp_DataConnector, SendProgramUpdate(DataManager::CProgram &));
    CONNECTSIGNALSLOT(mp_Programs, ProgramColorReplaced(DataManager::CProgram &,DataManager::CProgram &),
                      mp_DataConnector, SendProgramColorUpdate(DataManager::CProgram &,DataManager::CProgram &));
    CONNECTSIGNALSLOT(mp_Programs, AddProgram(DataManager::CProgram &),
                      mp_DataConnector, SendProgramAdd(DataManager::CProgram &));
    CONNECTSIGNALSLOT(mp_Programs, DeleteProgram(QString &),mp_DataConnector,SendProgramRemove(QString &));


    // Signals for the Users
    CONNECTSIGNALSLOT(mp_Users, UserLevelClicked(QDataStream &), mp_DataConnector,
                      SendUserLevel(QDataStream &));
    CONNECTSIGNALSLOT(mp_Users, ChangePasswordClicked(QDataStream &), mp_DataConnector,
                      SendChangepassword(QDataStream &));


    // signals for the users
    CONNECTSIGNALSLOT(mp_DataConnector, UserAuthentication(const qint32 &), mp_Users, UserAuthenticated(const qint32 &));
    CONNECTSIGNALSLOT(mp_DataConnector, ChangeAdminPassword(const QString &), mp_Users, ChangeInAdminPassword(const QString &));

    m_MainWindow.AddMenuGroup(mp_Dashboard, QPixmap(QString(":/%1/Icons/Maintabs/Maintabs_Programs.png").arg(Application::CLeicaStyle::GetProjectNameString())));
    m_MainWindow.AddMenuGroup(mp_Programs, QPixmap(QString(":/%1/Icons/Maintabs/Maintabs_Programs.png").arg(Application::CLeicaStyle::GetProjectNameString())));
    m_MainWindow.AddMenuGroup(mp_Reagents, QPixmap(QString(":/%1/Icons/Maintabs/Maintabs_Reagents.png").arg(Application::CLeicaStyle::GetProjectNameString())));
    m_MainWindow.AddMenuGroup(mp_Settings, QPixmap(QString(":/%1/Icons/Maintabs/Maintabs_Settings.png").arg(Application::CLeicaStyle::GetProjectNameString())));
    m_MainWindow.AddMenuGroup(mp_Users, QPixmap(QString(":/%1/Icons/Maintabs/Maintabs_User.png").arg(Application::CLeicaStyle::GetProjectNameString())));

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CStartup::~CStartup()
{
    try {
        delete mp_Users;
        delete mp_Settings;
        delete mp_Reagents;
        delete mp_Programs;
        delete mp_Dashboard;
        delete mp_KeyBoardWidget;
        delete mp_DataConnector;
    }
    catch (...) {}
}

} // end namespace Core
