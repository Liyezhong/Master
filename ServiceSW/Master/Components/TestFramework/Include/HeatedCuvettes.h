/****************************************************************************/
/*! \file HeatedCuvettes.h
 *
 *  \brief HeatedCuvettes definition.
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

#ifndef TESTFRAMEWORK_HEATEDCUVETTES_H
#define TESTFRAMEWORK_HEATEDCUVETTES_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/Initialization.h"
#include "TestCases/Include/TimedTest.h"
#include "TestCases/Include/AnalogInput.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief This widget displays all test for the heated cuvette assembly
 *        group.
 */
/****************************************************************************/
class CHeatedCuvettes : public CTestScreen
{
public:
    CHeatedCuvettes(QWidget *p_Parent = 0);
    virtual ~CHeatedCuvettes();

private:
    TestCases::CInitialization *mp_InitTest;    //!< Assembly group registration test
    TestCases::CTimedTest *mp_TempTest;         //!< Testing the heating capabilities
    TestCases::CAnalogInput *mp_Sensor[12];     //!< Temperature sensor tests
    TestCases::CAnalogInput *mp_Current;        //!< Electric current range test
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_HEATEDCUVETTES_H
