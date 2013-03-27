/****************************************************************************/
/*! \file ModifyReagentStatusDlg.cpp
 *
 *  \brief ModifyReagentStatusDlg Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-29
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
#ifndef MODIFYREAGENTSTATUSDLG_H
#define MODIFYREAGENTSTATUSDLG_H


#include "HimalayaDataContainer/Containers/Reagents/Include/Reagent.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "KeyBoard/Include/KeyBoardObserver.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "Reagents/Include/ReagentStatusWidget.h"
#include "MainMenu/Include/ScrollTable.h"
#include "Core/Include/ReagentStatusEditModel.h"
#include <QButtonGroup>

namespace Reagents {

namespace Ui {
    class CModifyReagentStatusDlg;
}

/****************************************************************************/
/**
 * \brief Edit dialog for  reagents of the user
 */
/****************************************************************************/
class CModifyReagentStatusDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

private:
    Ui::CModifyReagentStatusDlg *mp_Ui;                   //!< User interface    
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    MainMenu::CMessageDlg m_MessageDlg;             //! Message dialog object
    DataManager::CDashboardStation m_DashboardStation;
    DataManager::CDashboardStation *mp_DashboardStation;
    //Flags
    bool m_ProcessRunning;                          //!< Process running state
    bool m_UserRoleChanged;                         //!< True if user Role changed else false
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role
    MainMenu::CBaseTable *mp_TableWidget;           //!< Reagent table
    Core::CReagentStatusEditModel m_ReagentEditModel;             //!< Model for the table
    Core::CDataConnector *mp_DataConnector;         //!< Global data container

public:
    explicit CModifyReagentStatusDlg(QWidget *p_Parent = NULL,
                               MainMenu::CMainWindow *p_MainWindow = NULL,
                               Core::CDataConnector *p_DataConnector= NULL);
    virtual ~CModifyReagentStatusDlg();

    void SetDashboardStation(DataManager::CDashboardStation* p_Station);

private:
    void RetranslateUI();
    void PopulateReagentList();
    void ResizeHorizontalSection();

private slots:
    void OnOk();
    void OnCancel();
    void SelectionChanged(QModelIndex Index);
    void OnProcessStateChanged();
protected:
    void changeEvent(QEvent *p_Event);

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when Cancel button in the dailog is pressed.
     *
     */
    /****************************************************************************/
    void CancelPressed();

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when Station Reagent changed to empty
     *
     */
    /****************************************************************************/
    void UpdateStationSetAsEmpty(const QString&);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when DashboardStation Reagent is Changed.
     *
     */
    /****************************************************************************/
    void UpdateStationChangeReagent(const QString&, const QString&);

};
}

#endif // MODIFYREAGENTSTATUSDLG_H
