/****************************************************************************/
/*! \file UserInput.cpp
 *
 *  \brief UserInput implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-08
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

#include "TestCases/Include/UserInput.h"

namespace TestCases {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam Caption = Title of the test case
 *  \iparam SubCaption = Subtitle of the test case
 */
/****************************************************************************/
CUserInput::CUserInput(QString Caption, QString SubCaption) : CTestCaseBase(Caption, SubCaption, true)
{
    mp_EnterButton = new QPushButton("Enter");
    mp_EnterButton->setFocusPolicy(Qt::NoFocus);

    mp_InputLabel = new QLabel("n/a");

    AddWidget(1, mp_EnterButton);
    AddWidget(2, mp_InputLabel);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CUserInput::~CUserInput()
{
    try {
        delete mp_InputLabel;
        delete mp_EnterButton;
    }
    catch (...) {}
}

} // end namespace TestCases
