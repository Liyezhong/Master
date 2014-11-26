/****************************************************************************/
/*! \file DiagnosticsMessageDlg.cpp
 *
 *  \brief Implementation of Diagnostics message dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-29
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

#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include <QDebug>

namespace Diagnostics {

CDiagnosticMessageDlg::CDiagnosticMessageDlg(QWidget *p_Parent):
    mp_Parent(p_Parent)
{
    mp_WaitDlg = NULL;
}

CDiagnosticMessageDlg::~CDiagnosticMessageDlg()
{
    try {
        if (mp_WaitDlg) {
            mp_WaitDlg->hide();
            delete mp_WaitDlg;
            mp_WaitDlg = NULL;
        }
    }
    catch (...) {

    }
}

void CDiagnosticMessageDlg::ShowMessage(QString& MessageTitle, QString& MessageText, ErrorCode_t Ret)
{
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);
    dlg->SetTitle(MessageTitle);
    if (Ret == RETURN_OK || Ret == RETURN_ABORT) {
        dlg->SetIcon(QMessageBox::Information);
    }
    else {
        dlg->SetIcon(QMessageBox::Critical);
    }
    dlg->SetText(MessageText);
    dlg->HideButtons();
    if (Ret == RETURN_ABORT) {
        dlg->SetButtonText(1, "Abort");
    }
    else {
        dlg->SetButtonText(1, "OK");
    }
    dlg->setModal(true);
    qDebug()<<MessageText;

    (void)dlg->exec();
    delete dlg;
}

void CDiagnosticMessageDlg::ShowWaitingDialog(QString& MessageTitle, QString& MessageText)
{
    qDebug()<<"CDiagnosticMessageDlg::ShowWaitingDialog title="<<MessageTitle<<" MessageText="<<MessageText;

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

void CDiagnosticMessageDlg::HideWaitingDialog()
{
    qDebug()<<"CDiagnosticMessageDlg::HideWaitingDialog........";

    if (mp_WaitDlg) {
        mp_WaitDlg->hide();
        delete mp_WaitDlg;
        mp_WaitDlg = NULL;
    }
}

int CDiagnosticMessageDlg::ShowConfirmMessage(QString& MessageTitle, QString& MessageText, BUTTON_TYPE type)
{
    qDebug()<<"CDiagnosticMessageDlg::ShowConfirmMessage title="<<MessageTitle<<"  MessageText="<<MessageText;

    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);
    dlg->SetTitle(MessageTitle);
    dlg->SetIcon(QMessageBox::Question);
    dlg->SetText(MessageText);
    dlg->HideCenterButton();

    switch (type) {
    case YES_NO:
        dlg->SetButtonText(1, "Yes");
        dlg->SetButtonText(3, "No");
        break;
    case OK_ABORT:
        dlg->SetButtonText(1, "Ok");
        dlg->SetButtonText(3, "Abort");
        break;
    case NEXT_CANCEL:
        dlg->SetButtonText(1, "Next");
        dlg->SetButtonText(3, "Cancel");
        break;
    case NEXT_CANCEL_DISABLE:
        dlg->SetButtonText(1, "Next");
        dlg->SetButtonText(3, "Cancel");
        dlg->EnableButton(3, false);
        break;
    }

    dlg->setModal(true);
    qDebug()<<MessageText;

    int Ret = dlg->exec();
    delete dlg;

    return Ret;
}

void CDiagnosticMessageDlg::ShowRVMoveFailedDlg(QString& Title)
{
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);
    dlg->SetTitle(Title);
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText("Rotating Rotary Valve failed.");
    dlg->HideButtons();
    dlg->SetButtonText(1, "OK");
    dlg->setModal(true);
    //qDebug()<<MessageText;

    (void)dlg->exec();

    delete dlg;
}

}
