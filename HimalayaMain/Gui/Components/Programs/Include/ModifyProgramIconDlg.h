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
#ifndef MODIFYPROGRAMICONDLG_H
#define MODIFYPROGRAMICONDLG_H

#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"
#include "MainMenu/Include/DialogFrame.h"
#include "Programs/Include/ProgramModel.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/MessageDlg.h"
#include <QButtonGroup>

namespace Programs {

namespace Ui {
    class CModifyProgramIconDlg;
}

/****************************************************************************/
/**
 * \brief Edit dialog for  reagents of the user
 */
/****************************************************************************/
class CModifyProgramIconDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

private:
    Ui::CModifyProgramIconDlg *mp_Ui;               //!< User interface
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    MainMenu::CMessageDlg m_MessageDlg;             //! Message dialog object
    //Flags
    bool m_ProcessRunning;                          //!< Process running state
    bool m_UserRoleChanged;                         //!< True if user Role changed else false
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;//! < Current user role
    QStringList m_ProgramIconNames;                 //!< store Icon List names
    DataManager::CProgram m_Program;
    QButtonGroup m_ButtonGroup;


//    void AddEmptyRows();

public:
    explicit CModifyProgramIconDlg(QWidget *p_Parent = NULL,
                                   MainMenu::CMainWindow *p_MainWindow = NULL);
    virtual ~CModifyProgramIconDlg();
//    void SetProgramIconList( const DataManager::CProgram &Program);
private slots:
    void OnOk();
    void OnCancel();
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
    void UpdateProgram(DataManager::CProgram& Program);

};
}

#endif // MODIFYPROGRAMICONDLG_H
