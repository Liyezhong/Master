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

RetortInputDialog::RetortInputDialog(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::RetortInputDialog)
{
    ui->setupUi(this);
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(clickOk()));
}

RetortInputDialog::~RetortInputDialog()
{
    delete ui;
}

void RetortInputDialog::SetTitle(QString Title)
{
    this->SetDialogTitle(Title);
}

int RetortInputDialog::getEdit(QString &text)
{

    text = ui->lineEdit->text();

    return 0;
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
