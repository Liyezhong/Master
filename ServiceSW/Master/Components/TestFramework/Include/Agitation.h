/****************************************************************************/
/*! \file Agitation.h
 *
 *  \brief Agitation definition.
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

#ifndef TESTFRAMEWORK_AGITATION_H
#define TESTFRAMEWORK_AGITATION_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/StepperMotor.h"
#include "TestCases/Include/UserInput.h"
#include "TestCases/Include/DigitalOutput.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief This is the test screen for the agitation assembly group.
 */
/****************************************************************************/
class CAgitation : public CTestScreen
{
public:
    CAgitation(QWidget *p_Parent = 0);
    virtual ~CAgitation();

private:
    TestCases::CUserInput *mp_SerialTest;       //!< Input for the serial number of the E-box
    TestCases::CDigitalOutput *mp_PressureTest; //!< Pressure test for the water supply
    TestCases::CStepperMotor *mp_ReferenceTest; //!< Stepper motor refrence run
    TestCases::CStepperMotor *mp_Profile1Test;  //!< Stepper motor profile 1
    TestCases::CStepperMotor *mp_Profile2Test;  //!< Stepper motor profile 2
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_AGITATION_H
