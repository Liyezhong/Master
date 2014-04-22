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


#include "KeyBoard/Include/KeyBoard.h"

namespace Reagents {

namespace Ui {
    class CReagentStatusWidget;
}



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
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole; //!< Current user role
    bool m_ProcessRunning;                          //!< Process running state
    bool m_ShowMessageDialog;                       //!< To show Information Message Dialog
    DataManager::CReagent *mp_Reagent;              //!< Reagent object
    DataManager::CDashboardStation *mp_DashStation;        //!< Station object
    QStringList m_ReagentList;                      //!< List of Reagents
    Global::RMSOptions_t m_RMSOptions;       ///<  Definition/Declaration of variable m_RMSOptions
    Global::RMSOptions_t m_RMSCleaningOptions;       ///<  Definition/Declaration of variable m_RMSCleaningOptions
    QModelIndex m_CurrentIndex;       ///<  Definition/Declaration of variable m_CurrentIndex
    QString m_strSetAsEmpty;       ///<  Definition/Declaration of variable m_strSetAsEmpty
    QString m_strResetData;       ///<  Definition/Declaration of variable m_strResetData
    QString m_strSetAsFull;       ///<  Definition/Declaration of variable m_strSetAsFull
    QString m_CurrentStationName;       ///<  Definition/Declaration of variable m_CurrentStationName
    QList<QString> m_StationList;       ///<  Definition/Declaration of variable m_StationList
    QString m_strConfirmMsg;       ///<  Definition/Declaration of variable m_strConfirmMsg
    QString m_strYes;       ///<  Definition/Declaration of variable m_strYes
    QString m_strNo;       ///<  Definition/Declaration of variable m_strNo
protected:
    void showEvent(QShowEvent *);
    void changeEvent(QEvent *p_Event);
private:
    void PopulateReagentList();
    void ResizeHorizontalSection();
    void RetranslateUI();
    void ResetButtons();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ControlColumnShow
     *
     *  \return from ControlColumnShow
     */
    /****************************************************************************/
    void ControlColumnShow();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsProcessReagentExpired
     *
     *  \return from IsProcessReagentExpired
     */
    /****************************************************************************/
    bool IsProcessReagentExpired();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsCleaningReagentExpired
     *
     *  \return from IsCleaningReagentExpired
     */
    /****************************************************************************/
    bool IsCleaningReagentExpired();
public:
    explicit CReagentStatusWidget(QWidget *p_Parent = 0);
    ~CReagentStatusWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(Core::CDataConnector *p_DataConnector,
                            DataManager::CDataReagentList *p_ReagentList,
                            KeyBoard::CKeyBoard *p_KeyBoard = NULL);
private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnSetAsEmpty();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnSetAsFull();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnResetData();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void SelectionChanged(QModelIndex Index);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnProcessStateChanged();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnUserRoleChanged();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void StationReagentUpdated(const QString&);

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void RMSChanged(const Global::RMSOptions_t RMSValue);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void RMSCleaningChanged(const Global::RMSOptions_t RMSValue);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateSelectedStationList(QList<QString>&);

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when ReagentList is updated.
     *
     */
    /****************************************************************************/
    void UpdateReagentList();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateStationChangeReagent
     */
    /****************************************************************************/
    void UpdateStationChangeReagent(const QString&, const QString&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateStationSetAsEmpty
     */
    /****************************************************************************/
    void UpdateStationSetAsEmpty(const QString&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateStationSetAsFull
     */
    /****************************************************************************/
    void UpdateStationSetAsFull(const QString&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateStationResetData
     */
    /****************************************************************************/
    void UpdateStationResetData(const QString&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UnselectProgram
     */
    /****************************************************************************/
    void UnselectProgram();

};

} // end namespace Reagents

#endif // REAGENTSTATUSWIDGET_H
