#include "Dashboard/Include/SplashWidget.h"
#include "ui_SplashWidget.h"

SplashWidget::SplashWidget(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::SplashWidget)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    //this->SetDialogTitle(tr(""));
}

SplashWidget::~SplashWidget()
{
    delete ui;
}
