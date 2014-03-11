/****************************************************************************/
/*! \file TestScreen.h
 *
 *  \brief TestScreen definition.
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

#ifndef TESTFRAMEWORK_TESTSCREEN_H
#define TESTFRAMEWORK_TESTSCREEN_H

#include "TestCases/Include/TestCaseBase.h"
#include "MainMenu/Include/DialogFrame.h"
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>

namespace TestFramework {

namespace Ui {
    class CTestScreen;
}

/****************************************************************************/
/**
 * \brief The base class of all test screens.
 */
/****************************************************************************/
class CTestScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CTestScreen(QWidget *p_Parent = 0);
    virtual ~CTestScreen();

protected:
    void AddCaption(QString Caption);
    void AddTestCase(TestCases::CTestCaseBase *p_TestCase);

private:
    Ui::CTestScreen *mp_Ui;         //!< User interface
    QGridLayout *mp_TestCaseLayout; //!< Layout for the test cases
    QWidget *mp_TestCaseWidget;     //!< Widget holding the content
    QScrollArea *mp_ScrollArea;     //!< The scroll area embedding the main widget
    qint32 m_LineCounter;           //!< Number of test cases already added
    QList<QLabel *> m_Caption;      //!< List of subcaption on the screen
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_TESTSCREEN_H
