// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Source/HeatingStrategy.cpp
 *
 *  @brief
 *  Implementation of class Scheduler::HeatingStrategy.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ April 3rd, 2014
 *  $Author:    $ Songtao Yu 
 *
 *  @b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2014 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 * ================================================================================================
*/
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "Scheduler/Commands/Include/CmdRVStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdOvenStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdRTStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdALStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlON.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdOvenSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdRVSetTempCtrlOFF.h"

namespace Scheduler{
HeatingStrategy::HeatingStrategy(SchedulerMainThreadController* schedController,
                                SchedulerCommandProcessorBase* SchedCmdProcessor,
                                DataManager::CDataManager* DataManager)
								:mp_SchedulerController(schedController),
								mp_SchedulerCommandProcessor(SchedCmdProcessor),
                                mp_DataManager(DataManager)
{
    CONNECTSIGNALSLOT(mp_SchedulerCommandProcessor, ReportLevelSensorStatus1(), this, OnReportLevelSensorStatus1());
    m_CurScenario = 0;
    m_CmdResult = true;
    m_IsOvenHeatingStarted = false;
    m_OvenStartHeatingTime = 0;
    if (!this->ConstructHeatingSensorList()){
        mp_SchedulerController->LogDebug(QString("Initialize the heatingStrategy failed!"));
    }
}
DeviceControl::ReturnCode_t HeatingStrategy::RunHeatingStrategy(const HardwareMonitor_t& strctHWMonitor, qint32 scenario)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;

    /********************************************************************************
     *
    Firstly, check if current temperature exceeds max temperature for each sensor
    *
    ********************************************************************************/
    //For Level Sensor
    if (false == this->CheckSensorCurrentTemperature(m_RTLevelSensor, strctHWMonitor.TempALLevelSensor))
    {
        return DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE;
    }
    //For Retort Top Sensor
    if (false == this->CheckSensorCurrentTemperature(m_RTTop, strctHWMonitor.TempRTSide))
    {
        return DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE;
    }
    //For Retort Bottom Sensor
    if (false == this->CheckSensorCurrentTemperature(m_RTBottom, strctHWMonitor.TempRTBottom1))
    {
        return DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE;
    }
    //For Oven Top
    if (false == this->CheckSensorCurrentTemperature(m_OvenTop, strctHWMonitor.TempOvenTop))
    {
        return DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE;
    }
    //For Oven Bottom1
    if (false == this->CheckSensorCurrentTemperature(m_OvenBottom, strctHWMonitor.TempOvenBottom1))
    {
        return DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE;
    }
    //For Oven Bottom2
    if (false == this->CheckSensorCurrentTemperature(m_OvenBottom, strctHWMonitor.TempOvenBottom2))
    {
        return DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE;
    }
    //For Rotary Valve Rod
    if (false == this->CheckSensorCurrentTemperature(m_RV_1_HeatingRod, strctHWMonitor.TempRV1))
    {
        return DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE;
    }
    //For Rotary Valve outlet
    if (false == this->CheckSensorCurrentTemperature(m_RV_2_Outlet, strctHWMonitor.TempRV2))
    {
        return DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE;
    }

    //For LA RV Tube
    if (false == this->CheckSensorCurrentTemperature(m_LARVTube, strctHWMonitor.TempALTube1))
    {
        return DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE;
    }

    //For LA Wax Trap
    if (false == this->CheckSensorCurrentTemperature(m_LAWaxTrap, strctHWMonitor.TempALTube2))
    {
        return DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE;
    }

    /***************************************************
     *
    Set temperature for each sensor
    *
    ***************************************************/

    if (scenario != m_CurScenario)
    {
        mp_SchedulerController->LogDebug(QString("the coming scenario is:%1, but the last scenario is:%2").arg(scenario).arg(m_CurScenario));
        m_CurScenario = scenario;
        m_RTLevelSensor.SetTemp4High = false; //for each scenario, set the initial value is false
        m_RTLevelSensor.SetTemp4Low = false;  //for each scenario, set the initial value is false
        //For RTTop
        retCode = StartRTTemperatureControl(m_RTTop, RT_SIDE);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
        //For RTBottom
        retCode = StartRTTemperatureControl(m_RTBottom, RT_BOTTOM);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }

        // For Oven Top
        retCode = StartOvenTemperatureControl(m_OvenTop, OVEN_TOP);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
        //For Oven Bottom
        retCode = StartOvenTemperatureControl(m_OvenBottom, OVEN_BOTTOM);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }

        if (false == m_IsOvenHeatingStarted)
        {
            m_OvenStartHeatingTime = QDateTime::currentMSecsSinceEpoch();
            m_IsOvenHeatingStarted = true;
        }
        //For RVRod
        retCode = StartRVTemperatureControl(m_RV_1_HeatingRod);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }

        //For RVOutlet
        StartRVOutletHeatingOTCalculation();

        //For LA RVTube
        retCode = StartLATemperatureControl(m_LARVTube, AL_TUBE1);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
        //For LA WaxTrap
        retCode = StartLATemperatureControl(m_LAWaxTrap, AL_TUBE2);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }

    }

    // For Level Sensor, we need set two times (high and low) in each scenario
    retCode = this->StartLevelSensorTemperatureControl(strctHWMonitor);
    if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
    {
        return retCode;
    }


    /***********************************************************
     *
    Check temperature difference of two Retort bottom sensors
    *
    ***********************************************************/
    if ( !QFile::exists("TEST_RETORT") )
    {
        if (false == m_RTBottom.curModuleId.isEmpty() &&
                -1!= m_RTBottom.functionModuleList[m_RTBottom.curModuleId].ScenarioList.indexOf(m_CurScenario))
        {
            if ( isEffectiveTemp(strctHWMonitor.TempRTBottom1) && isEffectiveTemp(strctHWMonitor.TempRTBottom2))
            {
                if (std::abs(strctHWMonitor.TempRTBottom1 - strctHWMonitor.TempRTBottom2) >= m_RTBottom.TemperatureDiffList[m_RTBottom.curModuleId])
                {
                    mp_SchedulerController->LogDebug(QString("the difference of two retort bottom sensor over range."));
                    return DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED;
                }
            }
        }
    }

    /*******************************************************
     *
    Check Heating Overtime
    *
    ******************************************************/
    // For Level Sensor
    if (false == this->CheckSensorHeatingOverTime(m_RTLevelSensor, strctHWMonitor.TempALLevelSensor))
    {
        return DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME;
    }
    // For Retort Top
    if (false == this->CheckSensorHeatingOverTime(m_RTTop, strctHWMonitor.TempRTSide))
    {
        return DCL_ERR_DEV_RETORT_SIDETOP_HEATING_ELEMENT_FAILED;
    }
    // For Retort Bottom
    if (false == this->CheckSensorHeatingOverTime(m_RTBottom, strctHWMonitor.TempRTBottom1))
    {
        return DCL_ERR_DEV_RETORT_BOTTOM_HEATING_ELEMENT_FAILED;
    }
    //For Oven Top
    if (false == this->CheckSensorHeatingOverTime(m_OvenTop, strctHWMonitor.TempOvenTop))
    {
        return DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_ABNORMAL;
    }
    //For Oven Bottom
    if (false == this->CheckSensorHeatingOverTime(m_OvenBottom, strctHWMonitor.TempOvenBottom1))
    {
        return DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_ABNORMAL;
    }
    //For RV Outlet, Please note RV Rod(sensor 1) is NOT needed to check Heating overtime.
    if (false == this->CheckRVOutletHeatingOverTime(strctHWMonitor.TempRV2))
    {
        return DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_NOTREACHTARGET;
    }
    //For LA RVTube
    if (false == this->CheckSensorHeatingOverTime(m_LARVTube, strctHWMonitor.TempALTube1))
    {
        return DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP;
    }
    //For LA WaxTrap
    if (false == this->CheckSensorHeatingOverTime(m_LAWaxTrap, strctHWMonitor.TempALTube2))
    {
        return DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_NOTREACHTARGETTEMP;
    }

    return DCL_ERR_FCT_CALL_SUCCESS;
}

ReturnCode_t HeatingStrategy::StartTemperatureControl(const QString& HeaterName)
{
    CmdSchedulerCommandBase* pHeatingCmd = NULL;
    if ("LevelSensor" == HeaterName)
    {
        pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(AL_LEVELSENSOR);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].TemperatureOffset);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].SlopTempChange);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].MaxTemperature);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].ControllerGain);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].ResetTime);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].DerivativeTime);
        m_RTLevelSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RTLevelSensor.OTCheckPassed = false;
    }
    if ("RTSide" == HeaterName)
    {
        qreal userInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
            ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
        pHeatingCmd  = new CmdRTStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(RT_SIDE);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RTTop.functionModuleList[m_RTTop.curModuleId].TemperatureOffset+userInputTemp);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_RTTop.functionModuleList[m_RTTop.curModuleId].SlopTempChange);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_RTTop.functionModuleList[m_RTTop.curModuleId].MaxTemperature);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_RTTop.functionModuleList[m_RTTop.curModuleId].ControllerGain);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_RTTop.functionModuleList[m_RTTop.curModuleId].ResetTime);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_RTTop.functionModuleList[m_RTTop.curModuleId].DerivativeTime);
        m_RTTop.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RTTop.OTCheckPassed = false;
    }
    if ("RTBottom" == HeaterName)
    {
        qreal userInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
            ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
        pHeatingCmd  = new CmdRTStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(RT_BOTTOM);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RTTop.functionModuleList[m_RTTop.curModuleId].TemperatureOffset+userInputTemp);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_RTTop.functionModuleList[m_RTTop.curModuleId].SlopTempChange);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_RTTop.functionModuleList[m_RTTop.curModuleId].MaxTemperature);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_RTTop.functionModuleList[m_RTTop.curModuleId].ControllerGain);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_RTTop.functionModuleList[m_RTTop.curModuleId].ResetTime);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_RTTop.functionModuleList[m_RTTop.curModuleId].DerivativeTime);
        m_RTBottom.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RTBottom.OTCheckPassed = false;
    }
    if ("OvenTop" == HeaterName)
    {
        //Firstly, get the Parrifin melting point (user input)
        qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        pHeatingCmd  = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(OVEN_TOP);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].TemperatureOffset+userInputMeltingPoint);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].SlopTempChange);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].MaxTemperature);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].ControllerGain);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].ResetTime);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].DerivativeTime);
        m_OvenTop.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_OvenTop.OTCheckPassed = false;
    }
    if ("OvenBottom" == HeaterName)
    {
        //Firstly, get the Parrifin melting point (user input)
        qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        pHeatingCmd  = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(OVEN_BOTTOM);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].TemperatureOffset+userInputMeltingPoint);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].SlopTempChange);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].MaxTemperature);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].ControllerGain);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].ResetTime);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].DerivativeTime);
        m_OvenBottom.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_OvenBottom.OTCheckPassed = false;
    }
    if ("RV" == HeaterName)
    {
        //Firstly, get the Parrifin melting point (user input)
        qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        pHeatingCmd  = new CmdRVStartTemperatureControlWithPID(500, mp_SchedulerController);
        if (true == m_RV_1_HeatingRod.UserInputFlagList[m_RV_1_HeatingRod.curModuleId])
        {
            dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].TemperatureOffset+userInputMeltingPoint);
        }
        else
        {
            dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].TemperatureOffset);
        }
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].SlopTempChange);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].MaxTemperature);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].ControllerGain);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].ResetTime);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].DerivativeTime);
        m_RV_1_HeatingRod.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RV_1_HeatingRod.OTCheckPassed = false;
    }
    if ("LA_Tube1" == HeaterName)
    {
        pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(AL_TUBE1);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_LARVTube.functionModuleList[m_LARVTube.curModuleId].TemperatureOffset);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_LARVTube.functionModuleList[m_LARVTube.curModuleId].SlopTempChange);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_LARVTube.functionModuleList[m_LARVTube.curModuleId].MaxTemperature);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_LARVTube.functionModuleList[m_LARVTube.curModuleId].ControllerGain);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_LARVTube.functionModuleList[m_LARVTube.curModuleId].ResetTime);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_LARVTube.functionModuleList[m_LARVTube.curModuleId].DerivativeTime);
        m_LARVTube.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_LARVTube.OTCheckPassed = false;
    }
    if ("LA_Tube2" == HeaterName)
    {
        pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(AL_TUBE2);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_LAWaxTrap.functionModuleList[m_LAWaxTrap.curModuleId].TemperatureOffset);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_LAWaxTrap.functionModuleList[m_LAWaxTrap.curModuleId].SlopTempChange);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_LAWaxTrap.functionModuleList[m_LAWaxTrap.curModuleId].MaxTemperature);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_LAWaxTrap.functionModuleList[m_LAWaxTrap.curModuleId].ControllerGain);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_LAWaxTrap.functionModuleList[m_LAWaxTrap.curModuleId].ResetTime);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_LAWaxTrap.functionModuleList[m_LAWaxTrap.curModuleId].DerivativeTime);
        m_LAWaxTrap.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_LAWaxTrap.OTCheckPassed = false;
    }
    mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);

    SchedulerCommandShPtr_t pResHeatingCmd;
    mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd, pHeatingCmd->GetName());
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    pResHeatingCmd->GetResult(retCode);
    return retCode;
}

ReturnCode_t HeatingStrategy::StopTemperatureControl(const QString& HeaterName)
{
    CmdSchedulerCommandBase* pHeatingCmd = NULL;
    if ("LevelSensor" == HeaterName)
    {
        pHeatingCmd  = new CmdALSetTempCtrlOFF(500, mp_SchedulerController);
        dynamic_cast<CmdALSetTempCtrlOFF*>(pHeatingCmd)->Settype(AL_LEVELSENSOR);
    }
    if ("RTSide" == HeaterName)
    {
        pHeatingCmd  = new CmdRTSetTempCtrlOFF(500, mp_SchedulerController);
        dynamic_cast<CmdRTSetTempCtrlOFF*>(pHeatingCmd)->SetType(RT_SIDE);
    }
    if ("RTBottom" == HeaterName)
    {

        pHeatingCmd  = new CmdRTSetTempCtrlOFF(500, mp_SchedulerController);
        dynamic_cast<CmdRTSetTempCtrlOFF*>(pHeatingCmd)->SetType(RT_BOTTOM);
    }
    if ("OvenTop" == HeaterName)
    {

        pHeatingCmd  = new CmdOvenSetTempCtrlOFF(500, mp_SchedulerController);
        dynamic_cast<CmdOvenSetTempCtrlOFF*>(pHeatingCmd)->Settype(OVEN_TOP);
    }
    if ("OvenBottom" == HeaterName)
    {
        pHeatingCmd  = new CmdOvenSetTempCtrlOFF(500, mp_SchedulerController);
        dynamic_cast<CmdOvenSetTempCtrlOFF*>(pHeatingCmd)->Settype(OVEN_BOTTOM);
    }
    if ("RV" == HeaterName)
    {
        pHeatingCmd  = new CmdRVSetTempCtrlOFF(500, mp_SchedulerController);
    }
    if ("LA_Tube1" == HeaterName)
    {
        pHeatingCmd  = new CmdALSetTempCtrlOFF(500, mp_SchedulerController);
        dynamic_cast<CmdALSetTempCtrlOFF*>(pHeatingCmd)->Settype(AL_TUBE1);
    }
    if ("LA_Tube2" == HeaterName)
    {
        pHeatingCmd  = new CmdALSetTempCtrlOFF(500, mp_SchedulerController);
        dynamic_cast<CmdALSetTempCtrlOFF*>(pHeatingCmd)->Settype(AL_TUBE2);
    }  
    mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);

    SchedulerCommandShPtr_t pResHeatingCmd;
    mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd, pHeatingCmd->GetName());
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    pResHeatingCmd->GetResult(retCode);
    return retCode;
}

bool HeatingStrategy:: CheckRTBottomsDifference(qreal temp1, qreal temp2)
{
    if ( isEffectiveTemp(temp1) && isEffectiveTemp(temp2))
    {
        if (std::abs(temp1 - temp2) >= m_RTBottom.TemperatureDiffList[m_RTBottom.curModuleId])
        {
            mp_SchedulerController->LogDebug(QString("the difference of two retort bottom sensor over range."));
            return false;
        }
    }
    return true;
}

quint16 HeatingStrategy::CheckTemperatureOverTime(const QString& HeaterName, qreal HWTemp)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if ("LevelSensor" == HeaterName)
    {
        if ((now-m_RTLevelSensor.heatingStartTime) >= m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].HeatingOverTime*1000)
        {
            return 1; //failed
        }

        if (HWTemp >= m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].OTTargetTemperature)
        {
            return 2; //pass
        }
    }
    return 0; // Have not got the time out
}

bool HeatingStrategy::CheckTemperatureSenseorsStatus() const
{
    return  m_RTLevelSensor.OTCheckPassed;
}
bool HeatingStrategy::CheckSensorCurrentTemperature(const HeatingSensor& heatingSensor, qreal HWTemp)
{
    if (true == heatingSensor.curModuleId.isEmpty())
    {
        return true;
    }

    if ( !isEffectiveTemp(HWTemp) )
    {
        return true;
    }

    //For Scenarios NON-related sensors(Oven and LA)
    if (1 == heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.size()
            && 0 == heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.at(0))
    {
        if (heatingSensor.functionModuleList[heatingSensor.curModuleId].MaxTemperature <HWTemp)
        {
            mp_SchedulerController->LogDebug(QString("For Scenarios NON-related sensor,The hope maxtemp:%1, but the actual temp:%2")
                                                      .arg(heatingSensor.functionModuleList[heatingSensor.curModuleId].MaxTemperature)
                                                      .arg(HWTemp));
            return false;
        }
    }
    //For Scenarios related Sensors (Retort and Rotary valve)
    if (-1 != heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.indexOf(m_CurScenario))
    {
        if (heatingSensor.functionModuleList[heatingSensor.curModuleId].MaxTemperature <HWTemp)
        {
            mp_SchedulerController->LogDebug(QString("For Scenarios related sensors,The hope maxtemp:%1, but the actual temp:%2")
                                             .arg(heatingSensor.functionModuleList[heatingSensor.curModuleId].MaxTemperature)
                                             .arg(HWTemp));
            return false;
        }
    }

    return true;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartLevelSensorTemperatureControl(const HardwareMonitor_t& strctHWMonitor)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;

    //For LevelSensor
    QMap<QString, FunctionModule>::iterator iter = m_RTLevelSensor.functionModuleList.begin();
    for (; iter!=m_RTLevelSensor.functionModuleList.end(); ++iter)
    {
        // Current(new) scenario belongs to the specific scenario list
        if (iter->ScenarioList.indexOf(m_CurScenario) != -1)
        {
            // Check for High or Low speed
            if (m_RTLevelSensor.ExchangePIDTempList[iter->Id] >= strctHWMonitor.TempALLevelSensor)
            {
                if ("High" == m_RTLevelSensor.CurrentSpeedList[iter->Id] && (false == m_RTLevelSensor.SetTemp4High))
                {
                    m_RTLevelSensor.SetTemp4High = true;
                    break;
                }
                else
                {
                    continue;
                }
            }
            else if (m_RTLevelSensor.ExchangePIDTempList[iter->Id] < strctHWMonitor.TempALLevelSensor)
            {
                if ("Low" == m_RTLevelSensor.CurrentSpeedList[iter->Id] && (false == m_RTLevelSensor.SetTemp4Low))
                {
                    m_RTLevelSensor.SetTemp4Low = true;
                    break;
                }
                else
                {
                    continue;
                }
            }

        }
    }

    // Found out the level sensor's function module
    if (iter != m_RTLevelSensor.functionModuleList.end())
    {
        CmdALStartTemperatureControlWithPID* pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        pHeatingCmd->SetType(AL_LEVELSENSOR);
        pHeatingCmd->SetNominalTemperature(iter->TemperatureOffset);
        pHeatingCmd->SetSlopeTempChange(iter->SlopTempChange);
        pHeatingCmd->SetMaxTemperature(iter->MaxTemperature);
        pHeatingCmd->SetControllerGain(iter->ControllerGain);
        pHeatingCmd->SetResetTime(iter->ResetTime);
        pHeatingCmd->SetDerivativeTime(iter->DerivativeTime);
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);
        SchedulerCommandShPtr_t pResHeatingCmd;
        mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd, pHeatingCmd->GetName());
        pResHeatingCmd->GetResult(retCode);

        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
        else
        {
            mp_SchedulerController->LogDebug(
                        QString("start level sensor heating, scenario:%1, tmpoffset %2, slop %3, maxtemp %4, gain %5,resettime %6, derivativetime %7")
                        .arg(m_CurScenario).arg(iter->TemperatureOffset).arg(iter->SlopTempChange).arg(iter->MaxTemperature)
                        .arg(iter->ControllerGain).arg(iter->ResetTime).arg(iter->DerivativeTime));
            m_RTLevelSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
            m_RTLevelSensor.curModuleId = iter->Id;
            m_RTLevelSensor.OTCheckPassed = false;
            iter->OTTargetTemperature = iter->TemperatureOffset;
        }
    }

    return DCL_ERR_FCT_CALL_SUCCESS;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartRTTemperatureControl(HeatingSensor& heatingSensor, RTTempCtrlType_t RTType)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;

    QMap<QString, FunctionModule>::iterator iter = heatingSensor.functionModuleList.begin();
    for (; iter!=heatingSensor.functionModuleList.end(); ++iter)
    {
        // Current(new) scenario belongs to the specific scenario list
        if (iter->ScenarioList.indexOf(m_CurScenario) != -1)
        {
            break;
        }
    }

    // Found out the heating sensor's function module
    if (iter != heatingSensor.functionModuleList.end())
    {
		// Get the userInput temperature
		qreal userInputTemp = 0.0;
        // make sure program ID is NOT empty and current Program Step Index is NOT -1
        if (false == mp_SchedulerController->GetCurProgramID().isEmpty() && -1 != mp_SchedulerController->GetCurProgramStepIndex())
        {
            userInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
                ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
        }   
		else
		{
			return DCL_ERR_FCT_CALL_SUCCESS;
		}
        if (qAbs(userInputTemp) <= 0.000000000001)
        {   
            return DCL_ERR_FCT_CALL_SUCCESS;
        }  
        CmdRTStartTemperatureControlWithPID* pHeatingCmd  = new CmdRTStartTemperatureControlWithPID(500, mp_SchedulerController);
        pHeatingCmd->SetType(RTType);
        pHeatingCmd->SetNominalTemperature(iter->TemperatureOffset+userInputTemp);
        pHeatingCmd->SetSlopeTempChange(iter->SlopTempChange);
        pHeatingCmd->SetMaxTemperature(iter->MaxTemperature);
        pHeatingCmd->SetControllerGain(iter->ControllerGain);
        pHeatingCmd->SetResetTime(iter->ResetTime);
        pHeatingCmd->SetDerivativeTime(iter->DerivativeTime);
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);
        SchedulerCommandShPtr_t pResHeatingCmd;
        mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd, pHeatingCmd->GetName());
        pResHeatingCmd->GetResult(retCode);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
        else
        {
            mp_SchedulerController->LogDebug(
                        QString("start Retort(%8) heating, scenario:%1, tmpoffset %2, slop %3, maxtemp %4, gain %5,resettime %6, derivativetime %7")
                        .arg(m_CurScenario).arg(iter->TemperatureOffset+userInputTemp).arg(iter->SlopTempChange).arg(iter->MaxTemperature)
                        .arg(iter->ControllerGain).arg(iter->ResetTime).arg(iter->DerivativeTime).arg(RTType));
            heatingSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
            heatingSensor.curModuleId = iter->Id;
            heatingSensor.OTCheckPassed = false;
            iter->OTTargetTemperature = iter->TemperatureOffset+userInputTemp;
            return DCL_ERR_FCT_CALL_SUCCESS;
        }
    }

    // The current scenario is NOT related to Level Sensor's ones.
    return DCL_ERR_FCT_CALL_SUCCESS;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartOvenTemperatureControl(OvenSensor& heatingSensor, OVENTempCtrlType_t OvenType)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;

    //Firstly, get the Parrifin melting point (user input)
    qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();

    QMap<QString, FunctionModule>::iterator iter = heatingSensor.functionModuleList.begin();
    for(; iter!=heatingSensor.functionModuleList.end(); ++iter)
    {
        QPair<qreal,qreal> meltingRange = heatingSensor.ParaffinTempRangeList[iter->Id];
        QPair<qreal,qreal> timeRange = heatingSensor.TimeRangeList[iter->Id];

        // Get Time Elapse
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qreal timeElapse = 0.0;
        if (0 != heatingSensor.heatingStartTime)
        {
            timeElapse = (now - heatingSensor.heatingStartTime)/1000;
        }

        if (meltingRange.first<=userInputMeltingPoint && meltingRange.second>=userInputMeltingPoint && timeRange.first<=timeElapse && timeRange.second>=timeElapse)
        {
            break;
        }
    }

    // Found out the heating sensor's function module
    if (iter != heatingSensor.functionModuleList.end())
    {
        CmdOvenStartTemperatureControlWithPID* pHeatingCmd  = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        pHeatingCmd->SetType(OvenType);
        pHeatingCmd->SetNominalTemperature(iter->TemperatureOffset+userInputMeltingPoint);
        pHeatingCmd->SetSlopeTempChange(iter->SlopTempChange);
        pHeatingCmd->SetMaxTemperature(iter->MaxTemperature);
        pHeatingCmd->SetControllerGain(iter->ControllerGain);
        pHeatingCmd->SetResetTime(iter->ResetTime);
        pHeatingCmd->SetDerivativeTime(iter->DerivativeTime);
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);
        SchedulerCommandShPtr_t pResHeatingCmd;
        mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd, pHeatingCmd->GetName());
        pResHeatingCmd->GetResult(retCode);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
        else
        {
            mp_SchedulerController->LogDebug(
                        QString("start Oven(%8) heating, scenario:%1, tmpoffset %2, slop %3, maxtemp %4, gain %5,resettime %6, derivativetime %7")
                        .arg(m_CurScenario).arg(iter->TemperatureOffset+userInputMeltingPoint).arg(iter->SlopTempChange).arg(iter->MaxTemperature)
                        .arg(iter->ControllerGain).arg(iter->ResetTime).arg(iter->DerivativeTime).arg(OvenType));
            heatingSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
            heatingSensor.curModuleId = iter->Id;
            heatingSensor.OTCheckPassed = false;
            iter->OTTargetTemperature = heatingSensor.OTTempOffsetList[iter->Id]+userInputMeltingPoint;
            return DCL_ERR_FCT_CALL_SUCCESS;
        }
    }

    // The current scenario is NOT related to Level Sensor's ones.
    return DCL_ERR_FCT_CALL_SUCCESS;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartRVTemperatureControl(RVSensor& heatingSensor)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;

    //Firstly, get the Parrifin melting point (user input)
    qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();

    QMap<QString, FunctionModule>::iterator iter = heatingSensor.functionModuleList.begin();
    for (; iter!=heatingSensor.functionModuleList.end(); ++iter)
    {
        // Current(new) scenario belongs to the specific scenario list
        if (iter->ScenarioList.indexOf(m_CurScenario) != -1)
        {
            break;
        }
    }

    // Found out the heating sensor's function module
    if (iter != heatingSensor.functionModuleList.end())
    {
        qreal tempOffset = 0;
        CmdRVStartTemperatureControlWithPID* pHeatingCmd  = new CmdRVStartTemperatureControlWithPID(500, mp_SchedulerController);
        if (true == heatingSensor.UserInputFlagList[iter->Id])
        {
            tempOffset = iter->TemperatureOffset+userInputMeltingPoint;
            pHeatingCmd->SetNominalTemperature(iter->TemperatureOffset+userInputMeltingPoint);
            tempOffset = iter->TemperatureOffset+userInputMeltingPoint;
        }
        else
        {
            tempOffset = iter->TemperatureOffset;
            pHeatingCmd->SetNominalTemperature(iter->TemperatureOffset);
            tempOffset = iter->TemperatureOffset;
        }
        pHeatingCmd->SetSlopeTempChange(iter->SlopTempChange);
        pHeatingCmd->SetMaxTemperature(iter->MaxTemperature);
        pHeatingCmd->SetControllerGain(iter->ControllerGain);
        pHeatingCmd->SetResetTime(iter->ResetTime);
        pHeatingCmd->SetDerivativeTime(iter->DerivativeTime);
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);
        SchedulerCommandShPtr_t pResHeatingCmd;
        mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd, pHeatingCmd->GetName());
        pResHeatingCmd->GetResult(retCode);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
        else
        {
            mp_SchedulerController->LogDebug(
                        QString("start RV heating, scenario:%1, tmpoffset %2, slop %3, maxtemp %4, gain %5,resettime %6, derivativetime %7")
                        .arg(m_CurScenario).arg(tempOffset).arg(iter->SlopTempChange).arg(iter->MaxTemperature)
                        .arg(iter->ControllerGain).arg(iter->ResetTime).arg(iter->DerivativeTime));
            heatingSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
            heatingSensor.curModuleId = iter->Id;
            heatingSensor.OTCheckPassed = false;
            return DCL_ERR_FCT_CALL_SUCCESS;
        }
    }

    // The current scenario is NOT related to Level Sensor's ones.
    return DCL_ERR_FCT_CALL_SUCCESS;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartLATemperatureControl(HeatingSensor& heatingSensor,ALTempCtrlType_t LAType)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;

    QMap<QString, FunctionModule>::iterator iter = heatingSensor.functionModuleList.begin();

    // Found out the heating sensor's function module
    if (iter != heatingSensor.functionModuleList.end())
    {
        CmdALStartTemperatureControlWithPID* pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        pHeatingCmd->SetType(LAType);
        pHeatingCmd->SetNominalTemperature(iter->TemperatureOffset);
        pHeatingCmd->SetSlopeTempChange(iter->SlopTempChange);
        pHeatingCmd->SetMaxTemperature(iter->MaxTemperature);
        pHeatingCmd->SetControllerGain(iter->ControllerGain);
        pHeatingCmd->SetResetTime(iter->ResetTime);
        pHeatingCmd->SetDerivativeTime(iter->DerivativeTime);
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);
        SchedulerCommandShPtr_t pResHeatingCmd;
        mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd, pHeatingCmd->GetName());
        pResHeatingCmd->GetResult(retCode);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
        else
        {
            mp_SchedulerController->LogDebug(
                        QString("start LA(%8) heating, scenario:%1, tmpoffset %2, slop %3, maxtemp %4, gain %5,resettime %6, derivativetime %7")
                        .arg(m_CurScenario).arg(iter->TemperatureOffset).arg(iter->SlopTempChange).arg(iter->MaxTemperature)
                        .arg(iter->ControllerGain).arg(iter->ResetTime).arg(iter->DerivativeTime).arg(LAType));
            heatingSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
            heatingSensor.curModuleId = iter->Id;

            // For LA RV Tube and LA Wax Trap, we only check HeatingOverTime for the scenario which belong to OTScenarioList
            if (AL_TUBE1 == LAType || AL_TUBE2 == LAType)
            {
                if (-1 != static_cast<LASensor&>(heatingSensor).OTCheckScenarioList[iter->Id].indexOf(m_CurScenario))
                {
                    heatingSensor.OTCheckPassed = false;
                }
                else
                {
                    heatingSensor.OTCheckPassed = true;
                }
            }
            else //for Retort Level Sensor
            {
                heatingSensor.OTCheckPassed = false;
            }
            iter->OTTargetTemperature = iter->TemperatureOffset;
            return DCL_ERR_FCT_CALL_SUCCESS;
        }
    }

    // The current scenario is NOT related to Level Sensor's ones.
    return DCL_ERR_FCT_CALL_SUCCESS;
}

void HeatingStrategy::StartRVOutletHeatingOTCalculation()
{
    QMap<QString, FunctionModule>::iterator iter = m_RV_2_Outlet.functionModuleList.begin();
    for (; iter != m_RV_2_Outlet.functionModuleList.end(); ++iter)
    {
        // Current(new) scenario belongs to the specific scenario list
        if (iter->ScenarioList.indexOf(m_CurScenario) != -1)
        {
            m_RV_2_Outlet.curModuleId = iter->Id;
            if( -1 != m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.needCheckOTModuleId].ScenarioList.indexOf(m_CurScenario) )
            {
                mp_SchedulerController->LogDebug(QString("statrt RV sensor2 heating Calculation scenario:%1").arg(m_CurScenario));
                m_RV_2_Outlet.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
                m_RV_2_Outlet.OTCheckPassed = false;
                m_RV_2_Outlet.needCheckOT = true;
            }
            break;
        }
    }
}

bool HeatingStrategy::ConstructHeatingSensorList()
{
    //For Retort Level Sensor
    m_RTLevelSensor.devName = "Retort";
    m_RTLevelSensor.sensorName = "LevelSensor";
    m_RTLevelSensor.heatingStartTime = 0;
    m_RTLevelSensor.curModuleId = "";
    m_RTLevelSensor.OTCheckPassed = false;
    m_RTLevelSensor.SetTemp4High = false;
    m_RTLevelSensor.SetTemp4Low = false;
    QStringList sequenceList = {"11", "12", "21", "22"};
    if (false == this->ConstructHeatingSensor(m_RTLevelSensor, sequenceList))
    {
        return false;
    }
    // Add Current Speed (low or high) list and PID exchanging Temperature list
    QStringList::const_iterator iter = sequenceList.begin();
    for(; iter!=sequenceList.end(); ++iter)
    {
        DataManager::FunctionKey_t funcKey;
        funcKey.key = "Heating";
        funcKey.name = m_RTLevelSensor.sensorName;
        funcKey.sequence = *iter;
        QString curSpeed = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_RTLevelSensor.devName, funcKey, "CurrentSPeed");
        m_RTLevelSensor.CurrentSpeedList.insert(*iter, curSpeed);
        bool ok = false;
        qreal exchangePIDtmp = mp_DataManager->GetProgramSettings()->GetParameterValue(m_RTLevelSensor.devName, funcKey, "ExchangePIDTemp", ok);
        if (false == ok)
        {
            return false;
        }
        m_RTLevelSensor.ExchangePIDTempList.insert(*iter, exchangePIDtmp);
    }


    // For Retort Top
    m_RTTop.devName = "Retort";
    m_RTTop.sensorName = "RTTop";
    m_RTTop.heatingStartTime = 0;
    m_RTTop.curModuleId = "";
    m_RTTop.OTCheckPassed = false;
    sequenceList = {"1", "2", "3", "4"};
    if (false == this->ConstructHeatingSensor(m_RTTop, sequenceList))
    {
        return false;
    }

    // For Retort Bottom
    m_RTBottom.devName = "Retort";
    m_RTBottom.sensorName = "RTBottom";
    m_RTBottom.heatingStartTime = 0;
    m_RTBottom.curModuleId = "";
    m_RTBottom.OTCheckPassed = false;
    sequenceList = {"1", "2", "3", "4"};
    if (false == this->ConstructHeatingSensor(m_RTBottom, sequenceList))
    {
        return false;
    }
    // Add Temperature difference for each sequence
    iter = sequenceList.begin();
    for (; iter!= sequenceList.end(); ++iter)
    {
        DataManager::FunctionKey_t funcKey;
        funcKey.key = "Heating";
        funcKey.name = m_RTBottom.sensorName;
        funcKey.sequence = *iter;
        bool ok = false;
        qreal tempDiff = mp_DataManager->GetProgramSettings()->GetParameterValue(m_RTBottom.devName, funcKey, "TempDifference", ok);
        if (false == ok)
        {
            return false;
        }
        m_RTBottom.TemperatureDiffList.insert(*iter, tempDiff);
    }

    //For Oven Top
    m_OvenTop.devName = "Oven";
    m_OvenTop.sensorName = "OvenTop";
    m_OvenTop.heatingStartTime = 0;
    m_OvenTop.curModuleId = "";
    m_OvenTop.OTCheckPassed = false;
    sequenceList = {"1", "2", "3", "4"};
    if (false == this->ConstructHeatingSensor(m_OvenTop, sequenceList))
    {
        return false;
    }
    //Add other attributes for Oven Top
    iter = sequenceList.begin();
    for (; iter != sequenceList.end(); ++iter)
    {
        DataManager::FunctionKey_t funcKey;
        funcKey.key = "Heating";
        funcKey.name = m_OvenTop.sensorName;
        funcKey.sequence = *iter;
        bool ok = false;

        //OverTimeTempOffset
        qreal OTTempOffset = mp_DataManager->GetProgramSettings()->GetParameterValue(m_OvenTop.devName, funcKey, "OverTimeTempOffset", ok);
        if (false == ok)
        {
            return false;
        }
        m_OvenTop.OTTempOffsetList.insert(*iter, OTTempOffset);

        //ParaffinTempRange
        QString paraffinTempRange = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_OvenTop.devName, funcKey, "ParaffinTempRange");
        QStringList  strList = paraffinTempRange.split(",");
        QPair<qreal,qreal> tempPair;
        tempPair.first = strList[0].toDouble(&ok);
        tempPair.second = strList[1].toDouble(&ok);
        m_OvenTop.ParaffinTempRangeList.insert(*iter, tempPair);

        //TimeRange
        QString timeRange = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_OvenTop.devName, funcKey, "TimeRange");
        strList = timeRange.split(",");
        QPair<qreal, qreal> timePair;
        timePair.first = strList[0].toDouble(&ok);
        timePair.second = strList[1].toDouble(&ok);
        m_OvenTop.TimeRangeList.insert(*iter,timePair);
    }

    //For Oven Bottom
    m_OvenBottom.devName = "Oven";
    m_OvenBottom.sensorName = "OvenBottom";
    m_OvenBottom.heatingStartTime = 0;
    m_OvenBottom.curModuleId = "";
    m_OvenBottom.OTCheckPassed = false;
    sequenceList = {"1", "2", "3", "4"};
    if (false == this->ConstructHeatingSensor(m_OvenBottom, sequenceList))
    {
        return false;
    }
    //Add other attributes for Oven Bottom
    iter = sequenceList.begin();
    for (; iter != sequenceList.end(); ++iter)
    {
        DataManager::FunctionKey_t funcKey;
        funcKey.key = "Heating";
        funcKey.name = m_OvenBottom.sensorName;
        funcKey.sequence = *iter;
        bool ok = false;

        //OverTimeTempOffset
        qreal OTTempOffset = mp_DataManager->GetProgramSettings()->GetParameterValue(m_OvenBottom.devName, funcKey, "OverTimeTempOffset", ok);
        if (false == ok)
        {
            return false;
        }
        m_OvenBottom.OTTempOffsetList.insert(*iter, OTTempOffset);

        //ParaffinTempRange
        QString paraffinTempRange = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_OvenBottom.devName, funcKey, "ParaffinTempRange");
        QStringList  strList = paraffinTempRange.split(",");
        QPair<qreal,qreal> tempPair;
        tempPair.first = strList[0].toDouble(&ok);
        tempPair.second = strList[1].toDouble(&ok);
        m_OvenBottom.ParaffinTempRangeList.insert(*iter, tempPair);

        //TimeRange
        QString timeRange = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_OvenBottom.devName, funcKey, "TimeRange");
        strList = timeRange.split(",");
        QPair<qreal, qreal> timePair;
        timePair.first = strList[0].toDouble(&ok);
        timePair.second = strList[1].toDouble(&ok);
        m_OvenBottom.TimeRangeList.insert(*iter,timePair);
    }

    //For Rotary Valve Rod
    m_RV_1_HeatingRod.devName = "Rotary Valve";
    m_RV_1_HeatingRod.sensorName = "RV_1_HeatingRod";
    m_RV_1_HeatingRod.heatingStartTime = 0;
    m_RV_1_HeatingRod.curModuleId = "";
    m_RV_1_HeatingRod.OTCheckPassed = false;
    sequenceList = {"1", "2", "3", "4"};
    if (false == this->ConstructHeatingSensor(m_RV_1_HeatingRod, sequenceList))
    {
        return false;
    }
    //Add other attributes for Rotary Valve Rod
    iter = sequenceList.begin();
    for (; iter != sequenceList.end(); ++iter)
    {
       //For checking if user input temperature is needed
        DataManager::FunctionKey_t funcKey;
        funcKey.key = "Heating";
        funcKey.name = m_RV_1_HeatingRod.sensorName;
        funcKey.sequence = *iter;
        QString userInput = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_RV_1_HeatingRod.devName, funcKey, "UserInput");
        if ("false" == userInput)
        {
            m_RV_1_HeatingRod.UserInputFlagList.insert(*iter, false);
        }
        else
        {
            m_RV_1_HeatingRod.UserInputFlagList.insert(*iter, true);
        }
    }

    m_RV_2_Outlet.devName = "Rotary Valve";
    m_RV_2_Outlet.sensorName = "RV_2_Outlet";
    m_RV_2_Outlet.heatingStartTime = 0;
    m_RV_2_Outlet.curModuleId = "";
    m_RV_2_Outlet.OTCheckPassed = false;
    m_RV_2_Outlet.needCheckOT = false;
    m_RV_2_Outlet.needCheckOTModuleId = "";
    m_RV_2_Outlet.HeatingOverTime = 0;
    sequenceList = {"1", "2", "3", "4"};

    iter = sequenceList.begin();
    for (; iter!=sequenceList.end(); ++iter)
    {
        FunctionModule funcModule;
        funcModule.Id = *iter;
        DataManager::FunctionKey_t funcKey;
        funcKey.key = "Heating";
        funcKey.name = m_RV_2_Outlet.sensorName;

        // Firstly, get the scenario list
        funcKey.sequence = *iter;
        QString strScenarioList = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_RV_2_Outlet.devName, funcKey, "ScenarioList");
        QStringList  strList = strScenarioList.split(",");
        QStringList::const_iterator strIter = strList.begin();
        qint32 scenario = 0;
        bool ok = false;
        for (; strIter != strList.end(); ++strIter)
        {
            scenario = (*strIter).toInt(&ok);
            if (false == ok)
            {
                return false;
            }
            funcModule.ScenarioList.push_back(scenario);
        }

       qreal maxTemperature = mp_DataManager->GetProgramSettings()->GetParameterValue(m_RV_2_Outlet.devName, funcKey, "MaxTemperature", ok);
        if (false == ok)
        {
            return false;
        }
        funcModule.MaxTemperature = maxTemperature;

        qreal heatingOverTime = mp_DataManager->GetProgramSettings()->GetParameterValue(m_RV_2_Outlet.devName, funcKey, "HeatingOverTime", ok);
        if (false == ok)
        {
            return false;
        }
        if (heatingOverTime > 0){
            m_RV_2_Outlet.needCheckOTModuleId = *iter;
            m_RV_2_Outlet.HeatingOverTime = heatingOverTime;
        }

        m_RV_2_Outlet.functionModuleList.insert(*iter, funcModule);
    }

    //For LA RV Tube
    m_LARVTube.devName = "LA";
    m_LARVTube.sensorName = "RVTube";
    m_LARVTube.heatingStartTime = 0;
    m_LARVTube.curModuleId = "";
    m_LARVTube.OTCheckPassed = false;
    sequenceList = {"1"};
    if (false == this->ConstructHeatingSensor(m_LARVTube, sequenceList))
    {
        return false;
    }
    //Add other attributes for LA RV Tube 
    iter = sequenceList.begin();
    for (; iter != sequenceList.end(); ++iter)
    {
       //For checking if user input temperature is needed
        DataManager::FunctionKey_t funcKey;
        funcKey.key = "Heating";
        funcKey.name = m_LARVTube.sensorName;
        funcKey.sequence = *iter;
        QString strOTList = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_LARVTube.devName, funcKey, "OTScenarioList");
        QStringList  strList = strOTList.split(",");
        QStringList::const_iterator strIter = strList.begin();
        qint32 scenario = 0;
        bool ok = false;
        QVector<qint32> OTCheckList;
        for (; strIter != strList.end(); ++strIter)
        {
            scenario = (*strIter).toInt(&ok);
            if (false == ok)
            {
                return false;
            }
            OTCheckList.push_back(scenario);
        }
        m_LARVTube.OTCheckScenarioList.insert(*iter, OTCheckList);
    }

    //For LA Wax Trap
    m_LAWaxTrap.devName = "LA";
    m_LAWaxTrap.sensorName = "WaxTrap";
    m_LAWaxTrap.heatingStartTime = 0;
    m_LAWaxTrap.curModuleId = "";
    m_LAWaxTrap.OTCheckPassed = false;
    sequenceList = {"1"};
    if (false == this->ConstructHeatingSensor(m_LAWaxTrap, sequenceList))
    {
        return false;
    }

    //Add other attributes for LA Wax Trap
    iter = sequenceList.begin();
    for (; iter != sequenceList.end(); ++iter)
    {
       //For checking if user input temperature is needed
        DataManager::FunctionKey_t funcKey;
        funcKey.key = "Heating";
        funcKey.name = m_LAWaxTrap.sensorName;
        funcKey.sequence = *iter;
        QString strOTList = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_LAWaxTrap.devName, funcKey, "OTScenarioList");
        QStringList  strList = strOTList.split(",");
        QStringList::const_iterator strIter = strList.begin();
        qint32 scenario = 0;
        bool ok = false;
        QVector<qint32> OTCheckList;
        for (; strIter != strList.end(); ++strIter)
        {
            scenario = (*strIter).toInt(&ok);
            if (false == ok)
            {
                return false;
            }
            OTCheckList.push_back(scenario);
        }
        m_LAWaxTrap.OTCheckScenarioList.insert(*iter, OTCheckList);
    }

    return true;
}

bool HeatingStrategy::CheckSensorHeatingOverTime(HeatingSensor& heatingSensor, qreal HWTemp)
{
    if (true == heatingSensor.OTCheckPassed)
    {
        return true;
    }

    if (HWTemp >= heatingSensor.functionModuleList[heatingSensor.curModuleId].OTTargetTemperature)
    {
        heatingSensor.OTCheckPassed = true;
    }
    //heating over time less than ,it need check heating over time
    if(heatingSensor.functionModuleList[heatingSensor.curModuleId].HeatingOverTime < 0)
    {
        return true;
    }
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (false== heatingSensor.curModuleId.isEmpty() &&
            now-heatingSensor.heatingStartTime >= heatingSensor.functionModuleList[heatingSensor.curModuleId].HeatingOverTime*1000)
    {
        //For Scenarios NON-related sensors(Oven and LA)
        if (1 == heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.size()
                && 0 == heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.at(0))
        {
            if (HWTemp < heatingSensor.functionModuleList[heatingSensor.curModuleId].OTTargetTemperature)
            {
                mp_SchedulerController->LogDebug(QString("For Scenarios NON-related sensors heating overtime, the current temp:%1").arg(HWTemp));
                return false;
            }
        }
        //For Scenarios related Sensors (Retort and Rotary valve)
        if (-1 != heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.indexOf(m_CurScenario))
        {
            if (HWTemp < heatingSensor.functionModuleList[heatingSensor.curModuleId].OTTargetTemperature)
            {
                mp_SchedulerController->LogDebug(QString("For Scenarios related Sensors heating overtime, the current temp:%1").arg(HWTemp));
                return false;
            }
        }
    }
    return true;
}

bool HeatingStrategy::CheckRVOutletHeatingOverTime(qreal HWTemp)
{
    if (true == m_RV_2_Outlet.OTCheckPassed)
    {
        return true;
    }
    if (HWTemp >= mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath())
    {
        m_RV_2_Outlet.OTCheckPassed = true;
    }
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if ( (true == m_RV_2_Outlet.needCheckOT) && (now - m_RV_2_Outlet.heatingStartTime >= m_RV_2_Outlet.HeatingOverTime*1000) )
    {
        if (-1 != m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.needCheckOTModuleId].ScenarioList.indexOf(m_CurScenario))
        {
        if(-1 != m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.needCheckOTModuleId].ScenarioList.indexOf(m_CurScenario) )
        {
            if (HWTemp < mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath())
            {
                int retTmp = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
                mp_SchedulerController->LogDebug(QString("RVoutlet heat over time, the current temp:%1, but the hope temp:%2").arg(HWTemp).arg(retTmp));
                return false;
            }
            }
        }
    }
    return true;
}

bool HeatingStrategy::ConstructHeatingSensor(HeatingSensor& heatingSensor, const QStringList& sequenceList)
{
   DataManager::FunctionKey_t funcKey;
   funcKey.key = "Heating";
   funcKey.name = heatingSensor.sensorName;

   QStringList::const_iterator seqIter = sequenceList.begin();
   for (; seqIter!=sequenceList.end(); ++seqIter)
   {
       FunctionModule funcModule;
       funcModule.Id = *seqIter;

       // Firstly, get the scenario list
       funcKey.sequence = *seqIter;
       QString strScenarioList = mp_DataManager->GetProgramSettings()->GetParameterStrValue(heatingSensor.devName, funcKey, "ScenarioList");
       QStringList  strList = strScenarioList.split(",");
       QStringList::const_iterator strIter = strList.begin();
       qint32 scenario = 0;
       bool ok = false;
       for (; strIter != strList.end(); ++strIter)
       {
           scenario = (*strIter).toInt(&ok);
           if (false == ok)
           {
               return false;
           }
           funcModule.ScenarioList.push_back(scenario);
       }

       // Get the others
       qreal temperatureOffset = mp_DataManager->GetProgramSettings()->GetParameterValue(heatingSensor.devName, funcKey, "NominalTemperature", ok);
       if (false == ok)
       {
           return false;
       }
       funcModule.TemperatureOffset = temperatureOffset;

       qreal maxTemperature= mp_DataManager->GetProgramSettings()->GetParameterValue(heatingSensor.devName, funcKey, "MaxTemperature", ok);
       if (false == ok)
       {
           return false;
       }
       funcModule.MaxTemperature = maxTemperature;

       qreal heatingOverTime= mp_DataManager->GetProgramSettings()->GetParameterValue(heatingSensor.devName, funcKey, "HeatingOverTime", ok);
       if (false == ok)
       {
           return false;
       }
       funcModule.HeatingOverTime = heatingOverTime;

       qreal slopTempChange= mp_DataManager->GetProgramSettings()->GetParameterValue(heatingSensor.devName, funcKey, "SlopeTempChange", ok);
       if (false == ok)
       {
           return false;
       }
       funcModule.SlopTempChange = slopTempChange;

       qreal controllerGain= mp_DataManager->GetProgramSettings()->GetParameterValue(heatingSensor.devName, funcKey, "ControllerGain", ok);
       if (false == ok)
       {
           return false;
       }
       funcModule.ControllerGain = controllerGain;

       qreal resetTime= mp_DataManager->GetProgramSettings()->GetParameterValue(heatingSensor.devName, funcKey, "ResetTime", ok);
       if (false == ok)
       {
           return false;
       }
       funcModule.ResetTime = resetTime;

       qreal derivativeTime= mp_DataManager->GetProgramSettings()->GetParameterValue(heatingSensor.devName, funcKey, "DerivativeTime", ok);
       if (false == ok)
       {
           return false;
       }
       funcModule.DerivativeTime = derivativeTime;
       funcModule.OTTargetTemperature = 0.0; //Initialize target temperature to zero

       heatingSensor.functionModuleList.insert(*seqIter, funcModule);
   }

   return true;
}

void HeatingStrategy::OnReportLevelSensorStatus1()
{
    if (m_RTLevelSensor.curModuleId.isEmpty())
    {
        return;
    }

    CmdALSetTempCtrlOFF* pHeatingCmd = new CmdALSetTempCtrlOFF(500, mp_SchedulerController);
    pHeatingCmd->Settype(AL_LEVELSENSOR);
    mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);

    SchedulerCommandShPtr_t pResHeatingCmd;
    mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd, pHeatingCmd->GetName());
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    pResHeatingCmd->GetResult(retCode);
}

bool HeatingStrategy::isEffectiveTemp(qreal HWTemp)
{
    if (UNDEFINED_VALUE == HWTemp || UNDEFINED_1_BYTE == HWTemp || UNDEFINED_2_BYTE == HWTemp || UNDEFINED_4_BYTE == HWTemp || HWTemp >= 298)
    {
        return false;
    }
    return true;
}

bool HeatingStrategy::CheckSensorTempOverRange(const QString& HeatingName, qreal temperature)
{
    if ("LevelSensor" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_RTLevelSensor, temperature))
        {
            return false;
        }
    }
    else if ("RTSide" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_RTTop, temperature))
        {
            return false;
        }
    }
    else if ("RTBottom" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_RTBottom, temperature))
        {
            return false;
        }
    }
    else if ("OvenTop" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_OvenTop, temperature))
        {
            return false;
        }
    }
    else if ("OvenBottom" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_OvenBottom, temperature))
        {
            return false;
        }
    }
    else if ("RVRod" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_RV_1_HeatingRod, temperature))
        {
            return false;
        }
    }
    else if ("RVOutle" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_RV_2_Outlet, temperature))
        {
            return false;
        }
    }
    else if ("LA_Tube1" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_LARVTube, temperature))
        {
            return false;
        }
    }
    else if ("LA_Tube2" == HeatingName)
    {
        if (false == this->CheckSensorCurrentTemperature(m_LAWaxTrap, temperature))
        {
            return false;
        }
    }
    return true;
}

}// end of namespace Scheduler
