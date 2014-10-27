/****************************************************************************/
/*! \file RetortInputDialog.cpp
 *
 *  \brief Declaration of Retort Input Dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-13
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include "Diagnostics/Include/Retort/RetortInputDialog.h"
#include "ui_RetortInputDialog.h"

//lint -e1512
//lint -e1527
const QString REGEXP_NUMERIC_VALIDATOR  = "^[0-9.]{1,5}$"; //!< Reg expression for the validator

RetortInputDialog::RetortInputDialog(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::RetortInputDialog)
{
    ui->setupUi(this);
    setModal(true);
    (void)connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOk()));

    mp_KeyBoardWidget = new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
}

RetortInputDialog::~RetortInputDialog()
{
    try {
        delete mp_KeyBoardWidget;
        delete ui;
    } catch (...) {
    }
}

void RetortInputDialog::SetTitle(QString Title)
{
    this->SetDialogTitle(Title);
    _title = Title;
}

void RetortInputDialog::getEdit(QString &text)
{
    text = ui->lineEdit->text();
}

void RetortInputDialog::clickOk()
{
    if (!ui->lineEdit->text().length() || !ui->lineEdit_2->text().length()) {
        ShowMessage(tr("Retort Heating Test (with Water)"), tr("Edit box cannot be empty!"));
        return;
    }
    if (ui->lineEdit->text() != ui->lineEdit_2->text()) {
        ShowMessage(tr("Retort Heating Test (with Water)"),tr("The input value is different!"));
        return;
    }

    accept();
}

void RetortInputDialog::ShowMessage(QString title, QString text)
{
    MainMenu::CMessageDlg dlg(this);
    dlg.SetTitle(title);

    dlg.SetIcon(QMessageBox::Critical);

    dlg.SetText(text);
    dlg.HideButtons();
    dlg.SetButtonText(1, "OK");
    dlg.setModal(true);

    (void)dlg.exec();
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
bool RetortInputDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if ((p_Object == ui->lineEdit || p_Object == ui->lineEdit_2)
            && p_Event->type() == QEvent::MouseButtonPress)
    {
        ConnectKeyBoardSignalSlots();
        mp_KeyBoardWidget->setModal(true);
        mp_KeyBoardWidget->SetKeyBoardDialogTitle(_title);
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
    //return false;
}


/****************************************************************************/
/*!
 *  \brief This function hides the keyboard when Esc is clicked
 */
/****************************************************************************/
void RetortInputDialog::OnESCClicked()
{
    // Disconnect signals and slots connected to keyboard.
    DisconnectKeyBoardSignalSlots();
    mp_KeyBoardWidget->hide();
}


/****************************************************************************/
/*!
 *  \brief This function is called when OK is clicked
 *  \iparam EnteredString = Stores line edit string
 */
/****************************************************************************/
void RetortInputDialog::OnOkClicked(QString EnteredString)
{
    mp_KeyBoardWidget->hide();

    QWidget *p_Obj = this->focusWidget();
    if (p_Obj == ui->lineEdit) {
        m_LineEditString = EnteredString.simplified();
        ui->lineEdit->setText(m_LineEditString);
    }
    else if (p_Obj == ui->lineEdit_2) {
        m_LineEditString = EnteredString.simplified();
        ui->lineEdit_2->setText(m_LineEditString);
    }

    DisconnectKeyBoardSignalSlots();
}


/****************************************************************************/
/*!
 *  \brief Connects signals and slots of keyboard.
 */
/****************************************************************************/
void RetortInputDialog::ConnectKeyBoardSignalSlots()
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
void RetortInputDialog::DisconnectKeyBoardSignalSlots()
{
    // Disconnect signals and slots connected to keyboard.
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(OkButtonClicked(QString)), this, SLOT(OnOkClicked(QString)));
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(EscButtonClicked()), this, SLOT(OnESCClicked()));

}
