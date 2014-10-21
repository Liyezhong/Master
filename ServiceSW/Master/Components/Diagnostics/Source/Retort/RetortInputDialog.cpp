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
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(accept()));
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
    if (!ui->lineEdit->text().length() || !ui->lineEdit_2->text().length())
        return 1;
    if (ui->lineEdit->text() != ui->lineEdit_2->text())
        return 2;

    text = ui->lineEdit->text();

    return 0;
}
