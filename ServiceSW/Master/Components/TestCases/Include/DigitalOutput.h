/****************************************************************************/
/*! \file DigitalOutput.h
 *
 *  \brief DigitalOutput definition.
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

#ifndef TESTCASES_DIGITALOUTPUT_H
#define TESTCASES_DIGITALOUTPUT_H

#include "TestCases/Include/TestCaseBase.h"
#include <QLabel>
#include <QPushButton>

namespace TestCases {

/****************************************************************************/
/**
 * \brief Test case class for all digital outputs.
 */
/****************************************************************************/
class CDigitalOutput : public TestCases::CTestCaseBase
{
public:
    CDigitalOutput(QString Caption, QString Question);
    virtual ~CDigitalOutput();

private:
    QLabel *mp_QuestionLabel;   //!< The question to the user
    QPushButton *mp_YesButton;  //!< Button answering yes
    QPushButton *mp_NoButton;   //!< Button answering no
};

} // end namespace TestCases

#endif // TESTCASES_DIGITALOUTPUT_H
