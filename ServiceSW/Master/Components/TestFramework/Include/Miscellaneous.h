/****************************************************************************/
/*! \file Miscellaneous.h
 *
 *  \brief Miscellaneous definition.
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

#ifndef TESTFRAMEWORK_MISCELLANEOUS_H
#define TESTFRAMEWORK_MISCELLANEOUS_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/AnalogInput.h"
#include "TestCases/Include/DigitalInput.h"
#include "TestCases/Include/DigitalOutput.h"
#include "TestCases/Include/UserInput.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief This class contains all test steps regarding small components that
 *        cannot be assigned to any assembly group.
 */
/****************************************************************************/
class CMiscellaneous : public CTestScreen
{
public:
    CMiscellaneous(QWidget *p_Parent = 0);
    virtual ~CMiscellaneous();

private:
    TestCases::CUserInput *mp_SerialTest;           //!< Input for the serial number of the E-box
    TestCases::CDigitalOutput *mp_LightTest;        //!< Testing the main light of Colorado
    TestCases::CDigitalInput *mp_OpenCoverTest;     //!< Testing the opening of the cover
    TestCases::CDigitalInput *mp_CloseCoverTest;    //!< Testing the closing of the cover
    TestCases::CAnalogInput *mp_CurrentTest;        //!< Testing the electric current of the fans
    TestCases::CDigitalInput *mp_OpenExhaustTest;   //!< Testing the functionality of the exhaustion air unit
    TestCases::CDigitalInput *mp_CloseExhaustTest;  //!< Testing the purposely jammed unit
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_MISCELLANEOUS_H
