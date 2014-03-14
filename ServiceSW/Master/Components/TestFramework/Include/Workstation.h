/****************************************************************************/
/*! \file Workstation.h
 *
 *  \brief Workstation definition.
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

#ifndef TESTFRAMEWORK_WORKSTATION_H
#define TESTFRAMEWORK_WORKSTATION_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/Initialization.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief This is the test screen for the workstation mode, when a Sepia
 *        device is attached.
 */
/****************************************************************************/
class CWorkstation : public CTestScreen
{
public:
    CWorkstation(QWidget *p_Parent = 0);
    virtual ~CWorkstation();

private:
    TestCases::CInitialization *mp_InitTest;    //!< Sepia registration test
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_WORKSTATION_H
