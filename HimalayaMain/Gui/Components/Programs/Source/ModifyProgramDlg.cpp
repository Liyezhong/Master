/****************************************************************************/
/*! \file ModifyProgramDlg.cpp
 *
 *  \brief Implementation of file for class CModifyProgramDlg.
 *
 *  \b Description:
 *          This class implements a widget for displaying program step list of
 *          selected program in a table and provides the user interfaces to
 *          Edit program name and abbreviation, Add, Copy, Assigning color
 *          to the program and Delete a program step.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28
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

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"

#include "Programs/Include/ModifyProgramDlg.h"
#include "DataManager/Helper/Include/Helper.h"
#include "DataManager/Helper/Include/Types.h"
#include "ui_ModifyProgramDlg.h"
#include <QDebug>

namespace Programs {

const QString UNLOADER_STEP_ID = "S7";  //!< Unloader step id
const QString TRANSFER_STEP_ID = "S8";  //!< Transfer step id

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Reference to Parent widget
 *  \iparam p_KeyBoard = Reference to Keyboard widget
 *  \iparam p_MainWindow = Reference to MainWindow widget
 *  \iparam p_DataConnector = Reference to DataConnector
 */
/****************************************************************************/
CModifyProgramDlg::CModifyProgramDlg(QWidget *p_Parent, KeyBoard::CKeyBoard *p_KeyBoard,
                                     MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector):
                                     MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CModifyProgramDlg),
                                     mp_MainWindow(p_MainWindow), mp_DataConnector(p_DataConnector),
                                     m_ProgNameBtnClicked(false), m_ProgShortNameBtnClicked(false),
                                     m_ProcessRunning(false), m_TempColor(" "), m_TempColorFlag(false),
                                     mp_NewProgram(NULL), m_ColorReplaced(false)
{
    mp_Ui->setupUi(GetContentFrame());
    mp_KeyBoardWidget = p_KeyBoard;
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->horizontalHeader()->show();
    mp_Ui->scrollTable->SetContent(mp_TableWidget);
    mp_TableWidget->SetVisibleRows(6);

    mp_ModifyProgStepDlg = new CModifyProgramStepDlg(this, p_MainWindow);
    mp_ModifyProgStepDlg->setModal(true);
    mp_ModifyProgStepDlg->SetUserSettings(mp_DataConnector->SettingsInterface->GetUserSettings());

    mp_DlgRackGripColor = new CRackGripColorDlg(this,p_MainWindow);
    mp_DlgRackGripColor->setModal(true);

    CONNECTSIGNALSLOT(mp_Ui->btnColor, clicked(), this, OnColor());
    CONNECTSIGNALSLOT(mp_Ui->btnEdit, clicked(), this, OnEdit());
    CONNECTSIGNALSLOT(mp_Ui->btnNew, clicked(), this, OnNew());
    CONNECTSIGNALSLOT(mp_Ui->btnCopy, clicked(), this, OnCopy());
    CONNECTSIGNALSLOT(mp_Ui->btnDelete, clicked(), this, OnDelete());
    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnSave, clicked(), this, OnSave());
    CONNECTSIGNALSLOT(mp_TableWidget,clicked(QModelIndex), this, OnSelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(mp_Ui->btnPrgName, clicked(), this, OnEditName());
    CONNECTSIGNALSLOT(mp_Ui->btnPrgShortName, clicked(), this, OnEditShortName());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    CONNECTSIGNALSIGNAL(this, ReagentsUpdated(), mp_ModifyProgStepDlg, ReagentsUpdated());
    CONNECTSIGNALSLOT(this, UpdateStepModel(), &m_StepModel, UpdateStepModel());

    CONNECTSIGNALSLOT(mp_ModifyProgStepDlg, AddProgramStep(DataManager::CProgramStep*,bool),
                      this, UpdateProgramStepTable(DataManager::CProgramStep*,bool));
    CONNECTSIGNALSIGNAL(mp_DlgRackGripColor, UpdateProgram(DataManager::CProgram &),
                        this, UpdateProgram(DataManager::CProgram &));
    CONNECTSIGNALSLOT(mp_DlgRackGripColor, UpdateProgramColor(DataManager::CProgram &,bool),
                      this,OnUpdateProgramColor(DataManager::CProgram &,bool));

    CONNECTSIGNALSLOT(mp_Ui->groupBox, OnBeginButtonClicked(), this,OnBeginButtonClicked());
    CONNECTSIGNALSLOT(mp_Ui->groupBox, OnEndButtonClicked(), this, OnEndButtonClicked());
    CONNECTSIGNALSLOT(mp_Ui->groupBox, OnUpButtonClicked(), this, OnUpButtonClicked());
    CONNECTSIGNALSLOT(mp_Ui->groupBox, OnDownButtonClicked(), this, OnDownButtonClicked());

    m_StepModel.ConnectTableMoving(mp_Ui->groupBox);
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    OnProcessStateChanged();
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CModifyProgramDlg::~CModifyProgramDlg()
{
    try {
        delete mp_DlgRackGripColor;
        delete mp_ModifyProgStepDlg;
        delete mp_TableWidget;
        delete mp_Ui;
        delete mp_NewProgram;
    }
    catch (...) {
        // to please Lint.
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyProgramDlg::changeEvent(QEvent *p_Event)
{
    MainMenu::CDialogFrame::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        RetranslateUI();
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief Closes opened dialogs when master sends updated programs xml data.
 */
/****************************************************************************/
void CModifyProgramDlg::CloseDialogs()
{
    //  ResetButtons();

}
/****************************************************************************/
/*!
 *  \brief Resizes the columns in the program step table
 */
/****************************************************************************/
void CModifyProgramDlg::ResizeHorizontalSection()
{
    mp_TableWidget->horizontalHeader()->resizeSection(0, 45);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 180);
    mp_TableWidget->horizontalHeader()->resizeSection(2, 70);
    mp_TableWidget->horizontalHeader()->resizeSection(3, 70);
    mp_TableWidget->horizontalHeader()->resizeSection(4, 40);
}

/****************************************************************************/
/*!
 *  \brief Sets the content displayed in the dialog
 *
 *  \iparam p_Program = Reference to the Program displayed in the dialog
 */
/****************************************************************************/
void CModifyProgramDlg::InitDialog(DataManager::CProgram const *p_Program)
{
    if (p_Program != NULL) {
        m_Program = *(p_Program);
    }
    m_ProgramListClone = *(mp_DataConnector->ProgramList);
    if (m_StepModel.rowCount(QModelIndex()) > 0) {
        mp_TableWidget->selectRow(0);
    }
    m_StepModel.SetModifyProgramDlgPtr(this);
    ResizeHorizontalSection();

    QString LongName = m_Program.GetLongName();

    if (m_ButtonType == EDIT_BTN_CLICKED) {
        mp_Ui->btnPrgName->setText(QString("%1").arg(LongName));
        mp_Ui->btnPrgShortName->setText(QString("%1").arg(m_Program.GetShortName()));
        mp_Ui->btnColor->SetColor(m_Program.GetColor());
        mp_Ui->btnColor->setEnabled(false);
        // Pass a value same as the one passed to SetVisibleRows()
        m_StepModel.SetVisibleRowCount(6);
        m_StepModel.SetProgram(&m_Program, mp_DataConnector->ReagentList, 5);
        mp_TableWidget->setModel(&m_StepModel);
    }
    else if (m_ButtonType == COPY_BTN_CLICKED) {
        switch (LongName.length()) {
        case 32:
            LongName.replace(29, 3, "_cp");
            break;
        case 31:
            LongName.replace(28, 3, "_cp");
            break;
        case 30:
            LongName.replace(27, 3, "_cp");
            break;
        default:
            LongName.append("_cp");
            break;
        }
        mp_Ui->btnPrgName->setText(QString("%1").arg(LongName));
        mp_Ui->btnPrgShortName->setText("--");
        mp_Ui->btnColor->SetColor("black");
        mp_Ui->btnColor->setEnabled(false);
        m_Program.SetColor("white");
        // Pass a value same as the one passed to SetVisibleRows()
        m_StepModel.SetVisibleRowCount(6);
        m_StepModel.SetProgram(&m_Program, mp_DataConnector->ReagentList, 5);
        mp_TableWidget->setModel(&m_StepModel);
    }
    else {
        NewProgram();
    }
}

/****************************************************************************/
/*!
 *  \brief Sets up the dialog for New Program
 */
/****************************************************************************/
void CModifyProgramDlg::NewProgram()
{
    if (mp_NewProgram != NULL) {
        delete mp_NewProgram;
        mp_NewProgram = NULL;
    }

    mp_NewProgram = new DataManager::CProgram();
    //Pass a value same as the one passed to SetVisibleRows()
    m_StepModel.SetVisibleRowCount(6);
    m_StepModel.SetProgram(NULL, NULL, 5);
    mp_TableWidget->setModel(&m_StepModel);
    m_StepModel.SetModifyProgramDlgPtr(this);
    ResizeHorizontalSection();
    mp_Ui->btnPrgName->setText("--");
    mp_Ui->btnPrgShortName->setText("--");
    mp_Ui->btnColor->SetColor("black");
    mp_Ui->btnColor->setEnabled(false);
    mp_NewProgram->SetColor("white");
    mp_Ui->label_3->setText(tr("Program not ready to start"));
}

/****************************************************************************/
/*!
 *  \brief Returns the data of the currently selected program step
 *
 *  \return Currently selected program step
 */
/****************************************************************************/
DataManager::CProgramStep *CModifyProgramDlg::SelectedStep()
{
    QModelIndex Index = mp_TableWidget->selectionModel()->selectedRows()[0];
    QString Id = m_StepModel.data(Index, (int)Qt::UserRole).toString();
    if (m_ButtonType == NEW_BTN_CLICKED) {
        return const_cast<DataManager::CProgramStep*>(mp_NewProgram->GetProgramStep(Id));
    }
    else {
        return const_cast<DataManager::CProgramStep*>(m_Program.GetProgramStep(Id));
    }
}

/****************************************************************************/
/*!
 *  \brief This function focuses the required row
 *
 *  \iparam CurrentRow - Row to be focused.
 */
/****************************************************************************/
void CModifyProgramDlg::SetRowFocus(qint32 CurrentRow)
{
    mp_TableWidget->selectRow(CurrentRow);
}

/****************************************************************************/
/*!
 *  \brief This function is called when Ok button on keyboard is clicked
 */
/****************************************************************************/
void CModifyProgramDlg::Update()
{
    OnOkClicked();
}

/****************************************************************************/
/*!
 *  \brief This function is called when Ok button on keyboard is clicked
 */
/****************************************************************************/
void CModifyProgramDlg::UpdateOnESC()
{
    m_ProgNameBtnClicked = false;
    m_ProgShortNameBtnClicked = false;
}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to edit the program name
 */
/****************************************************************************/
void CModifyProgramDlg::OnEditName()
{
    mp_KeyBoardWidget->Attach(this);
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Program Name"));
    mp_KeyBoardWidget->SetPasswordMode(false);
    if (!(m_ButtonType == NEW_BTN_CLICKED)) {
        mp_KeyBoardWidget->SetLineEditContent(mp_Ui->btnPrgName->text());
    }
    else {
        if ((mp_Ui->btnPrgName->text() != "--")) {
            mp_KeyBoardWidget->SetLineEditContent(mp_Ui->btnPrgName->text());
        }
    }
    m_ValidationType = KeyBoard::VALIDATION_1;
    mp_KeyBoardWidget->SetValidationType(m_ValidationType);
    mp_KeyBoardWidget->SetMaxCharLength(MAX_LONGNAME_LENGTH);
    mp_KeyBoardWidget->SetMinCharLength(MIN_LONGNAME_LENGTH);
    mp_KeyBoardWidget->show();
    m_ProgNameBtnClicked  = true;
}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to edit the program short name
 */
/****************************************************************************/
void CModifyProgramDlg::OnEditShortName()
{
    mp_KeyBoardWidget->Attach(this);
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Program Short Name"));
    mp_KeyBoardWidget->SetPasswordMode(false);

    if (mp_Ui->btnPrgShortName->text() != "--") {
        mp_KeyBoardWidget->SetLineEditContent(mp_Ui->btnPrgShortName->text());
    }
    m_ValidationType = KeyBoard::VALIDATION_2;
    mp_KeyBoardWidget->SetValidationType(m_ValidationType);
    mp_KeyBoardWidget->SetMaxCharLength(MAX_SHORTNAME_LENGTH);
    mp_KeyBoardWidget->SetMinCharLength(MIN_SHORTNAME_LENGTH);
    mp_KeyBoardWidget->show();
    m_ProgShortNameBtnClicked = true;
}

/****************************************************************************/
/*!
 *  \brief Shows the edit program step dialog
 */
/****************************************************************************/
void CModifyProgramDlg::OnEdit()
{
    mp_ModifyProgStepDlg->SetDialogTitle(tr("Edit Program Step"));
    mp_ModifyProgStepDlg->SetDeviceMode(m_DeviceMode);
    mp_ModifyProgStepDlg->SetProgramStep(SelectedStep(), mp_DataConnector->ReagentList,
                                         mp_DataConnector->DashboardStationList);
    mp_ModifyProgStepDlg ->SetButtonType(EDIT_BTN_CLICKED);
    mp_ModifyProgStepDlg->move(80,50);
    mp_ModifyProgStepDlg->ShowSelectReagentPopup();
    mp_ModifyProgStepDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Inserts a new program step and shows the edit dialog
 */
/****************************************************************************/
void CModifyProgramDlg::OnNew()
{
    mp_ModifyProgStepDlg->SetDialogTitle(tr("New Program Step"));
    mp_ModifyProgStepDlg->SetDeviceMode(m_DeviceMode);
    mp_ModifyProgStepDlg->NewProgramStep(mp_DataConnector->ReagentList,
                                         mp_DataConnector->DashboardStationList);
    mp_ModifyProgStepDlg ->SetButtonType(NEW_BTN_CLICKED);
    mp_ModifyProgStepDlg->move(80,50);
    mp_ModifyProgStepDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Copies the currently selected program step
 */
/****************************************************************************/
void CModifyProgramDlg::OnCopy()
{
    mp_ModifyProgStepDlg->SetDialogTitle(tr("Edit Program Step"));
    mp_ModifyProgStepDlg->SetButtonType(COPY_BTN_CLICKED);
    mp_ModifyProgStepDlg->SetDeviceMode(m_DeviceMode);
    mp_ModifyProgStepDlg->SetProgramStep(SelectedStep(), mp_DataConnector->ReagentList,
                                         mp_DataConnector->DashboardStationList);
    mp_ModifyProgStepDlg->move(80,50);
    mp_ModifyProgStepDlg->show();
    mp_ModifyProgStepDlg->ShowSelectReagentPopup();
}

/****************************************************************************/
/*!
 *  \brief Deletes the currently selected program step
 */
/****************************************************************************/
void CModifyProgramDlg::OnDelete()
{
    MainMenu::CMessageDlg MessageDlg;
    MessageDlg.SetTitle(tr("Confirmation Message"));
    MessageDlg.SetIcon(QMessageBox::Question);
    MessageDlg.SetButtonText(1, tr("Yes"));
    MessageDlg.SetButtonText(3, tr("Cancel"));
    MessageDlg.HideCenterButton();
    MessageDlg.SetText(tr("Do you really want to delete the selected program step?"));

    if (MessageDlg.exec() == (int)QDialog::Accepted) {
        if (m_ButtonType == NEW_BTN_CLICKED) {
            DeleteSelectedStep(mp_NewProgram);
            if (mp_NewProgram->GetNumberOfSteps() == 0) {
                mp_Ui->btnDelete->setEnabled(false);
                mp_Ui->btnCopy->setEnabled(false);
                mp_Ui->btnEdit->setEnabled(false);
                mp_Ui->groupBox->setEnabled(false);
            }
        }
        else {
            DeleteSelectedStep(&m_Program);
            if (m_Program.GetNumberOfSteps() == 0) {
                mp_Ui->btnDelete->setEnabled(false);
                mp_Ui->btnCopy->setEnabled(false);
                mp_Ui->btnEdit->setEnabled(false);
                mp_Ui->groupBox->setEnabled(false);
            }
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Deletes the changes made in the dialog
 *  \iparam p_CurrentProgram = Reference to the selected program
 */
/****************************************************************************/
void CModifyProgramDlg::DeleteSelectedStep(DataManager::CProgram* p_CurrentProgram)
{
    int Index = mp_TableWidget->currentIndex().row();
    DataManager::CProgram *p_Program = p_CurrentProgram;

    // Added void to please lint
    (void) p_Program->DeleteProgramStep(Index);
    emit UpdateStepModel();
    ResetButtons(*p_Program, false);
}

/****************************************************************************/
/*!
 *  \brief Saves the changes made in the dialog
 */
/****************************************************************************/
void CModifyProgramDlg::OnSave()
{
    MainMenu::CMessageDlg MessageDlg;
    MessageDlg.SetTitle(tr("Information Message"));
    MessageDlg.SetIcon(QMessageBox::Information);
    MessageDlg.SetButtonText(1, tr("Ok"));
    MessageDlg.HideButtons();
    if (mp_Ui->btnPrgName->text() == "--") {
        MessageDlg.SetText(tr("Please enter a valid Program Name"));
        (void) MessageDlg.exec();
        return;
    }
    else if (mp_Ui->btnPrgShortName->text() == "--") {
        MessageDlg.SetText(tr("Please enter a valid Abbreviation"));
        (void) MessageDlg.exec();
        return;
    }
    m_Program.SetLongName(mp_Ui->btnPrgName->text());
    m_Program.SetShortName(mp_Ui->btnPrgShortName->text());
    if (m_ButtonType == EDIT_BTN_CLICKED) {
        if ((VerifyLastProgramStep(&m_Program, m_DeviceMode))) {
            if (m_ProgramListClone.UpdateProgram(&m_Program) == true) {
                if (m_ColorReplaced == true) {
                    emit ProgramColorReplaced(m_ColorReplacedProgram, m_Program);
                }
                else {
                    emit UpdateProgram(m_Program);
                }
            }
            else {
                ListOfErrors_t &ErrorList = m_ProgramListClone.GetErrorList();
                QString ErrorString;
                DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
                MessageDlg.SetText(ErrorString);
                (void) MessageDlg.exec();
            }
        }
    }
    else if (m_ButtonType == COPY_BTN_CLICKED) {
        if ((VerifyLastProgramStep(&m_Program, m_DeviceMode)) == true) {
            m_Program.SetID(m_ProgramListClone.GetNextFreeProgID(true));
            if (m_ProgramListClone.AddProgram(&m_Program) == true) {
                emit AddProgram(m_Program);
            }
            else {
                ListOfErrors_t &ErrorList = m_ProgramListClone.GetErrorList();
                QString ErrorString;
                DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
                MessageDlg.SetText(ErrorString);
                (void) MessageDlg.exec();
            }
        }
    }
    else {
        mp_NewProgram->SetLongName(mp_Ui->btnPrgName->text());
        mp_NewProgram->SetShortName(mp_Ui->btnPrgShortName->text());
        if ((VerifyLastProgramStep(mp_NewProgram, m_DeviceMode)) == true) {
            mp_NewProgram->SetID(m_ProgramListClone.GetNextFreeProgID(true));
            if (m_ProgramListClone.AddProgram(mp_NewProgram) == true) {
                emit AddProgram(*mp_NewProgram);
            }
            else {
                ListOfErrors_t &ErrorList = m_ProgramListClone.GetErrorList();
                QString ErrorString;
                DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
                MessageDlg.SetText(ErrorString);
                (void) MessageDlg.exec();
            }
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Cancels the changes made in the dialog
 */
/****************************************************************************/
void CModifyProgramDlg::OnCancel()
{
    m_TempColorFlag = false;
    emit CancelClicked();
    reject();
}

/****************************************************************************/
/*!
 *  \brief Opens the program color selection dialog
 */
/****************************************************************************/
void CModifyProgramDlg::OnColor()
{
    if (mp_DlgRackGripColor->Init(&m_ProgramListClone, &m_Program) == true) {
        mp_DlgRackGripColor->SetSaveButton(tr("Ok"));
        mp_DlgRackGripColor->move(220,150);
        mp_DlgRackGripColor->show();
    }
}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Currently selected cell
 */
/****************************************************************************/
void CModifyProgramDlg::OnSelectionChanged(QModelIndex Index)
{
    //Inform the model about the current row.
    m_StepModel.SetIndex(Index);
    int SelectedIndex = Index.row();
    //Retrieving only process running state and not user role since
    //user role can't change behind the scene.
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        if (m_ButtonType == NEW_BTN_CLICKED){

            if ((SelectedIndex+1) > mp_NewProgram->GetNumberOfSteps()) {
                ResetButtons(*mp_NewProgram, false);
                mp_TableWidget->clearSelection();
            }
            else {
                ResetButtons(*mp_NewProgram, true);
            }
        }
        else {
            //Edit Mode
            if ((SelectedIndex+1) > m_Program.GetNumberOfSteps()) {
                ResetButtons(m_Program, false);
                mp_TableWidget->clearSelection();
            }
            else {
                ResetButtons(m_Program, true);
            }
        }
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process Running state changes
 */
/****************************************************************************/
void CModifyProgramDlg::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnPrgName->setEnabled(true);
        mp_Ui->btnPrgShortName->setEnabled(true);
        if (m_ButtonType == NEW_BTN_CLICKED || m_ButtonType == COPY_BTN_CLICKED) {
            mp_Ui->btnColor->SetColor("black");
            mp_Ui->btnColor->setEnabled(false);
        }
        else {
            mp_Ui->btnColor->setEnabled(true);
            if(m_TempColorFlag == true ){
                mp_Ui->btnColor->SetColor(m_TempColor);
            } else{
                mp_Ui->btnColor->SetColor("white");
            }
        }
        mp_Ui->btnNew->setEnabled(true);
        mp_Ui->btnCancel->setEnabled(true);
        mp_Ui->btnSave->setEnabled(true);

    }
    else {
        if (m_ProcessRunning) {
            mp_Ui->btnSave->setEnabled(false);
        }
        else {
            //View Mode
            mp_Ui->btnPrgName->setEnabled(false);
            mp_Ui->btnPrgShortName->setEnabled(false);
            mp_Ui->btnCancel->setEnabled(true);
            mp_Ui->btnColor->SetColor("black");
            mp_Ui->btnColor->setEnabled(false);
            mp_Ui->btnDelete->setEnabled(false);
            mp_Ui->btnNew->setEnabled(false);
            mp_Ui->btnEdit->setEnabled(false);
            mp_Ui->btnCopy->setEnabled(false);
            mp_Ui->groupBox->setEnabled(false);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when OK button on Keyboard is clicked.
 */
/****************************************************************************/
void CModifyProgramDlg::OnOkClicked()
{
    QString LineEditString;
    mp_KeyBoardWidget->hide();
    if (m_ProgNameBtnClicked) {
        m_ProgNameBtnClicked = false;
        LineEditString = mp_KeyBoardWidget->GetLineEditString();
        mp_Ui->btnPrgName->setText(QString("%1").arg(LineEditString));
    }
    else if (m_ProgShortNameBtnClicked) {
        m_ProgShortNameBtnClicked = false;
        LineEditString = mp_KeyBoardWidget->GetLineEditString();
        mp_Ui->btnPrgShortName->setText(QString("%1").arg(LineEditString));
    }
    mp_KeyBoardWidget->Detach();
}

/****************************************************************************/
/*!
 *  \brief This event is called whenever widget is shown.Sets the states of
 *         various buttons as required for View and Edit Mode.
 *
 *   \iparam p_Event = Ref
 */
/****************************************************************************/
void CModifyProgramDlg::showEvent(QShowEvent *p_Event)
{
    Q_UNUSED(p_Event);
    ResizeHorizontalSection();
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        if (m_ButtonType == NEW_BTN_CLICKED) {
            mp_Ui->btnPrgName->setEnabled(true);
            mp_Ui->btnPrgShortName->setEnabled(true);
            mp_Ui->btnColor->SetColor("black");
            mp_Ui->btnColor->setEnabled(false);
            mp_Ui->btnNew->setEnabled(true);
            mp_Ui->btnCancel->setEnabled(true);
            mp_Ui->btnSave->setEnabled(true);
            mp_Ui->btnDelete->setEnabled(false);
            mp_Ui->btnCopy->setEnabled(false);
            mp_Ui->btnEdit->setEnabled(false);
            mp_Ui->groupBox->setEnabled(false);
        }
        else {
            m_TempColorFlag = true ;
            m_TempColor = m_Program.GetColor();
            mp_Ui->btnPrgName->setEnabled(true);
            mp_Ui->btnPrgShortName->setEnabled(true);
            if (m_ButtonType == COPY_BTN_CLICKED) {
                mp_Ui->btnColor->SetColor("black");
                mp_Ui->btnColor->setEnabled(false);
            }
            else {
                mp_Ui->btnColor->setEnabled(true);
            }
            mp_Ui->btnNew->setEnabled(true);
            mp_Ui->btnCancel->setEnabled(true);
            mp_Ui->btnSave->setEnabled(true);
            mp_Ui->btnDelete->setEnabled(false);
            mp_Ui->btnCopy->setEnabled(false);
            mp_Ui->btnEdit->setEnabled(false);
            mp_Ui->groupBox->setEnabled(false);
        }
    }
    else {
        //View Mode
        mp_Ui->btnPrgName->setEnabled(false);
        mp_Ui->btnPrgShortName->setEnabled(false);
        mp_Ui->btnCancel->setEnabled(true);
        mp_Ui->btnColor->setEnabled(false);
        mp_Ui->btnColor->SetColor("black");
        mp_Ui->btnCopy->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);
        mp_Ui->btnNew->setEnabled(false);
        mp_Ui->btnEdit->setEnabled(false);
        mp_Ui->groupBox->setEnabled(false);
        mp_Ui->btnSave->setEnabled(false);
        mp_Ui->btnCancel->setText("Close");
    }
}

/****************************************************************************/
/*!
 *  \brief Displays whether the program is ready to start or not.
 *
 *  \iparam ReadyString = Text to be displayed on the label.
 */
/****************************************************************************/
void CModifyProgramDlg::SetReadyProgram(QString ReadyString)
{
    mp_Ui->label_3->setText(ReadyString);
}

/****************************************************************************/
/*!
 *  \brief This resets the buttons on the right of the screen.
 *
 *  \iparam CurrentProgram = Currently selected program
 *  \iparam SelectionChanged = True if selection is changed else False.
 */
/****************************************************************************/
void CModifyProgramDlg::ResetButtons(DataManager::CProgram &CurrentProgram, bool SelectionChanged)
{
    if (SelectionChanged) {
        if (CurrentProgram.GetNumberOfSteps() > 0 )
        {
            mp_Ui->btnDelete->setEnabled(true);
            mp_Ui->btnCopy->setEnabled(true);
            mp_Ui->btnEdit->setEnabled(true);
            mp_Ui->groupBox->setEnabled(true);
            mp_Ui->btnNew->setEnabled(true);
        }
        else {
            if (CurrentProgram.GetNumberOfSteps()>= MAX_PROGRAM_STEPS) {
                mp_Ui->btnNew->setEnabled(false);
                mp_Ui->btnCopy->setEnabled(false);
            }
            else {
                mp_Ui->btnDelete->setEnabled(false);
                mp_Ui->btnCopy->setEnabled(false);
                mp_Ui->btnEdit->setEnabled(false);
                mp_Ui->groupBox->setEnabled(false);
                mp_Ui->btnNew->setEnabled(true);
            }
        }
    }
    else {
        mp_Ui->btnDelete->setEnabled(false);
        mp_Ui->btnCopy->setEnabled(false);
        mp_Ui->btnEdit->setEnabled(false);
        mp_Ui->groupBox->setEnabled(false);
        mp_Ui->btnNew->setEnabled(true);
        int NumOfSteps = CurrentProgram.GetNumberOfSteps();
        if (NumOfSteps >= MAX_PROGRAM_STEPS) {
            mp_Ui->btnNew->setEnabled(false);
            mp_Ui->btnCopy->setEnabled(false);
        }
    }
}
/********************************************************************************/
/*!
 *  \brief This slot is called when Ok button in ModifyProgramStep dialog is clicked.
 *
 *  \iparam p_PrgmStep = Reference to the program step
 *  \iparam AddNewProgramStep = True if New step is added  else False
 */
/********************************************************************************/
void CModifyProgramDlg::UpdateProgramStepTable(DataManager::CProgramStep *p_PrgmStep, bool AddNewProgramStep)
{
    //Add program Step expects the user to allocate memory for the object. It doesnt expect
    // an object in Stack.
    DataManager::CProgramStep *p_ProgramStep = new DataManager::CProgramStep();
    // Store the pointer in container , so that Destructor can delete them later
    m_ListOfProgramSteps << p_ProgramStep;
    *p_ProgramStep = *p_PrgmStep;
    static bool SetNextFreeStepId = true;

    //Check if new program is being added else update the selected program
    if (m_ButtonType == NEW_BTN_CLICKED) {
        if(SetNextFreeStepId) {
            mp_NewProgram->SetNextFreeStepID("0");
            SetNextFreeStepId = false;
        }
        if (!AddNewProgramStep) {
            (void) mp_NewProgram->UpdateProgramStep(p_ProgramStep);
        }
        else {
            p_ProgramStep->SetStepID(mp_NewProgram->GetNextFreeStepID(true));
            (void) mp_NewProgram->AddProgramStep(p_ProgramStep);
            m_StepModel.SetProgram(mp_NewProgram, mp_DataConnector->ReagentList, 5);
            m_StepModel.SetModifyProgramDlgPtr(this);
            mp_TableWidget->setModel(&m_StepModel);
            ResetButtons(*mp_NewProgram, false);
        }
    }
    else {
        // Check if new Program step is being added to the selected Program else update the program step
        if (AddNewProgramStep) {
            p_ProgramStep->SetStepID(m_Program.GetNextFreeStepID(true));
            (void) m_Program.AddProgramStep(p_ProgramStep);
            m_StepModel.SetProgram(&m_Program, mp_DataConnector->ReagentList, 5);
            m_StepModel.SetModifyProgramDlgPtr(this);
            mp_TableWidget->setModel(&m_StepModel);
            ResetButtons(m_Program, false);
        }
        else {
            (void) m_Program.UpdateProgramStep(p_ProgramStep);
        }
    }
}

/********************************************************************************/
/*!
 *  \brief Verifies last step of the program step list.
 *
 *  \iparam p_Program = Selected/New Program
 *  \iparam DeviceMode = Device is in Standalone/Workstation mode
 *
 *  \return VerificationResult (True if verification is success else False)
 */
/********************************************************************************/
bool CModifyProgramDlg::VerifyLastProgramStep(DataManager::CProgram *p_Program,
                                              QString DeviceMode)
{
    bool VerificationResult;
    if (DeviceMode == "Himalaya") {
        VerificationResult = VerifyLastStepForHimalayaMode(p_Program);
    }
    else {
        VerificationResult = VerifyLastStepForWorkStationMode(p_Program);
    }
    return VerificationResult;
}

/********************************************************************************/
/*!
 *  \brief This slot is called when UpdateProgramColor in RackColorGrip is emitted.
 *
 *  \iparam Program = Selected/New Program
 *  \iparam ColorReplaced = True(Color of existing program replaced)/ False
 */
/********************************************************************************/
void CModifyProgramDlg::OnUpdateProgramColor(DataManager::CProgram &Program,
                                             bool ColorReplaced)
{
    if (ColorReplaced) {
        m_ColorReplacedProgram = Program;
        (void) m_ProgramListClone.UpdateProgram(&m_ColorReplacedProgram);
        m_ColorReplaced = true;
    }
    else {
        m_Program.SetColor(Program.GetColor());
        (void) m_ProgramListClone.UpdateProgram(&m_Program);
        mp_Ui->btnColor->SetColor(Program.GetColor());
    }
}

/********************************************************************************/
/*!
 *  \brief Sets the UserSettings for ModifyStep dialog.
 */
/********************************************************************************/
void CModifyProgramDlg::UpdateUserSettings()
{
    mp_ModifyProgStepDlg->SetUserSettings(mp_DataConnector->SettingsInterface->GetUserSettings());
}

/********************************************************************************/
/*!
 *  \brief Verifies last step of the program step when device is in Himalaya mode.
 *
 *  \iparam p_SelectedProgram = Selected/New Program
 *
 *  \return True if verification is success else False
 */
/********************************************************************************/
bool CModifyProgramDlg::VerifyLastStepForHimalayaMode(DataManager::CProgram* p_SelectedProgram)
{
    DataManager::CProgram *p_Program = p_SelectedProgram;
    int NumberOfUnloaders = 0;
    int LastStepIndex = 0;
    bool Unloader = false;

    MainMenu::CMessageDlg MessageDlg;
    MessageDlg.SetTitle(tr("Information Message"));
    MessageDlg.SetIcon(QMessageBox::Information);
    MessageDlg.SetButtonText(1, tr("Ok"));
    MessageDlg.HideButtons();
    MessageDlg.SetText(tr("Please check:"
                          "\n 1.Unloader station should be the last step."
                          "\n 2.Program cannot have more than one Unloader station"));
    // Iterate over all program steps
    for (int StepCount = 0; StepCount < p_Program->GetNumberOfSteps(); StepCount++) {
        (void) p_Program->GetProgramStep(StepCount, m_LastProgramStep);
        // Check for UNLOADER step
        if (m_LastProgramStep.GetReagentID() == UNLOADER_STEP_ID) {
            Unloader = true;
            NumberOfUnloaders += 1;
            // Check if there are more than one UNLOADER steps
            if (NumberOfUnloaders > 1) {
                if (MessageDlg.exec() == (int)QDialog::Accepted) {
                    return false;
                }
            }
            else LastStepIndex = StepCount;
        }
    }
    // Check if UNLOADER is last step.
    if (LastStepIndex != (p_Program->GetNumberOfSteps() - 1)) {
        // Check if atleast one UNLOADER is present in the program steps.
        if (Unloader) {
            if(MessageDlg.exec() == (int)QDialog::Accepted) {
                return false;
            }
        }
        else {
            if(MessageDlg.exec() == (int)QDialog::Accepted) {
                return false;
            }
        }
    }
    // Check if only UNLOADER step is added.
    else if ((LastStepIndex == 0)) {
        if (!Unloader) {
            if(MessageDlg.exec() == (int)QDialog::Accepted) {
                return false;
            }
        }
    }
    return true;
}

/********************************************************************************/
/*!
 *  \brief Verifies last step of the program step list when device is in WorkStation mode.
 *
 *  \iparam p_SelectedProgram = Selected/New Program
 *
 *  \return True if verification is success else False
 */
/********************************************************************************/
bool CModifyProgramDlg::VerifyLastStepForWorkStationMode(DataManager::CProgram* p_SelectedProgram)
{
    DataManager::CProgram *p_Program = p_SelectedProgram;
    int NumberOfUnloaders = 0;
    int NumberOfTransfer = 0;
    int LastStepIndex = 0;
    bool Unloader = false;
    bool Transfer = false;

    MainMenu::CMessageDlg MessageDlg;
    MessageDlg.SetTitle(tr("Information Message"));
    MessageDlg.SetIcon(QMessageBox::Information);
    MessageDlg.SetButtonText(1, tr("Ok"));
    MessageDlg.HideButtons();
    MessageDlg.SetText(tr("Please check:"
                          "\n 1.Unloader or Transfer station should be the last step."
                          "\n 2.Program cannot have more than one Unloader or Transfer station"));

     // Iterate over all program steps
    for (int StepCount = 0; StepCount < p_Program->GetNumberOfSteps(); StepCount++) {
        (void) p_Program->GetProgramStep(StepCount, m_LastProgramStep);
         // Check for UNLOADER step
        if (m_LastProgramStep.GetReagentID() == UNLOADER_STEP_ID) {
            Unloader = true;
            NumberOfUnloaders+= 1;
            // Check if there are more than one UNLOADER steps
            if (NumberOfUnloaders > 1) {
                if (MessageDlg.exec()== (int)QDialog::Accepted) {
                    return false;
                }
            }
            else LastStepIndex = StepCount;
        }
         // Check for TRANSFER step
        else if (m_LastProgramStep.GetReagentID() == TRANSFER_STEP_ID) {
            Transfer = true;
            NumberOfTransfer+= 1;
            // Check if there are more than one TRANSFER steps
            if (NumberOfTransfer > 1) {
                if(MessageDlg.exec() == (int)QDialog::Accepted) {
                    return false;
                }
            }
            else LastStepIndex = StepCount;
        }
        // Check if both UNLOADER and TRANSFER steps are added into the program steps.
        if (Unloader && Transfer) {
            MessageDlg.SetText(tr("Please check:"
                                  "\n 1.Program cannot have both Unloader and Transfer in program steps."
                                  "\n 2.Please select either Unloader or Transfer."));
            if (MessageDlg.exec() == (int)QDialog::Accepted) {
                return false;
            }
        }
    }
    if (LastStepIndex != (p_Program->GetNumberOfSteps() - 1)) {
         // Check if atleast one UNLOADER step is present in the program steps.
        if (Unloader) {
            if (MessageDlg.exec() == (int)QDialog::Accepted) {
                return false;
            }
        }
         // Check if atleast one TRANSFER step is present in the program steps.
        else if (Transfer) {
            if (MessageDlg.exec() == (int)QDialog::Accepted) {
                return false;
            }
        }
        else {
            if (MessageDlg.exec() == (int)QDialog::Accepted) {
                return false;
            }
        }
    }
    // Check if only UNLOADER or TRANSFER step is added.
    else if ((LastStepIndex == 0)) {
        if (!(Unloader || Transfer)) {
            if (MessageDlg.exec() == (int)QDialog::Accepted) {
                return false;
            }
        }
    }
    return true;
}

/****************************************************************************/
/*!
 *  \brief This slot is called when End button in moving table is clicked.
 */
/****************************************************************************/
void CModifyProgramDlg::OnEndButtonClicked()
{
    mp_Ui->scrollTable->ScrollContent(3);
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Begin button in moving table is clicked.
 */
/****************************************************************************/
void CModifyProgramDlg::OnBeginButtonClicked()
{
    mp_Ui->scrollTable->ScrollContent(2);
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Down button in moving table is clicked.
 */
/****************************************************************************/
void CModifyProgramDlg::OnDownButtonClicked()
{
    if(m_StepModel.GetIndex() >= (qint32)m_Program.GetNumberOfSteps()-2)
    {
        mp_Ui->scrollTable->ScrollContent(3);
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Up button in moving table is clicked.
 */
/****************************************************************************/
void CModifyProgramDlg::OnUpButtonClicked()
{
    if (m_StepModel.GetIndex() <= 1)
    {
        mp_Ui->scrollTable->ScrollContent(2);
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when a Program/ProgramList is updated.
 */
/****************************************************************************/
void CModifyProgramDlg::UpdateProgramList()
{
    m_ProgramListClone = *(mp_DataConnector->ProgramList);
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CModifyProgramDlg::RetranslateUI()
{
    MainMenu::CDialogFrame::SetDialogTitle(QApplication::translate("Programs::CModifyProgramDlg",
                                            "Edit Program Step", 0, QApplication::UnicodeUTF8));
    MainMenu::CDialogFrame::SetDialogTitle(QApplication::translate("Programs::CModifyProgramDlg",
                                            "New Program Step", 0, QApplication::UnicodeUTF8));
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("Programs::CModifyProgramDlg",
                                            "Enter Program Name", 0, QApplication::UnicodeUTF8));
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("Programs::CModifyProgramDlg",
                                            "Enter Program Short Name", 0, QApplication::UnicodeUTF8));

    // Added void to please lint
    (void) m_StepModel.setHeaderData(0,Qt::Horizontal,QApplication::translate("Programs::CStepModel",
                                    "Step", 0, QApplication::UnicodeUTF8),0);
    (void) m_StepModel.setHeaderData(1,Qt::Horizontal,QApplication::translate("Programs::CStepModel",
                                    "Reagent", 0, QApplication::UnicodeUTF8),0);
    (void) m_StepModel.setHeaderData(2,Qt::Horizontal,QApplication::translate("Programs::CStepModel",
                                    "Duration", 0, QApplication::UnicodeUTF8),0);
    (void) m_StepModel.setHeaderData(3,Qt::Horizontal,QApplication::translate("Programs::CStepModel",
                                    "Tol.", 0, QApplication::UnicodeUTF8),0);
    (void) m_StepModel.setHeaderData(4,Qt::Horizontal,QApplication::translate("Programs::CStepModel",
                                    "Excl.", 0, QApplication::UnicodeUTF8),0);
}

} // end namespace Programs
