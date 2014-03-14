/****************************************************************************/
/*! \file Oven.h
 *
 *  \brief Oven definition.
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

#ifndef TESTFRAMEWORK_OVEN_H
#define TESTFRAMEWORK_OVEN_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/Initialization.h"
#include "TestCases/Include/StepperMotor.h"
#include "TestCases/Include/UserInput.h"
#include "TestCases/Include/TimedTest.h"
#include "TestCases/Include/AnalogInput.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief This class is a container for all tests regarding the Colorado
 *        oven.
 */
/****************************************************************************/
class COven : public CTestScreen
{
public:
    COven(QWidget *p_Parent = 0);
    virtual ~COven();

private:
    TestCases::CInitialization *mp_InitTest;    //!< Assembly group registration test
    TestCases::CStepperMotor *mp_ReferenceTest; //!< Stepper motor refrence run
    TestCases::CStepperMotor *mp_OpenTest;      //!< Test opening the oven
    TestCases::CStepperMotor *mp_CloseTest;     //!< Test closing the oven
    TestCases::CUserInput *mp_DesiredTemp;      //!< User input for the desired temperature
    TestCases::CTimedTest *mp_TempTest;         //!< Testing the heating capabilities
    TestCases::CAnalogInput *mp_Sensor1;        //!< Test of the first temperature sensor
    TestCases::CAnalogInput *mp_Sensor2;        //!< Test of the second temperature sensor
    TestCases::CAnalogInput *mp_FanSpeed1;      //!< Test of the first ventilation fan
    TestCases::CAnalogInput *mp_FanSpeed2;      //!< Test of the second ventilation fan
    TestCases::CAnalogInput *mp_Current;        //!< Electric current range test
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_OVEN_H
