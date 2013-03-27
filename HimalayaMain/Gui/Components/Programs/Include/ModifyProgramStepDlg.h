/****************************************************************************/
/*! \file ModifyProgramStepDlg.h
 *
 *  \brief Include file for class CModifyProgramStepDlg.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28
 *   $Author:  $ M.Scherer, C.Adaragunchi1234, Swati Tiwari
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

#ifndef PROGRAMS_MODIFYPROGRAMSTEPDLG_H
#define PROGRAMS_MODIFYPROGRAMSTEPDLG_H

#include "Core/Include/ReagentModel.h"
#include "HimalayaDataContainer/Containers/Programs/Include/ProgramStep.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardDataStationList.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/MainWindow.h"
#include "Programs/Include/ProgramWidget.h"
#include <QButtonGroup>

//lint -sem(Programs::CModifyProgramStepDlg::InitDurationWidget, initializer)
//lint -e1565
namespace Programs {

namespace Ui {
    class CModifyProgramStepDlg;
}

/****************************************************************************/
/**
 * \brief Edit dialog for a program stop.
 */
/****************************************************************************/
class CModifyProgramStepDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

private:
    Ui::CModifyProgramStepDlg *mp_Ui;                   //!< User interface
    MainMenu::CBaseTable *mp_TableWidget;               //!< Reagent table
    MainMenu::CScrollWheel *mp_ScrollWheelHour;         //!< Time of the program step (hours)
    MainMenu::CScrollWheel *mp_ScrollWheelMin;          //!< Time of the program step (minutes)
    MainMenu::CScrollWheel *mp_ScrollWheelSec;          //!< Time of the program step (seconds)
    MainMenu::CMessageDlg *mp_MessageBox;               //!< Message dialog object
    MainMenu::CMainWindow *mp_MainWindow;               //!< Reference to main window.
    DataManager::CProgramStep *mp_ProgramStep;          //!< The data of the dialog
    DataManager::CProgramStep m_ProgramStep;            //!< Local Program Step object.
    DataManager::CDataReagentList *mp_ReagentList;      //!< Global reagent list
    DataManager::CDashboardDataStationList *mp_DashboardStationList;      //!< Global Station list
    QButtonGroup m_ButtonGroup;                         //!< Group combining tolerance radio buttons
    Core::CReagentModel m_ReagentModel;                 //!< Data model of the reagent table
    qint32 m_RowSelected;                               //!< Row selected in table
    bool m_RowNotSelected;                              //!< True - row not selected else selected
    bool m_NewProgramStep;                              //!< True if dialog popped up for New Program Step.
    bool m_ProcessRunning;                              //!< State of process running    
    Programs::ButtonType_t m_ModifyProgramDlgButtonType;//!< Informs which button was clicked in ModifyProgramDialog
    QString m_SelectedStepReagentID;                    //!< Contains Selected ProgramStep reagent id
    bool m_ReagentExists;                               //!< True if Reagent exists in the reagent model
    QString m_DeviceMode;                               //!< To store device mode( Standalone/WorkstationMode)
    DataManager::CUserSettings m_UserSettings;          //!< UserSettings object



private:
    void showEvent(QShowEvent *p_Event);
    void RetranslateUI();

public:
    explicit CModifyProgramStepDlg(QWidget *p_Parent = NULL, MainMenu::CMainWindow *p_MainWindow = NULL);

    ~CModifyProgramStepDlg();

    /****************************************************************************/
    /*!
     *  \brief Informs which button was clicked on Menu program screen
     *
     *  \iparam ButtonType - Edit/New/Copy
     */
    /****************************************************************************/
    void SetButtonType(ButtonType_t ButtonType) { m_ModifyProgramDlgButtonType = ButtonType;}

    void SetProgramStep(DataManager::CProgramStep *p_ProgramStep,
                        DataManager::CDataReagentList *p_ReagentList);
    void NewProgramStep(DataManager::CDataReagentList *p_ReagentList);
    void SelectRow(qint32 Row);

    void ShowSelectReagentPopup();
    void ResetButtons(bool Disable);

    /****************************************************************************/
    /*!
     *  \brief Sets current device mode.
     *
     *  \iparam DeviceMode - Standalone/Workstation
     */
    /****************************************************************************/
    void SetDeviceMode(QString DeviceMode) { m_DeviceMode = DeviceMode; }

    /****************************************************************************/
    /*!
     *  \brief Sets UserSettings
     *
     *  \iparam p_UserSettings - Pointer of UserSettings.
     */
    /****************************************************************************/
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings) { m_UserSettings = *p_UserSettings;}

private:
    void InitDurationWidget();
    void ResizeHorizontalSection();

protected:
    void changeEvent(QEvent *p_Event);

private slots:
    void OnOk();
    void OnCancel();
    void OnSelectionChanged(QModelIndex);
    void ReagentTableUpdate();
    void OnProcessStateChanged();

signals:

    /****************************************************************************/
    /*!
     *  \brief This siganl is emitted when reagents are updated.
     */
    /****************************************************************************/
    void ReagentsUpdated();

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when a new program step is added.
     *
     *  \iparam p_ProgramStep = New Program step.
     *  \iparam AddProgramStep = True if New Program step is added else False
     */
    /****************************************************************************/
    void AddProgramStep(DataManager::CProgramStep *p_ProgramStep, bool AddProgramStep);

};

} // end namespace Programs

#endif // PROGRAMS_MODIFYPROGRAMSTEPDLG_H
