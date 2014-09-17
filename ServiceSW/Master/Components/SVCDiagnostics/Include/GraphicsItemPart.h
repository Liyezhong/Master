#ifndef GRAPHICSITEMPART_H
#define GRAPHICSITEMPART_H

#include <QObject>
#include <QGraphicsPixmapItem>

class CGraphicsItemPart : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    CGraphicsItemPart(const QPixmap &pixmapNormal,
                      QGraphicsItem *parent = 0,
                      QGraphicsScene *scene = 0);
    CGraphicsItemPart(const QPixmap &pixmapNormal,
                      const QPixmap &pixmapDisabled,
                      const QPixmap &pixmapWorking);

    enum PartStatus {
        Normal,
        Disabled,
        Working
    };

    inline PartStatus Status() { return m_PartStatus; }
    void SetStatus(PartStatus status);
protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    PartStatus m_PartStatus;
    QPixmap m_PixmapNormal;
    QPixmap m_PixmapDisabled;
    QPixmap m_PixmapWorking;
    bool s;
Q_SIGNALS:
    void PartSelected();
};

#endif // GRAPHICSITEMPART_H
