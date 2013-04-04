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
    mp_SoundScrollWheel = new MainMenu::CScrollWheel;
    mp_SecondWheel = new MainMenu::CScrollWheel();
    mp_MinWheel = new MainMenu::CScrollWheel();

    mp_Ui->scrollPanel->Init(1);
    mp_Ui->sound_scrollpanel->Init(1);
    mp_Ui->sound_scrollpanel->SetTitle("Sound");
    mp_Ui->scrollPanel->SetTitle("Volume");

    mp_Ui->periodictime_scrolltable->Init(2);
    mp_Ui->periodictime_scrolltable->SetTitle(tr("Periodi Time"));
    mp_Ui->periodictime_scrolltable->AddScrollWheel(mp_MinWheel, 0);
    mp_Ui->periodictime_scrolltable->SetSubtitle(tr("Minute"), 0);
    mp_Ui->periodictime_scrolltable->AddSeparator(MainMenu::CWheelPanel::COLON, 0);
    mp_Ui->periodictime_scrolltable->AddScrollWheel(mp_SecondWheel,1);
    mp_Ui->periodictime_scrolltable->SetSubtitle(tr("Second"), 1);

    if (Error == true) {
        SetDialogTitle(tr("Edit Alarm Type 2 - Error"));
        TypeLetter = 'E';
    }
    else {
        SetDialogTitle(tr("Edit Alarm Type 1 - Note"));
        TypeLetter = 'N';
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
        delete mp_SoundScrollWheel;
        delete mp_SecondWheel ;
        delete mp_MinWheel ;
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
    mp_SoundScrollWheel->SetCurrentData(Sound);
    //QDateTime DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    // Minute
    for (int i = 0; i <= 9; i++) {
        mp_MinWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }
    mp_MinWheel->SetNonContinuous();
    mp_MinWheel->SetCurrentData(0);

    //Second
    for (int i = 0; i < 60; i++) {
        mp_SecondWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }
    mp_SecondWheel->SetNonContinuous();
    mp_SecondWheel->SetCurrentData(0);

    for (qint32 i = 0; i <= 9; i++) {
        mp_ScrollWheel->AddItem(QString::number(i), i);
    }
    mp_ScrollWheel->SetNonContinuous();
    mp_Ui->scrollPanel->AddScrollWheel(mp_ScrollWheel, 0);

    for (qint32 i = 0; i <= 6; i++) {
        mp_SoundScrollWheel->AddItem(QString::number(i), i);
    }
    mp_SoundScrollWheel->SetNonContinuous();
    mp_Ui->sound_scrollpanel->AddScrollWheel(mp_SoundScrollWheel, 0);


    if(m_AlarmScreen == Information)  {
        SetDialogTitle(tr("Edit Alarm Type 1 - Information"));
        mp_ScrollWheel->SetCurrentData(mp_UserSettings->GetSoundLevelWarning());
        mp_SoundScrollWheel->SetCurrentData(mp_UserSettings->GetSoundNumberWarning());
        if (mp_Ui->periodic_onoffslider->GetPosition() == MainMenu::CSliderControl::PosRight)
            mp_Ui->periodictime_scrolltable->hide();
        else
            mp_Ui->periodictime_scrolltable->show();
    }

    else if(m_AlarmScreen == Warning){
        SetDialogTitle(tr("Edit Alarm Type 2 - Warning"));
        mp_ScrollWheel->SetCurrentData(mp_UserSettings->GetSoundLevelWarning());
        mp_SoundScrollWheel->SetCurrentData(mp_UserSettings->GetSoundNumberWarning());
        if (mp_Ui->periodic_onoffslider->GetPosition() == MainMenu::CSliderControl::PosRight)
            mp_Ui->periodictime_scrolltable->hide();
        else
            mp_Ui->periodictime_scrolltable->show();
    }

    else if (m_AlarmScreen == Error) {
        SetDialogTitle(tr("Edit Alarm Type 3 - Error"));
        mp_ScrollWheel->SetCurrentData(mp_UserSettings->GetSoundLevelError());
        mp_SoundScrollWheel->SetCurrentData(mp_UserSettings->GetSoundNumberError());
        mp_Ui->periodic_onoffslider->hide();
        mp_Ui->label_periodic_2->hide();
        mp_Ui->label_periodicoff_2->hide();
        mp_Ui->label_periodicon_2->hide();
        // m_ButtonGroup.
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
{
    qDebug()<<"calling the slot play tone"<<endl;
    // m_UserSettingsTemp = *mp_UserSettings;
    if (m_AlarmScreen == Information) {
        mp_UserSettings->SetSoundLevelWarning(mp_ScrollWheel->GetCurrentData().toInt());
        //Adding one to checkedId  because , the id starts from zero
        mp_UserSettings->SetSoundNumberWarning(mp_SoundScrollWheel->GetCurrentData().toInt());
    }
    if (m_AlarmScreen == Warning) {
        mp_UserSettings->SetSoundLevelWarning(mp_ScrollWheel->GetCurrentData().toInt());
        //Adding one to checkedId  because , the id starts from zero
        mp_UserSettings->SetSoundNumberWarning(mp_SoundScrollWheel->GetCurrentData().toInt());
    }
    else if(m_AlarmScreen == Error){
        mp_UserSettings->SetSoundLevelError(mp_ScrollWheel->GetCurrentData().toInt());
        //Adding one to checkedId  because , the id starts from zero
        mp_UserSettings->SetSoundNumberError(mp_SoundScrollWheel->GetCurrentData().toInt());
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

    if (m_AlarmScreen == Information){

        m_Type=true;
    }
    else if (m_AlarmScreen == Error){

        m_Type=true;
    }
    else if(m_AlarmScreen == Warning){
        m_Type=false;
    }
    emit PlayTestTone(mp_ScrollWheel->GetCurrentData().toInt(),(mp_SoundScrollWheel->GetCurrentData().toInt()), m_Type);
}


} // end namespace Settings
