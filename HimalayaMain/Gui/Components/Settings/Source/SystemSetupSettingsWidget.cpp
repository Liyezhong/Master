/****************************************************************************/
/*! \file AgitationSettingsWidget.cpp
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


#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Settings/Include/SystemSetupSettingsWidget.h"
#include "ui_SystemSetupSettingsWidget.h"

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
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CSystemSetupSettingsWidget::CSystemSetupSettingsWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CSystemSetupSettingsWidget), mp_UserSettings(NULL), mp_MainWindow(NULL)
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
void CSystemSetupSettingsWidget::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
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
        for (int i = MIN_CENTIGRADE_TEMP; i <= MAX_CENTIGRADE_TEMP; i += 5) {
            mp_ScrollWheel->AddItem(QString::number(i).rightJustified(2, '0'), i);
        }
        mp_Ui->scrollPanelWidget->SetSubtitle(QApplication::translate("COvenSettingsWidget", "\302\260C", 0, QApplication::UnicodeUTF8), 0);
    }
    else {
        for (int i = MIN_FARENHEIT_TEMP; i <= MAX_FARENHEIT_TEMP; i += 9) {
            mp_ScrollWheel->AddItem(QString::number(i).rightJustified(2, '0'), i);
        }
        mp_Ui->scrollPanelWidget->SetSubtitle(QApplication::translate("COvenSettingsWidget", "\302\260F", 0, QApplication::UnicodeUTF8), 0);
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
            mp_ScrollWheel->SetCurrentData(QString::number(mp_UserSettings->GetValue("Oven_Temp").toInt()));
             qDebug()<<"\n\n Oven settings widget Temp" << mp_UserSettings->GetValue("Oven_Temp").toInt();
        }
        else {
            mp_ScrollWheel->SetThreeDigitMode(true);
            mp_Ui->scrollPanelWidget->SetThreeDigitMode(true);
            mp_ScrollWheel->SetCurrentData(tr("%1").arg((((mp_UserSettings->GetValue("Oven_Temp").toInt() - 50) / 5) * 9) + 122));
            qDebug()<<"\n\n Oven settings widget Temp" << mp_UserSettings->GetValue("Oven_Temp").toInt();
        }
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
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CSystemSetupSettingsWidget", "Agitation", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
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
    m_UserSettingsTemp = *mp_UserSettings;
    if (m_UserSettingsTemp.GetTemperatureFormat() == Global::TEMP_FORMAT_FAHRENHEIT) {
        qint32 TemperatureCelsius = ((mp_ScrollWheel->GetCurrentData().toInt() - 32) * 5) / 9;
        m_UserSettingsTemp.SetValue("Oven_Temp", TemperatureCelsius);
    }
    else {
        m_UserSettingsTemp.SetValue("Oven_Temp", mp_ScrollWheel->GetCurrentData().toInt());

    emit TemperatureChanged(m_UserSettingsTemp);
    }
}

} // end namespace Settings
