/****************************************************************************/
/*! \file ServiceDeviceProcess.cpp
 *
 *  \brief Implementation file for class ServiceDeviceProcess.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-09-09
 *  $Author:    $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include <ServiceDeviceController/Include/ServiceDeviceController.h>
#include "DeviceControl/Include/SlaveModules/DigitalOutput.h"
#include "DeviceControl/Include/SlaveModules/BaseModule.h"
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmStepperMotor.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmTempControl.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmPressureControl.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperUtils.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalInput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBootLoader.h>

#include "Core/Include/CMessageString.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "Global/Include/SystemPaths.h"
#include "DeviceControl/Include/DeviceProcessing/DeviceLifeCycleRecord.h"

namespace Diagnostics {

#define RV_MOVE_OK      1
#define PRE_ALFA_TEST

/****************************************************************************/
ServiceDeviceProcess::ServiceDeviceProcess(IDeviceProcessing &iDevProc)
    : m_IsConfigured(false)
    , m_rIdevProc(iDevProc)
{
    mp_Utils = NULL;

    mp_TempOvenTop = NULL;
    mp_TempOvenBottom = NULL;
    mp_DIOven = NULL;
    mp_DOMainRelay = NULL;

    mp_TempRetortSide = NULL;
    mp_TempRetortBottom = NULL;
    mp_DIRetortLid = NULL;

    mp_TempTubeLiquid = NULL;
    mp_TempTubeAir = NULL;
    mp_MotorRV = NULL;
    mp_PressPump = NULL;
    mp_TempRV = NULL;

    mp_DIRemoteAlarm = NULL;
    mp_DILocalAlarm = NULL;

    mp_DORemoteAlarm = NULL;
    mp_DOLocalAlarm = NULL;

    mp_BaseModule3 = NULL;
    mp_BaseModule5 = NULL;
    mp_BaseModule15 = NULL;

    mp_TempLSensor = NULL;
}

/****************************************************************************/
void ServiceDeviceProcess::CreateWrappers()
{
    // Temperature control
    CTemperatureControl *pTemperature;

    pTemperature = NULL;
    pTemperature = static_cast<CTemperatureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_OVEN, CANObjectKeyLUT::m_OvenTopTempCtrlKey));
    if (NULL != pTemperature)
    {
        mp_TempOvenTop = new WrapperFmTempControl("temp_oven_top", pTemperature, this);
    }

    pTemperature = NULL;
    pTemperature = static_cast<CTemperatureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_OVEN, CANObjectKeyLUT::m_OvenBottomTempCtrlKey));
    if (NULL != pTemperature)
    {
        mp_TempOvenBottom = new WrapperFmTempControl("temp_oven_bottom", pTemperature, this);
    }

    pTemperature = NULL;
    pTemperature = static_cast<CTemperatureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_RETORT, CANObjectKeyLUT::m_RetortSideTempCtrlKey));
    if (NULL != pTemperature)
    {
        mp_TempRetortSide = new WrapperFmTempControl("temp_retort_side", pTemperature, this);
    }

    pTemperature = NULL;
    pTemperature = static_cast<CTemperatureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_RETORT, CANObjectKeyLUT::m_RetortBottomTempCtrlKey));
    if (NULL != pTemperature)
    {
        mp_TempRetortBottom = new WrapperFmTempControl("temp_retort_bottom", pTemperature, this);
    }

    CDigitalInput *pDigitalInput = NULL;
    pDigitalInput = static_cast<CDigitalInput *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_OVEN, CANObjectKeyLUT::m_OvenLidDIKey));
    if ( NULL != pDigitalInput ) {
        mp_DIOven = new WrapperFmDigitalInput("digitalinput_oven", pDigitalInput, this);
    }

    pDigitalInput = NULL;
    pDigitalInput = static_cast<CDigitalInput *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_RETORT, CANObjectKeyLUT::m_RetortLockDIKey));
    if ( NULL != pDigitalInput ) {
        mp_DIRetortLid = new WrapperFmDigitalInput("digitalinput_retortlid", pDigitalInput, this);
    }

    pDigitalInput = NULL;
    pDigitalInput = static_cast<CDigitalInput *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_MAIN_CONTROL, CANObjectKeyLUT::m_PerRemoteAlarmDIKey));
    if ( NULL != pDigitalInput ) {
        mp_DIRemoteAlarm = new WrapperFmDigitalInput("remote_alarm_digital_input", pDigitalInput, this);
    }

    pDigitalInput = NULL;
    pDigitalInput = static_cast<CDigitalInput *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_MAIN_CONTROL, CANObjectKeyLUT::m_PerLocalAlarmDIKey));
    if ( NULL != pDigitalInput ) {
        mp_DILocalAlarm = new WrapperFmDigitalInput("local_alarm_digital_input", pDigitalInput, this);
    }

    CDigitalOutput *pDigitalOutput = NULL;
    pDigitalOutput = static_cast<CDigitalOutput*>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_MAIN_CONTROL, CANObjectKeyLUT::m_PerMainRelayDOKey));
    if ( NULL != pDigitalOutput ) {
        mp_DOMainRelay = new WrapperFmDigitalOutput("heater_relay", pDigitalOutput, this);
    }

    pDigitalOutput = NULL;
    pDigitalOutput = static_cast<CDigitalOutput*>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_MAIN_CONTROL, CANObjectKeyLUT::m_PerRemoteAlarmCtrlDOKey));
    if ( NULL != pDigitalOutput ) {
        mp_DORemoteAlarm = new WrapperFmDigitalOutput("remote_alarm_digital_output", pDigitalOutput, this);
    }

    pDigitalOutput = NULL;
    pDigitalOutput = static_cast<CDigitalOutput*>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_MAIN_CONTROL, CANObjectKeyLUT::m_PerLocalAlarmCtrlDOKey));
    if ( NULL != pDigitalOutput ) {
        mp_DOLocalAlarm = new WrapperFmDigitalOutput("local_alarm_digital_output", pDigitalOutput, this);
    }

    pTemperature = NULL;
    pTemperature = static_cast<CTemperatureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_AIR_LIQUID, CANObjectKeyLUT::m_ALTube1TempCtrlKey));
    if (NULL != pTemperature)
    {
        mp_TempTubeLiquid = new WrapperFmTempControl("temp_tube1", pTemperature, this);
    }

    pTemperature = NULL;
    pTemperature = static_cast<CTemperatureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_AIR_LIQUID, CANObjectKeyLUT::m_ALTube2TempCtrlKey));
    if (NULL != pTemperature)
    {
        mp_TempTubeAir = new WrapperFmTempControl("temp_tube2", pTemperature, this);
    }

    CStepperMotor *pMotor = NULL;
    pMotor = static_cast<CStepperMotor *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_ROTARY_VALVE, CANObjectKeyLUT::m_RVMotorKey));
    if (NULL != pMotor)
    {
        mp_MotorRV = new WrapperFmStepperMotor("motor_rv", pMotor, this);
    }

    pTemperature = NULL;
    pTemperature = static_cast<CTemperatureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_ROTARY_VALVE, CANObjectKeyLUT::m_RVTempCtrlKey));
    if (NULL != pTemperature)
    {
        mp_TempRV = new WrapperFmTempControl("temp_rv", pTemperature, this);
    }

    pTemperature = NULL;
    pTemperature = static_cast<CTemperatureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_AIR_LIQUID, CANObjectKeyLUT::m_ALLevelSensorTempCtrlKey));
    if (NULL != pTemperature)
    {
        mp_TempLSensor = new WrapperFmTempControl("temp_lsensor", pTemperature, this);

    }

    CPressureControl *pPressure = NULL;
    pPressure = static_cast<CPressureControl *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_AIR_LIQUID, CANObjectKeyLUT::m_ALPressureCtrlKey));
    if (NULL != pPressure)
    {
        mp_PressPump = new WrapperFmPressureControl("pressurectrl", pPressure, this);
        connect(pTemperature, SIGNAL(ReportLevelSensorState(quint32, ReturnCode_t, quint8)), mp_PressPump, SLOT(OnLevelSensorState(quint32,ReturnCode_t,quint8)));
    }

    CBaseModule *pBaseModule = NULL;
    pBaseModule = static_cast<CBaseModule *> (m_rIdevProc.GetBaseModule(Slave_3));
    if (NULL != pBaseModule) {
        mp_BaseModule3 = new WrapperFmBaseModule("asb3_0", pBaseModule, this);
    }

    pBaseModule = static_cast<CBaseModule *> (m_rIdevProc.GetBaseModule(Slave_5));
    if (NULL != pBaseModule) {
        mp_BaseModule5 = new WrapperFmBaseModule("asb5_0", pBaseModule, this);
    }

    pBaseModule = static_cast<CBaseModule *> (m_rIdevProc.GetBaseModule(Slave_15));
    if (NULL != pBaseModule) {
        mp_BaseModule15 = new WrapperFmBaseModule("asb15_0", pBaseModule, this);
    }

}

/****************************************************************************/
void ServiceDeviceProcess::Initialize()
{
    (void) CreateWrappers();
    m_IsConfigured = true;

    mp_Utils = new WrapperUtils(this);
}

/****************************************************************************/
bool ServiceDeviceProcess::IsInitialized()
{
    return m_IsConfigured;
}

ErrorCode_t ServiceDeviceProcess::MainRelaySetOnOff(bool OnFlag)
{
    if (mp_DOMainRelay == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    bool Ret = true;
    if (OnFlag) {
        Ret = mp_DOMainRelay->SetHigh();
    }
    else {
        Ret = mp_DOMainRelay->SetLow();
    }

    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::AlarmSetOnOff(int LocalRemote, bool OnFlag)
{
    bool Ret = true;
    if (LocalRemote == 1) {  // Local Alarm
        if (mp_DOLocalAlarm == NULL) {
            return RETURN_ERR_NULL_POINTER;
        }
        if (OnFlag) {
            Ret = mp_DOLocalAlarm->SetLow();
        }
        else {
            Ret = mp_DOLocalAlarm->SetHigh();
        }
    }
    else if (LocalRemote == 2) { // Remote Alarm
        if (mp_DORemoteAlarm == NULL) {
            return RETURN_ERR_NULL_POINTER;
        }
        if (OnFlag) {
            Ret = mp_DORemoteAlarm->SetLow();
        }
        else {
            Ret = mp_DORemoteAlarm->SetHigh();
        }
    }
    else {
        return RETURN_ERR_NULL_POINTER;
    }

    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}
ErrorCode_t ServiceDeviceProcess::AlarmGetState(int LocalRemote, qint32 *RetState)
{
    if (LocalRemote == 1) {  // Local Alarm
        if (mp_DILocalAlarm == NULL || RetState == NULL) {
            return RETURN_ERR_NULL_POINTER;
        }
        *RetState = mp_DILocalAlarm->GetValue();
        return RETURN_OK;
    }
    else if (LocalRemote == 2) { // Remote Alarm
        if (mp_DIRemoteAlarm == NULL || RetState == NULL) {
            return RETURN_ERR_NULL_POINTER;
        }
        *RetState = mp_DIRemoteAlarm->GetValue();
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_NULL_POINTER;
    }
}
ErrorCode_t ServiceDeviceProcess::MainControlGetCurrent(HimSlaveType_t SlaveType, quint16 *RetCurrent)
{
    if (RetCurrent == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetCurrent = m_rIdevProc.IDGetSlaveCurrent(SlaveType);

    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::MainControlGetVoltage(HimSlaveType_t SlaveType, quint16 *RetVoltage)
{
    if (RetVoltage == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetVoltage = m_rIdevProc.IDGetSlaveVoltage(SlaveType);

    return RETURN_OK;
}


ErrorCode_t ServiceDeviceProcess::OvenStartHeating(qreal TargetTempTop, qreal TargetTempBottom)
{
    bool Ret(true);

    if (mp_TempOvenBottom == NULL || mp_TempOvenTop == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    mp_TempOvenBottom->StopTemperatureControl();
    mp_TempOvenTop->StopTemperatureControl();

    Ret = mp_TempOvenBottom->StartTemperatureControl(TargetTempBottom);
    Ret |= mp_TempOvenTop->StartTemperatureControl(TargetTempTop);

    if (Ret) {
        return RETURN_OK;
    }
    else {
        mp_TempOvenBottom->StopTemperatureControl();
        mp_TempOvenTop->StopTemperatureControl();
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::OvenStopHeating()
{
    bool Ret(true);

    if (mp_TempOvenBottom == NULL || mp_TempOvenTop == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    Ret = mp_TempOvenBottom->StopTemperatureControl();
    Ret |= mp_TempOvenTop->StopTemperatureControl();

    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::OvenGetTemp(qreal *RetTempTop, qreal *RetTempBottom1, qreal *RetTempBottom2)
{
    if (mp_TempOvenBottom == NULL || mp_TempOvenTop == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    if (RetTempTop) {
        *RetTempTop = mp_TempOvenTop->GetTemperature();
    }
    if (RetTempBottom1) {
        *RetTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
    }
    if (RetTempBottom2) {
        *RetTempBottom2 = mp_TempOvenBottom->GetTemperature(1);
    }

    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::OvenGetCurrent(qreal *RetCurrentTop, qreal *RetCurrentBottom)
{
    if (mp_TempOvenBottom == NULL || mp_TempOvenTop == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    if (RetCurrentTop) {
        *RetCurrentTop = mp_TempOvenTop->GetCurrent();
    }
    if (RetCurrentBottom) {
        *RetCurrentBottom = mp_TempOvenBottom->GetCurrent();
    }

    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::OvenGetCoverSensorState(qint32 *RetCoverSensorState)
{
    if (mp_DIOven == NULL || RetCoverSensorState == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetCoverSensorState = mp_DIOven->GetValue();
    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::OvenGetSwithType(int *RetSwithType)
{
    if (mp_DIOven == NULL || RetSwithType == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetSwithType = mp_TempOvenBottom->GetHeaterSwitchType();
    return RETURN_OK;
}


ErrorCode_t ServiceDeviceProcess::RetortStartHeating(qreal TargetTempSide, qreal TargetTempBottom)
{
    bool Ret(true);
    if (mp_TempRetortBottom == NULL || mp_TempRetortSide == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    mp_TempRetortBottom->StopTemperatureControl();
    mp_TempRetortSide->StopTemperatureControl();

    Ret = mp_TempRetortBottom->StartTemperatureControl(TargetTempBottom);
    Ret |= mp_TempRetortSide->StartTemperatureControl(TargetTempSide);

    if (Ret) {
        return RETURN_OK;
    }
    else {
        mp_TempRetortBottom->StopTemperatureControl();
        mp_TempRetortSide->StopTemperatureControl();
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::RetortStopHeating()
{
    bool Ret(true);
    if (mp_TempRetortBottom == NULL || mp_TempRetortSide == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }
    Ret = mp_TempRetortBottom->StopTemperatureControl();
    Ret |= mp_TempRetortSide->StopTemperatureControl();
    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::RetortGetTemp(qreal *RetTempSide, qreal *RetTempBottom1, qreal *RetTempBottom2)
{
    if (mp_TempRetortBottom == NULL || mp_TempRetortSide == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    if (RetTempSide) {
        *RetTempSide = mp_TempRetortSide->GetTemperature();
    }
    if (RetTempBottom1) {
        *RetTempBottom1 = mp_TempRetortBottom->GetTemperature(0);
    }
    if (RetTempBottom2) {
        *RetTempBottom2 = mp_TempRetortBottom->GetTemperature(1);
    }

    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::RetortGetCurrent(qreal *RetCurrentSide, qreal *RetCurrentBottom)
{
    if (mp_TempRetortBottom == NULL || mp_TempRetortSide == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    if (RetCurrentSide) {
        *RetCurrentSide = mp_TempRetortSide->GetCurrent();
    }
    if (RetCurrentBottom) {
        *RetCurrentBottom = mp_TempRetortBottom->GetCurrent();
    }

    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::RetortGetLidLockState(qint32 *RetLidLockState)
{
    if (mp_DIRetortLid == NULL || RetLidLockState == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetLidLockState = mp_DIRetortLid->GetValue();

    return RETURN_OK;
}


ErrorCode_t ServiceDeviceProcess::LiquidTubeStartHeating(qreal TargetTemp)
{
    bool Ret(true);
    if (mp_TempTubeLiquid == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    mp_TempTubeLiquid->StopTemperatureControl();
    Ret = mp_TempTubeLiquid->StartTemperatureControl(TargetTemp);

    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::LiquidTubeStopHeating()
{
    bool Ret(true);

    if (mp_TempTubeLiquid == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    Ret = mp_TempTubeLiquid->StopTemperatureControl();
    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::LiquidTubeGetTemp(qreal *RetTemp)
{
    if (mp_TempTubeLiquid == NULL || RetTemp == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetTemp = mp_TempTubeLiquid->GetTemperature();
    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::LiquidTubeGetCurrent(quint16 *RetCurrent)
{
    if (mp_TempTubeLiquid == NULL || RetCurrent == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetCurrent = mp_TempTubeLiquid->GetCurrent();
    return RETURN_OK;
}


ErrorCode_t ServiceDeviceProcess::AirTubeStartHeating(qreal TargetTemp)
{
    bool Ret(true);
    if (mp_TempTubeAir == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    mp_TempTubeAir->StopTemperatureControl();
    Ret = mp_TempTubeAir->StartTemperatureControl(TargetTemp);

    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::AirTubeStopHeating()
{
    bool Ret(true);

    if (mp_TempTubeAir == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    Ret = mp_TempTubeAir->StopTemperatureControl();
    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::AirTubeGetTemp(qreal *RetTemp)
{
    if (mp_TempTubeAir == NULL || RetTemp == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetTemp = mp_TempTubeAir->GetTemperature();
    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::AirTubeGetCurrent(quint16 *RetCurrent)
{
    if (mp_TempTubeAir == NULL || RetCurrent == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    *RetCurrent = mp_TempTubeAir->GetCurrent();
    return RETURN_OK;
}


ErrorCode_t ServiceDeviceProcess::RVStartHeating(qreal TargetTemp)
{
    bool Ret(true);

    if (mp_TempRV == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    mp_TempRV->StopTemperatureControl();
    Ret = mp_TempRV->StartTemperatureControl(TargetTemp);

    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::RVStopHeating()
{
    bool Ret(true);

    if (mp_TempRV == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    Ret = mp_TempRV->StopTemperatureControl();

    if (Ret) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::RVGetTemp(qreal *RetTempSensor1, qreal* RetTempSensor2)
{
    if (mp_TempRV == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    if (RetTempSensor1) {
        *RetTempSensor1 = mp_TempRV->GetTemperature(0);
    }

    if (RetTempSensor2) {
        *RetTempSensor2 = mp_TempRV->GetTemperature(1);
    }

    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::RVGetCurrent(quint16 *RetCurrent)
{
    if (mp_TempRV == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    if (RetCurrent) {
        *RetCurrent = mp_TempRV->GetCurrent();
    }

    return RETURN_OK;
}

ErrorCode_t ServiceDeviceProcess::RVInitialize()
{
    int Ret(0);
    if (mp_MotorRV == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    Ret = mp_MotorRV->MoveToInitialPosition();

    if (Ret == RV_MOVE_OK) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}

ErrorCode_t ServiceDeviceProcess::RVMovePosition(bool TubeFlag, int Position)
{
    int Ret(0);
    if (mp_MotorRV == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    if (TubeFlag) {
        Ret = mp_MotorRV->MoveToTubePosition(Position);
    }
    else {
        Ret = mp_MotorRV->MoveToSealPosition(Position);
    }

    if (Ret == RV_MOVE_OK) {
        return RETURN_OK;
    }
    else {
        return RETURN_ERR_FAIL;
    }
}


ErrorCode_t ServiceDeviceProcess::LSStartHeating(bool QuickFlag, bool WaterFlag)
{

}

ErrorCode_t ServiceDeviceProcess::LSStopHeating()
{

}

ErrorCode_t ServiceDeviceProcess::LSGetTemp(qreal *RetTemp)
{

}

ErrorCode_t ServiceDeviceProcess::LSGetCurrent(qreal *RetCurrent)
{

}


ErrorCode_t ServiceDeviceProcess::PumpBuildPressure(float TargetPressure)
{

}

ErrorCode_t ServiceDeviceProcess::PumpReleasePressure()
{

}

ErrorCode_t ServiceDeviceProcess::PumpGetPressure(float *RetPressure)
{

}

ErrorCode_t ServiceDeviceProcess::PumpSetFan(bool OnFlag)
{

}

ErrorCode_t ServiceDeviceProcess::PumpSetValve(quint8 ValveIndex, quint8 ValveState)
{

}

ErrorCode_t ServiceDeviceProcess::PumpStopCompressor()
{

}

ErrorCode_t ServiceDeviceProcess::PumpSucking(int DelayTime)
{

}

ErrorCode_t ServiceDeviceProcess::PumpDraining()
{

}



} // end namespace Diagnostics
