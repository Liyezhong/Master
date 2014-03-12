/****************************************************************************/
/*! \file DlgWizardInputBottlePosition.cpp
 *
 *  \brief Implementation of input-bottle-position dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "ServiceWidget/Include/DlgWizardInputBottlePosition.h"

namespace MainMenu {

const QString REGEXP_NUMERIC_VALIDATOR = "^[0-9]{1,2}$"; //!< Reg expression for the validator

CDlgWizardInputBottlePosition::CDlgWizardInputBottlePosition(QWidget *parent)
    : CDlgWizardBase(parent)
{
    mp_Text = new QLabel(this);
    mp_GroupInput = new QGroupBox(this);
    mp_LayoutInput = new QHBoxLayout(mp_GroupInput);
    mp_ButtonLabel = new QLabel(mp_GroupInput);
    mp_ButtonInput = new QPushButton(mp_GroupInput);

    mp_LayoutInput->addWidget(mp_ButtonLabel);
    mp_LayoutInput->addWidget(mp_ButtonInput);
    mp_GroupInput->setLayout(mp_LayoutInput);

    this->AddWidget(mp_Text);
    this->AddWidget(mp_GroupInput);

    // disable button "Next"
    this->DisableNext();

    // create keyboard
    mp_KeyBoard =
           new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);
    mp_KeyBoard->setModal(true);
    mp_KeyBoard->SetPasswordMode(false);

    CONNECTSIGNALSLOT(mp_ButtonInput, clicked(), this, OpenKeyBoard() );

    CONNECTSIGNALSLOT(mp_KeyBoard,
                      OkButtonClicked(QString),
                      this,
                      InputPosition(QString));

    CONNECTSIGNALSLOT(this, accepted(), this, EmitPosition() );
}

CDlgWizardInputBottlePosition::~CDlgWizardInputBottlePosition(void)
{
    try
    {
        delete mp_KeyBoard;

        delete mp_ButtonInput;
        delete mp_ButtonLabel;
        delete mp_LayoutInput;
        delete mp_GroupInput;
        delete mp_Text;
    }
    catch (...)
    { }
}

void CDlgWizardInputBottlePosition::SetText(const QString &Text)
{
    mp_Text->setText(Text);
}

void CDlgWizardInputBottlePosition::SetButtonLabel(const QString &Label)
{
    mp_ButtonLabel->setText(Label);
}

void CDlgWizardInputBottlePosition::OpenKeyBoard(void)
{
    mp_KeyBoard->SetKeyBoardDialogTitle(mp_ButtonLabel->text());
    mp_KeyBoard->SetLineEditContent(mp_ButtonInput->text());
    mp_KeyBoard->SetValidation(true);
    mp_KeyBoard->SetMinCharLength(1);
    mp_KeyBoard->SetMaxCharLength(2);
    mp_KeyBoard->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);
    mp_KeyBoard->DisplayNumericKeyBoard();

    mp_KeyBoard->show();
}

void CDlgWizardInputBottlePosition::InputPosition(QString Position)
{
    bool Result = false;
    m_Position = Position.toInt(&Result);

    if (Result)
    {
        this->EnableNext();
    }
    else
    {
        this->DisableNext();
    }

    mp_KeyBoard->hide();

    mp_ButtonInput->setText(QString::number(m_Position));
}

void CDlgWizardInputBottlePosition::EmitPosition(void)
{
    emit AcceptPosition(m_Position);
}

} // namespace MainMenu
