/****************************************************************************/
/*! \file OvenPreTest.cpp
 *
 *  \brief Implementation of Oven pre test..
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

#include "Diagnostics/Include/InitialSystem/OvenPreTest.h"
#include "MainMenu/Include/MessageDlg.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include <QDebug>

namespace Diagnostics {

namespace InitialSystem {

COvenPreTest::COvenPreTest(QWidget *parent)
    : CTestBase(parent)
{
}

COvenPreTest::~COvenPreTest(void)
{
}

int COvenPreTest::Run(void)
{
    ErrorCode_t Ret(RETURN_OK);

    qreal OvenTempTop(0);
    qreal OvenTempSensor1(0);
    qreal OvenTempSensor2(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SOvenPreTest");

    qreal DiffTemp = p_TestCase->GetParameter("OvenDiffTemp").toInt();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    ShowWaitingMessage(true);

    Ret = p_DevProc->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);

    if (Ret != RETURN_OK || (OvenTempSensor1-OvenTempSensor2) > DiffTemp) {
        ShowWaitingMessage(false);
        ShowFailMessage(1);
        return Ret;
    }

    qreal TopTargetTemp    = p_TestCase->GetParameter("OvenTopTargetTemp").toInt();
    qreal BottomTargetTemp = p_TestCase->GetParameter("OvenBottomTargetTemp").toInt();
    int WaitMSec = 3000;

    Ret = p_DevProc->OvenStartHeating(TopTargetTemp, BottomTargetTemp);
    ReportError_t ReportError;
    while(WaitMSec<=0) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        Ret = p_DevProc->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", 0, &ReportError);
        if (Ret == RETURN_OK) {
            if (ReportError.instanceID!=0 && (now-ReportError.errorTime)<=3*1000) {
                p_DevProc->OvenStopHeating();
                ShowWaitingMessage(false);
                ShowFailMessage(2);
                return RETURN_ERR_FAIL;
            }
        }

        WaitMSec -= 500;
    }

    return RETURN_OK;
}

void COvenPreTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Pre-test Oven";
        QString Text = Title.append(" is running ...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void COvenPreTest::ShowFailMessage(int ErrorCode)
{
    QString Title = "Pre-test Oven";
    QString Text;

    if (ErrorCode == 1) {
        Text = "Pre-test Oven failed .<br>" \
                "Detection of Oven temperature failed. Sequentially check " \
                "resistance of temperature sensors, function of ASB5 and oven heating elements." \
                "exchange ASB5 or oven accordingly and repeat this test.";
    }
    else if (ErrorCode == 2) {
        Text = "Pre-test Oven failed .<br>" \
                "Current of heating elements is out of specifications. Sequentially check " \
                "function of ASB5 and oven heating elements." \
                "Exchange ASB5 or oven accordingly and repeat this test .";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
