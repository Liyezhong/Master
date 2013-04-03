/****************************************************************************/
/*! \file ModifyProgramDlg.h
 *
 *  \brief Include file for class CModifyProgramDlg.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28
 *   $Author:  $ M.Scherer, C.Adaragunchi, Swati Tiwari
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

#ifndef PROGRAMS_MODIFYPROGRAMDLG_H
#define PROGRAMS_MODIFYPROGRAMDLG_H

#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/DialogFrame.h"
#include "Programs/Include/ModifyProgramStepDlg.h"
#include "Core/Include/ReagentRMSModel.h"
#include "Programs/Include/RackGripColorDlg.h"
#include "Programs/Include/StepModel.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "Programs/Include/ProgramWidget.h"
#include "KeyBoard/Include/KeyBoardObserver.h"
#include "MainMenu/Include/MainWindow.h"
#include "Core/Include/DataContainer.h"
#include "MainMenu/Include/ContentScroller.h"
#include "Programs/Include/ModifyProgramIconDlg.h"

namespace Programs {
    #define MAX_PROGRAM_STEPS 50 //!< Maximum number of program steps
    #define MAX_LONGNAME_LENGTH 20  //!< Maximum length program long name
    #define MIN_LONGNAME_LENGTH 1   //!< Manimum length program long name
    #define MAX_SHORTNAME_LENGTH 3  //!< Maximum length program short name
    #define MIN_SHORTNAME_LENGTH 1  //!< Maximum length program short name

namespace Ui {
    class CModifyProgramDlg;
}

/****************************************************************************/
/**
 * \brief Edit dialog for a user defined  program.
 */
/****************************************************************************/
class CModifyProgramDlg : public MainMenu::CDialogFrame,
                          public KeyBoard::CKeyBoardObserver
{
    Q_OBJECT
    friend class CTestPrograms;

private:
    Ui::CModifyProgramDlg *mp_Ui;                   //!< User interface
    CModifyProgramStepDlg *mp_ModifyProgStepDlg;    //!< Program step edit dialog
    MainMenu::CBaseTable *mp_TableWidget;           //!< Program step table
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    Core::CDataConnector *mp_DataConnector;         //!< Global data container
    DataManager::CProgram *mp_Program;              //!< Program data displayed
    DataManager::CProgram m_Program;                //!< Temporary/local Program data used for New Program
    DataManager::CProgramStep m_SelectedStep;       //!< Currently selected program step
    CStepModel m_StepModel;                         //!< Model for the program step table
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;         //!< Reference to Keyboard widget
    KeyBoard::ValidationType_t m_ValidationType;    //!< Keyboard input validation type
    bool m_ProgNameBtnClicked;                      //!< True if Btn Program Name is clicked
    bool m_ProgShortNameBtnClicked;                 //!< True if Btn Program Short Name is clicked
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //!< Current user role
    bool m_ProcessRunning;                          //!< Process running state
    Programs::ButtonType_t m_ButtonType;            //!< Informs which button was clicked
    QString m_TempColor;                            //!< String to store program color for temperory
    bool m_TempColorFlag;                           //!< True if process is running or User is a Standard user
    CProgramModel m_ProgramModel;                   //!< Program model object
    MainMenu::CMessageDlg *mp_MessageDlg;             //!< Message Dialog object
    DataManager::CProgramStep m_LastProgramStep;    //!< ProgramStep object
    QString m_DeviceMode;                           //!< Stores device mode
    DataManager::CDataProgramList m_ProgramListClone;   //!< ProgramList object
    DataManager::CProgram m_ColorReplacedProgram;       //!< Program object
    DataManager::CProgram *mp_NewProgram;           //!< Program object
    bool m_ColorReplaced;                           //!< True if program color is replaced
    QList<DataManager::CProgramStep *> m_ListOfProgramSteps; //!< Container for Program Step pointers.
    MainMenu::CContentScroller m_ContentScroller;       //!< ContentScroller object
    DataManager::CReagentGroup m_ReagentGroup;
    MainMenu::CMessageDlg m_MessageDlg;             //!< Information Message Dialog
    CModifyProgramIconDlg *mp_ModifyProgramIconDlg;     //!< Edit Program Icon dialog
    DataManager::CProgramStep m_LeicaProgramStepOne;           //!< ProgramStep object



protected:
    void showEvent(QShowEvent *p_Event);
    void changeEvent(QEvent *p_Event);

public:
    explicit CModifyProgramDlg(QWidget *p_Parent = NULL, KeyBoard::CKeyBoard *p_KeyBoard = NULL,
                               MainMenu::CMainWindow *p_MainWindow = NULL,
                               Core::CDataConnector *p_DataConnector = NULL);
    ~CModifyProgramDlg();
    void InitDialog(DataManager::CProgram const *p_Program);
    void SetRowFocus(qint32 CurrentRow);

    /****************************************************************************/
    /*!
     *  \brief Informs which button was clicked on Menu program screen
     *
     *  \iparam ButtonType - Edit/New/Copy
     */
    /****************************************************************************/
    void SetButtonType(ButtonType_t ButtonType) { m_ButtonType = ButtonType;}

    /****************************************************************************/
    /*!
     *  \brief Closes ModifyProgramStep dialog
     *
     */
    /****************************************************************************/
    void CloseDialogModifyStepDlg();

    /****************************************************************************/
    /*!
     *  \brief Sets device mode.
     *
     *  \iparam DeviceMode - Standalone/Workstation
     */
    /****************************************************************************/
    void SetCurrentDeviceMode(QString DeviceMode){ m_DeviceMode = DeviceMode;}

    void Update();
    void UpdateOnESC();
    void NewProgram();    
    void SetReadyProgram(QString);
    bool VerifyLastProgramStep(DataManager::CProgram *p_CurrentProgram, QString DeviceMode);
    void DeleteSelectedStep(DataManager::CProgram* p_CurrentProgram);

private:
    void ResizeHorizontalSection();
    DataManager::CProgramStep *SelectedStep();
    void OnOkClicked();
    void EscClicked();
    void RetranslateUI();
    void ResetButtons(DataManager::CProgram &CurrentProgram, bool SelectionChanged);
    bool VerifyLastStepForHimalayaMode(DataManager::CProgram* p_SelectedProgram);
    bool VerifyLastStepForWorkStationMode(DataManager::CProgram* p_SelectedProgram);

private slots:
//    void OnEditShortName();
    void OnEditName();
//    void OnColor();
    void OnEdit();
    void OnNew();
    void OnCopy();
    void OnDelete();
    void OnSave();
    void OnCancel();
    void OnSelectionChanged(QModelIndex);
    void OnProcessStateChanged();
    void UpdateProgramStepTable(DataManager::CProgramStep* ProgramStep, bool AddProgramStep);
    void CloseDialogs();
    void OnUpdateProgramColor(DataManager::CProgram &Program,bool ColorReplaced);
    void UpdateUserSettings();
    void OnEndButtonClicked();
    void OnUpButtonClicked();
    void OnDownButtonClicked();
    void OnIconClicked();


signals:
    /****************************************************************************/
    /*!
     *  \brief This siganl is emitted when reagents are updated.
     */
    /****************************************************************************/
    void ReagentsUpdated();

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when the ProgramStepList is updated and informs
     *  Program Step model to update the program step list data.
     */
    /****************************************************************************/
    void UpdateStepModel();

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
     *  \brief This signal is emitted when the color of one program is assigned to
     *         the selected program.
     *
     *  \iparam ColorReplacedProgram = Program whose color was assigned to selected program.
     *  \iparam ColorAssignedProgram = Selected program for which the color was assigned.
     */
    /****************************************************************************/
    void ProgramColorReplaced(DataManager::CProgram &ColorReplacedProgram,
                              DataManager::CProgram &ColorAssignedProgram);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when Cancel button in the dailog is pressed.
     *
     */
    /****************************************************************************/
    void CancelClicked();
};

} // end namespace Programs

#endif // PROGRAMS_MODIFYPROGRAMDLG_H
