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
    bool m_UserRoleChanged;                         //!< True if user Role changed else false
    bool m_ProcessRunning;
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role
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
    KeyBoard::CKeyBoard *mp_KeyBoard;
    QString m_strNewReagent;
    QString m_strEditReagent;
    QString m_strConfirmDelReagent;
public:
    explicit CReagentRMSWidget(QWidget *p_Parent = 0);
    ~CReagentRMSWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(Core::CDataConnector *p_DataConnector,
                            DataManager::CDataReagentList *p_ReagentList,
                            MainMenu::CMainWindow *p_MainWindow,
                            KeyBoard::CKeyBoard *p_KeyBoard = NULL);
    static Global::RMSOptions_t RMSPROCESSINGOPTION;
    static Global::RMSOptions_t RMSCLEANINGOPTIONS;

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
     *
     *  \iparam ReagentID = Selected reagent id.
     */
    /****************************************************************************/
    void RMSChanged(const Global::RMSOptions_t);

    void RMSCleaningChanged(const Global::RMSOptions_t);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to delete the selected reagent.
     *
     *  \iparam ReagentID = Selected reagent id.
     */
    /****************************************************************************/
    void RMSSettingChanged(DataManager::CUserSettings &);


private slots:
    void OnEdit();
    void OnNew();
    void OnDelete();
    void SelectionChanged(QModelIndex Index);
    void SelectionChangedCleaningTable(QModelIndex Index);
    void OnUserRoleChanged();
    void OnProcessStateChanged();
    void CloseDialogs();
    void OnRMSOFF();
    void OnRMSCassettes();
    void OnRMSCycles();
    void OnRMSDays();
    void OnCancelPressed();
    void OnCleaningRMSOFF();
    void OnCleaningRMSCycles();
    void OnCleaningRMSDays();
    void UpdateUserSetting();
};

} // end namespace Reagents

#endif // REAGENTS_REAGENTSWIDGET_H
