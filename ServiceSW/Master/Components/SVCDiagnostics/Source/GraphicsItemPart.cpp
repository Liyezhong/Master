/****************************************************************************/
/*! \file GraphicsItemPart.cpp
 *
 *  \brief Implementation of Graphics item part.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-02
 *   $Author:  $ Abe Yang
 *
 *   \b Company:
 *
 * 		 Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include "SVCDiagnostics/Include/GraphicsItemPart.h"

CGraphicsItemPart::CGraphicsItemPart(
        const QPixmap &pixmapNormal,
        QGraphicsItem *p_Parent,
        QGraphicsScene *p_Scene):
    QGraphicsPixmapItem(pixmapNormal,
                         p_Parent,
                         p_Scene),
    m_PartStatus(Normal),
    m_Clickable(false)
{
}

CGraphicsItemPart::CGraphicsItemPart(const QPixmap &pixmapNormal,
                                     const QPixmap &pixmapDisabled,
                                     const QPixmap &pixmapWorking,
                                     bool Clickable)
    :QGraphicsPixmapItem(pixmapNormal),
      m_PartStatus(Normal),
      m_PixmapNormal(pixmapNormal),
      m_PixmapDisabled(pixmapDisabled),
      m_PixmapWorking(pixmapWorking),
      m_Clickable(Clickable)
{
    if (Clickable) {
        this->setFlags(ItemIsSelectable | QGraphicsItem::ItemIgnoresParentOpacity |
                    ItemDoesntPropagateOpacityToChildren | QGraphicsItem::ItemClipsToShape
                    | QGraphicsItem::ItemClipsChildrenToShape);
    }
}

void CGraphicsItemPart::mouseReleaseEvent(QGraphicsSceneMouseEvent *p_Event)
{
    if (m_Clickable)
    {
        if (m_PartStatus == Normal) {
            SetStatus(Working);
        }
        else if (m_PartStatus == Working) {
            SetStatus(Normal);
        }
        emit PartSelected();
    }
    QGraphicsItem::mouseReleaseEvent(p_Event);
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
