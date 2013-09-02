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
#include "HimalayaDataContainer/Containers/ReagentGroups/Include/DataReagentGroupList.h"
#include <Dashboard/Include/CommonString.h>

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
#define MIN_FARENHEIT_TEMP    95
//! Maximal scroll wheel temperature in degree Fahrenheit
#define MAX_FARENHEIT_TEMP    167

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
                                            mp_ScrollWheelTemp(NULL), mp_MessageBox(new MainMenu::CMessageDlg(this)),
                                            mp_MainWindow(p_MainWindow), mp_ProgramStep(NULL),
                                            mp_ReagentList(NULL), m_RowSelected(-1),
                                            m_RowNotSelected(true), m_NewProgramStep(false),
                                            m_ProcessRunning(false),
                                            mp_DataConnector(p_DataConnector),
                                            m_strConfirmMsg(tr("Information Message")),
                                            m_strOK(tr("OK"))

{
    mp_Ui->setupUi(GetContentFrame());
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ReagentEditModel);

    mp_TableWidget->horizontalHeader()->show();
    mp_Ui->scrollTable->SetContent(mp_TableWidget);
    mp_TableWidget->SetVisibleRows(8);
    m_ReagentEditModel.SetModifiedProgramStepDlg(true);

    mp_ScrollWheelHour = new MainMenu::CScrollWheel();
    mp_ScrollWheelMin =  new MainMenu::CScrollWheel();
    mp_ScrollWheelTemp = new MainMenu ::CScrollWheel();
    mp_Program = new DataManager::CProgram();

    mp_UserSettings = NULL;
    m_ReagentEditModel.SetVisibleRowCount(8);
    m_ReagentEditModel.SetRequiredContainers(mp_DataConnector->ReagentList,mp_DataConnector->ReagentGroupList,mp_DataConnector->DashboardStationList, 2);
    m_pAmbientTempraturePixmap = new QPixmap(":/HimalayaImages/Digits/Snow.png");
    m_pAmbientTempratureBigPixmap = new QPixmap(":/HimalayaImages/Digits/SnowBig.png");
    InitDurationWidget();

    mp_ScrollWheelTemp->setEnabled(false);
    mp_Ui->scrollPanelWidgetTemperature->Init(1);
    mp_Ui->scrollPanelWidgetTemperature->AddScrollWheel(mp_ScrollWheelTemp, 0);
    //InitTemperatureWidget();

    m_ButtonGroup.addButton(mp_Ui->radioButton_0, 0);
    m_ButtonGroup.addButton(mp_Ui->radioButton_25, 1);
    m_ButtonGroup.addButton(mp_Ui->radioButton_50, 2);
    m_ButtonGroup.addButton(mp_Ui->radioButton_75, 3);

    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnOk, clicked(), this, OnOk());
    CONNECTSIGNALSLOT(mp_TableWidget,clicked(QModelIndex), this, OnSelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(p_Parent, ReagentsUpdated(), this, ReagentTableUpdate());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(),this,UpdateUserSetting());
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
        delete m_pAmbientTempraturePixmap;
        delete m_pAmbientTempratureBigPixmap;
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

void CModifyProgramStepDlg:: UpdateUserSetting()
{
    mp_UserSettings = mp_DataConnector->SettingsInterface->GetUserSettings();
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
void CModifyProgramStepDlg::InitTemperatureWidget(const DataManager::CReagent * pReagent)
{
    mp_ScrollWheelTemp->ClearItems();

    // set temperature symbol
    if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS) {
        mp_Ui->scrollPanelWidgetTemperature->SetSubtitle(QApplication::translate("CModifyProgramStepDlg", "\302\260C", 0, QApplication::UnicodeUTF8), 0);
    }
    else {
        mp_Ui->scrollPanelWidgetTemperature->SetSubtitle(QApplication::translate("CModifyProgramStepDlg", "\302\260F", 0, QApplication::UnicodeUTF8), 0);
    }

    if (!pReagent)
        return;

    //set temprature range
    QString groupID = pReagent->GetGroupID();

    const DataManager::CReagentGroup* reagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(groupID);
    if (!reagentGroup)
    {
        Q_ASSERT(0);
    }

    int minTemp = reagentGroup->GetMinTemprature();
    int maxTemp = reagentGroup->GetMaxTemprature();


    //Add the AmbientTemprature icon.
    if (!reagentGroup->IsParraffin())
    {
        if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_FAHRENHEIT)
            mp_ScrollWheelTemp->AddItem("", -1, *m_pAmbientTempratureBigPixmap);
        else
            mp_ScrollWheelTemp->AddItem("", -1, *m_pAmbientTempraturePixmap);
    }

    if (0 != minTemp && 0 != maxTemp)
    {
        for (int i = minTemp; i <= maxTemp; i += 1) {
            int temp = i;
            if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_FAHRENHEIT) {
                temp = qRound(((double)i * 9) / 5  + 32);
            }

            mp_ScrollWheelTemp->AddItem(QString::number(temp).rightJustified(2, '0'), temp);
        }
    }

    if (mp_ProgramStep)
    {
        if (mp_ProgramStep->GetTemperature() == "-1")//Ambient temprature
        {
            mp_ScrollWheelTemp->SetCurrentData(-1);
        }
        else
        if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_FAHRENHEIT) {
            double temperature = mp_ProgramStep->GetTemperature().toDouble() * 9 / 5  + 32;
            mp_ScrollWheelTemp->SetCurrentData((qRound(temperature)));
        }
        else {
            mp_ScrollWheelTemp->SetCurrentData(mp_ProgramStep->GetTemperature());
        }
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
void CModifyProgramStepDlg::SetProgramStep(DataManager::CProgramStep *p_ProgramStep)
{
    m_ReagentEditModel.UpdateReagentList();
    QTime Duration;
    mp_ProgramStep = p_ProgramStep;

    mp_ReagentList = mp_DataConnector->ReagentList;

    Duration = Duration.addSecs(mp_ProgramStep->GetDurationInSeconds());
    mp_ScrollWheelHour->SetCurrentData(Duration.hour());
    mp_ScrollWheelMin->SetCurrentData(Duration.minute());

    const DataManager::CReagent *pReagent = mp_ReagentList->GetReagent(mp_ProgramStep->GetReagentID());

    m_ReagentID = pReagent->GetReagentID();
    QString ID = mp_ProgramStep->GetReagentID();
    int Index = m_ReagentEditModel.GetReagentPositionOfReagent(ID);
    mp_TableWidget->selectRow(Index);

    ResizeHorizontalSection();
    m_NewProgramStep = false;
    InitTemperatureWidget(pReagent);
}

/****************************************************************************/
/*!
 *  \brief Sets the data displayed in the New program step dialog
 *
 *  \iparam p_ReagentList = Reagent list for the table
 *  \iparam p_StationList = Station list for the table
 */
/**************************************************************************/////////////****/
void CModifyProgramStepDlg::NewProgramStep()
{
    m_ReagentEditModel.UpdateReagentList();
    QTime Duration;
    mp_ProgramStep = NULL;
    mp_ReagentList = mp_DataConnector->ReagentList;

    mp_Ui->radioButton_50->setChecked(true);

    Duration = Duration.addSecs(0);
    mp_ScrollWheelHour->SetCurrentData(Duration.hour());
    mp_ScrollWheelMin->SetCurrentData(Duration.minute());
    ResizeHorizontalSection();

    m_NewProgramStep = true;
    InitTemperatureWidget(NULL);
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
    const DataManager::CReagent *pReagent = mp_ReagentList->GetReagent(m_ReagentID);
    const DataManager::CReagentGroup* reagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(pReagent->GetGroupID());
    if (reagentGroup ->IsParraffin())
    {
          DataManager::CHimalayaUserSettings *p_Settings = mp_DataConnector->SettingsInterface->GetUserSettings();
          int paraffinBathTemp = p_Settings->GetTemperatureParaffinBath();
          double diffTemp = 0;
          if( mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS)
              diffTemp = 2.0;
          else
          {
              diffTemp = 3.6;
              paraffinBathTemp  = qRound((paraffinBathTemp * 9.0) / 5.0) + 32;
          }

         int ddd = mp_ScrollWheelTemp->GetCurrentData().toInt();
         double diffSetting = qAbs(ddd - paraffinBathTemp);

         if (diffSetting > diffTemp)
         {
            mp_MessageBox->SetIcon(QMessageBox::Warning);
            mp_MessageBox->SetTitle(CommonString::strInforMsg);
            mp_MessageBox->SetText(m_strDiffTemp);
            mp_MessageBox->SetButtonText(1, CommonString::strYes);
            mp_MessageBox->SetButtonText(3, CommonString::strNo);
            mp_MessageBox->HideCenterButton();
            if (!mp_MessageBox->exec())
                return;
         }
    }

    int MinDurationInSec;
    QString Pressure;
    QString Vaccum;
    QString Temperature;
    MinDurationInSec = mp_ScrollWheelHour->GetCurrentData().toInt()*60*60;
    MinDurationInSec+= mp_ScrollWheelMin->GetCurrentData().toInt()*60;

    if( mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS)
    {
        Temperature = mp_ScrollWheelTemp->GetCurrentData().toString();
    }
    else
    {
        if (mp_ScrollWheelTemp->GetCurrentData().toInt() == -1)
        {
            Temperature = "-1";
        }
        else
        {
            double Temp = qRound(((mp_ScrollWheelTemp->GetCurrentData().toDouble() - 32) * 5) / 9);
            Temperature = QString::number(qRound(Temp));
        }
    }
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
    DataManager::CProgramStep ProgramStep;
    if (m_ModifyProgramDlgButtonType == COPY_BTN_CLICKED) {
        m_NewProgramStep = true ;
        ProgramStep.SetDurationInSeconds(MinDurationInSec);
        ProgramStep.SetReagentID(m_ReagentID);
        ProgramStep.SetTemperature(Temperature);
        ProgramStep.SetPressure(Pressure);
        ProgramStep.SetVacuum(Vaccum);
        emit AddProgramStep(&ProgramStep, m_NewProgramStep);
        accept();
    }
    else {
        if (m_ModifyProgramDlgButtonType == NEW_BTN_CLICKED) {
            m_NewProgramStep = true ;
            ProgramStep.SetDurationInSeconds(MinDurationInSec);
            ProgramStep.SetReagentID(m_ReagentID);
            ProgramStep.SetTemperature(Temperature);
            ProgramStep.SetPressure(Pressure);
            ProgramStep.SetVacuum(Vaccum);
            emit AddProgramStep(&ProgramStep, m_NewProgramStep);
        }
        else {
            m_NewProgramStep = false ;
            if (mp_ProgramStep)
            {
                ProgramStep = *mp_ProgramStep;
                ProgramStep.SetDurationInSeconds(MinDurationInSec);
                ProgramStep.SetReagentID(m_ReagentID);
                ProgramStep.SetTemperature(Temperature);
                ProgramStep.SetPressure(Pressure);
                ProgramStep.SetVacuum(Vaccum);
                emit AddProgramStep(&ProgramStep, m_NewProgramStep);
            }
        }
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
        m_ReagentID = m_ReagentEditModel.GetReagentID(Index.row());
        m_ReagentLongName = m_ReagentEditModel.GetReagentLongName(m_ReagentID);
        if (!m_ReagentLongName.isEmpty()) {
            m_RowNotSelected = false;
            mp_Ui->btnOk->setEnabled(true);
            if (mp_ReagentList)
            {
                const DataManager::CReagent *pReagent = mp_ReagentList->GetReagent(m_ReagentID);
                InitTemperatureWidget(pReagent);
            }
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

    if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS) {
        mp_ScrollWheelTemp->SetThreeDigitMode(false);
        mp_Ui->scrollPanelWidgetTemperature->SetThreeDigitMode(false);
        //mp_ScrollWheelTemp->SetCurrentData(QString::number(m_ProgramStep.GetTemperature().toInt()));
        qDebug()<<"\n\n Program widget Temp" << m_ProgramStep.GetTemperature().toInt();
    }
    else {
        mp_ScrollWheelTemp->SetThreeDigitMode(true);
        mp_Ui->scrollPanelWidgetTemperature->SetThreeDigitMode(true);
        //mp_ScrollWheelTemp->SetCurrentData(tr("%1").arg((((m_ProgramStep.GetTemperature().toInt() - 32) / 5) * 9) + 104));
        qDebug()<<"\n\n Program widget Temp" << m_ProgramStep.GetTemperature().toInt();
    }

    if (mp_ProgramStep && mp_ReagentList) {
        DataManager::CReagent const *p_Reagent = mp_ReagentList->GetReagent(mp_ProgramStep->GetReagentID());
        if (p_Reagent) {

            if (!m_ProcessRunning) {
                mp_Ui->btnOk->setEnabled(true);
            }
            else {
                mp_Ui->btnOk->setEnabled(false);
            }

            DataManager::CReagent Reagent;
            if (mp_ReagentList->GetReagent(mp_ProgramStep->GetReagentID(), Reagent) == true) {

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
    mp_Ui->scrollPanelWidgetTime->SetTitle(QApplication::translate("Programs::CModifyProgramStepDlg",
                                            "Time", 0, QApplication::UnicodeUTF8));
    mp_Ui->scrollPanelWidgetTime->SetSubtitle(QApplication::translate("Programs::CModifyProgramStepDlg",
                                             "Hour", 0, QApplication::UnicodeUTF8), 0);
    mp_Ui->scrollPanelWidgetTime->SetSubtitle(QApplication::translate("Programs::CModifyProgramStepDlg",
                                             "Minute", 0, QApplication::UnicodeUTF8), 1);

    m_strConfirmMsg = QApplication::translate("Programs::CModifyProgramStepDlg",
                                                        "Information Message", 0, QApplication::UnicodeUTF8);
    m_strOK = QApplication::translate("Programs::CModifyProgramStepDlg",
                                      "OK", 0, QApplication::UnicodeUTF8);

    m_strDiffTemp = QApplication::translate("Programs::CModifyProgramStepDlg",
                                      "The set temperature of paraffin baths differs from the temperature of the program by more than 2℃.Would you like to continue?",
                                            0, QApplication::UnicodeUTF8);

}

} // end namespace Programs
