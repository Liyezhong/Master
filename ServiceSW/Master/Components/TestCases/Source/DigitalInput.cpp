/****************************************************************************/
/*! \file DigitalInput.cpp
 *
 *  \brief DigitalInput implementation.
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

#include "TestCases/Include/DigitalInput.h"

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
CDigitalInput::CDigitalInput(QString Caption, QString SubCaption, bool AutoStart) : CTestCaseBase(Caption, SubCaption, AutoStart)
{
    QPixmap Pixmap(":/CheckBox/CheckBox-Disabled.png");

    mp_TextLabel = new QLabel("Switch:");
    mp_SwitchLabel = new QLabel();
    mp_SwitchLabel->setPixmap(Pixmap);

    if (AutoStart == true) {
        SetLayoutSpan(0, 3);
    }

    AddWidget(2, mp_TextLabel);
    AddWidget(3, mp_SwitchLabel);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDigitalInput::~CDigitalInput()
{
    try {
        delete mp_SwitchLabel;
        delete mp_TextLabel;
    }
    catch (...) {}
}

} // end namespace TestCases
