#include "Dashboard/Include/ScreenSaverWidget.h"
#include "ui_ScreenSaverWidget.h"
#include <QTimer>

ScreenSaverWidget::ScreenSaverWidget(QWidget *parent) :
    MainMenu::CPanelFrame(parent),
    ui(new Ui::ScreenSaverWidget),
    m_isFirstShow(true)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->move(0, 0);
    m_timer = new QTimer(this);
    m_timer->setInterval(1800000);//30 mintues
    (void)connect(m_timer, SIGNAL(timeout()), this, SLOT(AppIdleForLongTime()));
    m_timer->start();
}

ScreenSaverWidget::~ScreenSaverWidget()
{
    try
    {
        delete ui;
        delete m_timer;
    }
    catch(...)
    {}
}

void ScreenSaverWidget::AppIdleForLongTime()
{
    m_timer->stop();
    if (m_isFirstShow)
    {
        this->setStyleSheet(this->property("defaultStyleSheet").toString() +
                      "QWidget { border-image: url(:/HimalayaImages/Icons/Dashboard/ScreenSaver/ScreenSaver.png) }");

        m_isFirstShow = false;
    }

    this->show();
}

void ScreenSaverWidget::OnInteractStart()
{
    m_timer->stop();
    m_timer->start();

    if (this->isVisible())
    this->hide();
}
