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
                                SchedulerCommandProcessorBase* SchedCmdProcessor,
                                DataManager::CDataManager* DataManager, qreal Interval)
								:mp_SchedulerController(schedController),
								mp_SchedulerCommandProcessor(SchedCmdProcessor),
                                mp_DataManager(DataManager)
{
    m_Interval = Interval;
    this->ConstructHeatingSensorList();
}

bool HeatingStrategy::ConstructHeatingSensorList()
{
    // For Retort Bottom1
    m_RTBottom1.devName = "Retort";
    m_RTBottom1.sensorName = "RTBottom";
    QStringList seqenceList = {"1", "2", "3"};
    if (false == this->ConstructHeatingSensor(m_RTBottom1, seqenceList))
    {
        return false;
    }

    // For Retort Bottom2
    m_RTBottom2.devName = "Retort";
    m_RTBottom2.sensorName = "RTBottom";
    seqenceList = {"1", "2", "3"};
    if (false == this->ConstructHeatingSensor(m_RTBottom2, seqenceList))
    {
        return false;
    }

    // For Retort Top
    m_RTTop.devName = "Retort";
    m_RTTop.sensorName = "RTTop";
    seqenceList = {"1", "2", "3"};
    if (false == this->ConstructHeatingSensor(m_RTTop, seqenceList))
    {
        return false;
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

       qreal derivateTime= mp_DataManager->GetProgramSettings()->GetParameterValue(heatingSensor.devName, funcKey, "DerivateTime", ok);
       if (false == ok)
       {
           return false;
       }
       funcModule.DerivateTime = derivateTime;

       heatingSensor.functionModuleList.push_back(funcModule);
   }

   return true;
}
}// end of namespace Scheduler

