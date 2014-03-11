/****************************************************************************/
/*! \file DigitalOutput.cpp
 *
 *  \brief DigitalOutput implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-31
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

#include "TestCases/Include/DigitalOutput.h"

namespace TestCases {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam Caption = Subtitle of the test case
 *  \iparam Question = The question the user has to answer
 */
/****************************************************************************/
CDigitalOutput::CDigitalOutput(QString Caption, QString Question) : CTestCaseBase(Caption, "", true)
{
    mp_QuestionLabel = new QLabel(Question);
    mp_YesButton = new QPushButton("Yes");
    mp_NoButton = new QPushButton("No");

    mp_YesButton->setFocusPolicy(Qt::NoFocus);
    mp_NoButton->setFocusPolicy(Qt::NoFocus);

    AddWidget(1, mp_QuestionLabel);
    AddWidgetVertical(3, mp_YesButton, mp_NoButton);
    SetLayoutSpan(1, 3);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDigitalOutput::~CDigitalOutput()
{
    try {
        delete mp_NoButton;
        delete mp_YesButton;
        delete mp_QuestionLabel;
    }
    catch (...) {}
}

} // end namespace TestCases
