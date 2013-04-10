// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Include/SchedulerMainThreadController.h
 *
 *  @brief
 *  Definition of class Scheduler::Schedule.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-06
 *  $Author:    $ Abe Yang
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
#ifndef SCHEDULER_MAINTHREADCONTROLLER_H
#define SCHEDULER_MAINTHREADCONTROLLER_H

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                  From Qt-Library
#include <QTimer>
#include <QQueue>
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                 Project Specific
#include "Threads/Include/ThreadController.h"
#include <HimalayaErrorHandler/Include/Commands/CmdRaiseAlarm.h>
#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortLock.h"
#include "NetCommands/Include/CmdSystemAction.h"
#include "SchedulerMachine.h"
#include "ProgramStepStateMachine.h"
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
using namespace DeviceControl;

namespace DataManager
{
    class CDataManager;
}

namespace Scheduler {

class SchedulerCommandProcessor;
class SchedulerMachine;

typedef struct {
    QString stationID;
    int durationInSeconds;
    int temperature;
    bool isPressure;
    bool isVacuum;
} ProgramStepInfor;

typedef enum
{
    CTRL_CMD_START,
    CTRL_CMD_PAUSE,
    CTRL_CMD_ABORT,
    CTRL_CMD_LOCK_RETORT,
    CTRL_CMD_UNLOCK_RETORT,
    CTRL_CMD_SET_REMOTE_ALARM,
    CTRL_CMD_CLEAR_REMOTE_ALARM,
    CTRL_CMD_NONE,
    CTRL_CMD_UNKNOWN
}ControlCommandType_t;


    /****************************************************************************/
    /**
     * @brief Controller class for the main scheduler thread.
     *
     */
    /****************************************************************************/
    class SchedulerMainThreadController : public Threads::ThreadController {
        Q_OBJECT

    private:
        QTimer m_TickTimer;
        QMutex m_Mutex;
        QQueue<Global::CommandShPtr_t> m_SchedulerCmdQueue;
        QMutex m_MutexDeviceControlCmdQueue;                                        /// < mutex for accessing m_DeviceControlCmdQueue
        QQueue<Scheduler::SchedulerCommandShPtr_t> m_DeviceControlCmdQueue;                     ///< Queue(Q2) for receive respond from SechedulerCommandProcessor

        QThread* m_SchedulerCommandProcessorThread;
        SchedulerCommandProcessor* m_SchedulerCommandProcessor;
        SchedulerMachine* m_SchedulerMachine;
        ProgramStepStateMachine* mp_ProgramStepStateMachine;
        DeviceControl::IDeviceProcessing *mp_IDeviceProcessing;
        DataManager::CDataManager       *mp_DataManager;
        qint64 m_CurStepSoakStartTime;
        QString m_CurProgramStepID;
        QString m_CurProgramID;
        QString m_NewProgramID;
        qreal m_PressureAL;
        qreal m_TempALLevelSensor;
        qreal m_TempALTube1;
        qreal m_TempALTube2;
        qreal m_TempRV;
        RVPosition_t m_PositionRV;
        qreal m_TempRTBottom;
        qreal m_TempRTSide;
        qreal m_TempOvenBottom;
        qreal m_TempOvenTop;
        SchedulerMainThreadController();                                             ///< Not implemented.
        SchedulerMainThreadController(const SchedulerMainThreadController&);                      ///< Not implemented.
        const SchedulerMainThreadController& operator=(const SchedulerMainThreadController&);     ///< Not implemented.
        /****************************************************************************/
        /**
         * @brief Register commands.
         */
        /****************************************************************************/
        virtual void RegisterCommands();
        void HardwareMonitor(IDeviceProcessing* pIDP, const QString& StepID);
        //process commands, like: program Start, Pause, Abort.
        void ProcessNonDeviceCommand();
        ControlCommandType_t ReceiveNonDeviceCommand();
        QString GetStationIDFromReagentID(const QString& ReagentID, bool IsLastStep = false);
        /****************************************************************************/
        /**
         *
         * pop Command from Q2
         */
        /****************************************************************************/
         bool PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd);
         bool GetNextProgramStepInformation(const QString& ProgramID, ProgramStepInfor& ProgramStepInfor);
         quint32 GetLeftProgramNeededTime(const QString& ProgramID);

signals:
         void signalProgramStart(const QString& ProgramID);
         void signalProgramPause();
         void signalProgramAbort();
         void signalRetortLock(bool IsLock);

private slots:
         void ProgramStart(const QString& ProgramID);
         void ProgramPause();
         void ProgramAbort();
         void processNextProgramStep();
         void HandleIdleState(ControlCommandType_t ctrlCmd);
         //void HandleRunState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);
         void HandleRunState(ControlCommandType_t ctrlCmd, ReturnCode_t retCode);
         void StepStart();
         bool CheckStepTemperature();
         bool CheckLevelSensorTemperature(qreal targetTemperature);
         void HeatLevelSensor();
         void MoveRV();
         void Fill();
         void Soak();
         void Drain();
    protected:

        /****************************************************************************/
        /**
         * Called when the base class received the \ref Go signal.
         *
         * This means that everything is fine and normal operation started.
         * We are running in our own thread now.
         */
        /****************************************************************************/
        virtual void OnGoReceived();

        /****************************************************************************/
        /**
         * Called when the base class received the \ref Stop signal.
         *
         * This means that normal operation will stop after processing this signal.
         * We are still running in our own thread.
         */
        /****************************************************************************/
        virtual void OnStopReceived();
        
        /****************************************************************************/
        /**
         * Called when power failure is to expect.
         */
        /****************************************************************************/
        virtual void OnPowerFail();

        /****************************************************************************/
        /**
         * Called when a local/remote alarm raises.
         */
        /****************************************************************************/
        void OnRaiseAlarmLocalRemote(Global::tRefType Ref, const HimalayaErrorHandler::CmdRaiseAlarm &Cmd);

        void OnProgramAction(Global::tRefType Ref, const MsgClasses::CmdProgramAction& Cmd);
        void OnOnRetortLock(Global::tRefType Ref, const MsgClasses::CmdRetortLock& Cmd);

    public:
        /****************************************************************************/
        /**
         * @brief Constructor.
         *
         * @iparam   TheHeartBeatSource    Logging source to be used.
         */
        /****************************************************************************/
        SchedulerMainThreadController(
            Global::gSourceType TheHeartBeatSource
        );

        /****************************************************************************/
        /**
         * @brief Destructor.
         */
        /****************************************************************************/
        virtual ~SchedulerMainThreadController();

        /****************************************************************************/
        /**
         * Create and configure your objects.
         */
        /****************************************************************************/
        virtual void CreateAndInitializeObjects();

        /****************************************************************************/
        /**
         * Cleanup and destroy your objects.
         */
        /****************************************************************************/
        virtual void CleanupAndDestroyObjects();

        void OnActionCommandReceived(Global::tRefType Ref, const NetCommands::CmdSystemAction &Cmd);
        void DataManager(DataManager::CDataManager *p_DataManager);

        /****************************************************************************/
        /**
         *
         * push Command to Q2
         */
        /****************************************************************************/
        void PushDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t CmdPtr);
    public slots:

        /****************************************************************************/
        /**
         * @brief For advancing the time
         */
        /****************************************************************************/
        void OnTickTimer();
        void OnDCLConfigurationFinished(ReturnCode_t RetCode, IDeviceProcessing* pIDP);
        //void DeviceInitComplete();

    };

} // EONS ::Scheduler

#endif
