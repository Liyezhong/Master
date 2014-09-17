#include "SVCDiagnostics/Include/GraphicsItemPart.h"

CGraphicsItemPart::CGraphicsItemPart(
        const QPixmap &pixmapNormal,
        QGraphicsItem *parent,
        QGraphicsScene *scene):
    QGraphicsPixmapItem(pixmapNormal,
                         parent,
                         scene),
    s(false)
{
}

CGraphicsItemPart::CGraphicsItemPart(const QPixmap &pixmapNormal,
                                     const QPixmap &pixmapDisabled,
                                     const QPixmap &pixmapWorking)
    :QGraphicsPixmapItem(pixmapNormal),
      m_PixmapNormal(pixmapNormal),
      m_PixmapDisabled(pixmapDisabled),
      m_PixmapWorking(pixmapWorking)
{
    this->setFlags(ItemIsSelectable | QGraphicsItem::ItemIgnoresParentOpacity |
                   ItemDoesntPropagateOpacityToChildren | QGraphicsItem::ItemClipsToShape
                   | QGraphicsItem::ItemClipsChildrenToShape);
}

void CGraphicsItemPart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (s)
    {
       SetStatus(Working);
       s = false;
    }
    else
    {
     SetStatus(Normal);
     s = true;
    }
    emit PartSelected();
    QGraphicsItem::mouseReleaseEvent(event);
}

void CGraphicsItemPart::SetStatus(PartStatus status)
{
    m_PartStatus = status;
    if (m_PartStatus == Normal)
    {
        this->setPixmap(m_PixmapNormal);
    }
    else if (m_PartStatus == Disabled)
    {
        this->setPixmap(m_PixmapDisabled);
    }
    else if (m_PartStatus == Working)
    {
        this->setPixmap(m_PixmapWorking);
    }
    update();
}
