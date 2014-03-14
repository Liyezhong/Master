/****************************************************************************/
/*! \file Initialization.h
 *
 *  \brief Initialization definition.
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

#ifndef TESTCASES_INITIALIZATION_H
#define TESTCASES_INITIALIZATION_H

#include "TestCases/Include/TestCaseBase.h"
#include <QLabel>

namespace TestCases {

/****************************************************************************/
/**
 * \brief This test case checks, if a certain assembly group is connected to
 *        the communication bus of the device.
 */
/****************************************************************************/
class CInitialization : public TestCases::CTestCaseBase
{
public:
    CInitialization(QString SubCaption = "");
    virtual ~CInitialization();

private:
    QLabel *mp_TextLabel;   //!< Holding the sub caption
    QLabel *mp_NumberLabel; //!< Shows serial of the assembly group
};

} // end namespace TestCases

#endif // TESTCASES_INITIALIZATION_H
