/****************************************************************************/
/*! \file Himalaya/HimalayaMain/Master/Components/Scheduler/Source/HeatingStrategy.h
 *
 *  \brief HeatingStrategy class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 04.03.2014
 *   $Author:  $ Songtao Yu 
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2014 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef HEATING_STRATEGY_H
#define HEATING_STRATEGY_H
#include <QObject>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"


namespace DataManager {
class CDataManager;
}

namespace Scheduler{

class SchedulerMainThreadController;
class CSchedulerStateMachine;
class SchedulerCommandProcessorBase;

struct FunctionModule
{
    QVector<qint32>    ScenarioList;
    qreal              TemperatureOffset;
    qreal              MaxTemperature;
    qreal              HeatingOverTime;
    qreal              SlopTempChange;
    quint32            ControllerGain;
    quint32            ResetTime;
    quint32            DerivateTime;

};

struct HeatingSensor
{
    QString					devName;
    QString					sensorName;
    QVector<FunctionModule>	functionModuleList;
};

struct RTLevelSensor : public HeatingSensor
{
    QString             CurrentSpeed;
    QString             ExchangePIDTemp;
};

struct RTBottomTemperatureDiff
{
    QVector<qint32>		ScenarioList;
    qreal				TempDiff;
};

/****************************************************************************/
/*!
 * \brief Heating Strategy Class
 */
/****************************************************************************/
class HeatingStrategy : public QObject
{
    Q_OBJECT
public:
    HeatingStrategy(SchedulerMainThreadController* schedController,
                    SchedulerCommandProcessorBase* SchedCmdProcessor,
                    DataManager::CDataManager* dataManager, qreal Interval);
    ~HeatingStrategy() {}
private:
    SchedulerMainThreadController*      mp_SchedulerController;
    SchedulerCommandProcessorBase*      mp_SchedulerCommandProcessor;
    DataManager::CDataManager*          mp_DataManager;
    qreal                               m_Interval;
    RTLevelSensor                       m_RTLevelSensor;
    HeatingSensor                       m_RTTop;
    HeatingSensor                       m_RTBottom1;
    HeatingSensor                       m_RTBottom2;
    QVector<RTBottomTemperatureDiff>    m_RTBottomTempDiffList;
    bool ConstructHeatingSensorList();
    bool ConstructHeatingSensor(HeatingSensor& heatingSensor, const QStringList& sequenceList);
private:
    HeatingStrategy(const HeatingStrategy& rhs);
    HeatingStrategy& operator=(const HeatingStrategy& rhs);
};
}

#endif // SCHEDULERMACHINE_H
