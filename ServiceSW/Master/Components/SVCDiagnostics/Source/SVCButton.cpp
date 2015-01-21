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
#include <QFont>
#include "SVCDiagnostics/Include/SVCButton.h"

SVCButton::SVCButton(bool isBig, QWidget *_parent)
    : QPushButton(_parent)
{
    QFont Font;
    Font.setWeight((int)QFont::Black);
    Font.setPointSize(9);
    setStyleSheet("QPushButton { border-image: url(:/Images/Button) }");
    if (isBig) {
         // big background
        resize(110, 28);
    } else {
        resize(97, 28);
    }
    setFont(Font);
    //setAutoFillBackground(true);
    setFocusPolicy(Qt::NoFocus);
}

SVCButton::SVCButton(const QString &_text, bool isBig, QWidget *_parent)
    : QPushButton(_text, _parent)
{
    QFont Font;
    Font.setWeight((int)QFont::Bold);

    if (isBig) {
        Font.setPointSize(14);
        setFont(Font);
        setStyleSheet("QPushButton { border-image: url(:/Images/Button2) }"); // big background
    } else {
        Font.setPointSize(9);
        setFont(Font);
        setStyleSheet("QPushButton { border-image: url(:/Images/Button1) }"); // little
    }

    setFocusPolicy(Qt::NoFocus);
}

void SVCButton::setPos(int _x, int _y)
{
    setGeometry(QRect(_x, _y, frameSize().width(), frameSize().height()));
}

void SVCButton::setPos(QPoint _pos)
{
    setGeometry(QRect(_pos.x(), _pos.y(), frameSize().width(), frameSize().height()));
}

void SVCButton::setSize(int _width, int _height)
{
    resize(_width, _height);
}
