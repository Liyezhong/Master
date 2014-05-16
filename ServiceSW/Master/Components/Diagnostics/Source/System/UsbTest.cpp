/****************************************************************************/
/*! \file UsbTest.cpp
 *
 *  \brief Implementation of USB test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
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

#include "Diagnostics/Include/System/UsbTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgConfirmationStatus.h"
#include "Diagnostics/Include/System/DlgUsbStatus.h"


namespace Diagnostics {

namespace System {

CUsbTest::CUsbTest(void)
    : CTestBase()
{
}

CUsbTest::~CUsbTest(void)
{
}

void CUsbTest::Run(void)
{
    qDebug() << "USB Test starts!";

    this->FirstConnectUSB();
}

void CUsbTest::FirstConnectUSB(void)
{
    qDebug() << "USB Test: first connect the USB stick!";

    // inform the customer to connect the USB stick
    MainMenu::CDlgWizardText *dlg =
            new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("USB Test"));
    dlg->SetText(tr("Please plug in a USB stick!"));

    CONNECTSIGNALSLOT(dlg, accepted(), this, FirstCheckStatus());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CUsbTest::FirstCheckStatus(void)
{
    qDebug() << "USB Test: second check the USB status!";

    /// \todo: check the USB connection ***************************************/
    /// \todo: read the files in the USB stick ********************************/

    // inform the customer to connect bottle at Position
    CDlgUsbStatus *dlg = new CDlgUsbStatus;
    dlg->SetDialogTitle(tr("USB Test"));
    dlg->SetText(tr("Does the USB status show \"Plugged\" "
                    "and the files in the USB device is displayed?"));
    dlg->SetStatus(tr("USB status: Plugged")); /// \todo: change status here **/
    QStringList listTemp = QStringList()
            << "1111111111"
            << "222222222222"
            << "333333333333"
            << "444444444444"
            << "555555555555"
            << "66666666666"
            << "77777777777"
            << "88888888888";
    dlg->SetFiles(listTemp); /// \todo: change file list here *****************/

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondDisconnectUSB());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail());

    dlg->exec();

    delete dlg;
}

void CUsbTest::SecondDisconnectUSB(void)
{
    qDebug() << "USB Test: start bottle checking!";

    // inform the customer to disconnect the USB stick
    MainMenu::CDlgWizardText *dlg =
            new MainMenu::CDlgWizardText;
    dlg->SetDialogTitle(tr("USB Test"));
    dlg->SetText(tr("Please unplug the USB stick!"));

    CONNECTSIGNALSLOT(dlg, accepted(), this, SecondCheckStatus());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Cancel());

    dlg->exec();

    delete dlg;
}

void CUsbTest::SecondCheckStatus(void)
{
    qDebug() << "USB Test: second check the USB status!";

    /// \todo: check the USB connection ***************************************/

    // inform the customer to connect bottle at Position
    MainMenu::CDlgConfirmationStatus *dlg =
            new MainMenu::CDlgConfirmationStatus;
    dlg->SetDialogTitle(tr("USB Test"));
    dlg->SetText(tr("Does the USB status show \"Unplugged\"?"));
    dlg->SetStatus(tr("USB status: Unplugged")); /// \todo change status here */

    CONNECTSIGNALSLOT(dlg, accepted(), this, Succeed());
    CONNECTSIGNALSLOT(dlg, rejected(), this, Fail());

    dlg->exec();

    delete dlg;
}

void CUsbTest::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_USB_TEST_SUCCESS);
    qDebug() << "USB Test succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("USB Test"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("USB Test SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CUsbTest::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_USB_TEST_FAILURE);
    qDebug() << "USB Test failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("USB Test"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("USB Test FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CUsbTest::Cancel(void)
{
    qDebug() << "USB Test canceled!";
}

} // namespace System

} // namespace Diagnostics
