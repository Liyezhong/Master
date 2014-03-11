/****************************************************************************/
/*! \file RackTransfer.h
 *
 *  \brief RackTransfer definition.
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

#ifndef TESTFRAMEWORK_RACKTRANSFER_H
#define TESTFRAMEWORK_RACKTRANSFER_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/Initialization.h"
#include "TestCases/Include/StepperMotor.h"
#include "TestCases/Include/UserInput.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief Tests cases for the rack transfer assembly group.
 */
/****************************************************************************/
class CRackTransfer : public CTestScreen
{
public:
    CRackTransfer(QWidget *p_Parent = 0);
    virtual ~CRackTransfer();

private:
    TestCases::CInitialization *mp_InitTest;        //!< Assembly group registration test
    TestCases::CUserInput *mp_FrequencyTest;        //!< Input field for the resonant frequency
    TestCases::CStepperMotor *mp_ReferenceTest;     //!< Stepper motor refrence run
    TestCases::CStepperMotor *mp_StainerTest;       //!< Driving to the stainer side
    TestCases::CStepperMotor *mp_CoverslipperTest;  //!< Driving to the coverslipper side
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_RACKTRANSFER_H
