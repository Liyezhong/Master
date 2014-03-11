/****************************************************************************/
/*! \file AnalogInput.h
 *
 *  \brief AnalogInput definition.
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

#ifndef TESTCASES_ANALOGINPUT_H
#define TESTCASES_ANALOGINPUT_H

#include "TestCases/Include/TestCaseBase.h"
#include <QLabel>

namespace TestCases {

/****************************************************************************/
/**
 * \brief This class implements the test casses for all analog input values.
 */
/****************************************************************************/
class CAnalogInput : public TestCases::CTestCaseBase
{
public:
    CAnalogInput(QString Caption, QString SubCaption, bool AutoStart = true);
    virtual ~CAnalogInput();

private:
    QLabel *mp_TextLabel;   //!< Holding the sub caption
    QLabel *mp_NumberLabel; //!< Displays the measured value
};

} // end namespace TestCases

#endif // TESTCASES_ANALOGINPUT_H
