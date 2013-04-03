/****************************************************************************/
/*! \file ProgramWidget.h
 *
 *  \brief Include file for class CProgramWidget.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28
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

#ifndef PROGRAMS_PROGRAMWIDGET_H
#define PROGRAMS_PROGRAMWIDGET_H

#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/PanelFrame.h"
#include "Programs/Include/ModifyLeicaHne.h"
#include "Programs/Include/ProgramModel.h"
#include "Programs/Include/RackGripColorDlg.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "MainMenu/Include/MainWindow.h"
#include "HimalayaDataContainer/Containers/Programs/Include/ProgramBase.h"


namespace Programs {

namespace Ui {
    class CProgramWidget;
}

//!< Forward declaration of Manual Program Dlg and Modify Program Dialog class
class CManualProgramDlg;
class CModifyProgramDlg;
class CModifyProgramStepDlg;

//!< Button Type -Edit, New or Copy
typedef enum {
    EDIT_BTN_CLICKED = 1,
    NEW_BTN_CLICKED,
    COPY_BTN_CLICKED
}ButtonType_t;

/****************************************************************************/
/**
 * \brief This is the main widget for the  programs of the device.
 */
/****************************************************************************/
class CProgramWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    friend class CTestPrograms;

private:
    Ui::CProgramWidget *mp_Ui;                  //!< User interface
    MainMenu::CBaseTable *mp_TableWidget;       //!< Table for the program list
    Core::CDataConnector *mp_DataConnector;     //!< Global data container
    DataManager::CProgram const *mp_Program;    //!< Currently selected program
    CProgramModel m_ProgramModel;               //!< Model of the program table
    CModifyProgramDlg *mp_ModifyProgramDlg;     //!< Modify program dialog
    MainMenu::CMessageDlg ConfirmationMessageDlg;
    CModifyLeicaHne *mp_ModifyLeicaHne;         //!< Edit dialog for the Leica H&E program
    CManualProgramDlg *mp_ManualProgramDlg;     //!< Manual program dialog
    MainMenu::CMainWindow *mp_MainWindow;       //!< Reference to main window
    MainMenu::CMessageDlg m_MessageDlg;         //!< Information Message Dialog
    bool m_UserRoleChanged;                     //!< True if user Role changed else false
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //!< Current user role
    bool m_ProcessRunning;                      //!< Process running state
    bool m_ShowMessageDialog;                   //!< To show Information Message Dialog
    ButtonType_t m_ButtonType;                  //!< Informs which button was clicked
    DataManager::CProgram m_ColorReplacedProgram;   //!< Program object for color change
    bool m_ColorReplaced;                       //!< Flag for program color change.
    QMap<QString,QString> m_NonStartableProgramErrorMap;    //!< Error Map for non-startable programs
    QString m_ID;                               //!< Program ID
    QModelIndex m_CurrentIndex;
    bool m_UserChanged;
    int m_UserProgramCount;

protected:
    void showEvent(QShowEvent *);

public:
    explicit CProgramWidget(Core::CDataConnector *p_DataConnector, MainMenu::CMainWindow *p_Parent = NULL,
                            KeyBoard::CKeyBoard *p_KeyBoard = NULL);
    ~CProgramWidget();
    bool IsFlagSet() { if(m_UserChanged == true) return true; else return false;}
    int GetNumberOfUserPrograms();

private:
    void PopulateProgramList();
    void ResizeHorizontalSection();
    void ResetButtons();
    void RetranslateUI();

private slots:
    void OnEdit();
    void OnNew();
    void OnCopy();
    void OnDelete();
    void SelectionChanged(QModelIndex Index);
    void OnUserRoleChanged();
    void OnProcessStateChanged();
    void CloseDialogs();
    void OnCurrentTabChanged(int CurrentIndex);
    void OnDeviceModeChanged(QString);
    void OnUpdateProgramColor(DataManager::CProgram &Program,bool ColorReplaced);
    void OnCancelClicked();

protected:
    void changeEvent(QEvent *p_Event);

signals:
    void FavoriteProgramListUpdated();
    /****************************************************************************/
    /*!
     *  \brief This siganl is emitted when a white rack is inserted into the loading station.
     */
    /****************************************************************************/
    void ManualProgramShow(QString);

    /****************************************************************************/
    /*!
     *  \brief This siganl is emitted when reagents are updated.
     */
    /****************************************************************************/
    void ReagentsUpdated();

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when the ProgramList is updated and informs
     *  Program model to update the program list data.
     */
    /****************************************************************************/
    void UpdateProgramList();

    /****************************************************************************/
    /*!
     *  \brief  This signal is emitted when the ProgramSequenceList is updated and informs
     *   ProgramSequence model to update the program sequence list data.
     */
    /****************************************************************************/
    void UpdateProgramSequenceList();

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when a program is updated.
     *
     *  \iparam Program = Selected Program.
     */
    /****************************************************************************/
    void UpdateProgram(DataManager::CProgram &Program);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when a new program is added.
     *
     *  \iparam Program = New Program.
     */
    /****************************************************************************/
    void AddProgram(DataManager::CProgram &Program);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when a program is deleted.
     *
     *  \iparam ProgramID = ProgramID of the selected program to be deleted.
     */
    /****************************************************************************/
    void DeleteProgram(QString &ProgramID);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to send Leica kit scan command.
     *
     *  \iparam ProgramLongName = Leica program long name
     *  \iparam IsKitScanViaBlg = Kit scan sequence via blg or update reagent
     *  \iparam KitCount = Leica kit count
     */
    /****************************************************************************/
    void ScanLeicaKitPrgWidget(QString ProgramLongName, bool IsKitScanViaBlg, int KitCount);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to send Leica bottle scan command.
     *
     *  \iparam IsKitScanViaBlg = Kit scan sequence via blg or update reagent
     */
    /****************************************************************************/
    void ScanLeicaBottlePrgWidget(bool IsKitScanViaBlg);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to send command to replace back original
     *  container in case of failure
     */
    /****************************************************************************/
    void ReplaceBackContainersPrgWidget();

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when the color of one program is assigned to the selected program.
     *
     *  \iparam ColorReplacedProgram = Program whose color was assigned to selected program.
     *  \iparam ColorAssignedProgram = Selected program for which the color was assigned.
     */
    /****************************************************************************/
    void ProgramColorReplaced(DataManager::CProgram &ColorReplacedProgram,
                              DataManager::CProgram &ColorAssignedProgram);

};

} // end namespace Programs

#endif // PROGRAMS_PROGRAMWIDGET_H
