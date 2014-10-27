/****************************************************************************/
/*! \file MovementTest.cpp
 *
 *  \brief Implementation of Rotary valve movement test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-8
 *   $Author:  $ Arthur Li
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

#include "Diagnostics/Include/RotaryValve/MovementTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Diagnostics {

namespace RotaryValve {

CMovementTest::CMovementTest(CDiagnosticMessageDlg *dlg)
    : CTestBase()
    , mp_dlg(dlg)
{
}
 //lint -e1540
CMovementTest::~CMovementTest(void)
{
}

int CMovementTest::Run()
{
    qDebug() << "Rotary Valve movement test starts!";

    QString title((tr("Rotary Valve Movement Test")));
    QString text;

    int ret;

    text = tr("Please verify status of the retort. If there is any "
              "reagent or paraffin in the retort, abort this test and "
              "change to the \"Diagnostic_Retort_Reagent Drain\" "
              "function to drain the liquid back to the original "
               "position. Thereafter flush the retort if necessary.");

    ret = mp_dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
    if (ret == CDiagnosticMessageDlg::ABORT)
        return RETURN_ERR_FAIL;

    qreal RVSensor1TempCurrent;
    qreal RVSensor2TempCurrent;

    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("MovementTest");
    qreal RVSensor1TempMin = p_TestCase->GetParameter("RVSensor1TempMin").toFloat();
    qreal RVSensor2TempMin = p_TestCase->GetParameter("RVSensor2TempMin").toFloat();

    ret = dev->RVGetTemp(&RVSensor1TempCurrent, &RVSensor2TempCurrent);
    if (ret != RETURN_OK || RVSensor1TempCurrent < RVSensor1TempMin
            || RVSensor2TempCurrent < RVSensor2TempMin) {
        text = tr("Rotary Valve Movement Test failed. "
                  "Rotary Valve cannot rotate, due to the minimum "
                  "temperature has not been reached. Please check "
                  "resistance of temperature sensors, current of heating "
                  "element and function of ASB3. If no root cause "
                  "found, check main relay on ASB15 and cable "
                  "connections in addition. Exchange part accordingly.");
        mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    // RV initialize
    text = tr("Initializing the rotary valve...");
    mp_dlg->ShowWaitingDialog(title, text);
    if (dev->RVInitialize() != RETURN_OK) {
        text = tr("Rotary Valve Movement Test failed. "
                  "It might work in some minutes when "
                  "solidified paraffin in the rotary valve is "
                  "molten. Repeat initializing test again in "
                  "about 15mins. If it still fails in the second "
                  "try, exchange rotary valve, reboot the "
                  "Service Software and repeat this test.");
        mp_dlg->HideWaitingDialog();
        mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }
    mp_dlg->HideWaitingDialog();

    // tube position
    size_t pos[] = {
                    7,  // tube position 7
                    5,  // tube position 5
                    12, // tube position 12
                    2   // tube position 2
    };

    for (size_t i = 0; i < sizeof(pos)/sizeof(size_t); i++) {
        text = tr(" Rotating Rotary Valve to tube position ") + QString().sprintf("%u", pos[i]);
        mp_dlg->ShowWaitingDialog(title, text);
        if (dev->RVMovePosition(true, pos[i]) != RETURN_OK) {
            text = tr("Rotary Valve Movement Test failed. "
                      "It might work in some minutes when solidified paraffin in the rotary valve is molten. "
                      "Repeat initializing test again in about 15mins. If it still fails in the second try, "
                      "exchange rotary valve, reboot the Service Software and repeat this test.");
            mp_dlg->HideWaitingDialog();
            mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
            return RETURN_ERR_FAIL;
        }
    }

    mp_dlg->HideWaitingDialog();
    text = tr("Rotary Valve Movement Test successful.");
    mp_dlg->ShowMessage(title, text, RETURN_OK);
    return RETURN_OK;
}

} // namespace RotaryValve

} // namespace Diagnostics
