/****************************************************************************/
/*! \file AnalogInput.cpp
 *
 *  \brief AnalogInput implementation.
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

#include "TestCases/Include/AnalogInput.h"

namespace TestCases {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam Caption = Title of the test case
 *  \iparam SubCaption = Second title of the test
 *  \iparam AutoStart = Indicates if the test has a start button (false)
 */
/****************************************************************************/
CAnalogInput::CAnalogInput(QString Caption, QString SubCaption, bool AutoStart) : CTestCaseBase(Caption, "", AutoStart)
{
    mp_TextLabel = new QLabel(SubCaption);
    mp_NumberLabel = new QLabel("100");

    AddWidget(2, mp_TextLabel);
    AddWidget(3, mp_NumberLabel);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CAnalogInput::~CAnalogInput()
{
    try {
        delete mp_NumberLabel;
        delete mp_TextLabel;
    }
    catch (...) {}
}

}  // end namespace TestCases
