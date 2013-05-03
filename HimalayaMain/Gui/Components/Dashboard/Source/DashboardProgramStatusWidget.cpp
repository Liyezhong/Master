#include "Dashboard/Include/DashboardProgramStatusWidget.h"
#include "ui_DashboardProgramStatusWidget.h"
#include "Dashboard/Include/DashboardDateTimeWidget.h"

using namespace Dashboard;

CDashboardProgramStatusWidget::CDashboardProgramStatusWidget(QWidget *parent) :
    MainMenu::CPanelFrame(parent),
    ui(new Ui::CDashboardProgramStatusWidget)
{
    ui->setupUi(this);

    SetPanelTitle(QString(tr("\"%1\"").arg(CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME)));
    SetTitleCenter();

    CONNECTSIGNALSLOT(ui->btnClose, clicked(), this, OnClose());
}

CDashboardProgramStatusWidget::~CDashboardProgramStatusWidget()
{
    delete ui;
}

void CDashboardProgramStatusWidget::OnClose()
{
    this->close();
}
