#ifndef RETORTLOCKINDICATOR_H
#define RETORTLOCKINDICATOR_H

#include <QLabel>
namespace Dashboard {
class CRetortLockIndicator : public QLabel
{
    Q_OBJECT
public:
    explicit CRetortLockIndicator(QWidget *parent = 0);
    void lock(bool IsLocked);
signals:

protected:
    void paintEvent(QPaintEvent * e);

private:
    QPixmap m_Pixmap;
    bool m_bLock;
public slots:
    
};
}

#endif // RETORTLOCKINDICATOR_H
