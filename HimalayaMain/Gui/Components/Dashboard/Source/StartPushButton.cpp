/****************************************************************************/
/*! \file StartPushButton.cpp
 *
 *  \brief Implementation of file for class CStartPushButton.
 *
 *  \b Description:
 *          This is a customized Start/Abort button for Himalaya Project.
 *
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-03-12
 *   $Author:  $ A.Yang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2014 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Dashboard/Include/StartPushButton.h"
#include <QPainter>
#include <QPaintEvent>

namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CStartPushButton::CStartPushButton(QWidget *p_Parent) :
    QPushButton(p_Parent),
    m_IsStartStatus(true)
{
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CStartPushButton::~CStartPushButton()
{
}

void CStartPushButton::setText(const QString &text)
{
    m_Text = text;
}

void CStartPushButton::IsStartStatus(bool bSet)
{
    m_IsStartStatus = bSet;
}

/****************************************************************************/
/*!
 *  \brief Draw the pause button 
 */
/****************************************************************************/
void CStartPushButton::paintEvent(QPaintEvent * event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    QRect rect = event->rect();
    int XCenter = rect.width() / 2;
    int top = 13;

    //draw the caption at the left side
    const QFont& font = painter.font();
    QFontMetrics fm(font);
    painter.drawText(XCenter - 7, top + fm.height() -5 - fm.descent(), m_Text);

    //draw the virtical line
    int offset = 5;
    int lineLeft = XCenter + 33;
    painter.drawLine(lineLeft, top, lineLeft, rect.bottom() - 4);

    //draw the solid circle point at the right side
    QRectF rectangle(lineLeft + 8, top + 2, 10.0, 10.0);
    QPainterPath path;
    path.addEllipse(rectangle);

    QColor color(Qt::white);
    if (!this->isEnabled())
    {
        color = Qt::lightGray;
    }
    else if (m_IsStartStatus)
    {
        color.setRgb(162, 164, 164);
    }
    else if (!m_IsStartStatus)
        color.setRgb(255, 1, 1);

    QBrush brush(color);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillPath(path, brush);
}

} // end namespace Dashboard
