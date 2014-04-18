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
 *  (C) Copyright 2014 by Leica Biosystems Shanghai. All rights reserved.
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
    QString            Id;
    QVector<qint32>    ScenarioList;
    qreal              TemperatureOffset;
    qreal              MaxTemperature;
    qreal              HeatingOverTime;
    qreal              SlopTempChange;
    quint32            ControllerGain;
    quint32            ResetTime;
    quint32            DerivativeTime;

};

struct HeatingSensor
{
    QString							devName;
    QString							sensorName;
    QMap<QString, FunctionModule>	functionModuleList;
	qint64							heatingStartTime;
	QString							curModuleId;
};

struct RTLevelSensor : public HeatingSensor
{
	QMap<QString, QString>	CurrentSpeedList;
	QMap<QString, qreal>	ExchangePIDTempList;
};


struct RTBottomSensor : public HeatingSensor
{
    QMap<QString, qreal>    TemperatureDiffList;
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
                    DataManager::CDataManager* dataManager);
    ~HeatingStrategy() {}
    DeviceControl::ReturnCode_t RunHeatingStrategy(const HardwareMonitor_t& strctHWMonitor, qint32 scenario);
private:
    SchedulerMainThreadController*      mp_SchedulerController;
    SchedulerCommandProcessorBase*      mp_SchedulerCommandProcessor;
    DataManager::CDataManager*          mp_DataManager;
	qint32								m_CurScenario;
    RTLevelSensor                       m_RTLevelSensor;
    HeatingSensor                       m_RTTop;
    RTBottomSensor                      m_RTBottom;
    bool ConstructHeatingSensorList();
    bool ConstructHeatingSensor(HeatingSensor& heatingSensor, const QStringList& sequenceList);
	DeviceControl::ReturnCode_t StartLevelSensorTemperatureControl(const HardwareMonitor_t& strctHWMonitor);
	DeviceControl::ReturnCode_t StartRTTemperatureControl(HeatingSensor& heatingSensor, RTTempCtrlType_t RTType);
private:
    HeatingStrategy(const HeatingStrategy& rhs);
    HeatingStrategy& operator=(const HeatingStrategy& rhs);
};
}
#endif // SCHEDULERMACHINE_H
