/****************************************************************************/
/*! \file ServiceSettingsWidget.h
 *
 *  \brief ServiceSettingsWidget definition.
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

#ifndef SETTINGS_SERVICESETTINGSWIDGET_H
#define SETTINGS_SERVICESETTINGSWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"

namespace Settings {

namespace Ui {
    class CServiceSettingsWidget;
}

/****************************************************************************/
/**
 * \brief This widget contains a button to start the service software
 */
/****************************************************************************/
class CServiceSettingsWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

public:
    explicit CServiceSettingsWidget(QWidget *p_Parent = 0);
    ~CServiceSettingsWidget();
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);

private:
    Ui::CServiceSettingsWidget *mp_Ui;  //!< User interface
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    //Flags
    bool m_ProcessRunning;                  //!< Process running state
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role
    void RetranslateUI();
    void ResetButtons();

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);

private slots:
    void OnProcessStateChanged();
    void OnUserRoleChanged();
    void OnResetOperationDays();
    void OnResetCarbonFilter();
};

} // end namespace Settings

#endif // SETTINGS_SERVICESETTINGSWIDGET_H
