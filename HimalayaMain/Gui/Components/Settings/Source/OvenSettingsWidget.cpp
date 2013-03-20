/****************************************************************************/
/*! \file OvenSettingsWidget.cpp
 *
 *  \brief OvenSettingsWidget implementation.
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
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/SliderControl.h"
#include "Settings/Include/OvenSettingsWidget.h"
#include "ui_OvenSettingsWidget.h"
#include <QDebug>

namespace Settings {

//! Minimal scroll wheel temperature in degree Celsius
#define MIN_CENTIGRADE_TEMP   40
//! Maximal scroll wheel temperature in degree Celsius
#define MAX_CENTIGRADE_TEMP   70
//! Minimal scroll wheel temperature in degree Fahrenheit
#define MIN_FARENHEIT_TEMP    104
//! Maximal scroll wheel temperature in degree Fahrenheit
#define MAX_FARENHEIT_TEMP    158

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
COvenSettingsWidget::COvenSettingsWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::COvenSettingsWidget), mp_UserSettings(NULL)
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("Oven"));

    mp_ScrollWheel = new MainMenu::CScrollWheel;

    mp_Ui->scrollPanelWidget->Init(1);
    mp_Ui->scrollPanelWidget->AddScrollWheel(mp_ScrollWheel, 0);
    CONNECTSIGNALSLOT(mp_Ui->btnApply, clicked(), this, OnApply());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
COvenSettingsWidget::~COvenSettingsWidget()
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
void COvenSettingsWidget::changeEvent(QEvent *p_Event)
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
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void COvenSettingsWidget::showEvent(QShowEvent *p_Event)
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
            mp_ScrollWheel->SetCurrentData(tr("%1").arg((((mp_UserSettings->GetValue("Oven_Temp").toInt() - 40) / 5) * 9) + 104));
            qDebug()<<"\n\n Oven settings widget Temp" << mp_UserSettings->GetValue("Oven_Temp").toInt();
        }

        if (Global::StringToOvenStartMode(mp_UserSettings->GetValue("Oven_StartMode")) == Global::OVENSTART_BEFORE_PROGRAM) {
            mp_Ui->horizontalSlider->SetPosition(MainMenu::CSliderControl::PosLeft);
        }
        else {
            mp_Ui->horizontalSlider->SetPosition(MainMenu::CSliderControl::PosRight);
        }
        ResetButtons();
    }
}

/****************************************************************************/
/*!
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void COvenSettingsWidget::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;
}

/****************************************************************************/
/*!
 *  \brief Initializes the temperature scroll wheel
 */
/****************************************************************************/
void COvenSettingsWidget::InitTemperatureWidget()
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
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void COvenSettingsWidget::OnUserRoleChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void COvenSettingsWidget::OnProcessStateChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Enables/Disables the apply button based on the user role/process
 *         running status
 */
/****************************************************************************/
void COvenSettingsWidget::ResetButtons()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
         (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnApply->setEnabled(true);
    }
    else {
        mp_Ui->btnApply->setEnabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Called when the apply button is clicked
 */
/****************************************************************************/
void COvenSettingsWidget::OnApply()
{
    m_UserSettingsTemp = *mp_UserSettings;
    if (m_UserSettingsTemp.GetTemperatureFormat() == Global::TEMP_FORMAT_FAHRENHEIT) {
        qint32 TemperatureCelsius = ((mp_ScrollWheel->GetCurrentData().toInt() - 32) * 5) / 9;
        m_UserSettingsTemp.SetValue("Oven_Temp", TemperatureCelsius);
    }
    else {
        m_UserSettingsTemp.SetValue("Oven_Temp", mp_ScrollWheel->GetCurrentData().toInt());
    }
    if (mp_Ui->horizontalSlider->GetPosition() == MainMenu::CSliderControl::PosLeft) {
        m_UserSettingsTemp.SetValue("Oven_StartMode", Global::OvenStartModeToString(Global::OVENSTART_BEFORE_PROGRAM));
    }
    else {
        m_UserSettingsTemp.SetValue("Oven_StartMode", Global::OvenStartModeToString(Global::OVENSTART_AFTER_STARTUP));
    }
    emit OvenTemperatureChanged(m_UserSettingsTemp);
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void COvenSettingsWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::COvenSettingsWidget", "Oven", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void COvenSettingsWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
    CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
}
} // end namespace Settings
