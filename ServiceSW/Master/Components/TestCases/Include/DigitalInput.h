/****************************************************************************/
/*! \file DigitalInput.h
 *
 *  \brief DigitalInput definition.
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

#ifndef TESTCASES_DIGITALINPUT_H
#define TESTCASES_DIGITALINPUT_H

#include "TestCases/Include/TestCaseBase.h"
#include <QLabel>

namespace TestCases {

/****************************************************************************/
/**
 * \brief This class implements the test case for digital inputs.
 */
/****************************************************************************/
class CDigitalInput : public TestCases::CTestCaseBase
{
public:
    CDigitalInput(QString Caption, QString SubCaption = "", bool AutoStart = true);
    ~CDigitalInput();

private:
    QLabel *mp_TextLabel;   //!< Holding the sub caption
    QLabel *mp_SwitchLabel; //!< Displays if the the input is active
};

} // end namespace TestCases

#endif // TESTCASES_DIGITALINPUT_H
