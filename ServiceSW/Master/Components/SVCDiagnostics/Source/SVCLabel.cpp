/****************************************************************************/
/*! \file SVCLabel.cpp
 *
 *  \brief Implementation of custom labels.
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

#include "SVCDiagnostics/Include/SVCLabel.h"

SVCLabel::SVCLabel(bool isBig, QWidget *_parent, Qt::WindowFlags f)
    : QLabel(_parent, f)
{
    if (isBig) {
        setStyleSheet("QLabel { border-image: url(:/Images/Label2) }"); // big background
        resize(95, 14);
    } else {
        setStyleSheet("QLabel { border-image: url(:/Images/Label1) }"); // little
        resize(70, 14);
    }
    //setAlignment(Qt::AlignHCenter);
    setFont(QFont("WenQuanYi Zen Hei", 8, (int)QFont::Bold));
}

SVCLabel::SVCLabel(const QString &_text, bool isBig, QWidget *_parent, Qt::WindowFlags f)
    : QLabel(_text, _parent, f)
{
    if (isBig) {
        setStyleSheet("QLabel { border-image: url(:/Images/Label2) }"); // big background
        resize(95, 14);
    } else {
        setStyleSheet("QLabel { border-image: url(:/Images/Label1) }"); // little
        resize(70, 14);
    }
    setFont(QFont("WenQuanYi Zen Hei", 8, (int)QFont::Bold));
}

void SVCLabel::setPos(int _x, int _y)
{
    setGeometry(QRect(_x, _y, frameSize().width(), frameSize().height()));
}

void SVCLabel::setPos(QPoint _pos)
{
    setGeometry(QRect(_pos.x(), _pos.y(), frameSize().width(), frameSize().height()));
}

void SVCLabel::setSize(int _width, int _height)
{
    resize(_width, _height);
}

