#include "Dashboard/Include/ProgramRunningPanelWidget.h"
#include "ui_ProgramRunningPanelWidget.h"

namespace Dashboard
{
CProgramRunningPanelWidget::CProgramRunningPanelWidget(QWidget *parent):
    CIconTitlePanelFrame(parent),
    ui(new Ui::CProgramRunningPanelWidget)
{
    ui->setupUi(GetContentFrame());
    this->SetPanelIcon(":/HimalayaImages/Icons/MISC/LeicaLogo.png");//get the current program icon
    SetPanelTitle(tr("Test"));//get the current program name
}

CProgramRunningPanelWidget::~CProgramRunningPanelWidget()
{
    try {
            delete ui;
        }
        catch (...) {
            // to please Lint.
        }
}

}
