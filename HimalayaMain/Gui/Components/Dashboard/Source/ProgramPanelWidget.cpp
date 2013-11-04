#include "Dashboard/Include/ProgramPanelWidget.h"
#include "ui_ProgramPanelWidget.h"

namespace Dashboard {
CProgramPanelWidget::CProgramPanelWidget(QWidget *parent) :
    MainMenu::CPanelFrame(parent),
    ui(new Ui::CProgramPanelWidget)
{
    ui->setupUi(this);
}

CProgramPanelWidget::~CProgramPanelWidget()
{
    delete ui;
}
}
