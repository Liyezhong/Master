/****************************************************************************/
/*! \file Startup.h
 *
 *  \brief Startup definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-31
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

#ifndef CORE_STARTUP_H
#define CORE_STARTUP_H

#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/StatusBarManager.h"
#include "Dashboard/Include/DashboardWidget.h"
#include "Dashboard/Include/ScreenSaverWidget.h"
#include "Programs/Include/ProgramWidget.h"
#include "Reagents/Include/ReagentWidget.h"
#include "Settings/Include/SettingsWidget.h"
#include "Users/Include/UserPrivilegeWidget.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "MainMenu/Include/DataManagement.h"

namespace Core {

/****************************************************************************/
/**
 * \brief This class handles the initialization of the user interface.
 */
/****************************************************************************/
class CStartup : public QObject
{
    Q_OBJECT

public:
    CStartup();
    virtual ~CStartup();
    void InitialiseDeviceMode(QString Mode);

private:
    // Data Manager Components
    Core::CDataConnector *mp_DataConnector; //!< Connects data manager to the network
    // GUI components
    MainMenu::CMainWindow m_MainWindow;     //!< The main window of the application
    Dashboard::CDashboardWidget *mp_Dashboard;  //!< Dashboard screen
    Programs::CProgramWidget *mp_Programs;  //!< Programs screen
    Reagents::CReagentWidget *mp_Reagents; //!< Reagents screen
    Settings::CSettingsWidget *mp_Settings; //!< Settings screen
    Users::CUserPrivilegeWidget *mp_Users;  //!< Users screen
    ScreenSaverWidget* mp_ScreenSaver;
    KeyBoard::CKeyBoard *mp_KeyBoardWidget; //!< On-screen Keyboard
    QString m_Mode;							//!< GUI mode

private slots:
    void OnLanguangeSet();
};

} // end namespace Core

#endif // CORE_STARTUP_H
