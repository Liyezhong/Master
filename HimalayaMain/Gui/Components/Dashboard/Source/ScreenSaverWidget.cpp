#include "Dashboard/Include/ScreenSaverWidget.h"
#include "ui_ScreenSaverWidget.h"
#include <QTimer>


ScreenSaverWidget::ScreenSaverWidget(QWidget *p) :
    QWidget(p),
    ui(new Ui::ScreenSaverWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->move(0, 0);
    m_timer = new QTimer(this);
    m_timer->setInterval(900000);//, here should be 30 mintues, 1800000
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

bool ScreenSaverWidget::event ( QEvent * event )
{
    if (event->type() == QEvent::WindowBlocked)
    {
        OnInteractStart();
    }
    return QWidget::event(event);
}

void ScreenSaverWidget::AppIdleForLongTime()
{
    m_timer->stop();
    this->show();
}

void ScreenSaverWidget::OnInteractStart()
{
    m_timer->stop();
    m_timer->start();

    if (this->isVisible())
    this->hide();
}
