/****************************************************************************/
/*! \file NetworkSetting.cpp
 *
 *  \brief NetworkSetting implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-18
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

#include "ServiceUpdates/Include/NetworkSetting.h"

#include <QCheckBox>

#include "Global/Include/Utils.h"

#include "KeyBoard/Include/KeyBoard.h"

#include "ui_NetworkSetting.h"

namespace ServiceUpdates {

const QString REGEXP_NUMERIC_VALIDATOR = "^[0-9]*$";
const QString IPADDRESS_INPUT_MASK     = "000.000.000.000; ";
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CNetworkSetting::CNetworkSetting(QWidget *p_Parent)
    : MainMenu::CPanelFrame(p_Parent)
    , mp_Ui(new Ui::CNetworkSetting)
    , m_Password()
{
    mp_Ui->setupUi(GetContentFrame());

    SetPanelTitle(tr("Network"));

    mp_Ui->checkBox_RemoteCare->setChecked(true);

    CONNECTSIGNALSLOT(mp_Ui->checkBox_RemoteCare,
                      stateChanged(int),
                      this,
                      OnRemoteCareStateChanged(int));

    CONNECTSIGNALSLOT(mp_Ui->checkBox_DirectConnection,
                      stateChanged(int),
                      this,
                      OnDirectConnectionStateChanged(int));
    CONNECTSIGNALSLOT(mp_Ui->proxyUserNameButton,
                      clicked(void),
                      this,
                      OnProxyUserName(void));
    CONNECTSIGNALSLOT(mp_Ui->proxyPasswordButton,
                      clicked(void),
                      this,
                      OnProxyPassword(void));
    CONNECTSIGNALSLOT(mp_Ui->proxyIpAddressButton,
                      clicked(void),
                      this,
                      OnProxyIPAddress(void));
    CONNECTSIGNALSLOT(mp_Ui->proxyPortButton,
                      clicked(void),
                      this,
                      OnProxyPort(void));
    CONNECTSIGNALSLOT(mp_Ui->saveButton, clicked(void), this, OnSave(void));
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CNetworkSetting::~CNetworkSetting()
{
    try
    {
        delete mp_Ui;
    }
    catch (...) { }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CNetworkSetting::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);

    switch (p_Event->type())
    {
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
void CNetworkSetting::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSetting = p_UserSettings;

    InitDialog();
}

void CNetworkSetting::OnRemoteCareStateChanged(int State)
{
}

/****************************************************************************/
/*!
 *  \brief This slot is called when DirectConnection checkBox state is changed.
 */
/****************************************************************************/
void CNetworkSetting::OnDirectConnectionStateChanged(int State)
{
    if (Qt::Checked == State)
    {
        mp_Ui->proxyUserNameButton->setDisabled(true);
        mp_Ui->proxyPasswordButton->setDisabled(true);
        mp_Ui->proxyIpAddressButton->setDisabled(true);
        mp_Ui->proxyPortButton->setDisabled(true);
    }
    else
    {
        mp_Ui->proxyUserNameButton->setDisabled(false);
        mp_Ui->proxyPasswordButton->setDisabled(false);
        mp_Ui->proxyIpAddressButton->setDisabled(false);
        mp_Ui->proxyPortButton->setDisabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to Enter Proxy UserName
 */
/****************************************************************************/
void CNetworkSetting::OnProxyUserName(void)
{
    KeyBoard::CKeyBoard *p_KeyBoard =
           new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);

    p_KeyBoard->setModal(true);
    p_KeyBoard->SetPasswordMode(false);
    p_KeyBoard->SetMaxCharLength(16);
    p_KeyBoard->SetMinCharLength(1);

    p_KeyBoard->SetKeyBoardDialogTitle(tr("Enter Proxy User Name"));
    p_KeyBoard->SetLineEditContent(mp_Ui->proxyUserNameButton->text());

    CONNECTSIGNALSLOT(p_KeyBoard,
                      OkButtonClicked(QString),
                      this,
                      ChangeProxyUserName(QString));

    p_KeyBoard->exec();

    delete p_KeyBoard;
}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to Enter Proxy Password
 */
/****************************************************************************/
void CNetworkSetting::OnProxyPassword(void)
{
    KeyBoard::CKeyBoard *p_KeyBoard =
           new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);

    p_KeyBoard->setModal(true);
    p_KeyBoard->SetPasswordMode(true);
    p_KeyBoard->SetMaxCharLength(16);
    p_KeyBoard->SetMinCharLength(4);

    p_KeyBoard->SetKeyBoardDialogTitle(tr("Enter Proxy Password"));

    CONNECTSIGNALSLOT(p_KeyBoard,
                      OkButtonClicked(QString),
                      this,
                      ChangeProxyPassword(QString));

    p_KeyBoard->exec();

    delete p_KeyBoard;
}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to Enter Proxy IP Address
 */
/****************************************************************************/
void CNetworkSetting::OnProxyIPAddress(void)
{
    KeyBoard::CKeyBoard *p_KeyBoard =
           new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);

    p_KeyBoard->setModal(true);
    p_KeyBoard->SetPasswordMode(false);
    p_KeyBoard->SetMaxCharLength(16);
    p_KeyBoard->SetMinCharLength(2);

    p_KeyBoard->SetKeyBoardDialogTitle(tr("Enter Proxy IP Address"));
    p_KeyBoard->SetLineEditContent(mp_Ui->proxyIpAddressButton->text());
    p_KeyBoard->SetLineEditInputMask(IPADDRESS_INPUT_MASK);

    CONNECTSIGNALSLOT(p_KeyBoard,
                      OkButtonClicked(QString),
                      this,
                      ChangeProxyIPAddress(QString));

    p_KeyBoard->exec();

    delete p_KeyBoard;
}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to Enter Proxy Port number
 */
/****************************************************************************/
void CNetworkSetting::OnProxyPort(void)
{
    KeyBoard::CKeyBoard *p_KeyBoard =
           new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);

    p_KeyBoard->setModal(true);
    p_KeyBoard->SetPasswordMode(false);
    p_KeyBoard->SetMaxCharLength(16);
    p_KeyBoard->SetMinCharLength(2);

    p_KeyBoard->SetKeyBoardDialogTitle(tr("Enter Proxy Port"));
    p_KeyBoard->SetLineEditContent(mp_Ui->proxyPortButton->text());
    p_KeyBoard->SetValidation(true);
    p_KeyBoard->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);

    CONNECTSIGNALSLOT(p_KeyBoard,
                      OkButtonClicked(QString),
                      this,
                      ChangeProxyPort(QString));

    p_KeyBoard->exec();

    delete p_KeyBoard;
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Save button is clicked.
 */
/****************************************************************************/
void CNetworkSetting::OnSave(void)
{
    if (0 != mp_UserSetting)
    {
        if (mp_Ui->checkBox_RemoteCare->isChecked())
        {
            mp_UserSetting->SetRemoteCare(Global::ONOFFSTATE_ON);
        }
        else
        {
            mp_UserSetting->SetRemoteCare(Global::ONOFFSTATE_OFF);
        }

        if (mp_Ui->checkBox_DirectConnection->isChecked())
        {
            mp_UserSetting->SetDirectConnection(Global::ONOFFSTATE_ON);
        }
        else
        {
            mp_UserSetting->SetDirectConnection(Global::ONOFFSTATE_OFF);
        }

        mp_UserSetting->SetProxyUserName(mp_Ui->proxyUserNameButton->text());
        mp_UserSetting->SetProxyPassword(m_Password);
        mp_UserSetting->SetProxyIPAddress(mp_Ui->proxyIpAddressButton->text());
        mp_UserSetting->SetProxyIPPort(mp_Ui->proxyPortButton->text().toInt());

        emit UserSettingsChanged(*mp_UserSetting);
    }
}

void CNetworkSetting::ChangeProxyUserName(QString UserName)
{
    mp_Ui->proxyUserNameButton->setText(UserName);
}

void CNetworkSetting::ChangeProxyPassword(QString Password)
{
    m_Password = Password;

    int PasswordLength = m_Password.count();
    QString PasswordString;
    for (int i = 0; i < PasswordLength; i++) {
        PasswordString  = PasswordString + "*";
    }
    mp_Ui->proxyPasswordButton->setText(PasswordString);
}

void CNetworkSetting::ChangeProxyIPAddress(QString Address)
{
    mp_Ui->proxyIpAddressButton->setText(Address);
}

void CNetworkSetting::ChangeProxyPort(QString Port)
{
    mp_Ui->proxyPortButton->setText(Port);
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CNetworkSetting::RetranslateUI(void)
{
    SetPanelTitle(tr("Network"));

    mp_Ui->checkBox_RemoteCare->setText(tr("Enable Remote Care"));
    mp_Ui->checkBox_DirectConnection->setText(tr("Direct Connection"));
    mp_Ui->proxyUserNameLabel->setText(tr("Proxy User Name"));
    mp_Ui->proxyPasswordLabel->setText(tr("Proxy Password"));
    mp_Ui->proxyIpAddressLabel->setText(tr("Proxy IP Address"));
    mp_Ui->proxyPortLabel->setText(tr("Proxy Port"));

    mp_Ui->saveButton->setText(tr("Save"));
}

void CNetworkSetting::InitDialog(void)
{
    if (0 != mp_UserSetting)
    {
        if (Global::ONOFFSTATE_ON == mp_UserSetting->GetRemoteCare())
        {
            mp_Ui->checkBox_RemoteCare->setChecked(true);
        }
        else
        {
            mp_Ui->checkBox_RemoteCare->setChecked(false);
        }

        if (Global::ONOFFSTATE_ON == mp_UserSetting->GetDirectConnection())
        {
            mp_Ui->checkBox_DirectConnection->setChecked(true);
        }
        else
        {
            mp_Ui->checkBox_DirectConnection->setChecked(false);
        }

        mp_Ui->proxyUserNameButton->setText(mp_UserSetting->GetProxyUserName());

        m_Password = mp_UserSetting->GetProxyPassword();

        int PasswordLength = m_Password.count();
        QString PasswordString;
        for (int i = 0; i < PasswordLength; i++) {
            PasswordString  = PasswordString + "*";
        }
        mp_Ui->proxyPasswordButton->setText(PasswordString);

        mp_Ui->proxyIpAddressButton->setText(
                    mp_UserSetting->GetProxyIPAddress());

        mp_Ui->proxyPortButton->setText(QString::number(
                                            mp_UserSetting->GetProxyIPPort()));
    }
}

} // end namespace ServiceUpdates
