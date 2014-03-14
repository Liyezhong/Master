/****************************************************************************/
/*! \file StepperMotor.h
 *
 *  \brief StepperMotor definition.
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

#ifndef TESTCASES_STEPPERMOTOR_H
#define TESTCASES_STEPPERMOTOR_H

#include "TestCases/Include/TestCaseBase.h"
#include <QLabel>

namespace TestCases {

/****************************************************************************/
/**
 * \brief This class implements all test cases involving stepper motors.
 */
/****************************************************************************/
class CStepperMotor : public TestCases::CTestCaseBase
{
public:
    CStepperMotor(QString SubCaption, quint32 LightBarriers, bool AutoStart = true);
    virtual ~CStepperMotor();

private:
    QLabel *mp_LightBarrierText[2]; //!< Light barrier caption
    QLabel *mp_LightBarrierLed[2];  //!< Indicates if the barrier is active
};

} // end namespace TestCases

#endif // TESTCASES_STEPPERMOTOR_H
