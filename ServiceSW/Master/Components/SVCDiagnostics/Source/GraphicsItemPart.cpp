#include "SVCDiagnostics/Include/GraphicsItemPart.h"

CGraphicsItemPart::CGraphicsItemPart()
{
}

CGraphicsItemPart::CGraphicsItemPart(const QPixmap &pixmap, QGraphicsItem *parent, QGraphicsScene *scene)
    :QGraphicsPixmapItem(pixmap, parent, scene)
{
    this->setFlag(ItemIsSelectable);
}

QVariant CGraphicsItemPart::itemChange(GraphicsItemChange change, const QVariant &value)
{
     switch (change) {
     case ItemSelectedHasChanged:
         //emit PartSelectedChange();
        break;
     default:
         break;
     };
    return QGraphicsItem::itemChange(change, value);
}

