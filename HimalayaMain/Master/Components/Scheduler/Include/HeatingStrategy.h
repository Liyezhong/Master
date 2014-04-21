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
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct OTMonitor_t
 */
/****************************************************************************/
typedef struct
{
    QString						devName;       ///<  Definition/Declaration of variable devName
	DataManager::FunctionKey_t	funcKey;       ///<  Definition/Declaration of variable funcKey
    bool						needCheck;       ///<  Definition/Declaration of variable needCheck
    qreal						targetTempPadding;       ///<  Definition/Declaration of variable targetTempPadding
    qreal						elapseTime;       ///<  Definition/Declaration of variable elapseTime
    qreal						overTime;       ///<  Definition/Declaration of variable overTime
}OTMonitor_t;


const quint32	HEATING_OT_CALL_FAILED		= 0x09;       ///<  Definition/Declaration of variable HEATING_OT_CALL_FAILED
const quint32	HEATING_OT_RETORT_TOP		= 0x10;       ///<  Definition/Declaration of variable HEATING_OT_RETORT_TOP
const quint32	HEATING_OT_RETORT_BOTTOM	= 0x11;       ///<  Definition/Declaration of variable HEATING_OT_RETORT_BOTTOM
const quint32	HEATING_OT_OVEN_TOP			= 0x20;       ///<  Definition/Declaration of variable HEATING_OT_OVEN_TOP
const quint32	HEATING_OT_OVEN_BOTTOM		= 0x21;       ///<  Definition/Declaration of variable HEATING_OT_OVEN_BOTTOM

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
                    CSchedulerStateMachine* schedMachine,
                    SchedulerCommandProcessorBase* SchedCmdProcessor,
                    DataManager::CDataManager* dataManager, qreal Interval);
    ~HeatingStrategy() {}
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function StartTemperatureControlWithPID
     *
     *  \param cmdName = const QString type parameter
     *  \param funcKey =  const type parameter
     *
     *  \return from StartTemperatureControlWithPID
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t StartTemperatureControlWithPID(const QString& cmdName, const DataManager::FunctionKey_t& funcKey);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CheckHeatingOverTime
     *
     *  \param HWValueList = const HardwareMonitor_t type parameter
     *
     *  \return from CheckHeatingOverTime
     */
    /****************************************************************************/
    quint32 CheckHeatingOverTime(const HardwareMonitor_t& HWValueList); // Called by HardWareMonitor intervally
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function StartOverTimeCheck
	 *
	 *  \param moduleName = const QString type parameter
	 *
	 *  \return from StartOverTimeCheck
	 */
	/****************************************************************************/
	bool StartOverTimeCheck(const QString& moduleName);

private:
	SchedulerMainThreadController*	mp_SchedulerController;       ///<  Definition/Declaration of variable mp_SchedulerController
    CSchedulerStateMachine*			mp_SchedulerMachine;       ///<  Definition/Declaration of variable mp_SchedulerMachine
    SchedulerCommandProcessorBase*	mp_SchedulerCommandProcessor;       ///<  Definition/Declaration of variable mp_SchedulerCommandProcessor
    DataManager::CDataManager*		mp_DataManager;       ///<  Definition/Declaration of variable mp_DataManager
    qreal							m_Interval;       ///<  Definition/Declaration of variable m_Interval
    QMap<QString, OTMonitor_t>      m_MonitorList;       ///<  Definition/Declaration of variable m_MonitorList
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetTargetTempPaddingAndOverTime
     *
     *  \param devName = const QString type parameter
     *  \param funcKey =  const DataManager::FunctionKey_t type parameter
     *  \param TempPadding =  qreal type parameter
     *  \param overTime =  qreal type parameter
     *
     *  \return from GetTargetTempPaddingAndOverTime
     */
    /****************************************************************************/
    bool GetTargetTempPaddingAndOverTime(const QString& devName, const DataManager::FunctionKey_t& funcKey, qreal& TempPadding, qreal& overTime);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ConstructMonitorList
     *
     *  \return from ConstructMonitorList
     */
    /****************************************************************************/
    void ConstructMonitorList();
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
