// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Source/HeatingStrategy.cpp
 *
 *  @brief
 *  Implementation of class Scheduler::HeatingStrategy.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 04.03.2014 
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

namespace Scheduler{
HeatingStrategy::HeatingStrategy(SchedulerMainThreadController* schedController,
								CSchedulerStateMachine* schedMachine,
                                SchedulerCommandProcessorBase* SchedCmdProcessor,
                                DataManager::CDataManager* DataManager, qreal Interval)
								:mp_SchedulerController(schedController),
								mp_SchedulerMachine(schedMachine),
								mp_SchedulerCommandProcessor(SchedCmdProcessor),
                                mp_DataManager(DataManager)
{
    m_Interval = Interval;
    this->ConstructMonitorList();
}

ReturnCode_t HeatingStrategy::StartTemperatureControlWithPID(const QString& devName, const DataManager::FunctionKey_t& funcKey)
{
    bool flag4NominalTemp  = false, flag4SlopeTempChange  = false, flag4MaxTemp = false,
        flag4ControllerGain = false, flag4ResetTime = false, flag4DerivativeTime = false;
	ReturnCode_t retCode = DCL_ERR_FCT_CALL_FAILED;

    qreal	nominalTemp = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "NominalTemperature", flag4NominalTemp);
    qreal	slopeTempChange = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "SlopeTempChange", flag4SlopeTempChange);
    qreal	maxTemp = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "MaxTemperature", flag4MaxTemp);
    qreal	controllerGain = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "ControllerGain", flag4ControllerGain);
    qreal	resetTime = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "ResetTime", flag4ResetTime);
    qreal	derivativeTime = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "DerivativeTime", flag4DerivativeTime);

    if (false == flag4NominalTemp || false == flag4SlopeTempChange || false == flag4MaxTemp ||
        false == flag4ControllerGain || false == flag4ResetTime || false == flag4DerivativeTime)
	{
		return DCL_ERR_FCT_CALL_FAILED;
	}

    qreal UserInputTemp = 0.0;
    if ("Retort" == devName && "Heating" == funcKey.key) //For Retort
	{
        // For Ambient temperature, Retort need NOT set temperature.
        if (false == mp_SchedulerController->GetCurProgramID().isEmpty()) // make sure program ID is NOT empty
        {
            UserInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
                ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
        }
        if (qFuzzyCompare(UserInputTemp, -1))
        {
            return DCL_ERR_FCT_CALL_SUCCESS;
        }

        CmdRTStartTemperatureControlWithPID* pHeatingCmd = new CmdRTStartTemperatureControlWithPID(500, mp_SchedulerController);
        if ("Top" == funcKey.position)
        {
            pHeatingCmd->SetType(RT_SIDE);
        }
        else if ("Bottom" == funcKey.position)
        {
            pHeatingCmd->SetType(RT_BOTTOM);
        }

        pHeatingCmd->SetNominalTemperature(UserInputTemp+nominalTemp);
        pHeatingCmd->SetSlopeTempChange(slopeTempChange);
        pHeatingCmd->SetMaxTemperature(maxTemp);
        pHeatingCmd->SetControllerGain(controllerGain);
        pHeatingCmd->SetResetTime(resetTime);
        pHeatingCmd->SetDerivativeTime(derivativeTime);

        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);
    }
    else if ("Oven" == devName && "Heating" == funcKey.key) //For Oven
    {
        UserInputTemp = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        CmdOvenStartTemperatureControlWithPID* pHeatingCmd = new CmdOvenStartTemperatureControlWithPID(500, mp_SchedulerController);
        if ("Top" == funcKey.position)
        {
            pHeatingCmd->SetType(OVEN_TOP);
        }
        else
        {
            pHeatingCmd->SetType(OVEN_BOTTOM);
        }
        pHeatingCmd->SetNominalTemperature(UserInputTemp+nominalTemp);
        pHeatingCmd->SetSlopeTempChange(slopeTempChange);
        pHeatingCmd->SetMaxTemperature(maxTemp);
        pHeatingCmd->SetControllerGain(controllerGain);
        pHeatingCmd->SetResetTime(resetTime);
        pHeatingCmd->SetDerivativeTime(derivativeTime);

        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);
    }
    else if ("Rotary Valve" == devName && "Heating" == funcKey.key) //For Rotary Valve
    {
        CmdRVStartTemperatureControlWithPID* pHeatingCmd = new CmdRVStartTemperatureControlWithPID(500, mp_SchedulerController);
        pHeatingCmd->SetNominalTemperature(nominalTemp);
        pHeatingCmd->SetSlopeTempChange(slopeTempChange);
        pHeatingCmd->SetMaxTemperature(maxTemp);
        pHeatingCmd->SetControllerGain(controllerGain);
        pHeatingCmd->SetResetTime(resetTime);
        pHeatingCmd->SetDerivativeTime(derivativeTime);

        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);
    }
    else if ("LA" == devName && "Heating" == funcKey.key) //For LA
    {
        CmdALStartTemperatureControlWithPID* pHeatingCmd  = new CmdALStartTemperatureControlWithPID(500, mp_SchedulerController);
        pHeatingCmd->SetType(AL_LEVELSENSOR);
        pHeatingCmd->SetNominalTemperature(nominalTemp);
        pHeatingCmd->SetSlopeTempChange(slopeTempChange);
        pHeatingCmd->SetMaxTemperature(maxTemp);
        pHeatingCmd->SetControllerGain(controllerGain);
        pHeatingCmd->SetResetTime(resetTime);
        pHeatingCmd->SetDerivativeTime(derivativeTime);

        mp_SchedulerCommandProcessor->pushCmd(pHeatingCmd);

    }
	SchedulerCommandShPtr_t pResHeatingCmd;
    while (!mp_SchedulerController->PopDeviceControlCmdQueue(pResHeatingCmd));
    pResHeatingCmd->GetResult(retCode);

    return retCode;
}

bool HeatingStrategy::GetTargetTempPaddingAndOverTime(const QString& devName, const DataManager::FunctionKey_t& funcKey, qreal& TempPadding, qreal& overTime)
{
    bool ok = false;
    if ("Retort" == devName || "LA" == devName || "Rotary Valve" == devName)
    {
        TempPadding = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "NominalTemperature", ok);
        if (false == ok)
        {
            return false;
        }
    }
    else
    {
        TempPadding = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "HeatingOverTimeTemp", ok);
        if (false == ok)
        {
            return false;
        }
    }
    overTime = mp_DataManager->GetProgramSettings()->GetParameterValue(devName, funcKey, "HeatingOverTime", ok);
    if (false == ok)
    {
        return false;
    }
    return true;
}

void HeatingStrategy::ConstructMonitorList()
{
    // For Retort
    QString devName = "Retort";
    DataManager::FunctionKey_t funcKey = {"Heating", "Bottom", "With-Paraffin"};
    qreal tempPadding = 0.0;
    qreal overTime = 0.0;
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Retort-HT-BOTTOM-With-Paraffin",monitorVal);
    }

    funcKey = {"Heating", "Bottom", "Without-Reagents"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Retort-HT-BOTTOM-Without-Reagents",monitorVal);
    }

    funcKey = {"Heating", "Bottom", "With-Reagents"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Retort-HT-BOTTOM-With-Reagents",monitorVal);
    }

    funcKey = {"Heating", "Top", "With-Paraffin"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Retort-HT-TOP-With-Paraffin",monitorVal);
    }

    funcKey = {"Heating", "Top", "Without-Reagents"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Retort-HT-TOP-Without-Reagents",monitorVal);
    }

    funcKey = {"Heating", "Top", "With-Reagents"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Retort-HT-TOP-With-Reagents",monitorVal);
    }

    //For Oven
    devName = "Oven";
    funcKey = {"Heating", "Top", "5064-Paraffin"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Oven-HT-TOP-5064-Paraffin",monitorVal);
    }

    funcKey = {"Heating", "Top", "6470-Paraffin"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding,  0.0, overTime};
        m_MonitorList.insert("Oven-HT-TOP-6470-Paraffin",monitorVal);
    }

    funcKey = {"Heating", "Bottom", "5064-Paraffin"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Oven-HT-BOTTOM-5064-Paraffin",monitorVal);
    }

    funcKey = {"Heating", "Bottom", "6470-Paraffin"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("Oven-HT-BOTTOM-6470-Paraffin",monitorVal);
    }

    //For Rotary Valve
    devName = "Rotary Valve";
    funcKey = {"Heating", "", "Sucking-Paraffin"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("RV-HT-Sucking-Paraffin",monitorVal);
    }

    // For LA
    devName = "LA";
    funcKey = {"Heating", "", "RVTube"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("LA-HT-RVTube",monitorVal);
    }
    funcKey = {"Heating", "", "WaxTrap"};
    if (this->GetTargetTempPaddingAndOverTime(devName, funcKey,tempPadding, overTime))
    {
        OTMonitor_t monitorVal={devName, funcKey, false, tempPadding, 0.0, overTime};
        m_MonitorList.insert("LA-HT-WaxTrap",monitorVal);
    }
}

quint32 HeatingStrategy::CheckHeatingOverTime(const HardwareMonitor_t& HWValueList)
{
    QMap<QString, OTMonitor_t>::iterator iter;
    for (iter = m_MonitorList.begin(); iter!= m_MonitorList.end(); ++iter)
    {
        if (true == iter.value().needCheck)
        {
            iter.value().elapseTime += m_Interval;

            // Get the Temp offset
            if (iter.value().elapseTime > iter.value().overTime)
            {
                bool ok = false;
                QString devName = iter.value().devName;
                DataManager::FunctionKey_t funcKey = iter.value().funcKey;
                qreal userInputTemp = 0.0;
                if ("Retort" == devName)
                {
                    if (false == mp_SchedulerController->GetCurProgramID().isEmpty())
                    {
                        userInputTemp = mp_DataManager->GetProgramList()->GetProgram(mp_SchedulerController->GetCurProgramID())
                                ->GetProgramStep(mp_SchedulerController->GetCurProgramStepIndex())->GetTemperature().toDouble();
                    }

                }
                else if ("Oven" == devName)
                {
                    userInputTemp = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
                }
                else
                {
                   userInputTemp = 0.0;
                }

                if (iter.value().targetTempPadding+userInputTemp < 100) // Hard code.
                {
                    // Once the error occurs, the sensor needn't be checked.
                    iter.value().needCheck = false;

                    if ("Retort-HT-TOP-With-Paraffin" == iter.key())
                    {
                        return HEATING_OT_RETORT_TOP;
                    }
                    else if ("Retort-HT-BOTTOM-With-Paraffin" == iter.key())
                    {
                        return HEATING_OT_RETORT_BOTTOM;
                    }
                    else if ("Oven-HT-TOP-5064-Paraffin" == iter.key())
                    {
                        return HEATING_OT_OVEN_TOP;
                    }
                    else if ("Oven-HT-BOTTOM-5064-Paraffin" == iter.key())
                    {
                        return HEATING_OT_OVEN_BOTTOM;
                    }
                }
            }
            else
            {
                continue;
            }
        }
        else
        {
            continue;
        }
    }
    return 0;
}

bool HeatingStrategy::StartOverTimeCheck(const QString& moduleName)
{
    if (m_MonitorList.contains(moduleName))
    {
        m_MonitorList[moduleName].needCheck = true;
        return true;
    }
    return false;
}


}// end of namespace Scheduler

