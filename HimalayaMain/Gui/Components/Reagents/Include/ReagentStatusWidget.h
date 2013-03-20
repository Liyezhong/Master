#ifndef REAGENTSTATUSWIDGET_H
#define REAGENTSTATUSWIDGET_H

#include "Core/Include/ReagentStatusModel.h"
#include "Core/Include/DataConnector.h"
#include "Global/Include/Utils.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/FileView.h"
#include "Reagents/Include/ModifyReagentStatusDlg.h"

#include "KeyBoard/Include/KeyBoard.h"

namespace Reagents {

namespace Ui {
    class CReagentStatusWidget;
}

class CModifyReagentStatusDlg;

//!< Button Type -Edit, New or Copy
typedef enum {
    REDIT_BTN_CLICKED = 1,
    SET_AS_EMPTY_BTN_CLICKED,
    SET_AS_FULL_BTN_CLICKED,
    RESET_DATA_BTN_CLICKED
}ButtonGroup_t;
/****************************************************************************/
/**
 * \brief Main  reagents widget listing all reagents available
 */
/****************************************************************************/
class CReagentStatusWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    friend class CTestReagents;

private:
    Ui::CReagentStatusWidget *mp_Ui;      //!< User interface
    MainMenu::CBaseTable *mp_TableWidget;           //!< Reagent table
    Core::CReagentStatusModel m_ReagentStatusModel;             //!< Model for the table
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
    ButtonGroup_t m_ButtonType;                      //!< Informs which button was clicked
    CModifyReagentStatusDlg *mp_ModifiyReagentStatusDlg;        //!< Edit reagent dialog
    QStringList m_ReagentList;                      //!< List of Reagents
    MainMenu::CFileView *mp_ReagentStatusWidgetView;     //!< Content of this widget
    QString m_Status;                               //!< Current Staus of Reagent
    KeyBoard::CKeyBoard *mp_KeyBoard;

protected:
    void showEvent(QShowEvent *);
    void changeEvent(QEvent *p_Event);
private:
    void PopulateReagentList();
    void ResizeHorizontalSection();
    void RetranslateUI();
    void ResetButtons();

public:
    explicit CReagentStatusWidget(QWidget *p_Parent = 0);
    ~CReagentStatusWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(Core::CDataConnector *p_DataConnector,
                            DataManager::CDataReagentList *p_ReagentList,
                            MainMenu::CMainWindow *p_MainWindow,
                            KeyBoard::CKeyBoard *p_KeyBoard = NULL);
private slots:
    void OnEdit();
    void OnSetAsEmpty();
    void OnSetAsFull();
    void OnResetData();
    void SelectionChanged(QModelIndex Index);
    void OnProcessStateChanged();
    void OnUserRoleChanged();
    void OnContentScrolled();

public slots:
    void RMSChanged(const Global::RMSOptions_t RMSValue);
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
    void UpdateStationSetAsFull(const QString&);
    void UpdateStationResetData(const QString&);

};

} // end namespace Reagents

#endif // REAGENTSTATUSWIDGET_H
