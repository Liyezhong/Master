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
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBootLoader.h>

#include "Core/Include/CMessageString.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "Global/Include/SystemPaths.h"
#include "DeviceControl/Include/DeviceProcessing/DeviceLifeCycleRecord.h"
#include "DataManager/Containers/InstrumentHistory/Include/InstrumentHistory.h"
#include <QDebug>
#include <QApplication>
#include "Main/Include/HimalayaServiceEventCodes.h"

//lint -e613
//lint -e505

#define RV_MOVE_OK              1 //!< rotary valve move result


#define  OPTION_NODE_INDEX_DIP    0x0001   //!< DIP switch present
#define  OPTION_STATUS_LED        0x0002   //!< Status LED present

#define  OPTION_PROCESS_DATA      0x0010   //!< Enable process data
#define  OPTION_TRIGGER_EVENTS    0x0020   //!< Enable event handling
#define  OPTION_LIFECYCLE_DATA    0x0040   //!< Enable life cycle counters
#define  OPTION_IDENTIFICATION    0x0080   //!< Enable identification phase

#define  OPTION_TASK_STATISTICS   0x0100   //!< Enable task statistics
#define  OPTION_CURRENT_MONITOR   0x0200   //!< Enable current monitoring
#define  OPTION_VOLTAGE_MONITOR   0x0400   //!< Enable voltage monitoring
#define  OPTION_SAFE_MONITOR      0x0800   //!< Enable safe voltage monitoring
#define  OPTION_INFO_MESSAGES     0x1000   //!< Enable info messages
#define  OPTION_WAIT_POWER_UP     0x2000   //!< Enable wait-for-power-good

#define BASEMODULE_MODULE_ID  "0"       //!< Id of base module
#define DEFAULT_NODE_INDEX    "0"       //!< Node index (if no DIP switch)

#define INFOBLOCK_SIGNATURE  0x88888888  //!< Signature for valid info blocks

//lint -e438
//lint -e514
//lint -e515
//lint -e516
//lint -e525
//lint -e64
//lint -e84
//lint -e1025

namespace DeviceControl {

/****************************************************************************/
ManufacturingTestHandler::ManufacturingTestHandler(IDeviceProcessing &iDevProc)
    : m_rIdevProc(iDevProc),
      m_IsConfigured(false)
{
    m_UserAbort = false;
    mp_Utils = NULL;

    mp_TempOvenTop = NULL;
    mp_TempOvenBottom = NULL;
    mp_DigitalInputOven = NULL;
    mp_DigitalOutputMainRelay = NULL;

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
    mp_SpeakProc  = NULL;

    m_Continue = false;
    m_BootFirmwareFinished = false;
}



/****************************************************************************/
void ManufacturingTestHandler::CreateWrappers()
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
        mp_DigitalInputOven = new WrapperFmDigitalInput("digitalinput_oven", pDigitalInput, this);
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
        mp_DigitalOutputMainRelay = new WrapperFmDigitalOutput("heater_relay", pDigitalOutput, this);
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
        (void)connect(pTemperature, SIGNAL(ReportLevelSensorState(quint32, ReturnCode_t, quint8)), mp_PressPump, SLOT(OnLevelSensorState(quint32,ReturnCode_t,quint8)));
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
    Q_UNUSED(id);
    Q_UNUSED(Ref);

    if(!IsInitialized()) {
        Initialize();
    }
    if(NULL == mp_Utils) {
        emit ReturnErrorMessagetoMain(Service::MSG_DEVICE_NOT_INITIALIZED);
        return;
    }
    switch (AbortTestCaseId) {
    case Service::SYSTEM_SPEARKER:
        if (mp_SpeakProc) {
            mp_SpeakProc->terminate();
        }
        break;
    case Service::SYSTEM_REMOTE_LOCAL_ALARM:
    {
        (void)mp_DORemoteAlarm->SetHigh();
        (void)mp_DOLocalAlarm->SetHigh();
        break;
    }
    case Service::OVEN_COVER_SENSOR:
        break;
    case Service::RETORT_LID_LOCK:
        break;
    case Service::SYSTEM_EXHAUST_FAN:
        AbortExhaustFanOperation();
        break;
    default:
        mp_Utils->AbortPause();
    }
    m_UserAbort = true;
}

void ManufacturingTestHandler::AbortExhaustFanOperation()
{
    (void)mp_PressPump->SetFan(0);
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

    Service::ModuleTestCaseID Id = Service::OVEN_HEATING_WITH_WATER;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);


    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QTime DurationTime1 = QTime::fromString(p_TestCase->GetParameter("DurationTime1"), "hh:mm:ss");
    QTime DurationTime2 = QTime::fromString(p_TestCase->GetParameter("DurationTime2"), "hh:mm:ss");
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble();
    qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal HeatTargetTemp1 = p_TestCase->GetParameter("HeatTargetTemp1").toDouble();
    qreal HeatTargetTemp2 = p_TestCase->GetParameter("HeatTargetTemp2").toDouble();

    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    int DurSec1 = DurationTime1.hour()*60*60 + DurationTime1.minute()*60 + DurationTime1.second();
    int DurSec2 = DurationTime2.hour()*60*60 + DurationTime2.minute()*60 + DurationTime2.second();

    WaitSec = DurSec1 + DurSec2;

    SumSec = WaitSec;

    p_TestCase->ResetResult();

    int num = 10;
    while(num) {
        CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
        CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
        CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);
        if (CurrentTempTop==-1 || CurrentTempBottom1==-1 || CurrentTempBottom2==-1) {
            mp_Utils->Pause(100);
            num--;
        }
        else {
            break;
        }
    }

    if (CurrentTempTop<AmbTempLow || CurrentTempTop>AmbTempHigh ||
            CurrentTempBottom1<AmbTempLow || CurrentTempBottom1>AmbTempHigh ||
            CurrentTempBottom2<AmbTempLow || CurrentTempBottom2>AmbTempHigh){
        QString FailureMsg = Service::CMessageString::MSG_DIAGNOSTICS_OVEN_TEMP_NO_MATCH.arg(CurrentTempTop).arg(CurrentTempBottom1)
                .arg(CurrentTempBottom2).arg(AmbTempLow).arg(AmbTempHigh);

        SetFailReason(Id, FailureMsg);
        p_TestCase->SetStatus(false);
        p_TestCase->AddResult("OutOfRange", "1");

        QString TargetTempStr = QString("%1~%2").arg(TargetTemp+DepartureLow).arg(TargetTemp+DepartureHigh);

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

    (void)mp_DigitalOutputMainRelay->SetHigh();

    (void)mp_TempOvenTop->StopTemperatureControl();
    (void)mp_TempOvenBottom->StopTemperatureControl();
    (void)mp_TempOvenTop->StartTemperatureControl(HeatTargetTemp1);
    (void)mp_TempOvenBottom->StartTemperatureControl(HeatTargetTemp1);
    mp_Utils->Pause(500);

    while (!m_UserAbort && WaitSec)
    {
        QTime EndTime = QTime().currentTime().addSecs(1);

        if ((WaitSec+DurSec1) == SumSec) {

            (void)mp_TempOvenTop->StopTemperatureControl();
            (void)mp_TempOvenBottom->StopTemperatureControl();
            (void)mp_TempOvenTop->StartTemperatureControl(HeatTargetTemp2);
            (void)mp_TempOvenBottom->StartTemperatureControl(HeatTargetTemp2);
        }

        CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
        CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
        CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);

        TopValue = QString("%1").arg(CurrentTempTop);
        BottomValue1 = QString("%1").arg(CurrentTempBottom1);
        BottomValue2 = QString("%1").arg(CurrentTempBottom2);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.clear();
        (void)Status.insert("UsedTime", UsedTime);
        (void)Status.insert("CurrentTempTop", TopValue);
        (void)Status.insert("CurrentTempBottom1", BottomValue1);
        (void)Status.insert("CurrentTempBottom2", BottomValue2);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1~%2").arg(TargetTemp+DepartureLow).arg(TargetTemp+DepartureHigh);

            (void)Status.insert("TargetTemp", TargetTempStr);

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            (void)Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);
        WaitSec--;
        int offset = 0;
        if (WaitSec%10 == 0) {
            offset = 8;
        }
        int MSec = QTime().currentTime().msecsTo(EndTime)-offset;
        if (MSec <= 0) {
            MSec = 1000;
        }
        mp_Utils->Pause(MSec);
    }

    (void)mp_TempOvenTop->StopTemperatureControl();
    (void)mp_TempOvenBottom->StopTemperatureControl();
    (void)mp_DigitalOutputMainRelay->SetLow();


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

        EmitRefreshTestStatustoMain(TestCaseName, INFORM_DONE);
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

    Service::ModuleTestCaseID Id = Service::OVEN_HEATING_EMPTY;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);


    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QTime DurationTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble();
    qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    //qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal MinTargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
    //qreal MaxTargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureHigh").toDouble();

    qreal HeatTargetTemp = p_TestCase->GetParameter("HeatTargetTemp").toDouble();
    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    int KeepTimeSum = 60;

    int KeepSeconds = 0;
    Service::ModuleTestStatus Status;

    p_TestCase->ResetResult();

    WaitSec = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second() + KeepTimeSum;

    SumSec = WaitSec;

    int num = 10;
    while(num) {
        CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
        CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
        CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);
        if (CurrentTempTop==-1 || CurrentTempBottom1==-1 || CurrentTempBottom2==-1) {
            mp_Utils->Pause(100);
            num--;
        }
        else {
            break;
        }
    }

    if (CurrentTempTop<AmbTempLow || CurrentTempTop>AmbTempHigh ||
            CurrentTempBottom1<AmbTempLow || CurrentTempBottom1>AmbTempHigh ||
            CurrentTempBottom2<AmbTempLow || CurrentTempBottom2>AmbTempHigh ){
        QString FailureMsg = Service::CMessageString::MSG_DIAGNOSTICS_OVEN_TEMP_NO_MATCH.arg(CurrentTempTop).arg(CurrentTempBottom1)
                .arg(CurrentTempBottom2).arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Id, FailureMsg);
        p_TestCase->AddResult("OutOfRange", "1");
        p_TestCase->SetStatus(false);

        //QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTemp).arg(DepartureLow).arg(DepartureHigh);
        QString TargetTempStr = QString(">= %1").arg(TargetTemp+DepartureLow);


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

    (void)mp_DigitalOutputMainRelay->SetHigh();

    (void)mp_TempOvenTop->StopTemperatureControl();
    (void)mp_TempOvenBottom->StopTemperatureControl();
    (void)mp_TempOvenTop->StartTemperatureControl(HeatTargetTemp);
    (void)mp_TempOvenBottom->StartTemperatureControl(HeatTargetTemp);

    mp_Utils->Pause(500);

    while (!m_UserAbort && WaitSec)
    {
        QTime EndTime = QTime().currentTime().addSecs(1);

        CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
        CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
//        mp_Utils->Pause(200);
        CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);

        TopValue = QString("%1").arg(CurrentTempTop);
        BottomValue1 = QString("%1").arg(CurrentTempBottom1);
        BottomValue2 = QString("%1").arg(CurrentTempBottom2);

        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        (void)Status.insert("UsedTime", UsedTime);
        (void)Status.insert("CurrentTempTop", TopValue);
        (void)Status.insert("CurrentTempBottom1", BottomValue1);
        (void)Status.insert("CurrentTempBottom2", BottomValue2);
        if (WaitSec == SumSec) {
            //QString TargetTempStr = QString("%1 (%2~%3)").arg(TargetTemp).arg(DepartureLow).arg(DepartureHigh);
            QString TargetTempStr = QString(">= %1").arg(TargetTemp+DepartureLow);

            (void)Status.insert("TargetTemp", TargetTempStr);

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            (void)Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        if ( CurrentTempTop >= MinTargetTemp &&
             CurrentTempBottom1 >= MinTargetTemp &&
             CurrentTempBottom2 >= MinTargetTemp )
        {
            if ( KeepSeconds > KeepTimeSum ) {
                OvenStatus = 1;
                break;
            }
            else {
                KeepSeconds++;
            }
        }
        else {
            KeepSeconds = 0;
            if (WaitSec <= 60) {
                break;
            }
        }

        int MSec = QTime().currentTime().msecsTo(EndTime);
        if (MSec <= 0) {
            MSec = 1000;
        }
        mp_Utils->Pause(MSec);
        WaitSec--;
    }

    (void)mp_TempOvenTop->StopTemperatureControl();
    (void)mp_TempOvenBottom->StopTemperatureControl();
    (void)mp_DigitalOutputMainRelay->SetLow();

    p_TestCase->AddResult("UsedTime", UsedTime);
    p_TestCase->AddResult("CurrentTempTop", TopValue);
    p_TestCase->AddResult("CurrentTempBottom1", BottomValue1);
    p_TestCase->AddResult("CurrentTempBottom2", BottomValue2);

    if ( OvenStatus == -1 )  // failed.
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


qint32 ManufacturingTestHandler::TestOvenCoverSensor()
{
    if (mp_DigitalInputOven == NULL) {
        SetFailReason(Service::OVEN_COVER_SENSOR, Service::MSG_DEVICE_NOT_INITIALIZED);
        emit ReturnManufacturingTestMsg(false);
        return 0;
    }

    Service::ModuleTestStatus Status;
    qint32 Value = mp_DigitalInputOven->GetValue();

    if ( Value == 0) { //  oven cover sensor status : close
        (void)Status.insert("OvenCoverSensorStatus", Service::CMessageString::MSG_DIAGNOSTICS_STATUS_CLOSE);
    }
    else {
        (void)Status.insert("OvenCoverSensorStatus", Service::CMessageString::MSG_DIAGNOSTICS_STATUS_OPEN);
    }

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::OVEN_COVER_SENSOR);
    emit RefreshTestStatustoMain(TestCaseName, Status);

    return 0;
}

qint32 ManufacturingTestHandler::TestLidLocker()
{
    if (NULL == mp_DIRetortLid) {
        SetFailReason(Service::RETORT_LID_LOCK, Service::MSG_DEVICE_NOT_INITIALIZED);
        emit ReturnManufacturingTestMsg(false);
        return 0;
    }

    Service::ModuleTestStatus status;
    qint32 value = mp_DIRetortLid->GetValue();
    if (value == 0) { //  Lid locker status : close
        (void)status.insert("LidLockerStatus", Service::CMessageString::MSG_DIAGNOSTICS_STATUS_CLOSE);
    }
    else {
        (void)status.insert("LidLockerStatus", Service::CMessageString::MSG_DIAGNOSTICS_STATUS_OPEN);
    }

    QString testCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::RETORT_LID_LOCK);
    emit RefreshTestStatustoMain(testCaseName, status);
    return 0;
}

qint32 ManufacturingTestHandler::TestRetortHeating()
{
    Service::ModuleTestCaseID Id = Service::RETORT_HEATING_EMPTY;
    QString testCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(testCaseName);

    QTime durTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
    qreal tgtTemp = p_TestCase->GetParameter("TargetTemp").toDouble();
    qreal deptLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal deptHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal minTgtTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal maxTgtTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureHigh").toDouble();

    qreal sideTgtTemp = p_TestCase->GetParameter("SideTargetTemp").toDouble();
    qreal btmTgtTemp  = p_TestCase->GetParameter("BottomTargetTemp").toDouble();
    qreal ambLow  = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal ambHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();
    qreal Difference = p_TestCase->GetParameter("Difference").toDouble();


    qreal curSideTemp   = 0;
    qreal curBottomTemp1 = 0;
    qreal curBottomTemp2 = 0;

    int num = 10;
    while(num) {
        curSideTemp   = mp_TempRetortSide->GetTemperature(0);
        curBottomTemp1 = mp_TempRetortBottom->GetTemperature(0);
        curBottomTemp2 = mp_TempRetortBottom->GetTemperature(1);

        if (curSideTemp==-1 || curBottomTemp1==-1 || curBottomTemp2==-1) {
            mp_Utils->Pause(100);
            num--;
        }
        else {
            break;
        }
    }

    p_TestCase->ResetResult();

    qDebug()<<"Difference = "<<Difference;

    const quint32 sumSec = durTime.hour() * 60 * 60 + durTime.minute() * 60 + durTime.second() + 60;
    quint32 waitSec = sumSec;

    QString sideTemp, btmTemp1, btmTemp2;

    if (curSideTemp < ambLow || curSideTemp > ambHigh || curBottomTemp1 < ambLow || curBottomTemp1 > ambHigh
            || curBottomTemp2 < ambLow || curBottomTemp2 > ambHigh) {
        QString failMsg = Service::CMessageString::MSG_DIAGNOSTICS_RETORT_TEMP_NO_MATCH.arg(curSideTemp)
                .arg(curBottomTemp1).arg(curBottomTemp2).arg(ambLow).arg(ambHigh);
        SetFailReason(Id, failMsg);
        p_TestCase->SetStatus(false);
        p_TestCase->AddResult("OutOfRange", "1");

        QString target = QString("%1~%2").arg(tgtTemp+deptLow).arg(tgtTemp+deptHigh);
        QString duration = QTime().addSecs(sumSec).toString("hh:mm:ss");
        p_TestCase->AddResult("Duration", duration);
        p_TestCase->AddResult("TargetTemp", target);

        sideTemp = QString("%1").arg(curSideTemp);
        btmTemp1  = QString("%1").arg(curBottomTemp1);
        btmTemp2  = QString("%1").arg(curBottomTemp2);
        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTempSide", sideTemp);
        p_TestCase->AddResult("CurrentTempBottom1", btmTemp1);
        p_TestCase->AddResult("CurrentTempBottom2", btmTemp2);

        return -1;
    }

    (void)mp_DigitalOutputMainRelay->SetHigh();
    (void)mp_TempRetortSide->StopTemperatureControl();
    (void)mp_TempRetortBottom->StopTemperatureControl();
    (void)mp_TempRetortSide->StartTemperatureControl(sideTgtTemp+7);
    (void)mp_TempRetortBottom->StartTemperatureControl(btmTgtTemp+2);

    Service::ModuleTestStatus testStat;
    QString target = QString("%1~%2").arg(tgtTemp+deptLow).arg(tgtTemp+deptHigh);
    (void)testStat.insert("TargetTemp", target);

    QString duration = QTime().addSecs(sumSec).toString("hh:mm:ss");
    (void)testStat.insert("Duration", duration);
    p_TestCase->AddResult("Duration", duration);
    p_TestCase->AddResult("TargetTemp", target);

    QString usedTime;
    //qint32  keepSec  = 0;
    qint32 progStat = 1;
    while (!m_UserAbort && waitSec) {
        QTime EndTime = QTime().currentTime().addSecs(1);

        curSideTemp   = mp_TempRetortSide->GetTemperature(0);
        curBottomTemp1 = mp_TempRetortBottom->GetTemperature(0);
        //mp_Utils->Pause(300);
        curBottomTemp2 = mp_TempRetortBottom->GetTemperature(1);

        if (waitSec <= 60) {
            if (curSideTemp>maxTgtTemp||curSideTemp<minTgtTemp ||
                    curBottomTemp1>maxTgtTemp||curBottomTemp1<minTgtTemp ||
                    qAbs(curBottomTemp1-curBottomTemp2) >= Difference ) {

                //qDebug()<<"Side="<<curSideTemp<<"  curBottomTemp1="<<curBottomTemp1<<"   curBottomTemp2="<<curBottomTemp2;

                progStat = -1;

                sideTemp = QString("%1").arg(curSideTemp);
                btmTemp1  = QString("%1").arg(curBottomTemp1);
                btmTemp2  = QString("%1").arg(curBottomTemp2);
                usedTime = QTime().addSecs(sumSec - waitSec).toString("hh:mm:ss");
                (void)testStat.insert("UsedTime", usedTime);
                (void)testStat.insert("CurrentTempSide", sideTemp);
                (void)testStat.insert("CurrentTempBottom1", btmTemp1);
                (void)testStat.insert("CurrentTempBottom2", btmTemp2);

                break;
            }
        }
#if 0
        if (curSideTemp >= minTgtTemp &&
                curBottomTemp1 >= minTgtTemp &&
                curBottomTemp2 >= minTgtTemp) {
            if (keepSec > 60) {
                progStat = 1;
                break;
            }
            ++ keepSec;
            if (curSideTemp>maxTgtTemp || curBottomTemp1>maxTgtTemp || curBottomTemp2>maxTgtTemp) {
                progStat = -1;

                sideTemp = QString("%1").arg(curSideTemp);
                btmTemp1  = QString("%1").arg(curBottomTemp1);
                btmTemp2  = QString("%1").arg(curBottomTemp2);
                usedTime = QTime().addSecs(sumSec - waitSec).toString("hh:mm:ss");

                (void)testStat.insert("UsedTime", usedTime);
                (void)testStat.insert("CurrentTempSide", sideTemp);
                (void)testStat.insert("CurrentTempBottom1", btmTemp1);
                (void)testStat.insert("CurrentTempBottom2", btmTemp2);

                break;
            }
        }
        else {
            keepSec = 0;
        }
#endif
        sideTemp = QString("%1").arg(curSideTemp);
        btmTemp1  = QString("%1").arg(curBottomTemp1);
        btmTemp2  = QString("%1").arg(curBottomTemp2);
        usedTime = QTime().addSecs(sumSec - waitSec).toString("hh:mm:ss");

        (void)testStat.insert("UsedTime", usedTime);
        (void)testStat.insert("CurrentTempSide", sideTemp);
        (void)testStat.insert("CurrentTempBottom1", btmTemp1);
        (void)testStat.insert("CurrentTempBottom2", btmTemp2);
        emit RefreshTestStatustoMain(testCaseName, testStat);

        int MSec = QTime().currentTime().msecsTo(EndTime);
        if (MSec <= 0) {
            MSec = 1000;
        }
        mp_Utils->Pause(MSec);

        -- waitSec;
    }

    (void)mp_TempRetortSide->StopTemperatureControl();
    (void)mp_TempRetortBottom->StopTemperatureControl();
    (void)mp_DigitalOutputMainRelay->SetLow();

    p_TestCase->AddResult("UsedTime", usedTime);
    p_TestCase->AddResult("CurrentTempSide", sideTemp);
    p_TestCase->AddResult("CurrentTempBottom1", btmTemp1);
    p_TestCase->AddResult("CurrentTempBottom2", btmTemp2);

    int exitStat = -1;
    if (m_UserAbort) {
        p_TestCase->AddResult("FailReason", "Abort");
        m_UserAbort = false;
        exitStat = 1;
    }
    else if (progStat == 1) {
        exitStat = 0;
    }
    p_TestCase->SetStatus(exitStat == 0 ? true : false);

    return exitStat;
}

qint32 ManufacturingTestHandler::TestRetortLevelSensorHeating()
{
    const quint16 TargetTemperature = 115;
    const quint16 ControllerGainHigh = 120;
    const quint16 ResetTimeHigh = 1212;
    const quint16 DerivativeTimeHigh = 80;

    const quint16 ControllerGainLow = 200;
    const quint16 ResetTimeLow = 1000;
    const quint16 DerivativeTimeLow = 0;

    const quint16 TargetDropRange = 6;

    const quint16 MinTemperature = 110;
    const quint16 MaxTemperature = 120;
    const quint16 ExchangeTemperature = 110;

    Service::ModuleTestCaseID Id = Service::RETORT_LEVEL_SENSOR_HEATING;

    QString testCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(testCaseName);

    QTime durTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");

    qreal ambLow  = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal ambHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();



    Service::ModuleTestStatus testStat;

    int duration = durTime.hour() * 60 * 60 + durTime.minute() * 60 + durTime.second();
    int waitSeconds = 0;
    qreal curTemp = 0;
    int totalSeconds = 0;
    int remainder = duration % 10;
    int dd = 2*60;
    bool isTouched = false;
    int steadyNum = 0;
    int steadyTime = 0;
    bool isOver = false;

    duration -= remainder;

    p_TestCase->ResetResult();
    int num = 10;
    while(num) {
        curTemp = mp_TempLSensor->GetTemperature();
        if (curTemp == -1) {
            mp_Utils->Pause(100);
            num--;
        }
        else {
            break;
        }
    }

    if (curTemp <ambLow || curTemp > ambHigh) {
        QString FailureMsg = Service::CMessageString::MSG_DIAGNOSTICS_LEVEL_SENSOR_TEMP_NO_MATCH.arg(curTemp).arg(ambLow).arg(ambHigh);
        SetFailReason(Id, FailureMsg);
        p_TestCase->SetStatus(false);
        p_TestCase->AddResult("OutOfRange", "1");
        goto ERROR_EXIT;
    }

    // pre-heating
    (void)mp_TempLSensor->SetTemperaturePid(132, ControllerGainHigh,ResetTimeHigh, DerivativeTimeHigh);
    (void)mp_TempLSensor->StartTemperatureControl(TargetTemperature, TargetDropRange);
    mp_Utils->Pause(500);
    p_TestCase->AddResult("UsedTime", "00:00:00");
    p_TestCase->AddResult("TargetTemp", QString(">115"));
    (void)testStat.insert("TargetTemp", QString(">115"));
    (void)testStat.insert("Duration", "00:02:00");
    (void)testStat.insert("labelText", QApplication::translate("Core::CStartup", "Pre-heating...", 0, QApplication::UnicodeUTF8));


    while(!m_UserAbort && dd) {
        QTime EndTime = QTime().currentTime().addSecs(1);

        curTemp = mp_TempLSensor->GetTemperature();
        if (curTemp > 115) {
            isTouched = true;
            break;
        }
        (void)testStat.insert("UsedTime", QTime().addSecs(120-dd).toString("hh:mm:ss"));
        (void)testStat.insert("CurrentTemp", QString("%1").arg(curTemp));
        emit RefreshTestStatustoMain(testCaseName, testStat);

        int MSec = QTime().currentTime().msecsTo(EndTime);
        if (MSec <= 0) {
            MSec = 1000;
        }
        mp_Utils->Pause(MSec);
        dd--;
    }
    (void)mp_TempLSensor->StopTemperatureControl();
    if (!isTouched) {
        if (m_UserAbort) {
            m_UserAbort = false;
            p_TestCase->AddResult("FailReason", "Abort");
        }
        p_TestCase->AddResult("TargetTemp",  QString(">115"));
        p_TestCase->AddResult("CurrentTemp", QString("%1").arg(curTemp));
        p_TestCase->AddResult("Duration", "00:02:00");
        p_TestCase->AddResult("UsedTime", QTime().addSecs(120-dd).toString("hh:mm:ss"));
        return -1;
    }
    // cooling
    dd = 15*60; // 15 min
    isTouched = false;
    p_TestCase->AddResult("UsedTime", "00:00:00");
    p_TestCase->AddResult("TargetTemp", QString("<35"));
    (void)testStat.insert("TargetTemp", QString("<35"));
    (void)testStat.insert("Duration", "00:15:00");
    (void)testStat.insert("labelText",
                          QApplication::translate("Core::CStartup", "Cooling...", 0, QApplication::UnicodeUTF8));

    while(!m_UserAbort && dd) {
        QTime EndTime = QTime().currentTime().addSecs(1);

        curTemp = mp_TempLSensor->GetTemperature();
        if (curTemp < 35) {
            isTouched = true;
            break;
        }
        (void)testStat.insert("UsedTime", QTime().addSecs(900-dd).toString("hh:mm:ss"));
        (void)testStat.insert("CurrentTemp", QString("%1").arg(curTemp));
        emit RefreshTestStatustoMain(testCaseName, testStat);

        int MSec = QTime().currentTime().msecsTo(EndTime);
        if (MSec <= 0) {
            MSec = 1000;
        }
        mp_Utils->Pause(MSec);
        dd--;
    }
    if (!isTouched) {
        if (m_UserAbort) {
            m_UserAbort = false;
            p_TestCase->AddResult("FailReason", "Abort");
        }
        p_TestCase->AddResult("TargetTemp",  QString("<35"));
        p_TestCase->AddResult("CurrentTemp", QString("%1").arg(curTemp));
        p_TestCase->AddResult("Duration", "00:15:00");
        p_TestCase->AddResult("UsedTime", QTime().addSecs(900-dd).toString("hh:mm:ss"));
        return -1;
    }
    //
    (void)testStat.insert("TargetTemp", QString("%1~%2").arg(MinTemperature).arg(MaxTemperature));
    (void)testStat.insert("Duration", durTime.toString());
    (void)testStat.insert("labelText", QApplication::translate("Core::CStartup", "Heating...", 0, QApplication::UnicodeUTF8));

    p_TestCase->AddResult("UsedTime", "00:00:00");
    p_TestCase->AddResult("TargetTemp", QString("%1~%2").arg(MinTemperature).arg(MaxTemperature));
    waitSeconds = duration;

    for(int i=0; i<3; i++) {
        if (i==0) {
            (void)mp_TempLSensor->StopTemperatureControl();
            (void)mp_TempLSensor->SetTemperaturePid(132, ControllerGainHigh,ResetTimeHigh, DerivativeTimeHigh);
            (void)mp_TempLSensor->StartTemperatureControl(TargetTemperature, TargetDropRange);
            mp_Utils->Pause(200);
        }
        else if (i==1){
            (void)mp_TempLSensor->StopTemperatureControl();
            (void)mp_TempLSensor->SetTemperaturePid(132, ControllerGainLow,ResetTimeLow, DerivativeTimeLow);
            (void)mp_TempLSensor->StartTemperatureControl(TargetTemperature, TargetDropRange);
            mp_Utils->Pause(200);
        }
        else {
            waitSeconds = remainder;
        }

        while (!m_UserAbort && waitSeconds) {
            QTime EndTime = QTime().currentTime().addSecs(1);

            curTemp = mp_TempLSensor->GetTemperature();
#if 0
            if (totalSeconds == 10 &&
                    (curTemp > 93 || curTemp < 71)) {  // (82-11) ~ (82+11)
                goto ERROR_EXIT;
            }
#else
            if (i==1 && curTemp > MaxTemperature) {
                isOver = true;
            }

            if (isOver && steadyNum<6) {
                if (curTemp >=MinTemperature && curTemp <= MaxTemperature) {
                    steadyNum++;
                }
                else {
                    steadyNum=0;
                }

                if (steadyNum == 6) {
                    steadyTime = totalSeconds - 5 ;
                }
            }
#endif
            if (i==0 && curTemp >= ExchangeTemperature) {
                steadyTime = totalSeconds;
                break;
            }
            if (i==2 && (curTemp<MinTemperature||curTemp>MaxTemperature)) {
                goto ERROR_EXIT;
            }

            if (curTemp != -1) {
                (void)testStat.insert("UsedTime", QTime().addSecs(totalSeconds).toString("hh:mm:ss"));
                (void)testStat.insert("CurrentTemp", QString("%1").arg(curTemp));
                emit RefreshTestStatustoMain(testCaseName, testStat);
            }

            int MSec = QTime().currentTime().msecsTo(EndTime);
            if (MSec <= 0) {
                MSec = 1000;
            }
            mp_Utils->Pause(MSec);

            -- waitSeconds;

            totalSeconds++;
        }
        if (m_UserAbort ||
                (i==0 && waitSeconds==0) ||
                (i==1 && waitSeconds==0 && curTemp<MinTemperature)) {
            if (m_UserAbort) {
                m_UserAbort = false;
                p_TestCase->AddResult("FailReason", "Abort");
            }
            goto ERROR_EXIT;
        }
    }
    if (m_UserAbort) {
        m_UserAbort = false;
        p_TestCase->AddResult("FailReason", "Abort");
    }

    (void)mp_TempLSensor->StopTemperatureControl();

    p_TestCase->AddResult("TargetTemp",  QString("%1~%2").arg(MinTemperature).arg(MaxTemperature));
    p_TestCase->AddResult("CurrentTemp", QString("%1").arg(curTemp));
    p_TestCase->AddResult("Duration", durTime.toString());
    p_TestCase->AddResult("UsedTime", QTime().addSecs(totalSeconds).toString("hh:mm:ss"));

    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("Level sensor temperatue is steady at %1 seconds.").arg(steadyTime));
    return 0;

ERROR_EXIT:
    (void)mp_TempLSensor->StopTemperatureControl();
    p_TestCase->AddResult("TargetTemp",  QString("%1~%2").arg(MinTemperature).arg(MaxTemperature));
    p_TestCase->AddResult("CurrentTemp", QString("%1").arg(curTemp));
    p_TestCase->AddResult("Duration", durTime.toString());
    p_TestCase->AddResult("UsedTime", QTime().addSecs(totalSeconds).toString("hh:mm:ss"));
    return -1;
}

#if 0
qint32 ManufacturingTestHandler::TestRetortLevelSensorHeating11()
{
    const quint16 TargetTemperature = 115;
    const quint16 ControllerGainHigh = 120;
    const quint16 ResetTimeHigh = 1212;
    const quint16 DerivativeTimeHigh = 80;

    const quint16 ControllerGainLow = 200;
    const quint16 ResetTimeLow = 1000;
    const quint16 DerivativeTimeLow = 0;

    const quint16 TargetDropRange = 6;
    const quint16 OverTemperature = 135;

    const quint16 MinTemperature = 110;
    const quint16 MaxTemperature = 120;
    const quint16 ExchangeTemperature = 110;

    Service::ModuleTestCaseID Id = Service::RETORT_LEVEL_SENSOR_HEATING;

    QString testCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(testCaseName);

    QTime durTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");

    qreal ambLow  = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal ambHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();



    Service::ModuleTestStatus testStat;
    (void)testStat.insert("TargetTemp", QString("%1~%2").arg(MinTemperature).arg(MaxTemperature));
    (void)testStat.insert("Duration", durTime.toString());

    int duration = durTime.hour() * 60 * 60 + durTime.minute() * 60 + durTime.second();
    int waitSeconds = 0;
    qreal curTemp = 0;
    int totalSeconds = 0;
    int remainder = duration % 10;

    duration -= remainder;

    p_TestCase->ResetResult();
    int num = 10;
    while(num) {
        curTemp = mp_TempLSensor->GetTemperature();
        if (curTemp == -1) {
            mp_Utils->Pause(100);
            num--;
        }
        else {
            break;
        }
    }

    if (curTemp <ambLow || curTemp > ambHigh) {
        QString FailureMsg = Service::CMessageString::MSG_DIAGNOSTICS_LEVEL_SENSOR_TEMP_NO_MATCH.arg(curTemp).arg(ambLow).arg(ambHigh);
        SetFailReason(Id, FailureMsg);
        p_TestCase->SetStatus(false);
        p_TestCase->AddResult("OutOfRange", "1");
        goto ERROR_EXIT;
    }

    p_TestCase->AddResult("UsedTime", "00:00:00");
    p_TestCase->AddResult("TargetTemp", QString("%1~%2").arg(MinTemperature).arg(MaxTemperature));
    waitSeconds = duration;

    for(int i=0; i<3; i++) {
        if (i==0) {
            (void)mp_TempLSensor->StopTemperatureControl();
            (void)mp_TempLSensor->SetTemperaturePid(132, ControllerGainHigh,ResetTimeHigh, DerivativeTimeHigh);
            (void)mp_TempLSensor->StartTemperatureControl(TargetTemperature, TargetDropRange);
            mp_Utils->Pause(200);
        }
        else if (i==1){
            (void)mp_TempLSensor->StopTemperatureControl();
            (void)mp_TempLSensor->SetTemperaturePid(132, ControllerGainLow,ResetTimeLow, DerivativeTimeLow);
            (void)mp_TempLSensor->StartTemperatureControl(TargetTemperature, TargetDropRange);
            mp_Utils->Pause(200);
        }
        else {
            waitSeconds = remainder;
        }

        while (!m_UserAbort && waitSeconds) {
            QTime EndTime = QTime().currentTime().addSecs(1);

            curTemp = mp_TempLSensor->GetTemperature();

            if (curTemp > OverTemperature) {
                goto ERROR_EXIT;
            }
            else if (i==0 && curTemp >= ExchangeTemperature) {
                break;
            }
            else if (i==2 && (curTemp<MinTemperature||curTemp>MaxTemperature)) {
                goto ERROR_EXIT;
            }

            if (curTemp != -1) {
                (void)testStat.insert("UsedTime", QTime().addSecs(totalSeconds).toString("hh:mm:ss"));
                (void)testStat.insert("CurrentTemp", QString("%1").arg(curTemp));
                emit RefreshTestStatustoMain(testCaseName, testStat);
            }

            int MSec = QTime().currentTime().msecsTo(EndTime);
            if (MSec <= 0) {
                MSec = 1000;
            }
            mp_Utils->Pause(MSec);

            -- waitSeconds;

            totalSeconds++;
        }
        if (m_UserAbort ||
                (i==0 && waitSeconds==0) ||
                (i==1 && waitSeconds==0 && curTemp<MinTemperature)) {
            if (m_UserAbort) {
                m_UserAbort = false;
                p_TestCase->AddResult("FailReason", "Abort");
            }
            goto ERROR_EXIT;
        }
    }
    if (m_UserAbort) {
        m_UserAbort = false;
        p_TestCase->AddResult("FailReason", "Abort");
    }

    (void)mp_TempLSensor->StopTemperatureControl();

    p_TestCase->AddResult("TargetTemp",  QString("%1~%2").arg(MinTemperature).arg(MaxTemperature));
    p_TestCase->AddResult("CurrentTemp", QString("%1").arg(curTemp));
    p_TestCase->AddResult("Duration", durTime.toString());
    p_TestCase->AddResult("UsedTime", QTime().addSecs(totalSeconds).toString("hh:mm:ss"));
    return 0;

ERROR_EXIT:
    (void)mp_TempLSensor->StopTemperatureControl();
    p_TestCase->AddResult("TargetTemp",  QString("%1~%2").arg(MinTemperature).arg(MaxTemperature));
    p_TestCase->AddResult("CurrentTemp", QString("%1").arg(curTemp));
    p_TestCase->AddResult("Duration", durTime.toString());
    p_TestCase->AddResult("UsedTime", QTime().addSecs(totalSeconds).toString("hh:mm:ss"));
    return -1;
}
#endif

qint32 ManufacturingTestHandler::TestRetortLevelSensorDetecting()
{

    QString testCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::RETORT_LEVEL_SENSOR_DETECTING);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(testCaseName);

    int result = 0;
    //int bottlePos = p_TestCase->GetParameter("Position").toInt();
    const int bottlePos = 1;
    p_TestCase->ResetResult();

    EmitRefreshTestStatustoMain(testCaseName, RV_INITIALIZING);
    if(mp_MotorRV->MoveToInitialPosition()!=RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(testCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_INIT_RV_FAILED);
        return -1;
    }

    EmitRefreshTestStatustoMain(testCaseName, RV_MOVE_TO_TUBE_POSITION, bottlePos);
    //mp_Utils->Pause(1000);
    if(mp_MotorRV->MoveToTubePosition(bottlePos)!=RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(testCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg(bottlePos));
        return -1;
    }

    EmitRefreshTestStatustoMain(testCaseName, LS_HEATING);
    if (-1 == HeatingLevelSensor()) {
         p_TestCase->AddResult("TestResult", "Level sensor self-test is failed.");
         EmitRefreshTestStatustoMain(testCaseName, HIDE_MESSAGE);
         return -1;
    }

    EmitRefreshTestStatustoMain(testCaseName, RETORT_FILLING);
    result = mp_PressPump->Sucking(0, bottlePos, false);
    if (result != SUCKING_RET_OK ) {
        result = -1;
    }
    else {
        // Level sensor detected!
        result = 0;
    }
    (void)mp_TempLSensor->StopTemperatureControl();

    if (result == 0) {
        EmitRefreshTestStatustoMain(testCaseName, RV_MOVE_TO_SEALING_POSITION, bottlePos);
        if (mp_MotorRV->MoveToSealPosition(bottlePos)!=RV_MOVE_OK) {
            p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED.arg(bottlePos));
            result = -1;
        }
        else {
            //Tell the operator to comfirm the water level;
            (void)mp_PressPump->ReleasePressure();
            EmitRefreshTestStatustoMain(testCaseName, WAIT_CONFIRM);
            while(!m_Continue && !m_UserAbort) {
                mp_Utils->Pause(100);
            }
            if (m_Continue == true) {
                m_Continue = false;
            }
            if (m_UserAbort == true) {
                m_UserAbort = false;
            }
            p_TestCase->AddResult("TestResult", "Level sensor self-test is OK.");
        }
    }
    else {
        p_TestCase->AddResult("TestResult", "Level sensor self-test is failed.");
    }

    // draing
    EmitRefreshTestStatustoMain(testCaseName, RV_MOVE_TO_TUBE_POSITION, bottlePos);
    (void)mp_MotorRV->MoveToTubePosition(bottlePos);
    EmitRefreshTestStatustoMain(testCaseName, RETORT_DRAINING);
    (void)mp_PressPump->Draining(0);

    EmitRefreshTestStatustoMain(testCaseName, HIDE_MESSAGE);

    // prompt result
    if (result == 0) {
        EmitRefreshTestStatustoMain(testCaseName, INFORM_DONE);
    }
    // Tell the operator to close the retort lid
    return result;
}

qint32 ManufacturingTestHandler::TestRetortHeatingWater()
{
    Service::ModuleTestCaseID Id = Service::RETORT_HEATING_WITH_WATER;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    int Position = 0;
    int Ret = 0;
    Service::ModuleTestStatus Status;

    int num = 10;
    qreal curSideTemp, curBottomTemp1, curBottomTemp2;
    qreal ambLow  = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal ambHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    p_TestCase->ResetResult();

    while(num) {
        curSideTemp   = mp_TempRetortSide->GetTemperature(0);
        curBottomTemp1 = mp_TempRetortBottom->GetTemperature(0);
        curBottomTemp2 = mp_TempRetortBottom->GetTemperature(1);

        if (curSideTemp==-1 || curBottomTemp1==-1 || curBottomTemp2==-1) {
            mp_Utils->Pause(100);
            num--;
        }
        else {
            break;
        }
    }

    if (curSideTemp < ambLow || curSideTemp > ambHigh || curBottomTemp1 < ambLow || curBottomTemp1 > ambHigh
            || curBottomTemp2 < ambLow || curBottomTemp2 > ambHigh) {
        QString failMsg = Service::CMessageString::MSG_DIAGNOSTICS_RETORT_TEMP_NO_MATCH.arg(curSideTemp)
                .arg(curBottomTemp1).arg(curBottomTemp2).arg(ambLow).arg(ambHigh);

        SetFailReason(Id, failMsg);
        p_TestCase->SetStatus(false);
        p_TestCase->AddResult("OutOfRange", "1");

        return -1;
    }

    Position = p_TestCase->GetParameter("Position").toInt();

    EmitRefreshTestStatustoMain(TestCaseName, RV_INITIALIZING);
    if (mp_MotorRV->MoveToInitialPosition()!=RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_INIT_RV_FAILED);
        return -1;
    }

    EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, Position);
    if (mp_MotorRV->MoveToTubePosition(Position)!=RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg(Position));
        return -1;
    }

    EmitRefreshTestStatustoMain(TestCaseName, LS_HEATING);
    Ret = HeatingLevelSensor();
    if (Ret == -1) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR_FAILED);
        return -1;
    }


    EmitRefreshTestStatustoMain(TestCaseName, RETORT_FILLING);
    Ret = mp_PressPump->Sucking();

    (void)mp_TempLSensor->StopTemperatureControl();

    if (Ret != SUCKING_RET_OK) {
        // if failed, then draining
        EmitRefreshTestStatustoMain(TestCaseName, RETORT_DRAINING);
        (void)mp_PressPump->Draining();
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->SetStatus(false);
        return -1;
    }
    else {
        QString CurrentTempSideValue;
        QString CurrentTempBottomValue1;
        QString CurrentTempBottomValue2;
        QString UsedTime;

        // move to sealing positioin
        EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_SEALING_POSITION, Position);
        if (mp_MotorRV->MoveToSealPosition(Position)!=RV_MOVE_OK) {
            EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
            p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED.arg(Position));

            EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, Position);
            (void)mp_MotorRV->MoveToTubePosition(Position);

            EmitRefreshTestStatustoMain(TestCaseName, RETORT_DRAINING);
            (void)mp_PressPump->Draining();
            EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);

            EmitRefreshTestStatustoMain(TestCaseName, INFORM_DONE);
            return -1;
        }

        (void)mp_PressPump->ReleasePressure();

        // tell operator to put the external sensor to retort.
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        EmitRefreshTestStatustoMain(TestCaseName, WAIT_CONFIRM);
        // wait for the confirm of operator
        while(!m_Continue && !m_UserAbort) {
            mp_Utils->Pause(100);
        }
        if (m_Continue == true) {
            m_Continue = false;
        }

        // begin heating.
        qreal TargetTempSide = p_TestCase->GetParameter("SideTargetTemp").toFloat();
        qreal TargetTempBottom = p_TestCase->GetParameter("BottomTargetTemp").toFloat();
        qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toFloat();
        qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toFloat();
        QTime DurationTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
        qreal CurrentTempSide(0);
        qreal CurrentTempBottom1(0);
        qreal CurrentTempBottom2(0);

        int WaitSec = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second();

        int SumSec = WaitSec;
        if (m_UserAbort == true) {
            goto EXIT_TEST_RETORT_HEATING_WATER;
        }

        (void)mp_TempRetortBottom->StopTemperatureControl();
        (void)mp_TempRetortSide->StopTemperatureControl();

        (void)mp_DigitalOutputMainRelay->SetHigh();
        (void)mp_TempRetortSide->StartTemperatureControl(TargetTempSide+7);
        (void)mp_TempRetortBottom->StartTemperatureControl(TargetTempBottom+2);
        mp_Utils->Pause(500);

        while(!m_UserAbort && WaitSec) {
            QTime EndTime = QTime().currentTime().addSecs(1);

            if (WaitSec == SumSec) {
                Status.clear();
                QString TargetTempStr = QString("%1~%2").arg(TargetTempSide+DepartureLow).arg(TargetTempSide+DepartureHigh);
                (void)Status.insert("TargetTemp", TargetTempStr);

                QString Duration = p_TestCase->GetParameter("DurationTime");
                (void)Status.insert("Duration", Duration);
                p_TestCase->AddResult("Duration", Duration);
                p_TestCase->AddResult("TargetTemp", TargetTempStr);
            }

            CurrentTempSide = mp_TempRetortSide->GetTemperature(0);
            CurrentTempBottom1 = mp_TempRetortBottom->GetTemperature(0);
            CurrentTempBottom2 = mp_TempRetortBottom->GetTemperature(1);

            CurrentTempSideValue = QString("%1").arg(CurrentTempSide);
            CurrentTempBottomValue1 = QString("%1").arg(CurrentTempBottom1);
            CurrentTempBottomValue2 = QString("%1").arg(CurrentTempBottom2);
            UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");


            (void)Status.insert("UsedTime", UsedTime);
            (void)Status.insert("CurrentTempSide", CurrentTempSideValue);
            (void)Status.insert("CurrentTempBottom1", CurrentTempBottomValue1);
            (void)Status.insert("CurrentTempBottom2", CurrentTempBottomValue2);

            emit RefreshTestStatustoMain(TestCaseName, Status);
            WaitSec--;

            int offset = 0;
            if (WaitSec%10 == 0) {
                offset = 8;
            }
            int MSec = QTime().currentTime().msecsTo(EndTime)-offset;
            if (MSec <= 0) {
                MSec = 1000;
            }
            mp_Utils->Pause(MSec);
        }

        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);

        (void)mp_TempRetortSide->StopTemperatureControl();
        (void)mp_TempRetortBottom->StopTemperatureControl();
        (void)mp_DigitalOutputMainRelay->SetLow();


        p_TestCase->AddResult("UsedTime", UsedTime);
        p_TestCase->AddResult("CurrentTempSide", CurrentTempSideValue);
        p_TestCase->AddResult("CurrentTempBottom1", CurrentTempBottomValue1);
        p_TestCase->AddResult("CurrentTempBottom2", CurrentTempBottomValue2);

        if (!m_UserAbort) {
            // wait operator input value of external sensor
            EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
            EmitRefreshTestStatustoMain(TestCaseName, WAIT_CONFIRM2);
            while( !m_Continue && !m_UserAbort) {
                mp_Utils->Pause(1000);
            }
            if (m_Continue == true) {
                m_Continue = false;
            }
        }


EXIT_TEST_RETORT_HEATING_WATER:
        EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, Position);
        (void)mp_MotorRV->MoveToTubePosition(Position);

        EmitRefreshTestStatustoMain(TestCaseName, RETORT_DRAINING);
        (void)mp_PressPump->Draining();
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);

        if (m_UserAbort) {
            m_UserAbort = false;
            p_TestCase->AddResult("FailReason", "Abort");
            p_TestCase->SetStatus(false);
            return 1;
        }

        EmitRefreshTestStatustoMain(TestCaseName, INFORM_DONE);

        return 0;
    }
}


qint32 ManufacturingTestHandler::TestSystemSpeaker()
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

    QString SetVolume;
    QStringList PlayParams;

    SetVolume = "amixer set PCM " + VolumeLevel + "%";
    PlayParams<<"-r"<<Global::SystemPaths::Instance().GetSoundPath() + "/Note6.ogg";


    mp_SpeakProc->start(SetVolume);
    (void)mp_SpeakProc->waitForFinished();

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

    bool AlarmFlag = p_TestCase->GetParameter("AlarmFlag").toInt();
    bool ConnectFlag  = p_TestCase->GetParameter("ConnectFlag").toInt();

    qint32 RetValue(0);
    if (AlarmFlag) {
        RetValue = mp_DIRemoteAlarm->GetValue();
    }
    else {
        RetValue = mp_DILocalAlarm->GetValue();
    }

    if (ConnectFlag) {
        if (AlarmFlag) {
            // remote connect test
            (void)mp_DORemoteAlarm->SetLow();

        }
        else {
            // local connect test
            (void)mp_DOLocalAlarm->SetLow();
        }
    }
    else {

        p_TestCase->SetParameter("AlarmFlag", QString::number(!AlarmFlag));

    }
#if 0  // this is for pre-alpha
    if (RetValue == 0) {
        Status.insert("AlarmStatus", Service::CMessageString::MSG_DIAGNOSTICS_STATUS_DISCONNECTED);
    }
    else {
        Status.insert("AlarmStatus", Service::CMessageString::MSG_DIAGNOSTICS_STATUS_CONNECTED);
    }
#else // this is for alpha
    if (RetValue == 0) {
        (void)Status.insert("AlarmStatus", Service::CMessageString::MSG_DIAGNOSTICS_STATUS_CONNECTED);
    }
    else {
        (void)Status.insert("AlarmStatus", Service::CMessageString::MSG_DIAGNOSTICS_STATUS_DISCONNECTED);
    }
#endif

    emit RefreshTestStatustoMain(TestCaseName, Status);
    return 0;
}

qint32 ManufacturingTestHandler::TestSystem110v220vSwitch()
{
    qint32 CurrentVoltage3 = 110;
    qint32 CurrentVoltage5 = 110;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_110V_220V_SWITCH);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    qint32 ConnectedVoltage = p_TestCase->GetParameter("ConnectedVoltage").toInt();

    quint8 ASB5SwitchType = mp_TempRetortBottom->GetHeaterSwitchType();
    quint8 ASB3SwitchType = mp_TempRV->GetHeaterSwitchType();



    if (ASB3SwitchType == TEMPCTRL_VOLTAGE_220V) {
        CurrentVoltage3 = 220;
    }
    else if (ASB3SwitchType == TEMPCTRL_VOLTAGE_110V) {
        CurrentVoltage3 = 110;
    }
    else {
        CurrentVoltage3 = 0;
    }

    if (ASB5SwitchType == TEMPCTRL_VOLTAGE_220V) {
        CurrentVoltage5 = 220;
    }
    else if (ASB5SwitchType == TEMPCTRL_VOLTAGE_110V) {
        CurrentVoltage5 = 110;
    }
    else {
        CurrentVoltage5 = 0;
    }

    p_TestCase->AddResult("CurrentVoltage3", QString::number(CurrentVoltage3));
    p_TestCase->AddResult("CurrentVoltage5", QString::number(CurrentVoltage5));

    if (CurrentVoltage3 == ConnectedVoltage && CurrentVoltage5 == ConnectedVoltage) {
        p_TestCase->SetStatus(true);
        return 0;
    }
    else {
        p_TestCase->SetStatus(false);
        return -1;
    }
}

qint32 ManufacturingTestHandler::TestSystemMainsRelay()
{
    bool Result = false;
    Service::ModuleTestStatus Status;
    float ASB3Current = 0.00;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_MAINS_RELAY);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    bool RelaySwitchStatus    = p_TestCase->GetParameter("RelaySwitchStatus").toInt();
    float SwitchOnCurrentLow  = p_TestCase->GetParameter("SwitchOnCurrentLow").toFloat();
    float SwitchOnCurrentHigh = p_TestCase->GetParameter("SwitchOnCurrentHigh").toFloat();
    float SWitchOffCurrentLow = p_TestCase->GetParameter("SwitchOffCurrentLow").toFloat();

    p_TestCase->AddResult("ASB3Current", QString("%1A").arg(ASB3Current));

    if (RelaySwitchStatus) {
         // switch on
        (void)mp_DigitalOutputMainRelay->SetHigh();
        (void)mp_TempRV->StartTemperatureControl(80);
        mp_Utils->Pause(2000);

        ASB3Current = mp_TempRV->GetCurrent();

        Result = (ASB3Current > SwitchOnCurrentLow && ASB3Current < SwitchOnCurrentHigh);
        if (!Result) {
            (void)mp_TempRV->StopTemperatureControl();
            (void)mp_DigitalOutputMainRelay->SetLow();
        }

        p_TestCase->AddResult("ASB3Current", QString("%1mA").arg(ASB3Current));
    }
    else {
         // switch off
        (void)mp_DigitalOutputMainRelay->SetLow();
        mp_Utils->Pause(2000);
        ASB3Current = mp_TempRV->GetCurrent();
        (void)mp_TempRV->StopTemperatureControl();

        Result = (ASB3Current < SWitchOffCurrentLow);
        p_TestCase->AddResult("ASB3Current", QString("%1mA").arg(ASB3Current));
    }
    p_TestCase->SetStatus(Result);
    if (Result) {
        (void)Status.insert("Result", Service::CMessageString::MSG_BUTTON_PASS);
    }
    else {
        (void)Status.insert("Result", Service::CMessageString::MSG_BUTTON_FAIL);
    }

    emit RefreshTestStatustoMain(TestCaseName, Status);

    if (Result) {
        return 0;
    }
    else {
        return -1;
    }
}

qint32 ManufacturingTestHandler::TestSystemExhaustFan()
{
    Service::ModuleTestStatus Status;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_EXHAUST_FAN);
    (void)mp_PressPump->SetFan(1);

    emit RefreshTestStatustoMain(TestCaseName, Status);
    return 0;
}

qint32 ManufacturingTestHandler::TestSystemOverflow()
{
    int ret = 0;
    int result(-1);
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_OVERFLOW);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QTime delayTime = QTime::fromString(p_TestCase->GetParameter("DelayTime"), "hh:mm:ss");
    int delay = delayTime.hour() * 60 * 60 + delayTime.minute() * 60 + delayTime.second();

    p_TestCase->ResetResult();

    EmitRefreshTestStatustoMain(TestCaseName, RV_INITIALIZING);
    if (mp_MotorRV->MoveToInitialPosition()!=RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_INIT_RV_FAILED);
        return -1;
    }

    EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, 1);

    if (mp_MotorRV->MoveToTubePosition(1)!=RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg("1"));
        return -1;
    }
    else {
        EmitRefreshTestStatustoMain(TestCaseName, RETORT_FILLING);

        ret = mp_PressPump->Sucking4Overflow(delay, 1);

        if (ret == SUCKING_RET_OVERFLOW) {
            result = 0;
        }
        else {
            result = -1;
        }
    }

    EmitRefreshTestStatustoMain(TestCaseName, RETORT_DRAINING);
    ret = mp_PressPump->Draining();
    if (ret != DRAINGING_RET_OK) {
        ;
    }

    return result;
}

qint32 ManufacturingTestHandler::TestSystemSealing(int CurStep)
{
    qint32 RetValue(0);
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_SEALING_TEST);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    p_TestCase->ResetResult();
    if (CurStep == 1 || CurStep == 2) {
        float TargetPressure(0);
        float Offset(3);
        if (CurStep == 1 ){
            TargetPressure = p_TestCase->GetParameter("SealTargetPressure1").toFloat();
            Offset = 3;
            if (p_TestCase->GetParameter("RVMove") == "1") {
                EmitRefreshTestStatustoMain(TestCaseName, RV_INITIALIZING);
                if (mp_MotorRV->MoveToInitialPosition()!=RV_MOVE_OK) {
                    EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
                    p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_INIT_RV_FAILED);
                    //return -1;  //20150205 by dixiong
                }

                EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_SEALING_POSITION, 1);
                if (mp_MotorRV->MoveToSealPosition(1)!=RV_MOVE_OK) {
                    EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
                    p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED.arg(1));
                    //return -1; //20150205 by dixiong
                }
                p_TestCase->SetParameter("RVMove", "0");
            }
        }
        else {
            TargetPressure = p_TestCase->GetParameter("SealTargetPressure2").toFloat();
            Offset = -3;
        }

        QTime Duration = QTime::fromString(p_TestCase->GetParameter("SealDuration"), "hh:mm:ss");
        QTime KeepDuration = QTime::fromString(p_TestCase->GetParameter("SealKeepDuration"), "hh:mm:ss");
        float Departure = p_TestCase->GetParameter("Departure").toFloat();

        EmitRefreshTestStatustoMain(TestCaseName, PUMP_CREATE_PRESSURE, TargetPressure);
        int WaitSec = Duration.hour()*60*60 + Duration.minute()*60 + Duration.second();
        (void)mp_PressPump->ReleasePressure();
        (void)mp_PressPump->SetFan(1);

        bool result = CreatePressure(WaitSec, TargetPressure, Offset, TestCaseName);
        if (result == false) {
            (void)mp_PressPump->StopCompressor();
            RetValue = -1;
        }
        else {
            (void)mp_PressPump->StopCompressor();
            qreal OrigPressure = mp_PressPump->GetPressure();
            qDebug()<<"OrigPressure="<<OrigPressure;

            WaitSec = KeepDuration.hour()*60*60 + KeepDuration.minute()*60 + KeepDuration.second();
            EmitRefreshTestStatustoMain(TestCaseName, PUMP_KEEP_PRESSURE, WaitSec);
            mp_Utils->Pause(WaitSec*1000);

            qreal CurrentPressure = mp_PressPump->GetPressure();
            qDebug()<<"CurrentPressure="<<CurrentPressure<<"  Departure="<<Departure;

            if (CurStep==1 && CurrentPressure<(TargetPressure-Offset-Departure)) {
                RetValue = -1;
            }
            else if (CurStep==2 && CurrentPressure>(TargetPressure-Offset+Departure)) {
                RetValue = -1;
            }
        }

        EmitRefreshTestStatustoMain(TestCaseName, PUMP_RELEASE_PRESSURE);
        (void)mp_PressPump->SetFan(0);
        (void)mp_PressPump->SetValve(0, 0);
        (void)mp_PressPump->SetValve(1, 0);
        mp_Utils->Pause(20*1000);
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        return RetValue ;
    }
    else if (CurStep == 3) {

        Service::ModuleTestStatus Status;
        float TargetPressure = p_TestCase->GetParameter("TubeTargetPressure").toFloat();
        QTime Duration = QTime::fromString(p_TestCase->GetParameter("TubeDuration"), "hh:mm:ss");
        QTime KeepDuration = QTime::fromString(p_TestCase->GetParameter("TubeKeepDuration"), "hh:mm:ss");
        float Departure = p_TestCase->GetParameter("Departure").toFloat();
        QString LabelStr;

        QList<int> PositionList;
        int Position(0);
        PositionList.clear();
        if (p_TestCase->GetParameter("TestMode") == "0") {
            for(int i=1; i<=16; i++) {
                PositionList.append(i);
            }
        }
        else {
            Position = p_TestCase->GetParameter("Position").toInt();
            PositionList.append(Position);
        }

        for(int i=0; i<PositionList.count(); i++) {
            if (m_UserAbort == true) {
                RetValue = 1;
                break;
            }

            Position = PositionList.at(i);

            LabelStr = Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE.arg(Position);
            Status.clear();
            (void)Status.insert("Label", LabelStr);
            emit RefreshTestStatustoMain(TestCaseName, Status);
            if (mp_MotorRV->MoveToTubePosition(Position)!=RV_MOVE_OK) {
                EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
                p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg(Position));

                LabelStr = Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg(Position);
                (void)Status.insert("Label", LabelStr);
                (void)Status.insert("Finish", "1");
                emit RefreshTestStatustoMain(TestCaseName, Status);
                return -1;
            }

            (void)mp_PressPump->SetFan(1);
            Status.clear();
            LabelStr = Service::CMessageString::MSG_DIAGNOSTICS_CREATING_PRESSURE.arg(TargetPressure);
            (void)Status.insert("Label", LabelStr);
            emit RefreshTestStatustoMain(TestCaseName, Status);
            int WaitSec = Duration.hour()*60*60 + Duration.minute()*60 + Duration.second();
            bool result = CreatePressure(WaitSec, TargetPressure, 3, TestCaseName);

            qreal OrigPressure = mp_PressPump->GetPressure();
            if (result == false) {
                Status.clear();
                (void)Status.insert("Position", QString("%1").arg(Position));
                (void)Status.insert("Pressure", QString("%1").arg(OrigPressure));
                (void)Status.insert("Result", Service::CMessageString::MSG_BUTTON_FAIL);
                RetValue = -1;
                if (m_UserAbort) {
                    RetValue = 1;
                    (void)mp_PressPump->StopCompressor();
                    break;
                }
                if (i == PositionList.count()-1) {
                    (void)mp_PressPump->StopCompressor();
                }
            }
            else {
                (void)mp_PressPump->StopCompressor();
                WaitSec = KeepDuration.hour()*60*60 + KeepDuration.minute()*60 + KeepDuration.second();
                LabelStr = Service::CMessageString::MSG_DIAGNOSTICS_KEEP_PRESSURE.arg(WaitSec);
                Status.clear();
                (void)Status.insert("Label", LabelStr);
                emit RefreshTestStatustoMain(TestCaseName, Status);

                while(WaitSec) {
                    if (m_UserAbort) {
                        RetValue = 1;
                        break;
                    }
                    mp_Utils->Pause(1000);
                    WaitSec--;
                }
                if (m_UserAbort) {
                    RetValue = 1;
                    break;
                }

                qreal CurrentPressure = mp_PressPump->GetPressure();
                Status.clear();
                (void)Status.insert("Position", QString("%1").arg(Position));
                (void)Status.insert("Pressure", QString("%1").arg(CurrentPressure));
                if (CurrentPressure<(OrigPressure-Departure)) {
                    (void)Status.insert("Result", Service::CMessageString::MSG_BUTTON_FAIL);
                    RetValue = -1;
                }
                else {
                    (void)Status.insert("Result", Service::CMessageString::MSG_BUTTON_PASS);
                }
            }

            emit RefreshTestStatustoMain(TestCaseName, Status);
        }

        mp_Utils->Pause(1000);

        LabelStr = Service::CMessageString::MSG_DIAGNOSTICS_RELEASING_PRESSURE;
        Status.clear();
        (void)Status.insert("Label", LabelStr);
        emit RefreshTestStatustoMain(TestCaseName, Status);
        (void)mp_PressPump->SetFan(0);
        (void)mp_PressPump->SetValve(0, 0);
        (void)mp_PressPump->SetValve(1, 0);
        if (m_UserAbort) {
            RetValue = 1;
            m_UserAbort = false;
            p_TestCase->AddResult("FailReason", "Abort");
            mp_Utils->Pause(10*1000);
        }
        else {
            mp_Utils->Pause(20*1000);
        }
        LabelStr = Service::CMessageString::MSG_DIAGNOSTICS_TEST_FINISH;
        (void)Status.insert("Label", LabelStr);
        (void)Status.insert("Finish", "1");
        emit RefreshTestStatustoMain(TestCaseName, Status);
        return RetValue;
    }
    else {
        return -1;
    }
}

qint32 ManufacturingTestHandler::CleaningSystem()
{
    qint32 RetValue(0);

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::CLEANING_SYSTEM_TEST);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    int TestStep = p_TestCase->GetParameter("TestStep").toInt();
    QTime time1 = QTime::fromString(p_TestCase->GetParameter(QString("Time1")), "hh:mm:ss");
    QTime time2 = QTime::fromString(p_TestCase->GetParameter(QString("Time2")), "hh:mm:ss");
    QTime time3 = QTime::fromString(p_TestCase->GetParameter(QString("Time3")), "hh:mm:ss");
    QTime blowTime = QTime::fromString(p_TestCase->GetParameter("BlowTime"), "hh:mm:ss");
    int waitSec1 = time1.hour()*60*60 + time1.minute()*60 + time1.second();
    int waitSec2 = time2.hour()*60*60 + time2.minute()*60 + time2.second();
    int waitSec3 = time3.hour()*60*60 + time3.minute()*60 + time3.second();
    int blowSec = blowTime.hour()*60*60 + blowTime.minute()*60 + blowTime.second();
    int targetPressure1 = p_TestCase->GetParameter(QString("TargetPressure1")).toInt();
    int targetPressure2 = p_TestCase->GetParameter(QString("TargetPressure2")).toInt();
    int departure = p_TestCase->GetParameter("Departure").toInt();

    p_TestCase->ResetResult();

    EmitRefreshTestStatustoMain(TestCaseName, RV_INITIALIZING);
    if (mp_MotorRV->MoveToInitialPosition() != RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_INIT_RV_FAILED);
        return -1;
    }

    if (TestStep == 1) {
        EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, 1);
        if (mp_MotorRV->MoveToTubePosition(1) != RV_MOVE_OK) {
            EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
            p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg("1"));
            return -1;
        }
    }
    else {
        EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_SEALING_POSITION, 1);
        if (mp_MotorRV->MoveToSealPosition(1) != RV_MOVE_OK) {
            EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
            p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED.arg("1"));
            return -1;
        }
    }

    if (TestStep == 1) {
        int i = 1;
        do {
            EmitRefreshTestStatustoMain(TestCaseName, PUMP_CREATE_PRESSURE, targetPressure1);
            (void)mp_PressPump->SetFan(1);
            (void)mp_PressPump->SetTargetPressure(9, targetPressure1);
            mp_Utils->Pause(waitSec1*1000);
            EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);

            (void)mp_PressPump->StopCompressor();

            EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_SEALING_POSITION, i);
            if (mp_MotorRV->MoveToSealPosition(i) != RV_MOVE_OK) {
                EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
                p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED.arg(i));
                RetValue = -1;
                goto CLEANING_EXIT;
            }

            EmitRefreshTestStatustoMain(TestCaseName, PUMP_RELEASE_PRESSURE);
            (void)mp_PressPump->SetFan(0);
            (void)mp_PressPump->SetValve(0, 0);
            (void)mp_PressPump->SetValve(1, 0);
            mp_Utils->Pause(20*1000);
            i++;
            EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, i);
            if (mp_MotorRV->MoveToTubePosition(i) != RV_MOVE_OK) {
                EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
                p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg(i));
                RetValue = -1;
                goto CLEANING_EXIT;
            }

            EmitRefreshTestStatustoMain(TestCaseName, PUMP_CREATE_PRESSURE, targetPressure2);
            (void)mp_PressPump->SetFan(1);
            (void)mp_PressPump->SetTargetPressure(1, targetPressure2);
            mp_Utils->Pause(waitSec2*1000);
            EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);

            EmitRefreshTestStatustoMain(TestCaseName, PUMP_RELEASE_PRESSURE);
            (void)mp_PressPump->StopCompressor();
            (void)mp_PressPump->SetFan(0);
            (void)mp_PressPump->SetValve(0, 0);
            (void)mp_PressPump->SetValve(1, 0);
            mp_Utils->Pause(20*1000);
            EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        } while (i < 16);

        return RetValue;
    }
    else {
        (void)mp_PressPump->SetFan(1);
        for (int i = 1; i <= 16; ++i) {
            EmitRefreshTestStatustoMain(TestCaseName, PUMP_CREATE_PRESSURE, targetPressure2);
            if (!CreatePressure(waitSec3, targetPressure2, departure, TestCaseName)) {
                EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
                p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_CREATE_PRESSURE_FALIED);
                RetValue = -1;
                goto CLEANING_EXIT;
            }

            EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, i);
            if (mp_MotorRV->MoveToTubePosition(i) != RV_MOVE_OK) {
                EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
                p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg(i));
                RetValue = -1;
                goto CLEANING_EXIT;
            }
            EmitRefreshTestStatustoMain(TestCaseName, SYSTEM_FLUSH);
            mp_Utils->Pause(blowSec*1000);

            //mp_PressPump->ReleasePressure();

            EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_SEALING_POSITION, i);
            if (mp_MotorRV->MoveToSealPosition(i) != RV_MOVE_OK) {
                EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
                p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED.arg(i));
                RetValue = -1;
                goto CLEANING_EXIT;
            }
        }

        EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, 2);
        (void)mp_MotorRV->MoveToTubePosition(2);
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
    }
CLEANING_EXIT:
    (void)mp_PressPump->ReleasePressure();

    return RetValue;
}

bool ManufacturingTestHandler:: CreatePressure(int waitSecond, float targetPressure, float departure, const QString& TestCaseName)
{
    bool result = false;

    if (targetPressure > 0) {
        qDebug()<<"CreatePressure : target = "<<targetPressure<<" currentPressure = "<<mp_PressPump->GetPressure();

        (void)mp_PressPump->SetTargetPressure(1, targetPressure);
    }
    else if (targetPressure < 0) {
        qDebug()<<"xxn CreatePressure : target = "<<targetPressure<<" currentPressure = "<<mp_PressPump->GetPressure();

        (void)mp_PressPump->SetTargetPressure(9, targetPressure);
    }

    float pressure = mp_PressPump->GetPressure();
    waitSecond += 1;
    while (waitSecond) {
        QTime EndTime = QTime().currentTime().addSecs(1);

        if (m_UserAbort) {
            result = false;
            break;
        }
        pressure = mp_PressPump->GetPressure();

        if (departure == 0) {
            if (targetPressure > 0 && pressure >= targetPressure) {
                result = true;
                break;
            }

            if (targetPressure < 0 && pressure <= targetPressure) {
                result = true;
                break;
            }

        }
        else if (targetPressure == 40) {
            if (pressure > (targetPressure-departure)) {
                result = true;
                break;
            }
        }
        else if (targetPressure == -40) {
            if (pressure < (targetPressure-departure)) {
                result = true;
                break;
            }
        }
        else if (pressure >= (targetPressure - qAbs(departure)) && pressure <= (targetPressure + qAbs(departure))) {
            qDebug()<<"Current pressure "<<pressure<<" target pressure: "<<targetPressure<<"  departure:"<<departure;
            result = true;
            break;
        }

        EmitRefreshTestStatustoMain(TestCaseName, PUMP_CURRENT_PRESSURE, pressure);

        --waitSecond;

        int MSec = QTime().currentTime().msecsTo(EndTime);
        if (MSec <= 0) {
            MSec = 1000;
        }
        mp_Utils->Pause(MSec);
    }
    if (result == true) {
        qDebug()<<"CreatePressure : currentPressure ================================== "<<pressure;
        EmitRefreshTestStatustoMain(TestCaseName, PUMP_CURRENT_PRESSURE, pressure);
        mp_Utils->Pause(1000);
    }

    return result;
}

qint32 ManufacturingTestHandler::TestMainControlASB(Service::ModuleTestCaseID_t Id)
{
    WrapperFmBaseModule *p_BaseModule = NULL;

    if (Id == Service::MAINCONTROL_ASB3) {
       p_BaseModule = mp_BaseModule3;
    }
    else if (Id == Service::MAINCONTROL_ASB5) {
       p_BaseModule = mp_BaseModule5;
    }
    else if (Id == Service::MAINCONTROL_ASB15) {
       p_BaseModule = mp_BaseModule15;
    }
    else {
       return -1;
    }

    qreal ActualVoltage = p_BaseModule->GetVoltage()/1000.0;
    quint16 ActualCurrent = p_BaseModule->GetCurrent();

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    p_TestCase->ResetResult();
    qreal Voltage = p_TestCase->GetParameter("Voltage").toDouble();
    qreal VoltageTolerance = p_TestCase->GetParameter("VolTolerance").toDouble();
    qreal VolLow = Voltage - Voltage*VoltageTolerance;
    qreal VolHigh = Voltage + Voltage*VoltageTolerance;
    qint16 CurrentLow = p_TestCase->GetParameter("CurrentLow").toInt();
    qint16 CurrentHigh = p_TestCase->GetParameter("CurrentHigh").toInt();

    p_TestCase->AddResult("Voltage", QString("%1").arg(ActualVoltage));
    p_TestCase->AddResult("Current", QString("%1").arg(ActualCurrent));

    if (ActualVoltage>=VolLow && ActualVoltage<=VolHigh) {
        p_TestCase->AddResult("VoltageRet", Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
    }
    else {
        p_TestCase->AddResult("VoltageRet", Service::CMessageString::MSG_DIAGNOSTICS_FAILED);
    }

    // for CurrentLow is 0. It's failed that ActualCurrent=0
    if (ActualCurrent>CurrentLow && ActualCurrent<= CurrentHigh) {
        p_TestCase->AddResult("CurrentRet", Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
    }
    else {
        p_TestCase->AddResult("CurrentRet", Service::CMessageString::MSG_DIAGNOSTICS_FAILED);
    }

    if (ActualVoltage>=VolLow && ActualVoltage<= VolHigh &&
            ActualCurrent>CurrentLow && ActualCurrent<= CurrentHigh) {
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
//    qreal MaxTargetTemp = TargetTemp + DepartureHigh;

    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    Service::ModuleTestStatus Status;

    WaitSec = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second()+60;

    SumSec = WaitSec;

    int num = 10;
    while(num) {
        CurrentTemp = p_TempCtrl->GetTemperature(0);
        if (CurrentTemp==-1) {
            mp_Utils->Pause(100);
            num--;
        }
        else {
            break;
        }
    }

    p_TestCase->ResetResult();
    if (CurrentTemp<AmbTempLow || CurrentTemp>AmbTempHigh) {
        QString FailureMsg = Service::CMessageString::MSG_DIAGNOSTICS_TUBE_TEMP_NO_MATCH.arg(CurrentTemp).arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Id, FailureMsg);
        p_TestCase->SetStatus(false);
        p_TestCase->AddResult("OutOfRange", "1");

        QString TargetTempStr = QString("%1~%2").arg(TargetTemp+DepartureLow).arg(TargetTemp+DepartureHigh);

        QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");

        CurrentValue = QString("%1").arg(CurrentTemp);

        p_TestCase->AddResult("Duration", Duration);
        p_TestCase->AddResult("TargetTemp", TargetTempStr);

        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTemp", CurrentValue);
        return -1;
    }

    (void)p_TempCtrl->StopTemperatureControl();
    (void)p_TempCtrl->StartTemperatureControl(TargetTemp);
    mp_Utils->Pause(500);

    SetFailReason(Id, "");
    while (!m_UserAbort && WaitSec)
    {
        QTime EndTime = QTime().currentTime().addSecs(1);

        CurrentTemp = p_TempCtrl->GetTemperature(0);

        CurrentValue = QString("%1").arg(CurrentTemp);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        (void)Status.insert("UsedTime", UsedTime);
        (void)Status.insert("CurrentTemp", CurrentValue);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1~%2").arg(TargetTemp+DepartureLow).arg(TargetTemp+DepartureHigh);
            (void)Status.insert("TargetTemp", TargetTempStr);

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            (void)Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        if ( CurrentTemp > MinTargetTemp)
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
            if (WaitSec <= 60) {
                break;
            }
            KeepSeconds = 0;
        }

        int MSec = QTime().currentTime().msecsTo(EndTime);
        if (MSec <= 0) {
            MSec = 1000;
        }
        mp_Utils->Pause(MSec);

        WaitSec--;
    }

    (void)p_TempCtrl->StopTemperatureControl();

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
    if(mp_MotorRV->MoveToInitialPosition()!=RV_MOVE_OK){
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

    quint32 Result(0);

    if (TubeFlag == 1) {
        Result = mp_MotorRV->MoveToTubePosition(Position);
    }
    else {
        Result = mp_MotorRV->MoveToSealPosition(Position);
    }
    if (Result != RV_MOVE_OK) {
        return -1;
    }

    return 0;
}

qint32 ManufacturingTestHandler::TestRVHeatingStation()
{
    qint32 WaitSec(0);
    quint32 SumSec(0);
    qint32  RVStatus(-1);
    qreal   CurrentTempSensor1(0), CurrentTempSensor2(0);
    QString Sensor1Value;
    QString Sensor2Value;
    QString UsedTime;
    qint32 UsedTimeSec(0);
    quint32 KeepSeconds(0);
    QTime StartTime;
    qint32 StartTimeSec(0);

    Service::ModuleTestCaseID Id = Service::ROTARY_VALVE_HEATING_STATION;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

//    QTime DurationTimeSensor1 = QTime::fromString(p_TestCase->GetParameter("Sensor1Duration"), "hh:mm:ss");
    QTime DurationTimeSensor2 = QTime::fromString(p_TestCase->GetParameter("Sensor2Duration"), "hh:mm:ss");

    qreal TargetTempSensor1 = p_TestCase->GetParameter("Sensor1TargetTemp").toDouble();
    //qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toDouble();
    //qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal TargetTempSensor2 = p_TestCase->GetParameter("Sensor2TargetTemp").toDouble();
    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    Service::ModuleTestStatus Status;

//    quint32 WaitSecSensor1 = DurationTimeSensor1.hour()*60*60 + DurationTimeSensor1.minute()*60 + DurationTimeSensor1.second();

    WaitSec = DurationTimeSensor2.hour()*60*60 + DurationTimeSensor2.minute()*60 + DurationTimeSensor2.second() + 60;

    SumSec = WaitSec;

    p_TestCase->ResetResult();


    int num=10;
    while(num) {
        CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
        CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

        if (CurrentTempSensor1==-1 || CurrentTempSensor2==-1 ||
                CurrentTempSensor1==0 || CurrentTempSensor2==0) {
            mp_Utils->Pause(100);
            num--;
        }
        else {
            break;
        }
    }

    if (CurrentTempSensor1<AmbTempLow || CurrentTempSensor1>AmbTempHigh ||
            CurrentTempSensor2<AmbTempLow || CurrentTempSensor2>AmbTempHigh  ){
        QString FailureMsg = Service::CMessageString::MSG_DIAGNOSTICS_RV_TEMP_NO_MATCH.arg(CurrentTempSensor1).arg(CurrentTempSensor2)
                .arg(AmbTempLow).arg(AmbTempHigh);
        p_TestCase->AddResult("OutOfRange", "1");
        SetFailReason(Id, FailureMsg);
        p_TestCase->SetStatus(false);

        QString TargetTempStr = QString(">=%1").arg(TargetTempSensor2);

        QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");

        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
        Sensor2Value = QString("%1").arg(CurrentTempSensor2);

        p_TestCase->AddResult("Duration", Duration);
        p_TestCase->AddResult("TargetTemp", TargetTempStr);

        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTempSensor1", Sensor1Value);
        p_TestCase->AddResult("CurrentTempSensor2", Sensor2Value);

        return -1;
    }

    (void)mp_DigitalOutputMainRelay->SetHigh();

    (void)mp_TempRV->StopTemperatureControl();
    (void)mp_TempRV->StartTemperatureControl(TargetTempSensor1);
    mp_Utils->Pause(500);

    StartTime = QTime::currentTime();
    StartTimeSec = StartTime.hour()*60*60+StartTime.minute()*60+StartTime.second();
    while (!m_UserAbort && UsedTimeSec <= WaitSec+1)
    {
        qDebug()<<"\n UsedTime: "<<UsedTimeSec;
        //QTime EndTime = QTime().currentTime().addSecs(1);
        QTime CurTime = QTime::currentTime();
        qint32 CurTimeSec = CurTime.hour()*60*60+CurTime.minute()*60+CurTime.second();
        UsedTimeSec = CurTimeSec - StartTimeSec + 1;
        qDebug()<<"\n StartTime: "<<StartTimeSec;
        qDebug()<<"\n CurTime: "<<CurTimeSec;
        if (UsedTimeSec < 0){
            UsedTimeSec = 24*60*60+60-StartTimeSec +CurTimeSec;
        }

        CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
        CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
        Sensor2Value = QString("%1").arg(CurrentTempSensor2);
        //UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");
        UsedTime = QTime().addSecs(UsedTimeSec).toString("hh:mm:ss");

        (void)Status.insert("UsedTime", UsedTime);
        (void)Status.insert("CurrentTempSensor1", Sensor1Value);
        (void)Status.insert("CurrentTempSensor2", Sensor2Value);
        //if (WaitSec == SumSec) {
        if(UsedTimeSec <= 1){
            QString TargetTempStr = QString(">=%1").arg(TargetTempSensor2);
            (void)Status.insert("TargetTemp", TargetTempStr);


            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            (void)Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);
        //WaitSec--;
        if ( CurrentTempSensor2 >= TargetTempSensor2 ) {
            // if more than target temp in 1 minute, then the test pass
            if (KeepSeconds > 120) {
                RVStatus = 1;
                break;
            }
            else {
                KeepSeconds++;
            }
        }
        else {
            KeepSeconds=0;
            //if (WaitSec <= 60) {
            if (UsedTimeSec > WaitSec - 60){
                break;
            }
        }

        mp_Utils->Pause(500);

//        int offset = 0;
//        if (WaitSec%10 == 0) {
//            //offset = 8;
//            offset = 4; //added by jack
//        }
//        int MSec = QTime().currentTime().msecsTo(EndTime)-offset;
//        mp_Utils->Pause(MSec);
    }

//    while (!m_UserAbort && WaitSec)
//    {
//        QTime EndTime = QTime().currentTime().addSecs(1);

//        CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
//        CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

//        qDebug()<<"1111111111   "<<CurrentTempSensor1<<"      "<<CurrentTempSensor2;


//#if 0
//        if ( (SumSec-WaitSec) >= WaitSecSensor1 ) {

//            // if less than target temp in duration time (1h), than exit from this process and report fail.
//            if (CurrentTempSensor1 < (TargetTempSensor1+DepartureLow)) {
//                break;
//            }

//            if ( CurrentTempSensor2 >= TargetTempSensor2 ) {
//                // if more than target temp in 1 minute, then the test pass
//                if (KeepSeconds > 60) {
//                    RVStatus = 1;
//                    break;
//                }
//                else {
//                    KeepSeconds++;
//                }
//            }
//            else {
//                KeepSeconds=0;
//            }
//        }
//#else
//        if ( CurrentTempSensor2 >= TargetTempSensor2 ) {
//            // if more than target temp in 1 minute, then the test pass
//            if (KeepSeconds > 60) {
//                RVStatus = 1;
//                break;
//            }
//            else {
//                KeepSeconds++;
//            }
//        }
//        else {
//            KeepSeconds=0;
//            if (WaitSec <= 60) {
//                break;
//            }
//        }
//#endif
//        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
//        Sensor2Value = QString("%1").arg(CurrentTempSensor2);
//        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

//        (void)Status.insert("UsedTime", UsedTime);
//        (void)Status.insert("CurrentTempSensor1", Sensor1Value);
//        (void)Status.insert("CurrentTempSensor2", Sensor2Value);
//        if (WaitSec == SumSec) {
//            QString TargetTempStr = QString(">=%1").arg(TargetTempSensor2);
//            (void)Status.insert("TargetTemp", TargetTempStr);


//            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
//            (void)Status.insert("Duration", Duration);
//            p_TestCase->AddResult("Duration", Duration);
//            p_TestCase->AddResult("TargetTemp", TargetTempStr);
//        }

    //        emit RefreshTestStatustoMain(TestCaseName, Status);
    //        WaitSec--;

    //        int offset = 0;
    //        if (WaitSec%10 == 0) {
    //            //offset = 8;
    //            offset = 4; //added by jack
    //        }
    //        int MSec = QTime().currentTime().msecsTo(EndTime)-offset;
    //        mp_Utils->Pause(MSec);
    //    }

    (void)mp_TempRV->StopTemperatureControl();
    (void)mp_DigitalOutputMainRelay->SetLow();

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
    qint32 WaitSec(0);
    quint32 WaitSecSensor1(0);
    qint32 SumSec(0);
    qint32  RVStatus(-1);
    qreal   CurrentTempSensor1(0), CurrentTempSensor2(0);
    QString Sensor1Value;
    QString Sensor2Value;
    QString UsedTime;
    quint8 Position(0);

    Service::ModuleTestCaseID Id = Service::ROTARY_VALVE_HEATING_END;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    QTime DurationTimeSensor1 = QTime::fromString(p_TestCase->GetParameter("Sensor1Duration"), "hh:mm:ss");
    QTime DurationTimeSensor2 = QTime::fromString(p_TestCase->GetParameter("Sensor2Duration"), "hh:mm:ss");
    QTime DepartureTime = QTime::fromString(p_TestCase->GetParameter("DepartureTime"), "hh:mm:ss");
    int DepartureTimeSec = DepartureTime.hour()*60*60 + DepartureTime.minute()*60 + DepartureTime.second();

    qreal TargetTempSensor1 = p_TestCase->GetParameter("Sensor1TargetTemp").toDouble();
//    qreal DepartureLow = p_TestCase->GetParameter("DepartureLow").toDouble();
//    qreal DepartureHigh = p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal TargetTempSensor2 = p_TestCase->GetParameter("Sensor2TargetTemp").toDouble();
    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

    int KeepSeconds = 0;

    Service::ModuleTestStatus Status;

    WaitSecSensor1 = DurationTimeSensor1.hour()*60*60 + DurationTimeSensor1.minute()*60 + DurationTimeSensor1.second();

    WaitSec = WaitSecSensor1;

    SumSec = WaitSec;

    p_TestCase->ResetResult();

    CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
    CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

    if (CurrentTempSensor1<AmbTempLow || CurrentTempSensor1>AmbTempHigh ||
            CurrentTempSensor2<AmbTempLow || CurrentTempSensor2>AmbTempHigh  ){
        QString FailureMsg = Service::CMessageString::MSG_DIAGNOSTICS_RV_TEMP_NO_MATCH.arg(CurrentTempSensor1).arg(CurrentTempSensor2)
                .arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Id, FailureMsg);
        p_TestCase->SetStatus(false);
        p_TestCase->AddResult("OutOfRange", "1");

        QString TargetTempStr = QString("%1").arg(TargetTempSensor1);

        QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");

        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
        Sensor2Value = QString("%1").arg(CurrentTempSensor2);

        p_TestCase->AddResult("Duration", Duration);
        p_TestCase->AddResult("TargetTemp", TargetTempStr);

        p_TestCase->AddResult("UsedTime", "00:00:00");
        p_TestCase->AddResult("CurrentTempSensor1", Sensor1Value);
        p_TestCase->AddResult("CurrentTempSensor2", Sensor2Value);

        return -1;
    }

    int Ret(0);
    // heating level sensor
    EmitRefreshTestStatustoMain(TestCaseName, LS_HEATING);
    Ret = HeatingLevelSensor();
    if (Ret == -1) {
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR_FAILED);
        p_TestCase->AddResult("FailStatus", "NOT-IN-HEATING");
        goto RV_HEATING_END_EXIT;
    }

    // set rotary valve to initial position.
    emit RefreshTestStatustoMain(TestCaseName, Status);
    EmitRefreshTestStatustoMain(TestCaseName, RV_INITIALIZING);
    if ( mp_MotorRV->MoveToInitialPosition()!=RV_MOVE_OK ) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_INIT_RV_FAILED);
        p_TestCase->AddResult("FailStatus", "NOT-IN-HEATING");
        goto RV_HEATING_END_EXIT;
    }
    // set rotary valve to tube positon

    Position =  p_TestCase->GetParameter("Position").toInt();
    EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, Position);

    if ( mp_MotorRV->MoveToTubePosition(Position)!=RV_MOVE_OK ) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg(Position));
        p_TestCase->AddResult("FailStatus", "NOT-IN-HEATING");
        goto RV_HEATING_END_EXIT;
    }

    // sucking
    EmitRefreshTestStatustoMain(TestCaseName, RETORT_FILLING);
    Ret = mp_PressPump->Sucking();
    (void)mp_TempLSensor->StopTemperatureControl();
    if ( Ret != SUCKING_RET_OK ) {
        EmitRefreshTestStatustoMain(TestCaseName, RETORT_DRAINING);
        Ret = mp_PressPump->Draining();

        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_FILLING_FAILED);
        p_TestCase->AddResult("FailStatus", "NOT-IN-HEATING");
        goto RV_HEATING_END_EXIT;
    }

    // set rotary valve to sealing position
    EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_SEALING_POSITION, Position);
    if (mp_MotorRV->MoveToSealPosition(Position)!=RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED.arg(Position));
        p_TestCase->AddResult("FailStatus", "NOT-IN-HEATING");
        goto RV_HEATING_END_EXIT;
    }

    (void)mp_TempRV->StopTemperatureControl();
    (void)mp_DigitalOutputMainRelay->SetHigh();
    (void)mp_TempRV->StartTemperatureControl(TargetTempSensor1);

    while (!m_UserAbort && WaitSec)
    {
        QTime EndTime = QTime().currentTime().addSecs(1);

        CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
        CurrentTempSensor2 = mp_TempRV->GetTemperature(1);


        Sensor1Value = QString("%1").arg(CurrentTempSensor1);
        Sensor2Value = QString("%1").arg(CurrentTempSensor2);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.clear();
        (void)Status.insert("UsedTime", UsedTime);
        (void)Status.insert("CurrentTempSensor1", Sensor1Value);
        (void)Status.insert("CurrentTempSensor2", Sensor2Value);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1").arg(TargetTempSensor1);
            (void)Status.insert("TargetTemp", TargetTempStr);

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            (void)Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTempStr);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);
        WaitSec--;

        int offset = 0;
        if (WaitSec%10 == 0) {
            offset = 8;
        }
        int MSec = QTime().currentTime().msecsTo(EndTime)-offset;
        if (MSec <= 0) {
            MSec = 1000;
        }
        mp_Utils->Pause(MSec);
    }

    // set rotary valve to tube position

    EmitRefreshTestStatustoMain(TestCaseName, RV_MOVE_TO_TUBE_POSITION, Position);
    if (mp_MotorRV->MoveToTubePosition(Position)!=RV_MOVE_OK) {
        EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);
        p_TestCase->AddResult("FailReason", Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED.arg(Position));
        goto RV_HEATING_END_EXIT;
    }

    // draining
    EmitRefreshTestStatustoMain(TestCaseName, RETORT_DRAINING);
    Ret = mp_PressPump->Draining();
     if ( Ret != DRAINGING_RET_OK || m_UserAbort ) {
    //     goto RV_HEATING_END_EXIT;
     }

     EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);

     p_TestCase->AddResult("CurStatus", "IN-HEATING");
      // heating at second stage.
     TargetTempSensor1 = p_TestCase->GetParameter("Sensor1TargetTemp2").toFloat();
     (void)mp_TempRV->StopTemperatureControl();
     (void)mp_TempRV->StartTemperatureControl(TargetTempSensor1);

     WaitSec = DurationTimeSensor2.hour()*60*60 + DurationTimeSensor2.minute()*60 + DurationTimeSensor2.second() + DepartureTimeSec;
     SumSec = WaitSec;
     while (!m_UserAbort && WaitSec) {
         QTime EndTime = QTime().currentTime().addSecs(1);

         CurrentTempSensor1 = mp_TempRV->GetTemperature(0);
         CurrentTempSensor2 = mp_TempRV->GetTemperature(1);

         if (CurrentTempSensor1 == -1 || CurrentTempSensor2 == -1 ) {
             mp_Utils->Pause(1000);
             WaitSec--;
             continue;
         }

         Sensor1Value = QString("%1").arg(CurrentTempSensor1);
         Sensor2Value = QString("%1").arg(CurrentTempSensor2);
         UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

         Status.clear();
         (void)Status.insert("UsedTime", UsedTime);
         (void)Status.insert("CurrentTempSensor1", Sensor1Value);
         (void)Status.insert("CurrentTempSensor2", Sensor2Value);
         if (WaitSec == SumSec) {
             QString TargetTempStr = QString(">=%1").arg(TargetTempSensor2);//.arg(DepartureLow).arg(DepartureHigh);
             (void)Status.insert("TargetTemp", TargetTempStr);

             QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
             (void)Status.insert("Duration", Duration);
             p_TestCase->AddResult("Duration", Duration);
             p_TestCase->AddResult("TargetTemp", TargetTempStr);
         }

         emit RefreshTestStatustoMain(TestCaseName, Status);

         if ( CurrentTempSensor2 >= TargetTempSensor2 ) {
             if ( KeepSeconds > DepartureTimeSec ) {
                 RVStatus = 1;
                 break;
             }
             else {
                 KeepSeconds++;
             }
         }
         else {
             KeepSeconds = 0;
             if (WaitSec <= DepartureTimeSec) {
                 break;
             }
         }

         int MSec = QTime().currentTime().msecsTo(EndTime);
         if (MSec <= 0) {
             MSec = 1000;
         }
         mp_Utils->Pause(MSec);

        WaitSec--;
    }

RV_HEATING_END_EXIT:

    EmitRefreshTestStatustoMain(TestCaseName, HIDE_MESSAGE);

    (void)mp_TempLSensor->StopTemperatureControl();

    (void)mp_TempRV->StopTemperatureControl();
    (void)mp_DigitalOutputMainRelay->SetLow();

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

void ManufacturingTestHandler::EmitFailMsgTimeout()
{
    if (m_BootFirmwareFinished == false) {

        Service::ModuleTestStatus Status;
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::FIRMWARE_UPDATE);
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

        QString Index = p_TestCase->GetParameter("Index");

        (void)Status.insert("Result", "false");
        (void)Status.insert("Index", Index);
        emit RefreshTestStatustoMain(TestCaseName, Status);
        m_BootFirmwareFinished = true;
    }
}

qint32 ManufacturingTestHandler::UpdateFirmware()
{
    Service::ModuleTestCaseID Id = Service::FIRMWARE_UPDATE;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    HimSlaveType_t SlaveType = (HimSlaveType_t) p_TestCase->GetParameter("SlaveType").toInt();
    QString BinPath = p_TestCase->GetParameter("Path");
    QString Index = p_TestCase->GetParameter("Index");

    CBaseModule *pBaseModule = m_rIdevProc.GetBaseModule(SlaveType);

    bool ret(false);
    bool RetValue(false);

    WrapperFmBootLoader* p_WrapperBootLoader(NULL);
    CBootLoader *pBootLoader(NULL);

    if (pBaseModule == NULL) {
        return false;
    }

    WrapperFmBaseModule *p_WrapperBaseModule(NULL);
    if (SlaveType == Slave_3) {
        p_WrapperBaseModule = mp_BaseModule3;
    }
    else if (SlaveType == Slave_5) {
        p_WrapperBaseModule = mp_BaseModule5;
    }
    else {
        p_WrapperBaseModule = mp_BaseModule15;
    }

    p_WrapperBaseModule->WaitForUpdate(true);
    ret = p_WrapperBaseModule->ResetNode();


    if (ret == false ){
        goto ERROR_EXIT;
    }

    mp_Utils->Pause(2000);

    pBootLoader = pBaseModule->GetBootLoader();

    if (pBootLoader == NULL) {
        goto ERROR_EXIT;
    }

    p_WrapperBootLoader = new WrapperFmBootLoader("asb_bootloader", pBootLoader, this);

    if (p_WrapperBootLoader == NULL) {
        goto ERROR_EXIT;
    }

    RetValue = WriteBoardOption(p_WrapperBootLoader, SlaveType);

    RetValue &= p_WrapperBootLoader->UpdateFirmware(BinPath);

    m_BootFirmwareFinished = false;
    if (RetValue == true) {
        mp_Utils->Pause(2000);

        RetValue = p_WrapperBootLoader->BootFirmware();

        if (SlaveType == Slave_3) {
            mp_Utils->Pause(10000);
        }
        else {
            mp_Utils->Pause(6000);
        }

        if (SlaveType == Slave_15) {
            QTimer timer;
            timer.setSingleShot(true);
            timer.setInterval(15000);
            (void) connect(&timer, SIGNAL(timeout()), this, SLOT(EmitFailMsgTimeout()));
            timer.start();

            RetValue = p_WrapperBaseModule->ReqFormatMemory();

            if (RetValue == false) {
                goto ERROR_EXIT;
            }
            else {
                mp_Utils->Pause(7000);
            }

            if ((mp_DORemoteAlarm && mp_DORemoteAlarm->SetHigh()) &&
                   mp_DOLocalAlarm && mp_DOLocalAlarm->SetHigh()) {
                if (AutoSetASB3HeaterSwitchType(Service::SYSTEM_SELF_TEST) == -1) {
                    goto ERROR_EXIT;
                }
            }
            else {
                goto ERROR_EXIT;
            }
        }

        Service::ModuleTestStatus Status;

        (void)Status.insert("Result", "true");
        (void)Status.insert("Index", Index);
        emit RefreshTestStatustoMain(TestCaseName, Status);
        delete p_WrapperBootLoader;

        m_BootFirmwareFinished = true;

        return 0;
    }
    else {

        RetValue = p_WrapperBootLoader->BootFirmware();

        if (SlaveType == Slave_3) {
            mp_Utils->Pause(10000);
        }
        else {
            mp_Utils->Pause(5000);
        }
    }
ERROR_EXIT:
    if (p_WrapperBootLoader) {
        delete p_WrapperBootLoader;
    }

    Service::ModuleTestStatus Status;
    (void)Status.insert("Result", "false");
    (void)Status.insert("Index", Index);
    emit RefreshTestStatustoMain(TestCaseName, Status);
    m_BootFirmwareFinished = true;
    return -1;
}

bool ManufacturingTestHandler::WriteBoardOption(WrapperFmBootLoader* p_Bootloader, HimSlaveType_t SlaveType)
{
    quint16 BASEMODULE_OPTIONS =
            OPTION_IDENTIFICATION  |        // Enable identification phase
            OPTION_NODE_INDEX_DIP  |        // Enable node index DIP switch
            OPTION_STATUS_LED      |        // Enable onboard status/error LEDs
            OPTION_LIFECYCLE_DATA  |        // Enable life cycle data collection
            OPTION_PROCESS_DATA    |        // Enable process data support
            OPTION_TRIGGER_EVENTS  |        // Enable trigger event support
            OPTION_TASK_STATISTICS |        // Enable statistics collection
            OPTION_VOLTAGE_MONITOR |        // Enable supply voltage monitor
            OPTION_CURRENT_MONITOR |        // Enable supply current monitor
            OPTION_INFO_MESSAGES;           // Enable info messages on startup

    QStringList BoardOption;
    QString str;

    BoardOption.clear();

    BoardOption.append(BASEMODULE_MODULE_ID);  // 0
    BoardOption.append("6");                   // 1
    (void)str.setNum(BASEMODULE_OPTIONS, 10);
    BoardOption.append(str);                    // 2
    BoardOption.append(DEFAULT_NODE_INDEX);     // 3


    if (SlaveType == Slave_3) {
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase("BoardOptionASB3");

        BoardOption.append(p_TestCase->GetParameter("VoltageLimitWarning")); //4
        BoardOption.append(p_TestCase->GetParameter("VoltageLimitFailure")); //5
        BoardOption.append(p_TestCase->GetParameter("CurrentLimitWarning")); //6
        BoardOption.append(p_TestCase->GetParameter("CurrentLimitFailure")); //7

        (void)str.setNum(MODULE_ID_TEMPERATURE, 10);
        BoardOption.append(str);                        //8
        BoardOption.append("1");                        //9

        (void)str.setNum(0x31012, 10);
        BoardOption.append(str);                        //10
    }
    else if (SlaveType == Slave_5) {
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase("BoardOptionASB5");

        BoardOption.append(p_TestCase->GetParameter("VoltageLimitWarning")); //4
        BoardOption.append(p_TestCase->GetParameter("VoltageLimitFailure")); //5
        BoardOption.append(p_TestCase->GetParameter("CurrentLimitWarning")); //6
        BoardOption.append(p_TestCase->GetParameter("CurrentLimitFailure")); //7

        (void)str.setNum(MODULE_ID_TEMPERATURE, 10);
        BoardOption.append(str);                        //8
        BoardOption.append("4");                        //9

        (void)str.setNum(0x11011, 10);
        BoardOption.append(str);                        //10

        (void)str.setNum(0xF11012, 10);
        BoardOption.append(str);                        //11

        (void)str.setNum(0x11012, 10);
        BoardOption.append(str);                        //12

        (void)str.setNum(0x11011, 10);
        BoardOption.append(str);                        //13
    }
    else if (SlaveType == Slave_15) {
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase("BoardOptionASB15");

        BoardOption.append(p_TestCase->GetParameter("VoltageLimitWarning")); //4
        BoardOption.append(p_TestCase->GetParameter("VoltageLimitFailure")); //5
        BoardOption.append(p_TestCase->GetParameter("CurrentLimitWarning")); //6
        BoardOption.append(p_TestCase->GetParameter("CurrentLimitFailure")); //7

        (void)str.setNum(MODULE_ID_TEMPERATURE, 10);
        BoardOption.append(str);                        //8
        BoardOption.append("3");                        //9

        (void)str.setNum(0x11041011, 10);
        BoardOption.append(str);                        //10

        (void)str.setNum(0x01041011, 10);
        BoardOption.append(str);                        //11

        (void)str.setNum(0x01041011, 10);
        BoardOption.append(str);                        //12

        (void)str.setNum(MODULE_ID_DIGITAL_OUT, 10);
        BoardOption.append(str);                        //13

        BoardOption.append("6");                        //14
        BoardOption.append("1");                        //15
        BoardOption.append("1");                        //16
        BoardOption.append("1");                        //17
        BoardOption.append("1");                        //18
        BoardOption.append("1");                        //19
        BoardOption.append("1");                        //20

        (void)str.setNum(MODULE_ID_PRESSURE, 10);
        BoardOption.append(str);                        //21

        BoardOption.append("1");                        //22

        (void)str.setNum(0x2111, 10);
        BoardOption.append(str);                        //23
    }

    return p_Bootloader->UpdateInfo(BoardOption, 2);
}

void ManufacturingTestHandler::GetSlaveInformation()
{
    Service::ModuleTestCaseID Id = Service::FIRMWARE_GET_SLAVE_INFO;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    HimSlaveType_t SlaveType = (HimSlaveType_t) p_TestCase->GetParameter("SlaveType").toInt();

    WrapperFmBaseModule *p_BaseModule = NULL;
    Service::ModuleTestStatus Status;


    if (SlaveType == Slave_3) {
        p_BaseModule = mp_BaseModule3;
        (void)Status.insert("SlaveType", "3");
    }
    else if (SlaveType == Slave_5) {
        p_BaseModule = mp_BaseModule5;
        (void)Status.insert("SlaveType", "5");
    }
    else if(SlaveType == Slave_15) {
        p_BaseModule = mp_BaseModule15;
        (void)Status.insert("SlaveType", "15");
    }

    QString Str = p_BaseModule->GetHWInfo();
    if (Str != "error" && Str != "request error") {
        QStringList HWInfo = Str.split("/");

        (void)Status.insert("HardwareMajorVersion", HWInfo[0]);
        (void)Status.insert("HardwareMinorVersion", HWInfo[1]);
        (void)Status.insert("HardwareProductionDate", HWInfo[2]);
    }
    else {
        (void)Status.insert("HardwareMajorVersion", "MAJOR_VER");
        (void)Status.insert("HardwareMinorVersion", "MINOR_VER");
        (void)Status.insert("HardwareProductionDate", "DATE");
    }

    Str = p_BaseModule->ReqSerialNumber();

    int i = 0;
    while (Str.isEmpty() && i < 2) {
        Str = p_BaseModule->ReqSerialNumber();
        ++i;
    }

    (void)Status.insert("SerialNumber", Str);

    Str = p_BaseModule->GetBootloaderInfo();
    if (Str != "error" && Str != "request error") {
        QStringList BootloaderInfo = Str.split("/");
        (void)Status.insert("BootLoaderMajorVersion", BootloaderInfo[0]);
        (void)Status.insert("BootLoaderMinorVersion", BootloaderInfo[1]);
        (void)Status.insert("BootLoaderReleaseDate", BootloaderInfo[2]);
    }
    else {
        (void)Status.insert("BootLoaderMajorVersion", "MAJOR_VER");
        (void)Status.insert("BootLoaderMinorVersion", "MINOR_VER");
        (void)Status.insert("BootLoaderReleaseDate", "DATE");
    }

    Str = p_BaseModule->GetSWInfo();
    if (Str != "error" && Str != "request error") {
        QStringList SWInfo = Str.split("/");
        (void)Status.insert("SoftwareVersion", SWInfo[0]);
        (void)Status.insert("SoftwareReleaseDate", SWInfo[1]);
    }
    else {
        (void)Status.insert("SoftwareVersion", "SW_VER");
        (void)Status.insert("SoftwareReleaseDate", "DATE");
    }

    Str = p_BaseModule->GetEndTestResultInfo();
    if (Str != "error" && Str != "request error") {
        QStringList EndTestInfo = Str.split("/");

        (void) Status.insert("EndTestResult", EndTestInfo[0]);
        (void) Status.insert("EndTestDate", EndTestInfo[1]);
    }
    else {
        (void) Status.insert("EndTestResult", "Open");
        (void) Status.insert("EndTestDate", "DATE");
    }

    qDebug()<<Status;

    emit RefreshTestStatustoMain(TestCaseName, Status);
}

void ManufacturingTestHandler::CalibratePressureSensor()
{
    Service::ModuleTestStatus Status;
    Service::ModuleTestCaseID Id = Service::PRESSURE_CALIBRATION;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);

    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    int FirstTime = p_TestCase->GetParameter("FirstTime").toInt();
    float PressureHigh = p_TestCase->GetParameter("PressureHigh").toFloat();
    float PressureLow  = p_TestCase->GetParameter("PressureLow").toFloat();

    if (FirstTime == 1) {
        p_TestCase->SetParameter("FirstTime", "0");
        (void)mp_PressPump->WritePressureDrift(0.0);
    }

    (void)mp_PressPump->SetValve(0, 0);
    (void)mp_PressPump->SetValve(1, 0);
    mp_Utils->Pause(20*1000);

    float Pressure = mp_PressPump->GetPressure();

    float Drift(0);


    if (qAbs(Pressure) > PressureHigh) {
        (void)Status.insert("Result", "2"); // open retort lid and retry
    }
    else if (qAbs(Pressure) < PressureLow) {
        (void)Status.insert("Result", "0"); // success
    }
    else {
        (void)Status.insert("Result", "1"); // Set drift and retry
        Drift =  mp_PressPump->ReadPressureDrift();

        (void)mp_PressPump->WritePressureDrift(Drift+Pressure);
    }
//    Drift =  mp_PressPump->ReadPressureDrift();
//    qDebug()<<"Drift="<<Drift << " Pressure="<<Pressure;

    emit RefreshTestStatustoMain(TestCaseName, Status);
}

void ManufacturingTestHandler::SetSlaveStandby()
{
    (void)mp_BaseModule3->SetNodeState(NODE_STATE_SHUTDOWN);
    (void)mp_BaseModule5->SetNodeState(NODE_STATE_SHUTDOWN);
    (void)mp_BaseModule15->SetNodeState(NODE_STATE_SHUTDOWN);

    mp_Utils->Pause(2000);
}

qint32 ManufacturingTestHandler::HeatingLevelSensor()
{
    int LSENSOR_TEMP_WATER = 95;
    int LSENSOR_SLOPE_TEMPCHANGE_NORMAL = 10;
    int TEST_LSENSOR_TIMEOUT = 120;        //Sec.
    int TEST_LSENSOR_TEMP_TOLERANCE = 2;

    int LSENSOR_PID_MAXTEMP_NORMAL = 112;  //Degrees
    int LSENSOR_PID_KC_NORMAL = 120;
    int LSENSOR_PID_TI_NORMAL = 1212;
    int LSENSOR_PID_TD_NORMAL = 80;

    int LSENSOR_PID_MAXTEMP_SLOW = 112;  //Degrees
    int LSENSOR_PID_KC_SLOW = 200;
    int LSENSOR_PID_TI_SLOW = 1000;
    int LSENSOR_PID_TD_SLOW = 0;

    int ExchangeTemp = 90;
    int TargetTemp = LSENSOR_TEMP_WATER;

    int LSensorTempChange = LSENSOR_SLOPE_TEMPCHANGE_NORMAL;   // Slope temp

    (void)mp_TempLSensor->StopTemperatureControl();
    (void)mp_TempLSensor->SetTemperaturePid(LSENSOR_PID_MAXTEMP_NORMAL, LSENSOR_PID_KC_NORMAL,
                                      LSENSOR_PID_TI_NORMAL, LSENSOR_PID_TD_NORMAL);
    (void)mp_TempLSensor->StartTemperatureControl(TargetTemp, LSensorTempChange);

    int WaitSeconds = TEST_LSENSOR_TIMEOUT;
    int ReadyStatus(-1);
    while(WaitSeconds){
        if (mp_TempLSensor->GetTemperature() > ExchangeTemp) {
            ReadyStatus = 1;
            break;
        }
        mp_Utils->Pause(1000);
        WaitSeconds--;
    }
    if (ReadyStatus == -1) {
        (void)mp_TempLSensor->StopTemperatureControl();
        return -1;
    }

    (void)mp_TempLSensor->StopTemperatureControl();
    (void)mp_TempLSensor->SetTemperaturePid(LSENSOR_PID_MAXTEMP_SLOW, LSENSOR_PID_KC_SLOW,
                                      LSENSOR_PID_TI_SLOW, LSENSOR_PID_TD_SLOW);
    (void)mp_TempLSensor->StartTemperatureControl(TargetTemp, LSensorTempChange);

    mp_Utils->Pause(1000);

    while(WaitSeconds){
        if (mp_TempLSensor->GetTemperature() > (TargetTemp-TEST_LSENSOR_TEMP_TOLERANCE)) {
            ReadyStatus = 1;
            break;
        }
        mp_Utils->Pause(1000);
        WaitSeconds--;
    }
    if (ReadyStatus == -1) {
        (void)mp_TempLSensor->StopTemperatureControl();
        return -1;
    }

    return 1;

}

qint32 ManufacturingTestHandler::AutoSetASB3HeaterSwitchType(Service::ModuleTestCaseID Id)
{
    qint32 RetVal(0);

    if (!mp_TempRV || !mp_TempRetortBottom || !mp_TempOvenBottom || !mp_DigitalOutputMainRelay || !mp_Utils) {
        return -1;
    }

    (void)mp_TempRV->SetTemperatureSwitchState(-1, 1);

    (void)mp_TempRetortBottom->SetTemperatureSwitchState(-1, 1);

    (void)mp_DigitalOutputMainRelay->SetHigh();

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    qreal RVTargetTemp = p_TestCase->GetParameter("RVTargetTemp").toInt();
    qreal RetortTargetTemp = p_TestCase->GetParameter("RetortTargetTemp").toInt();

    (void)mp_TempRV->StartTemperatureControl(RVTargetTemp);
    (void)mp_TempRetortBottom->StartTemperatureControl(RetortTargetTemp);

    int i=0;
    while(i<10)
    {
        i++;
        mp_Utils->Pause(500);
    }

    (void)mp_TempRV->StopTemperatureControl();
    (void)mp_TempRetortBottom->StopTemperatureControl();

    (void)mp_DigitalOutputMainRelay->SetLow();
#if 0 // Disabled by Sunny on Oct, 31, 2014.
    quint8 ASB3SwitchType = mp_TempRV->GetHeaterSwitchType();
    quint8 ASB5SwitchType = mp_TempRetortBottom->GetHeaterSwitchType();

    if (ASB3SwitchType != ASB5SwitchType && ASB5SwitchType != 0) {
        (void)mp_TempRV->SetTemperatureSwitchState(ASB5SwitchType, -1);

        ASB3SwitchType = mp_TempRV->GetHeaterSwitchType();

        if (ASB3SwitchType != ASB5SwitchType) {
            RetVal = -1;
        }
    }
#endif
    (void)mp_TempRV->SetTemperatureSwitchState(-1, 0);
    (void)mp_TempRetortBottom->SetTemperatureSwitchState(-1, 0);

    return RetVal;
}

qint32 ManufacturingTestHandler::SystemSelfTest()
{
    Service::ModuleTestCaseID Id = Service::SYSTEM_SELF_TEST;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);

    Service::ModuleTestStatus Status;

    (void)Status.insert("CurState", "Begin");
    emit RefreshTestStatustoMain(TestCaseName, Status);
    // close remote alarm and local alarm.
    if (mp_DORemoteAlarm) {
        (void)mp_DORemoteAlarm->SetHigh();
    }

    if (mp_DOLocalAlarm) {
        (void)mp_DOLocalAlarm->SetHigh();
    }

    qint32 RetVal = AutoSetASB3HeaterSwitchType(Id);

    (void)Status.insert("CurState", "End");
    emit RefreshTestStatustoMain(TestCaseName, Status);

    // Get slave information.
    Id = Service::FIRMWARE_GET_SLAVE_INFO;
    TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    p_TestCase->SetParameter("TestType", "Init");
    p_TestCase->SetParameter("SlaveType", "3");
    GetSlaveInformation();
    p_TestCase->SetParameter("SlaveType", "5");
    GetSlaveInformation();
    p_TestCase->SetParameter("SlaveType", "15");
    GetSlaveInformation();

    return RetVal;
}

qint32 ManufacturingTestHandler::ResetOperationTime()
{
    Service::ModuleTestCaseID Id = Service::RESET_OPERATION_TIME;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    QString Module = p_TestCase->GetParameter("Module");
    QString SubModule = p_TestCase->GetParameter("SubModule").remove(" ");

    bool Ret = true;

    if (Module == "Main Control") {
        if (SubModule == "ASB3") {
            Ret = mp_BaseModule3->ReqDataReset();
            Ret &= ResetOperationTime2Ref("RotaryValveDevice", "asb3_0", "OperationTime");
        }
        else if (SubModule == "ASB5") {
            Ret = mp_BaseModule5->ReqDataReset();
            Ret &= ResetOperationTime2Ref("OvenDevice", "asb5_0", "OperationTime");
            Ret &= ResetOperationTime2Ref("RetortDevice", "asb5_0", "OperationTime");
        }
        else if (SubModule == "ASB15") {
            Ret = mp_BaseModule15->ReqDataReset();
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "asb15_0", "OperationTime");
        }
        else {
            QString ParamName;
            QString ParamNameRef;

            if (SubModule == "EBox") {
                ParamName = "EBox_LifeTime";
                ParamNameRef = "EBox";
            }
            else if (SubModule == "TouchScreen") {
                ParamName = "Touch_Screen_LifeTime";
                ParamNameRef = "Touch_Screen";
            }
            else if (SubModule == "VentilationFan") {
                ParamName = "Ventilation_Fan_LifeTime";
                ParamNameRef = "Ventilation_Fan";
            }

            Ret = ResetOperationTime2Ebox("OtherDevice", SubModule, ParamName);
            Ret &= ResetOperationTime2Ref("OtherDevice", ParamNameRef, "OperationTime");
        }
    }
    else if (Module =="Retort") {
        if (SubModule == "Heater") {
            Ret = mp_TempRetortSide->ResetHeaterOperatingTime(0);
            Ret &= mp_TempRetortBottom->ResetHeaterOperatingTime(0);
            Ret &= mp_TempRetortBottom->ResetHeaterOperatingTime(1);
            Ret &= ResetOperationTime2Ebox("RetortDevice", "temp_retort_bottom", "History_OperationTime");
            Ret &= ResetOperationTime2Ref("RetortDevice", "temp_retort_bottom", "OperationTime");
        }
        else if (SubModule == "LidLock") {
            Ret = ResetOperationTime2Ebox("RetortDevice", "Retort_Lid_Lock", "lid_status_LifeCycle");
            Ret &= ResetOperationTime2Ref("RetortDevice", "lid_status", "OperationTime");
        }
        else if (SubModule == "LevelSensor") {
            Ret = ResetOperationTime2Ebox("LA", "AL_level_sensor_temp_ctrl", "temp_lsensor_LifeCycle");
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "temp_lsensor", "OperationCycles");
        }
    }
    else if (Module =="Paraffin Oven"){
        if (SubModule == "Heater") {
            Ret = mp_TempOvenTop->ResetHeaterOperatingTime(0);
            Ret &= mp_TempOvenBottom->ResetHeaterOperatingTime(0);
            Ret &= mp_TempOvenBottom->ResetHeaterOperatingTime(1);

            Ret &= ResetOperationTime2Ebox("OvenDevice", "temp_oven_bottom", "History_OperationTime");
            Ret &= ResetOperationTime2Ebox("OvenDevice", "temp_oven_top", "History_OperationTime");
            Ret &= ResetOperationTime2Ref("OvenDevice", "temp_oven_top", "OperationTime");
            Ret &= ResetOperationTime2Ref("OvenDevice", "temp_oven_bottom", "OperationTime");
        }
        else if (SubModule == "CoverSensor") {
            Ret = ResetOperationTime2Ebox("OvenDevice", "Oven_Cover_Sensor", "oven_door_status_LifeCycle");
            Ret &= ResetOperationTime2Ref("OvenDevice", "oven_door_status", "OperationTime");
        }
    }
    else if (Module == "Rotary Valve"){
        if (SubModule == "Heater") {
            Ret = mp_TempRV->ResetHeaterOperatingTime(0);
            Ret &= ResetOperationTime2Ebox("RotaryValveDevice", "temp_rv", "History_OperationTime");
            Ret &= ResetOperationTime2Ref("RotaryValveDevice", "temp_rv", "OperationTime");
        }
        else if (SubModule == "Motor") {
            Ret = mp_MotorRV->ReqDataReset();
            Ret &= ResetOperationTime2Ebox("RotaryValveDevice", "motor_rv", "History_OperationTime");
            Ret &= ResetOperationTime2Ref("RotaryValveDevice", "motor_rv", "OperationTime");
        }
    }
    else if (Module == "L&A System") {
        if (SubModule == "Pump") {
            Ret = mp_PressPump->ResetPumpOperatingTime(0);
            Ret &= ResetOperationTime2Ebox("LA", "AL_pressure_ctrl", "History_Pump_OperationTime");
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "pressurectrl", "PumpOperationTime");
        }
        else if (SubModule == "Valve1") {
            Ret = ResetOperationTime2Ebox("LA", "AL_pressure_ctrl", "Valve1_LifeCycle");
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "pressurectrl", "Valve1OperationCycle");
        }
        else if (SubModule == "Valve2") {
            Ret = ResetOperationTime2Ebox("LA", "AL_pressure_ctrl", "Valve2_LifeCycle");
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "pressurectrl", "Valve2OperationCycle");
        }
        else if (SubModule == "AirHeatingTube") {
            Ret = mp_TempTubeAir->ResetHeaterOperatingTime(0);
            Ret &= ResetOperationTime2Ebox("LA", "temp_tube2", "History_OperationTime");
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "temp_tube2", "OperationTime");
        }
        else if (SubModule == "LiquidHeatingTube") {
            Ret = mp_TempTubeLiquid->ResetHeaterOperatingTime(0);
            Ret &= ResetOperationTime2Ebox("LA", "temp_tube1", "History_OperationTime");
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "temp_tube1", "OperationTime");
        }
        else if (SubModule == "PressureSensor") {
            Ret = ResetOperationTime2Ebox("OtherDevice", "PressureSensor", "Pressure_Sensor_LifeTime");
            Ret &= ResetOperationTime2Ref("OtherDevice", "Pressure_Sensor", "OperationTime");
        }
        else if (SubModule == "CarbonFilter") {
            // TBD
            Ret = ResetOperationTime2Ebox("LA", "AL_pressure_ctrl", "ActiveCarbonFilter_LifeTime");
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "pressurectrl", "ActiveCarbonFilterLifeTime");
        }
        else if (SubModule == "ExhaustFan") {
            // TBD
            Ret = ResetOperationTime2Ebox("LA", "AL_pressure_ctrl", "Exhaust_Fan_LifeTime");
            Ret &= ResetOperationTime2Ref("AirLiquidDevice", "pressurectrl", "ExhaustFanLifeTime");
        }
    }
    p_TestCase->AddResult("Result", "1");
    if (Ret) {
        return 0;
    }

    return -1;
}

bool ManufacturingTestHandler::ResetOperationTime2Ebox(const QString &ModuleName, const QString &SubModuleName, const QString &ParamName)
{
    DeviceLifeCycleRecord *p_DeviceRecord = new DeviceLifeCycleRecord();
    if (p_DeviceRecord == NULL) {
        return false;
    }


    if(!p_DeviceRecord->ReadRecord()){
        delete p_DeviceRecord;
        return false;
    }

    ModuleLifeCycleRecord* p_ModuleRecord = p_DeviceRecord->m_ModuleLifeCycleMap.value(ModuleName);

    if (p_ModuleRecord == NULL) {
        delete p_DeviceRecord;
        return false;
    }

    PartLifeCycleRecord* p_PartRecord = p_ModuleRecord->m_PartLifeCycleMap.value(SubModuleName);
    if (p_PartRecord == NULL) {
        delete p_DeviceRecord;
        return false;
    }

    p_PartRecord->m_ParamMap[ParamName] = "0";
    if(!p_DeviceRecord->WriteRecord()){
        delete p_DeviceRecord;
        return false;
    }
    delete p_DeviceRecord;
    return true;
}

bool ManufacturingTestHandler::ResetOperationTime2Ref(const QString& ModuleName, const QString& SubModuleName, const QString& ParamName)
{
    QString FilenameRef = Global::SystemPaths::Instance().GetSettingsPath() + "/InstrumentHistoryRef.xml";
    DataManager::CInstrumentHistory InstrumentHistoryRef;
    if (!InstrumentHistoryRef.ReadFile(FilenameRef)) {
        qDebug()<<"ManufacturingTestHandler::ResetOperationTime2Ref::read ref file failed.";
        return false;
    }

    DataManager::CModuleDataList* ModuleListRef = InstrumentHistoryRef.GetModuleList(0);

    if (!ModuleListRef) {
        qDebug()<<"ManufacturingTestHandler::ResetOperationTime2Ref failed: invalid MoudlistRef.";
        return false;
    }

    DataManager::CModule* p_ModuleRef = ModuleListRef->GetModule(ModuleName);

    if (p_ModuleRef) {
        DataManager::CSubModule* p_SubModuleRef = p_ModuleRef->GetSubModuleInfo(SubModuleName);
        if (p_SubModuleRef) {
            DataManager::Parameter_t* p_Param = p_SubModuleRef->GetParameterInfo(ParamName);
            if (p_Param) {
                p_Param->ParameterValue = "0";
            }
        }
        //p_ModuleRef->GetSubModuleInfo(SubModuleName)->GetParameterInfo(ParamName)->ParameterValue = "0";
    }

    if (!InstrumentHistoryRef.Write()) {
        qDebug()<<"ManufacturingTestHandler::ResetOperationTime2Ref MoudleListRef write failed.";
        return false;
    }

    return true;
}

void ManufacturingTestHandler::SetFailReason(Service::ModuleTestCaseID Id, const QString &FailMsg)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    p_TestCase->AddResult("FailReason", FailMsg);
    p_TestCase->SetStatus(false);
}

void ManufacturingTestHandler::EmitRefreshTestStatustoMain(const QString& TestCaseName, TestCurStatus_t CurStatus, qreal Param)
{
    Service::ModuleTestStatus Status;
    QString Msg;
    Status.clear();
    switch (CurStatus) {
    case RV_INITIALIZING:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_INIT_RV;
        break;
    case RV_MOVE_TO_TUBE_POSITION:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE.arg(Param);
        break;
    case RV_MOVE_TO_SEALING_POSITION:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALINT.arg(Param);
        break;
    case LS_HEATING:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR;
        break;
    case RETORT_FILLING:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_FILLING;
        break;
    case RETORT_DRAINING:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_DRAINING;
        break;
    case PUMP_CREATE_PRESSURE:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_CREATING_PRESSURE.arg(Param);
        break;
    case PUMP_KEEP_PRESSURE:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_KEEP_PRESSURE.arg(Param);
        break;
    case PUMP_RELEASE_PRESSURE:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_RELEASING_PRESSURE;
        break;
    case PUMP_CURRENT_PRESSURE:
        (void)Status.insert("CurrentPressure", "1");
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_CURRENT_PRESSURE.arg(Param);
        break;
    case SYSTEM_FLUSH:
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_AIR_BLOWING;
        break;
    case WAIT_CONFIRM:
        Msg = "WaitConfirm";
        break;
    case WAIT_CONFIRM2:
        Msg = "WaitConfirm2";
        break;
    case HIDE_MESSAGE:
        Msg = "HideMessage";
        break;
    case INFORM_DONE:
        Msg = "InformDone";
        break;
    default:
        break;
    }

    (void)Status.insert("CurrentStatus", Msg);
    emit RefreshTestStatustoMain(TestCaseName, Status);
    if (CurStatus != PUMP_CURRENT_PRESSURE) {
        mp_Utils->Pause(1000);
    }
}

void ManufacturingTestHandler::PerformModuleManufacturingTest(Service::ModuleTestCaseID_t TestId, Service::ModuleTestCaseID_t AbortTestCaseId)
{
    if(!IsInitialized()){
        Initialize();
    }

    m_UserAbort = false;
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
        (void)TestOvenCoverSensor();
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
        (void)TestLidLocker();
        break;
    case Service::RETORT_LEVEL_SENSOR_HEATING:
        if (NULL == mp_TempLSensor) {
            SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
            emit ReturnManufacturingTestMsg(false);
            return;
        }
        else if (0 == TestRetortLevelSensorHeating()) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::RETORT_LEVEL_SENSOR_DETECTING:
    {
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(TestId);
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        if (1 == p_TestCase->GetParameter("CurStep").toInt()) {
            if (NULL == mp_TempLSensor) {
                SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
                emit ReturnManufacturingTestMsg(false);
                return ;
            }
            else if (0 == TestRetortLevelSensorDetecting()) {
                return ;
            }
            else {
                emit ReturnManufacturingTestMsg(false);
            }
        }
        else {
            m_Continue = true;
            return ;
        }
        break;
    }
    case Service::RETORT_HEATING_WITH_WATER:
    {
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(TestId);
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        int CurStep = p_TestCase->GetParameter("CurStep").toInt();
        if (CurStep == 1) {
            if (NULL == mp_TempRetortSide && NULL == mp_TempRetortBottom) {
                SetFailReason(TestId, Service::MSG_DEVICE_NOT_INITIALIZED);
                emit ReturnManufacturingTestMsg(false);
                return;
            }
            if (0 == TestRetortHeatingWater()) {
                return;
            }
            else {
                emit ReturnManufacturingTestMsg(false);
            }
        }
        else if (CurStep == 2 || CurStep == 3) {
            m_Continue = true;
            return;
        }
        break;
    }
    case Service::RETORT_HEATING_EMPTY:
        if (NULL == mp_TempRetortSide && NULL == mp_TempRetortBottom) {
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
        (void)TestSystemSpeaker();
        break;
    case Service::SYSTEM_110V_220V_SWITCH:
        if ( 0 == TestSystem110v220vSwitch()) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::SYSTEM_REMOTE_LOCAL_ALARM:
        (void)TestSystemAlarm();
        break;
    case Service::SYSTEM_MAINS_RELAY:
        if ( 0 == TestSystemMainsRelay() ) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::SYSTEM_EXHAUST_FAN:
        (void)TestSystemExhaustFan();
        break;
    case Service::SYSTEM_OVERFLOW:
        if ( 0 == TestSystemOverflow() ) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::SYSTEM_SEALING_TEST:
    {
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(TestId);
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        int CurStep = p_TestCase->GetParameter("CurStep").toInt();
        if ( 0 == TestSystemSealing(CurStep) ) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    }
    case Service::CLEANING_SYSTEM_TEST:
        if (0 == CleaningSystem()) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    case Service::FIRMWARE_UPDATE:
        (void)UpdateFirmware();
        break;
    case Service::FIRMWARE_GET_SLAVE_INFO:
        GetSlaveInformation();
        break;
    case Service::PRESSURE_CALIBRATION:
        CalibratePressureSensor();
        break;
    case Service::SYSTEM_SHUTDOWN:
        SetSlaveStandby();
        break;
    case Service::SYSTEM_110V_220V_AUTO_SWITCH:
        (void)AutoSetASB3HeaterSwitchType(Service::SYSTEM_SELF_TEST);
        break;
    case Service::SYSTEM_SELF_TEST:
        (void)SystemSelfTest();
        break;
    case Service::RESET_OPERATION_TIME:
        if (0 == ResetOperationTime()) {
            emit ReturnManufacturingTestMsg(true);
        }
        else {
            emit ReturnManufacturingTestMsg(false);
        }
        break;
    default:
        break;
    }
}

} // end namespace DeviceControl
