/****************************************************************************/
/*! \file AlarmSettingsDlg.cpp
 *
 *  \brief AlarmSettingsDlg implementation.
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
#include "Settings/Include/AlarmSettingsDlg.h"
#include "ui_AlarmSettingsDlg.h"

namespace Settings {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam Error = Error tone (true) or not (false)
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CAlarmSettingsDlg::CAlarmSettingsDlg(bool Error, QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CAlarmSettingsDlg)
{
    char TypeLetter;

    mp_Ui->setupUi(GetContentFrame());

    mp_ScrollWheel = new MainMenu::CScrollWheel;
    mp_Ui->scrollPanel->Init(1);
//    for (qint32 i = 0; i <= 9; i++) {
//        mp_ScrollWheel->AddItem(QString::number(i), i);
//    }
//    mp_ScrollWheel->SetNonContinuous();
//    mp_Ui->scrollPanel->AddScrollWheel(mp_ScrollWheel, 0);

    m_ButtonGroup.addButton(mp_Ui->soundButton0, 0);
    m_ButtonGroup.addButton(mp_Ui->soundButton1, 1);
    m_ButtonGroup.addButton(mp_Ui->soundButton2, 2);
    m_ButtonGroup.addButton(mp_Ui->soundButton3, 3);
    m_ButtonGroup.addButton(mp_Ui->soundButton4, 4);
    m_ButtonGroup.addButton(mp_Ui->soundButton5, 5);

    if (Error == true) {
        SetDialogTitle(tr("Edit Alarm Type 2 - Error"));
        TypeLetter = 'E';
    }
    else {
        SetDialogTitle(tr("Edit Alarm Type 1 - Note"));
        TypeLetter = 'N';
    }
    for (qint32 i = 0; i < 6; i++) {
        m_ButtonGroup.button(i)->setText(QString("Sound %1%2").arg(TypeLetter).arg(i + 1));
    }

    CONNECTSIGNALSLOT(mp_Ui->cancelButton, clicked(), this, reject());
    CONNECTSIGNALSLOT(mp_Ui->saveButton, clicked(), this , OnApply());
    CONNECTSIGNALSLOT(mp_Ui->testButton, clicked(), this, OnPlayTone());
}


/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CAlarmSettingsDlg::~CAlarmSettingsDlg()
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

void CAlarmSettingsDlg::changeEvent(QEvent *p_Event)
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
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CAlarmSettingsDlg::showEvent(QEvent *p_Event)
{
    if((p_Event != NULL) && !p_Event->spontaneous()) {
        ResetButtons();
    }    
}

/****************************************************************************/
/*!
 *  \brief Updates the content of the dialog
 *
 *  \iparam Volume = Sound volume
 *  \iparam Sound = Selected alarm sound
 */
/****************************************************************************/
void CAlarmSettingsDlg::UpdateDisplay(qint32 Volume, qint32 Sound)
{
    mp_ScrollWheel->SetCurrentData(Volume);
    m_ButtonGroup.button(Sound - 1)->setChecked(true);

    if (m_ErrorAlarmScreen == true) {
        SetDialogTitle(tr("Edit Alarm Type 2 - Error"));
        for (qint32 i = 2; i <= 9; i++) {
            mp_ScrollWheel->AddItem(QString::number(i), i);
        }
        mp_ScrollWheel->SetNonContinuous();
        mp_Ui->scrollPanel->AddScrollWheel(mp_ScrollWheel, 0);
        mp_ScrollWheel->SetCurrentData(mp_UserSettings->GetSoundLevelError());
       // m_ButtonGroup.
    }
    else {
        SetDialogTitle(tr("Edit Alarm Type 1 - Note"));
        for (qint32 i = 0; i <= 9; i++) {
            mp_ScrollWheel->AddItem(QString::number(i), i);
        }
        mp_ScrollWheel->SetNonContinuous();
        mp_Ui->scrollPanel->AddScrollWheel(mp_ScrollWheel, 0);
        mp_ScrollWheel->SetCurrentData(mp_UserSettings->GetSoundLevelWarning());
    }

}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CAlarmSettingsDlg::OnProcessStateChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Enables/Disables the apply button based on the user role/process
 *         running status
 */
/****************************************************************************/
void CAlarmSettingsDlg::ResetButtons()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (!m_ProcessRunning) {
        //Edit Mode
        mp_Ui->saveButton->setEnabled(true);
    }
    else {
        mp_Ui->saveButton->setEnabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CAlarmSettingsDlg::RetranslateUI()
{
   MainMenu::CDialogFrame::SetDialogTitle(QApplication::translate("Settings::CAlarmSettingsDlg", "Edit Alarm Type 2 - Error", 0, QApplication::UnicodeUTF8));
   MainMenu::CDialogFrame::SetDialogTitle(QApplication::translate("Settings::CAlarmSettingsDlg", "Edit Alarm Type 1 - Note", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CAlarmSettingsDlg::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
}

/****************************************************************************/
/*!
 *  \brief Called when the apply button is clicked
 */
/****************************************************************************/
void CAlarmSettingsDlg::OnApply()
{   qDebug()<<"calling the slot play tone"<<endl;
   // m_UserSettingsTemp = *mp_UserSettings;
    if (!m_ErrorAlarmScreen) {
        mp_UserSettings->SetSoundLevelWarning(mp_ScrollWheel->GetCurrentData().toInt());
        //Adding one to checkedId  because , the id starts from zero
        mp_UserSettings->SetSoundNumberWarning(m_ButtonGroup.checkedId() + 1);

    }
    else {
        mp_UserSettings->SetSoundLevelError(mp_ScrollWheel->GetCurrentData().toInt());
        //Adding one to checkedId  because , the id starts from zero
        mp_UserSettings->SetSoundNumberError(m_ButtonGroup.checkedId() + 1);
    }
    emit AlarmSettingsChanged(*mp_UserSettings);
    //close();
    accept();
}

/****************************************************************************/
/*!
 *  \brief Called when the Test button is clicked
 */
/****************************************************************************/
void CAlarmSettingsDlg::OnPlayTone()
{
    qDebug()<<"calling the slot play tone"<<endl;

    if (!m_ErrorAlarmScreen){

        m_Type=true;
    }
    else{
        m_Type=false;
    }
    emit PlayTestTone(mp_ScrollWheel->GetCurrentData().toInt(), (m_ButtonGroup.checkedId() + 1), m_Type);
}


} // end namespace Settings
