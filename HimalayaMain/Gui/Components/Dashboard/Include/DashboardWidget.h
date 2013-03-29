/****************************************************************************/
/*! \file DashboardWidget.h
 *
 *  \brief Dashboard Widget Class Definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-03
 *   $Author:  $ Swati Tiwari
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "Dashboard/Include/DashboardScene.h"
#include "Dashboard/Include/DashboardComboBox.h"
#include "Dashboard/Include/DashboardDataTypes.h"
#include <QFrame>
#include <QGridLayout>

namespace Dashboard {

namespace Ui {
    class CDashboardWidget;
}

/****************************************************************************/
/**
 * \brief This is the main widget for Dash Board.
 */
/****************************************************************************/
class CDashboardWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

private:
    Ui::CDashboardWidget *mp_Ui;                                //!< User interface
    MainMenu::CMainWindow *mp_MainWindow;                           //!< Reference to MainWindow.
    Core::CDataConnector *mp_DataConnector;                     //!< Global data container
    CDashboardScene *mp_DashboardScene;                         //!< Displays the station items
    QFrame *mp_Separator;                                       //!< Separator Line between View & Operation
    Dashboard::DashboardOperation_t m_Operation;                //!< Operation to be performed in Dashboatrd
    QButtonGroup m_btnGroup;                                    //!< Button Group
    Global::RMSOptions_t m_RMSState;                            //!< Current RMS State
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;        //!< Current user role
    bool m_UserRoleChanged;                                     //!< Flag to Verify the Change in User Role
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue

    void RunProgram();
    void PauseProgram();
    void AbortProgram();

public:
    explicit CDashboardWidget(Core::CDataConnector *p_DataConnector, MainMenu::CMainWindow *p_Parent = NULL);
    ~CDashboardWidget();

    void CheckPreConditionsToRunProgram();
    void CheckPreConditionsToPauseProgram();
    void CheckPreConditionsToAbortProgram();

protected:
    void DrawSeparatorLine();
    void AddItemsToComboBox();

signals:
    void ProgramAction(const QString& ProgramID, DataManager::ProgramActionType_t ActionType);

public slots:
    void OnUserRoleChanged();
    void OnButtonClicked(int whichBtn);
    void OnRMSValueChanged(Global::RMSOptions_t);

    // Warning Message Slots
    void OnPressYes();
    void OnPressNo();
    void OnPressCancel();
};

}
#endif // DASHBOARDWIDGET_H
