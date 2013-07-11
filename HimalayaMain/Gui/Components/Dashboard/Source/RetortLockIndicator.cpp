#include "Dashboard/Include/RetortLockIndicator.h"
#include <QPainter>

namespace Dashboard {

CRetortLockIndicator::CRetortLockIndicator(QWidget *parent) :
    QLabel(parent),m_bLock(false)
{
    m_Pixmap.load(":/HimalayaImages/Icons/Program/Icon2.png");
}

void CRetortLockIndicator::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter Painter(this);
    int left(0);
    if (!m_bLock)
    {
        left = this->width() - 40;
    }
    Painter.drawPixmap(left, 0, m_Pixmap);
}

void CRetortLockIndicator::lock(bool IsLocked)
{
    m_bLock = IsLocked;
    update();
}

}
