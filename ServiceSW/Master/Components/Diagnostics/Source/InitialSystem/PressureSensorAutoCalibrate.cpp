/****************************************************************************/
/*! \file PressureSensorAutoCalibrate.cpp
 *
 *  \brief Implementation of Pressure Sensor Auto Calibrate.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2015-10-22
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

#include "Diagnostics/Include/InitialSystem/PressureSensorAutoCalibrate.h"
#include "MainMenu/Include/MessageDlg.h"

#include <QDebug>
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include "Main/Include/HimalayaServiceEventCodes.h"

namespace Diagnostics {

namespace InitialSystem {

CPressureSensorAutoCalibrate::CPressureSensorAutoCalibrate(QWidget *p_Parent)
    : CTestBase(p_Parent)
{
}

CPressureSensorAutoCalibrate::~CPressureSensorAutoCalibrate(void)
{
}

int CPressureSensorAutoCalibrate::Run(void)
{
    QString TestName = "Pressure Sensor Calibration";

    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("Start %1.").arg(TestName));

    Diagnostics::ServiceDeviceProcess* p_Dev = Diagnostics::ServiceDeviceProcess::Instance();

    QString Text;
    float Pressure(0);
    float Drift(0);
    bool  Ret = false;

    ShowWaitingMessage(true);

    for (int i = 0; i < 3; ++i) {
        if (i==0) {
            Text = "Executing the first calibration, please wait...";

            // we reset the offset to 0 in case the old value is wrong after last failed calibration.
            (void)p_Dev->PumpWritePressureDrift(0);
        }
        else if (i==1) {
            Text = "Executing the second calibration, please wait...";
        }
        else if (i==2) {
            Text = "Executing the third calibration, please wait...";
        }

        (void)p_Dev->PumpSetValve(0, 0);
        (void)p_Dev->PumpSetValve(1, 0);
        p_Dev->Pause(20*1000);

        (void)p_Dev->PumpGetPressure(&Pressure);
        qDebug()<<"Get Pressure:"<<Pressure;
        if (qAbs(Pressure) < 0.2) {
            Ret = true;
            break;
        }
        //else if (qAbs(Pressure) < 1.5){
        else if (qAbs(Pressure) < 2.0){
            (void)p_Dev->PumpReadPressureDrift(&Drift);
            (void)p_Dev->PumpWritePressureDrift(Pressure + Drift);
        }
    }
    ShowWaitingMessage(false);

    if (Ret) {
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Successful.").arg(TestName));
        return RETURN_OK;
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Failed.").arg(TestName));
        ShowFailMessage(0);
        return RETURN_ERR_FAIL;
    }
}

void CPressureSensorAutoCalibrate::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Pressure Sensor Calibration";
        QString Text = Title + " is running...";
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CPressureSensorAutoCalibrate::ShowFailMessage(int Error)
{
    QString Title = "Pressure Sensor Calibration";
    QString Text = "Pressure Sensor Calibration failed. Please Enter SVC Main Display, manually calibrate the pressure sensor again.";

    Q_UNUSED(Error);
    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}


} // namespace InitialSystem

} // namespace Diagnostics
