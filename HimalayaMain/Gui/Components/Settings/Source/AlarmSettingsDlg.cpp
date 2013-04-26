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
CAlarmSettingsDlg::CAlarmSettingsDlg(AlarmDialogType DialogType, QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CAlarmSettingsDlg),
    m_AlarmScreen(DialogType)
{
    mp_Ui->setupUi(GetContentFrame());

    mp_VolumeScrollWheel = new MainMenu::CScrollWheel;
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

    for (int i = 0; i <= 10; i++) {
        mp_MinWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }
    mp_MinWheel->SetNonContinuous();

    //Second
    for (int i = 0; i < 60; i++) {
        mp_SecondWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }
    mp_SecondWheel->SetNonContinuous();

    if (Error == m_AlarmScreen)
    {
        for (qint32 i = 2; i <= 9; i++) {
            mp_VolumeScrollWheel->AddItem(QString::number(i), i);
        }
    }
    else
    {
        for (qint32 i = 0; i <= 9; i++) {
            mp_VolumeScrollWheel->AddItem(QString::number(i), i);
        }
    }
    mp_VolumeScrollWheel->SetNonContinuous();
    mp_Ui->scrollPanel->AddScrollWheel(mp_VolumeScrollWheel, 0);

    for (qint32 i = 1; i <= 6; i++) {
        mp_SoundScrollWheel->AddItem(QString::number(i), i);
    }
    mp_SoundScrollWheel->SetNonContinuous();
    mp_Ui->sound_scrollpanel->AddScrollWheel(mp_SoundScrollWheel, 0);


    CONNECTSIGNALSLOT(mp_Ui->cancelButton, clicked(), this, reject());
    CONNECTSIGNALSLOT(mp_Ui->saveButton, clicked(), this , OnApply());
    CONNECTSIGNALSLOT(mp_Ui->testButton, clicked(), this, OnPlayTone());
    CONNECTSIGNALSLOT(mp_Ui->periodic_onoffslider,
                      positionChanged(MainMenu::CSliderControl::Position_t),
                      this,
                      OnPeriodicChanged(MainMenu::CSliderControl::Position_t));
}


/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CAlarmSettingsDlg::~CAlarmSettingsDlg()
{
    try {
        delete mp_VolumeScrollWheel;
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
void CAlarmSettingsDlg::UpdateDisplay(void)
{
    int PeriodicTime;

    if(m_AlarmScreen == Information)  {
        SetDialogTitle(tr("Edit Information"));

        // set volume and number
        mp_VolumeScrollWheel->SetCurrentData(mp_UserSettings->GetSoundLevelInformation());
        mp_SoundScrollWheel->SetCurrentData(mp_UserSettings->GetSoundNumberInformation());

        // set periodic time
        PeriodicTime = mp_UserSettings->GetSoundPeriodicTimeInformation();
        mp_MinWheel->SetCurrentData((PeriodicTime/60));
        mp_SecondWheel->SetCurrentData((PeriodicTime%60));

        // set periodic on/off
        if(!mp_UserSettings->GetSoundPeriodicInformation())
        {
            mp_Ui->periodic_onoffslider->SetPosition(MainMenu::CSliderControl::PosRight);
            mp_Ui->periodictime_scrolltable->SetDisabled(true);
        }
        else
        {
            mp_Ui->periodictime_scrolltable->SetDisabled(false);
        }
    }
    else if(m_AlarmScreen == Warning){
        SetDialogTitle(tr("Edit Warning"));

        // set volume and number
        mp_VolumeScrollWheel->SetCurrentData(mp_UserSettings->GetSoundLevelWarning());
        mp_SoundScrollWheel->SetCurrentData(mp_UserSettings->GetSoundNumberWarning());

        // set periodic time
        PeriodicTime = mp_UserSettings->GetSoundPeriodicTimeWarning();
        mp_MinWheel->SetCurrentData((PeriodicTime/60));
        mp_SecondWheel->SetCurrentData((PeriodicTime%60));

        // set periodic on/off
        if(!mp_UserSettings->GetSoundPeriodicWarning())
        {
            mp_Ui->periodic_onoffslider->SetPosition(MainMenu::CSliderControl::PosRight);
            mp_Ui->periodictime_scrolltable->SetDisabled(true);
        }
        else
        {
            mp_Ui->periodictime_scrolltable->SetDisabled(false);
        }
    }
    else if (m_AlarmScreen == Error) {
        SetDialogTitle(tr("Edit Error"));

        // set volume and number
        mp_VolumeScrollWheel->SetCurrentData(mp_UserSettings->GetSoundLevelError());
        mp_SoundScrollWheel->SetCurrentData(mp_UserSettings->GetSoundNumberError());

        // set periodic time
        PeriodicTime = mp_UserSettings->GetSoundPeriodicTimeError();
        mp_MinWheel->SetCurrentData((PeriodicTime/60));
        mp_SecondWheel->SetCurrentData((PeriodicTime%60));

        // hide periodic
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
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) && (!m_ProcessRunning)) {
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
   MainMenu::CDialogFrame::SetDialogTitle(QApplication::translate("Settings::CAlarmSettingsDlg", "Edit Alarm Type 1 - Information", 0, QApplication::UnicodeUTF8));
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
    int PeriodicTime = mp_MinWheel->GetCurrentData().toInt() * 60 +
            mp_SecondWheel->GetCurrentData().toInt();

    m_UserSettingsTemp = *mp_UserSettings;
    if (m_AlarmScreen == Information) {
        m_UserSettingsTemp.SetSoundLevelInformation(mp_VolumeScrollWheel->GetCurrentData().toInt());
        //Adding one to checkedId  because , the id starts from zero
        m_UserSettingsTemp.SetSoundNumberInformation(mp_SoundScrollWheel->GetCurrentData().toInt());
        // m_UserSettingsTemp.SetValue("InformationTone_PeriodicTime",mp_MinWheel->GetCurrentData().toInt());
        if(mp_Ui->periodic_onoffslider->GetPosition() == MainMenu::CSliderControl::PosRight)
        {
            m_UserSettingsTemp.SetSoundPeriodicInformation(false);
        }
        else
        {
            m_UserSettingsTemp.SetSoundPeriodicInformation(true);
            m_UserSettingsTemp.SetSoundPeriodicTimeInformation(PeriodicTime);
        }

    }
    if (m_AlarmScreen == Warning) {
        m_UserSettingsTemp.SetSoundLevelWarning(mp_VolumeScrollWheel->GetCurrentData().toInt());
        //Adding one to checkedId  because , the id starts from zero
        m_UserSettingsTemp.SetSoundNumberWarning(mp_SoundScrollWheel->GetCurrentData().toInt());
        //m_UserSettingsTemp.SetValue("WarningTone_PeriodicTime",mp_MinWheel->GetCurrentData().toInt());
        if(mp_Ui->periodic_onoffslider->GetPosition() == MainMenu::CSliderControl::PosRight)
        {
            m_UserSettingsTemp.SetSoundPeriodicWarning(false);
        }
        else
        {
            m_UserSettingsTemp.SetSoundPeriodicWarning(true);
            m_UserSettingsTemp.SetSoundPeriodicTimeWarning(PeriodicTime);
        }
    }
    else if(m_AlarmScreen == Error){
        m_UserSettingsTemp.SetSoundLevelError(mp_VolumeScrollWheel->GetCurrentData().toInt());
        //Adding one to checkedId  because , the id starts from zero
        m_UserSettingsTemp.SetSoundNumberError(mp_SoundScrollWheel->GetCurrentData().toInt());
        //m_UserSettingsTemp.SetValue("ErrorTone_PeriodicTime",mp_MinWheel->GetCurrentData().toInt());
        m_UserSettingsTemp.SetSoundPeriodicTimeError(PeriodicTime);
    }
    emit AlarmSettingsChanged(m_UserSettingsTemp);
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
    emit PlayTestTone(mp_VolumeScrollWheel->GetCurrentData().toInt(),(mp_SoundScrollWheel->GetCurrentData().toInt()), m_Type);
}

/****************************************************************************/
/*!
 *  \brief Called when the periodic slider is changed.
 */
/****************************************************************************/
void CAlarmSettingsDlg::OnPeriodicChanged(MainMenu::CSliderControl::Position_t Position)
{
    if (MainMenu::CSliderControl::PosLeft == Position)
    {
        mp_Ui->periodictime_scrolltable->SetDisabled(false);
    }
    else
    {
        mp_Ui->periodictime_scrolltable->SetDisabled(true);
    }
}

} // end namespace Settings
