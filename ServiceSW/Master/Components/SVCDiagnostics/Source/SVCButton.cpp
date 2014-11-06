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
    if (isBig) {
        setStyleSheet("QPushButton { background: url(:/Images/Button2) }"); // big background
        width = 194;
        height = 44;
    } else {
        setStyleSheet("QPushButton { background: url(:/Images/Button1) }"); // little
        width = 97;
        height = 26;
    }
    setFont(QFont("WenQuanYi Zen Hei", 12, QFont::Bold));
}

SVCButton::SVCButton(const QString &text, bool isBig, QWidget *parent)
    : QPushButton(text, parent)
{
    if (isBig)
        setStyleSheet("QPushButton { background: url(:/Images/Button2) }"); // big background
    else
        setStyleSheet("QPushButton { background: url(:/Images/Button1) }"); // little
    setFont(QFont("WenQuanYi Zen Hei", 12, QFont::Bold));
}

void SVCButton::setPos(int x, int y)
{
    setGeometry(QRect(x, y, width, height));
}

void SVCButton::setPos(QPoint pos)
{
    setGeometry(QRect(pos.x(), pos.y(), width, height));
}
