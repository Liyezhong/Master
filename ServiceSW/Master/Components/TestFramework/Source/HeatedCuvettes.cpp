/****************************************************************************/
/*! \file HeatedCuvettes.cpp
 *
 *  \brief HeatedCuvettes implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-10
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

#include "TestFramework/Include/HeatedCuvettes.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CHeatedCuvettes::CHeatedCuvettes(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_InitTest = new TestCases::CInitialization();
    mp_TempTest = new TestCases::CTimedTest("Temperature test", false);
    mp_Current = new TestCases::CAnalogInput("Heater", "Current:");

    AddTestCase(mp_InitTest);
    AddTestCase(mp_TempTest);

    for (qint32 i = 0; i < 12; i++) {
        if (i % 2 == 0) {
            mp_Sensor[i] = new TestCases::CAnalogInput("Cuvette "+QString::number(i/2+1), "Reagent temperature:");
        }
        else {
            mp_Sensor[i] = new TestCases::CAnalogInput("", "Hotplate temperature:");
        }
        AddTestCase(mp_Sensor[i]);
    }

    AddTestCase(mp_Current);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CHeatedCuvettes::~CHeatedCuvettes()
{
    try {
        delete mp_Current;

        for (qint32 i = 0; i < 12; i++) {
            delete mp_Sensor[i];
        }

        delete mp_TempTest;
        delete mp_InitTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
