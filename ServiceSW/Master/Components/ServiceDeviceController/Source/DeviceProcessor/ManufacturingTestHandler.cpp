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
    mp_TempTubeLiquid = NULL;
    mp_TempTubeAir = NULL;
    mp_MotorRV = NULL;
    mp_PressPump = NULL;

    mp_BaseModule3 = NULL;
    mp_BaseModule5 = NULL;
    mp_BaseModule15 = NULL;
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

    CDigitalInput *pDigitalInput = NULL;
    pDigitalInput = static_cast<CDigitalInput *>(m_rIdevProc.GetFunctionModuleRef(DEVICE_INSTANCE_ID_OVEN, CANObjectKeyLUT::m_OvenLidDIKey));
    if ( NULL != pDigitalInput ) {
        mp_DigitalInpputOven = new WrapperFmDigitalInput("digitalinput_oven", pDigitalInput, this);
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
void ManufacturingTestHandler::OnAbortTest(Global::tRefType Ref, quint32 id)
{
    qDebug()<<"ManufacturingTestHandler::OnAbortTest";
    if(!IsInitialized()){
        Initialize();
    }

    if(NULL == mp_Utils)
    {
        emit ReturnErrorMessagetoMain(Service::MSG_DEVICE_NOT_INITIALIZED);
        return;
    }

    mp_Utils->AbortPause();

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
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
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

        QString TargetTempStr = QString("%1~%2").arg(TargetTemp).arg(MaxTargetTemp);

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
            QString TargetTempStr = QString("%1~%2").arg(TargetTemp).arg(MaxTargetTemp);
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
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal MaxTargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureHigh").toDouble();
    qreal TopTargetTemp = p_TestCase->GetParameter("TopTargetTemp").toDouble();
    qreal BottomTargetTemp = p_TestCase->GetParameter("BottomTargetTemp").toDouble();
    qreal AmbTempLow = p_TestCase->GetParameter("AmbTempLow").toDouble();
    qreal AmbTempHigh = p_TestCase->GetParameter("AmbTempHigh").toDouble();

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

        QString TargetTempStr = QString("%1~%2").arg(TargetTemp).arg(MaxTargetTemp);

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

        if ( CurrentTempTop >= TargetTemp &&
             CurrentTempBottom1 >= TargetTemp &&
             CurrentTempBottom2 >= TargetTemp )
        {
            OvenStatus = 1;
            break;
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
            QString TargetTempStr = QString("%1~%2").arg(TargetTemp).arg(MaxTargetTemp);
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

qint32 ManufacturingTestHandler::TestMainControlASB(Service::ModuleTestCaseID_t Id)
{
    DeviceControl::HimSlaveType_t Slave ;
    if (Id == Service::MAINCONTROL_ASB3) {
       Slave = DeviceControl::Slave_3;
    }
    else if (Id == Service::MAINCONTROL_ASB5) {
       Slave = DeviceControl::Slave_5;
    }
    else if (Id == Service::MAINCONTROL_ASB15) {
       Slave = DeviceControl::Slave_15;
    }
    else {
       qDebug()<<"Error : Wrong Parameter !";
       return -1;
    }

    qDebug()<<"Get SN for BaseModule 3:"<<mp_BaseModule3->GetSerialNumber();
    qDebug()<<"Get SN for BaseModule 5:"<<mp_BaseModule5->GetSerialNumber();
    qDebug()<<"Get SN for BaseModule 15:"<<mp_BaseModule15->GetSerialNumber();

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
    qreal MinTargetTemp = TargetTemp + p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal MaxTargetTemp = TargetTemp + p_TestCase->GetParameter("DepartureHigh").toDouble();

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


    if (CurrentTemp<AmbTempLow || CurrentTemp>AmbTempHigh) {
        QString FailureMsg = QString("Tube Current Temperature is (%1) which is not in (%2~%3)").arg(CurrentTemp).arg(AmbTempLow).arg(AmbTempHigh);
        SetFailReason(Id, FailureMsg);
        p_TestCase->SetStatus(false);

        QString TargetTempStr = QString("%1~%2").arg(MinTargetTemp).arg(MaxTargetTemp);

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

        qDebug()<<"Current temp = "<<CurrentTemp;

        if (CurrentTemp == -1) {
            mp_Utils->Pause(1000);
            WaitSec--;
            continue;
        }

        if ( CurrentTemp >= MinTargetTemp )
        {
            LAStatus = 1;
            break;
        }

        mp_Utils->Pause(1000);
        CurrentValue = QString("%1").arg(CurrentTemp);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.insert("UsedTime", UsedTime);
        Status.insert("CurrentTemp", CurrentValue);
        if (WaitSec == SumSec) {
            QString TargetTempStr = QString("%1~%2").arg(MinTargetTemp).arg(MaxTargetTemp);
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

qint32 ManufacturingTestHandler::MoveRVToTubePos(qint32 Pos)
{
    if ( !mp_MotorRV->MoveToTubePosition(Pos) )
    {
        return -1;
    }

    mp_PressPump->SetTargetPressure(1, Service::TEST_RV_TUBEPOS_PRESS);

    mp_Utils->Pause(Service::TEST_RV_TUBEPOS_PRESS_DURATION);

    mp_PressPump->ReleasePressure();

    return 0;
}

qint32 ManufacturingTestHandler::MoveRVToSealPos(qint32 Pos)
{
    if ( !mp_MotorRV->MoveToSealPosition(Pos) )
    {
        return -1;
    }

    mp_PressPump->SetTargetPressure(1, Service::TEST_RV_SEALPOS_PRESS);

    mp_Utils->Pause(Service::TEST_RV_SEALPOS_PRESS_DURATION);

    if ( mp_PressPump->GetPressure(0) > Service::TEST_RV_SEALPOS_PRESS - Service::TEST_RV_SEALPOS_PRESS_DROP )
    {
        mp_PressPump->ReleasePressure();
        return 0;
    }
    else
    {
        mp_PressPump->ReleasePressure();
        return -1;
    }

}

void ManufacturingTestHandler::SetFailReason(Service::ModuleTestCaseID Id, const QString &FailMsg)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    p_TestCase->AddResult("FailReason", FailMsg);
    p_TestCase->SetStatus(false);

}

void ManufacturingTestHandler::PerformModuleManufacturingTest(Service::ModuleTestCaseID TestId)
{
    qDebug()<<"ManufacturingTestHandler::PerformModuleManufacturingTest  test="<<TestId;

    if(!IsInitialized()){
        Initialize();
    }

    switch (TestId) {
    case Service::TEST_ABORT:
        OnAbortTest(0, TestId);
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
        break;
    case Service::ROTARY_VALVE_SEALING_FUNCTION:
        break;
    case Service::ROTARY_VALVE_HEATING_STATION:
        break;
    case Service::ROTARY_VALVE_HEATING_END:
        break;
    default:
        break;
    }

    qDebug()<<"ManufacturingTestHandler::PerformModuleManufacturingTest  test="<<TestId<<"  End !!";

}

} // end namespace DeviceControl
