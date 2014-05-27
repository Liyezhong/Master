/****************************************************************************/
/*! \file UserInputDialog.cpp
 *
 *  \brief Implementation of file for class UserInputDialog.
 *
 *  \b Description:
 *          This class implements a base widget for displaying the wait dialogs.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-27
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
#include "DiagnosticsManufacturing/Include/UserInputDialog.h"
#include "ui_UserInputDialog.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Core/Include/CMessageString.h"
#include <QDebug>

namespace DiagnosticsManufacturing {

const QString REGEXP_NUMERIC_VALIDATOR  = "^[0-9.]{1,5}$"; //!< Reg expression for the validator

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CUserInputDialog::CUserInputDialog(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CUserInputDialog),
    m_LineEditString("")
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    this->SetDialogTitle("User Input");

    mp_Ui->InputValue->installEventFilter(this);

    mp_KeyBoardWidget = new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);

    CONNECTSIGNALSLOTGUI(mp_Ui->pushButtonOk, clicked(), this, AbortDialog());

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CUserInputDialog::~CUserInputDialog()
{
    try {
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
void CUserInputDialog::SetText(QString Text)
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
void CUserInputDialog::UpdateLabel(const Service::ModuleTestStatus &Status)
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
bool CUserInputDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if (p_Object == mp_Ui->InputValue && p_Event->type() == QEvent::MouseButtonPress)
    {
        ConnectKeyBoardSignalSlots();
        mp_KeyBoardWidget->setModal(true);
        mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("DiagnosticsManufacturing::CUserInputDialog",
                                                           "Enter Value", 0, QApplication::UnicodeUTF8));
        mp_KeyBoardWidget->SetPasswordMode(false);
        mp_KeyBoardWidget->SetValidation(true);
        mp_KeyBoardWidget->SetMinCharLength(1);
        mp_KeyBoardWidget->SetMaxCharLength(5);
        mp_KeyBoardWidget->DisplayNumericKeyBoard();
        // ^ and $ is used for any character. * is used to enter multiple characters
        // [0-9] is used to allow user to enter only 0 to 9 digits
        mp_KeyBoardWidget->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);
        mp_KeyBoardWidget->show();
        return true;
    }
    if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(p_Object, p_Event);
    }
    return false;
}

/****************************************************************************/
/*!
 *  \brief This function is called when OK is clicked
 *  \iparam EnteredString = Stores line edit string
 */
/****************************************************************************/
void CUserInputDialog::OnOkClicked(QString EnteredString)
{
    mp_KeyBoardWidget->hide();

    m_LineEditString = EnteredString.simplified();

    mp_Ui->InputValue->setText(m_LineEditString);

    DisconnectKeyBoardSignalSlots();
}

/****************************************************************************/
/*!
 *  \brief This function hides the keyboard when Esc is clicked
 */
/****************************************************************************/
void CUserInputDialog::OnESCClicked()
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
void CUserInputDialog::ConnectKeyBoardSignalSlots()
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
void CUserInputDialog::DisconnectKeyBoardSignalSlots()
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
void CUserInputDialog::AbortDialog()
{
    if (m_LineEditString == "" || m_LineEditString.at(0)=='.') {
        // display success message
        MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(this);
        //dlg->SetTitle(TestCaseDescription);
        dlg->SetIcon(QMessageBox::Warning);
        dlg->SetText(QApplication::translate("DiagnosticsManufacturing::CUserInputDialog",
                                             "Please enter the right value", 0, QApplication::UnicodeUTF8));
        dlg->HideButtons();
        dlg->SetButtonText(1, tr("Ok"));

        dlg->exec();
        delete dlg;
    }
    else {
        accept();
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CUserInputDialog::RetranslateUI()
{

}

} // end namespace MainMenu
