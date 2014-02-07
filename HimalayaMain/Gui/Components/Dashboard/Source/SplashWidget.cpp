#include "Dashboard/Include/SplashWidget.h"
#include "ui_SplashWidget.h"
#include <QPalette>

SplashWidget::SplashWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashWidget)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet ("background-color:transparent");
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
}

SplashWidget::~SplashWidget()
{
    try
    {
        delete ui;
    }
    catch(...)
    {}
}

void SplashWidget::ShowModel()
{
    QWidget* pParent = this->parentWidget();
    if (pParent)
    {
        QRect scr = pParent->geometry();
        this->move( scr.center() - this->rect().center());
    }
    this->exec();
}
