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

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct HardwareMonitor_t
 */
/****************************************************************************/
typedef struct
{
	qreal			PressureAL;       ///<  Definition/Declaration of variable PressureAL
	qreal			TempALLevelSensor;       ///<  Definition/Declaration of variable TempALLevelSensor
	qreal			TempALTube1;       ///<  Definition/Declaration of variable TempALTube1
	qreal			TempALTube2;       ///<  Definition/Declaration of variable TempALTube2
	qreal			TempRV1;       ///<  Definition/Declaration of variable TempRV1
	qreal			TempRV2;       ///<  Definition/Declaration of variable TempRV2
	RVPosition_t	PositionRV;       ///<  Definition/Declaration of variable PositionRV
	qreal			TempRTBottom;       ///<  Definition/Declaration of variable TempRTBottom
	qreal			TempRTSide;       ///<  Definition/Declaration of variable TempRTSide
	qreal			TempOvenBottom;       ///<  Definition/Declaration of variable TempOvenBottom
	qreal			TempOvenTop;       ///<  Definition/Declaration of variable TempOvenTop
	quint16			OvenLidStatus;       ///<  Definition/Declaration of variable OvenLidStatus
	quint16			RetortLockStatus;       ///<  Definition/Declaration of variable RetortLockStatus
} HardwareMonitor_t;

class SchedulerMainThreadController;


/****************************************************************************/
/**
 * @brief command processing thread class for the main scheduler thread.
 *
 */
/****************************************************************************/
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
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function HardwareMonitor
	 *
	 *  \return from HardwareMonitor
	 */
	/****************************************************************************/
	virtual HardwareMonitor_t HardwareMonitor() = 0;
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function ALBreakAllOperation
	 *
	 *  \return from ALBreakAllOperation
	 */
	/****************************************************************************/
	virtual DeviceControl::ReturnCode_t ALBreakAllOperation() = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ALGetRecentPressure
     *
     *  \return from ALGetRecentPressure
     */
    /****************************************************************************/
    virtual qreal ALGetRecentPressure() = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ALSetPressureDrift
     *
     *  \return from ALSetPressureDrift
     */
    /****************************************************************************/
    virtual void ALSetPressureDrift(qreal pressureDrift) = 0;

signals:
    /****************************************************************************/
    /**
     * @brief emit the signal when command be finished!
     *
     */
    /****************************************************************************/
    void onCmdFinished(Global::CommandShPtr_t *cmd, bool result);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal DCLConfigurationFinished
     */
    /****************************************************************************/
    void DCLConfigurationFinished(ReturnCode_t RetCode);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal NewCmdAdded
     */
    /****************************************************************************/
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
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function run4Slot
     *
     *  \return from run4Slot
     */
    /****************************************************************************/
    virtual void run4Slot() = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function pushCmd4Slot
     *
     *  \return from pushCmd4Slot
     */
    /****************************************************************************/
    virtual void pushCmd4Slot(CmdSchedulerCommandBase *cmd) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcInitialisationAckn4Slot
     *
     *  \return from DevProcInitialisationAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcInitialisationAckn4Slot(quint32 instanceID, ReturnCode_t configResult) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcConfigurationAckn4Slot
     *
     *  \return from DevProcConfigurationAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcConfigurationAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcStartNormalOpModeAckn4Slot
     *
     *  \return from DevProcStartNormalOpModeAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcStartNormalOpModeAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ThrowError4Slot
     *
     *  \return from ThrowError4Slot
     */
    /****************************************************************************/
    virtual void ThrowError4Slot(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcDestroyAckn4Slot
     *
     *  \return from DevProcDestroyAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcDestroyAckn4Slot() = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function OnNewCmdAdded4Slot
     *
     *  \return from OnNewCmdAdded4Slot
     */
    /****************************************************************************/
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
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function HardwareMonitor
	 *
	 *  \return from HardwareMonitor
	 */
	/****************************************************************************/
	virtual HardwareMonitor_t HardwareMonitor();
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function ALBreakAllOperation
	 *
	 *  \return from ALBreakAllOperation
	 */
	/****************************************************************************/
	virtual DeviceControl::ReturnCode_t ALBreakAllOperation() { return mp_IDeviceProcessing->ALBreakAllOperation(); } 
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ALGetRecentPressure
     *
     *  \return from ALGetRecentPressure
     */
    /****************************************************************************/
    virtual qreal ALGetRecentPressure() { return mp_IDeviceProcessing->ALGetRecentPressure(); }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ALSetPressureDrift
     *
     *  \return from ALSetPressureDrift
     */
    /****************************************************************************/
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
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function run4Slot
     *
     *  \return from run4Slot
     */
    /****************************************************************************/
    virtual void run4Slot();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function pushCmd4Slot
     *
     *  \return from pushCmd4Slot
     */
    /****************************************************************************/
    virtual void pushCmd4Slot(CmdSchedulerCommandBase *cmd);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcInitialisationAckn4Slot
     *
     *  \return from DevProcInitialisationAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcInitialisationAckn4Slot(quint32 instanceID, ReturnCode_t configResult);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcConfigurationAckn4Slot
     *
     *  \return from DevProcConfigurationAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcConfigurationAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcStartNormalOpModeAckn4Slot
     *
     *  \return from DevProcStartNormalOpModeAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcStartNormalOpModeAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ThrowError4Slot
     *
     *  \return from ThrowError4Slot
     */
    /****************************************************************************/
    virtual void ThrowError4Slot(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcDestroyAckn4Slot
     *
     *  \return from DevProcDestroyAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcDestroyAckn4Slot();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function OnNewCmdAdded4Slot
     *
     *  \return from OnNewCmdAdded4Slot
     */
    /****************************************************************************/
    virtual void OnNewCmdAdded4Slot();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ExecuteCmd
     *
     *  \return from ExecuteCmd
     */
    /****************************************************************************/
    void ExecuteCmd();
    SchedulerCommandProcessor(const SchedulerCommandProcessor&);                      ///< Not implemented.
    const SchedulerCommandProcessor& operator=(const SchedulerCommandProcessor&);     ///< Not implemented.


private:
    SchedulerMainThreadController *mp_SchedulerThreadController;       ///<  Definition/Declaration of variable mp_SchedulerThreadController
    DP *mp_IDeviceProcessing;       ///<  Definition/Declaration of variable mp_IDeviceProcessing

    QQueue<Scheduler::SchedulerCommandShPtr_t> m_Cmds;       ///<  Definition/Declaration of variable m_Cmds
    Scheduler::SchedulerCommandShPtr_t m_currentCmd;       ///<  Definition/Declaration of variable m_currentCmd
    QMutex m_CmdMutex;       ///<  Definition/Declaration of variable m_CmdMutex
};

} // end of namespace Scheduler

#endif // SCHEDULERCOMMANDPROCESSOR_H
