/****************************************************************************/
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
*/
/****************************************************************************/
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/SchedulerEventCodes.h"
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

/*lint -e578 */
/*lint -e524 */
/*lint -e429 */
/*lint -e423 */
/*lint -e613 */
/*lint -e506 */

namespace Scheduler{
/****************************************************************************/
 /*!
 *  \brief  delay a period
 *  \param  the time to be delayed in seconds - int
 *  \return from DelaySomeTime
 */
/****************************************************************************/
static void DelaySomeTime(int DelayTime)
{
    if(DelayTime > 0)
    {
        QEventLoop event;
        QTimer::singleShot(DelayTime, &event, SLOT(quit()));
        event.exec();
    }
}

HeatingStrategy::HeatingStrategy(SchedulerMainThreadController* schedController,
                                SchedulerCommandProcessorBase* SchedCmdProcessor,
                                DataManager::CDataManager* DataManager)
                                :mp_SchedulerController(schedController),
                                mp_SchedulerCommandProcessor(SchedCmdProcessor),
                                mp_DataManager(DataManager),
                                m_OvenSensorsCheck(std::string("000000"))
{
    CONNECTSIGNALSLOT(mp_SchedulerCommandProcessor, ReportLevelSensorStatus1(), this, OnReportLevelSensorStatus1());
    m_CurScenario = 0;
    m_CmdResult = true;
    m_DiasbleOvenHeatingError = Global::Workaroundchecking("OVEN_HEATING");
    if (!this->ConstructHeatingSensorList())
    {
        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_STRATEGY_INITIALIZE_FAILED);
    }
    memset(&m_SensorsChecking, 0, sizeof(m_SensorsChecking));
    m_SensorsChecking.firstBottle = true;
    m_RTBottomCurrentTemp = 1.0;
    m_RTSideCurrentTemp = 1.0;
    m_RVHeatingRodCurrentTemp = 1.0;
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
#if 0
    if (false == this->CheckSensorCurrentTemperature(m_RTLevelSensor, strctHWMonitor.TempALLevelSensor))
#endif
    if (strctHWMonitor.TempALLevelSensor > 180.0 && isEffectiveTemp(strctHWMonitor.TempALLevelSensor))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE,0);
            return DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE,val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE,0);
    }
    //For Retort Bottom1 Sensor
    if (false == this->CheckSensorCurrentTemperature(m_RTBottom, strctHWMonitor.TempRTBottom1))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE, 0);
            return DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE, val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE, 0);
    }
    //For Retort Bottom2 Sensor
    if (false == this->CheckSensorCurrentTemperature(m_RTBottom, strctHWMonitor.TempRTBottom2))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE,0);
            return DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE,val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE,0);
    }
    //For Retort Top Sensor
    if (false == this->CheckSensorCurrentTemperature(m_RTTop, strctHWMonitor.TempRTSide))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE,0);
            return DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE,val);
        }

    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE,0);
    }
    //For Oven Top
    if (false == this->CheckSensorCurrentTemperature(m_OvenTop, strctHWMonitor.TempOvenTop))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE,0);
            return DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE,val);
        }
    }
    else
    {
        if (strctHWMonitor.TempOvenTop >= 40)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE,0);
        }
    }
    //For Oven Bottom1
    if (false == this->CheckSensorCurrentTemperature(m_OvenBottom, strctHWMonitor.TempOvenBottom1))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE,0);
            return DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE,val);
        }
    }
    else
    {
        if (strctHWMonitor.TempOvenBottom1 >=40)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE,0);
        }
    }
    //For Oven Bottom2
    if (false == this->CheckSensorCurrentTemperature(m_OvenBottom, strctHWMonitor.TempOvenBottom2))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE,0);
            return DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE,val);
        }
    }
    else
    {
        if (strctHWMonitor.TempOvenBottom2 >=40)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE,0);
        }
    }
    //For Rotary Valve Rod
    if (false == this->CheckSensorCurrentTemperature(m_RV_1_HeatingRod, strctHWMonitor.TempRV1))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE,0);
            return DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE,val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE,0);
    }
    //For Rotary Valve outlet
    if (false == this->CheckSensorCurrentTemperature(m_RV_2_Outlet, strctHWMonitor.TempRV2))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE,0);
            return DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE,val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE,0);
    }

    //For LA RV Tube
    if (false == this->CheckSensorCurrentTemperature(m_LARVTube, strctHWMonitor.TempALTube1))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE,0);
            return DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE,val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE,0);
    }

    //For LA Wax Trap
    if (false == this->CheckSensorCurrentTemperature(m_LAWaxTrap, strctHWMonitor.TempALTube2))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE,0);
            return DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE,val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE,0);
    }

    if (scenario != m_CurScenario)
    {
        m_CurScenario = scenario;
        m_RTLevelSensor.SetTemp4High = false; //for each scenario, set the initial value is false
        m_RTLevelSensor.SetTemp4Low = false;  //for each scenario, set the initial value is false

        if(4 == m_CurScenario && "RG6" != mp_SchedulerController->GetLastReagentGroup())
        {
            retCode = StopTemperatureControl("RTSide");
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                return retCode;
            }
            retCode = StopTemperatureControl("RTBottom");
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                return retCode;
            }
        }
        else
        {
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

    if ( !QFile::exists("TEST_RETORT") )
    {
        if (false == m_RTBottom.curModuleId.isEmpty() && -1!= m_RTBottom.functionModuleList[m_RTBottom.curModuleId].ScenarioList.indexOf(m_CurScenario))
        {
            if(!CheckRTBottomsDifference(strctHWMonitor.TempRTBottom1, strctHWMonitor.TempRTBottom2))
            {
                quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED);
                val++;
                if (val>=3)
                {
                    mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED,0);
                    return DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED;
                }
                else
                {
                    mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED,val);
                }
            }
            else
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED,0);
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
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME, 0);
            return DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME,val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME, 0);
    }
    // For Retort Top and Retort Bottom
    if (false == this->CheckSensorHeatingOverTime(m_RTTop, strctHWMonitor.TempRTSide) ||
            false == this->CheckSensorHeatingOverTime(m_RTBottom, strctHWMonitor.TempRTBottom1))
    {
        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RETORT_HEATING_OVERTIME);
        val++;
        if (val>=3)
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_HEATING_OVERTIME, 0);
            return DCL_ERR_DEV_RETORT_HEATING_OVERTIME;
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_HEATING_OVERTIME,val);
        }
    }
    else
    {
        mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RETORT_HEATING_OVERTIME,0);
    }

    //For Oven Top
    retCode = CheckOvenHeatingOverTime(m_OvenTop, strctHWMonitor.TempOvenTop, OVEN_TOP_SENSOR);
    if( DCL_ERR_FCT_CALL_SUCCESS != retCode)
    {
        return retCode;
    }
    //For Oven Bottom1
    retCode = CheckOvenHeatingOverTime(m_OvenBottom, strctHWMonitor.TempOvenBottom1, OVEN_BOTTOM1_SENSOR);
    if( DCL_ERR_FCT_CALL_SUCCESS != retCode)
    {
        return retCode;
    }
    //For oven Bottom2
    retCode = CheckOvenHeatingOverTime(m_OvenBottom, strctHWMonitor.TempOvenBottom2, OVEN_BOTTOM2_SENSOR);
    if( DCL_ERR_FCT_CALL_SUCCESS != retCode)
    {
        return retCode;
    }

    if(3 == m_CurScenario)
    {
        if (false == this->CheckRVOutletHeatingOverTime(strctHWMonitor.TempRV2))
        {
            quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_40DEGREEC_OVERTIME);
            val++;
            if (val>=3)
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_40DEGREEC_OVERTIME,0);
                return DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_40DEGREEC_OVERTIME;
            }
            else
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_40DEGREEC_OVERTIME,val);
            }
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_40DEGREEC_OVERTIME,0);
        }
     }

    // We only check LA heating overtime in scenario 260
    if (260 == m_CurScenario)
    {
        //For RV Outlet, Please note RV Rod(sensor 1) is NOT needed to check Heating overtime.
        if (false == this->CheckRVOutletHeatingOverTime(strctHWMonitor.TempRV2))
        {
            quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_NOTREACHTARGET);
            val++;
            if (val>=3)
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_NOTREACHTARGET,0);
                return DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_NOTREACHTARGET;
            }
            else
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_NOTREACHTARGET,val);
            }
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_NOTREACHTARGET,0);
        }
        //For LA RVTube
        if (false == this->CheckSensorHeatingOverTime(m_LARVTube, strctHWMonitor.TempALTube1))
        {
            quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP);
            val++;
            if (val>=3)
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP,0);
                return DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP;
            }
            else
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP,val);
            }
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP,0);
        }
#if 0
        //For LA WaxTrap
        if (false == this->CheckSensorHeatingOverTime(m_LAWaxTrap, strctHWMonitor.TempALTube2))
        {
            return DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_NOTREACHTARGETTEMP;
        }
#endif
    }

    if (m_CurScenario >= 271 && m_CurScenario <=277)
    {
        //Parrifin melting point (user input)
        qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        if (strctHWMonitor.TempALTube1 < (userInputMeltingPoint -1) || qFuzzyCompare(strctHWMonitor.TempALTube1,299))
        {
            quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL);
            val++;
            if (val>=3)
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL,0);
                return DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL;
            }
            else
            {
                mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL,val);
            }
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL,0);
        }

        if (strctHWMonitor.TempALTube2 < (userInputMeltingPoint-1) || qFuzzyCompare(strctHWMonitor.TempALTube2,299))
        {
            //Based on discussion on Auguest 13rd, 2015, we will report error only when temperature doesn't reach target AND heater status is off
            if (false == strctHWMonitor.LATube2HeatingStatus)
            {
                quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL);
                val++;
                if (val>=3)
                {
                    mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL,0);
                    return DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL;
                }
                else
                {
                    mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL,val);
                }
            }
        }
        else
        {
            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL,0);
        }
    }

    return DCL_ERR_FCT_CALL_SUCCESS;
}

ReturnCode_t HeatingStrategy::StartTemperatureControlForPreTest(const QString& HeaterName)
{
    CmdSchedulerCommandBase* pHeatingCmd = NULL;

    if ("RTSide" == HeaterName)
    {
        m_RTTop.curModuleId = "1";
        qreal userInputTemp = 0;
        if(mp_SchedulerController->GetCurProgramStepIndex() >= 0 )
        {
            userInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
            ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
        }
        if(userInputTemp < 0)
        {
            (void)this->StopTemperatureControl("RTSide");
            (void)this->StopTemperatureControl("RTBottom");
            return DCL_ERR_FCT_CALL_SUCCESS; // for ambient
        }
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
        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_RETORTSIDE_PRETEST, QStringList()<<QString("[%1]").arg(m_RTTop.functionModuleList[m_RTTop.curModuleId].TemperatureOffset+userInputTemp));
    }
    if ("RTBottom" == HeaterName)
    {
        m_RTBottom.curModuleId = "1";
        qreal userInputTemp = 0;
        if(mp_SchedulerController->GetCurProgramStepIndex() >= 0 )
        {
            userInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
            ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
        }
        if(userInputTemp < 0)
        {
            (void)this->StopTemperatureControl("RTSide");
            (void)this->StopTemperatureControl("RTBottom");
            return DCL_ERR_FCT_CALL_SUCCESS; // for ambient
        }
        pHeatingCmd  = new CmdRTStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(RT_BOTTOM);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].TemperatureOffset+userInputTemp);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].SlopTempChange);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].MaxTemperature);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].ControllerGain);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].ResetTime);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].DerivativeTime);
        m_RTBottom.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RTBottom.OTCheckPassed = false;
        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_RETORTBOT_PRETEST, QStringList()<<QString("[%1]").arg(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].TemperatureOffset+userInputTemp));
    }

    mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);
    return DCL_ERR_FCT_CALL_SUCCESS;
}

void HeatingStrategy::InitSelfTest()
{
    m_RTLevelSensor.curModuleId = "21";
    m_RTTop.curModuleId = "1";
    m_RTBottom.curModuleId = "1";
    m_OvenTop.curModuleId = "1";
    m_OvenBottom.curModuleId = "1";
    m_RV_1_HeatingRod.curModuleId = "3";
    m_RV_2_Outlet.curModuleId = "3";
    m_LARVTube.curModuleId = "1";
    m_LAWaxTrap.curModuleId = "1";
    m_CurScenario = 2;
}

void HeatingStrategy::UpdateOvenParamAt110V()
{
    HardwareMonitor_t strctHWMonitor = mp_SchedulerCommandProcessor->HardwareMonitor();
    qreal curTemp = strctHWMonitor.TempOvenBottom1;
    for (int i=0; i<2; i++)
    {
        DelaySomeTime(500);
        HardwareMonitor_t strctHWMonitor = mp_SchedulerCommandProcessor->HardwareMonitor();
        curTemp += strctHWMonitor.TempOvenBottom1;
    }
    curTemp = curTemp/3;
    if (curTemp >= 25.0)
    {
        return;
    }

    // At the former 9 hours AND melting point is less than 64
    m_OvenTop.functionModuleList["1"].TemperatureOffset +=5;
    m_OvenTop.functionModuleList["1"].TemperatureOffsetWithParaffin +=5;
    m_OvenTop.TimeRangeList["1"].second+=3600;
    m_OvenTop.TimeRangeList["2"].first+=3600;

    m_OvenBottom.functionModuleList["1"].TemperatureOffset +=5;
    m_OvenBottom.functionModuleList["1"].TemperatureOffsetWithParaffin +=5;
    m_OvenBottom.TimeRangeList["1"].second+=3600;
    m_OvenBottom.TimeRangeList["2"].first+=3600;

}

ReturnCode_t HeatingStrategy::StartTemperatureControlForSelfTest(const QString& HeaterName, bool NotSureTemperature)
{
    CmdSchedulerCommandBase* pHeatingCmd = NULL;
    if ("LevelSensor" == HeaterName)
    {
        m_RTLevelSensor.curModuleId = "21";
        pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(AL_LEVELSENSOR);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(95);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(10);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(112);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(120);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(1212);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(80);
    }
    if ("RTSide" == HeaterName)
    {
        m_RTTop.curModuleId = "1";
        m_RTSideCurrentTemp = 80.0 - 7;  // Offset: 7 degree C, from programsettings file
        pHeatingCmd  = new CmdRTStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(RT_SIDE);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(80);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(0);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(112);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(6000);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(1000);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(80);
    }
    if ("RTBottom" == HeaterName)
    {
        m_RTBottom.curModuleId = "1";
        qreal TempRTBottom = 80;
        if(NotSureTemperature)
        {
            TempRTBottom = mp_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom1;
            if(isEffectiveTemp(TempRTBottom))
            {
                if(TempRTBottom < m_RTBottom.functionModuleList[m_RTBottom.curModuleId].MaxTemperature - 25)
                {
                    TempRTBottom += 20;
                }
                else
                {
                    TempRTBottom = m_RTBottom.functionModuleList[m_RTBottom.curModuleId].MaxTemperature - 5;
                }
            }
        }
        m_RTBottomCurrentTemp = TempRTBottom - 2;  // Offset: 2 degree C, from programsettings file
        pHeatingCmd  = new CmdRTStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(RT_BOTTOM);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(TempRTBottom);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(0);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(107);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(6000);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(30000);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(0);
    }
    if ("OvenTop" == HeaterName)
    {
        m_OvenTop.curModuleId = "1";
        pHeatingCmd  = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(OVEN_TOP);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(80);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(0);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(100);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(6000);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(1000);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(80);
    }
    if ("OvenBottom" == HeaterName)
    {
        m_OvenBottom.curModuleId = "1";
        pHeatingCmd  = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(OVEN_BOTTOM);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(80);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(0);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(100);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(6000);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(1000);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(80);
    }
    if ("RV" == HeaterName)
    {
        m_RV_1_HeatingRod.curModuleId = "3";
        m_RV_2_Outlet.curModuleId = "3";
        qreal TempRV1 = 80;
        if(NotSureTemperature)
        {
            TempRV1 = mp_SchedulerCommandProcessor->HardwareMonitor().TempRV1;
            if(isEffectiveTemp(TempRV1))
            {
                if(TempRV1 < m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].MaxTemperature - 25)
                {
                    TempRV1 += 20;
                }
                else
                {
                    TempRV1 = m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].MaxTemperature - 5;
                }
            }
        }
        m_RVHeatingRodCurrentTemp = TempRV1;
        pHeatingCmd  = new CmdRVStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(TempRV1);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(0);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(130);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(1212);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(1000);
        dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(80);
    }
    if ("LA_Tube1" == HeaterName)
    {
        m_LARVTube.curModuleId = "1";
        pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(AL_TUBE1);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(80);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(0);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(100);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(6000);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(1000);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(80);
    }
    if ("LA_Tube2" == HeaterName)
    {
        m_LAWaxTrap.curModuleId = "1";
        pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(AL_TUBE2);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(80);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(0);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(100);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(6000);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(1000);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(80);
    }
    mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

    return DCL_ERR_FCT_CALL_SUCCESS;
}

ReturnCode_t HeatingStrategy::StartTemperatureControlForPowerFailure(const QString& HeaterName)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    if ("RTSide" == HeaterName)
    {
        retCode = StartRTTemperatureControl(m_RTTop, RT_SIDE);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
    }
    if ("RTBottom" == HeaterName)
    {
        retCode = StartRTTemperatureControl(m_RTBottom, RT_BOTTOM);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
    }
    if ("OvenTop" == HeaterName)
    {
        retCode = StartOvenTemperatureControl(m_OvenTop, OVEN_TOP);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
    }
    if ("OvenBottom" == HeaterName)
    {
        retCode = StartOvenTemperatureControl(m_OvenBottom, OVEN_BOTTOM);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
    }
    if ("RV" == HeaterName)
    {
        retCode = StartRVTemperatureControl(m_RV_1_HeatingRod);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }

        //For RVOutlet
        StartRVOutletHeatingOTCalculation();
    }
    if ("LA_Tube1" == HeaterName)
    {
        retCode = StartLATemperatureControl(m_LARVTube, AL_TUBE1);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
    }
    if ("LA_Tube2" == HeaterName)
    {
        retCode = StartLATemperatureControl(m_LAWaxTrap, AL_TUBE2);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            return retCode;
        }
    }
    return retCode;
}

ReturnCode_t HeatingStrategy::StartTemperatureControl(const QString& HeaterName)
{
    CmdSchedulerCommandBase* pHeatingCmd = NULL;
    if ("LevelSensor" == HeaterName)
    {
        // For Level sensor has two types of PIDs, we need check current temperature at first to decide which PID we need use
        QString highSeq, lowSeq;
        if (m_RTLevelSensor.curModuleId.right(1) == "1")
        {
            highSeq = m_RTLevelSensor.curModuleId;
            lowSeq = m_RTLevelSensor.curModuleId.left(1)+"2";
        }
        else
        {
            highSeq = m_RTLevelSensor.curModuleId.left(1)+"1";
            lowSeq = m_RTLevelSensor.curModuleId;
        }
        m_RTLevelSensor.SetTemp4High = false;
        m_RTLevelSensor.SetTemp4Low = false;
        qreal tmpLevelSensor = mp_SchedulerCommandProcessor->HardwareMonitor().TempALLevelSensor;
        quint8 counter = 0; // At most 10 times;
        while (!isEffectiveTemp(tmpLevelSensor))
        {
            if (counter>=10)
            {
                return DCL_ERR_FCT_CALL_FAILED;
            }
            DelaySomeTime(500);
            tmpLevelSensor = mp_SchedulerCommandProcessor->HardwareMonitor().TempALLevelSensor;
            counter++;
        }

        if (m_RTLevelSensor.ExchangePIDTempList[m_RTLevelSensor.curModuleId] > tmpLevelSensor) // High seppd
        {
            m_RTLevelSensor.SetTemp4High = true;
            m_RTLevelSensor.curModuleId = highSeq;
        }
        else // low speed
        {
            m_RTLevelSensor.SetTemp4Low = true;
            m_RTLevelSensor.curModuleId = lowSeq;
        }
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
        m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].OTTargetTemperature = m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].TemperatureOffset -1;
    }
    if ("RTSide" == HeaterName)
    {
        qreal userInputTemp = 0;
        if(mp_SchedulerController->GetCurProgramStepIndex() >= 0 )
        {
            userInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
            ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
        }
        else if(2 == m_CurScenario)    // Self test
        {
            userInputTemp = m_RTSideCurrentTemp;
        }
        else if(203 == m_CurScenario)  // Dry step
        {
            userInputTemp = 80;
        }
        else if(4 == m_CurScenario && "RG6" == mp_SchedulerController->GetLastReagentGroup())  // Idle and last reagent was paraffin
        {
            userInputTemp = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        }

        if(userInputTemp < 0)
        {
            (void)this->StopTemperatureControl("RTSide");
            (void)this->StopTemperatureControl("RTBottom");
            return DCL_ERR_FCT_CALL_SUCCESS; // for ambient
        }
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
        qreal userInputTemp = 0;
        if(mp_SchedulerController->GetCurProgramStepIndex() >= 0 )
        {
            userInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
            ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
        }
        else if(2 == m_CurScenario)    // Self test
        {
            userInputTemp = m_RTBottomCurrentTemp;
        }
        else if(203 == m_CurScenario)  // Dry step
        {
            userInputTemp = 80;
        }
        else if(4 == m_CurScenario && "RG6" == mp_SchedulerController->GetLastReagentGroup())  // Idle and last reagent was paraffin
        {
            userInputTemp = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        }
        if(userInputTemp < 0)
        {
            (void)this->StopTemperatureControl("RTSide");
            (void)this->StopTemperatureControl("RTBottom");
            return DCL_ERR_FCT_CALL_SUCCESS; // for ambient
        }
        pHeatingCmd  = new CmdRTStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(RT_BOTTOM);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].TemperatureOffset+userInputTemp);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].SlopTempChange);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].MaxTemperature);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].ControllerGain);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].ResetTime);
        dynamic_cast<CmdRTStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_RTBottom.functionModuleList[m_RTBottom.curModuleId].DerivativeTime);
        m_RTBottom.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RTBottom.OTCheckPassed = false;
    }
    if ("OvenTop" == HeaterName)
    {
        //Firstly, get the Parrifin melting point (user input)
        qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        if(userInputMeltingPoint < 0)
        {
            userInputMeltingPoint = 0.0;
        }
        qreal tmpTemperatureOffset = 0.0;
        if ( 4 == m_CurScenario || (214 == m_CurScenario && mp_SchedulerController->HasParaffinReagent()) )
        {
            tmpTemperatureOffset = m_OvenTop.functionModuleList[m_OvenTop.curModuleId].TemperatureOffsetWithParaffin + userInputMeltingPoint;
        }
        else
        {
            tmpTemperatureOffset = m_OvenTop.functionModuleList[m_OvenTop.curModuleId].TemperatureOffset + userInputMeltingPoint;
        }
        pHeatingCmd  = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(OVEN_TOP);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(tmpTemperatureOffset);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].SlopTempChange);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].MaxTemperature);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].ControllerGain);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].ResetTime);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_OvenTop.functionModuleList[m_OvenTop.curModuleId].DerivativeTime);
    }
    if ("OvenBottom" == HeaterName)
    {
        //Firstly, get the Parrifin melting point (user input)
        qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        if(userInputMeltingPoint < 0)
        {
            userInputMeltingPoint = 0.0;
        }
        qreal tmpTemperatureOffset = 0.0;
        if ( 4 == m_CurScenario || (214 == m_CurScenario && mp_SchedulerController->HasParaffinReagent()) )
        {
            tmpTemperatureOffset = m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].TemperatureOffsetWithParaffin + userInputMeltingPoint;
        }
        else
        {
            tmpTemperatureOffset = m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].TemperatureOffset + userInputMeltingPoint;
        }
        pHeatingCmd  = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(OVEN_BOTTOM);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(tmpTemperatureOffset);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].SlopTempChange);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].MaxTemperature);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].ControllerGain);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].ResetTime);
        dynamic_cast<CmdOvenStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_OvenBottom.functionModuleList[m_OvenBottom.curModuleId].DerivativeTime);
    }
    if ("RV" == HeaterName)
    {
        if(2 == m_CurScenario)
        {
            dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RVHeatingRodCurrentTemp);
        }
        else
        {
            //Firstly, get the Parrifin melting point (user input)
            qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
            if(userInputMeltingPoint < 0)
            {
                userInputMeltingPoint = 0.0;
            }
            pHeatingCmd  = new CmdRVStartTemperatureControlWithPID(500, mp_SchedulerController);
            if (true == m_RV_1_HeatingRod.UserInputFlagList[m_RV_1_HeatingRod.curModuleId])
            {
                dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].TemperatureOffset+userInputMeltingPoint);
            }
            else
            {
                dynamic_cast<CmdRVStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RV_1_HeatingRod.functionModuleList[m_RV_1_HeatingRod.curModuleId].TemperatureOffset);
            }
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
    mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

    return DCL_ERR_FCT_CALL_SUCCESS;
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
        if(!m_OvenTop.curModuleId.isEmpty())
        {
            m_OvenTop.IsStartedHeating = false;
            m_OvenTop.heatingStartTime = 0;
        }
    }
    if ("OvenBottom" == HeaterName)
    {
        pHeatingCmd  = new CmdOvenSetTempCtrlOFF(500, mp_SchedulerController);
        dynamic_cast<CmdOvenSetTempCtrlOFF*>(pHeatingCmd)->Settype(OVEN_BOTTOM);
        if(!m_OvenBottom.curModuleId.isEmpty())
        {
            m_OvenBottom.IsStartedHeating = false;
            m_OvenBottom.heatingStartTime = 0;
        }
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
    mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

    return DCL_ERR_FCT_CALL_SUCCESS;
}

/*lint -e1023 */
bool HeatingStrategy:: CheckRTBottomsDifference(qreal temp1, qreal temp2)
{
    if ( isEffectiveTemp(temp1) && isEffectiveTemp(temp2))
    {
        if (std::abs(temp1 - temp2) >= m_RTBottom.TemperatureDiffList[m_RTBottom.curModuleId])
        {
            return false;
        }
    }
    return true;
}

quint16 HeatingStrategy::CheckTemperatureOverTime(const QString& HeaterName, qreal HWTemp)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (!isEffectiveTemp(HWTemp))
    {
        // we only check the invalid temperature in OT time range
        if ((now-m_RTLevelSensor.heatingStartTime) < m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].HeatingOverTime*1000)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    if ("LevelSensor" == HeaterName)
    {
#if 0
        if(HWTemp >= m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].MaxTemperature)
#endif
        if(HWTemp >= 180.0 && isEffectiveTemp(HWTemp))
        {
            return 1;
        }
        if ((now-m_RTLevelSensor.heatingStartTime) >= m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].HeatingOverTime*1000)
        {
            return 1; //failed
        }

        if (HWTemp >= m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].OTTargetTemperature
        #if 0
                && HWTemp < m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].MaxTemperature)
        #endif
                && HWTemp < 180.0)
        {
            return 2; //pass
        }
    }

    if (m_RTLevelSensor.CurrentSpeedList[m_RTLevelSensor.curModuleId] == "High" && false == m_RTLevelSensor.SetTemp4Low &&
           HWTemp >= m_RTLevelSensor.ExchangePIDTempList[m_RTLevelSensor.curModuleId])
    {
        m_RTLevelSensor.SetTemp4Low = true;
        m_RTLevelSensor.curModuleId = m_RTLevelSensor.curModuleId.left(1)+"2";
        CmdSchedulerCommandBase* pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetType(AL_LEVELSENSOR);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetNominalTemperature(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].TemperatureOffset);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetSlopeTempChange(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].SlopTempChange);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetMaxTemperature(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].MaxTemperature);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetControllerGain(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].ControllerGain);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetResetTime(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].ResetTime);
        dynamic_cast<CmdALStartTemperatureControlWithPID*>(pHeatingCmd)->SetDerivativeTime(m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].DerivativeTime);
        m_RTLevelSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RTLevelSensor.OTCheckPassed = false;
        m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].OTTargetTemperature = m_RTLevelSensor.functionModuleList[m_RTLevelSensor.curModuleId].TemperatureOffset -1;
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

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

    // Workaround for below sensors. In no-signal situation, we take it to be over range
    if (heatingSensor.sensorName == "LevelSensor" || heatingSensor.sensorName == "OvenTop"
            || heatingSensor.sensorName == "OvenBottom" || heatingSensor.sensorName == "RV_1_HeatingRod"
            || heatingSensor.sensorName == "RV_2_Outlet" || heatingSensor.sensorName == "RVTube"
            || heatingSensor.sensorName == "WaxTrap")
    {
        if (qFuzzyCompare(299,HWTemp))
        {
            return false;
        }

    }

    if ( !isEffectiveTemp(HWTemp) )
    {
        return true;
    }

    if ( heatingSensor.functionModuleList[heatingSensor.curModuleId].MaxTemperature <= HWTemp )
     {
        return false;
    }

    return true;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartLevelSensorTemperatureControl(const HardwareMonitor_t& strctHWMonitor)
{
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
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

        if ("High" == m_RTLevelSensor.CurrentSpeedList[iter->Id])
        {
            mp_SchedulerController->LogDebug("Level sensor PID changed to high");
        }
        else
        {
            mp_SchedulerController->LogDebug("Level sensor PID changed to low");
        }
        mp_SchedulerController->LogDebug(QString("Target temperature is: %1, SlopTemperature change is: %2, Max Temperature is: %3, Controller gain is: %4, Reset time is: %5, Derivative Time is: %6")
                                         .arg(iter->TemperatureOffset).arg(iter->SlopTempChange).arg(iter->MaxTemperature).arg(iter->ControllerGain).arg(iter->ResetTime).arg(iter->DerivativeTime));
        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_LEVEL_SENSOR, QStringList()<<QString("[%1]").arg(m_CurScenario)<<QString("[%1]").arg(iter->TemperatureOffset));
        m_RTLevelSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RTLevelSensor.curModuleId = iter->Id;
        m_RTLevelSensor.OTCheckPassed = false;
        iter->OTTargetTemperature = iter->TemperatureOffset -1;
    }

    return DCL_ERR_FCT_CALL_SUCCESS;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartRTTemperatureControl(HeatingSensor& heatingSensor, RTTempCtrlType_t RTType)
{
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
        // cleaning dry step
        else if(mp_SchedulerController->GetCurProgramStepIndex() == -1
                && false == mp_SchedulerController->GetCurProgramID().isEmpty()
                && mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())->IsCleaningProgram())
        {
            userInputTemp = 80;
        }
        else if(4 == m_CurScenario && "RG6" == mp_SchedulerController->GetLastReagentGroup())
        {
            userInputTemp = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        }
        else
        {
            return DCL_ERR_FCT_CALL_SUCCESS;
        }
        if (userInputTemp < 0) // In this case, the GUI setting is to be ambient
        {
            (void)this->StopTemperatureControl("RTSide");
            (void)this->StopTemperatureControl("RTBottom");
            //it should check the retort temperature over range
            heatingSensor.curModuleId = iter->Id;
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
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_RETORT, QStringList()<<QString("[%1]").arg(RTType)<<QString("[%1]").arg(m_CurScenario)<<QString("[%1]").arg(iter->TemperatureOffset+userInputTemp));
        heatingSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        heatingSensor.curModuleId = iter->Id;
        heatingSensor.OTCheckPassed = false;
        qreal meltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        iter->OTTargetTemperature =  meltingPoint -1;
        return DCL_ERR_FCT_CALL_SUCCESS;

    }

    // The current scenario is NOT related to Level Sensor's ones.
    return DCL_ERR_FCT_CALL_SUCCESS;
}

void HeatingStrategy::ResetTheOvenHeating()
{
    m_OvenTop.curModuleId = "";
    m_OvenTop.heatingStartTime = 0;
    m_OvenTop.IsStartedHeating = false;

    m_OvenBottom.curModuleId = "";
    m_OvenBottom.heatingStartTime = 0;
    m_OvenBottom.IsStartedHeating = false;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartOvenTemperatureControl(OvenSensor& heatingSensor, OVENTempCtrlType_t OvenType)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;

    //Firstly, get the Parrifin melting point (user input)
    int userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
    qint64 timeElapse = 0;
    qint64 remainTime = 0;

    QMap<QString, FunctionModule>::iterator iter = heatingSensor.functionModuleList.begin();
    for(; iter!=heatingSensor.functionModuleList.end(); ++iter)
    {
        timeElapse = 0;
        QPair<qint64,qint64> timeRange = heatingSensor.TimeRangeList[iter->Id];
        QPair<qreal,qreal> meltingRange = heatingSensor.ParaffinTempRangeList[iter->Id];

        qint64 now = QDateTime::currentMSecsSinceEpoch();

        if (50 <= userInputMeltingPoint && 64 >= userInputMeltingPoint)
        {
            if (0 == heatingSensor.heatingStartTime)
            {
                 remainTime = mp_SchedulerController->GetOvenHeatingRemainingTime();
                 timeElapse = 12 * 3600000 - remainTime;

                 if(timeElapse < 0)
                 {
                     timeElapse = 0;
                 }
                 else if(timeElapse > 12 * 3600000)
                 {
                     timeElapse = 12 * 3600000;
                 }
            }
            else
            {
                timeElapse = now - heatingSensor.heatingStartTime;
            }
        }
        else if (65 <= userInputMeltingPoint && 70 >= userInputMeltingPoint)
        {
            if (0 == heatingSensor.heatingStartTime)
            {
                 remainTime = mp_SchedulerController->GetOvenHeatingRemainingTime();
                 timeElapse = 15 * 3600000 - remainTime;
                 if(timeElapse < 0)
                 {
                     timeElapse = 0;
                 }
                 else if(timeElapse > 15 * 3600000)
                 {
                     timeElapse = 15 * 3600000;
                 }
            }
            else
            {
                timeElapse = now - heatingSensor.heatingStartTime;
            }
        }


        if (meltingRange.first <= userInputMeltingPoint && meltingRange.second >= userInputMeltingPoint
                && (timeRange.first * 1000) <= timeElapse && (timeRange.second * 1000) >= timeElapse)
        {
            break;
        }
    }

    // Found out the heating sensor's function module
    if (iter != heatingSensor.functionModuleList.end())
    {
        if(true == heatingSensor.IsStartedHeating && heatingSensor.curModuleId == iter->Id)
        {
            return retCode;
        }
        CmdOvenStartTemperatureControlWithPID* pHeatingCmd  = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        pHeatingCmd->SetType(OvenType);
        pHeatingCmd->SetNominalTemperature(iter->TemperatureOffset+userInputMeltingPoint);
        pHeatingCmd->SetSlopeTempChange(iter->SlopTempChange);
        pHeatingCmd->SetMaxTemperature(iter->MaxTemperature);
        pHeatingCmd->SetControllerGain(iter->ControllerGain);
        pHeatingCmd->SetResetTime(iter->ResetTime);
        pHeatingCmd->SetDerivativeTime(iter->DerivativeTime);
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_OVEN, QStringList()<<QString("[%1]").arg(OvenType)<<QString("[%1]").arg(m_CurScenario)
                                           <<QString("[%1]").arg(iter->TemperatureOffset+userInputMeltingPoint));
        heatingSensor.curModuleId = iter->Id;
        heatingSensor.IsStartedHeating = true;
        iter->OTTargetTemperature = heatingSensor.OTTempOffsetList[iter->Id]+userInputMeltingPoint;
        if(0 == heatingSensor.heatingStartTime)
        {
            heatingSensor.OvenBottom2OTCheckPassed = false;
            heatingSensor.OTCheckPassed = false;
            heatingSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch() - timeElapse;
        }

        return DCL_ERR_FCT_CALL_SUCCESS;

    }

    return DCL_ERR_FCT_CALL_SUCCESS;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartRVTemperatureControl(RVSensor& heatingSensor)
{
    //Firstly, get the Parrifin melting point (user input)
    qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
    if(userInputMeltingPoint < 0)
    {
        userInputMeltingPoint = 0.0;
    }

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
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_RV, QStringList()<<QString("[%1]").arg(m_CurScenario)<<QString("[%1]").arg(tempOffset));
        heatingSensor.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
        heatingSensor.curModuleId = iter->Id;
        heatingSensor.OTCheckPassed = false;
        return DCL_ERR_FCT_CALL_SUCCESS;
    }

    // The current scenario is NOT related to Level Sensor's ones.
    return DCL_ERR_FCT_CALL_SUCCESS;
}

DeviceControl::ReturnCode_t HeatingStrategy::StartLATemperatureControl(HeatingSensor& heatingSensor,ALTempCtrlType_t LAType)
{
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
        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);

        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_LA, QStringList()<<QString("[%1]").arg(LAType)<<QString("[%1]").arg(m_CurScenario)
                                           <<QString("[%1]").arg(iter->TemperatureOffset));
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

        iter->OTTargetTemperature = iter->TemperatureOffset -2;

        return DCL_ERR_FCT_CALL_SUCCESS;

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
            if(m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.curModuleId].HeatingOverTime > 0)
            {
                m_RV_2_Outlet.heatingStartTime = QDateTime::currentMSecsSinceEpoch();
                m_RV_2_Outlet.OTCheckPassed = false;
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
    QStringList sequenceList;
    sequenceList << "11" << "12" << "21" <<  "22";
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
        (void)m_RTLevelSensor.CurrentSpeedList.insert(*iter, curSpeed);
        bool ok = false;
        qreal exchangePIDtmp = mp_DataManager->GetProgramSettings()->GetParameterValue(m_RTLevelSensor.devName, funcKey, "ExchangePIDTemp", ok);
        if (false == ok)
        {
            return false;
        }
        (void)m_RTLevelSensor.ExchangePIDTempList.insert(*iter, exchangePIDtmp);
    }
    sequenceList.clear();

    // For Retort Top
    m_RTTop.devName = "Retort";
    m_RTTop.sensorName = "RTTop";
    m_RTTop.heatingStartTime = 0;
    m_RTTop.curModuleId = "";
    m_RTTop.OTCheckPassed = false;
    sequenceList << "1" << "2" << "3" << "4";
    if (false == this->ConstructHeatingSensor(m_RTTop, sequenceList))
    {
        return false;
    }
    sequenceList.clear();

    // For Retort Bottom
    m_RTBottom.devName = "Retort";
    m_RTBottom.sensorName = "RTBottom";
    m_RTBottom.heatingStartTime = 0;
    m_RTBottom.curModuleId = "";
    m_RTBottom.OTCheckPassed = false;
    sequenceList << "1" << "2" << "3" << "4";
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
        (void)m_RTBottom.TemperatureDiffList.insert(*iter, tempDiff);
    }
    sequenceList.clear();

    //For Oven Top
    m_OvenTop.devName = "Oven";
    m_OvenTop.sensorName = "OvenTop";
    m_OvenTop.IsStartedHeating = false;
    m_OvenTop.heatingStartTime = 0;
    m_OvenTop.curModuleId = "";
    m_OvenTop.OTCheckPassed = false;
    m_OvenTop.OvenBottom2OTCheckPassed = false;
    sequenceList << "1" << "2" << "3" << "4";
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

        qreal temperatureOffset = mp_DataManager->GetProgramSettings()->GetParameterValue(m_OvenTop.devName, funcKey, "NominalTemperatureWithParaffin", ok);
        if (false == ok)
        {
            return false;
        }
        m_OvenTop.functionModuleList[*iter].TemperatureOffsetWithParaffin = temperatureOffset;

        //OverTimeTempOffset
        qreal OTTempOffset = mp_DataManager->GetProgramSettings()->GetParameterValue(m_OvenTop.devName, funcKey, "OverTimeTempOffset", ok);
        if (false == ok)
        {
            return false;
        }
        (void)m_OvenTop.OTTempOffsetList.insert(*iter, OTTempOffset);

        //ParaffinTempRange
        QString paraffinTempRange = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_OvenTop.devName, funcKey, "ParaffinTempRange");
        QStringList  strList = paraffinTempRange.split(",");
        QPair<qreal,qreal> tempPair;
        tempPair.first = strList[0].toDouble(&ok);
        tempPair.second = strList[1].toDouble(&ok);
        (void)m_OvenTop.ParaffinTempRangeList.insert(*iter, tempPair);

        //TimeRange
        QString timeRange = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_OvenTop.devName, funcKey, "TimeRange");
        strList = timeRange.split(",");
        QPair<qint64, qint64> timePair;
        timePair.first = strList[0].toInt(&ok);
        timePair.second = strList[1].toInt(&ok);
        (void)m_OvenTop.TimeRangeList.insert(*iter,timePair);
    }
    sequenceList.clear();

    //For Oven Bottom
    m_OvenBottom.devName = "Oven";
    m_OvenBottom.sensorName = "OvenBottom";
    m_OvenBottom.IsStartedHeating = false;
    m_OvenBottom.heatingStartTime = 0;
    m_OvenBottom.curModuleId = "";
    m_OvenBottom.OTCheckPassed = false;
    m_OvenBottom.OvenBottom2OTCheckPassed = false;
    sequenceList << "1" << "2" << "3" << "4";
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

        qreal temperatureOffset = mp_DataManager->GetProgramSettings()->GetParameterValue(m_OvenBottom.devName, funcKey, "NominalTemperatureWithParaffin", ok);
        if (false == ok)
        {
            return false;
        }
        m_OvenBottom.functionModuleList[*iter].TemperatureOffsetWithParaffin = temperatureOffset;

        //OverTimeTempOffset
        qreal OTTempOffset = mp_DataManager->GetProgramSettings()->GetParameterValue(m_OvenBottom.devName, funcKey, "OverTimeTempOffset", ok);
        if (false == ok)
        {
            return false;
        }
        (void)m_OvenBottom.OTTempOffsetList.insert(*iter, OTTempOffset);

        //ParaffinTempRange
        QString paraffinTempRange = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_OvenBottom.devName, funcKey, "ParaffinTempRange");
        QStringList  strList = paraffinTempRange.split(",");
        QPair<qreal,qreal> tempPair;
        tempPair.first = strList[0].toDouble(&ok);
        tempPair.second = strList[1].toDouble(&ok);
        (void)m_OvenBottom.ParaffinTempRangeList.insert(*iter, tempPair);

        //TimeRange
        QString timeRange = mp_DataManager->GetProgramSettings()->GetParameterStrValue(m_OvenBottom.devName, funcKey, "TimeRange");
        strList = timeRange.split(",");
        QPair<qint64, qint64> timePair;
        timePair.first = strList[0].toInt(&ok);
        timePair.second = strList[1].toInt(&ok);
        (void)m_OvenBottom.TimeRangeList.insert(*iter,timePair);
    }
    sequenceList.clear();

    //For Rotary Valve Rod
    m_RV_1_HeatingRod.devName = "Rotary Valve";
    m_RV_1_HeatingRod.sensorName = "RV_1_HeatingRod";
    m_RV_1_HeatingRod.heatingStartTime = 0;
    m_RV_1_HeatingRod.curModuleId = "";
    m_RV_1_HeatingRod.OTCheckPassed = false;
    sequenceList << "1" << "2" << "3" << "4";
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
            (void)m_RV_1_HeatingRod.UserInputFlagList.insert(*iter, false);
        }
        else
        {
            (void)m_RV_1_HeatingRod.UserInputFlagList.insert(*iter, true);
        }
    }
    sequenceList.clear();

    m_RV_2_Outlet.devName = "Rotary Valve";
    m_RV_2_Outlet.sensorName = "RV_2_Outlet";
    m_RV_2_Outlet.heatingStartTime = 0;
    m_RV_2_Outlet.curModuleId = "";
    m_RV_2_Outlet.OTCheckPassed = false;
    sequenceList << "1" << "2" << "3" << "4";

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

        qreal heatingOverTime = 0.0;
        heatingOverTime = mp_DataManager->GetProgramSettings()->GetParameterValue(m_RV_2_Outlet.devName, funcKey, "HeatingOverTime", ok);
        if (false == ok)
        {
            return false;
        }
        funcModule.HeatingOverTime = heatingOverTime;

        (void)m_RV_2_Outlet.functionModuleList.insert(*iter, funcModule);
    }
    sequenceList.clear();

    //For LA RV Tube
    m_LARVTube.devName = "LA";
    m_LARVTube.sensorName = "RVTube";
    m_LARVTube.heatingStartTime = 0;
    m_LARVTube.curModuleId = "";
    m_LARVTube.OTCheckPassed = false;
    sequenceList << "1";
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
        (void)m_LARVTube.OTCheckScenarioList.insert(*iter, OTCheckList);
    }
    sequenceList.clear();

    //For LA Wax Trap
    m_LAWaxTrap.devName = "LA";
    m_LAWaxTrap.sensorName = "WaxTrap";
    m_LAWaxTrap.heatingStartTime = 0;
    m_LAWaxTrap.curModuleId = "";
    m_LAWaxTrap.OTCheckPassed = false;
    sequenceList << "1";
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
        (void)m_LAWaxTrap.OTCheckScenarioList.insert(*iter, OTCheckList);
    }
    sequenceList.clear();

    return true;
}

bool HeatingStrategy::CheckSensorHeatingOverTime(HeatingSensor& heatingSensor, qreal HWTemp)
{
    if (true == heatingSensor.OTCheckPassed)
    {
        return true;
    }

    if ((heatingSensor.functionModuleList[heatingSensor.curModuleId].OTTargetTemperature)<= HWTemp && isEffectiveTemp(HWTemp))
    {
        heatingSensor.OTCheckPassed = true;
    }
    //heating over time less than zero,it need NOT check heating over time
    if(heatingSensor.functionModuleList[heatingSensor.curModuleId].HeatingOverTime < 0)
    {
        return true;
    }
    /*lint -e1055 */
    /*lint -e628 */
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (false == heatingSensor.curModuleId.isEmpty() &&
            now-heatingSensor.heatingStartTime >= heatingSensor.functionModuleList[heatingSensor.curModuleId].HeatingOverTime*1000)
    {
        //For Scenarios NON-related sensors(Oven and LA)
        if (1 == heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.size()
                && 0 == heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.at(0))
        {
            if (HWTemp < heatingSensor.functionModuleList[heatingSensor.curModuleId].OTTargetTemperature)
            {
                return false;
            }
        }
        //For Scenarios related Sensors (Retort and Rotary valve)
        if (-1 != heatingSensor.functionModuleList[heatingSensor.curModuleId].ScenarioList.indexOf(m_CurScenario))
        {
            if (HWTemp < heatingSensor.functionModuleList[heatingSensor.curModuleId].OTTargetTemperature)
            {
                return false;
            }
        }
    }
    return true;
}

DeviceControl::ReturnCode_t HeatingStrategy::CheckOvenHeatingOverTime(OvenSensor& heatingSensor, qreal HWTemp, OVENSensorType_t OvenType)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    if(m_DiasbleOvenHeatingError)
    {
        return retCode;
    }
    if(!isEffectiveTemp(HWTemp))
    {
        return retCode;
    }

    if(!heatingSensor.curModuleId.isEmpty())
    {
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qint64 timeElapse = 0;
        if (0 != heatingSensor.heatingStartTime)
        {
            timeElapse = now - heatingSensor.heatingStartTime;
        }
        QPair<qint64,qint64> timeRange = heatingSensor.TimeRangeList[heatingSensor.curModuleId];

        if(0 == timeRange.first)
        {
            //the abnormal error was deleted
        }
        else
        {

            int userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
            if(timeElapse >= timeRange.second*1000)
            {
                //If temperature of Oven sensors is below to 40 degree, out of range error will be raised
                if (HWTemp < 40.0)
                {
                    switch(OvenType)
                    {
                    case OVEN_TOP_SENSOR:
                    {
                        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE);
                        val++;
                        if (val>=3)
                        {
                            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE,0);
                            return DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE;
                        }
                        else
                        {
                            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE,val);
                        }
                    }
                        break;
                    case OVEN_BOTTOM1_SENSOR:
                    {
                        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE);
                        val++;
                        if (val>=3)
                        {
                            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE,0);
                            return DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE;
                        }
                        else
                        {
                            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE,val);
                        }
                    }
                        break;
                    case OVEN_BOTTOM2_SENSOR:
                    {
                        quint8 val = mp_SchedulerController->GetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE);
                        val++;
                        if (val>=3)
                        {
                            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE,0);
                            return DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE;
                        }
                        else
                        {
                            mp_SchedulerController->SetRetCodeCounter(DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE,val);
                        }
                    }
                        break;
                    }
                }

                if (HWTemp > userInputMeltingPoint+12)
                {
                    switch(OvenType)
                    {
                        case OVEN_TOP_SENSOR:
                        if (!m_OvenSensorsCheck.test(0))
                        {
                            retCode = DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE_HIGH;
                            m_OvenSensorsCheck.set(0,true);
                        }
                        else
                        {
                            if (false == this->CheckSensorCurrentTemperature(m_OvenTop, HWTemp))
                            {
                                mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE_HIGH, m_CurScenario,true, false);
                                //m_OvenSensorsCheck.set(0,false);
                            }
                        }
                            break;
                        case OVEN_BOTTOM1_SENSOR:
                        if (!m_OvenSensorsCheck.test(1))
                        {
                            retCode = DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE_HIGH;
                            m_OvenSensorsCheck.set(1,true);
                        }
                        else
                        {
                            if (false == CheckSensorCurrentTemperature(m_OvenBottom, HWTemp))
                            {
                                mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE_HIGH, m_CurScenario,true, false);
                               // m_OvenSensorsCheck.set(1,false);
                            }
                        }
                            break;
                        case OVEN_BOTTOM2_SENSOR:
                        if (!m_OvenSensorsCheck.test(2))
                        {
                            retCode = DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE_HIGH;
                            m_OvenSensorsCheck.set(2,true);
                        }
                        else
                        {
                            if (false == CheckSensorCurrentTemperature(m_OvenBottom,HWTemp))
                            {
                                mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE_HIGH, m_CurScenario,true, false);
                                //m_OvenSensorsCheck.set(2,false);
                            }

                        }
                            break;
                        default:
                            break;
                    }
                }
                else if (HWTemp < userInputMeltingPoint-4)
                {
                    switch(OvenType)
                    {
                        case OVEN_TOP_SENSOR:
                        if (!m_OvenSensorsCheck.test(3))
                        {
                            retCode = DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE_LOW;
                            m_OvenSensorsCheck.set(3,true);
                        }
                        else
                        {
                            if (HWTemp<40)
                            {
                                mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE_LOW, m_CurScenario,true, false);
                                //m_OvenSensorsCheck.set(3,false);
                            }
                        }
                            break;
                        case OVEN_BOTTOM1_SENSOR:
                        if (!m_OvenSensorsCheck.test(4))
                        {
                            retCode = DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE_LOW;
                            m_OvenSensorsCheck.set(4,true);
                        }
                        else
                        {
                            if (HWTemp<40)
                            {
                                mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE_LOW, m_CurScenario,true, false);
                                //m_OvenSensorsCheck.set(4,false);
                            }
                        }
                            break;
                        case OVEN_BOTTOM2_SENSOR:
                        if (!m_OvenSensorsCheck.test(5))
                        {
                            retCode = DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE_LOW;
                            m_OvenSensorsCheck.set(5,true);
                        }
                        else
                        {
                            if (HWTemp<40)
                            {
                                mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE_LOW, m_CurScenario,true, false);
                                //m_OvenSensorsCheck.set(5,false);
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    switch(OvenType)
                    {
                    case OVEN_TOP_SENSOR:
                        if (m_OvenSensorsCheck[0])
                        {
                            mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE_HIGH, m_CurScenario,true, false);
                            m_OvenSensorsCheck.set(0,false);
                        }
                        if (m_OvenSensorsCheck[3])
                        {
                            mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE_LOW, m_CurScenario,true, false);
                            m_OvenSensorsCheck.set(3,false);
                        }
                        break;
                    case OVEN_BOTTOM1_SENSOR:
                        if (m_OvenSensorsCheck[1])
                        {
                            mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE_HIGH, m_CurScenario,true, false);
                            m_OvenSensorsCheck.set(1,false);
                        }
                        if (m_OvenSensorsCheck[4])
                        {
                            mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE_LOW, m_CurScenario,true, false);
                            m_OvenSensorsCheck.set(4,false);
                        }
                        break;
                    case OVEN_BOTTOM2_SENSOR:
                        if (m_OvenSensorsCheck[2])
                        {
                            mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE_HIGH, m_CurScenario,true, false);
                            m_OvenSensorsCheck.set(2,false);
                        }
                        if (m_OvenSensorsCheck[5])
                        {
                            mp_SchedulerController->RaiseError(0, DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE_LOW, m_CurScenario,true, false);
                            m_OvenSensorsCheck.set(5,false);
                        }
                        break;
                    }
                }
            }
        }
    }
    return retCode;
}

/*lint -e527 */
bool HeatingStrategy::CheckRVOutletHeatingOverTime(qreal HWTemp)
{
    if (true == m_RV_2_Outlet.OTCheckPassed)
    {
        return true;
    }
    qreal LimitTemp = 0.0;
    if(3 == m_CurScenario)
    {
        LimitTemp = 40.0;
        if(isEffectiveTemp(HWTemp) && ((HWTemp > LimitTemp) || qFuzzyCompare(HWTemp, LimitTemp)))
        {
            m_RV_2_Outlet.OTCheckPassed = true;
        }
        else
        {
            qint64 now = QDateTime::currentMSecsSinceEpoch();
            if (m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.curModuleId].HeatingOverTime > 0
                    && (now - m_RV_2_Outlet.heatingStartTime >= m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.curModuleId].HeatingOverTime*1000) )
            {
                if (-1 != m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.curModuleId].ScenarioList.indexOf(m_CurScenario))
                {
                    return false;
                }
            }
        }
    }
    else if(260 == m_CurScenario)
    {
        LimitTemp = 68.0;
        qreal meltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        if (meltingPoint < LimitTemp)
        {
            if ((HWTemp > meltingPoint || qFuzzyCompare(HWTemp, meltingPoint))&& isEffectiveTemp(HWTemp))
            {
                m_RV_2_Outlet.OTCheckPassed = true;
            }
        }
        else
        {
            if (((HWTemp > LimitTemp) || qFuzzyCompare(HWTemp, LimitTemp)) && isEffectiveTemp(HWTemp))
            {
                m_RV_2_Outlet.OTCheckPassed = true;
            }
        }

        qint64 now = QDateTime::currentMSecsSinceEpoch();
        if ( m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.curModuleId].HeatingOverTime
             && (now - m_RV_2_Outlet.heatingStartTime >= m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.curModuleId].HeatingOverTime*1000) )
        {
            if (-1 != m_RV_2_Outlet.functionModuleList[m_RV_2_Outlet.curModuleId].ScenarioList.indexOf(m_CurScenario))
            {

                if (meltingPoint < LimitTemp)
                {
                    if (HWTemp < meltingPoint)
                    {
                        return false;
                    }
                }
                else
                {
                    if (HWTemp < LimitTemp)
                    {
                        return false;
                    }
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

       (void)heatingSensor.functionModuleList.insert(*seqIter, funcModule);
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
    mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd, false);
}

bool HeatingStrategy::isEffectiveTemp(qreal HWTemp)
{
    if (qFuzzyCompare(UNDEFINED_VALUE,HWTemp) || qFuzzyCompare(UNDEFINED_1_BYTE,HWTemp) || qFuzzyCompare(UNDEFINED_2_BYTE,HWTemp) || qFuzzyCompare(UNDEFINED_4_BYTE,HWTemp) || HWTemp >= 298)
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

void HeatingStrategy::Init260ParamList(bool IsClickStartProgram)
{
    m_SensorsChecking.ovenTopPass = false;
    m_SensorsChecking.LATube1Pass = false;

    if(IsClickStartProgram)
    {
        m_SensorsChecking.firstBottle = true;
    }
    if(m_SensorsChecking.firstBottle)
    {
        m_SensorsChecking.startTime = QDateTime::currentMSecsSinceEpoch();
        m_SensorsChecking.meltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();

        if (m_SensorsChecking.meltingPoint <= 64.0)
        {
            m_SensorsChecking.minTime = 5*60*1000;
        }
        else
        {
            m_SensorsChecking.minTime = 8*60*1000;
        }
    }
}

bool HeatingStrategy::Check260SensorsTemp(bool IsPowerFailure)
{
    HardwareMonitor_t strctHWMonitor = mp_SchedulerCommandProcessor->HardwareMonitor();
    if (false == m_SensorsChecking.ovenTopPass)
    {
        qreal ovenTopTemp = strctHWMonitor.TempOvenTop;
        qreal checkTemp = m_SensorsChecking.meltingPoint-4;
        if ((ovenTopTemp >m_SensorsChecking.meltingPoint-4) || qFuzzyCompare(ovenTopTemp,checkTemp))
        {
            m_SensorsChecking.ovenTopPass = true;
        }
    }

    if (false == m_SensorsChecking.LATube1Pass)
    {
        qreal LATube1Temp = strctHWMonitor.TempALTube1;
        qreal checkTemp = m_SensorsChecking.meltingPoint+2;
        if (LATube1Temp >checkTemp || qFuzzyCompare(LATube1Temp, checkTemp))
        {
            m_SensorsChecking.LATube1Pass = true;
        }
    }

    // For LA tube2, we only check it heating status. If it is off, we just log it.
    if (false == strctHWMonitor.LATube2HeatingStatus)
    {
        mp_SchedulerController->RaiseEvent(EVENT_SCHEDULER_HEATING_LATBUE2_OFF);
    }

    if(IsPowerFailure)
    {
        qreal HWTemp = 0.0;
        qreal LimitTemp = 68.0;
        if(!m_RV_2_Outlet.OTCheckPassed)
        {
            HWTemp = strctHWMonitor.TempRV2;
            if (m_SensorsChecking.meltingPoint < LimitTemp)
            {
                if ((HWTemp > m_SensorsChecking.meltingPoint || qFuzzyCompare(HWTemp, m_SensorsChecking.meltingPoint)) && isEffectiveTemp(HWTemp))
                    m_RV_2_Outlet.OTCheckPassed = true;
            }
            else
            {
                if (((HWTemp > LimitTemp) || qFuzzyCompare(HWTemp, LimitTemp)) && isEffectiveTemp(HWTemp))
                    m_RV_2_Outlet.OTCheckPassed = true;
            }
        }
        if(!m_RTTop.OTCheckPassed)
        {
            if ((m_RTTop.functionModuleList[m_RTTop.curModuleId].OTTargetTemperature)<= HWTemp && isEffectiveTemp(HWTemp))
                m_RTTop.OTCheckPassed = true;
        }

        if(!m_RTBottom.OTCheckPassed)
        {
            if ((m_RTBottom.functionModuleList[m_RTBottom.curModuleId].OTTargetTemperature)<= HWTemp && isEffectiveTemp(HWTemp))
                m_RTBottom.OTCheckPassed = true;
        }
    }

    // for NON-first bottle, we need not check minimal time
    bool ret = m_RTTop.OTCheckPassed && m_RTBottom.OTCheckPassed && m_SensorsChecking.ovenTopPass
            && m_RV_2_Outlet.OTCheckPassed && m_SensorsChecking.LATube1Pass;

    if (m_SensorsChecking.firstBottle)
    {
        ret = ret && ((QDateTime::currentMSecsSinceEpoch()-m_SensorsChecking.startTime) >= m_SensorsChecking.minTime);

        // when first bottle checking is done, set the flag to be false
        if (ret)
        {
            m_SensorsChecking.firstBottle = false;
        }
    }

    // Reset OvenTop and LATbue1 when checking passed
    if (ret)
    {
        m_SensorsChecking.ovenTopPass = false;
        m_SensorsChecking.LATube1Pass = false;
    }

    return ret;
}
bool HeatingStrategy::CheckLATbueTempAbnormal(qreal temp)
{
    //Parrifin melting point (user input)
    qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
    if (temp < (userInputMeltingPoint -1) || qFuzzyCompare(temp,299))
    {
        return false;
    }

    return true;
}

}// end of namespace Scheduler
