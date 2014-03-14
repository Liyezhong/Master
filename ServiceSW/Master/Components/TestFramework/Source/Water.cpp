/****************************************************************************/
/*! \file Water.cpp
 *
 *  \brief Water implementation.
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

#include "TestFramework/Include/Water.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CWater::CWater(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_SerialTest = new TestCases::CUserInput("Serial number", "");

    for(qint32 i = 0; i < 6; i++) {
        mp_WaterValve[i] = new TestCases::CAnalogInput("Water Valve " + QString::number(i), "Liquid Level:", i);
    }

    mp_WasteWaterFill = new TestCases::CDigitalInput("Water fill test", "Block the drain first", false);
    mp_WasteWaterWait = new TestCases::CTimedTest("Wait two minutes");
    mp_WasteWaterRelease = new TestCases::CDigitalInput("Water release test", "Unblock the drain first");
    mp_WasteWaterLeakage = new TestCases::CDigitalOutput("Look for leaked water", "Is the tub leak-proof?");

    AddTestCase(mp_SerialTest);
    AddCaption("Water");

    for(qint32 i = 0; i < 6; i++) {
        AddTestCase(mp_WaterValve[i]);
    }

    AddCaption("Waste Water");
    AddTestCase(mp_WasteWaterFill);
    AddTestCase(mp_WasteWaterWait);
    AddTestCase(mp_WasteWaterRelease);
    AddTestCase(mp_WasteWaterRelease);
    AddTestCase(mp_WasteWaterLeakage);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CWater::~CWater()
{
    try {
        delete mp_WasteWaterLeakage;
        delete mp_WasteWaterRelease;
        delete mp_WasteWaterWait;
        delete mp_WasteWaterFill;
        delete mp_SerialTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
