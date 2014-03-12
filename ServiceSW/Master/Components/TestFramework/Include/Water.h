/****************************************************************************/
/*! \file Water.h
 *
 *  \brief Water definition.
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

#ifndef TESTFRAMEWORK_WATER_H
#define TESTFRAMEWORK_WATER_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/UserInput.h"
#include "TestCases/Include/DigitalInput.h"
#include "TestCases/Include/DigitalOutput.h"
#include "TestCases/Include/AnalogInput.h"
#include "TestCases/Include/TimedTest.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief This class holds all tests regarding the water and waste water
 *        functionality of Colorado.
 */
/****************************************************************************/
class CWater : public CTestScreen
{
public:
    CWater(QWidget *p_Parent = 0);
    virtual ~CWater();

private:
    TestCases::CUserInput *mp_SerialTest;               //!< Input for the serial number of the E-box
    TestCases::CDigitalInput *mp_WasteWaterFill;        //!< Filling the device with water
    TestCases::CTimedTest *mp_WasteWaterWait;           //!< Waiting for leakages
    TestCases::CDigitalInput *mp_WasteWaterRelease;     //!< Releasing the water
    TestCases::CDigitalOutput *mp_WasteWaterLeakage;    //!< Result input concerning leakage
    TestCases::CAnalogInput *mp_WaterValve[6];          //!< Testing the water valves
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_WATER_H
