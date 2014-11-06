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
class SVCButton : public QPushButton
{
    Q_OBJECT

public:
    explicit SVCButton(bool isBig = true, QWidget *parent=0);
    explicit SVCButton(const QString &text, bool isBig = true, QWidget *parent=0);

public:
    void setPos(int x, int y);
    void setPos(QPoint pos);
    void setSize(int width, int height);

private:
    int width;
    int height;
};

#endif // SVC_BUTTON_H
