/****************************************************************************/
/*! \file ModifyReagentGroupColorDlg.cpp
 *
 *  \brief ModifyReagentGroupColorDlg Implementation.
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
#ifndef ModifyReagentGroupColorDlg_H
#define ModifyReagentGroupColorDlg_H


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
    class CModifyReagentGroupColorDlg;
}

/****************************************************************************/
/**
 * \brief Edit dialog for  reagents of the user
 */
/****************************************************************************/
class CModifyReagentGroupColorDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

private:
    Ui::CModifyReagentGroupColorDlg *mp_Ui;                   //!< User interface
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    MainMenu::CMessageDlg m_MessageDlg;             //! Message dialog object
    DataManager::CReagentGroup m_ReagentGroup;
    //Flags
    bool m_ProcessRunning;                          //!< Process running state
    bool m_UserRoleChanged;                         //!< True if user Role changed else false
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role
    MainMenu::CBaseTable *mp_TableWidget;           //!< Reagent table
    QStandardItemModel m_ReagentStatusColorModel;   //!< Model for the table
    QStringList m_ColorNames;                       //!< store color names


    void AddEmptyRows();

public:
    explicit CModifyReagentGroupColorDlg(QWidget *p_Parent = NULL,
                               MainMenu::CMainWindow *p_MainWindow = NULL);
    virtual ~CModifyReagentGroupColorDlg();
    void SetReagentGroupList(DataManager::CReagentGroupColorList &mp_CReagentGroupColorList,const DataManager::CDataReagentGroupList &ReagentGroupList, const DataManager::CReagentGroup &ReagentGroup);
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
     *  \brief This signal is emitted when ReagentList is updated.
     *
     */
    /****************************************************************************/
    void UpdateReagentGroup(DataManager::CReagentGroup& ReagentGroup);

};
}

#endif // ModifyReagentGroupColorDlg_H
