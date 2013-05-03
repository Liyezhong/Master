#include "Dashboard/Include/CassetteNumberInputWidget.h"
#include "ui_CassetteNumberInputWidget.h"
#include "Global/Include/Utils.h"

using namespace Dashboard;

CCassetteNumberInputWidget::CCassetteNumberInputWidget(QWidget *parent) :
    MainMenu::CPanelFrame(parent),
    ui(new Ui::CCassetteNumberInputWidget)
{
    ui->setupUi(this);
    CONNECTSIGNALSLOT(ui->btnOK, clicked(), this, OnOK());

    mp_singleWheel = new MainMenu::CScrollWheel();
    mp_tenWheel = new MainMenu::CScrollWheel();
    mp_hundredWheel = new MainMenu::CScrollWheel();

    for (int i = 0; i <= 9; i++) {
        mp_singleWheel->AddItem(QString("%1").arg(i, 1, 10, QChar('0')), i);
    }

    for (int i = 0; i <= 9; i++) {
        mp_tenWheel->AddItem(QString("%1").arg(i, 1, 10, QChar('0')), i);
    }

    for (int i = 0; i <= 9; i++) {
        mp_hundredWheel->AddItem(QString("%1").arg(i, 1, 10, QChar('0')), i);
    }

    ui->scrollPanelCassetteNumber->Init(3);
    ui->scrollPanelCassetteNumber->AddScrollWheel(mp_hundredWheel, 0);
    ui->scrollPanelCassetteNumber->AddSeparator(MainMenu::CWheelPanel::FULLSTOP, 0);
    ui->scrollPanelCassetteNumber->AddScrollWheel(mp_tenWheel, 1);
    ui->scrollPanelCassetteNumber->AddSeparator(MainMenu::CWheelPanel::FULLSTOP, 1);
    ui->scrollPanelCassetteNumber->AddScrollWheel(mp_singleWheel, 2);
}

CCassetteNumberInputWidget::~CCassetteNumberInputWidget()
{
    delete ui;
    delete mp_singleWheel;
    delete mp_tenWheel;
    delete mp_hundredWheel;
}

void CCassetteNumberInputWidget::OnOK()
{
    this->close();
}
