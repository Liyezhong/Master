/****************************************************************************/
/*! \file OvenHeatingTestEmptyTimingDialog.cpp
 *
 *  \brief Declaration of Heating test empty Timing Dialog.
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
#include "Diagnostics/Include/Oven/OvenHeatingTestEmptyTimingDialog.h"
#include "ui_OvenHeatingTestEmptyTimingDialog.h"

OvenHeatingTestEmptyTimingDialog::OvenHeatingTestEmptyTimingDialog(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::OvenHeatingTestEmptyTimingDialog)
{
    ui->setupUi(this);
    connect(ui->abort, SIGNAL(clicked()), this, SLOT(reject()));
}

OvenHeatingTestEmptyTimingDialog::~OvenHeatingTestEmptyTimingDialog()
{
    delete ui;
}

void OvenHeatingTestEmptyTimingDialog::SetTitle(QString Title)
{
    this->SetDialogTitle(Title);
}

void OvenHeatingTestEmptyTimingDialog::UpdateLabel(const Service::ModuleTestStatus &Status)
{
    ui->labelUsedTime->setText(Status.value("UsedTime"));
    ui->labelDuration->setText(Status.value("Duration"));

    ui->labelTargetTemp->setText(Status.value("TargetTempTopRange"));

    ui->labelCurTempTop->setText(Status.value("CurrentTempTop"));
    ui->labelCurTempBottom1->setText(Status.value("CurrentTempBottom1"));
    ui->labelCurTempBottom2->setText(Status.value("CurrentTempBottom2"));
}
