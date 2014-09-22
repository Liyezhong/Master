/****************************************************************************/
/*! \file PressureInputDialog.cpp
 *
 *  \brief Implementation of file for class PressureInputDialog.
 *
 *  \b Description:
 *          This class implements a base widget for displaying the wait dialogs.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-09
 *   $Author:  $ Sunny Qu
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
#include "DiagnosticsManufacturing/Include/PressureInputDialog.h"
#include "ui_PressureInputDialog.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Core/Include/CMessageString.h"
#include <QDebug>
#include <QtCore/qmath.h>

namespace DiagnosticsManufacturing {

const QString REGEXP_NUMERIC_VALIDATOR  = "^[0-9.]{1,5}$"; //!< Reg expression for the validator

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CPressureInputDialog::CPressureInputDialog(Service::ModuleTestCaseID Id, QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CPressureInputDialog),
    m_LineEditStringLeft(""),
    m_LineEditStringMiddle(""),
    m_LineEditStringRight(""),
    m_Seconds(0),
    m_WaitSeconds(0),
    m_PassFlag(false),
    m_TestCaseId(Id)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    this->SetDialogTitle("User Input");

    mp_Ui->lineEdit->installEventFilter(this);
    mp_Ui->lineEdit_2->installEventFilter(this);
    mp_Ui->lineEdit_2->setDisabled(true);
    mp_Ui->pushButtonOk->setDisabled(true);

    mp_KeyBoardWidget = new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);

    CONNECTSIGNALSLOTGUI(mp_Ui->pushButtonOk, clicked(), this, AbortDialog());

    m_Timer = new QTimer(this);
    CONNECTSIGNALSLOTGUI(m_Timer, timeout(), this, OnTimeout());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CPressureInputDialog::~CPressureInputDialog()
{
    try {
        m_Timer->stop();
        delete m_Timer;
        delete mp_KeyBoardWidget;
        delete mp_Ui;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Sets the text displayed in the wait dialog
 *
 *  \iparam Text = Label text
 */
/****************************************************************************/
void CPressureInputDialog::SetText(QString Text)
{
    mp_Ui->labelPrompt->setText(Text);
//    mp_Ui->contentLabel->setText(QString("%1").arg(Text));
}

/****************************************************************************/
/*!
 *  \brief Update label status
 *
 *  \iparam Status = Label test status
 */
/****************************************************************************/
void CPressureInputDialog::UpdateLabel(const Service::ModuleTestStatus &Status)
{

}


/****************************************************************************/
/*!
 *  \brief Filters all mouse events
 *
 *      There can be a small period of time in which the wait dialog is not
 *      displayed. This is required so that the wait dialog does not have to
 *      be displayed everytime network communication takes place.
 *
 *  \iparam p_Object = Object that is watched
 *  \iparam p_Event = Current event
 *
 *  \return True if an event should be filtered
 */
/****************************************************************************/
bool CPressureInputDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if ((p_Object == mp_Ui->lineEdit || p_Object == mp_Ui->lineEdit_2 )
            && p_Event->type() == QEvent::MouseButtonPress)
    {
        ConnectKeyBoardSignalSlots();
        mp_KeyBoardWidget->setModal(true);
        mp_KeyBoardWidget->SetKeyBoardDialogTitle(Service::CMessageString::MSG_TITLE_ENTER_VALVE);
        mp_KeyBoardWidget->SetPasswordMode(false);
        mp_KeyBoardWidget->SetValidation(true);
        mp_KeyBoardWidget->SetMinCharLength(1);
        mp_KeyBoardWidget->SetMaxCharLength(5);
        mp_KeyBoardWidget->DisplayNumericKeyBoard();
        // ^ and $ is used for any character. * is used to enter multiple characters
        // [0-9] is used to allow user to enter only 0 to 9 digits
        mp_KeyBoardWidget->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);
        mp_KeyBoardWidget->show();

        m_Timer->stop();
        return true;
    }
    if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(p_Object, p_Event);
    }
}

/****************************************************************************/
/*!
 *  \brief This function is called when OK is clicked
 *  \iparam EnteredString = Stores line edit string
 */
/****************************************************************************/
void CPressureInputDialog::OnOkClicked(QString EnteredString)
{
    mp_KeyBoardWidget->hide();

    QWidget *p_Obj = this->focusWidget();
    if (p_Obj == mp_Ui->lineEdit) {
        m_LineEditStringLeft = EnteredString.simplified();
        mp_Ui->lineEdit->setText(m_LineEditStringLeft);

        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(m_TestCaseId);
        DataManager::CTestCase *mp_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

        qreal OrigValue = mp_Ui->lineEdit->text().toDouble();

        qreal PressureLow = mp_TestCase->GetParameter("PressureLow").toDouble();
        qreal PressureHigh = mp_TestCase->GetParameter("PressureHigh").toDouble();
        if (OrigValue < PressureLow || OrigValue > PressureHigh) {
            // display success message
            MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(this);
            QString PromptText = Service::CMessageString::MSG_DIAGNOSTICS_USER_INPUT_PRESSURE;
            QString DisplayText = QString("%1 %2~%3 kpa").arg(PromptText).arg(PressureLow).arg(PressureHigh);
            dlg->SetIcon(QMessageBox::Warning);
            dlg->SetText(DisplayText);
            dlg->HideButtons();
            dlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_OK);

            (void)dlg->exec();
            delete dlg;

            mp_Ui->lineEdit->setFocus();

            mp_Ui->lineEdit->selectAll();
        }
        else {

            QTime DurationTime = QTime::fromString(mp_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
            m_WaitSeconds  = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second();
            qDebug()<<"Wait Seconds = "<<m_WaitSeconds;
            m_Timer->stop();
            m_Seconds = 0;
            m_Timer->start(1000);
        }
    }
    else if (p_Obj == mp_Ui->lineEdit_2) {
        m_LineEditStringMiddle = EnteredString.simplified();
        mp_Ui->lineEdit_2->setText(m_LineEditStringMiddle);

        mp_Ui->pushButtonOk->setDisabled(false);
    }

    DisconnectKeyBoardSignalSlots();
}

/****************************************************************************/
/*!
 *  \brief This function hides the keyboard when Esc is clicked
 */
/****************************************************************************/
void CPressureInputDialog::OnESCClicked()
{
    // Disconnect signals and slots connected to keyboard.
    DisconnectKeyBoardSignalSlots();
    mp_KeyBoardWidget->hide();
}


/****************************************************************************/
/*!
 *  \brief Connects signals and slots of keyboard.
 */
/****************************************************************************/
void CPressureInputDialog::ConnectKeyBoardSignalSlots()
{
    // Connect signals and slots to keyboard.
    CONNECTSIGNALSLOT(mp_KeyBoardWidget, OkButtonClicked(QString), this, OnOkClicked(QString));
    CONNECTSIGNALSLOT(mp_KeyBoardWidget, EscButtonClicked(), this, OnESCClicked());
}

/****************************************************************************/
/*!
 *  \brief Disconnects signals and slots of keyboard.
 */
/****************************************************************************/
void CPressureInputDialog::DisconnectKeyBoardSignalSlots()
{
    // Disconnect signals and slots connected to keyboard.
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(OkButtonClicked(QString)), this, SLOT(OnOkClicked(QString)));
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(EscButtonClicked()), this, SLOT(OnESCClicked()));

}

/****************************************************************************/
/*!
 *  \brief Abort dialog
 */
/****************************************************************************/
void CPressureInputDialog::AbortDialog()
{
    qreal OrigValue = mp_Ui->lineEdit->text().toFloat();
    qreal CurrentValue = mp_Ui->lineEdit_2->text().toFloat();

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(m_TestCaseId);
    DataManager::CTestCase *mp_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    qreal Departure = mp_TestCase->GetParameter("Departure").toDouble();

    qDebug() <<"qFabs="<<qFabs(OrigValue-CurrentValue)<<" Departure="<<Departure;

    if (qFabs(OrigValue-CurrentValue) <= Departure) {
        m_PassFlag = true;
    }
    else {
        m_PassFlag = false;
    }
    accept();
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CPressureInputDialog::RetranslateUI()
{

}

void CPressureInputDialog::OnTimeout()
{
    m_Seconds++;
    if (m_Seconds > m_WaitSeconds) {
        mp_Ui->lineEdit_2->setDisabled(false);
        m_Timer->stop();
        m_Seconds = 0;
        mp_Ui->lineEdit_2->setFocus();
    }
    else {
        QString Text = QTime(0, 0, m_Seconds).toString("hh:mm:ss");
        mp_Ui->labelTime->setText(Text);
    }
}

} // end namespace MainMenu
