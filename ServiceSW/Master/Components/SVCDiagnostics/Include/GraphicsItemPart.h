/****************************************************************************/
/*! \file GraphicsItemPart.h
 *
 *  \brief Graphics item part definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-02
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
#ifndef GRAPHICSITEMPART_H
#define GRAPHICSITEMPART_H

#include <QObject>
#include <QGraphicsPixmapItem>

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CGraphicsItemPart
 */
/****************************************************************************/
class CGraphicsItemPart : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CGraphicsItemPart
     *  \iparam pixmapNormal = the normal pixmap
     *  \iparam p_Parent = the parent graphics item
     *  \iparam p_Scene = the graphics scence
     */
    /****************************************************************************/
    CGraphicsItemPart(const QPixmap &pixmapNormal,
                      QGraphicsItem *p_Parent = 0,
                      QGraphicsScene *p_Scene = 0);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CGraphicsItemPart
     *  \iparam pixmapNormal = the normal pixmap
     *  \iparam pixmapDisabled = the disbaled pixmap
     *  \iparam pixmapWorking = the working pixmap
     *
     *  \iparam Clickable = clickable flag
     */
    /****************************************************************************/
    CGraphicsItemPart(const QPixmap &pixmapNormal,
                      const QPixmap &pixmapDisabled,
                      const QPixmap &pixmapWorking,
                      bool Clickable = true);

    /****************************************************************************/
    /*!
     *  \brief  enum of the part status
     */
    /****************************************************************************/
    enum PartStatus {
        Normal,
        Disabled,
        Working
    };

    /****************************************************************************/
    /*!
     *  \brief  To get part status
     *  \return PartStatus
     */
    /****************************************************************************/
    inline PartStatus Status() { return m_PartStatus; }

    /****************************************************************************/
    /*!
     *  \brief  To set part status
     *  \iparam status = status of the part
     */
    /****************************************************************************/
    void SetStatus(PartStatus status);

protected:
    /****************************************************************************/
    /*!
     *  \brief Event handler for mouse release events
     *  \iparam p_Event = Event for Mouse release
     */
    /****************************************************************************/
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *p_Event);

private:
    PartStatus m_PartStatus;    //!< Store the part status
    QPixmap m_PixmapNormal;     //!< The normal pixmap
    QPixmap m_PixmapDisabled;   //!< The disbaled pixmap
    QPixmap m_PixmapWorking;    //!< The working pixmap
    bool m_Clickable;           //!< Flag for part clicked
Q_SIGNALS:

    /****************************************************************************/
    /*!
     *  \brief  The signal is emit when part is clicked.
     */
    /****************************************************************************/
    void PartSelected();
};

#endif // GRAPHICSITEMPART_H
