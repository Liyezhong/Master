/****************************************************************************/
/*! \file Initialization.cpp
 *
 *  \brief Initialization implementation.
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

#include "TestCases/Include/Initialization.h"

namespace TestCases {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam SubCaption = Second title of the test
 */
/****************************************************************************/
CInitialization::CInitialization(QString SubCaption) : CTestCaseBase("Initialization", SubCaption, true)
{
    mp_TextLabel = new QLabel("Serial number:");
    mp_NumberLabel = new QLabel("12345678");

    AddWidget(2, mp_TextLabel);
    AddWidget(3, mp_NumberLabel);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CInitialization::~CInitialization()
{
    try {
        delete mp_NumberLabel;
        delete mp_TextLabel;
    }
    catch (...) {}
}

} // end namespace TestCases
