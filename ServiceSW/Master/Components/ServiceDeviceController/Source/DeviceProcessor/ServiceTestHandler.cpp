/****************************************************************************/
/*! \file ServiceTestHandler.cpp
 *
 *  \brief Implementation file for class ServiceTestHandler.
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

#include "ServiceDeviceController/Include/DeviceProcessor/ServiceTestHandler.h"
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

namespace DeviceControl {

#define RV_MOVE_OK      1

ServiceTestHandler* ServiceTestHandler::mp_Instance = NULL;

ServiceTestHandler* ServiceTestHandler::NewInstance(DeviceControl::IDeviceProcessing &iDevProc)
{
    if (mp_Instance==NULL) {
        mp_Instance = new ServiceTestHandler(iDevProc);
    }
    return mp_Instance;
}

ServiceTestHandler* ServiceTestHandler::Instance()
{
    return mp_Instance;
}

void ServiceTestHandler::Destroy()
{
    if (mp_Instance) {
        delete mp_Instance;
    }
    mp_Instance = NULL;
}

/****************************************************************************/
ServiceTestHandler::ServiceTestHandler(DeviceControl::IDeviceProcessing &iDevProc)
    : m_IsConfigured(false),
      m_rIdevProc(iDevProc)
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
void ServiceTestHandler::CreateWrappers()
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
void ServiceTestHandler::Initialize()
{
    (void) CreateWrappers();
    m_IsConfigured = true;

    mp_Utils = new WrapperUtils(this);
}

/****************************************************************************/
bool ServiceTestHandler::IsInitialized()
{
    return m_IsConfigured;
}

void ServiceTestHandler::MainRelaySetOnOff(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    bool OnFlag = (bool) Params.at(0).toInt();

    if (mp_DOMainRelay == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    bool Ret = true;
    if (OnFlag) {
        Ret = mp_DOMainRelay->SetHigh();
    }
    else {
        Ret = mp_DOMainRelay->SetLow();
    }

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::AlarmSetOnOff(QString& ReqName, QStringList& Params)
{
    int LocalRemote = Params.at(0).toInt();
    bool OnFlag = (bool) Params.at(1).toInt();
    bool Ret = true;
    QStringList Results;
    Results.clear();

    if (LocalRemote == 1) {  // Local Alarm
        if (mp_DOLocalAlarm == NULL) {
            emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
            return ;
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
            emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        }
        if (OnFlag) {
            Ret = mp_DORemoteAlarm->SetLow();
        }
        else {
            Ret = mp_DORemoteAlarm->SetHigh();
        }
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
    }

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::AlarmGetState(QString& ReqName, QStringList& Params )
{
    int LocalRemote = Params.at(0).toInt();
    qint32 RetState;

    QStringList Results;
    Results.clear();

    if (LocalRemote == 1) {  // Local Alarm
        if (mp_DILocalAlarm == NULL) {
            emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
            return ;
        }
        RetState = mp_DILocalAlarm->GetValue();
    }
    else if (LocalRemote == 2) { // Remote Alarm
        if (mp_DIRemoteAlarm == NULL) {
            emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
            return ;
        }
        RetState = mp_DIRemoteAlarm->GetValue();
    }

    Results.append(QString("%1").arg(RetState));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}
void ServiceTestHandler::MainControlGetCurrent(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    HimSlaveType_t SlaveType = (HimSlaveType_t)Params.at(0).toInt();

    quint16 Current = m_rIdevProc.IDGetSlaveCurrent(SlaveType);

    Results.append(QString("%1").arg(Current));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::MainControlGetVoltage(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    HimSlaveType_t SlaveType = (HimSlaveType_t)Params.at(0).toInt();

    quint16 Voltage = m_rIdevProc.IDGetSlaveVoltage(SlaveType);

    Results.append(QString("%1").arg(Voltage));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}


void ServiceTestHandler::OvenStartHeating(QString& ReqName, QStringList& Params)
{
    qreal TargetTempTop = Params.at(0).toFloat();
    qreal TargetTempBottom = Params.at(1).toFloat();
    bool Ret(true);
    QStringList Results;
    Results.clear();
    if (mp_TempOvenBottom == NULL || mp_TempOvenTop == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    mp_TempOvenBottom->StopTemperatureControl();
    mp_TempOvenTop->StopTemperatureControl();

    if (TargetTempBottom > 0) {
        Ret = mp_TempOvenBottom->StartTemperatureControl(TargetTempBottom);
    }
    if (TargetTempTop > 0) {
        Ret |= mp_TempOvenTop->StartTemperatureControl(TargetTempTop);
    }

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        mp_TempOvenBottom->StopTemperatureControl();
        mp_TempOvenTop->StopTemperatureControl();
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::OvenStopHeating(QString& ReqName, QStringList& Params)
{
    bool Ret(true);
    QStringList Results;
    Results.clear();
    if (mp_TempOvenBottom == NULL || mp_TempOvenTop == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    Ret = mp_TempOvenBottom->StopTemperatureControl();
    Ret |= mp_TempOvenTop->StopTemperatureControl();

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::OvenGetTemp(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempOvenBottom == NULL || mp_TempOvenTop == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qreal RetTempTop = mp_TempOvenTop->GetTemperature();
    qreal RetTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
    qreal RetTempBottom2 = mp_TempOvenBottom->GetTemperature(1);

    Results.append(QString("%1").arg(RetTempTop));
    Results.append(QString("%1").arg(RetTempBottom1));
    Results.append(QString("%1").arg(RetTempBottom2));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::OvenGetCurrent(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempOvenBottom == NULL || mp_TempOvenTop == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    quint16 RetCurrentTop = mp_TempOvenTop->GetCurrent();
    quint16 RetCurrentBottom = mp_TempOvenBottom->GetCurrent();

    Results.append(QString("%1").arg(RetCurrentTop));
    Results.append(QString("%1").arg(RetCurrentBottom));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::OvenGetCoverSensorState(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    if (mp_DIOven == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qint32 RetCoverSensorState = mp_DIOven->GetValue();

    Results.append(QString("%1").arg(RetCoverSensorState));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::OvenGetSwithType(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    if (mp_DIOven == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    int RetSwithType = mp_TempOvenBottom->GetHeaterSwitchType();

    Results.append(QString("%1").arg(RetSwithType));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}


void ServiceTestHandler::RetortStartHeating(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    bool Ret(true);
    if (mp_TempRetortBottom == NULL || mp_TempRetortSide == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qreal TargetTempSide = Params.at(0).toFloat();
    qreal TargetTempBottom = Params.at(1).toFloat();

    mp_TempRetortBottom->StopTemperatureControl();
    mp_TempRetortSide->StopTemperatureControl();

    Ret = mp_TempRetortBottom->StartTemperatureControl(TargetTempBottom);
    Ret |= mp_TempRetortSide->StartTemperatureControl(TargetTempSide);

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        mp_TempRetortBottom->StopTemperatureControl();
        mp_TempRetortSide->StopTemperatureControl();
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::RetortStopHeating(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    bool Ret(true);
    if (mp_TempRetortBottom == NULL || mp_TempRetortSide == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }
    Ret = mp_TempRetortBottom->StopTemperatureControl();
    Ret |= mp_TempRetortSide->StopTemperatureControl();
    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::RetortGetTemp(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempRetortBottom == NULL || mp_TempRetortSide == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qreal RetTempSide = mp_TempRetortSide->GetTemperature();
    qreal RetTempBottom1 = mp_TempRetortBottom->GetTemperature(0);
    qreal RetTempBottom2 = mp_TempRetortBottom->GetTemperature(1);

    Results.append(QString("%1").arg(RetTempSide));
    Results.append(QString("%1").arg(RetTempBottom1));
    Results.append(QString("%1").arg(RetTempBottom2));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::RetortGetCurrent(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempRetortBottom == NULL || mp_TempRetortSide == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }


    quint16 RetCurrentSide = mp_TempRetortSide->GetCurrent();
    quint16 RetCurrentBottom = mp_TempRetortBottom->GetCurrent();

    Results.append(QString("%1").arg(RetCurrentSide));
    Results.append(QString("%1").arg(RetCurrentBottom));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::RetortGetLidLockState(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    if (mp_DIRetortLid == NULL ) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    quint32 RetLidLockState = mp_DIRetortLid->GetValue();

    Results.append(QString("%1").arg(RetLidLockState));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::RetortSetTemperatureSwitchState(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempRetortBottom == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    bool Flag;
    qint8 SwitchState = Params.at(0).toInt();
    qint8 AutoSwitch = Params.at(1).toInt();

    Flag = mp_TempRetortBottom->SetTemperatureSwitchState(SwitchState, AutoSwitch);

    if (Flag) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::RetortGetHeaterSwitchType(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempRetortBottom == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    quint8 RetSwitchType = mp_TempRetortBottom->GetHeaterSwitchType();

    Results.append(QString("%1").arg(RetSwitchType));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}


void ServiceTestHandler::LiquidTubeStartHeating(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    bool Ret(true);
    if (mp_TempTubeLiquid == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qreal TargetTemp = Params.at(0).toFloat();

    mp_TempTubeLiquid->StopTemperatureControl();
    Ret = mp_TempTubeLiquid->StartTemperatureControl(TargetTemp);

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::LiquidTubeStopHeating(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    bool Ret(true);

    if (mp_TempTubeLiquid == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    Ret = mp_TempTubeLiquid->StopTemperatureControl();
    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::LiquidTubeGetTemp(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    if (mp_TempTubeLiquid == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qreal RetTemp = mp_TempTubeLiquid->GetTemperature();
    Results.append(QString("%1").arg(RetTemp));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::LiquidTubeGetCurrent(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    if (mp_TempTubeLiquid == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    quint16 RetCurrent = mp_TempTubeLiquid->GetCurrent();

    Results.append(QString("%1").arg(RetCurrent));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}


void ServiceTestHandler::AirTubeStartHeating(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    bool Ret(true);
    if (mp_TempTubeAir == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qreal TargetTemp = Params.at(0).toFloat();

    mp_TempTubeAir->StopTemperatureControl();
    Ret = mp_TempTubeAir->StartTemperatureControl(TargetTemp);

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::AirTubeStopHeating(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    bool Ret(true);
    if (mp_TempTubeAir == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    Ret = mp_TempTubeAir->StopTemperatureControl();
    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::AirTubeGetTemp(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    if (mp_TempTubeAir == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qreal RetTemp = mp_TempTubeAir->GetTemperature();

    Results.append(QString("%1").arg(RetTemp));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::AirTubeGetCurrent(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    if (mp_TempTubeAir == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    quint16 RetCurrent = mp_TempTubeAir->GetCurrent();

    Results.append(QString("%1").arg(RetCurrent));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}


void ServiceTestHandler::RVStartHeating(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    bool Ret(true);

    if (mp_TempRV == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }
    qreal TargetTemp = Params.at(0).toFloat();

    mp_TempRV->StopTemperatureControl();
    Ret = mp_TempRV->StartTemperatureControl(TargetTemp);

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::RVStopHeating(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    bool Ret(true);

    if (mp_TempRV == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    Ret = mp_TempRV->StopTemperatureControl();

    if (Ret) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::RVGetTemp(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempRV == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    qreal RetTempSensor1 = mp_TempRV->GetTemperature(0);
    qreal RetTempSensor2 = mp_TempRV->GetTemperature(1);

    Results.append(QString("%1").arg(RetTempSensor1));
    Results.append(QString("%1").arg(RetTempSensor2));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::RVGetCurrent(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempRV == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    quint16 RetCurrent = mp_TempRV->GetCurrent();
    Results.append(QString("%1").arg(RetCurrent));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::RVInitialize(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    int Ret(0);
    if (mp_MotorRV == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    Ret = mp_MotorRV->MoveToInitialPosition();

    if (Ret == RV_MOVE_OK) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::RVMovePosition(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();
    int Ret(0);
    if (mp_MotorRV == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    bool TubeFlag = (bool) Params.at(0).toInt();
    int Position = Params.at(1).toInt();

    if (TubeFlag) {
        Ret = mp_MotorRV->MoveToTubePosition(Position);
    }
    else {
        Ret = mp_MotorRV->MoveToSealPosition(Position);
    }

    if (Ret == RV_MOVE_OK) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::RVSetTemperatureSwitchState(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempRV == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    bool Flag;
    qint8 SwitchState = Params.at(0).toInt();
    qint8 AutoSwitch = Params.at(1).toInt();

    Flag = mp_TempRV->SetTemperatureSwitchState(SwitchState, AutoSwitch);

    if (Flag) {
        emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
    }
    else {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_FAIL, Results);
    }
}

void ServiceTestHandler::RVGetHeaterSwitchType(QString& ReqName, QStringList& Params)
{
    QStringList Results;
    Results.clear();

    if (mp_TempRV == NULL) {
        emit ReturnServiceRequestResult(ReqName, RETURN_ERR_NULL_POINTER, Results);
        return ;
    }

    quint8 RetSwitchType = mp_TempRV->GetHeaterSwitchType();
    Results.append(QString("%1").arg(RetSwitchType));

    emit ReturnServiceRequestResult(ReqName, RETURN_OK, Results);
}

void ServiceTestHandler::LSStartHeating(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::LSStopHeating(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::LSGetTemp(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::LSGetCurrent(QString& ReqName, QStringList& Params)
{

}


void ServiceTestHandler::PumpBuildPressure(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::PumpReleasePressure(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::PumpGetPressure(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::PumpSetFan(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::PumpSetValve(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::PumpStopCompressor(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::PumpSucking(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::PumpDraining(QString& ReqName, QStringList& Params)
{

}

void ServiceTestHandler::GetSlaveModuleReportError(QString& ReqName, QStringList& Params)
{
    int WaitMSec = 3000;

    QStringList Results;
    Results.clear();
    ErrorCode_t Ret(RETURN_OK);

    quint8 ErrorCode = Params.at(0).toInt();
    QString DevName = Params.at(1);
    quint32 SensorName = Params.at(2).toInt();

    qDebug()<<"ErrorCode="<<ErrorCode<<"  DevName="<<DevName<<"  SensorName="<<SensorName;

    ReportError_t ReportError;

    while(WaitMSec>0) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        ReportError = m_rIdevProc.GetSlaveModuleReportError(ErrorCode, DevName, SensorName);

        if (ReportError.instanceID!=0 && (now-ReportError.errorTime)<=3*1000) {
            Ret = RETURN_ERR_FAIL;
            break;
        }

        WaitMSec -= 500;

        Pause(500);
    }

    emit ReturnServiceRequestResult(ReqName, Ret, Results);
}

void ServiceTestHandler::Pause(quint32 MilliSeconds)
{
    mp_Utils->Pause(MilliSeconds);
}

void ServiceTestHandler::HandleRequest(QString ReqName, QStringList Params)
{
    qDebug()<<"HandleRequest:"<<"  ReqName="<<ReqName;
    qDebug()<<Params;

    if (ReqName == "MainControlGetCurrent") {
        MainControlGetCurrent(ReqName, Params);
    }
    else if (ReqName == "MainControlGetVoltage") {
        MainControlGetVoltage(ReqName, Params);
    }
    else if (ReqName == "MainRelaySetOnOff") {
        MainRelaySetOnOff(ReqName, Params);
    }
    else if (ReqName == "AlarmSetOnOff") {
        AlarmSetOnOff(ReqName, Params);
    }
    else if (ReqName == "AlarmGetState") {
        AlarmGetState(ReqName, Params);
    }
    // oven
    else if (ReqName == "OvenStartHeating") {
        OvenStartHeating(ReqName, Params);
    }
    else if (ReqName == "OvenStopHeating") {
        OvenStopHeating(ReqName, Params);
    }
    else if (ReqName == "OvenGetTemp") {
        OvenGetTemp(ReqName, Params);
    }
    else if (ReqName == "OvenGetCurrent") {
        OvenGetCurrent(ReqName, Params);
    }
    else if (ReqName == "OvenGetCoverSensorState") {
        OvenGetCoverSensorState(ReqName, Params);
    }
    else if (ReqName == "OvenGetSwithType") {
        OvenGetSwithType(ReqName, Params);
    }
    // retort
    else if (ReqName == "RetortStartHeating") {
        RetortStartHeating(ReqName, Params);
    }
    else if (ReqName == "RetortStopHeating") {
        RetortStopHeating(ReqName, Params);
    }
    else if (ReqName == "RetortGetTemp") {
        RetortGetTemp(ReqName, Params);
    }
    else if (ReqName == "RetortGetCurrent") {
        RetortGetCurrent(ReqName, Params);
    }
    else if (ReqName == "RetortGetLidLockState") {
        RetortGetLidLockState(ReqName, Params);
    }
    else if (ReqName == "RetortSetTemperatureSwitchState") {
        RetortSetTemperatureSwitchState(ReqName, Params);
    }
    else if (ReqName == "RetortGetHeaterSwitchType") {
        RetortGetHeaterSwitchType(ReqName, Params);
    }
    // liquid tube
    else if (ReqName == "LiquidTubeStartHeating") {
        LiquidTubeStartHeating(ReqName, Params);
    }
    else if (ReqName == "LiquidTubeStopHeating") {
        LiquidTubeStopHeating(ReqName, Params);
    }
    else if (ReqName == "LiquidTubeGetTemp") {
        LiquidTubeGetTemp(ReqName, Params);
    }
    else if (ReqName == "LiquidTubeGetCurrent") {
        LiquidTubeGetCurrent(ReqName, Params);
    }
    // air tube
    else if (ReqName == "AirTubeStartHeating") {
        AirTubeStartHeating(ReqName, Params);
    }
    else if (ReqName == "AirTubeStopHeating") {
        AirTubeStopHeating(ReqName, Params);
    }
    else if (ReqName == "AirTubeGetTemp") {
        AirTubeGetTemp(ReqName, Params);
    }
    else if (ReqName == "AirTubeGetCurrent") {
        AirTubeGetCurrent(ReqName, Params);
    }
    // Rotary valve
    else if (ReqName == "RVStartHeating") {
        RVStartHeating(ReqName, Params);
    }
    else if (ReqName == "RVStopHeating") {
        RVStopHeating(ReqName, Params);
    }
    else if (ReqName == "RVGetTemp") {
        RVGetTemp(ReqName, Params);
    }
    else if (ReqName == "RVGetCurrent") {
        RVGetCurrent(ReqName, Params);
    }
    else if (ReqName == "RVInitialize") {
        RVInitialize(ReqName, Params);
    }
    else if (ReqName == "RVMovePosition") {
        RVMovePosition(ReqName, Params);
    }
    else if (ReqName == "RVSetTemperatureSwitchState") {
        RVSetTemperatureSwitchState(ReqName, Params);
    }
    else if (ReqName == "RVGetHeaterSwitchType") {
        RVGetHeaterSwitchType(ReqName, Params);
    }
    else if (ReqName == "GetSlaveModuleReportError") {
        GetSlaveModuleReportError(ReqName, Params);
    }
}

} // end namespace DeviceControl
