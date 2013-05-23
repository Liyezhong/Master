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
#include "SelfTestStateMachine.h"
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "DataManager/Helper/Include/Types.h"

using namespace DeviceControl;

namespace MsgClasses
{
    class CmdKeepCassetteCount;
    class CmdProgramSelected;
}

namespace DataManager
{
    class CDataManager;
    class CReagent;
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
    CTRL_CMD_DRAIN,
    CTRL_CMD_NONE,
    CTRL_CMD_UNKNOWN
}ControlCommandType_t;

typedef struct
{
    QString ReagentGroupID;
    QString StationID;
}ProgramStationInfo_t;

typedef struct
{
    qint64 OvenStartHeatingTime;
    qint64 DelayProgramStartTime;
    qint64 PauseStartTime;
    qint64 CurStepSoakStartTime;
}SchedulerTimeStamps_t;

typedef struct
{
    QString StationID;
    int UsedTimes;

}StationUseRecord_t;



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
        QQueue<ProgramStationInfo_t> m_ProgramStationList;

        QThread* m_SchedulerCommandProcessorThread;
        SchedulerCommandProcessor* m_SchedulerCommandProcessor;
        SchedulerMachine* m_SchedulerMachine;
        ProgramStepStateMachine* mp_ProgramStepStateMachine;
        SelfTestStateMachine* mp_SelfTestStateMachine;
        DeviceControl::IDeviceProcessing *mp_IDeviceProcessing;
        DataManager::CDataManager       *mp_DataManager;
        //qint64 m_CurStepSoakStartTime;
        int m_CurProgramStepIndex;
        QString m_CurReagnetName;
        ProgramStepInfor m_CurProgramStepInfo;
        QString m_CurProgramID;
        QString m_NewProgramID;
        qreal m_PressureAL;
        qreal m_TempALLevelSensor;
        qreal m_TempALTube1;
        qreal m_TempALTube2;
        qreal m_TempRV1;
        qreal m_TempRV2;
        RVPosition_t m_PositionRV;
        qreal m_TempRTBottom;
        qreal m_TempRTSide;
        qreal m_TempOvenBottom;
        qreal m_TempOvenTop;
        QStringList m_UsedStationIDs;                                                 ///in a whole of program processing
        SchedulerTimeStamps_t m_TimeStamps;
        QList<QString> m_StationList;
        bool m_PauseToBeProcessed;
        int m_ProcessCassetteCount;
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
        ControlCommandType_t PeekNonDeviceCommand();
        void DequeueNonDeviceCommand();
        QString GetReagentName(const QString& ReagentID);
        QString GetReagentGroupID(const QString& ReagentID);
        bool IsCleaningReagent(const QString& ReagentID);
        void UpdateStationReagentStatus();

        /****************************************************************************/
        /**
         *
         * pop Command from Q2
         */
        /****************************************************************************/
         bool PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd);
         bool GetNextProgramStepInformation(const QString& ProgramID, ProgramStepInfor& ProgramStepInfor);
         quint32 GetLeftProgramStepsNeededTime(const QString& ProgramID, int SpecifiedStepIndex = -1);
         quint32 GetCurrentProgramStepNeededTime(const QString& ProgramID);
         bool PrepareProgramStationList(const QString& ProgramID);
         QString SelectStationFromReagentID(const QString& ReagentID,
                                           ListOfIDs_t& unusedStationIDs,
                                           QList<StationUseRecord_t>& usedStations,
                                           bool IsLastStep);

         const QString& SelectStationByReagent(const DataManager::CReagent* pReagent,
                                                                             ListOfIDs_t& unusedStationIDs,
                                                                             QList<StationUseRecord_t>& usedStations,
                                                                             bool bFindNewestOne,
                                                                             Global::RMSOptions_t rmsMode) const;
         QString GetStationIDFromProgramStep(int ProgramStepIndex);
         RVPosition_t GetRVTubePositionByStationID(const QString stationID);
         RVPosition_t GetRVSealPositionByStationID(const QString stationID);

signals:
         void signalProgramStart(const QString& ProgramID);
         void signalProgramPause();
         void signalProgramAbort();
         void signalRetortLock(bool IsLock);

private slots:
         void HandleIdleState(ControlCommandType_t ctrlCmd);
         //void HandleRunState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);
         void HandleRunState(ControlCommandType_t ctrlCmd, ReturnCode_t retCode);
         void StepStart();
         bool CheckStepTemperature();
         bool CheckLevelSensorTemperature(qreal targetTemperature);
         void HeatLevelSensor();
         void MoveRV();
         void Fill();
         void StopFill();
         void Soak();
         void Drain();
         void StopDrain();
         void Pressure();
         void Vaccum();
         void Abort();
         bool SelfTest(ReturnCode_t RetCode);
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
        void OnRetortLock(Global::tRefType Ref, const MsgClasses::CmdRetortLock& Cmd);
        void OnKeepCassetteCount(Global::tRefType Ref, const MsgClasses::CmdKeepCassetteCount & Cmd);
        void OnProgramSelected(Global::tRefType Ref, const MsgClasses::CmdProgramSelected& Cmd);
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
        /****************************************************************************/
        /**
         *
         * Get the time(in seconds) that Oven has been heated
         */
        /****************************************************************************/
        qint64 GetOvenHeatingTime();
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
