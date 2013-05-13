/****************************************************************************/
/*! \file DashboardStationConnector.cpp
 *
 *  \brief DashboardStationConnector implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 19-Jan-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Dashboard/Include/DashboardStationConnector.h"
#include <QPainter>

namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CDashboardStationConnector::CDashboardStationConnector(int Width,
                                                       int Height,
                                                       bool Animation,
                                                       QGraphicsItem *p_Parent) :
                                                       QGraphicsPathItem(p_Parent),
                                                       m_Width(Width),
                                                       m_Height(Height),
                                                       m_Animate(Animation)
{
    InitializePipePointsPositions();
    setCacheMode(QGraphicsItem::ItemCoordinateCache);

    update(); //Draw the Pipe inthe paint event
}

/****************************************************************************/
/*!
 *  \brief Destructor
 *
 */
/****************************************************************************/
CDashboardStationConnector::~CDashboardStationConnector()
{

}

/****************************************************************************/
/*!
 *  \brief Initialize All Points of the Pipe
 *
 */
/****************************************************************************/
void CDashboardStationConnector:: InitializePipePointsPositions()
{

    m_PipePoints << QPoint(175, 167) << QPoint(175, 178)
                 << QPoint(161, 178) << QPoint(161, 192) << QPoint(170, 192) << QPoint(170, 186)

                 << QPoint(289, 186) << QPoint(289, 192) << QPoint(298, 192) << QPoint(298, 186)

                 << QPoint(417, 186) << QPoint(417, 192) << QPoint(426, 192) << QPoint(426, 186)
                 << QPoint(502, 186) << QPoint(502, 262)
                 << QPoint(142, 262) << QPoint(142, 276) << QPoint(151, 276) << QPoint(151, 270)
                 << QPoint(226, 270) << QPoint(226, 276) << QPoint(235, 276) << QPoint(235, 270)
                 << QPoint(311, 270) << QPoint(311, 276) << QPoint(320, 276) << QPoint(320, 270)
                 << QPoint(396, 270) << QPoint(396, 276) << QPoint(405, 276) << QPoint(405, 270)
                 << QPoint(481, 270) << QPoint(481, 276) << QPoint(490, 276) << QPoint(490, 270)
                 << QPoint(565, 270) << QPoint(565, 276) << QPoint(574, 276) << QPoint(574, 270)
                 << QPoint(651, 270) << QPoint(651, 276) << QPoint(660, 276) << QPoint(660, 270)
                 << QPoint(715, 270) << QPoint(715, 382)
                 << QPoint(142, 382) << QPoint(142, 396) << QPoint(151, 396) << QPoint(151, 390)
                 << QPoint(226, 390) << QPoint(226, 396) << QPoint(235, 396) << QPoint(235, 390)
                 << QPoint(311, 390) << QPoint(311, 396) << QPoint(320, 396) << QPoint(320, 390)
                 << QPoint(396, 390) << QPoint(396, 396) << QPoint(405, 396) << QPoint(405, 390)
                 << QPoint(481, 390) << QPoint(481, 396) << QPoint(490, 396) << QPoint(490, 390)
                 << QPoint(565, 390) << QPoint(565, 396) << QPoint(574, 396) << QPoint(574, 390)
                 << QPoint(724, 390) << QPoint(724, 262)
                 << QPoint(511, 262) << QPoint(511, 178)
                 << QPoint(184, 178) << QPoint(184, 167);
}


void CDashboardStationConnector::paint(QPainter *p_Painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    p_Painter->setPen(QPen(Qt::black));
    p_Painter->setBrush(Qt::DiagCrossPattern);
    p_Painter->drawPolygon(m_PipePoints);
}

QRectF CDashboardStationConnector::boundingRect() const
{
   return QRectF(142, 167, m_Width, m_Height);  // (142, 167) Starting Position  of the bounding reactangle
}

} // end of Dashboard namespace
