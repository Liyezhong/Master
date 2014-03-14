/****************************************************************************/
/*! \file DataManagement.cpp
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-08
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

#include "ServiceUpdates/Include/DataManagement.h"

#include "Global/Include/Utils.h"

#include "ui_DataManagement.h"

namespace ServiceUpdates {

CDataManagement::CDataManagement(QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CDataManagement)
    , mp_WaitDialog(new MainMenu::CWaitDialog)
    , mp_MessageDlg(new MainMenu::CMessageDlg)
{
    mp_Ui->setupUi(this);

    CONNECTSIGNALSLOT(mp_Ui->exportBtn,
                      clicked(),
                      this,
                      Export());
    CONNECTSIGNALSLOT(mp_Ui->importBtn,
                      clicked(),
                      this,
                      Import());

    mp_WaitDialog->HideAbort();
    mp_WaitDialog->setModal(true);

    mp_MessageDlg->SetButtonText(1, tr("Ok"));
    mp_MessageDlg->setModal(true);
    mp_MessageDlg->HideButtons();
}

CDataManagement::~CDataManagement(void)
{
    try
    {
        delete mp_MessageDlg;

        delete mp_WaitDialog;

        delete mp_Ui;
    }
    catch (...) { }

}

void CDataManagement::Export(void)
{
    mp_WaitDialog->SetDialogTitle(tr("Service Export"));
    mp_WaitDialog->SetText(tr("Exporting data ..."));
    mp_WaitDialog->SetTimeout(10000);

    mp_MessageDlg->SetIcon(QMessageBox::Critical);
    mp_MessageDlg->SetTitle(tr("Error"));
    mp_MessageDlg->SetText(tr("Export failed!"));

    (void)connect(mp_WaitDialog, SIGNAL(Timeout()), mp_MessageDlg, SLOT(Show()));

    mp_WaitDialog->show();

    emit ServiceImportExportRequested(false);
}

void CDataManagement::Import(void)
{
    mp_WaitDialog->SetDialogTitle(tr("Import"));
    mp_WaitDialog->SetText(tr("Importing data ..."));
    mp_WaitDialog->SetTimeout(10000);

    mp_MessageDlg->SetIcon(QMessageBox::Critical);
    mp_MessageDlg->SetTitle(tr("Error"));
    mp_MessageDlg->SetText(tr("Import failed!"));

    (void)connect(mp_WaitDialog, SIGNAL(Timeout()), mp_MessageDlg, SLOT(Show()));

    mp_WaitDialog->show();

    emit ServiceImportExportRequested(true);
}

void CDataManagement::ExportFinished(bool Failed)
{
    qDebug() << "CDataManagement::ExportFinished";

    mp_WaitDialog->accept();

    if (Failed)
    {
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        mp_MessageDlg->SetTitle(tr("Error"));
        mp_MessageDlg->SetText(tr("Export failed!"));
    }
    else
    {
        mp_MessageDlg->SetIcon(QMessageBox::Information);
        mp_MessageDlg->SetTitle(tr("Information"));
        mp_MessageDlg->SetText(tr("Export Succeeds!"));
    }

    mp_MessageDlg->show();
}

void CDataManagement::ImportFinished(bool Failed)
{
    qDebug() << "CDataManagement::ImportFinished";

    mp_WaitDialog->accept();

    if (Failed)
    {
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        mp_MessageDlg->SetTitle(tr("Error"));
        mp_MessageDlg->SetText(tr("Import failed!"));
    }
    else
    {
        mp_MessageDlg->SetIcon(QMessageBox::Information);
        mp_MessageDlg->SetTitle(tr("Information"));
        mp_MessageDlg->SetText(tr("Import Succeeds!"));
    }

    mp_MessageDlg->show();
}

} // end namespace ServiceUpdates
