/****************************************************************************/
/*! \file SVCButton.cpp
 *
 *  \brief Implementation of custom buttons.
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

#include "SVCDiagnostics/Include/SVCButton.h"

SVCButton::SVCButton(bool isBig, QWidget *parent)
    : QPushButton(parent)
{
    QFont Font;
    Font.setWeight(QFont::Black);
    if (isBig) {
        Font.setPointSize(14);
        setFont(Font);
        setStyleSheet("QPushButton { border-image: url(:/Images/Button2) }"); // big background
        width = 194;
        height = 44;
    } else {
        Font.setPointSize(9);
        setFont(Font);
        setStyleSheet("QPushButton { border-image: url(:/Images/Button1) }"); // little
        width = 97;
        height = 28;
    }
    //setAutoFillBackground(true);
    setFocusPolicy(Qt::NoFocus);
}

SVCButton::SVCButton(const QString &text, bool isBig, QWidget *parent)
    : QPushButton(text, parent)
{
    QFont Font;
    Font.setWeight(QFont::Bold);
    if (isBig) {
        Font.setPointSize(14);
        setFont(Font);
        setStyleSheet("QPushButton { border-image: url(:/Images/Button2) }"); // big background
    }
    else {
        Font.setPointSize(9);
        setFont(Font);
        setStyleSheet("QPushButton { border-image: url(:/Images/Button1) }"); // little
   }
   setFocusPolicy(Qt::NoFocus);
}

void SVCButton::setPos(int x, int y)
{
    setGeometry(QRect(x, y, width, height));
}

void SVCButton::setPos(QPoint pos)
{
    setGeometry(QRect(pos.x(), pos.y(), width, height));
}

void SVCButton::setSize(int width, int height)
{
    this->width = width;
    this->height = height;

    setGeometry(QRect(geometry().x(), geometry().y(), width, height));
}
