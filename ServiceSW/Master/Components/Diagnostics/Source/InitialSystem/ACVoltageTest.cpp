/****************************************************************************/
/*! \file ACVoltageTest.cpp
 *
 *  \brief Implementation of AC voltage selection test..
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-15
 *   $Author:  $ Sunny.Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Diagnostics/Include/InitialSystem/ACVoltageTest.h"
#include "MainMenu/Include/MessageDlg.h"

#include <QDebug>

namespace Diagnostics {

namespace InitialSystem {

CACVoltageTest::CACVoltageTest(QWidget *parent)
    : CTestBase(parent)
{
}

CACVoltageTest::~CACVoltageTest(void)
{
}

int CACVoltageTest::Run(void)
{
    return RETURN_OK;

}

void CACVoltageTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "AC Voltage Selection Self-test";
        QString Text = Title.append(" is running...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CACVoltageTest::ShowFailMessage(int Error)
{
    QString Title = "AC Voltage Selection Self-test";
    QString Text;
    if (Error == 1) {
        Text = "AC Voltage Selection Selftest Failed. <br>Please check " \
                "if mains input voltage is within [198-253V AC] or [90-132V AC]." \
                "Alternatively check resistance of Retort bottom heater, cable and function of ASB5." \
                "Resolve the issue accordingly and restart service software.<br>" \
                "Unless this error is resolved, all test functions using AC " \
                "heating are disabled. The instrument must not be used " \
                "by the user unless this error is resolved";
    }
    else if (Error == 2) {
        Text = "AC Voltage Selection Selftest Failed.<br>" \
                "Please check if mains input voltage is within [198-253V AC] or [90-132V AC]. " \
                "Alternatively check resistance of Retort bottom heater, cable and function of ASB5." \
                "Resolve the issue accordingly and restart service software.<br>" \
                "Unless this error is resolved, all test functions using AC heating are " \
                "disabled. The instrument must not be used by the user unless this error " \
                "is resolved.";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
