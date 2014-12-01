#include "SVCDiagnostics/Include/GraphicsItemPart.h"

CGraphicsItemPart::CGraphicsItemPart(
        const QPixmap &pixmapNormal,
        QGraphicsItem *p_Parent,
        QGraphicsScene *p_Scene):
    QGraphicsPixmapItem(pixmapNormal,
                         p_Parent,
                         p_Scene),
    m_PartStatus(Normal),
    m_StatusChange(false)
{
}

CGraphicsItemPart::CGraphicsItemPart(const QPixmap &pixmapNormal,
                                     const QPixmap &pixmapDisabled,
                                     const QPixmap &pixmapWorking)
    :QGraphicsPixmapItem(pixmapNormal),
      m_PartStatus(Normal),
      m_PixmapNormal(pixmapNormal),
      m_PixmapDisabled(pixmapDisabled),
      m_PixmapWorking(pixmapWorking),
      m_StatusChange(false)
{
    this->setFlags(ItemIsSelectable | QGraphicsItem::ItemIgnoresParentOpacity |
                   ItemDoesntPropagateOpacityToChildren | QGraphicsItem::ItemClipsToShape
                   | QGraphicsItem::ItemClipsChildrenToShape);
}

void CGraphicsItemPart::mouseReleaseEvent(QGraphicsSceneMouseEvent *p_Event)
{
    if (m_StatusChange)
    {
        SetStatus(Working);
        m_StatusChange = false;
    }
    else
    {
        SetStatus(Normal);
        m_StatusChange = true;
    }
    emit PartSelected();
    QGraphicsItem::mouseReleaseEvent(p_Event);
}

void CGraphicsItemPart::SetStatus(PartStatus status)
{
    m_PartStatus = status;
    if (m_PartStatus == Normal)
    {
        this->setPixmap(m_PixmapNormal);
        m_StatusChange = true;
    }
    else if (m_PartStatus == Disabled)
    {
        this->setPixmap(m_PixmapDisabled);
    }
    else if (m_PartStatus == Working)
    {
        this->setPixmap(m_PixmapWorking);
        m_StatusChange = false;
    }
    update();
}
