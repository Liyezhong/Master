/****************************************************************************/
/*! \file SystemSetupSettingsWidget.cpp
 *
 *  \brief AgitationSettingsWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer
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

#include <QMap>

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Settings/Include/SystemSetupSettingsWidget.h"
#include "ui_SystemSetupSettingsWidget.h"
#include "Core/Include/GlobalHelper.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Dashboard/Include/CommonString.h"

namespace Settings {


//! Minimal scroll wheel temperature in degree Celsius
#define MIN_CENTIGRADE_TEMP   50
//! Maximal scroll wheel temperature in degree Celsius
#define MAX_CENTIGRADE_TEMP   70
//! Minimal scroll wheel temperature in degree Fahrenheit
#define MIN_FARENHEIT_TEMP    122
//! Maximal scroll wheel temperature in degree Fahrenheit
#define MAX_FARENHEIT_TEMP    158

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function GetTemperatureMap
 *
 *  \return from GetTemperatureMap
 */
/****************************************************************************/
QMap<int, int> GetTemperatureMap(int MinC, int MaxC, int StepC)
{
    QMap<int, int> MapTemp;
    for (int Cel = MinC; Cel <= MaxC; Cel += StepC)
    {
        int Faren =
                qRound(((static_cast<double>(Cel)  - 50.0) / 5.0) * 9.0) + 122;

        (void)MapTemp.insert(Faren, Cel);
    }

    return MapTemp;
}

static QMap<int, int> s_MapTemperature =       ///<  Definition/Declaration of variable s_MapTemperature
        GetTemperatureMap(MIN_CENTIGRADE_TEMP, MAX_CENTIGRADE_TEMP, 1);


/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CSystemSetupSettingsWidget::CSystemSetupSettingsWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CSystemSetupSettingsWidget), mp_UserSettings(NULL), mp_MainWindow(NULL),m_ProcessRunning(false),
    m_CurrentUserRole(MainMenu::CMainWindow::Operator)
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("Oven"));

    mp_ScrollWheel = new MainMenu::CScrollWheel;

    mp_Ui->scrollPanelWidget->Init(1);
    mp_Ui->scrollPanelWidget->AddScrollWheel(mp_ScrollWheel, 0);
    CONNECTSIGNALSLOT(mp_Ui->btnSave, clicked(), this, OnApply());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSystemSetupSettingsWidget::~CSystemSetupSettingsWidget()
{
    try {
        delete mp_ScrollWheel;
        delete mp_Ui;
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

void CSystemSetupSettingsWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
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
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::SetUserSettings(DataManager::CHimalayaUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;
}

/****************************************************************************/
/*!
 *  \brief Initializes the temperature scroll wheel
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::InitTemperatureWidget()
{
    if (mp_UserSettings == NULL) {
        return;
    }

    mp_ScrollWheel->ClearItems();

    // Temperature Control
    if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS) {
        for (int i = MIN_CENTIGRADE_TEMP; i <= MAX_CENTIGRADE_TEMP; i += 1) {
            mp_ScrollWheel->AddItem(QString::number(i).rightJustified(2, '0'), i);
        }
        mp_Ui->scrollPanelWidget->SetSubtitle(QApplication::translate("CSystemSetupSettingsWidget", "\302\260C", 0, QApplication::UnicodeUTF8), 0);
    }
    else {
        for (QMap<int, int>::iterator itr = s_MapTemperature.begin();
             s_MapTemperature.end() != itr;
             ++itr)
        {
            mp_ScrollWheel->AddItem(
                        QString::number(itr.key()).rightJustified(2, '0'),
                        itr.value());
        }

        mp_Ui->scrollPanelWidget->SetSubtitle(QApplication::translate("CSystemSetupSettingsWidget", "\302\260F", 0, QApplication::UnicodeUTF8), 0);
    }
    mp_ScrollWheel->SetNonContinuous();
}


/****************************************************************************/
/*!
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::showEvent(QShowEvent *p_Event)
{
    if ((mp_UserSettings != NULL) && (p_Event != NULL) && !p_Event->spontaneous()) {
        InitTemperatureWidget();

        if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS) {
            mp_ScrollWheel->SetThreeDigitMode(false);
            mp_Ui->scrollPanelWidget->SetThreeDigitMode(false);
        }
        else {
            mp_ScrollWheel->SetThreeDigitMode(true);
            mp_Ui->scrollPanelWidget->SetThreeDigitMode(true);
        }

        int Temp = mp_UserSettings->GetTemperatureParaffinBath();
        mp_ScrollWheel->SetCurrentData(Temp);
         qDebug()<<"\n\n SystemSetup settings widget Temp " << Temp;

        ResetButtons();
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::OnUserRoleChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::OnProcessStateChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Enables/Disables the apply button based on the user role/process
 *         running status
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::ResetButtons()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();

    if (Core::CGlobalHelper::CheckIfCanEdit() == false) {
        if (m_CurrentUserRole == MainMenu::CMainWindow::Service) {
            mp_Ui->btnSave->setEnabled(true);
        }
        else {
            mp_Ui->btnSave->setEnabled(false);
        }
        return ;
    }

    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
         (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnSave->setEnabled(true);
    }
    else {
        mp_Ui->btnSave->setEnabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CSystemSetupSettingsWidget", "System Setup", 0, QApplication::UnicodeUTF8));
   m_strChangeMeltPointConfirm12Hrs = QApplication::translate("Settings::CSystemSetupSettingsWidget", "If paraffin melting point is changed, %1 hours will be needed to melt paraffin, really change it?", 0, QApplication::UnicodeUTF8);
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 *  \param p_MainWindow = the application's main window
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
    CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
}

/****************************************************************************/
/*!
 *  \brief Called when the Save button is clicked
 */
/****************************************************************************/

void CSystemSetupSettingsWidget::OnApply()
{
    MainMenu::CMessageDlg ConfirmationMessageDlg;
    ConfirmationMessageDlg.SetTitle(CommonString::strInforMsg);
    int paraffinMeltingPoint = m_UserSettingsTemp.GetTemperatureParaffinBath();
    if (paraffinMeltingPoint <= 63)
    {
        ConfirmationMessageDlg.SetText(m_strChangeMeltPointConfirm12Hrs.arg("12"));
    }
    else
        ConfirmationMessageDlg.SetText(m_strChangeMeltPointConfirm12Hrs.arg("15"));

    ConfirmationMessageDlg.SetIcon(QMessageBox::Warning);
    ConfirmationMessageDlg.SetButtonText(1, CommonString::strYes);//right
    ConfirmationMessageDlg.SetButtonText(3, CommonString::strCancel);//left
    ConfirmationMessageDlg.HideCenterButton();
    if (ConfirmationMessageDlg.exec())
    {
        if (mp_UserSettings)
        {
            m_UserSettingsTemp = *mp_UserSettings;
        }

        int Temp = mp_ScrollWheel->GetCurrentData().toInt();
        m_UserSettingsTemp.SetTemperatureParaffinBath(Temp);

        emit TemperatureChanged(m_UserSettingsTemp);
    }


}

} // end namespace Settings
