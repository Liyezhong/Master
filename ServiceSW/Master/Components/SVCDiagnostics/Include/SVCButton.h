/****************************************************************************/
/*! \file SVCButton.h
 *
 *  \brief Custom button definition.
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
#ifndef SVC_BUTTON_H
#define SVC_BUTTON_H

#include <QPushButton>


/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class SVCButton
 */
/****************************************************************************/
class SVCButton : public QPushButton
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SVCButton
     *  \iparam isBig = whether select big pixmap
     *  \iparam _parent = the parent graphics item
     */
    /****************************************************************************/
    explicit SVCButton(bool isBig = true, QWidget *_parent=0);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SVCButton
     *  \iparam _text = button text
     *  \iparam isBig = whether select big pixmap
     *  \iparam _parent = the parent graphics item
     */
    /****************************************************************************/
    explicit SVCButton(const QString &_text, bool isBig = true, QWidget *_parent=0);

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function setPos, change button position;
     *  \iparam _x = position of x
     *  \iparam _y = position of y
     */
    /****************************************************************************/
    void setPos(int _x, int _y);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function setPos, change button position;
     *  \iparam _pos = pointer of position
     */
    /****************************************************************************/
    void setPos(QPoint _pos);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function setSize, change button size;
     *  \iparam _width = width of value
     *  \iparam _heigth = heigth of value
     */
    /****************************************************************************/
    void setSize(int _width, int _height);
};

#endif // SVC_BUTTON_H
