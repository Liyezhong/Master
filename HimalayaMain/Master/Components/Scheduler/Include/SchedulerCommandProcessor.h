// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Include/SchedulerCommandProcessor.h
 *
 *  @brief
 *  Definition of class Scheduler::SchedulerCommandProcessor.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-10
 *  $Author:    $ L. Yang
 *
 *  @b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 * ================================================================================================
*/



#ifndef SCHEDULERCOMMANDPROCESSOR_H
#define SCHEDULERCOMMANDPROCESSOR_H

#include <QObject>
#include <QQueue>
#include <QMutex>

#include "Global/Include/Commands/Command.h"
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "DeviceControl/Include/Devices/RotaryValveDevice.h"
#include "DeviceControl/Include/Devices/AirLiquidDevice.h"
#include "DeviceControl/Include/Devices/RetortDevice.h"
#include "DeviceControl/Include/Devices/OvenDevice.h"
#include "DeviceControl/Include/Devices/PeripheryDevice.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"


using namespace DeviceControl;
//{
//    class IDeviceProcessing;
//}

namespace Scheduler{

typedef struct
{
	qreal			PressureAL;
	qreal			TempALLevelSensor;
	qreal			TempALTube1;
	qreal			TempALTube2;
	qreal			TempRV1;
	qreal			TempRV2;
	RVPosition_t	PositionRV;
	qreal			TempRTBottom;
	qreal			TempRTSide;
	qreal			TempOvenBottom;
	qreal			TempOvenTop;
	quint16			OvenLidStatus;
	quint16			RetortLockStatus;
} HardwareMonitor_t;

class SchedulerMainThreadController;

class SchedulerCommandProcessorBase : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /**
     * @brief constructor.
     *
     */
    /****************************************************************************/
    SchedulerCommandProcessorBase(){}
    virtual ~SchedulerCommandProcessorBase(){}
	virtual HardwareMonitor_t HardwareMonitor() = 0;
	virtual DeviceControl::ReturnCode_t ALBreakAllOperation() = 0;
    virtual qreal ALGetRecentPressure() = 0;
    virtual void ALSetPressureDrift(qreal pressureDrift) = 0;

signals:
    /****************************************************************************/
    /**
     * @brief emit the signal when command be finished!
     *
     */
    /****************************************************************************/
    void onCmdFinished(Global::CommandShPtr_t *cmd, bool result);
    void DCLConfigurationFinished(ReturnCode_t RetCode);
    void NewCmdAdded();

public slots:
    /****************************************************************************/
    /**
     * @brief thread's callback function
     *
     */
    /****************************************************************************/
    virtual void run() { this->run4Slot(); }

    /****************************************************************************/
    /**
     * @brief controller call this function to add a new command to process.
     *
     */
    /****************************************************************************/
    virtual void pushCmd(CmdSchedulerCommandBase *cmd) { this->pushCmd4Slot(cmd); }

    virtual void DevProcInitialisationAckn(quint32 instanceID, ReturnCode_t configResult)
    {
        this->DevProcInitialisationAckn4Slot(instanceID, configResult);
    }

    virtual void DevProcConfigurationAckn(quint32 instanceID, ReturnCode_t hdlInfo)
    {
        this->DevProcConfigurationAckn4Slot(instanceID, hdlInfo);
    }

    virtual void DevProcStartNormalOpModeAckn(quint32 instanceID, ReturnCode_t hdlInfo)
    {
        this->DevProcStartNormalOpModeAckn4Slot(instanceID, hdlInfo);
    }

    virtual void ThrowError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp)
    {
        this->ThrowError4Slot(instanceID, usErrorGroup, usErrorID, usErrorData, TimeStamp);
    }

    virtual void DevProcDestroyAckn()
    {
        this->DevProcDestroyAckn4Slot();
    }

    virtual void OnNewCmdAdded()
    {
        this->OnNewCmdAdded4Slot();
    }

private:

    /****************************************************************************/
    /**
     * @brief check if there are new command comming.
     *
     */
    /****************************************************************************/
    virtual bool newCmdComing() =0;
    virtual void run4Slot() = 0;
    virtual void pushCmd4Slot(CmdSchedulerCommandBase *cmd) = 0;
    virtual void DevProcInitialisationAckn4Slot(quint32 instanceID, ReturnCode_t configResult) = 0;
    virtual void DevProcConfigurationAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo) = 0;
    virtual void DevProcStartNormalOpModeAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo) = 0;
    virtual void ThrowError4Slot(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp) = 0;
    virtual void DevProcDestroyAckn4Slot() = 0;
    virtual void OnNewCmdAdded4Slot() = 0;
    SchedulerCommandProcessorBase(const SchedulerCommandProcessorBase&);              			///< Not implemented.
    const SchedulerCommandProcessorBase& operator=(const SchedulerCommandProcessorBase&);		///< Not implemented.
};


/****************************************************************************/
/**
 * @brief command processing thread class for the main scheduler thread.
 *
 */
/****************************************************************************/
template <class DP>
class SchedulerCommandProcessor : public SchedulerCommandProcessorBase 
{
public:
    /****************************************************************************/
    /**
     * @brief constructor and destructor.
     *
     */
    /****************************************************************************/
    explicit SchedulerCommandProcessor(SchedulerMainThreadController *controller);
    ~SchedulerCommandProcessor();
	virtual HardwareMonitor_t HardwareMonitor();
	virtual DeviceControl::ReturnCode_t ALBreakAllOperation() { return mp_IDeviceProcessing->ALBreakAllOperation(); } 
    virtual qreal ALGetRecentPressure() { return mp_IDeviceProcessing->ALGetRecentPressure(); }
    virtual void ALSetPressureDrift(qreal pressureDrift) { mp_IDeviceProcessing->ALSetPressureDrift(pressureDrift); }
#ifdef GOOGLE_MOCK
    void SetIDeviceProcessing(DP* IDeviceProcessing) { mp_IDeviceProcessing = IDeviceProcessing; }
#endif

private:

    /****************************************************************************/
    /**
     * @brief check if there are new command comming.
     *
     */
    /****************************************************************************/
    virtual bool newCmdComing();
    virtual void run4Slot();
    virtual void pushCmd4Slot(CmdSchedulerCommandBase *cmd);
    virtual void DevProcInitialisationAckn4Slot(quint32 instanceID, ReturnCode_t configResult);
    virtual void DevProcConfigurationAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo);
    virtual void DevProcStartNormalOpModeAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo);
    virtual void ThrowError4Slot(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp);
    virtual void DevProcDestroyAckn4Slot();
    virtual void OnNewCmdAdded4Slot();

    void ExecuteCmd();
    SchedulerCommandProcessor(const SchedulerCommandProcessor&);                      ///< Not implemented.
    const SchedulerCommandProcessor& operator=(const SchedulerCommandProcessor&);     ///< Not implemented.


private:
    SchedulerMainThreadController *mp_SchedulerThreadController;
    DP *mp_IDeviceProcessing;

    QQueue<Scheduler::SchedulerCommandShPtr_t> m_Cmds;
    Scheduler::SchedulerCommandShPtr_t m_currentCmd;
    QMutex m_CmdMutex;
};

} // end of namespace Scheduler

#endif // SCHEDULERCOMMANDPROCESSOR_H
