/****************************************************************************/
/*! \file ManufacturingTestHandler.cpp
 *
 *  \brief Implementation file for class ManufacturingTestHandler.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-05-20
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

#include <ServiceDeviceController/Include/DeviceProcessor/ManufacturingTestHandler.h>

#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include <ServiceDeviceController/Include/ServiceDeviceController.h>
#include "DeviceControl/Include/SlaveModules/DigitalOutput.h"
#include "DeviceControl/Include/SlaveModules/BaseModule.h"
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmStepperMotor.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmTempControl.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmPressureControl.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperUtils.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalInput.h>
#include "Core/Include/CMessageString.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "Global/Include/SystemPaths.h"

namespace DeviceControl {

/****************************************************************************/
ManufacturingTestHandler::ManufacturingTestHandler(IDeviceProcessing &iDevProc)
    : m_IsConfigured(false)
    , m_rIdevProc(iDevProc)
{
    m_UserAbort = false;
    mp_Utils = NULL;

    mp_TempOvenTop = NULL;
    mp_TempOvenBottom = NULL;
    mp_DigitalInpputOven = NULL;
    mp_DigitalOutputMainRelay = NULL;

    mp_TempRetortSide = NULL;
    mp_TempRetortBttom = NULL;
    mp_TempRetortInputLid = NULL;

    mp_TempTubeLiquid = NULL;
    mp_TempTubeAir = NULL;
    mp_MotorRV = NULL;
    mp_PressPump = NULL;
    mp_TempRV = NULL;

    mp_BaseModule3 = NULL;
    mp_BaseModule5 = NULL;
    mp_BaseModule15 = NULL;

    mp_TempLSensor = NULL;
    mp_SpeakProc  = NULL;
}

/****************************************************************************/
void ManufacturingTestHandler::CreateWrappers()
{
    qDebug()<<"ManufacturingTestHandler::CreateWrappers" ;

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
        mp_TempRetortBttom = new WrapperFmTempControl("temp_retort_bottom", pTemperature, this);
    }

    CDigitalInput *pDigitalInput = NULL;
    pDigitalInput = static_cast<CDigitalInput *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_OVEN, CANObjectKeyLUT::m_OvenLidDIKey));
    if ( NULL != pDigitalInput ) {
        mp_DigitalInpputOven = new WrapperFmDigitalInput("digitalinput_oven", pDigitalInput, this);
    }

    pDigitalInput = NULL;
    pDigitalInput = static_cast<CDigitalInput *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_RETORT, CANObjectKeyLUT::m_RetortLockDIKey));
    if ( NULL != pDigitalInput ) {
        mp_TempRetortInputLid = new WrapperFmDigitalInput("digitalinput_retortlid", pDigitalInput, this);
    }

    CDigitalOutput *pDigitalOutput = NULL;
    pDigitalOutput = static_cast<CDigitalOutput*>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_MAIN_CONTROL, CANObjectKeyLUT::m_PerMainRelayDOKey));
    if ( NULL != pDigitalOutput ) {
        mp_DigitalOutputMainRelay = new WrapperFmDigitalOutput("heater_relay", pDigitalOutput, this);
    }
    else {
        qDebug()<<"new WrapperFmDigitalOutput for MainRelay failed !!!!";
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
void ManufacturingTestHandler::Initialize()
{
    (void) CreateWrappers();
    m_IsConfigured = true;

    mp_Utils = new WrapperUtils(this);
}

/****************************************************************************/
bool ManufacturingTestHandler::IsInitialized()
{
    return m_IsConfigured;
}


/****************************************************************************/
void ManufacturingTestHandler::OnAbortTest(Global::tRefType Ref, quint32 id, quint32 AbortTestCaseId)
{
    qDebug()<<"ManufacturingTestHandler::OnAbortTest";

    if(!IsInitialized()) {
        Initialize();
    }
    if(NULL == mp_Utils) {
        emit ReturnErrorMessagetoMain(Service::MSG_DEVICE_NOT_INITIALIZED);
        return;
    }
    switch (AbortTestCaseId) {
    case Service::SYSTEM_SPEARKER:
    case Service::SYSTEM_REMOTE_LOCAL_ALARM:
        qDebug()<<"abort the system speaker test";
        if (mp_SpeakProc) {
            mp_SpeakProc->terminate();
        }
        break;
    case Service::OVEN_COVER_SENSOR:
        qDebug()<<"abort the oven cover sensor test";
        break;
    case Service::RETORT_LID_LOCK:
        qDebug()<<"abort the retort lid test";
        break;
    default:
        mp_Utils->AbortPause();
    }
    m_UserAbort = true;
}

qint32 ManufacturingTestHandler::TestOvenHeatingWater()
{
    quint32 WaitSec(0);
    quint32 SumSec(0);
    qreal   CurrentTempTop(0), CurrentTempBottom1(0), CurrentTempBottom2(0);
    QString TopValue;
    QString BottomValue1;
    QString BottomValue2;
    QString UsedTime;
    Service::ModuleTestStatus Status;
    bool NeedAC = false;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::OVEN_HEATING_WITH_WATER);


    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QTime DurationTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble();
    qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal MinTargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal MaxTargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal TopTargetTemp = p_TestCase->GetParameter("TopTargetTemp").toDouble();
    qreal BottomTargetTemp = p_TestCase->GetParameter("BottomTargetTemp").toDouble();

    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    if (p_TestCase->GetParameter("PowerSupply") == "AC") {
        NeedAC = true;
    }

    WaitSec = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second();

    SumSec = WaitSec;

    CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
    CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
    CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);

    if (CurrentTempTop<AmbTempLow || CurrentTempTop>AmbTempHigh ||
            CurrentTempBottom1<AmbTempLow || CurrentTempBottom1>AmbTempHigh ||
            CurrentTempBottom2<AmbTempLow || CurrentTempBottom2>AmbTempHigh ){
        QString FailureMsg = QString("Oven Current Temperature is (%1 %2 %3) which is not in (%4~%5)").arg(CurrentTempTop).arg(CurrentTempBottom1)
                .arg(CurrentTempBottom2).arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Service::OVEN_HEATING_WITH_WATER, FailureMsg);
        p_TestCase->SetStatus(false);

        QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTemp).arg(DepartureLow).arg(DepartureHigh);

        QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");

        TopValue = QString("%1").arg(CurrentTempTop);
        BottomValue1 = QString("%1").arg(CurrentTempBottom1);
        BottomValue2 = QString("%1").arg(CurrentTempBottom2);

        p_TestCase->AddResult("Duration", Duration);
        p_TestCase->AddResult("TargetTemp", TargetTempStr);
        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTempTop", TopValue);
        p_TestCase->AddResult("CurrentTempBottom1", BottomValue1);
        p_TestCase->AddResult("CurrentTempBottom2", BottomValue2);

        return -1;
    }

    if (NeedAC) {
        mp_DigitalOutputMainRelay->SetHigh();
    }

    mp_TempOvenTop->StartTemperatureControl(TopTargetTemp);
    mp_TempOvenBottom->StartTemperatureControl(BottomTargetTemp);

    while (!m_UserAbort && WaitSec)
    {
        CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
        CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
        CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);

        if (CurrentTempTop == -1 || CurrentTempBottom1 == -1 || CurrentTempBottom2 == -1) {
            mp_Utils->Pause(1000);
            WaitSec--;
            continue;
        }

        mp_Utils->Pause(1000);
        TopValue = QString("%1").arg(CurrentTempTop);
        BottomValue1 = QString("%1").arg(CurrentTempBottom1);
        BottomValue2 = QString("%1").arg(CurrentTempBottom2);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.clear();
        Status.insert("UsedTime", UsedTime);
        Status.insert("CurrentTempTop", TopValue);
        Status.insert("CurrentTempBottom1", BottomValue1);
        Status.insert("CurrentTempBottom2", BottomValue2);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTemp).arg(DepartureLow).arg(DepartureHigh);

            Status.insert("TargetTemp", TargetTempStr);

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        WaitSec--;
    }

    mp_TempOvenTop->StopTemperatureControl();
    mp_TempOvenBottom->StopTemperatureControl();
    if (NeedAC) {
        mp_DigitalOutputMainRelay->SetLow();
    }

    if (m_UserAbort) {
        m_UserAbort = false;
        p_TestCase->AddResult("FailReason", "Abort");
        return 1;
    }
    else {
        p_TestCase->AddResult("UsedTime", UsedTime);
        p_TestCase->AddResult("CurrentTempTop", TopValue);
        p_TestCase->AddResult("CurrentTempBottom1", BottomValue1);
        p_TestCase->AddResult("CurrentTempBottom2", BottomValue2);

        Status.clear();
        Status.insert("OvenHeatingWaterStatus", "Finished");
        emit RefreshTestStatustoMain(TestCaseName, Status);
        return 0;
    }
}

/****************************************************************************/
qint32 ManufacturingTestHandler::TestOvenHeating()
{
    quint32 WaitSec(0);
    quint32 SumSec(0);
    qint32  OvenStatus(-1);
    qreal   CurrentTempTop(0), CurrentTempBottom1(0), CurrentTempBottom2(0);
    QString TopValue;
    QString BottomValue1;
    QString BottomValue2;
    QString UsedTime;
    bool NeedAC = false;

    Service::ModuleTestCaseID Id = Service::OVEN_HEATING_EMPTY;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);


    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QTime DurationTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble();
    qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal MinTargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal MaxTargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureHigh").toDouble();

    qreal TopTargetTemp = p_TestCase->GetParameter("TopTargetTemp").toDouble();
    qreal BottomTargetTemp = p_TestCase->GetParameter("BottomTargetTemp").toDouble();
    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    int KeepSeconds = 0;

    if (p_TestCase->GetParameter("PowerSupply") == "AC") {
        NeedAC = true;
    }

    Service::ModuleTestStatus Status;

    WaitSec = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second();

    SumSec = WaitSec;

    if (NeedAC) {
        qDebug()<<"MainRelay SetHigh return :"<< mp_DigitalOutputMainRelay->SetHigh();
    }

    qDebug()<<"Start top return : "<<mp_TempOvenTop->StartTemperatureControl(TopTargetTemp);
    qDebug()<<"Start bottom return :"<< mp_TempOvenBottom->StartTemperatureControl(BottomTargetTemp);

    CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
    CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
    CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);
    qDebug()<<"AmbTempLow="<<AmbTempLow<<" AmbTempHigh="<<AmbTempHigh<<" Top="<<CurrentTempTop<<" Bot1="<<CurrentTempBottom1<<" Bot2="<<CurrentTempBottom2;

    if (CurrentTempTop<AmbTempLow || CurrentTempTop>AmbTempHigh ||
            CurrentTempBottom1<AmbTempLow || CurrentTempBottom1>AmbTempHigh ||
            CurrentTempBottom2<AmbTempLow || CurrentTempBottom2>AmbTempHigh ){
        QString FailureMsg = QString("Oven Current Temperature is (%1 %2 %3) which is not in (%4~%5)").arg(CurrentTempTop).arg(CurrentTempBottom1)
                .arg(CurrentTempBottom2).arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Service::OVEN_HEATING_EMPTY, FailureMsg);
        p_TestCase->SetStatus(false);

        QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTemp).arg(DepartureLow).arg(DepartureHigh);

        QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");

        TopValue = QString("%1").arg(CurrentTempTop);
        BottomValue1 = QString("%1").arg(CurrentTempBottom1);
        BottomValue2 = QString("%1").arg(CurrentTempBottom2);

        p_TestCase->AddResult("Duration", Duration);
        p_TestCase->AddResult("TargetTemp", TargetTempStr);

        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTempTop", TopValue);
        p_TestCase->AddResult("CurrentTempBottom1", BottomValue1);
        p_TestCase->AddResult("CurrentTempBottom2", BottomValue2);

        mp_TempOvenTop->StopTemperatureControl();
        mp_TempOvenBottom->StopTemperatureControl();
        if (NeedAC) {
            mp_DigitalOutputMainRelay->SetLow();
        }

        return -1;
    }


    while (!m_UserAbort && WaitSec)
    {
        CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
        CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
        CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);

        if (CurrentTempTop == -1 || CurrentTempBottom1 == -1 || CurrentTempBottom2 == -1) {
            qDebug()<<"Top="<<CurrentTempTop<<" Bot1="<<CurrentTempBottom1<<" Bot2="<<CurrentTempBottom2;

            mp_Utils->Pause(1000);
            WaitSec--;
            continue;
        }

        qDebug()<<"Target="<<TargetTemp<<" Top="<<CurrentTempTop<<" Bot1="<<CurrentTempBottom1<<" Bot2="<<CurrentTempBottom2;

        if ( CurrentTempTop >= MinTargetTemp &&
             CurrentTempBottom1 >= MinTargetTemp &&
             CurrentTempBottom2 >= MinTargetTemp )
        {
            if ( KeepSeconds > 60 ) {
                OvenStatus = 1;
                break;
            }
            else {
                KeepSeconds++;
            }
        }
        else {
            KeepSeconds = 0;
        }

        mp_Utils->Pause(1000);
        TopValue = QString("%1").arg(CurrentTempTop);
        BottomValue1 = QString("%1").arg(CurrentTempBottom1);
        BottomValue2 = QString("%1").arg(CurrentTempBottom2);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.insert("UsedTime", UsedTime);
        Status.insert("CurrentTempTop", TopValue);
        Status.insert("CurrentTempBottom1", BottomValue1);
        Status.insert("CurrentTempBottom2", BottomValue2);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTemp).arg(DepartureLow).arg(DepartureHigh);

            Status.insert("TargetTemp", TargetTempStr);

            qDebug()<<"TargetTemp="<<TargetTempStr;

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        WaitSec--;
    }

    mp_TempOvenTop->StopTemperatureControl();
    mp_TempOvenBottom->StopTemperatureControl();
    if (NeedAC) {
        mp_DigitalOutputMainRelay->SetLow();
    }

    p_TestCase->AddResult("UsedTime", UsedTime);
    p_TestCase->AddResult("CurrentTempTop", TopValue);
    p_TestCase->AddResult("CurrentTempBottom1", BottomValue1);
    p_TestCase->AddResult("CurrentTempBottom2", BottomValue2);

    if ( OvenStatus == -1 )  // failed.
    {
        p_TestCase->SetStatus(false);
 //       p_TestCase->AddResult("FailReason", Service::MSG_HEATING_FAIL);
        if (m_UserAbort)
        {
            m_UserAbort = false;
            p_TestCase->AddResult("FailReason", "Abort");
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else  // success
    {
        p_TestCase->SetStatus(true);
        return 0;
    }
}


qint32 ManufacturingTestHandler::TestOvenCoverSensor()
{
    if (mp_DigitalInpputOven == NULL) {
        SetFailReason(Service::OVEN_COVER_SENSOR, Service::MSG_DEVICE_NOT_INITIALIZED);
        emit ReturnManufacturingTestMsg(false);
        return 0;
    }

    Service::ModuleTestStatus Status;
    qint32 Value = mp_DigitalInpputOven->GetValue();

    if ( Value == 0) { //  oven cover sensor status : close
        Status.insert("OvenCoverSensorStatus", "Close");
    }
    else {
        Status.insert("OvenCoverSensorStatus", "Open");
    }

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::OVEN_COVER_SENSOR);
    emit RefreshTestStatustoMain(TestCaseName, Status);

    return 0;
}

qint32 ManufacturingTestHandler::TestLidLocker()
{
    if (NULL == mp_TempRetortInputLid) {
        SetFailReason(Service::RETORT_LID_LOCK, Service::MSG_DEVICE_NOT_INITIALIZED);
        emit ReturnManufacturingTestMsg(false);
        return 0;
    }

    Service::ModuleTestStatus status;
    qint32 value = mp_TempRetortInputLid->GetValue();
    if (value == 0) { //  Lid locker status : close
        status.insert("LidLockerStatus", "Close");
    }
    else {
        status.insert("LidLockerStatus", "Open");
    }

    QString testCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::RETORT_LID_LOCK);
    emit RefreshTestStatustoMain(testCaseName, status);
    return 0;
}

qint32 ManufacturingTestHandler::TestRetortHeating()
{
    QString testCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::RETORT_HEATING_EMPTY);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(testCaseName);

    QTime durTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
    qreal tgtTemp = p_TestCase->GetParameter("TargetTemp").toDouble();
    qreal deptLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal deptHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal minTgtTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
    //qreal maxTgtTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureHigh").toDouble();

    qreal sideTgtTemp = p_TestCase->GetParameter("SideTargetTemp").toDouble();
    qreal btmTgtTemp  = p_TestCase->GetParameter("BottomTargetTemp").toDouble();
    qreal ambLow  = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal ambHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    bool needAC = false;
    if (p_TestCase->GetParameter("PowerSupply") == "AC") {
        needAC = true;
        bool acRet = mp_DigitalOutputMainRelay->SetHigh();
        qDebug() << "MainRelay SetHigh return :"<< acRet << "\n";
    }

    bool sideCtrlRet = mp_TempRetortSide->StartTemperatureControl(sideTgtTemp);
    bool btmCtrlRet  = mp_TempRetortBttom->StartTemperatureControl(btmTgtTemp);
    qDebug() << "Start top return : "<< sideCtrlRet << "\n";
    qDebug() << "Start bottom return :"<< btmCtrlRet << "\n";

    qreal curSideTemp   = mp_TempRetortSide->GetTemperature(0);
    qreal curBottomTemp = mp_TempRetortBttom->GetTemperature(0);
    qDebug() << "AmbTempLow= " << ambLow << " AmbTempHigh= " << ambHigh <<" Side= " << curSideTemp <<" Bottom= " << curBottomTemp << "\n";

    const quint32 sumSec = durTime.hour() * 60 * 60 + durTime.minute() * 60 + durTime.second();
    quint32 waitSec = sumSec;

    QString sideTemp, btmTemp;
    if (curSideTemp < ambLow || curSideTemp > ambHigh || curBottomTemp < ambLow || curBottomTemp > ambHigh) {
        QString failMsg = QString("Retort current temperature is (%1 %2) which is not in (%4~%5)")\
                .arg(curSideTemp).arg(curBottomTemp).arg(ambLow).arg(ambHigh);
        SetFailReason(Service::RETORT_HEATING_EMPTY, failMsg);
        p_TestCase->SetStatus(false);

        QString target = QString("%1 (%2~%3)").arg(tgtTemp).arg(deptLow).arg(deptHigh);
        QString duration = QTime().addSecs(sumSec).toString("hh:mm:ss");
        p_TestCase->AddResult("Duration", duration);
        p_TestCase->AddResult("TargetTemp", target);

        sideTemp = QString("%1").arg(curSideTemp);
        btmTemp  = QString("%1").arg(curBottomTemp);
        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTempSide", sideTemp);
        p_TestCase->AddResult("CurrentTempBottom", btmTemp);

        mp_TempOvenTop->StopTemperatureControl();
        mp_TempOvenBottom->StopTemperatureControl();
        if (needAC) {
            mp_DigitalOutputMainRelay->SetLow();
        }
        return -1;
    }

    Service::ModuleTestStatus testStat;
    QString target = QString("%1 (%2~%3)").arg(tgtTemp).arg(deptLow).arg(deptHigh);
    testStat.insert("TargetTemp", target);
    qDebug() << "TargetTemp=" << target;

    QString duration = QTime().addSecs(sumSec).toString("hh:mm:ss");
    testStat.insert("Duration", duration);
    p_TestCase->AddResult("Duration", duration);
    p_TestCase->AddResult("TargetTemp", target);

    QString usedTime;
    qint32  keepSec  = 0, progStat = -1;
    while (!m_UserAbort && waitSec) {
        curSideTemp   = mp_TempRetortSide->GetTemperature(0);
        curBottomTemp = mp_TempRetortBttom->GetTemperature(0);
        if (curSideTemp == -1 || curBottomTemp == -1) {
            qDebug() << "Side=" << curSideTemp << " Bottom=" << curBottomTemp << "\n";
            mp_Utils->Pause(1000);
            -- waitSec;
            continue;
        }
        qDebug() << "Target=" << tgtTemp << " Side=" << curSideTemp << " Bottom=" << curBottomTemp << "\n";

        if (curSideTemp >= minTgtTemp && curBottomTemp >= minTgtTemp) {
            if (keepSec > 60) {
                progStat = 1;
                break;
            }
            ++ keepSec;
        }
        else {
            keepSec = 0;
        }

        mp_Utils->Pause(1000);
        -- waitSec;

        sideTemp = QString("%1").arg(curSideTemp);
        btmTemp  = QString("%1").arg(curBottomTemp);
        usedTime = QTime().addSecs(sumSec - waitSec + 1).toString("hh:mm:ss");

        testStat.insert("UsedTime", usedTime);
        testStat.insert("CurrentTempSide", sideTemp);
        testStat.insert("CurrentTempBottom", btmTemp);
        emit RefreshTestStatustoMain(testCaseName, testStat);
    }

    mp_TempOvenTop->StopTemperatureControl();
    mp_TempOvenBottom->StopTemperatureControl();
    if (needAC) {
        mp_DigitalOutputMainRelay->SetLow();
    }

    p_TestCase->AddResult("UsedTime", usedTime);
    p_TestCase->AddResult("CurrentTempSide", sideTemp);
    p_TestCase->AddResult("CurrentTempBottom", btmTemp);

    int exitStat = -1;
    if (m_UserAbort) {
        p_TestCase->AddResult("FailReason", "Abort");
        m_UserAbort = false;
        exitStat = 1;
    }
    if (progStat == 1) {
        p_TestCase->SetStatus(true);
        exitStat = 0;
    }
    p_TestCase->SetStatus(exitStat == 0 ? true : false);

    return exitStat;
}

qint32 ManufacturingTestHandler::TestRetortHeatingWater()
{
    return -1;
}

qint32 ManufacturingTestHandler::TestSystemSpeaker(bool IsLowVolume)
{
    Service::ModuleTestStatus Status;
    if (!mp_SpeakProc) {
        mp_SpeakProc = new QProcess;
    }

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_SPEARKER);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    bool VolumeFlag = p_TestCase->GetParameter("VolumeFlag").toInt();
    QString VolumeLevel = VolumeFlag ? QString("HighVolume") : QString("LowVolume");
    VolumeLevel = p_TestCase->GetParameter(VolumeLevel);

    qDebug()<<"System Speaker test Volume Level : "<<VolumeLevel;
    QString SetVolume;
    QStringList PlayParams;

    SetVolume = "amixer set PCM " + VolumeLevel + " %";
    PlayParams<<"-r"<<Global::SystemPaths::Instance().GetSoundPath() + "/Note1.ogg";

    mp_SpeakProc->start(SetVolume);
    mp_SpeakProc->waitForFinished();

    mp_SpeakProc->start("ogg123", PlayParams);

    p_TestCase->SetParameter("VolumeFlag", QString::number(!VolumeFlag));
    emit RefreshTestStatustoMain(TestCaseName, Status);

    return 0;
}

qint32 ManufacturingTestHandler::TestSystemAlarm()
{
    Service::ModuleTestStatus Status;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_REMOTE_LOCAL_ALARM);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    bool PositionFlag = p_TestCase->GetParameter("PositionFlag").toInt();
    bool ConnectFlag  = p_TestCase->GetParameter("ConnectFlag").toInt();

    QStringList AlarmParams;
    if (ConnectFlag) {
        if (PositionFlag) {
            // remote connect test
            AlarmParams<<"-r"<<Global::SystemPaths::Instance().GetSoundPath() + "/Alarm1.ogg";
        }
        else {
            // local connect test
            AlarmParams<<"-r"<<Global::SystemPaths::Instance().GetSoundPath() + "/Alarm2.ogg";
        }
        Status.insert("AlarmStatus", "Connected");
    }
    else {
        if (PositionFlag) {
            // remote disconnect test
            AlarmParams<<"-r"<<Global::SystemPaths::Instance().GetSoundPath() + "/Alarm3.ogg";
        }
        else {
            // local disconnect test
            AlarmParams<<"-r"<<Global::SystemPaths::Instance().GetSoundPath() + "/Alarm4.ogg";
        }
        p_TestCase->SetParameter("PostionFlag", QString::number(!PositionFlag));
        Status.insert("AlarmStatus", "disConnected");
    }

    p_TestCase->SetParameter("ConnectFlag", QString::number(!ConnectFlag));

    if (!mp_SpeakProc) {
        mp_SpeakProc = new QProcess;
    }
    mp_SpeakProc->start("ogg123", AlarmParams);

    emit RefreshTestStatustoMain(TestCaseName, Status);
    return 0;
}

qint32 ManufacturingTestHandler::TestSystem110v220vSwitch()
{
    qint32 CurrentVoltage = 110;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_110V_220V_SWITCH);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QString ConnectedVoltage = p_TestCase->GetParameter("ConnectedVoltage");

    p_TestCase->AddResult("CurrentVoltage", QString::number(CurrentVoltage));

    return CurrentVoltage == ConnectedVoltage.toInt();
}

qint32 ManufacturingTestHandler::TestMainControlASB(Service::ModuleTestCaseID_t Id)
{
    DeviceControl::HimSlaveType_t Slave ;
    if (Id == Service::MAINCONTROL_ASB3) {
       Slave = DeviceControl::Slave_3;
       qDebug()<<"Get SN for BaseModule 3:"<<mp_BaseModule3->GetSerialNumber();
    }
    else if (Id == Service::MAINCONTROL_ASB5) {
       qDebug()<<"Get SN for BaseModule 5:"<<mp_BaseModule5->GetSerialNumber();
       Slave = DeviceControl::Slave_5;
    }
    else if (Id == Service::MAINCONTROL_ASB15) {
        qDebug()<<"Get SN for BaseModule 15:"<<mp_BaseModule15->GetSerialNumber();
       Slave = DeviceControl::Slave_15;
    }
    else {
       qDebug()<<"Error : Wrong Parameter !";
       return -1;
    }





    qreal ActualVoltage = m_rIdevProc.IDGetSlaveVoltage(Slave)/1000.0;
    qreal ActualCurrent = m_rIdevProc.IDGetSlaveCurrent(Slave);

    qDebug()<<"voltage = "<<ActualVoltage <<" current = "<<ActualCurrent;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    qreal Voltage = p_TestCase->GetParameter("Voltage").toDouble();
    qreal VoltageTolerance = p_TestCase->GetParameter("VolTolerance").toDouble();
    qreal VolLow = Voltage - Voltage*VoltageTolerance;
    qreal VolHigh = Voltage + Voltage*VoltageTolerance;
    qreal CurrentLow = p_TestCase->GetParameter("CurrentLow").toDouble();
    qreal CurrentHigh = p_TestCase->GetParameter("CurrentHigh").toDouble();

    p_TestCase->AddResult("Voltage", QString("%1").arg(ActualVoltage));
    p_TestCase->AddResult("Current", QString("%1").arg(ActualCurrent));

    if (ActualVoltage>=VolLow && ActualVoltage<= VolHigh &&
            ActualCurrent>=CurrentLow && ActualCurrent<= CurrentHigh) {

        return 0;
    }
    else {
        return 1;
    }
}

qint32 ManufacturingTestHandler::TestLAHeatingTube(Service::ModuleTestCaseID_t Id)
{
    quint32 WaitSec(0);
    quint32 SumSec(0);
    qint32  LAStatus(-1);
    qreal   CurrentTemp(0);
    QString CurrentValue;
    QString UsedTime;
    bool NeedAC = false;
    WrapperFmTempControl *p_TempCtrl = NULL;
    int KeepSeconds = 0;

    if (Id == Service::LA_SYSTEM_HEATING_LIQUID_TUBE) {
        p_TempCtrl = mp_TempTubeLiquid;
    }
    else if (Id == Service::LA_SYSTEM_HEATING_AIR_TUBE ) {
        p_TempCtrl = mp_TempTubeAir;
    }

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);

    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QTime DurationTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() ;
    qreal DepartureLow =  p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal DepartureHigh =  p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal MinTargetTemp = TargetTemp + DepartureLow;
    qreal MaxTargetTemp = TargetTemp + DepartureHigh;

    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    if (p_TestCase->GetParameter("PowerSupply") == "AC") {
        NeedAC = true;
    }

    qDebug()<<"DurationTime="<<DurationTime<<" TargetTemp="<<TargetTemp<<" Min="<<MinTargetTemp<<" Max="<<MaxTargetTemp<<" AmbLow="<<AmbTempLow<<" AmbHigh="<<AmbTempHigh<<" NeedAc="<<NeedAC;
    Service::ModuleTestStatus Status;

    WaitSec = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second();

    SumSec = WaitSec;
#if 1
    CurrentTemp = p_TempCtrl->GetTemperature(0);

    qDebug()<<"Current temp = "<<CurrentTemp;

    if (CurrentTemp<AmbTempLow || CurrentTemp>AmbTempHigh) {
        QString FailureMsg = QString("Tube Current Temperature is (%1) which is not in (%2~%3)").arg(CurrentTemp).arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Id, FailureMsg);
        p_TestCase->SetStatus(false);

        QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTemp).arg(DepartureLow).arg(DepartureHigh);

        QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");

        CurrentValue = QString("%1").arg(CurrentTemp);

        p_TestCase->AddResult("Duration", Duration);
        p_TestCase->AddResult("TargetTemp", TargetTempStr);

        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTemp", CurrentValue);
        return -1;
    }
#endif

    if (NeedAC) {
        mp_DigitalOutputMainRelay->SetHigh();
    }

    qDebug()<<"Start Temperature control return : "<<p_TempCtrl->StartTemperatureControl(TargetTemp);

    while (!m_UserAbort && WaitSec)
    {
        CurrentTemp = p_TempCtrl->GetTemperature(0);

        qDebug()<<"Current temp = "<<CurrentTemp <<" Target=" << MinTargetTemp;

        if (CurrentTemp == -1) {
            mp_Utils->Pause(1000);
            WaitSec--;
            continue;
        }

        if ( CurrentTemp >= MinTargetTemp )
        {
            if ( KeepSeconds > 60 ) {
                LAStatus = 1;

                break;
            }
            else {
                KeepSeconds++;
            }
        }
        else {
            KeepSeconds = 0;
        }

        mp_Utils->Pause(1000);
        CurrentValue = QString("%1").arg(CurrentTemp);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.insert("UsedTime", UsedTime);
        Status.insert("CurrentTemp", CurrentValue);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTemp).arg(DepartureLow).arg(DepartureHigh);
            Status.insert("TargetTemp", TargetTempStr);

            qDebug()<<"TargetTemp="<<TargetTempStr;

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        WaitSec--;
    }

    p_TempCtrl->StopTemperatureControl();
    if (NeedAC) {
        mp_DigitalOutputMainRelay->SetLow();
    }

    p_TestCase->AddResult("UsedTime", UsedTime);
    p_TestCase->AddResult("CurrentTemp", CurrentValue);

    if ( LAStatus == -1 )  // failed.
    {
        p_TestCase->SetStatus(false);

        if (m_UserAbort)
        {
            m_UserAbort = false;
            p_TestCase->AddResult("FailReason", "Abort");
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else  // success
    {
        p_TestCase->SetStatus(true);
        return 0;
    }
}

qint32 ManufacturingTestHandler::TestRVInitialization( )
{
    if ( !mp_MotorRV->MoveToInitialPosition() )
    {
        return -1;
    }

    return 0;
}

qint32 ManufacturingTestHandler::TestRVSelectingAndSealing(Service::ModuleTestCaseID Id)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);

    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    qint32 Position = p_TestCase->GetParameter("Position").toInt();
    qint32 TubeFlag = p_TestCase->GetParameter("TubeFlag").toInt();

    quint32 Result(-1);

    qDebug()<<"Position = "<<Position;

    if (TubeFlag == 1) {
        Result = mp_MotorRV->MoveToTubePosition(Position);
    }
    else {
        Result = mp_MotorRV->MoveToSealPosition(Position);
    }
    if (Result == 0) {
        return -1;
    }

    return 0;
}

qint32 ManufacturingTestHandler::MoveRVToTubePos(qint32 Pos)
{
    if ( !mp_MotorRV->MoveToTubePosition(Pos) )
    {
        return -1;
    }

    return 0;
}

qint32 ManufacturingTestHandler::MoveRVToSealPos(qint32 Pos)
{
    if ( !mp_MotorRV->MoveToSealPosition(Pos) )
    {
        return -1;
    }

    return 0;
}

qint32 ManufacturingTestHandler::TestRVHeatingStation()
{
    quint32 WaitSec(0);
    quint32 WaitSecSensor1(0);
    quint32 SumSec(0);
    qint32  RVStatus(-1);
    qreal   CurrentTempSensor1(0), CurrentTempSensor2(0);
    QString Sensor1Value;
    QString Sensor2Value;
    QString UsedTime;
    bool NeedAC = false;
    quint32 KeepSeconds(0);

    Service::ModuleTestCaseID Id = Service::ROTARY_VALVE_HEATING_STATION;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    QTime DurationTimeSensor1 = QTime::fromString(p_TestCase->GetParameter("Sensor1Duration"), "hh:mm:ss");
    QTime DurationTimeSensor2 = QTime::fromString(p_TestCase->GetParameter("Sensor2Duration"), "hh:mm:ss");

    qreal TargetTempSensor1 = p_TestCase->GetParameter("Sensor1TargetTemp").toDouble();
    qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal TargetTempSensor2 = p_TestCase->GetParameter("Sensor2TargetTemp").toDouble();
    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    if (p_TestCase->GetParameter("PowerSupply") == "AC") {
        NeedAC = true;
    }

    Service::ModuleTestStatus Status;

    WaitSecSensor1 = DurationTimeSensor1.hour()*60*60 + DurationTimeSensor1.minute()*60 + DurationTimeSensor1.second();

    WaitSec = DurationTimeSensor2.hour()*60*60 + DurationTimeSensor2.minute()*60 + DurationTimeSensor2.second();

    SumSec = WaitSec;

    if (NeedAC) {
        mp_DigitalOutputMainRelay->SetHigh();
    }

    qDebug()<<"Start top return : "<<mp_TempRV->StartTemperatureControl(TargetTempSensor1);

    CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
    CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

    if (CurrentTempSensor1<AmbTempLow || CurrentTempSensor1>AmbTempHigh ||
            CurrentTempSensor2<AmbTempLow || CurrentTempSensor2>AmbTempHigh  ){
        QString FailureMsg = QString("Rotary Valve Current Temperature is (%1 %2) which is not in (%3~%4)").arg(CurrentTempSensor1).arg(CurrentTempSensor2)
                .arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Service::ROTARY_VALVE_HEATING_STATION, FailureMsg);
        p_TestCase->SetStatus(false);

        QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTempSensor1).arg(DepartureLow).arg(DepartureHigh);

        QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");

        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
        Sensor2Value = QString("%1").arg(CurrentTempSensor2);

        p_TestCase->AddResult("Duration", Duration);
        p_TestCase->AddResult("TargetTemp", TargetTempStr);

        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTempSensor1", Sensor1Value);
        p_TestCase->AddResult("CurrentTempSensor2", Sensor2Value);

        mp_TempRV->StopTemperatureControl();
        if (NeedAC) {
            mp_DigitalOutputMainRelay->SetLow();
        }

        return -1;
    }


    while (!m_UserAbort && WaitSec)
    {
        CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
        CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

        if (CurrentTempSensor1 == -1 || CurrentTempSensor1 == -1 ) {
            mp_Utils->Pause(1000);
            WaitSec--;
            continue;
        }

        qDebug()<<"Target="<<TargetTempSensor1<<" Sensor1="<<CurrentTempSensor1<<" Sensor2="<<CurrentTempSensor2;

        if ( (SumSec-WaitSec) >= WaitSecSensor1 ) {

            // if less than target temp in duration time (1h), than exit from this process and report fail.
            if (CurrentTempSensor1 < (TargetTempSensor1+DepartureLow)) {
                break;
            }

            if ( CurrentTempSensor2 >= TargetTempSensor2 ) {
                // if more than target temp in 1 minute, then the test pass
                if (KeepSeconds > 60) {
                    RVStatus = 1;
                    break;
                }
                else {
                    KeepSeconds++;
                }
            }
            else {
                KeepSeconds=0;
            }
        }


        mp_Utils->Pause(1000);
        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
        Sensor2Value = QString("%1").arg(CurrentTempSensor2);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.insert("UsedTime", UsedTime);
        Status.insert("CurrentTempSensor1", Sensor1Value);
        Status.insert("CurrentTempSensor2", Sensor2Value);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTempSensor1).arg(DepartureLow).arg(DepartureHigh);
            Status.insert("TargetTemp", TargetTempStr);

            qDebug()<<"TargetTemp="<<TargetTempStr;

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        WaitSec--;
    }

    mp_TempRV->StopTemperatureControl();
    if (NeedAC) {
        mp_DigitalOutputMainRelay->SetLow();
    }

    p_TestCase->AddResult("UsedTime", UsedTime);
    p_TestCase->AddResult("CurrentTempSensor1", Sensor1Value);
    p_TestCase->AddResult("CurrentTempSensor2", Sensor2Value);

    if ( RVStatus == -1 )  // failed.
    {
        p_TestCase->SetStatus(false);
        if (m_UserAbort)
        {
            m_UserAbort = false;
            p_TestCase->AddResult("FailReason", "Abort");
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else  // success
    {
        p_TestCase->SetStatus(true);
        return 0;
    }
}

qint32 ManufacturingTestHandler::TestRVHeatingEnd()
{
    quint32 WaitSec(0);
    quint32 WaitSecSensor1(0);
    quint32 SumSec(0);
    qint32  RVStatus(-1);
    qreal   CurrentTempSensor1(0), CurrentTempSensor2(0);
    QString Sensor1Value;
    QString Sensor2Value;
    QString UsedTime;
    bool NeedAC = false;
    int Ret(0);
    quint8 Position(0);

    Service::ModuleTestCaseID Id = Service::ROTARY_VALVE_HEATING_END;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    qDebug()<<"Get Parameter:";
    qDebug()<<"Duration1 = "<<p_TestCase->GetParameter("Sensor1Duration");

    QTime DurationTimeSensor1 = QTime::fromString(p_TestCase->GetParameter("Sensor1Duration"), "hh:mm:ss");
    QTime DurationTimeSensor2 = QTime::fromString(p_TestCase->GetParameter("Sensor2Duration"), "hh:mm:ss");

    qreal TargetTempSensor1 = p_TestCase->GetParameter("Sensor1TargetTemp").toDouble();
    qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal TargetTempSensor2 = p_TestCase->GetParameter("Sensor2TargetTemp").toDouble();
    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    int KeepSeconds = 0;

    if (p_TestCase->GetParameter("PowerSupply") == "AC") {
        NeedAC = true;
    }

    Service::ModuleTestStatus Status;

    WaitSecSensor1 = DurationTimeSensor1.hour()*60*60 + DurationTimeSensor1.minute()*60 + DurationTimeSensor1.second();

    WaitSec = WaitSecSensor1;

    SumSec = WaitSec;

    if (NeedAC) {
        qDebug()<<"Main Relay Set hight : " << mp_DigitalOutputMainRelay->SetHigh();
    }


    CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
    CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

    if (CurrentTempSensor1<AmbTempLow || CurrentTempSensor1>AmbTempHigh ||
            CurrentTempSensor2<AmbTempLow || CurrentTempSensor2>AmbTempHigh  ){
        QString FailureMsg = QString("Rotary Valve Current Temperature is (%1 %2) which is not in (%3~%4)").arg(CurrentTempSensor1).arg(CurrentTempSensor2)
                .arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Service::ROTARY_VALVE_HEATING_STATION, FailureMsg);
        p_TestCase->SetStatus(false);

        QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTempSensor1).arg(DepartureLow).arg(DepartureHigh);

        QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");

        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
        Sensor2Value = QString("%1").arg(CurrentTempSensor2);

        p_TestCase->AddResult("Duration", Duration);
        p_TestCase->AddResult("TargetTemp", TargetTempStr);

        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTempSensor1", Sensor1Value);
        p_TestCase->AddResult("CurrentTempSensor2", Sensor2Value);

        mp_TempRV->StopTemperatureControl();
        if (NeedAC) {
            mp_DigitalOutputMainRelay->SetLow();
        }

        return -1;
    }


    // heating level sensor
    qDebug()<<"Heating level sensor....";

    Status.clear();
    Status.insert("CurrentStatus", "Heating level sensor ...");
    emit RefreshTestStatustoMain(TestCaseName, Status);

    Ret = HeatingLevelSensor();
    if (Ret == -1) {
        goto RV_HEATING_END_EXIT;
    }

    // set rotary valve to initial position.
    qDebug()<<"Rotary Value is initializing...........";
    Status.clear();
    Status.insert("CurrentStatus", "Rotary Valve Initializing ...");
    emit RefreshTestStatustoMain(TestCaseName, Status);
    if ( TestRVInitialization() == -1 ) {
        goto RV_HEATING_END_EXIT;
    }
    // set rotary valve to tube positon

    Position =  p_TestCase->GetParameter("Position").toInt();
    qDebug()<<"Rotary Valve is turning to Tube #"<<Position;
    Status.clear();
    Status.insert("CurrentStatus", "Moving Rotary Valve to tube position 11# ...");
    emit RefreshTestStatustoMain(TestCaseName, Status);

    mp_Utils->Pause(1000);

    if ( !mp_MotorRV->MoveToTubePosition(Position) ) {
        goto RV_HEATING_END_EXIT;
    }

    // sucking
    qDebug()<<"Begin sucking....";
    Status.clear();
    Status.insert("CurrentStatus", "Sucking water to retort...");
    emit RefreshTestStatustoMain(TestCaseName, Status);

    mp_Utils->Pause(1000);

    Ret = m_rIdevProc.ALFilling(0, true);
//    Ret = mp_PressPump->Sucking(0, Position, false);
    if ( Ret != 0 ) {
        qDebug()<<"Sucking failed......... Ret = "<<Ret;
        //Ret = mp_PressPump->Draining(1000, Position);
        Ret = m_rIdevProc.ALDraining(0);

        qDebug()<<"Draining return : "<<Ret;

        p_TestCase->AddResult("FailReason", "NOT-IN-HEATING");
        goto RV_HEATING_END_EXIT;
    }

    // set rotary valve to sealing position
    qDebug()<<"Rotary Valve is turning to Sealing #"<<Position;
    Status.clear();
    Status.insert("CurrentStatus", "Moving rotary valve to sealing position ...");
    emit RefreshTestStatustoMain(TestCaseName, Status);
    mp_Utils->Pause(1000);

    if (!mp_MotorRV->MoveToSealPosition(Position)) {
        goto RV_HEATING_END_EXIT;
    }

    qDebug()<<"Begin heating ( at first stage ) .....";
    // heating at first stage.
 //   Status.insert("CurrentStatus", "Heating rotary valve at first stage ...");
 //   emit RefreshTestStatustoMain(TestCaseName, Status);

    qDebug()<<"Start top return : "<<mp_TempRV->StartTemperatureControl(TargetTempSensor1);

    while (!m_UserAbort && WaitSec)
    {
        CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
        CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

        if (CurrentTempSensor1 == -1 || CurrentTempSensor1 == -1 ) {
            mp_Utils->Pause(1000);
            WaitSec--;
            continue;
        }

        qDebug()<<"Target="<<TargetTempSensor1<<" Sensor1="<<CurrentTempSensor1<<" Sensor2="<<CurrentTempSensor2;

        mp_Utils->Pause(1000);
        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
        Sensor2Value = QString("%1").arg(CurrentTempSensor2);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.clear();
        Status.insert("UsedTime", UsedTime);
        Status.insert("CurrentTempSensor1", Sensor1Value);
        Status.insert("CurrentTempSensor2", Sensor2Value);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTempSensor1).arg(DepartureLow).arg(DepartureHigh);
            Status.insert("TargetTemp", TargetTempStr);

            qDebug()<<"TargetTemp="<<TargetTempStr;

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        WaitSec--;
    }

    // set rotary valve to tube position
    qDebug()<<"Rotary Valve is turning to Tube #"<<Position;
    Status.clear();
    Status.insert("CurrentStatus", "Moving rotary valve to tube position 11# ...");
    emit RefreshTestStatustoMain(TestCaseName, Status);

    mp_Utils->Pause(1000);

    if ( !mp_MotorRV->MoveToTubePosition(Position) ) {
        goto RV_HEATING_END_EXIT;
    }

    // draining
    qDebug()<<"Begin draining.........";
    Status.clear();
    Status.insert("CurrentStatus", "Draining water from retort...");
    emit RefreshTestStatustoMain(TestCaseName, Status);

    mp_Utils->Pause(1000);

     Ret = m_rIdevProc.ALDraining(0);
     //Ret = mp_PressPump->Draining(1000, Position);
     if ( Ret != 0 || m_UserAbort ) {
         qDebug()<<"ALDraining return : "<< Ret;
    //     goto RV_HEATING_END_EXIT;
     }

     Status.clear();
     Status.insert("CurrentStatus", "HideMessage");
     emit RefreshTestStatustoMain(TestCaseName, Status);

     qDebug()<<"Begin heating (at second stage).....";
//     Status.insert("CurrentStatus", "Heating rotary valve at second stage ...");
//     emit RefreshTestStatustoMain(TestCaseName, Status);
     // heating at second stage.
     TargetTempSensor1 = p_TestCase->GetParameter("Sensor1TargetTemp2").toFloat();
     mp_TempRV->StopTemperatureControl();
     mp_TempRV->StartTemperatureControl(TargetTempSensor1);

     WaitSec = DurationTimeSensor2.hour()*60*60 + DurationTimeSensor2.minute()*60 + DurationTimeSensor2.second();
     SumSec = WaitSec;
     while (!m_UserAbort && WaitSec)
     {
         CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
         CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

         if (CurrentTempSensor1 == -1 || CurrentTempSensor1 == -1 ) {
             mp_Utils->Pause(1000);
             WaitSec--;
             continue;
         }
         qDebug()<<"Target="<<TargetTempSensor2<<" Sensor1="<<CurrentTempSensor1<<" Sensor2="<<CurrentTempSensor2;

         if ( CurrentTempSensor2 >= TargetTempSensor2 ) {
             if ( KeepSeconds > 60 ) {
                 RVStatus = 1;
                 break;
             }
             else {
                 KeepSeconds++;
             }
         }
         else {
             KeepSeconds = 0;
         }


         mp_Utils->Pause(1000);
         Sensor1Value = QString("%1").arg(CurrentTempSensor1);
         Sensor2Value = QString("%1").arg(CurrentTempSensor2);
         UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

         Status.clear();
         Status.insert("UsedTime", UsedTime);
         Status.insert("CurrentTempSensor1", Sensor1Value);
         Status.insert("CurrentTempSensor2", Sensor2Value);
         if (WaitSec == SumSec) {
             QString TargetTempStr = QString("%1").arg(TargetTempSensor2);//.arg(DepartureLow).arg(DepartureHigh);
             Status.insert("TargetTemp", TargetTempStr);

             qDebug()<<"TargetTemp="<<TargetTempStr;

             QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
             Status.insert("Duration", Duration);
             p_TestCase->AddResult("Duration", Duration);
             p_TestCase->AddResult("TargetTemp", TargetTempStr);
         }

         emit RefreshTestStatustoMain(TestCaseName, Status);

         WaitSec--;
     }

RV_HEATING_END_EXIT:
     Status.clear();
     Status.insert("CurrentStatus", "HideMessage");
     emit RefreshTestStatustoMain(TestCaseName, Status);

     qDebug()<<"RV heating test exit......";
     mp_TempLSensor->StopTemperatureControl();

    mp_TempRV->StopTemperatureControl();
    if (NeedAC) {
        mp_DigitalOutputMainRelay->SetLow();
    }

    p_TestCase->AddResult("UsedTime", UsedTime);
    p_TestCase->AddResult("CurrentTempSensor1", Sensor1Value);
    p_TestCase->AddResult("CurrentTempSensor2", Sensor2Value);


    if ( RVStatus == -1 )  // failed.
    {
        p_TestCase->SetStatus(false);
        if (m_UserAbort)
        {
            m_UserAbort = false;
            p_TestCase->AddResult("FailReason", "Abort");
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else  // success
    {
        p_TestCase->SetStatus(true);
        return 0;
    }
}

qint32 ManufacturingTestHandler::TestLSensorDetecting(qint32 Pos)
{
    qint32 Ret;

    if ( !mp_MotorRV->MoveToTubePosition(Pos) )
    {
        return -1;
    }

    Ret = mp_PressPump->Sucking(0, Pos, false);
    switch(Ret)  //1=ok, -1=err, -2=time out, -3=overflow
    {
        case -1:
            qDebug() << "Sucking return: [General]";
            break;
        case -2:
            qDebug() << "Sucking return: [TimeOut]";
            break;
        case -3:
            qDebug() << "Sucking return: [Overflow]";
            mp_PressPump->Draining(1000, Pos);
            break;
        case -4:
            qDebug() << "Sucking return: [Insufficient]";
            break;
        default:
            break;
    }

    return Ret;
}

qint32 ManufacturingTestHandler::HeatingLevelSensor()
{
    int LSENSOR_TEMP_WATER = 95;
    int LSENSOR_SLOPE_TEMPCHANGE_NORMAL = 10;
    int TEST_LSENSOR_TIMEOUT = 300;        //Sec.
    int TEST_LSENSOR_TEMP_TOLERANCE = 2;

    int LSENSOR_PID_MAXTEMP_NORMAL = 120;  //Degrees
    int LSENSOR_PID_KC_NORMAL = 1212;
    int LSENSOR_PID_TI_NORMAL = 1000;
    int LSENSOR_PID_TD_NORMAL = 80;

    int LSENSOR_PID_MAXTEMP_SLOW = 120;  //Degrees
    int LSENSOR_PID_KC_SLOW = 200;
    int LSENSOR_PID_TI_SLOW = 1000;
    int LSENSOR_PID_TD_SLOW = 0;


    int LSensorTemp = LSENSOR_TEMP_WATER;  // desired temp
    int LSensorTempHigh = LSensorTemp + 10;
    int LSensorTempChange = LSENSOR_SLOPE_TEMPCHANGE_NORMAL;   // Slope temp

    mp_TempLSensor->StopTemperatureControl();
    mp_TempLSensor->SetTemperaturePid(LSENSOR_PID_MAXTEMP_NORMAL, LSENSOR_PID_KC_NORMAL,
                                      LSENSOR_PID_TI_NORMAL, LSENSOR_PID_TD_NORMAL);

    mp_TempLSensor->StartTemperatureControl(LSensorTempHigh, LSensorTempChange);

    int WaitSeconds = TEST_LSENSOR_TIMEOUT;
    int ReadyStatus(-1);
    int i = 0;
    while(WaitSeconds){
        qDebug()<<"Level Sensor temp is : "<<mp_TempLSensor->GetTemperature()<<" time:"<<i++;
        if (mp_TempLSensor->GetTemperature() > (LSensorTemp-TEST_LSENSOR_TEMP_TOLERANCE)) {
            ReadyStatus = 1;
            break;
        }
        mp_Utils->Pause(1000);
        WaitSeconds--;
    }
    if (ReadyStatus == -1) {
        mp_TempLSensor->StopTemperatureControl();

        qDebug()<<"Level Sensor heating error !";
        return -1;
    }

    mp_TempLSensor->StopTemperatureControl();
    mp_TempLSensor->SetTemperaturePid(LSENSOR_PID_MAXTEMP_SLOW, LSENSOR_PID_KC_SLOW,
                                      LSENSOR_PID_TI_SLOW, LSENSOR_PID_TD_SLOW);
    mp_TempLSensor->StartTemperatureControl(LSensorTempHigh, LSensorTempChange);

    mp_Utils->Pause(3000);

    WaitSeconds = TEST_LSENSOR_TIMEOUT;
    i = 0;
    while(WaitSeconds){
        qDebug()<<"Level Sensor temp is : "<<mp_TempLSensor->GetTemperature()<<" time:"<<i++;
        if (mp_TempLSensor->GetTemperature() > (LSensorTemp-TEST_LSENSOR_TEMP_TOLERANCE)) {
            ReadyStatus = 1;
            break;
        }
        mp_Utils->Pause(1000);
        WaitSeconds--;
    }
    if (ReadyStatus == -1) {
        mp_TempLSensor->StopTemperatureControl();

        qDebug()<<"Level Sensor heating error 2 !";
        return -1;
    }

    return 1;

}


void ManufacturingTestHandler::SetFailReason(Service::ModuleTestCaseID Id, const QString &FailMsg)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    p_TestCase->AddResult("FailReason", FailMsg);
    p_TestCase->SetStatus(false);

}

void ManufacturingTestHandler::PerformModuleManufacturingTest(Service::ModuleTestCaseID_t TestId, Service::ModuleTestCaseID_t AbortTestCaseId)
{
    qDebug()<<"ManufacturingTestHandler::PerformModuleManufacturingTest  test="<<TestId;

    if(!IsInitialized()){
        Initialize();
    }

    switch (TestId) {
    case Service::TEST_ABORT:
        OnAbortTest(0, TestId, AbortTestCaseId);
        return ;
    case Service::MAINCONTROL_ASB3:
    case Service::MAINCONTROL_ASB5:
    case Service::MAINCONTROL_ASB15:
        if (TestMainControlASB(TestId) != 0) {
            emit ReturnManufacturingTestMsg(false);
        }
        else {
            emit ReturnManufacturingTestMsg(true);
        }
        break;
    case Service::OVEN_COVER_SENSOR :
        TestOvenCoverSensor();
        break;
    case Service::OVEN_HEATING_WITH_WATER:
        if ( NULL == mp_TempOvenTop || NULL == mp_TempOvenBottom )
        {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if ( 0 != TestOvenHeatingWater()) {
            emit ReturnManufacturingTestMsg(false);
            return ;
        }
        break;
    case Service::OVEN_HEATING_EMPTY:
        if ( NULL == mp_TempOvenTop || NULL == mp_TempOvenBottom || NULL == mp_DigitalOutputMainRelay)
        {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if ( 0 == TestOvenHeating() )
        {
            emit ReturnManufacturingTestMsg(true);
        }
        else
        {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::LA_SYSTEM_HEATING_LIQUID_TUBE:
    case Service::LA_SYSTEM_HEATING_AIR_TUBE:
        if (NULL == mp_TempTubeLiquid || NULL == mp_TempTubeAir) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if ( 0 == TestLAHeatingTube(TestId) ) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;

    case Service::RETORT_LID_LOCK:
        TestLidLocker();
        break;

    case Service::RETORT_LEVEL_SENSOR_HEATING:
        if (0) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if (0) {
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::RETORT_LEVEL_SENSOR_DETECTING:
        if (NULL == mp_TempRetortSide) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if (0) {
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;

    case Service::RETORT_HEATING_WITH_WATER:
        if (NULL == mp_TempRetortSide && NULL == mp_TempRetortBttom) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if (0 == TestRetortHeatingWater()) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;

    case Service::RETORT_HEATING_EMPTY:
        if (NULL == mp_TempRetortSide && NULL == mp_TempRetortBttom) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if (0 == TestRetortHeating()) {
            ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;

    case Service::ROTARY_VALVE_INITIALIZING:
        if (NULL == mp_MotorRV) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if ( 0 ==  TestRVInitialization() ) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::ROTARY_VALVE_SELECTION_FUNCTION:
    case Service::ROTARY_VALVE_SEALING_FUNCTION:
        if (NULL == mp_MotorRV) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if ( 0 ==  TestRVSelectingAndSealing(TestId) ) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::ROTARY_VALVE_HEATING_STATION:
        if (NULL == mp_TempRV) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if ( 0 ==  TestRVHeatingStation() ) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::ROTARY_VALVE_HEATING_END:
        if (NULL == mp_TempRV) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if ( 0 ==  TestRVHeatingEnd() ) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::SYSTEM_SPEARKER:
        TestSystemSpeaker();
        break;
    case Service::SYSTEM_110V_220V_SWITCH:
        emit ReturnManufacturingTestMsg(TestSystem110v220vSwitch());
        break;
    case Service::SYSTEM_REMOTE_LOCAL_ALARM:
        TestSystemAlarm();
        break;
    default:
        break;
    }

    qDebug()<<"ManufacturingTestHandler::PerformModuleManufacturingTest  test="<<TestId<<"  End !!";
}

} // end namespace DeviceControl
