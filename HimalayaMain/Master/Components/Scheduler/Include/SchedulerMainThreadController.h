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
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "DataManager/Helper/Include/Types.h"

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
class HeatingStrategy;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class ProgramStepInfor
 */
/****************************************************************************/

typedef struct {
    QString stationID;       ///<  Definition/Declaration of variable stationID
    int durationInSeconds;       ///<  Definition/Declaration of variable durationInSeconds
    int temperature;       ///<  Definition/Declaration of variable temperature
    bool isPressure;       ///<  Definition/Declaration of variable isPressure
    bool isVacuum;       ///<  Definition/Declaration of variable isVacuum
    QString reagentGroup;       ///<  Definition/Declaration of variable reagentGroup
} ProgramStepInfor;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ControlCommandType_t
 */
/****************************************************************************/
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

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct ProgramStepInfor
 */
/****************************************************************************/
typedef struct
{
    QString ReagentGroupID;       ///<  Definition/Declaration of variable ReagentGroupID
    QString StationID;       ///<  Definition/Declaration of variable StationID
}ProgramStationInfo_t;

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct ProgramStepInfor
 */
/****************************************************************************/
typedef struct
{
    qint64 OvenStartHeatingTime;       ///<  Definition/Declaration of variable OvenStartHeatingTime
    qint64 DelayProgramStartTime;       ///<  Definition/Declaration of variable DelayProgramStartTime
    qint64 PauseStartTime;       ///<  Definition/Declaration of variable PauseStartTime
    qint64 CurStepSoakStartTime;       ///<  Definition/Declaration of variable CurStepSoakStartTime
}SchedulerTimeStamps_t;

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct ProgramStepInfor
 */
/****************************************************************************/
typedef struct
{
    QString StationID;       ///<  Definition/Declaration of variable StationID
    int UsedTimes;       ///<  Definition/Declaration of variable UsedTimes

}StationUseRecord_t;

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct ProgramStepInfor
 */
/****************************************************************************/
typedef struct
{
    CANObjectKeyLUT::CANObjectIdentifier_t FctModID;       ///<  Definition/Declaration of variable FctModID
    bool IsAvialable;       ///<  Definition/Declaration of variable IsAvialable
    bool IsWorking;       ///<  Definition/Declaration of variable IsWorking
    bool IsHealth;       ///<  Definition/Declaration of variable IsHealth
    qint64 StartTime;       ///<  Definition/Declaration of variable StartTime
    qint64 StopTime;       ///<  Definition/Declaration of variable StopTime
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
        QTimer m_TickTimer;       ///<  Definition/Declaration of variable m_TickTimer
        QMutex m_Mutex;       ///<  Definition/Declaration of variable m_Mutex
        QQueue<Global::CommandShPtr_t> m_SchedulerCmdQueue;       ///<  Definition/Declaration of variable m_SchedulerCmdQueue
        QMutex m_MutexDeviceControlCmdQueue;                                        ///< mutex for accessing m_DeviceControlCmdQueue
        QQueue<Scheduler::SchedulerCommandShPtr_t> m_DeviceControlCmdQueue;                     ///< Queue(Q2) for receive respond from SechedulerCommandProcessor
        QQueue<ProgramStationInfo_t> m_ProgramStationList;       ///<  Definition/Declaration of variable m_ProgramStationList
        QList<FunctionModuleStatus_t> m_FunctionModuleStatusList;       ///<  Definition/Declaration of variable m_FunctionModuleStatusList

        QThread* m_SchedulerCommandProcessorThread;       ///<  Definition/Declaration of variable m_SchedulerCommandProcessorThread
        SchedulerCommandProcessorBase* m_SchedulerCommandProcessor;       ///<  Definition/Declaration of variable m_SchedulerCommandProcessor
        CSchedulerStateMachine* m_SchedulerMachine;       ///<  Definition/Declaration of variable m_SchedulerMachine
        DeviceControl::IDeviceProcessing *mp_IDeviceProcessing;       ///<  Definition/Declaration of variable mp_IDeviceProcessing
        DataManager::CDataManager       *mp_DataManager;       ///<  Definition/Declaration of variable mp_DataManager
        int m_CurProgramStepIndex;        ///<  Definition/Declaration of variable m_CurProgramStepIndex
        int m_FirstProgramStepIndex;      ///<  Definition/Declaration of variable m_CurProgramStepIndex
        QString m_CurReagnetName;       ///<  Definition/Declaration of variable m_CurReagnetName
        ProgramStepInfor m_CurProgramStepInfo;       ///<  Definition/Declaration of variable m_CurProgramStepInfo
        QString m_CurProgramID;       ///<  Definition/Declaration of variable m_CurProgramID
        QString m_NewProgramID;       ///<  Definition/Declaration of variable m_NewProgramID
        qreal m_PressureAL;       ///<  Definition/Declaration of variable m_PressureAL
        qreal m_TempALLevelSensor;       ///<  Definition/Declaration of variable m_TempALLevelSensor
        qreal m_TempALTube1;       ///<  Definition/Declaration of variable m_TempALTube1
        qreal m_TempALTube2;       ///<  Definition/Declaration of variable m_TempALTube2
        qreal m_TempRV1;       ///<  Definition/Declaration of variable m_TempRV1
        qreal m_TempRV2;       ///<  Definition/Declaration of variable m_TempRV2
        RVPosition_t m_PositionRV;       ///<  Definition/Declaration of variable m_PositionRV
        qreal m_TempRTBottom;       ///<  Definition/Declaration of variable m_TempRTBottom
        qreal m_TempRTSide;       ///<  Definition/Declaration of variable m_TempRTSide
        qreal m_TempOvenBottom;       ///<  Definition/Declaration of variable m_TempOvenBottom
        qreal m_TempOvenTop;       ///<  Definition/Declaration of variable m_TempOvenTop
        quint16 m_OvenLidStatus;       ///<  Definition/Declaration of variable m_OvenLidStatus
        quint16 m_RetortLockStatus;       ///<  Definition/Declaration of variable m_RetortLockStatus
        QStringList m_UsedStationIDs;        ///< in a whole of program processing
        SchedulerTimeStamps_t m_TimeStamps;     ///<  Definition/Declaration of variable m_TimeStamps
        QList<QString> m_StationList;       ///<  Definition/Declaration of variable m_StationList
        QString m_ReagentIdOfLastStep;       ///<  Definition/Declaration of variable m_ReagentIdOfLastStep
        bool m_PauseToBeProcessed;       ///<  Definition/Declaration of variable m_PauseToBeProcessed
        int m_ProcessCassetteCount;       ///<  Definition/Declaration of variable m_ProcessCassetteCount
        quint32 m_EventKey; ///<  \todo: add mechanism to cash the key
        QSharedPointer<HeatingStrategy> mp_HeatingStrategy;       ///<  Definition/Declaration of variable mp_HeatingStrategy
        SchedulerMainThreadController();                                             ///< Not implemented.
        SchedulerMainThreadController(const SchedulerMainThreadController&);                      ///< Not implemented.
        const SchedulerMainThreadController& operator=(const SchedulerMainThreadController&);     ///< Not implemented.
        /****************************************************************************/
        /**
         * @brief Register commands.
         */
        /****************************************************************************/
        virtual void RegisterCommands();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function HardwareMonitor
         *
         *  \param StepID = const QString type parameter
         *
         *  \return from HardwareMonitor
         */
        /****************************************************************************/
        void HardwareMonitor(const QString& StepID);
        //process commands, like: program Start, Pause, Abort.
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function ProcessNonDeviceCommand
         *
         *  \return from ProcessNonDeviceCommand
         */
        /****************************************************************************/
        void ProcessNonDeviceCommand();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function PeekNonDeviceCommand
         *
         *  \return from PeekNonDeviceCommand
         */
        /****************************************************************************/
        ControlCommandType_t PeekNonDeviceCommand();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function DequeueNonDeviceCommand
         *
         *  \return from DequeueNonDeviceCommand
         */
        /****************************************************************************/
        void DequeueNonDeviceCommand();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetReagentName
         *
         *  \param ReagentID = const QString type parameter
         *
         *  \return from GetReagentName
         */
        /****************************************************************************/
        QString GetReagentName(const QString& ReagentID);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetReagentGroupID
         *
         *  \param ReagentID = const QString type parameter
         *
         *  \return from GetReagentGroupID
         */
        /****************************************************************************/
        QString GetReagentGroupID(const QString& ReagentID);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetScenarioBySchedulerState
         *
         *  \param State = SchedulerStateMachine_t type parameter
         *  \param ReagentGroup =  QString type parameter
         *
         *  \return from GetScenarioBySchedulerState
         */
        /****************************************************************************/
        qint32 GetScenarioBySchedulerState(SchedulerStateMachine_t State, QString ReagentGroup);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function IsCleaningReagent
         *
         *  \param ReagentID = const QString type parameter
         *
         *  \return from IsCleaningReagent
         */
        /****************************************************************************/
        bool IsCleaningReagent(const QString& ReagentID);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function UpdateStationReagentStatus
         *
         *  \return from UpdateStationReagentStatus
         */
        /****************************************************************************/
        void UpdateStationReagentStatus();

        /****************************************************************************/
        /*!
         *  \brief pop Command from Q2
         */
        /****************************************************************************/
         bool GetNextProgramStepInformation(const QString& ProgramID, ProgramStepInfor& ProgramStepInfor, bool onlyGetFirstProgramStepIndex = false);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetLeftProgramStepsNeededTime
          *
          *  \param ProgramID = const QString type parameter
          *  \param SpecifiedStepIndex =  int type parameter
          *
          *  \return from GetLeftProgramStepsNeededTime
          */
         /****************************************************************************/
         quint32 GetLeftProgramStepsNeededTime(const QString& ProgramID, int SpecifiedStepIndex = -1);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetCurrentProgramStepNeededTime
          *
          *  \param ProgramID = const QString type parameter
          *
          *  \return from GetCurrentProgramStepNeededTime
          */
         /****************************************************************************/
         quint32 GetCurrentProgramStepNeededTime(const QString& ProgramID);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function PrepareProgramStationList
          *
          *  \param ProgramID = const QString type parameter
          *  \param beginStep =  int type parameter
          *
          *  \return from PrepareProgramStationList
          */
         /****************************************************************************/
         bool PrepareProgramStationList(const QString& ProgramID, int beginStep = 0);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function SelectStationFromReagentID
          *
          *  \param ReagentID = const QString type parameter
          *  \param  =  type parameter
          *
          *  \return from SelectStationFromReagentID
          */
         /****************************************************************************/
         QString SelectStationFromReagentID(const QString& ReagentID,
                                           ListOfIDs_t& unusedStationIDs,
                                           QList<StationUseRecord_t>& usedStations,
                                           bool IsLastStep);

         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function SelectStationByReagent
          *
          *  \param pReagent = const DataManager::CReagent type parameter
          *  \param  =  type parameter
          *
          *  \return from SelectStationByReagent
          */
         /****************************************************************************/
         QString SelectStationByReagent(const DataManager::CReagent* pReagent,
                                                                             ListOfIDs_t& unusedStationIDs,
                                                                             QList<StationUseRecord_t>& usedStations,
                                                                             bool bFindNewestOne,
                                                                             Global::RMSOptions_t rmsMode) const;
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetStationIDFromProgramStep
          *
          *  \param ProgramStepIndex = int type parameter
          *
          *  \return from GetStationIDFromProgramStep
          */
         /****************************************************************************/
         QString GetStationIDFromProgramStep(int ProgramStepIndex);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetRVTubePositionByStationID
          *
          *  \param stationID = const QString type parameter
          *
          *  \return from GetRVTubePositionByStationID
          */
         /****************************************************************************/
         RVPosition_t GetRVTubePositionByStationID(const QString stationID);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetRVSealPositionByStationID
          *
          *  \param stationID = const QString type parameter
          *
          *  \return from GetRVSealPositionByStationID
          */
         /****************************************************************************/
         RVPosition_t GetRVSealPositionByStationID(const QString stationID);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function IsLastStep
          *
          *  \param currentStepIndex = int type parameter
          *  \param currentProgramID = const QString type parameter
          *
          *  \return from IsLastStep
          */
         /****************************************************************************/
         bool IsLastStep(int currentStepIndex,const QString& currentProgramID);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetSafeReagentStationList
          *
          *  \return from IsLastStep
          */
         /****************************************************************************/
         bool GetSafeReagentStationList(const QString& reagentGroupID, QList<QString>& stationList);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function WhichStepHasNoSafeReagent
          *
          *  \return from IsLastStep
          */
         /****************************************************************************/
         int WhichStepHasNoSafeReagent(const QString& ProgramID);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function CreateFunctionModuleStatusList
          *
          *  \param pList = QList<FunctionModuleStatus_t> type parameter
          *
          *  \return from CreateFunctionModuleStatusList
          */
         /****************************************************************************/
         bool CreateFunctionModuleStatusList(QList<FunctionModuleStatus_t>* pList);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function SetFunctionModuleWork
          *
          *  \param pList = QList<FunctionModuleStatus_t> type parameter
          *  \param ID =  CANObjectKeyLUT::CANObjectIdentifier_t type parameter
          *  \param isWorking =  bool type parameter
          *
          *  \return from SetFunctionModuleWork
          */
         /****************************************************************************/
         bool SetFunctionModuleWork(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID, bool isWorking);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function SetFunctionModuleHealth
          *
          *  \param pList = QList<FunctionModuleStatus_t> type parameter
          *  \param ID =  CANObjectKeyLUT::CANObjectIdentifier_t type parameter
          *  \param isHealth =  bool type parameter
          *
          *  \return from SetFunctionModuleHealth
          */
         /****************************************************************************/
         bool SetFunctionModuleHealth(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID, bool isHealth);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function SetFunctionModuleStarttime
          *
          *  \param pList = QList<FunctionModuleStatus_t> type parameter
          *  \param ID =  CANObjectKeyLUT::CANObjectIdentifier_t type parameter
          *
          *  \return from SetFunctionModuleStarttime
          */
         /****************************************************************************/
         bool SetFunctionModuleStarttime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function SetFunctionModuleStoptime
          *
          *  \param pList = QList<FunctionModuleStatus_t> type parameter
          *  \param ID =  CANObjectKeyLUT::CANObjectIdentifier_t type parameter
          *
          *  \return from SetFunctionModuleStoptime
          */
         /****************************************************************************/
         bool SetFunctionModuleStoptime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetFailedFunctionModuleList
          *
          *  \param pList = QList<FunctionModuleStatus_t> type parameter
          *
          *  \return from GetFailedFunctionModuleList
          */
         /****************************************************************************/
         QList<FunctionModuleStatus_t> GetFailedFunctionModuleList(QList<FunctionModuleStatus_t>* pList);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetFunctionModuleStartworkTime
          *
          *  \param pList = QList<FunctionModuleStatus_t> type parameter
          *  \param ID =  CANObjectKeyLUT::CANObjectIdentifier_t type parameter
          *
          *  \return from GetFunctionModuleStartworkTime
          */
         /****************************************************************************/
         qint64 GetFunctionModuleStartworkTime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID);
signals:
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of signal signalProgramStart
          */
         /****************************************************************************/
         void signalProgramStart(const QString& ProgramID);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of signal signalProgramPause
          */
         /****************************************************************************/
         void signalProgramPause();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of signal signalProgramAbort
          */
         /****************************************************************************/
         void signalProgramAbort();

private slots:
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot HandleIdleState
          */
         /****************************************************************************/
         void HandleIdleState(ControlCommandType_t ctrlCmd);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot HandleRunState
          */
         /****************************************************************************/
         //void HandleRunState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);
         void HandleRunState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot HandleErrorState
          */
         /****************************************************************************/
         void HandleErrorState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd, SchedulerStateMachine_t currentState);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot StepStart
          */
         /****************************************************************************/
         void StepStart();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot CheckStepTemperature
          *
          *  \return return from CheckStepTemperature
          */
         /****************************************************************************/
         bool CheckStepTemperature();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot CheckLevelSensorTemperature
          *
          *  \param targetTemperature = qreal type parameter
          *
          *  \return return from CheckLevelSensorTemperature
          */
         /****************************************************************************/
         bool CheckLevelSensorTemperature(qreal targetTemperature);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot SealingCheck
          */
         /****************************************************************************/
         void SealingCheck();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot HeatLevelSensor
          */
         /****************************************************************************/
         void HeatLevelSensor();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot MoveRV
          */
         /****************************************************************************/
         void MoveRV();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot Fill
          */
         /****************************************************************************/
         void Fill();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot StopFill
          */
         /****************************************************************************/
         void StopFill();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot Soak
          */
         /****************************************************************************/
         void Soak();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot Drain
          */
         /****************************************************************************/
         void Drain();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot StopDrain
          */
         /****************************************************************************/
         void StopDrain();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot Pressure
          */
         /****************************************************************************/
         void Pressure();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot Vaccum
          */
         /****************************************************************************/
         void Vaccum();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot AllStop
          */
         /****************************************************************************/
         void AllStop();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot Abort
          */
         /****************************************************************************/
         void Abort();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot SelfTest
          *
          *  \return return from SelfTest
          */
         /****************************************************************************/
         bool SelfTest(ReturnCode_t RetCode);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot Pause
          */
         /****************************************************************************/
         void Pause();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot MoveRVToInit
          */
         /****************************************************************************/
         void MoveRVToInit();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot ShutdownRetortHeater
          */
         /****************************************************************************/
         void ShutdownRetortHeater();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot ReleasePressure
          */
         /****************************************************************************/
         void ReleasePressure();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot ShutdownFailedHeater
          */
         /****************************************************************************/
         void ShutdownFailedHeater();

protected:

        /****************************************************************************/
        /**
         *  \brief Called when the base class received the Go signal.
         * This means that everything is fine and normal operation started.
         * We are running in our own thread now.
         */
        /****************************************************************************/
        virtual void OnGoReceived();

        /****************************************************************************/
        /**
         *  \brief Called when the base class received the Stop signal.
         * This means that normal operation will stop after processing this signal.
         * We are still running in our own thread.
         */
        /****************************************************************************/
        virtual void OnStopReceived();
        
        /****************************************************************************/
        /**
         *  \brief Called when power failure is to expect.
         */
        /****************************************************************************/
    virtual void OnPowerFail(const Global::PowerFailStages PowerFailStage);

        /****************************************************************************/
        /**
         *  \brief Called when a local/remote alarm raises.
         */
        /****************************************************************************/
//        void OnRaiseAlarmLocalRemote(Global::tRefType Ref, const HimalayaErrorHandler::CmdRaiseAlarm &Cmd);

        void OnProgramAction(Global::tRefType Ref, const MsgClasses::CmdProgramAction& Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnKeepCassetteCount
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param MsgClasses =  const type parameter
         *
         *  \return from OnKeepCassetteCount
         */
        /****************************************************************************/
        void OnKeepCassetteCount(Global::tRefType Ref, const MsgClasses::CmdKeepCassetteCount & Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnProgramSelected
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param MsgClasses =  const type parameter
         *
         *  \return from OnProgramSelected
         */
        /****************************************************************************/
        void OnProgramSelected(Global::tRefType Ref, const MsgClasses::CmdProgramSelected& Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnQuitAppShutdown
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param MsgClasses =  const type parameter
         *
         *  \return from OnQuitAppShutdown
         */
        /****************************************************************************/
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
         * @param   message
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
         *  \brief Create and configure your objects.
         */
        /****************************************************************************/
        virtual void CreateAndInitializeObjects();

        /****************************************************************************/
        /**
         *  \brief Cleanup and destroy your objects.
         */
        /****************************************************************************/
        virtual void CleanupAndDestroyObjects();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnActionCommandReceived
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param NetCommands =  const type parameter
         *
         *  \return from OnActionCommandReceived
         */
        /****************************************************************************/
        void OnActionCommandReceived(Global::tRefType Ref, const NetCommands::CmdSystemAction &Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function DataManager
         *
         *  \param p_DataManager = DataManager::CDataManager type parameter
         *
         *  \return from DataManager
         */
        /****************************************************************************/
        void DataManager(DataManager::CDataManager *p_DataManager);

        /****************************************************************************/
        /**
         *  \brief push Command to Q2
         */
        /****************************************************************************/
        void PushDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t CmdPtr);

        /****************************************************************************/
        /**
         *  \brief pop Command from Q2
         */
        /****************************************************************************/
         bool PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd);
        /****************************************************************************/
        /**
         *  \brief Get the time(in seconds) that Oven has been heated
         */
        /****************************************************************************/
        qint64 GetOvenHeatingTime();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetCurProgramID
         *
         *  \return from GetCurProgramID
         */
        /****************************************************************************/
        QString& GetCurProgramID() {return m_CurProgramID; }
		/****************************************************************************/
		/*!
		 *  \brief  Definition/Declaration of function GetCurProgramStepIndex
		 *
		 *  \return from GetCurProgramStepIndex
		 */
		/****************************************************************************/
		int	GetCurProgramStepIndex() { return m_CurProgramStepIndex; }
        /****************************************************************************/
        /**
         *  \brief push Command to Q2
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
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnDCLConfigurationFinished
         *
         *  \param RetCode = ReturnCode_t type parameter
         *
         *  \return from OnDCLConfigurationFinished
         */
        /****************************************************************************/
        void OnDCLConfigurationFinished(ReturnCode_t RetCode);
        //void DeviceInitComplete();

    };

} // EONS ::Scheduler

#endif
