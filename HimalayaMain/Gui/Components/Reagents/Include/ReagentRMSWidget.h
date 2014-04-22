/****************************************************************************/
/*! \file ReagentRMSWidget.h
 *
 *  \brief ReagentRMSWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-08-22,  2011-09-29
 *   $Author:  $ M.Scherer, C.Adaragunchi1234
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

#ifndef REAGENTS_REAGENTRMSWIDGET_H
#define REAGENTS_REAGENTRMSWIDGET_H

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/MainWindow.h"
#include "Core/Include/ReagentRMSModel.h"
#include "Core/Include/DataConnector.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "Global/Include/Utils.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/SliderControl.h"
#include "MainMenu/Include/FileView.h"

#include "KeyBoard/Include/KeyBoard.h"
#include "MainMenu/Include/MessageDlg.h"

namespace Reagents {

namespace Ui {
    class CReagentRMSWidget;
}

class CModifyReagentRMSDlg;

//!< Button Type -Edit, New or Copy
typedef enum {
    EDIT_BTN_CLICKED = 1,
    NEW_BTN_CLICKED,
    COPY_BTN_CLICKED
}ButtonType_t;

/****************************************************************************/
/**
 * \brief This class displays all settings related to the reagents of Himalaya
 */
/****************************************************************************/
class CReagentRMSWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    friend class CTestReagents;
private:
    Ui::CReagentRMSWidget *mp_Ui;                     //!< User interface
    DataManager::CHimalayaUserSettings m_UserSettings;    //!< Data object
    //Flags
    bool m_ProcessRunning;       ///<  Definition/Declaration of variable m_ProcessRunning
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //!< Current user role
    MainMenu::CBaseTable *mp_TableWidget;           //!< Reagent table
    MainMenu::CBaseTable *mp_TableWidgetCleaning;           //!< Reagent table
    Core::CReagentRMSModel m_ReagentRMSModel;             //!< Model for the table
    Core::CReagentRMSModel m_ReagentCleaningModel;             //!< Model for the table
    Core::CDataConnector *mp_DataConnector;         //!< Global data container
    DataManager::CDataReagentList *mp_ReagentList;  //!< Reagent list
    DataManager::CReagent m_SelectedReagent;        //!< Currently selected reagent
    CModifyReagentRMSDlg *mp_ModifiyReagentRMSDlg;        //!< Edit reagent dialog
    MainMenu::CMessageDlg m_MessageDlg;             //!< Information Message Dialog
    bool m_ShowMessageDialog;                       //!< To show Information Message Dialog
    DataManager::CReagent const *mp_Reagent;        //!< Reagent object
    QString m_strNewReagent;       ///<  Definition/Declaration of variable m_strNewReagent
    QString m_strEditReagent;       ///<  Definition/Declaration of variable m_strEditReagent
    QString m_strConfirmDelReagent;       ///<  Definition/Declaration of variable m_strConfirmDelReagent
    QString m_strConfirmMsg;       ///<  Definition/Declaration of variable m_strConfirmMsg
    QString m_strYes;       ///<  Definition/Declaration of variable m_strYes
    QString m_strCancel;       ///<  Definition/Declaration of variable m_strCancel
public:
    explicit CReagentRMSWidget(QWidget *p_Parent = 0);
    ~CReagentRMSWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(Core::CDataConnector *p_DataConnector,
                            DataManager::CDataReagentList *p_ReagentList,
                            MainMenu::CMainWindow *p_MainWindow,
                            KeyBoard::CKeyBoard *p_KeyBoard = NULL);
    static Global::RMSOptions_t RMSPROCESSINGOPTION;       ///<  Definition/Declaration of variable RMSPROCESSINGOPTION
    static Global::RMSOptions_t RMSCLEANINGOPTIONS;       ///<  Definition/Declaration of variable RMSCLEANINGOPTIONS

private:
    void PopulateReagentList();
    void ResizeHorizontalSection();
    void ResetButtons();
    void RetranslateUI();
    void UpdateButtons(QString Id);

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);  

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
     *  \brief This signal is emitted to update the selected reagent.
     *
     *  \iparam Reagent = Selected reagent.
     */
    /****************************************************************************/
    void UpdateReagent(DataManager::CReagent &Reagent);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to add new reagent.
     *
     *  \iparam Reagent = Selected reagent.
     */
    /****************************************************************************/
    void AddReagent(DataManager::CReagent &Reagent);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to delete the selected reagent.
     *
     *  \iparam ReagentID = Selected reagent id.
     */
    /****************************************************************************/
    void RemoveReagent(QString ReagentID);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted Whenever Reagent Processing mode is changes
     */
    /****************************************************************************/
    void RMSChanged(const Global::RMSOptions_t);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal RMSCleaningChanged
     */
    /****************************************************************************/
    void RMSCleaningChanged(const Global::RMSOptions_t);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to delete the selected reagent.
     */
    /****************************************************************************/
    void RMSSettingChanged(DataManager::CUserSettings &);


private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnEdit();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnNew();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnDelete();
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
    void SelectionChangedCleaningTable(QModelIndex Index);
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
    void OnProcessStateChanged();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void CloseDialogs();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnRMSOFF();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnRMSCassettes();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnRMSCycles();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnRMSDays();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnCancelPressed();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnCleaningRMSOFF();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnCleaningRMSCycles();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnCleaningRMSDays();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateUserSetting();
};

} // end namespace Reagents

#endif // REAGENTS_REAGENTSWIDGET_H
