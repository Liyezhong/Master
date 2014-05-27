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

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::OVEN_HEATING_WITH_WATER);


    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QTime DurationTime = QTime::fromString(p_TestCase->GetParameter("DurationTime"), "hh:mm:ss");
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal TopTargetTemp = p_TestCase->GetParameter("TopTargetTemp").toDouble();
    qreal BottomTargetTemp = p_TestCase->GetParameter("BottomTargetTemp").toDouble();

    qDebug()<<"DurationTime="<<p_TestCase->GetParameter("DurationTime")<<" TargetTemp="<<TargetTemp;

    WaitSec = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second();

    SumSec = WaitSec;

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
            QString TargetTemp = QString("%1").arg(Service::TEST_OVEN_HEATING_MIN_TEMP);
            Status.insert("TargetTemp", TargetTemp);

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTemp);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        WaitSec--;
    }

    mp_TempOvenTop->StopTemperatureControl();
    mp_TempOvenBottom->StopTemperatureControl();

    p_TestCase->AddResult("UsedTime", UsedTime);
    p_TestCase->AddResult("CurrentTempTop", TopValue);
    p_TestCase->AddResult("CurrentTempBottom1", BottomValue1);
    p_TestCase->AddResult("CurrentTempBottom2", BottomValue2);

    Status.clear();
    Status.insert("OvenHeatingWaterStatus", "Finished");
    emit RefreshTestStatustoMain(TestCaseName, Status);

    if (m_UserAbort) {
        return 1;
    }
    else {

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
    qreal TargetTemp = p_TestCase->GetParameter("TargetTemp").toDouble() + p_TestCase->GetParameter("DepartureLow").toDouble();
    qreal TopTargetTemp = p_TestCase->GetParameter("TopTargetTemp").toDouble();
    qreal BottomTargetTemp = p_TestCase->GetParameter("BottomTargetTemp").toDouble();

    qDebug()<<"DurationTime="<<p_TestCase->GetParameter("DurationTime")<<" TargetTemp="<<TargetTemp;

    WaitSec = DurationTime.hour()*60*60 + DurationTime.minute()*60 + DurationTime.second();

    SumSec = WaitSec;

    mp_TempOvenTop->StartTemperatureControl(TopTargetTemp);
    mp_TempOvenBottom->StartTemperatureControl(BottomTargetTemp);

    while (!m_UserAbort && WaitSec)
    {
        CurrentTempTop = mp_TempOvenTop->GetTemperature(0);
        CurrentTempBottom1 = mp_TempOvenBottom->GetTemperature(0);
        CurrentTempBottom2 = mp_TempOvenBottom->GetTemperature(1);

        if (CurrentTempTop == -1 || CurrentTempBottom1 == -1 || CurrentTempBottom2 == -1) {
            mp_Utils->Pause(10);
            continue;
        }

        if ( CurrentTempTop >= TargetTemp &&
             CurrentTempBottom1 >= TargetTemp &&
             CurrentTempBottom2 >= TargetTemp )
        {
            OvenStatus = 1;
            break;
        }

        mp_Utils->Pause(1000);
        Service::ModuleTestStatus Status;
        TopValue = QString("%1").arg(CurrentTempTop);
        BottomValue1 = QString("%1").arg(CurrentTempBottom1);
        BottomValue2 = QString("%1").arg(CurrentTempBottom2);
        UsedTime = QTime().addSecs(SumSec-WaitSec+1).toString("hh:mm:ss");

        Status.insert("UsedTime", UsedTime);
        Status.insert("CurrentTempTop", TopValue);
        Status.insert("CurrentTempBottom1", BottomValue1);
        Status.insert("CurrentTempBottom2", BottomValue2);
        if (WaitSec == SumSec) {
            QString TargetTemp = QString("%1").arg(Service::TEST_OVEN_HEATING_MIN_TEMP);
            Status.insert("TargetTemp", TargetTemp);

            QString Duration = QTime().addSecs(SumSec).toString("hh:mm:ss");
            Status.insert("Duration", Duration);
            p_TestCase->AddResult("Duration", Duration);
            p_TestCase->AddResult("TargetTemp", TargetTemp);
        }

        emit RefreshTestStatustoMain(TestCaseName, Status);

        WaitSec--;
    }

    mp_TempOvenTop->StopTemperatureControl();
    mp_TempOvenBottom->StopTemperatureControl();

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
    qDebug()<<"Slave 3 voltage = " << m_rIdevProc.IDGetSlaveVoltage(DeviceControl::Slave_3);
    qDebug()<<"Slave 5 voltage = " << m_rIdevProc.IDGetSlaveVoltage(DeviceControl::Slave_5);
    qDebug()<<"Slave 15 voltage = " << m_rIdevProc.IDGetSlaveVoltage(DeviceControl::Slave_15);


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

void ManufacturingTestHandler::PerformModuleManufacturingTest(Service::ModuleTestCaseID TestId)
{
    qDebug()<<"ManufacturingTestHandler::PerformModuleManufacturingTest  test="<<TestId;
    if(!IsInitialized()){
        Initialize();
    }

    switch (TestId) {
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
        if ( NULL == mp_TempOvenTop || NULL == mp_TempOvenBottom )
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
    default:
        break;
    }
}

void ManufacturingTestHandler::SetFailReason(Service::ModuleTestCaseID Id, const QString &FailMsg)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    p_TestCase->AddResult("FailReason", FailMsg);
    p_TestCase->SetStatus(false);

}

} // end namespace DeviceControl
