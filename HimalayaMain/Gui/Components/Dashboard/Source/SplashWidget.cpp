#include "Dashboard/Include/SplashWidget.h"
#include "ui_SplashWidget.h"

SplashWidget::SplashWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplashWidget)
{
    ui->setupUi(this);
}

SplashWidget::~SplashWidget()
{
    delete ui;
}
