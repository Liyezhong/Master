/****************************************************************************/
/*! \file USBKeyValidation.cpp
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-04-09
 *   $Author:  $ Soumya. D
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

#include "Core/Include/USBKeyValidator.h"

namespace Core {

const QString REGEXP_NUMERIC_VALIDATOR = "^[0-9]{1,4}$"; //!< Reg expression for the validator

CUSBKeyValidator::CUSBKeyValidator(Core::CStartup* StartUp) :
    mp_StartUp(StartUp)
{
    mp_MessageDlg = new MainMenu::CMessageDlg;
    mp_KeyBoardWidget = new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);
    mp_KeyBoardWidget->setModal(true);
    m_ServiceKey = QString(tr("1111"));
    m_ManufacturingKey = QString(tr("0000"));
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("ENTER PIN"));
    mp_KeyBoardWidget->SetPasswordMode(true);
    mp_KeyBoardWidget->SetValidation(true);
    mp_KeyBoardWidget->SetMinCharLength(4);
    mp_KeyBoardWidget->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);
    mp_KeyBoardWidget->DisplayNumericKeyBoard();
    //mp_KeyBoardWidget->show();
    ConnectKeyBoardSignalSlots();
    mp_StartUp->ServiceGuiInit();
}

CUSBKeyValidator::~CUSBKeyValidator()
{
    delete mp_KeyBoardWidget;
}

void CUSBKeyValidator::OnOkClicked(QString EnteredString)
{
    m_EnteredPin = EnteredString;
    mp_KeyBoardWidget->hide();
    //if (m_EnteredPin == m_ServiceKey)
    if (1)
    {
        mp_StartUp->ServiceGuiInit();
    }
    else if(m_EnteredPin == m_ManufacturingKey)
    {
        mp_StartUp->ManufacturingGuiInit();
    }
    else
    {
        mp_MessageDlg->SetTitle(QString(tr("Invalid Pin")));
        mp_MessageDlg->SetText(QString(tr("Invalid Pin, Cannot boot into Service/Manufacturing Software.")));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        mp_MessageDlg->Show();
    }

    DisconnectKeyBoardSignalSlots();
}

void CUSBKeyValidator::OnESCClicked()
{
    // Disconnect signals and slots connected to keyboard.
    DisconnectKeyBoardSignalSlots();
    mp_KeyBoardWidget->hide();
}

void CUSBKeyValidator::ConnectKeyBoardSignalSlots()
{
    // Connect signals and slots to keyboard.
    CONNECTSIGNALSLOT(mp_KeyBoardWidget, OkButtonClicked(QString), this, OnOkClicked(QString));
    CONNECTSIGNALSLOT(mp_KeyBoardWidget, EscButtonClicked(), this, OnESCClicked());
}

void CUSBKeyValidator::DisconnectKeyBoardSignalSlots()
{
    // Disconnect signals and slots connected to keyboard.
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(OkButtonClicked(QString)), this, SLOT(OnOkClicked(QString)));
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(EscButtonClicked()), this, SLOT(OnESCClicked()));

}

}   // end of namespace Core

