/****************************************************************************/
/*! \file SVCLabel.h
 *
 *  \brief Custom label definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-02
 *   $Author:  $ Arthur Li
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
#ifndef SVC_LABEL_H
#define SVC_LABEL_H

#include <QLabel>


/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class SVCLabel
 */
/****************************************************************************/
class SVCLabel : public QLabel
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SVCLabel
     *  \iparam isBig = whether select big pixmap
     *  \iparam _parent = the parent graphics item
     *  \iparam f = window flag
     */
    /****************************************************************************/
    explicit SVCLabel(bool isBig = true, QWidget *_parent=0, Qt::WindowFlags f=0);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SVCLabel
     *  \iparam _text = text for label display
     *  \iparam isBig = whether select big pixmap
     *  \iparam _parent = the parent graphics item
     *  \iparam f = window flag
     */
    /****************************************************************************/
    explicit SVCLabel(const QString &_text, bool isBig = true, QWidget *_parent=0, Qt::WindowFlags f=0);

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function setPos, change label position;
     *  \iparam _x = x coordinate of the point.
     *  \iparam _y = y coordinate of the point.
     */
    /****************************************************************************/
    void setPos(int _x, int _y);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function setPos, change label position;
     *  \iparam _pos = point of pos
     */
    /****************************************************************************/
    void setPos(QPoint _pos);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function setSize;change label size;
     *  \iparam _width = size width
     *  \iparam _height = size height
     */
    /****************************************************************************/
    void setSize(int _width, int _height);
};

#endif // SVC_LABEL_H
