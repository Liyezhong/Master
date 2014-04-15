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

// structure for monitoring each component
typedef struct
{
    QString						devName;
	DataManager::FunctionKey_t	funcKey;
    bool						needCheck;
    qreal						targetTempPadding;
    qreal						elapseTime;
    qreal						overTime;
}OTMonitor_t;


const quint32	HEATING_OT_CALL_FAILED		= 0x09;
const quint32	HEATING_OT_RETORT_TOP		= 0x10;
const quint32	HEATING_OT_RETORT_BOTTOM	= 0x11;
const quint32	HEATING_OT_OVEN_TOP			= 0x20;
const quint32	HEATING_OT_OVEN_BOTTOM		= 0x21;

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
                    CSchedulerStateMachine* schedMachine,
                    SchedulerCommandProcessorBase* SchedCmdProcessor,
                    DataManager::CDataManager* dataManager, qreal Interval);
    ~HeatingStrategy() {}
    DeviceControl::ReturnCode_t StartTemperatureControlWithPID(const QString& cmdName, const DataManager::FunctionKey_t& funcKey);
    quint32 CheckHeatingOverTime(const HardwareMonitor_t& HWValueList); // Called by HardWareMonitor intervally
	bool StartOverTimeCheck(const QString& moduleName);

private:
	SchedulerMainThreadController*	mp_SchedulerController;
    CSchedulerStateMachine*			mp_SchedulerMachine;
    SchedulerCommandProcessorBase*	mp_SchedulerCommandProcessor;
    DataManager::CDataManager*		mp_DataManager;
    qreal							m_Interval;
    QMap<QString, OTMonitor_t>      m_MonitorList;
    bool GetTargetTempPaddingAndOverTime(const QString& devName, const DataManager::FunctionKey_t& funcKey, qreal& TempPadding, qreal& overTime);
    void ConstructMonitorList();
private:
    HeatingStrategy(const HeatingStrategy& rhs);
    HeatingStrategy& operator=(const HeatingStrategy& rhs);
};
}

#endif // SCHEDULERMACHINE_H
