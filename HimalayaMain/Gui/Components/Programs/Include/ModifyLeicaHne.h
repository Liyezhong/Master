/****************************************************************************/
/*! \file ModifyLeicaHne.h
 *
 *  \brief Include file for class CModifyLeicaHne.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-16
 *   $Author:  $ M.Scherer, N.Kamath, C.Adaragunchi1234
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

#ifndef PROGRAMS_MODIFYLEICAHNE_H
#define PROGRAMS_MODIFYLEICAHNE_H

#include "MainMenu/Include/DialogFrame.h"
#include "Programs/Include/RackGripColorDlg.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "Core/Include/DataConnector.h"

namespace Programs {

namespace Ui {
    class CModifyLeicaHne;
}

/****************************************************************************/
/**
 * \brief Modification dialog for the Leica H&E program.
 */
/****************************************************************************/
class CModifyLeicaHne : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    explicit CModifyLeicaHne(QWidget *p_Parent = 0, MainMenu::CMainWindow *p_MainWindow = NULL,
                             Core::CDataConnector *p_DataConnector = NULL);
    ~CModifyLeicaHne();
    void InitDailog(DataManager::CProgram const *p_Program, DataManager::CProgramStep FirstProgramStep,
                    DataManager::CProgramStep SecondProgramStep);
    void SetIntensity(int FirstReagentIntensity, int SecondReagentIntensity);
    void SetReadyProgram(QString ReadyString);

private:
    Ui::CModifyLeicaHne *mp_Ui;                     //!< User interface
    DataManager::CProgram m_Program;                //!< Currently selected program
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //!< Current user role
    Core::CDataConnector *mp_DataConnector;         //!< DataConnector local object
    MainMenu::CScrollWheel *mp_ScrollWheelHaemotoxylin; //!< Scroll wheel object
    MainMenu::CScrollWheel *mp_ScrollWheelEosin;         //!< Scroll wheel object
    CRackGripColorDlg *mp_DlgRackGripColor;         //!< Rack color selection dialog
    DataManager::CDataProgramList *mp_ProgramList;  //!< Program list
    DataManager::CDataProgramList m_ProgramListClone; //!< Program list clone
    DataManager::CProgram m_ColorReplacedProgram;       //!< Program object
    DataManager::CProgramStep m_LeicaProgramStepOne;    //!< ProgramStep object
    DataManager::CProgramStep m_LeicaProgramStepTwo;    //!< ProgramStep object
    bool m_ColorReplaced;                               //!< Flag for color change
    bool m_ProcessRunning;                          //!< Process running state
    void RetranslateUI();


protected:
    void showEvent(QShowEvent *p_Event);
    void changeEvent(QEvent *p_Event);

private slots:
    void OnSave();
    void OnColor();
    void OnProcessStateChanged();
    void OnUpdateProgramColor(DataManager::CProgram &Program, bool ColorReplaced);

signals:
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
     *  \brief This signal is emitted when the color of one program is assigned to
     *         the selected program.
     *
     *  \iparam ColorReplacedProgram = Program whose color was assigned to selected program.
     *  \iparam ColorAssignedProgram = Selected program for which the color was assigned.
     */
    /****************************************************************************/
    void ProgramColorReplaced(DataManager::CProgram &ColorReplacedProgram,
                              DataManager::CProgram &ColorAssignedProgram);
};

} // end namespace Programs

#endif // PROGRAMS_MODIFYLEICAHNE_H
