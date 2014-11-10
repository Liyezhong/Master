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
class SVCLabel : public QLabel
{
    Q_OBJECT

public:
    explicit SVCLabel(bool isBig = true, QWidget *parent=0, Qt::WindowFlags f=0);
    explicit SVCLabel(const QString &text, bool isBig = true, QWidget *parent=0, Qt::WindowFlags f=0);

public:
    void setPos(int x, int y);
    void setPos(QPoint pos);
    void setSize(int width, int height);
    //void setText(const QString &text);

private:
    int width;
    int height;
};

#endif // SVC_LABEL_H
