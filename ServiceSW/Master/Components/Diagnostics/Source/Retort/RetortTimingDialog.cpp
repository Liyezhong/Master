/****************************************************************************/
/*! \file RetortTimingDialog.cpp
 *
 *  \brief Declaration of Retort Timing Dialog.
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
#include "Diagnostics/Include/Retort/RetortTimingDialog.h"
#include "ui_RetortTimingDialog.h"

RetortTimingDialog::RetortTimingDialog(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::RetortTimingDialog)
{
    ui->setupUi(this);
    connect(ui->abort, SIGNAL(clicked()), this, SLOT(reject()));
}

RetortTimingDialog::~RetortTimingDialog()
{
    delete ui;
}

void RetortTimingDialog::SetTitle(QString Title)
{
    this->SetDialogTitle(Title);
}

void RetortTimingDialog::UpdateLabel(const Service::ModuleTestStatus &Status)
{
    ui->labelUsedTime->setText(Status.value("UsedTime"));
    ui->labelDuration->setText(Status.value("Duration"));
    ui->labelTargetTemp->setText(Status.value("TargetTemp"));
    ui->labelCurTempSide->setText(Status.value("CurrentTempSide"));
    ui->labelCurTempBottom1->setText(Status.value("CurrentTempBottom1"));
    ui->labelCurTempBottom2->setText(Status.value("CurrentTempBottom2"));
}
