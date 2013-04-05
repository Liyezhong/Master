/****************************************************************************/
/*! \file ModifyProgramStepDlg.cpp
 *
 *  \brief Implementation of file for class CModifyProgramStepDlg.
 *
 *   \b Description:
 *          This class provides the widget to Edit, Add, Copy the program steps
 *          parameters like Step duration, Exclusiveness and Tolerance for the
 *          selected Program.
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
#include "Programs/Include/ModifyProgramStepDlg.h"
#include "ui_ModifyProgramStepDlg.h"
#include <QDebug>
#include <QTime>

namespace Programs {

const QString TRANSFER_STEP_ID = "S8";           //!< Transfer station id
const QString OVEN_STEP_ID = "S4";               //!< Oven station id
const QString DISTILLED_WATER_STEP_ID = "S6";    //!< DI water id
const QString TAP_WATER_STEP_ID = "S5";          //!< TAP water id
const QString UNLOADER_STEP_ID = "S7";           //!< Unloader id
//! Minimal scroll wheel temperature in degree Celsius
#define MIN_CENTIGRADE_TEMP   35
//! Maximal scroll wheel temperature in degree Celsius
#define MAX_CENTIGRADE_TEMP   75
//! Minimal scroll wheel temperature in degree Fahrenheit
#define MIN_FARENHEIT_TEMP    104
//! Maximal scroll wheel temperature in degree Fahrenheit
#define MAX_FARENHEIT_TEMP    158

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 *  \iparam p_MainWindow = MainWindow widget
 */
/****************************************************************************/
CModifyProgramStepDlg::CModifyProgramStepDlg(QWidget *p_Parent, MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector) :
                                            MainMenu::CDialogFrame(p_Parent),
                                            mp_Ui(new Ui::CModifyProgramStepDlg), mp_TableWidget(NULL),
                                            mp_ScrollWheelHour(NULL), mp_ScrollWheelMin(NULL),
                                            mp_ScrollWheelTemp(NULL), mp_MessageBox(new MainMenu::CMessageDlg),
                                            mp_MainWindow(p_MainWindow), mp_ProgramStep(NULL),
                                            mp_ReagentList(NULL), m_RowSelected(-1),
                                            m_RowNotSelected(true), m_NewProgramStep(false),
                                            m_ProcessRunning(false), m_ReagentExists(true),
                                            m_DeviceMode(""),mp_DataConnector(p_DataConnector)
{
    mp_Ui->setupUi(GetContentFrame());
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ReagentEditModel);

    mp_TableWidget->horizontalHeader()->show();
    mp_Ui->scrollTable->SetContent(mp_TableWidget);
    mp_TableWidget->SetVisibleRows(8);

    mp_ProgramStep =     new DataManager::CProgramStep;
    mp_ScrollWheelHour = new MainMenu::CScrollWheel();
    mp_ScrollWheelMin =  new MainMenu::CScrollWheel();
    mp_ScrollWheelTemp = new MainMenu ::CScrollWheel();
    mp_Program = new DataManager::CProgram();

    m_ReagentEditModel.SetVisibleRowCount(8);
    m_ReagentEditModel.SetRequiredContainers(mp_DataConnector->ReagentList,mp_DataConnector->ReagentGroupList,mp_DataConnector->DashboardStationList, 2);

    InitDurationWidget();
    InitTemperatureWidget();

    m_ButtonGroup.addButton(mp_Ui->radioButton_0, 0);
    m_ButtonGroup.addButton(mp_Ui->radioButton_25, 1);
    m_ButtonGroup.addButton(mp_Ui->radioButton_50, 2);
    m_ButtonGroup.addButton(mp_Ui->radioButton_75, 3);

    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnOk, clicked(), this, OnOk());
    CONNECTSIGNALSLOT(mp_TableWidget,clicked(QModelIndex), this, OnSelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(p_Parent, ReagentsUpdated(), this, ReagentTableUpdate());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    CONNECTSIGNALSLOT(mp_TableWidget,clicked(QModelIndex), this, OnSelectionChanged(QModelIndex));

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CModifyProgramStepDlg::~CModifyProgramStepDlg()
{
    try {
        delete mp_ScrollWheelHour;
        delete mp_ScrollWheelMin;
        delete mp_ScrollWheelTemp;
        delete mp_TableWidget;
        delete mp_Program;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint.
    }
}

/****************************************************************************/
/*!
 *  \brief Resizes the columns in the program step table
 */
/****************************************************************************/
void CModifyProgramStepDlg::ResizeHorizontalSection()
{
    mp_TableWidget->horizontalHeader()->resizeSection(0, 134);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 130);
}

/****************************************************************************/
/*!
 *  \brief Initializes the duration scroll wheels
 */
/****************************************************************************/
void CModifyProgramStepDlg::InitDurationWidget()
{
    for (int HourItemCnt = 0; HourItemCnt <= 99; HourItemCnt++) {
        mp_ScrollWheelHour->AddItem(QString("%1").arg(HourItemCnt, 2, 10, QChar('0')), HourItemCnt);
    }

    for (int MinItemCnt = 0; MinItemCnt <= 59; MinItemCnt++) {
        mp_ScrollWheelMin->AddItem(QString("%1").arg(MinItemCnt, 2, 10, QChar('0')), MinItemCnt);
    }

    mp_Ui->scrollPanelWidgetTime->Init(2);
    mp_Ui->scrollPanelWidgetTime->SetTitle(tr("Time"));
    mp_Ui->scrollPanelWidgetTime->AddScrollWheel(mp_ScrollWheelHour, 0);
    mp_Ui->scrollPanelWidgetTime->SetSubtitle(tr("Hour"), 0);
    mp_Ui->scrollPanelWidgetTime->AddSeparator(MainMenu::CWheelPanel::COLON, 0);
    mp_Ui->scrollPanelWidgetTime->AddScrollWheel(mp_ScrollWheelMin, 1);
    mp_Ui->scrollPanelWidgetTime->SetSubtitle(tr("Minute"), 1);

}
/****************************************************************************/
/*!
 *  \brief Initializes the temperature scroll wheel
 */
/****************************************************************************/
void CModifyProgramStepDlg::InitTemperatureWidget()
{
    mp_Ui->scrollPanelWidgetTemperature->Init(1);
    mp_Ui->scrollPanelWidgetTemperature->AddScrollWheel(mp_ScrollWheelTemp, 0);

    mp_ScrollWheelTemp->ClearItems();
    // Temperature Control
    if (m_UserSettings.GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS) {
        for (int i = MIN_CENTIGRADE_TEMP; i <= MAX_CENTIGRADE_TEMP; i += 5) {
            mp_ScrollWheelTemp->AddItem(QString::number(i).rightJustified(2, '0'), i);
        }
        mp_Ui->scrollPanelWidgetTemperature->SetSubtitle(QApplication::translate("CModifyProgramStepDlg", "\302\260C", 0, QApplication::UnicodeUTF8), 0);
    }
    else {
        for (int i = MIN_FARENHEIT_TEMP; i <= MAX_FARENHEIT_TEMP; i += 9) {
            mp_ScrollWheelTemp->AddItem(QString::number(i).rightJustified(2, '0'), i);
        }
        mp_Ui->scrollPanelWidgetTemperature->SetSubtitle(QApplication::translate("CModifyProgramStepDlg", "\302\260F", 0, QApplication::UnicodeUTF8), 0);
    }

    mp_ScrollWheelTemp->SetNonContinuous();
}
/****************************************************************************/
/*!
 *  \brief Sets the data displayed in the dialog
 *
 *  \iparam p_ProgramStep = Program step to be displayed
 *  \iparam p_ReagentList = Reagent list for the table
 *  \iparam p_StationList = Station list for the table
 */
/****************************************************************************/
void CModifyProgramStepDlg::SetProgramStep(DataManager::CProgramStep *p_ProgramStep, DataManager::CDataReagentList *p_ReagentList)
{
    m_ReagentEditModel.UpdateReagentList();
    QTime Duration;
    mp_ProgramStep = p_ProgramStep;
    mp_ReagentList = p_ReagentList;

    Duration = Duration.addSecs(mp_ProgramStep->GetDurationInSeconds());
    mp_ScrollWheelHour->SetCurrentData(Duration.hour());
    mp_ScrollWheelMin->SetCurrentData(Duration.minute());

    if (m_UserSettingsTemp.GetTemperatureFormat() == Global::TEMP_FORMAT_FAHRENHEIT) {
        qint32 TemperatureCelsius = ((mp_ScrollWheelTemp->GetCurrentData().toInt() - 32) * 5) / 9;
        m_UserSettingsTemp.SetValue("Temp", TemperatureCelsius);
    }
    else {
        m_UserSettingsTemp.SetValue("Temp", mp_ScrollWheelTemp->GetCurrentData().toInt());
    }
    m_ReagentModel.FilterLeicaReagents(true);
    m_ReagentModel.SetUserSettings(&m_UserSettings);
    m_ReagentModel.OnDeviceModeChanged(m_DeviceMode);
    m_ReagentModel.SetParentPtr(this);

    const DataManager::CReagent *Reagent = mp_ReagentList->GetReagent(mp_ProgramStep->GetReagentID());
    QString ID = mp_ProgramStep->GetReagentID();
    int Index = m_ReagentEditModel.GetReagentPositionOfReagent(ID);
    mp_TableWidget->selectRow(Index);
    ResizeHorizontalSection();
    m_NewProgramStep = false;
}

/****************************************************************************/
/*!
 *  \brief Sets the data displayed in the New program step dialog
 *
 *  \iparam p_ReagentList = Reagent list for the table
 *  \iparam p_StationList = Station list for the table
 */
/****************************************************************************/
void CModifyProgramStepDlg::NewProgramStep(DataManager::CDataReagentList *p_ReagentList)
{

    m_ReagentEditModel.UpdateReagentList();
    QTime Duration;
    mp_ProgramStep = NULL;
    mp_ReagentList = p_ReagentList;

    mp_Ui->radioButton_50->setChecked(true);

    Duration = Duration.addSecs(0);
    mp_ScrollWheelHour->SetCurrentData(Duration.hour());
    mp_ScrollWheelMin->SetCurrentData(Duration.minute());

    m_ReagentModel.FilterLeicaReagents(true);
    m_ReagentModel.OnDeviceModeChanged(m_DeviceMode);
    m_ReagentModel.SetUserSettings(&m_UserSettings);
    m_ReagentModel.SetReagentList(mp_ReagentList, 2);
    m_ReagentModel.SetParentPtr(this);
    ResizeHorizontalSection();

    m_NewProgramStep = true;
    m_ReagentExists = true;
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyProgramStepDlg::changeEvent(QEvent *p_Event)
{
    QDialog::changeEvent(p_Event);
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
 *  \brief Selects the Row in the table
 *
 *  \iparam Row = Row to be selected/focused
 */
/****************************************************************************/
void CModifyProgramStepDlg::SelectRow(qint32 Row)
{
    if (Row >= 0 && (!m_RowNotSelected)) {
        mp_TableWidget->selectRow(Row);
    }
    else {
        m_RowSelected = -1;
    }
}

/****************************************************************************/
/*!
 *  \brief Saves the changes made in the dialog for temporary.
 */
/****************************************************************************/
void CModifyProgramStepDlg::OnOk()
{
    int MinDurationInSec;
    QString Temperature;
    QString Pressure;
    QString Vaccum;
    MainMenu::CMessageDlg MessageDlg(this);
    MessageDlg.SetTitle(tr("Information Message"));
    MessageDlg.SetIcon(QMessageBox::Information);
    MessageDlg.SetButtonText(1, tr("Ok"));
    MessageDlg.HideButtons();

    if(m_ReagentExists) {
        MinDurationInSec = mp_ScrollWheelHour->GetCurrentData().toInt()*60*60;
        MinDurationInSec+= mp_ScrollWheelMin->GetCurrentData().toInt()*60;
        Temperature = mp_ScrollWheelTemp->GetCurrentData().toString();


        if (mp_Ui->radioButton_0->isChecked()) {
            Pressure = "On";
            Vaccum = "Off";
        }
        else if(mp_Ui->radioButton_25->isChecked()) {
            Vaccum = "On";
            Pressure = "Off";
        }
        else if(mp_Ui->radioButton_50->isChecked()) {
            Vaccum = "On";
            Pressure = "On";
        }
        else if(mp_Ui->radioButton_75->isChecked()) {
            Vaccum = "Off";
            Pressure = "Off";
        }

        m_ReagentEditModel.UpdateReagentList();
        QString ReagentId = m_ReagentEditModel.GetReagentID(m_ReagentLongName);
        DataManager::CProgramStep ProgramStep;
        if (m_ModifyProgramDlgButtonType == COPY_BTN_CLICKED) {

            if (ReagentId == m_SelectedStepReagentID) {
                (void) mp_MessageBox->exec();
            }
            else {
                m_NewProgramStep = true ;
                ProgramStep.SetDurationInSeconds(MinDurationInSec);
                ProgramStep.SetReagentID(ReagentId);
                ProgramStep.SetTemperature(Temperature);
                ProgramStep.SetPressure(Pressure);
                ProgramStep.SetVacuum(Vaccum);
                emit AddProgramStep(&ProgramStep, m_NewProgramStep);
                accept();
            }
        }
        else {
            if (m_ModifyProgramDlgButtonType == NEW_BTN_CLICKED) {
                m_NewProgramStep = true ;

                ProgramStep.SetDurationInSeconds(MinDurationInSec);
                ProgramStep.SetReagentID(ReagentId);
                ProgramStep.SetTemperature(Temperature);
                ProgramStep.SetPressure(Pressure);
                ProgramStep.SetVacuum(Vaccum);

                emit AddProgramStep(&ProgramStep, m_NewProgramStep);
            }
            else {
                m_NewProgramStep = false ;
                ProgramStep = *mp_ProgramStep;
                ProgramStep.SetDurationInSeconds(MinDurationInSec);
                ProgramStep.SetReagentID(ReagentId);
                ProgramStep.SetTemperature(Temperature);
                ProgramStep.SetPressure(Pressure);
                ProgramStep.SetVacuum(Vaccum);
                emit AddProgramStep(&ProgramStep, m_NewProgramStep);
            }
            accept();
        }
    }
    else {
        ResetButtons(true);
        accept();
    }
}

/****************************************************************************/
/*!
 *  \brief Cancels the changes made in the dialog
 */
/****************************************************************************/
void CModifyProgramStepDlg::OnCancel()
{
    if (!m_ReagentExists) {
        ResetButtons(true);
    }
    reject();
}

/****************************************************************************/
/*!
 *  \brief Updates the reagent list table.
 */
/****************************************************************************/
void CModifyProgramStepDlg::ReagentTableUpdate()
{
    if (mp_ReagentList) {
        m_ReagentModel.UpdateReagentList();
    }
}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Currently selected cell
 */
/****************************************************************************/
void CModifyProgramStepDlg::OnSelectionChanged(QModelIndex Index)
{
    if (Index.isValid() && (!m_ProcessRunning)) {
        m_ReagentLongName = m_ReagentEditModel.GetReagentLongName(Index.row());
        m_ReagentID= m_ReagentEditModel.GetReagentID(m_ReagentLongName);
        if (!m_ReagentLongName.isEmpty()) {
            m_ReagentModel.SetCurrentReagent(m_ReagentLongName);
            m_RowNotSelected = false;
            mp_Ui->btnOk->setEnabled(true);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process Running state changes
 */
/****************************************************************************/
void CModifyProgramStepDlg::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (!m_ProcessRunning) {
        //Edit Mode
        if (!m_RowNotSelected) {
            mp_Ui->btnOk->setEnabled(true);
        }
        else {
            mp_Ui->btnOk->setEnabled(false);
        }
    }
    else {
        //View Mode
        mp_Ui->btnOk->setEnabled(false);
    }
}
/****************************************************************************/
/*!
 *  \brief This event is called whenever widget is shown.
 *
 *  \iparam p_Event = Show event reference.
 */
/****************************************************************************/
void CModifyProgramStepDlg::showEvent(QShowEvent *p_Event)
{
    Q_UNUSED(p_Event);
    m_ReagentModel.OnDeviceModeChanged(m_DeviceMode);
    if (mp_ProgramStep && mp_ReagentList) {
        DataManager::CReagent const *p_Reagent = mp_ReagentList->GetReagent(mp_ProgramStep->GetReagentID());
        if (p_Reagent && m_ReagentModel.rowCount(QModelIndex()) > 0) {

            if (!m_ProcessRunning) {
                mp_Ui->btnOk->setEnabled(true);
            }
            else {
                mp_Ui->btnOk->setEnabled(false);
            }

            if (m_UserSettings.GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS) {
                mp_ScrollWheelTemp->SetThreeDigitMode(false);
                mp_Ui->scrollPanelWidgetTemperature->SetThreeDigitMode(false);
                mp_ScrollWheelTemp->SetCurrentData(QString::number(m_UserSettings.GetValue("Temp").toInt()));
                 qDebug()<<"\n\n Program widget Temp" << m_UserSettings.GetValue("Temp").toInt();
            }
            else {
                mp_ScrollWheelTemp->SetThreeDigitMode(true);
                mp_Ui->scrollPanelWidgetTemperature->SetThreeDigitMode(true);
                mp_ScrollWheelTemp->SetCurrentData(tr("%1").arg((((m_UserSettings.GetValue("Temp").toInt() - 40) / 5) * 9) + 104));
                qDebug()<<"\n\n Program widget Temp" << m_UserSettings.GetValue("Temp").toInt();
            }
            DataManager::CReagent Reagent;
            if ((mp_ReagentList->GetReagent(mp_ProgramStep->GetReagentID(), Reagent) == true)
                    && (m_ReagentModel.rowCount(QModelIndex()) > 0)) {

                    mp_Ui->scrollPanelWidgetTime->SetDisabled(false);
                    mp_Ui->scrollPanelWidgetTemperature->SetDisabled(false);
                    mp_Ui->radioButton_0->setEnabled(true);
                    mp_Ui->radioButton_25->setEnabled(true);
                    mp_Ui->radioButton_50->setEnabled(true);
                    mp_Ui->radioButton_75->setEnabled(true);
            }
        }
    }
    else {
        //Disable Ok btn if  a reagent is not selected on the table
        mp_Ui->btnOk->setEnabled(false);
        mp_TableWidget->clearSelection();
        mp_Ui->scrollTable->Reset();
        mp_Ui->scrollPanelWidgetTime->SetDisabled(false);
        mp_Ui->scrollPanelWidgetTemperature->SetDisabled(false);
        mp_Ui->radioButton_0->setEnabled(true);
        mp_Ui->radioButton_25->setEnabled(true);
        mp_Ui->radioButton_50->setEnabled(true);
        mp_Ui->radioButton_75->setEnabled(true);
        mp_Ui->radioButton_50->setChecked(true);
    }
}

/****************************************************************************/
/*!
 *  \brief Pops up a Message Dialog asking user to select a different regeant.
 */
/****************************************************************************/
void CModifyProgramStepDlg::ShowSelectReagentPopup()
{
    mp_MessageBox->SetIcon(QMessageBox::Information);
    mp_MessageBox->SetButtonText(1, tr("Ok"));
    mp_MessageBox->HideButtons();
    if (!mp_ProgramStep) {
        return;
    }
    if (m_ModifyProgramDlgButtonType == COPY_BTN_CLICKED) {
        if (m_ReagentModel.ContainsReagent(mp_ProgramStep->GetReagentID())) {
            QString ReagentLongName = m_ReagentModel.GetReagentLongName(mp_TableWidget->currentIndex().row());
            m_SelectedStepReagentID = m_ReagentModel.GetReagentID(ReagentLongName);
            mp_MessageBox->SetText(tr("Please select a reagent other than \"%1\""
                                      "\nand which is not used in the selected program").arg(ReagentLongName));
            if (mp_MessageBox->exec()==(int)QDialog::Accepted) {
                return;
            }
        }
        else {
            ResetButtons(false);
            m_ReagentExists = false;
            mp_MessageBox->SetText(tr("Action failed!""\nThe selected program step reagent does not exist."));
            if (mp_MessageBox->exec()==(int)QDialog::Accepted) {
                return;
            }
        }
    }
    else if (m_ModifyProgramDlgButtonType == EDIT_BTN_CLICKED) {
        if (!m_ReagentModel.ContainsReagent(mp_ProgramStep->GetReagentID())) {
            ResetButtons(false);
            mp_MessageBox->SetText(tr("Action failed!""\nThe selected program step reagent does not exist."));
            m_ReagentExists = false;
            if (mp_MessageBox->exec()==(int)QDialog::Accepted) {
                return;
            }
        }
    }
    else {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief This resets the radio buttons,scroll table and wheel panels of the
 *         screen.
 *
 *  \iparam Disable = True for buttons disable else False
 */
/****************************************************************************/
void CModifyProgramStepDlg::ResetButtons(bool Disable)
{
    mp_Ui->groupBox->setEnabled(Disable);
    mp_Ui->scrollTable->setEnabled(Disable);
    mp_Ui->scrollPanelWidgetTime->SetDisabled(!Disable);
    mp_Ui->scrollPanelWidgetTemperature->SetDisabled(!Disable);

}
/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CModifyProgramStepDlg::RetranslateUI()
{
    MainMenu::CDialogFrame::SetDialogTitle(QApplication::translate("Programs::CModifyProgramStepDlg",
                                            "Edit Program Step", 0, QApplication::UnicodeUTF8));
    mp_MessageBox->SetTitle(QApplication::translate("Programs::CModifyProgramStepDlg",
                                            "Information Message", 0, QApplication::UnicodeUTF8));
    mp_MessageBox->SetText(QApplication::translate("Programs::CModifyProgramStepDlg",
                            "Please select a reagent from the list", 0, QApplication::UnicodeUTF8));
    mp_Ui->scrollPanelWidgetTime->SetTitle(QApplication::translate("Programs::CModifyProgramStepDlg",
                                            "Duration", 0, QApplication::UnicodeUTF8));
    mp_Ui->scrollPanelWidgetTemperature->SetTitle(QApplication::translate("Programs::CModifyProgramStepDlg",
                                            "Temp", 0, QApplication::UnicodeUTF8));
    // Added void to please lint
    (void) m_ReagentModel.setHeaderData(0,Qt::Horizontal,QApplication::translate("Core::CReagentRMSModel",
                                        "Program Name", 0, QApplication::UnicodeUTF8),0);
    (void) m_ReagentModel.setHeaderData(1,Qt::Horizontal,QApplication::translate("Core::CReagentRMSModel",
                                        "Nr.", 0, QApplication::UnicodeUTF8),0);
    (void) m_ReagentModel.setHeaderData(2,Qt::Horizontal,QApplication::translate("Core::CReagentRMSModel",
                                        "Color", 0, QApplication::UnicodeUTF8),0);
    (void) m_ReagentModel.setHeaderData(3,Qt::Horizontal,QApplication::translate("Core::CReagentRMSModel",
                                        "Abbr.", 0, QApplication::UnicodeUTF8),0);
    (void) m_ReagentModel.setHeaderData(4,Qt::Horizontal,QApplication::translate("Core::CReagentRMSModel",
                                        "Leica", 0, QApplication::UnicodeUTF8),0);
}

} // end namespace Programs
