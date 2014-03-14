/****************************************************************************/
/*! \file GripperArm.h
 *
 *  \brief GripperArm definition.
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

#ifndef TESTFRAMEWORK_GRIPPERARM_H
#define TESTFRAMEWORK_GRIPPERARM_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/Initialization.h"
#include "TestCases/Include/StepperMotor.h"
#include "TestCases/Include/AnalogInput.h"
#include "TestCases/Include/IdReader.h"
#include "TestCases/Include/UserInput.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief Tests cases for the left and right gripper arm.
 */
/****************************************************************************/
class CGripperArm : public CTestScreen
{
public:
    CGripperArm(QWidget *p_Parent = 0);
    virtual ~CGripperArm();

private:
    // X-Axis
    TestCases::CUserInput *mp_XAxisSerialTest;          //!< Input for the serial number of the E-box
    TestCases::CUserInput *mp_XAxisFrequencyTest;       //!< Input field for the resonant frequency
    TestCases::CStepperMotor *mp_XAxisReferenceTest;    //!< Stepper motor refrence run
    TestCases::CStepperMotor *mp_XAxisProfile1Test;     //!< Stepper motor profile 1
    TestCases::CStepperMotor *mp_XAxisProfile2Test;     //!< Stepper motor profile 2
    // Y-Axis
    TestCases::CInitialization *mp_YAxisInitTest;       //!< Assembly group registration test
    TestCases::CUserInput *mp_YAxisFrequencyTest;       //!< Input field for the resonant frequency
    TestCases::CStepperMotor *mp_YAxisReferenceTest;    //!< Stepper motor refrence run
    TestCases::CStepperMotor *mp_YAxisProfile1Test;     //!< Stepper motor profile 1
    TestCases::CStepperMotor *mp_YAxisProfile2Test;     //!< Stepper motor profile 2
    // Z-Axis
    TestCases::CInitialization *mp_ZAxisInitTest;       //!< Assembly group registration test
    TestCases::CUserInput *mp_ZAxisFrequencyTest;       //!< Input field for the resonant frequency
    TestCases::CStepperMotor *mp_ZAxisReferenceTest;    //!< Stepper motor refrence run
    TestCases::CStepperMotor *mp_ZAxisProfile1Test;     //!< Stepper motor profile 1
    TestCases::CStepperMotor *mp_ZAxisProfile2Test;     //!< Stepper motor profile 2
    TestCases::CAnalogInput *mp_ZAxisFillingTest;       //!< Test of the liquid level sensor
    TestCases::CIdReader *mp_ZAxisRfidTest;             //!< Test of the RFID functionality
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_GRIPPERARM_H
