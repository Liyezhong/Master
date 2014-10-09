/****************************************************************************/
/*! \file ExhaustFanTest.cpp
 *
 *  \brief Implementation of System Exhaust Fan test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-10-08
 *   $Author:  $ Dixiong Li
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

#include "Diagnostics/Include/System/ExhaustFanTest.h"

#include <QDebug>
namespace Diagnostics {

namespace System {

CExhaustFanTest::CExhaustFanTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_MessageDlg(p_MessageDlg)
{
}

CExhaustFanTest::~CExhaustFanTest(void)
{
}

int CExhaustFanTest::Run(void)
{
    qDebug() << "Exhaust fan Test starts!";

    (void)ServiceDeviceProcess::Instance()->PumpSetFan(1);

    int Ret = ShowConfirmDlg(1);
    int RetNum = 1;

    if (Ret == 1) {
        RetNum = 3;
    }
    else {
        Ret = ShowConfirmDlg(2);
        if (Ret == 1) {
            RetNum = 2;
        }
    }

    (void)ServiceDeviceProcess::Instance()->PumpSetFan(0);
    ShowFinishDlg(RetNum);
}

int CExhaustFanTest::ShowConfirmDlg(int StepNum)
{
    QString Title = "Exhaust Fan Test";
    QString Text;

    if (StepNum == 1) {
        Text = "Please disconnected the exhaust hose (if connected), " \
                "remove the carbon filter and close its cover. "       \
                "Please verify the fan works. Can you feel an"         \
                "airflow out of the instrument?";
    }
    else if (StepNum == 2) {
        Text = "Can you feel the airflow into the instrument?";
    }

    return mp_MessageDlg->ShowConfirmMessage(Title, Text);
}

void CExhaustFanTest::ShowFinishDlg(int RetNum)
{
    QString Title = "Exhaust Fan Test";
    QString Text;

    ErrorCode_t Ret = RETURN_ERR_FAIL;

    if (RetNum == 1) {
        Text = "Exhaust Fan Test failed. Fan is not turning, please check wiring and exchange the fan is necessary.";
    }
    else if (RetNum == 2) {
        Text = "Exhaust Fan Test failed. Please check for proper installation, correct wiring to ensure the airflow goes "\
                "out of instrument?";
    }
    else if (RetNum == 3) {
        Text = "Exhaust Fan Test successful.";
        Ret = RETURN_OK;
    }

    mp_MessageDlg->ShowMessage(Title, Text, Ret);
}

} // namespace System

} // namespace Diagnostics

