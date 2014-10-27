/****************************************************************************/
/*! \file LaTimingDialog.cpp
 *
 *  \brief Declaration of L & A Timing Dialog.
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
#include "Diagnostics/Include/LaSystem/LaTimingDialog.h"
#include "ui_LaTimingDialog.h"

LaTimingDialog::LaTimingDialog(QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent),
    ui(new Ui::LaTimingDialog)
{
    ui->setupUi(this);
    (void)connect(ui->abort, SIGNAL(clicked()), this, SLOT(reject()));
}

LaTimingDialog::~LaTimingDialog()
{
    delete ui;
}

void LaTimingDialog::SetTitle(QString Title)
{
    this->SetDialogTitle(Title);
}

void LaTimingDialog::UpdateLabel(const Service::ModuleTestStatus &Status)
{
    ui->labelUsedTime->setText(Status.value("UsedTime"));
    ui->labelDuration->setText(Status.value("Duration"));
    ui->labelTargetTemp->setText(Status.value("TargetTemp"));
    ui->labelCurrentTemp->setText(Status.value("CurrentTemp"));
}
