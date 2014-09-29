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
    if (mp_WaitDlg) {
        mp_WaitDlg->hide();
        delete mp_WaitDlg;
        mp_WaitDlg = NULL;
    }
}

void CDiagnosticMessageDlg::ShowMessage(QString& MessageTitle, QString& MessageText, ErrorCode_t Ret)
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
    dlg = NULL;
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

int CDiagnosticMessageDlg::ShowConfirmMessage(QString& MessageTitle, QString& MessageText)
{
    qDebug()<<"CDiagnosticMessageDlg::ShowConfirmMessage title="<<MessageTitle<<"  MessageText="<<MessageText;

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
    dlg = NULL;

    return Ret;
}

}
