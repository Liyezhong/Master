/****************************************************************************/
/*! \file LTubePreTest.cpp
 *
 *  \brief Implementation of liquid tube pre test..
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

#include "Diagnostics/Include/InitialSystem/LTubePreTest.h"
#include "MainMenu/Include/MessageDlg.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include <QDebug>

namespace Diagnostics {

namespace InitialSystem {

CLTubePreTest::CLTubePreTest(QWidget *parent)
    : CTestBase(parent)
{
}

CLTubePreTest::~CLTubePreTest(void)
{
}

int CLTubePreTest::Run(void)
{
    ErrorCode_t Ret(RETURN_OK);

    qreal LTubeTempSensor(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SLTubePreTest");

    qreal RangeTemp = p_TestCase->GetParameter("LTubeRangeTemp").toInt();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    ShowWaitingMessage(true);

    Ret = p_DevProc->LiquidTubeGetTemp(&LTubeTempSensor);

    if (Ret != RETURN_OK || (LTubeTempSensor) >= RangeTemp) {
        ShowWaitingMessage(false);
        ShowFailMessage(1);
        return Ret;
    }

    qreal LTubeTargetTemp = p_TestCase->GetParameter("LTubeTargetTemp").toInt();

    int WaitMSec = 3000;

    Ret = p_DevProc->LiquidTubeStartHeating(LTubeTargetTemp);
    ReportError_t ReportError;
    while(WaitMSec<=0) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        Ret = p_DevProc->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", 0, &ReportError);
        if (Ret == RETURN_OK) {
            if (ReportError.instanceID!=0 && (now-ReportError.errorTime)<=3*1000) {
                p_DevProc->LiquidTubeStopHeating();
                ShowWaitingMessage(false);
                ShowFailMessage(2);
                return RETURN_ERR_FAIL;
            }
        }

        WaitMSec -= 500;
    }

    return RETURN_OK;

}

void CLTubePreTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Pre-test Liquid Heating Tube";
        QString Text = Title.append(" is running ...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CLTubePreTest::ShowFailMessage(int ErrorCode)
{
    QString Title = "Pre-test Liquid Heating Tube";
    QString Text;

    if (ErrorCode == 1) {
        Text = "Pre-test Liquid Heating Tube failed .<br>" \
                "Detection of liquid heating tube temperature failed. Sequentially check " \
                "resistance of liquid heating tube temperature sensor and function of ASB15. " \
                "Exchange ASB15 or liquid heating tube accordingly and repeat this test.";
    }
    else if (ErrorCode == 2) {
        Text = "Pre-test Liquid Heating Tube failed .<br>" \
                "Current of liquid heating tube is out of specifications. Sequentially check " \
                "function of ASB15 and heating element of liquid heating tube." \
                "Exchange ASB15 or liquid heating tube accordingly and repeat this test .";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
