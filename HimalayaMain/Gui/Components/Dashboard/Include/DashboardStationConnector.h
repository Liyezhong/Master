/****************************************************************************/
/*! \file DashboardStationConnector.h
 *
 *  \brief DashboardStationConnector definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 29-Jan-2013
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
#ifndef DASHBOARD_DASHBOARDSTATIONCONNECTOR_H
#define DASHBOARD_DASHBOARDSTATIONCONNECTOR_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QGraphicsItem>

namespace Dashboard {

/****************************************************************************/
/**
 * \brief This class is the graphics scene for the bath layout.
 */
/****************************************************************************/
class CDashboardStationConnector: public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit CDashboardStationConnector(int Width, int Height, bool Animation = false, QGraphicsItem *p_Parent = NULL);
    virtual ~CDashboardStationConnector();
    void InitializePipePointsPositions();

protected:
    QRectF boundingRect() const;
    void paint(QPainter *p_Painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
    int m_Width;
    int m_Height;
    bool m_Animate;
    QVector<QPoint> m_PipePoints;


};

}    // end of Dashboard namespace

#endif // DASHBOARD_DASHBOARDSTATIONCONNECTOR_H
