/****************************************************************************/
/*! \file InitialSystem.cpp
 *
 *  \brief Implementation of Initial system check.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-10
 *   $Author:  $ Dixiong Li
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

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

CTestBase::CTestBase(QWidget *parent)
    :mp_Parent(parent)
{
    mp_WaitDlg = NULL;
}

CTestBase::~CTestBase()
{
    if (mp_WaitDlg) {
        mp_WaitDlg->hide();
        delete mp_WaitDlg;
        mp_WaitDlg = NULL;
    }
}

void CTestBase::ShowMessage(QString &MessageTitle, QString &MessageText, ErrorCode_t Ret)
{
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);
    dlg->SetTitle(MessageTitle);
    if (Ret == RETURN_OK) {
        dlg->SetIcon(QMessageBox::Information);
    }
    else {
        dlg->SetIcon(QMessageBox::Critical);
    }
    dlg->SetText(MessageText);
    dlg->HideButtons();
    dlg->SetButtonText(1, "OK");
    dlg->setModal(true);
    qDebug()<<MessageText;

    dlg->exec();

    delete dlg;
}

void CTestBase::ShowWaitingDialog(QString &MessageTitle, QString &MessageText)
{
    qDebug()<<"CTestBase::ShowWaitingDialog title="<<MessageTitle<<" MessageText="<<MessageText;

    if (mp_WaitDlg) {
        mp_WaitDlg->hide();
        delete mp_WaitDlg;
        mp_WaitDlg = NULL;
    }

    mp_WaitDlg = new MainMenu::CMessageDlg(mp_Parent);
    mp_WaitDlg->SetTitle(MessageTitle);
    mp_WaitDlg->SetIcon(QMessageBox::Information);

    mp_WaitDlg->SetText(MessageText);
    mp_WaitDlg->HideAllButtons();
    mp_WaitDlg->setModal(true);
    mp_WaitDlg->show();
}

void CTestBase::HideWaitingDialog()
{
    qDebug()<<"CTestBase::HideWaitingDialog........";

    if (mp_WaitDlg) {
        mp_WaitDlg->hide();
        delete mp_WaitDlg;
        mp_WaitDlg = NULL;
    }
}

int CTestBase::ShowConfirmMessage(QString& MessageTitle, QString& MessageText)
{
    qDebug()<<"CTestBase::ShowConfirmMessage title="<<MessageTitle<<"  MessageText="<<MessageText;

    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);
    dlg->SetTitle(MessageTitle);
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(MessageText);
    dlg->HideCenterButton();
    dlg->SetButtonText(1, "Yes");
    dlg->SetButtonText(3, "No");
    dlg->setModal(true);
    qDebug()<<MessageText;

    int Ret = dlg->exec();

    delete dlg;

    return Ret;
}

} // namespace Diagnostics



