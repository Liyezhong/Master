/****************************************************************************/
/*! \file ServiceDeviceController/Source/DeviceProcessor/CalibrationHandler.cpp
 *
 *  \brief Implementation file for class CalibrationHandler.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-12-24
 *  $Author:    $ Srivathsa HH
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

#include "Core/Include/CMessageString.h"
#include <ServiceDeviceController/Include/DeviceProcessor/CalibrationHandler.h>
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include <ServiceDeviceController/Include/ServiceDeviceController.h>
#include "DeviceControl/Include/SlaveModules/DigitalOutput.h"
#include "DeviceControl/Include/SlaveModules/BaseModule.h"
#include <ServiceDeviceController/Include/DeviceProcessor/DeviceProcessor.h>
#include <unistd.h>

//lint -e438
//lint -e550

namespace DeviceControl {

/****************************************************************************/
CalibrationHandler::CalibrationHandler(DeviceProcessor &DevProc)
    : m_rDeviceProcessor(DevProc)
{
   // mp_MotorOvenLid = Wrappers->mp_MotorOvenLid;
}

/****************************************************************************/
void CalibrationHandler::PerformOvenLidCalibrationInit()
{
    /*if(!mp_DeviceOven->Initialize())
    {
        emit ReturnCalibrationInitMessagetoMain(Service::CMessageString::MSG_CALIBRATION_FAILURE, false);
        Global::EventObject::Instance().RaiseEvent(EVENT_DEVICE_OVENINIT_FAILURE);
        return;
    }
    if(!mp_MotorOvenLid->DoReferenceRun())
    {
        emit ReturnCalibrationInitMessagetoMain(Service::CMessageString::MSG_CALIBRATION_FAILURE, false);
        Global::EventObject::Instance().RaiseEvent(EVENT_DEVICE_OVENLID_REFRUN_FAILURE);
        return;
    }*/

    /* Store present parameter set values */
    /*m_LimitSwitchFirstWidth = mp_DeviceOven->GetLimitSwitchFirstWidth();
    m_LimitSwitchSecondWidth = mp_DeviceOven->GetLimitSwitchSecondWidth();
    m_WidthPositionCode1 = mp_MotorOvenLid->GetWidthPositionCode1();
    m_WidthPositionCode2 = mp_MotorOvenLid->GetWidthPositionCode2();

    emit ReturnCalibrationInitMessagetoMain(Service::CMessageString::MSG_CALIBRATION_SUCCESS, true);*/
}

/****************************************************************************/
void CalibrationHandler::PerformOvenLidCalibration()
{

}

void CalibrationHandler::PerformPressureSensorCalibration()
{

}

/****************************************************************************/
void CalibrationHandler::OnCalibrateDevice(Service::DeviceCalibrationCmdType CmdType)
{
    qDebug()<<"CalibrationHandler::OnCalibrateDevice";

    switch(CmdType)
    {
    case Service::DEVICE_OVENLID_INIT:
        (void) PerformOvenLidCalibrationInit();
        break;
    case Service::DEVICE_PRESSURE_SENSOR:
        (void) PerformPressureSensorCalibration();
        break;
    case Service::DEVICE_OVENLID:
        (void) PerformOvenLidCalibration();
        break;
    default:
        emit ReturnErrorMessagetoMain(Service::CMessageString::MSG_CALIBRATION_UNKNOWNCOMMAND);
        break;
    }
}

} // end namespace DeviceControl
