/****************************************************************************/
/*! \file TestCaseBase.cpp
 *
 *  \brief TestCaseBase implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-23
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "TestCases/Include/TestCaseBase.h"
#include <QDebug>
#include <QPainter>

namespace TestCases {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam Caption = Title of the test case
 *  \iparam SubCaption = Subtitle of the test case
 *  \iparam AutoStart = Indicates if the test has a start button (false)
 */
/****************************************************************************/
CTestCaseBase::CTestCaseBase(QString Caption, QString SubCaption, bool AutoStart) : QObject()
{
    QPixmap Pixmap(":/RadioButton/RadioButton-Disabled.png");

    mp_Caption = new QLabel(Caption);
    mp_SubCaption = new QLabel(SubCaption);
    mp_Button = new QPushButton("Start");
    mp_ResultLed = new QLabel();

    mp_Button->setFocusPolicy(Qt::NoFocus);

    if (SubCaption == "") {
        AddWidget(0, mp_Caption);
    }
    else {
        AddWidgetVertical(0, mp_Caption, mp_SubCaption);
    }

    if (AutoStart == false) {
        AddWidget(1, mp_Button);
    }
    else {
        mp_WidgetArray[1] = NULL;
    }

    mp_WidgetArray[2] = NULL;
    mp_WidgetArray[3] = NULL;

    mp_ResultLed->setPixmap(Pixmap);
    AddWidget(4, mp_ResultLed);

    for (qint32 i = 0; i < 5; i++) {
        m_SpanArray[i] = 1;
    }
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTestCaseBase::~CTestCaseBase()
{
    try {
        delete mp_ResultLed;
        delete mp_Button;
        delete mp_SubCaption;
        delete mp_Caption;

        while (!m_VerticalLayout.isEmpty()) {
            delete m_VerticalLayout.takeFirst();
        }

        while (!m_VerticalWidget.isEmpty()) {
            delete m_VerticalWidget.takeFirst();
        }
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Fetches a sub widget of a test case
 *
 *  \iparam Index = Position index in the test case screen
 *
 *  \return Pointer to the widget
 */
/****************************************************************************/
QWidget *CTestCaseBase::GetLayoutItem(qint32 Index)
{
    return mp_WidgetArray[Index];
}

/****************************************************************************/
/*!
 *  \brief Fetches the column span for a sub widget
 *
 *  \iparam Index = Position index in the test case screen
 *
 *  \return Column span
 */
/****************************************************************************/
qint32 CTestCaseBase::GetLayoutSpan(qint32 Index)
{
    return m_SpanArray[Index];
}

/****************************************************************************/
/*!
 *  \brief Sets the column span for a sub widget
 *
 *  \iparam Index = Position index in the test case screen
 *  \iparam Span = Column span
 */
/****************************************************************************/
void CTestCaseBase::SetLayoutSpan(qint32 Index, qint32 Span)
{
    m_SpanArray[Index] = Span;
}

/****************************************************************************/
/*!
 *  \brief Adds a single widget to the test case
 *
 *  \iparam Index = Position index in the test case screen
 *  \iparam p_Widget = Pointer to the widget
 */
/****************************************************************************/
void CTestCaseBase::AddWidget(qint32 Index, QWidget *p_Widget)
{
    mp_WidgetArray[Index] = p_Widget;
}

/****************************************************************************/
/*!
 *  \brief Adds two widgets in a vertical layout
 *
 *  \iparam Index = Position index in the test case screen
 *  \iparam p_Widget1 = Pointer to the first widget
 *  \iparam p_Widget2 = Pointer to the second widget
 */
/****************************************************************************/
void CTestCaseBase::AddWidgetVertical(qint32 Index, QWidget *p_Widget1, QWidget *p_Widget2)
{
    //lint -esym(429, p_Widget1)
    //lint -esym(429, p_Widget2)
    //lint -esym(429, p_Widget)
    //lint -esym(429, p_Layout)
    QWidget *p_Widget = new QWidget();
    mp_WidgetArray[Index] = p_Widget;
    m_VerticalWidget << p_Widget;

    QVBoxLayout *p_Layout = new QVBoxLayout(p_Widget);
    p_Layout->setMargin(0);
    p_Layout->addWidget(p_Widget1);
    p_Layout->addWidget(p_Widget2);
    m_VerticalLayout << p_Layout;
}

} // end namespace TestCases
