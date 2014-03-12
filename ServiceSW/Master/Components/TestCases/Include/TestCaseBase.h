/****************************************************************************/
/*! \file TestCaseBase.h
 *
 *  \brief TestCaseBase definition.
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

#ifndef TESTCASES_TESTCASEBASE_H
#define TESTCASES_TESTCASEBASE_H

#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace TestCases {

/****************************************************************************/
/**
 * \brief The base class for all test cases.
 */
/****************************************************************************/
class CTestCaseBase : public QObject
{
    Q_OBJECT

public:
    CTestCaseBase(QString Caption, QString SubCaption, bool AutoStart);
    virtual ~CTestCaseBase();
    QWidget *GetLayoutItem(qint32 Index);
    qint32 GetLayoutSpan(qint32 Index);

signals:
/****************************************************************************/
/**
 * \brief Starts the update of the status dialog of a test run
 */
/****************************************************************************/
    void UpdateDialog();

protected:
    void AddWidget(qint32 Index, QWidget *p_Widget);
    void AddWidgetVertical(qint32 Index, QWidget *p_Widget1, QWidget *p_Widget2);
    void SetLayoutSpan(qint32 Index, qint32 Span);

private:
    QWidget *mp_WidgetArray[5]; //!< The widgets placed in the layout slots of the test screen
    qint32 m_SpanArray[5];      //!< The layout span of the individual widgets
    QLabel *mp_Caption;         //!< The caption of the test case
    QLabel *mp_SubCaption;      //!< The text below the caption of the test case
    QPushButton *mp_Button;     //!< Start button
    QLabel *mp_ResultLed;       //!< Indicates the success of the test
    QList<QVBoxLayout *> m_VerticalLayout;  //!< Layout for vertically stacked elements
    QList<QWidget *> m_VerticalWidget;      //!< The widget holding the vertical layout
};

} // end namespace TestCases

#endif // TESTCASES_TESTCASEBASE_H
