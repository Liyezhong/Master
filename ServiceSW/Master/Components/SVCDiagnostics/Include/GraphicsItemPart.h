#ifndef GRAPHICSITEMPART_H
#define GRAPHICSITEMPART_H

#include <QGraphicsPixmapItem>

class CGraphicsItemPart : public QGraphicsPixmapItem
{
public:
    CGraphicsItemPart();
    CGraphicsItemPart(const QPixmap &pixmap, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void PartSelectedChange();
};

#endif // GRAPHICSITEMPART_H
