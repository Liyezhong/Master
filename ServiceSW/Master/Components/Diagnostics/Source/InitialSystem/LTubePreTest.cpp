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

#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

using namespace DeviceControl;

namespace Diagnostics {

namespace InitialSystem {

CLTubePreTest::CLTubePreTest(QWidget *p_Parent)
    : CTestBase(p_Parent)
{
}

CLTubePreTest::~CLTubePreTest(void)
{
}

int CLTubePreTest::Run(void)
{
    int Ret(RETURN_OK);

    qreal LTubeTempSensor(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SLTubePreTest");

    qreal RangeTemp = p_TestCase->GetParameter("LTubeRangeTemp").toInt();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    ShowWaitingMessage(true);
    qDebug()<<"LiquidTubeGetTemp";

    Ret = p_DevProc->LiquidTubeGetTemp(&LTubeTempSensor);
    qDebug()<<"LiquidTubeGetTemp --- "<<LTubeTempSensor;


    if (Ret != RETURN_OK || (LTubeTempSensor) >= RangeTemp) {
        ShowWaitingMessage(false);
        ShowFailMessage(1);
        return RETURN_ERR_FAIL;
    }

    qreal LTubeTargetTemp = p_TestCase->GetParameter("LTubeTargetTemp").toFloat();


    Ret = p_DevProc->LiquidTubeStartHeating(LTubeTargetTemp);

    Ret = p_DevProc->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", 1);

    (void)p_DevProc->LiquidTubeStopHeating();

    ShowWaitingMessage(false);
    if (Ret != RETURN_OK) {
        ShowFailMessage(2);
    }

    return Ret;
}

void CLTubePreTest::StartPreHeating()
{
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SLTubePreTest");
    qreal TargetTemp = p_TestCase->GetParameter("PreHeatingTargetTemp").toFloat();

    (void)ServiceDeviceProcess::Instance()->LiquidTubeStartHeating(TargetTemp);
}

void CLTubePreTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Pre-test Liquid Heating Tube";
        QString Text = Title + QString(" is running ...");
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
