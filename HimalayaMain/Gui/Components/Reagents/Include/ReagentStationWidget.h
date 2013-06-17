#ifndef REAGENTSTATIONWIDGET_H
#define REAGENTSTATIONWIDGET_H

#include "Core/Include/ReagentStationModel.h"
#include "Core/Include/DataConnector.h"
#include "Global/Include/Utils.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/FileView.h"
#include "Reagents/Include/ModifyReagentStationDlg.h"
#include "KeyBoard/Include/KeyBoard.h"

namespace Reagents {
namespace Ui{
    class CReagentStationWidget;
}

class CModifyReagentStationDlg;

/**********************************************************/
/**
 * \brief Main  reagents widget listing all reagents available
 */
/****************************************************************************/

class CReagentStationWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    friend class CTestReagents;

private:
    Ui::CReagentStationWidget *mp_Ui;      //!< User interface
    MainMenu::CBaseTable *mp_TableWidget;           //!< Reagent table
    Core::CReagentStationModel m_ReagentStationModel;             //!< Model for the table
    Core::CDataConnector *mp_DataConnector;         //!< Global data container
    DataManager::CDataReagentList *mp_ReagentList;  //!< Reagent list
    DataManager::CReagent m_SelectedReagent;        //!< Currently selected reagent
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    MainMenu::CMessageDlg m_MessageDlg;             //!< Information Message Dialog
    bool m_UserRoleChanged;                         //!< True if user Role changed else false
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole; //! < Current user role
    bool m_ProcessRunning;                          //!< Process running state
    bool m_ShowMessageDialog;                       //!< To show Information Message Dialog
    DataManager::CReagent *mp_Reagent;              //!< Reagent object
    DataManager::CDashboardStation *mp_DashStation;        //!< Station object
    DataManager::CUserSettings *mp_UserSettings;    //!< Data object
    CModifyReagentStationDlg *mp_ModifiyReagentStationDlg;        //!< Edit reagent dialog
    MainMenu::CFileView *mp_ReagentStatusWidgetView;     //!< Content of this widget
    KeyBoard::CKeyBoard *mp_KeyBoard;
    Global::RMSOptions_t m_RMSOptions;
    QString m_strSelectReagent;
protected:
    void showEvent(QShowEvent *);
    void changeEvent(QEvent *p_Event);
private:
    void PopulateReagentList();
    void ResizeHorizontalSection();
    void RetranslateUI();
    void ResetButtons();

public:
    explicit CReagentStationWidget(QWidget *p_Parent = 0);
    ~CReagentStationWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(Core::CDataConnector *p_DataConnector,
                            DataManager::CDataReagentList *p_ReagentList,
                            MainMenu::CMainWindow *p_MainWindow,
                            KeyBoard::CKeyBoard *p_KeyBoard = NULL);
private slots:
    void OnEdit();
    void SelectionChanged(QModelIndex Index);
    void OnProcessStateChanged();
    void OnUserRoleChanged();
    void StationReagentUpdated(QString);

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when ReagentList is updated.
     *
     */
    /****************************************************************************/
    void UpdateReagentList();
    void UpdateStationChangeReagent(const QString&, const QString&);
    void UpdateStationSetAsEmpty(const QString&);
    void UnselectProgram();

};

} // end namespace Reagents

#endif // REAGENTSTATUSWIDGET_H
