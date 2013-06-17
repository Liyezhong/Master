/****************************************************************************/
/*! \file DashboardDateTimeWidget.cpp
 *
 *  \brief DashboardDateTimeWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 27-Mar-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Dashboard/Include/DashboardDateTimeWidget.h"
#include <Global/Include/AdjustedTime.h>
#include "Application/Include/LeicaStyle.h"

#include "ui_DashboardDateTimeWidget.h"
#include <QDebug>
#include <QPainter>

namespace Dashboard {

QString CDashboardDateTimeWidget :: SELECTED_PROGRAM_NAME = tr("");
const int ONE_WEEK_TIME_OFFSET_VALUE = (7 * 24 * 60 * 60);

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CDashboardDateTimeWidget::CDashboardDateTimeWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CDashboardDateTimeWidget),
    m_strEndTimeForProgram(tr("End Time for Program"))
{

    mp_Ui->setupUi(GetContentFrame());


    mp_DayWheel = new MainMenu::CScrollWheel();
    mp_MonthWheel = new MainMenu::CScrollWheel();
    mp_YearWheel = new MainMenu::CScrollWheel();
    mp_HourWheel = new MainMenu::CScrollWheel();
    mp_MinWheel = new MainMenu::CScrollWheel();

    mp_MessageDlg = new MainMenu::CMessageDlg();

    // Day
    for (int i = 1; i <= 31; i++) {
        mp_DayWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }

    // Month
    for (int i = 1; i <= 12; i++) {
        mp_MonthWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }

    // Year
    for (int i = 2000; i <= 2099; i++) {
        mp_YearWheel->AddItem(QString::number(i).right(QString::number(i).length() - 2), i);
    }

    // Minute
    for (int i = 0; i < 60; i++) {
        mp_MinWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }

    mp_Ui->scrollPanelDate->Init(3);
    mp_Ui->scrollPanelDate->SetTitle(tr("Date"));
    mp_Ui->scrollPanelDate->AddScrollWheel(mp_DayWheel, 0);
    mp_Ui->scrollPanelDate->SetSubtitle(tr("Day"), 0);
    mp_Ui->scrollPanelDate->AddSeparator(MainMenu::CWheelPanel::FULLSTOP, 0);
    mp_Ui->scrollPanelDate->AddScrollWheel(mp_MonthWheel, 1);
    mp_Ui->scrollPanelDate->SetSubtitle(tr("Month"), 1);
    mp_Ui->scrollPanelDate->AddSeparator(MainMenu::CWheelPanel::FULLSTOP, 1);
    mp_Ui->scrollPanelDate->AddScrollWheel(mp_YearWheel, 2);
    mp_Ui->scrollPanelDate->SetSubtitle(tr("Year"), 2);

    mp_Ui->scrollPanelTime->Init(2);
    mp_Ui->scrollPanelTime->SetTitle(tr("Time"));
    mp_Ui->scrollPanelTime->AddScrollWheel(mp_HourWheel, 0);
    mp_Ui->scrollPanelTime->SetSubtitle(tr("Hour"), 0);
    mp_Ui->scrollPanelTime->AddSeparator(MainMenu::CWheelPanel::COLON, 0);
    mp_Ui->scrollPanelTime->AddScrollWheel(mp_MinWheel, 1);
    mp_Ui->scrollPanelTime->SetSubtitle(tr("Minute"), 1);



    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnASAP, clicked(), this, OnSetASAPDateTime());
    CONNECTSIGNALSLOT(mp_Ui->btnOK, clicked(), this, OnOK());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDashboardDateTimeWidget::~CDashboardDateTimeWidget()
{
    try {
        delete mp_DayWheel;
        delete mp_MonthWheel;
        delete mp_YearWheel;
        delete mp_HourWheel;
        delete mp_MinWheel;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint.
    }
}

void CDashboardDateTimeWidget ::UpdateProgramName()
{
     QString str(m_strEndTimeForProgram + " \"%1\"");
     SetPanelTitle(str.arg(SELECTED_PROGRAM_NAME));
     SetTitleCenter();
}

void CDashboardDateTimeWidget::SetASAPDateTime(const QDateTime& DateTime)
{
    m_ASAPDateTime = DateTime;
}

void CDashboardDateTimeWidget::show()
{
    RefreshDateTime();
    MainMenu::CPanelFrame::show();
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CDashboardDateTimeWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            mp_Ui->retranslateUi(this);
            this->RetranslateUI();
            break;
        default:
            break;
    }
}

/****************************************************************************/
/*!
 *  \brief Sets the date and time displayed by the scroll wheels
 *
 *  \iparam TimeFormat = 12 or 24 hour format
 */
/****************************************************************************/
void CDashboardDateTimeWidget::RefreshDateTime(Global::TimeFormat TimeFormat)
{
    // Hour
    mp_HourWheel->ClearItems();
    for (int i = 0; i < 24; i++) {
        if (TimeFormat == Global::TIME_24) {
            mp_HourWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
        }
        else {
            QPixmap Pixmap = (i < 12) ?
                            QPixmap(QString(":/%1/Digits/Digit_AM.png").arg(Application::CLeicaStyle::GetProjectNameString())) :
                            QPixmap(QString(":/%1/Digits/Digit_PM.png").arg(Application::CLeicaStyle::GetProjectNameString())) ;
            qint32 Hour = (i % 12 == 0) ? 12 : i % 12;
            mp_HourWheel->AddItem(QString("%1").arg(Hour, 2, 10, QChar('0')), i, Pixmap);
        }
    }

    mp_DayWheel->SetCurrentData(m_ASAPDateTime.date().day());
    mp_MonthWheel->SetCurrentData(m_ASAPDateTime.date().month());
    mp_YearWheel->SetCurrentData(m_ASAPDateTime.date().year());

    mp_HourWheel->SetCurrentData(m_ASAPDateTime.time().hour());
    mp_MinWheel->SetCurrentData(m_ASAPDateTime.time().minute());

    // make it UTC
    m_selDateTime.setTimeSpec(Qt::UTC);

    m_selDateTime.setDate(QDate(mp_YearWheel->GetCurrentData().toInt(),
                             mp_MonthWheel->GetCurrentData().toInt(),
                             mp_DayWheel->GetCurrentData().toInt()));

    m_selDateTime.setTime(QTime(mp_HourWheel->GetCurrentData().toInt(), mp_MinWheel->GetCurrentData().toInt()));
    mp_Ui->lblDateTimeDisplay->setText(m_ASAPDateTime.toString());
}

/****************************************************************************/
/*!
 *  \brief Reads data from the scroll wheels
 */
/****************************************************************************/
void CDashboardDateTimeWidget::OnOK()
{
    // make it UTC
    m_selDateTime.setTimeSpec(Qt::UTC);

    m_selDateTime.setDate(QDate(mp_YearWheel->GetCurrentData().toInt(),
                             mp_MonthWheel->GetCurrentData().toInt(),
                             mp_DayWheel->GetCurrentData().toInt()));

    m_selDateTime.setTime(QTime(mp_HourWheel->GetCurrentData().toInt(), mp_MinWheel->GetCurrentData().toInt()));


    // get adjusted time
    QDateTime CurTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    int secsDifference = CurTime.secsTo(m_selDateTime);
    qDebug() << "Date Time Difference is" << secsDifference;

    if(m_selDateTime >= m_ASAPDateTime && secsDifference <= ONE_WEEK_TIME_OFFSET_VALUE) {
        emit OnSelectDateTime(m_selDateTime);
        close();
    } else {
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(tr("Warning"));
        mp_MessageDlg->SetText(tr("Program End Date Time cannot be later than one week or earlier than the ASAP End Date Time."));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();    // Hiding First Two Buttons in the Message Dialog
        mp_MessageDlg->Show();
    }
}


/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CDashboardDateTimeWidget::RetranslateUI()
{
    mp_Ui->scrollPanelDate->SetTitle(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Date", 0, QApplication::UnicodeUTF8));
    mp_Ui->scrollPanelDate->SetSubtitle(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Day", 0, QApplication::UnicodeUTF8),0);
    mp_Ui->scrollPanelDate->SetSubtitle(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Month", 0, QApplication::UnicodeUTF8),1);
    mp_Ui->scrollPanelDate->SetSubtitle(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Year", 0, QApplication::UnicodeUTF8),2);


    mp_Ui->scrollPanelTime->SetTitle(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Time", 0, QApplication::UnicodeUTF8));
    mp_Ui->scrollPanelTime->SetSubtitle(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Hour", 0, QApplication::UnicodeUTF8),0);
    mp_Ui->scrollPanelTime->SetSubtitle(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Minute", 0, QApplication::UnicodeUTF8),1);

    mp_MessageDlg->SetTitle(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Warning", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetText(QApplication::translate("Dashboard::CDashboardDateTimeWidget", "Program End Date Time cannot be later than one week or earlier than the ASAP End Date Time.", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetButtonText(1, QApplication::translate("Dashboard::CDashboardDateTimeWidget", "OK", 0, QApplication::UnicodeUTF8));
    m_strEndTimeForProgram = QApplication::translate("Dashboard::CDashboardDateTimeWidget", "End Time for Program", 0, QApplication::UnicodeUTF8);
}


void CDashboardDateTimeWidget::OnCancel()
{
    close();
}

void CDashboardDateTimeWidget::OnSetASAPDateTime()
{
    RefreshDateTime();
}

} // end namespace Dashboard
