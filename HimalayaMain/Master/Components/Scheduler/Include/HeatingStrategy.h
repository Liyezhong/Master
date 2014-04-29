/****************************************************************************/
/*! \file Himalaya/HimalayaMain/Master/Components/Scheduler/Include/HeatingStrategy.h
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
    qreal              OTTargetTemperature;
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
    QMap<QString, bool>     StartTempFlagList;
};


struct RTBottomSensor : public HeatingSensor
{
    QMap<QString, qreal>    TemperatureDiffList;
};

struct OvenSensor : public HeatingSensor
{
    QMap<QString, qreal>                    OTTempOffsetList;
    QMap< QString, QPair<qreal,qreal> >     ParaffinTempRangeList;
    QMap< QString, QPair<qreal, qreal> >    TimeRangeList;
    QMap<QString, bool>						StartTempFlagList;

};

struct RVSensor : public HeatingSensor
{
    QMap<QString, bool>     UserInputFlagList;
};

struct RVOutlet 
{
    QString				devName;
    QString				sensorName;
    qreal				HeatingOverTime;
    qint64				heatingStartTime;
    QVector<qint32>		ScenarioList;
	bool				needCheckOT;
};

struct LASensor : public HeatingSensor
{
    QMap<QString, bool>		StartTempFlagList;
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
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function HeatingStrategy
     *
     *  \param schedController
     *  \param schedMachine
     *  \param SchedCmdProcessor
     *  \param dataManager
     *  \param Interval
     *
     *  \return from HeatingStrategy
     */
    /****************************************************************************/
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
    OvenSensor                          m_OvenTop;
    OvenSensor                          m_OvenBottom;
    RVSensor                            m_RVRod;
	RVOutlet							m_RVOutlet;
    LASensor							m_LARVTube;
    LASensor							m_LAWaxTrap;

    bool ConstructHeatingSensorList();
    bool ConstructHeatingSensor(HeatingSensor& heatingSensor, const QStringList& sequenceList);
	inline bool CheckSensorCurrentTemperature(const HeatingSensor& heatingSensor, qreal HWTemp);
	DeviceControl::ReturnCode_t StartLevelSensorTemperatureControl(const HardwareMonitor_t& strctHWMonitor);
    DeviceControl::ReturnCode_t StartRTTemperatureControl(HeatingSensor& heatingSensor, RTTempCtrlType_t RTType);
    DeviceControl::ReturnCode_t StartOvenTemperatureControl(OvenSensor& heatingSensor, OVENTempCtrlType_t OvenType);
    DeviceControl::ReturnCode_t StartRVTemperatureControl(RVSensor& heatingSensor);
    inline void StartRVOutletHeatingOTCalculation();
    DeviceControl::ReturnCode_t StartLATemperatureControl(LASensor& heatingSensor,ALTempCtrlType_t LAType);
	inline bool CheckSensorHeatingOverTime(const HeatingSensor& heatingSensor, qreal HWTemp);
	inline bool CheckRVOutletHeatingOverTime(qreal HWTemp);
private:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function HeatingStrategy
     *
     *  \param rhs = const HeatingStrategy type parameter
     *
     *  \return from HeatingStrategy
     */
    /****************************************************************************/
    HeatingStrategy(const HeatingStrategy& rhs);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function operator=
     *
     *  \param  rhs
     *
     *  \return from operator=
     */
    /****************************************************************************/
    HeatingStrategy& operator=(const HeatingStrategy& rhs);
};
}
#endif // SCHEDULERMACHINE_H
