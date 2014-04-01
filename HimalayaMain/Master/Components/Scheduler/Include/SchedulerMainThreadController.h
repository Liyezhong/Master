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
#include <QQueue>
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                 Project Specific
#include "Threads/Include/ThreadController.h"
//#include <HimalayaErrorHandler/Include/Commands/CmdRaiseAlarm.h>
#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "NetCommands/Include/CmdSystemAction.h"
#include "SchedulerMachine.h"
#include "ProgramStepStateMachine.h"
#include "SelfTestStateMachine.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "DataManager/Helper/Include/Types.h"
#include "Scenario.h"

using namespace DeviceControl;

namespace MsgClasses
{
    class CmdKeepCassetteCount;
    class CmdProgramSelected;
    class CmdQuitAppShutdown;
}

namespace DataManager
{
    class CDataManager;
    class CReagent;
}

namespace Scheduler {

class SchedulerCommandProcessorBase;
class CSchedulerStateMachine;

typedef struct {
    QString stationID;
    int durationInSeconds;
    int temperature;
    bool isPressure;
    bool isVacuum;
    QString reagentGroup;
} ProgramStepInfor;

typedef enum
{
    CTRL_CMD_START,
    CTRL_CMD_PAUSE,
    CTRL_CMD_ABORT,
    CTRL_CMD_SET_REMOTE_ALARM,
    CTRL_CMD_CLEAR_REMOTE_ALARM,
    CTRL_CMD_DRAIN,
    CTRL_CMD_QUITAPP,
    CTRL_CMD_SHUTDOWN,
    CTRL_CMD_RS_GET_ORIGINAL_POSITION_AGAIN,
    CTRL_CMD_RC_REPORT,
    CTRL_CMD_RC_RESTART,
    CTRL_CMD_RS_STANDBY,
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

typedef struct
{
    CANObjectKeyLUT::CANObjectIdentifier_t FctModID;
    bool IsAvialable;
    bool IsWorking;
    bool IsHealth;
    qint64 StartTime;
    qint64 StopTime;
}FunctionModuleStatus_t;


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
        QList<FunctionModuleStatus_t> m_FunctionModuleStatusList;

        QThread* m_SchedulerCommandProcessorThread;
        SchedulerCommandProcessorBase* m_SchedulerCommandProcessor;
        CSchedulerStateMachine* m_SchedulerMachine;
        DeviceControl::IDeviceProcessing *mp_IDeviceProcessing;
        DataManager::CDataManager       *mp_DataManager;
        int m_CurProgramStepIndex, m_FirstProgramStepIndex;
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
        quint16 m_OvenLidStatus;
        quint16 m_RetortLockStatus;
        QStringList m_UsedStationIDs;                                                 ///in a whole of program processing
        SchedulerTimeStamps_t m_TimeStamps;
        QList<QString> m_StationList;
        QString m_ReagentIdOfLastStep;
        bool m_PauseToBeProcessed;
        int m_ProcessCassetteCount;
        quint32 m_EventKey; //todo: add mechanism to cash the key
        SchedulerMainThreadController();                                             ///< Not implemented.
        SchedulerMainThreadController(const SchedulerMainThreadController&);                      ///< Not implemented.
        const SchedulerMainThreadController& operator=(const SchedulerMainThreadController&);     ///< Not implemented.
        /****************************************************************************/
        /**
         * @brief Register commands.
         */
        /****************************************************************************/
        virtual void RegisterCommands();
        void HardwareMonitor(const QString& StepID);
        //process commands, like: program Start, Pause, Abort.
        void ProcessNonDeviceCommand();
        ControlCommandType_t PeekNonDeviceCommand();
        void DequeueNonDeviceCommand();
        QString GetReagentName(const QString& ReagentID);
        QString GetReagentGroupID(const QString& ReagentID);
        qint32 GetScenarioBySchedulerState(SchedulerStateMachine_t State, QString ReagentGroup);
        bool IsCleaningReagent(const QString& ReagentID);
        void UpdateStationReagentStatus();

        /****************************************************************************/
        /**
         *
         * pop Command from Q2
         */
        /****************************************************************************/
         bool PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd);
         bool GetNextProgramStepInformation(const QString& ProgramID, ProgramStepInfor& ProgramStepInfor, bool onlyGetFirstProgramStepIndex = false);
         quint32 GetLeftProgramStepsNeededTime(const QString& ProgramID, int SpecifiedStepIndex = -1);
         quint32 GetCurrentProgramStepNeededTime(const QString& ProgramID);
         bool PrepareProgramStationList(const QString& ProgramID, int beginStep = 0);
         QString SelectStationFromReagentID(const QString& ReagentID,
                                           ListOfIDs_t& unusedStationIDs,
                                           QList<StationUseRecord_t>& usedStations,
                                           bool IsLastStep);

         QString SelectStationByReagent(const DataManager::CReagent* pReagent,
                                                                             ListOfIDs_t& unusedStationIDs,
                                                                             QList<StationUseRecord_t>& usedStations,
                                                                             bool bFindNewestOne,
                                                                             Global::RMSOptions_t rmsMode) const;
         QString GetStationIDFromProgramStep(int ProgramStepIndex);
         RVPosition_t GetRVTubePositionByStationID(const QString stationID);
         RVPosition_t GetRVSealPositionByStationID(const QString stationID);
         bool IsLastStep(int currentStepIndex,const QString& currentProgramID);
         bool GetSafeReagentStationList(const QString& reagentGroupID, QList<QString>& stationList);
         int WhichStepHasNoSafeReagent(const QString& ProgramID);
         bool CreateFunctionModuleStatusList(QList<FunctionModuleStatus_t>* pList);
         bool SetFunctionModuleWork(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID, bool isWorking);
         bool SetFunctionModuleHealth(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID, bool isHealth);
         bool SetFunctionModuleStarttime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID);
         bool SetFunctionModuleStoptime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID);
         QList<FunctionModuleStatus_t> GetFailedFunctionModuleList(QList<FunctionModuleStatus_t>* pList);
         qint64 GetFunctionModuleStartworkTime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID);
signals:
         void signalProgramStart(const QString& ProgramID);
         void signalProgramPause();
         void signalProgramAbort();

private slots:
         void HandleIdleState(ControlCommandType_t ctrlCmd);
         //void HandleRunState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);
         void HandleRunState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);
         void HandleErrorState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd, SchedulerStateMachine_t currentState);
         void StepStart();
         bool CheckStepTemperature();
         bool CheckLevelSensorTemperature(qreal targetTemperature);
         void SealingCheck();
         void HeatLevelSensor();
         void MoveRV();
         void Fill();
         void StopFill();
         void Soak();
         void Drain();
         void StopDrain();
         void Pressure();
         void Vaccum();
         void AllStop();
         void Abort();
         bool SelfTest(ReturnCode_t RetCode);
         void Pause();
         void MoveRVToInit();
         void ShutdownRetortHeater();
         void ReleasePressure();
         void ShutdownFailedHeater();

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
    virtual void OnPowerFail(const Global::PowerFailStages PowerFailStage);

        /****************************************************************************/
        /**
         * Called when a local/remote alarm raises.
         */
        /****************************************************************************/
//        void OnRaiseAlarmLocalRemote(Global::tRefType Ref, const HimalayaErrorHandler::CmdRaiseAlarm &Cmd);

        void OnProgramAction(Global::tRefType Ref, const MsgClasses::CmdProgramAction& Cmd);
        void OnKeepCassetteCount(Global::tRefType Ref, const MsgClasses::CmdKeepCassetteCount & Cmd);
        void OnProgramSelected(Global::tRefType Ref, const MsgClasses::CmdProgramSelected& Cmd);
        void OnQuitAppShutdown(Global::tRefType Ref, const MsgClasses::CmdQuitAppShutdown& Cmd);
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
         * @brief log debug message.
         *
         * @param   log message
         */
        /****************************************************************************/
        inline void LogDebug(QString message)
        {
            qDebug() << message;
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_ID_DEBUG_MESSAGE,Global::tTranslatableStringList()<<message);
        }

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

        /****************************************************************************/
        /**
         *
         * push Command to Q2
         */
        /****************************************************************************/
        void SetSchedCommandProcessor( Scheduler::SchedulerCommandProcessorBase* pSchedCmdProcessor ) { m_SchedulerCommandProcessor = pSchedCmdProcessor; }
    public slots:

        /****************************************************************************/
        /**
         * @brief For advancing the time
         */
        /****************************************************************************/
        void OnTickTimer();
        void OnDCLConfigurationFinished(ReturnCode_t RetCode);
        //void DeviceInitComplete();

    };

} // EONS ::Scheduler

#endif
