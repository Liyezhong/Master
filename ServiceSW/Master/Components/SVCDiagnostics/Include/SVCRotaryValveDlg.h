/****************************************************************************/
/*! \file SVCRotaryValveDlg.cpp
 *
 *  \brief Declaration of rotary valve detail dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-15
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef SVCROTARYVALVEDLG_H
#define SVCROTARYVALVEDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "ui_RotaryValveDlg.h"
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QLineEdit>

namespace SVCDiagnostics {

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CGraphicsPixmapItem
 */
/****************************************************************************/
class CGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CGraphicsPixmapItem
     *  \iparam enableState = image enable path
     *  \iparam diableState = image disable path
     */
    /****************************************************************************/
    CGraphicsPixmapItem(QString enableState, QString diableState = "");

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function boundingRect
     *  \return QRectF
     */
    /****************************************************************************/
    QRectF boundingRect() const;

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function shape
     *  \return QPainterPath
     */
    /****************************************************************************/
    QPainterPath shape() const;

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function paint
     *  \iparam painter
     */
    /****************************************************************************/
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

public:
    /****************************************************************************/
    /*!
     *  \brief Set pixmap state
     *  \iparam status = enable/false
     */
    /****************************************************************************/
    void SetState(bool status);
    /****************************************************************************/
    /*!
     *  \brief Set tag
     *  \iparam _tag
     */
    /****************************************************************************/
    void SetTag(int _tag);

signals:
    /****************************************************************************/
    /*!
     *  \brief when pixamp is Clicked, send a signal
     *  \iparam _tag
     */
    /****************************************************************************/
    void Clicked(int _tag);

protected:
    /****************************************************************************/
    /*!
     *  \brief when pixamp is Pressed, send a event
     *  \iparam _event = the event when mouse press
     */
    /****************************************************************************/
    void mousePressEvent(QGraphicsSceneMouseEvent *_event);
    /****************************************************************************/
    /*!
     *  \brief when pixamp is Released, send a event
     *  \iparam _event = the event when mouse release
     */
    /****************************************************************************/
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *_event);

private:
    QPixmap pixmapEnable;     //!< save pixmap enable state
    QPixmap pixmapDisable;     //!< save pixmap disable state

    int tag;     //!< save tag
};

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CSVCRotaryValveDlg
 */
/****************************************************************************/
class CSVCRotaryValveDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of constructor function CSVCRotaryValveDlg
     *  \iparam p_Parent = parent widget
     */
    /****************************************************************************/
    explicit CSVCRotaryValveDlg(QWidget* p_Parent);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CSVCRotaryValveDlg();

    /****************************************************************************/
    /*!
     *  \brief  Set rotary valve position
     *  \iparam flag = sealing or tube flag
     *  \iparam position = target position
     */
    /****************************************************************************/
    void SetPos(bool flag, qint32 position);

    /****************************************************************************/
    /*!
     *  \brief  Get rotary valve selected position
     *  \iparam flag = sealing or tube flag
     *  \oparam position = target position
     */
    /****************************************************************************/
    void GetPos(bool &flag, qint32 &position);


private Q_SLOTS:

    /****************************************************************************/
    /*!
     *  \brief  Slot for rotary valve position button clicked
     *  \iparam _tag = tag of image
     */
    /****************************************************************************/
    void RotaryValveClicked(int _tag);

private:
    Ui::CRotaryValveDlg* ui;    //!< User interface
    QGraphicsScene *mp_Scene;   //!< graphics scene

    CGraphicsPixmapItem *rotaryValvePointer; //!< rotary valve position pointer
    CGraphicsPixmapItem *rotaryValvePos[32]; //!< 0 - 15: tube, 16-31: sealing

    int tag; //!< save rotay valve position
};

}//end of namespace SVCDiagnostics

#endif // SVCROTARYVALVEDLG_H
