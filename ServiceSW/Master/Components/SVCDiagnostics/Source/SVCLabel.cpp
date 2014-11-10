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

SVCLabel::SVCLabel(bool isBig, QWidget *parent, Qt::WindowFlags f)
	: QLabel(parent, f)
{
    if (isBig) {
        setStyleSheet("QLabel { border-image: url(:/Images/Label2) }"); // big background
        width = 95;
        height = 14;
    } else {
        setStyleSheet("QLabel { border-image: url(:/Images/Label1) }"); // little
        width = 40;
        height = 14;
    }
    //setAlignment(Qt::AlignHCenter);
    setFont(QFont("WenQuanYi Zen Hei", 8, QFont::Bold));
}

SVCLabel::SVCLabel(const QString &text, bool isBig, QWidget *parent, Qt::WindowFlags f)
	: QLabel(text, parent, f)
{
    if (isBig) {
        setStyleSheet("QLabel { border-image: url(:/Images/Label2) }"); // big background
        width = 95;
        height = 14;
    } else {
        setStyleSheet("QLabel { border-image: url(:/Images/Label1) }"); // little
        width = 40;
        height = 14;
    }
    setFont(QFont("WenQuanYi Zen Hei", 9, QFont::Bold));
}

void SVCLabel::setPos(int x, int y)
{
    setGeometry(QRect(x, y, width, height));
}

void SVCLabel::setPos(QPoint pos)
{
    setGeometry(QRect(pos.x(), pos.y(), width, height));
}

/*
void SVCLabel::setText(const QString &text)
{
    QLabel::setText("  " + text);
}
*/
void SVCLabel::setSize(int width, int height)
{
    this->width = width;
    this->height = height;

    setGeometry(QRect(geometry().x(), geometry().y(), width, height));
}

