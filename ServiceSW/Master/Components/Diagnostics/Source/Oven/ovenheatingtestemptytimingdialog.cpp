#include "Diagnostics/Include/Oven/ovenheatingtestemptytimingdialog.h"
#include "ui_ovenheatingtestemptytimingdialog.h"

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
    ui->labelCurTempTop->setText(Status.value("CurrentTempTop"));
    ui->labelCurTempBottom1->setText(Status.value("CurrentTempBottom1"));
    ui->labelCurTempBottom2->setText(Status.value("CurrentTempBottom2"));
    ui->labelDuration->setText(Status.value("Duration"));
    ui->labelTargetTemp->setText(Status.value("TargetTempTop"));
    ui->labelTargetTempName_bottom->setText(Status.value("TargetTempBottom"));
}
