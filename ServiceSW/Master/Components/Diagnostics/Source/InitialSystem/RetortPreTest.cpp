/****************************************************************************/
/*! \file RetortPreTest.cpp
 *
 *  \brief Implementation of Retort pre test..
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

#include "Diagnostics/Include/InitialSystem/RetortPreTest.h"
#include "MainMenu/Include/MessageDlg.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include <QDebug>

namespace Diagnostics {

namespace InitialSystem {

CRetortPreTest::CRetortPreTest(QWidget *parent)
    : CTestBase(parent)
{
}

CRetortPreTest::~CRetortPreTest(void)
{
}

int CRetortPreTest::Run(void)
{
    ErrorCode_t Ret(RETURN_OK);

    qreal RetortTempSide(0);
    qreal RetortTempBottom1(0);
    qreal RetortTempBottom2(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortPreTest");

    qreal DiffTemp = p_TestCase->GetParameter("RetortDiffTemp").toFloat();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    ShowWaitingMessage(true);

    Ret = p_DevProc->RetortGetTemp(&RetortTempSide, &RetortTempBottom1, &RetortTempBottom1);

    if (Ret != RETURN_OK || (RetortTempBottom1-RetortTempBottom1) > DiffTemp) {
        ShowWaitingMessage(false);
        ShowFailMessage(1);
        return Ret;
    }

    qreal RetortTargetTemp    = p_TestCase->GetParameter("RetortTargetTemp").toFloat();
    int WaitMSec = 3000;

    Ret = p_DevProc->RetortStartHeating(RetortTargetTemp, RetortTargetTemp);
    ReportError_t ReportError;
    while(WaitMSec>0) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        Ret = p_DevProc->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", 0, &ReportError);
        if (Ret == RETURN_OK) {
            if (ReportError.instanceID!=0 && (now-ReportError.errorTime)<=3*1000) {
                p_DevProc->RetortStopHeating();
                ShowWaitingMessage(false);
                ShowFailMessage(2);
                return RETURN_ERR_FAIL;
            }
        }

        WaitMSec -= 500;
    }

    ShowWaitingMessage(false);

    return RETURN_OK;
}

void CRetortPreTest::StartPreHeating(qreal MeltPoint)
{
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortPreTest");
    qreal MoreTargetTemp = p_TestCase->GetParameter("PreHeatingMoreTargetTemp").toFloat();

    ServiceDeviceProcess::Instance()->RetortStartHeating(MeltPoint+MoreTargetTemp+2, MeltPoint+MoreTargetTemp+7);
}

void CRetortPreTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Pre-test Retort";
        QString Text = Title + QString(" is running ...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CRetortPreTest::ShowFailMessage(int ErrorCode)
{
    QString Title = "Pre-test Retort";
    QString Text;

    if (ErrorCode == 1) {
        Text = "Pre-test Retort failed .<br>" \
                "Detection of Retort temperature failed. Sequentially check " \
                "resistance of temperature sensor, function of ASB5 and retort heating elements." \
                "exchange ASB5 or retort accordingly and repeat this test.";
    }
    else if (ErrorCode == 2) {
        Text = "Pre-test Retort failed .<br>" \
                "Current of heating elements is out of specifications. Sequentially check " \
                "function of ASB5 and retort heating elements." \
                "Exchange ASB5 or retort accordingly and repeat this test .";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
