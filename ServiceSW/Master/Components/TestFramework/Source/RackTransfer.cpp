/****************************************************************************/
/*! \file RackTransfer.cpp
 *
 *  \brief RackTransfer implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-20
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

#include "TestFramework/Include/RackTransfer.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CRackTransfer::CRackTransfer(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_InitTest = new TestCases::CInitialization();
    mp_FrequencyTest = new TestCases::CUserInput("Resonant frequency");
    mp_ReferenceTest = new TestCases::CStepperMotor("Reference run", 2, false);
    mp_StainerTest = new TestCases::CStepperMotor("Stainer transfer", 2);
    mp_CoverslipperTest = new TestCases::CStepperMotor("Coverslipper transfer", 2);

    AddTestCase(mp_InitTest);
    AddTestCase(mp_FrequencyTest);
    AddTestCase(mp_ReferenceTest);
    AddTestCase(mp_StainerTest);
    AddTestCase(mp_CoverslipperTest);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CRackTransfer::~CRackTransfer()
{
    try {
        delete mp_CoverslipperTest;
        delete mp_StainerTest;
        delete mp_ReferenceTest;
        delete mp_FrequencyTest;
        delete mp_InitTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
