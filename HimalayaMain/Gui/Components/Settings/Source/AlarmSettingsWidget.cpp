/****************************************************************************/
/*! \file AlarmSettingsWidget.cpp
 *
 *  \brief AlarmSettingsWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-20
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

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Settings/Include/AlarmSettingsWidget.h"
#include "ui_AlarmSettingsWidget.h"

namespace Settings {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CAlarmSettingsWidget::CAlarmSettingsWidget(QWidget *p_Parent) :
    MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CAlarmSettingsWidget),
    mp_DataConnector(NULL)
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("Alarm"));

    mp_Note = new Settings::CAlarmSettingsDlg(false, this);
    mp_Note->setModal(true);

    mp_Error = new Settings::CAlarmSettingsDlg(true, this);
    mp_Error->setModal(true);

    CONNECTSIGNALSLOT(mp_Ui->noteButton, clicked(), this, OnNoteEdit());
    CONNECTSIGNALSLOT(mp_Ui->errorButton, clicked(), this, OnErrorEdit());
    CONNECTSIGNALSIGNAL(mp_Error, AlarmSettingsChanged(DataManager::CUserSettings &), this , AlarmSettingsChanged(DataManager::CUserSettings &));
    CONNECTSIGNALSIGNAL(mp_Note, AlarmSettingsChanged(DataManager::CUserSettings &), this , AlarmSettingsChanged(DataManager::CUserSettings &));
    CONNECTSIGNALSIGNAL(mp_Error, PlayTestTone(quint8, quint8, bool ), this, PlayTestTone(quint8, quint8, bool ));
    CONNECTSIGNALSIGNAL(mp_Note, PlayTestTone(quint8, quint8, bool ), this, PlayTestTone(quint8, quint8, bool ));
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CAlarmSettingsWidget::~CAlarmSettingsWidget()
{
    try {
        delete mp_Error;
        delete mp_Note;
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

void CAlarmSettingsWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
         //   mp_Ui->retranslateUi(this);
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
void CAlarmSettingsWidget::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
    m_UserSettings = *p_UserSettings;
    mp_Note->SetUserSettings(&m_UserSettings);
    mp_Error->SetUserSettings(&m_UserSettings);
    UpdateLabels();
}

/****************************************************************************/
/*!
 *  \brief Updates the content of this widget
 */
/****************************************************************************/
void CAlarmSettingsWidget::UpdateLabels()
{
	mp_Ui->noteSound->setText(tr("Sound: Sound N %1").arg(QString::number(m_UserSettings.GetSoundNumberWarning())));
    mp_Ui->noteVolume->setText(tr("Volume:  %1").arg(QString::number(m_UserSettings.GetSoundLevelWarning())));
    mp_Ui->errorSound->setText(tr("Sound: Sound E %1").arg(QString::number(m_UserSettings.GetSoundNumberError())));
    mp_Ui->errorVolume->setText(tr("Volume:  %1").arg( QString::number(m_UserSettings.GetSoundLevelError())));
}

/****************************************************************************/
/*!
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CAlarmSettingsWidget::showEvent(QShowEvent *p_Event)
{
    if((p_Event != NULL) && !p_Event->spontaneous()) {
        UpdateLabels();
        ResetButtons();
    }
}

/****************************************************************************/
/*!
 *  \brief Opens the note alarm editing dialog
 */
/****************************************************************************/
void CAlarmSettingsWidget::OnNoteEdit()
{
    mp_Note->SetDialogType(false);
    mp_Note->UpdateDisplay(m_UserSettings.GetSoundLevelWarning(), m_UserSettings.GetSoundNumberWarning());
    mp_Note->show();
}

/****************************************************************************/
/*!
 *  \brief Opens the error alarm editing dialog
 */
/****************************************************************************/
void CAlarmSettingsWidget::OnErrorEdit()
{  
     mp_Error->SetDialogType(true);
     mp_Error->UpdateDisplay(m_UserSettings.GetSoundLevelError(), m_UserSettings.GetSoundNumberError());
     mp_Error->show();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CAlarmSettingsWidget::OnProcessStateChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Enables/Disables the apply button based on the user role/process
 *         running status
 */
/****************************************************************************/
void CAlarmSettingsWidget::ResetButtons()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (!m_ProcessRunning) {
        //Edit Mode
        mp_Ui->noteButton->setEnabled(true);
        mp_Ui->errorButton->setEnabled(true);
    }
    else {
        mp_Ui->noteButton->setEnabled(false);
        mp_Ui->errorButton->setEnabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CAlarmSettingsWidget::RetranslateUI()
{
    MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CAlarmSettingsWidget", "Alarm", 0, QApplication::UnicodeUTF8));
    mp_Ui->groupBox->setTitle(QApplication::translate("Settings::CAlarmSettingsWidget", "Alarm Type 1 - Note", 0, QApplication::UnicodeUTF8));
    mp_Ui->noteButton->setText(QApplication::translate("Settings::CAlarmSettingsWidget", "Edit", 0, QApplication::UnicodeUTF8));
    mp_Ui->groupBox_2->setTitle(QApplication::translate("Settings::CAlarmSettingsWidget", "AlarmType 2 - Error", 0, QApplication::UnicodeUTF8));
    mp_Ui->errorButton->setText(QApplication::translate("Settings::CAlarmSettingsWidget", "Edit", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CAlarmSettingsWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    mp_Note->SetPtrToMainWindow(mp_MainWindow);
    mp_Error->SetPtrToMainWindow(mp_MainWindow);
}

} // end namespace Settings
