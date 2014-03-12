/****************************************************************************/
/*! \file UserInput.h
 *
 *  \brief UserInput definition.
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

#ifndef TESTCASES_USERINPUT_H
#define TESTCASES_USERINPUT_H

#include "TestCases/Include/TestCaseBase.h"
#include <QLabel>
#include <QPushButton>

namespace TestCases {

/****************************************************************************/
/**
 * \brief This test case class opens a virtual keyboard for user input and
 *        forwards the information to the master.
 */
/****************************************************************************/
class CUserInput : public CTestCaseBase
{
public:
    CUserInput(QString Caption, QString SubCaption = "");
    virtual ~CUserInput();

private:
    QPushButton *mp_EnterButton;    //!< The text that was entered
    QLabel *mp_InputLabel;          //!< The button starting the keyboard
};

} // end namespace TestCases

#endif // TESTCASES_USERINPUT_H
