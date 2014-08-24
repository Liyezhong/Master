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
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "DataManager/Helper/Include/Types.h"

using namespace DeviceControl;

namespace MsgClasses
{
    class CmdKeepCassetteCount;
    class CmdProgramSelected;
    class CmdQuitAppShutdown;
    class CmdSavedServiceInfor;
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
    QString nextStationID;   ///< Definition/Declaration of variable stationID
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
    CTRL_CMD_RS_RV_GET_ORIGINAL_POSITION_AGAIN,
    CTRL_CMD_RS_STANDBY,
    CTRL_CMD_RS_STANDBY_WITHTISSUE,
    CTRL_CMD_RS_HEATINGERR30SRETRY,
    CTRL_CMD_RS_PRESSUREOVERRANGE_3SRETRY,
    CTRL_CMD_RS_TSENSORERR3MINRETRY,
    CTRL_CMD_RC_RESTART,
    CTRL_CMD_RC_REPORT,
    CTRL_CMD_RC_LEVELSENSOR_HEATING_OVERTIME,
    CTRL_CMD_RC_PRESSURE,
    CTRL_CMD_RC_VACUUM,
    CTRL_CMD_RC_FILLING,
    CTRL_CMD_RC_DRAINING,
    CTRL_CMD_RS_DRAINATONCE,
    CTRL_CMD_RC_BOTTLECHECK_I,
    CTRL_CMD_RS_FILLINGAFTERFLUSH,
    CTRL_CMD_RS_CHECK_BLOCKAGE,
    CTRL_CMD_RS_PAUSE,
    CTRL_CMD_RS_RV_WAITINGTEMPUP,
    CTRL_CMD_RS_TISSUE_PROTECT,
    CTRL_CMD_USER_RESPONSE_PAUSE_ALARM,
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
    qint64 ProposeSoakStartTime;       ///<  Definition/Declaration of variable ProposeSoakStartTime
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
    QString         ReagentGrpId;
    RVPosition_t    RvPos;
}BottlePosition_t;

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
/*!
 *  \brief  Definition of Heater Type
 */
/****************************************************************************/
typedef enum
{
    UNKNOWN,
    LEVELSENSOR,
    LATUBE1,
    LATUBE2,
    RETORT,
    OVEN,
    RV
} HeaterType_t;

/****************************************************************************/
/*!
 *  \brief struct for Received command
 */
/****************************************************************************/
typedef struct
{
    DeviceControl::ReturnCode_t retCode;
    QString                     cmdName;
} RecvCommand_t;

    /****************************************************************************/
    /**
     * @brief Controller class for the main scheduler thread.
     *
     */
    /****************************************************************************/
    class SchedulerMainThreadController : public Threads::ThreadController {
        Q_OBJECT

		// add "friend class TestSchedulerController" for UT
        friend class TestSchedulerController;
    private:
        QTimer m_TickTimer;       ///<  Definition/Declaration of variable m_TickTimer
        QMutex m_Mutex;       ///<  Definition/Declaration of variable m_Mutex
        QQueue<Global::CommandShPtr_t> m_SchedulerCmdQueue;       ///< Definition/Declaration of variable m_SchedulerCmdQueue
        QMutex m_MutexDeviceControlCmdQueue;                      ///< mutex for accessing m_DeviceControlCmdQueue
        QWaitCondition m_WaitCondition;                           ///< condition variable for DeviceControlCmdQueue
        QQueue<Scheduler::SchedulerCommandShPtr_t> m_DeviceControlCmdQueue;                     ///< Queue(Q2) for receive respond from SechedulerCommandProcessor
        QQueue<ProgramStationInfo_t> m_ProgramStationList;       ///<  Definition/Declaration of variable m_ProgramStationList
        QList<FunctionModuleStatus_t> m_FunctionModuleStatusList;       ///<  Definition/Declaration of variable m_FunctionModuleStatusList

        QThread* m_SchedulerCommandProcessorThread;       ///<  Definition/Declaration of variable m_SchedulerCommandProcessorThread
        SchedulerCommandProcessorBase*  m_SchedulerCommandProcessor;       ///<  Definition/Declaration of variable m_SchedulerCommandProcessor
        CSchedulerStateMachine* m_SchedulerMachine;       ///<  Definition/Declaration of variable m_SchedulerMachine
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
        quint32 m_EventKey;                                   ///< Current Event key
        ReturnCode_t m_CurErrEventID;                         ///< Current Event ID
        quint32 m_CurrentScenario;                            ///< Current Scenario
        QSharedPointer<HeatingStrategy> mp_HeatingStrategy;   ///< Definition/Declaration of variable mp_HeatingStrategy
        Global::tRefType    m_RefCleanup;                     ///< Command reference of the cleanup command
        int m_delayTime;
        bool m_IsInSoakDelay;                                 ///< Delay in Soak
        bool m_AllProgramCount;                            ///< count the all program
        bool m_IsPrecheckMoveRV;                            ///< precheck done move rv
        qint64 m_lastPVTime;                                  ///< Time for last PV operation
        bool m_completionNotifierSent;                        ///< Flag to indication if program completion is sent to Gui.
        bool m_IsCleaningProgram;                             ///< cleaning program run or not
        QMap<QString, QString> m_ProgramStatusFileMap;        ///< the map of program status
        BottlePosition_t    m_CurrentBottlePosition;          ///< the current BottlePosition for bottle check
        SchedulerStateMachine_t m_CurrentStepState;           ///< The current protocol(program) step, which is used to recovery from RC_Restart
        QVector<RecvCommand_t> m_RecvCommandList;             ///< Recevied command list, which were from DeviceControl
        bool m_hasParaffin;                                   ///< the program has paraffin
        bool m_IsReleasePressureOfSoakFinish;                 ///< wether release pressure when soak finish
        bool m_ReleasePressureSucessOfSoakFinish;             ///< release pressure sucess
        bool m_Is5MinPause;                                   ///< Instrument alarm when pausing exceed 5 minutes
        bool m_Is10MinPause;                                  ///< Local alarm when pausing exceed 10 minutes
        bool m_Is15MinPause;                                  ///< Remote alarm when pausing exceed 15 minutes

    private:
        SchedulerMainThreadController(const SchedulerMainThreadController&);                      ///< Not implemented.
        SchedulerMainThreadController& operator=(const SchedulerMainThreadController&);     ///< Not implemented.

        /****************************************************************************/
        /**
         * \brief CreateRebootFile.
         * \param p_StatusFile = QFile
         */
        /****************************************************************************/
        void CreateProgramStatusFile(QFile *p_StatusFile);

        /****************************************************************************/
        /**
         * \brief ReadProgramStatusFile
         * \param p_StatusFile = QFile
         */
        /****************************************************************************/
        void ReadProgramStatusFile(QFile *p_StatusFile);

        /****************************************************************************/
        /**
         * \brief UpdateProgramStatusFile
         * \param key = QString
         * \param value = QString
         */
        /****************************************************************************/
        void UpdateProgramStatusFile(const QString& key, const QString& value);

        /****************************************************************************/
        /**
         * \brief UpdateProgramStatusFile
         * \param key = QString
         * \param value = QString
         * \return from QString
         */
        /****************************************************************************/
        QString getTheProgramStatus(const QString& key);

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
#if 0
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function DequeueNonDeviceCommand
         *
         *  \return from DequeueNonDeviceCommand
         */
        /****************************************************************************/
        void DequeueNonDeviceCommand();
#endif
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
          *  \param unusedStationIDs
          *  \param usedStations
          *  \param IsLastStep
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
          *  \param unusedStationIDs
          *  \param usedStations
          *  \param bFindNewestOne
          *  \param rmsMode
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
          *  \brief  Definition/Declaration of function GetFunctionModuleStartworkTime
          *
          *  \param pList = QList<FunctionModuleStatus_t> type parameter
          *  \param ID =  CANObjectKeyLUT::CANObjectIdentifier_t type parameter
          *
          *  \return from GetFunctionModuleStartworkTime
          */
         /****************************************************************************/
         qint64 GetFunctionModuleStartworkTime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID);

         /****************************************************************************/
         /*!
          *  \brief Check no-signal error for temperature sensors
          *
          *  \param Scenario - current scenario
          *  \param HwTemp - current temperature of sensor
          *
          *  \return ture - no error, false - error was gotten
          */
         /****************************************************************************/
         bool CheckRetortTempSensorNoSignal(quint32 Scenario, qreal HWTemp);

         /****************************************************************************/
         /*!
          *  \brief Check if current over range error occurs
          *
          *    \param Scenario - current scenario
          *
          *  \return void
          */
         /****************************************************************************/
         void CheckTempSensorCurrentOverRange(quint32 Scenario);
         /****************************************************************************/
         /*!
          *  \brief Enable pause button
          *
          *    \param bEnable - enabled or disabled
          *
          *  \return void
          */
         /****************************************************************************/
         void EnablePauseButton(bool bEnable);
signals:
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of signal signalProgramStart
          *  \param ProgramID
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

         /****************************************************************************/
         /*!
          *  \brief  signal to resume paused/error program
          */
         /****************************************************************************/
         void NotifyResume();

private slots:
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot HandleIdleState
          */
         /****************************************************************************/
         void HandleIdleState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);

         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot HandleRunState
          */
         /****************************************************************************/
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
          *  \brief  Definition/Declaration of slot DevProcDestroyed
          */
         /****************************************************************************/
         void DevProcDestroyed();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot ReportGetServiceInfo
          *  \param ReturnCode = Return Code
          *  \param ModuleInfo = module information
          *  \param deviceName = which device got the service information
          */
         /****************************************************************************/
         void ReportGetServiceInfo(ReturnCode_t ReturnCode, const DataManager::CModule &ModuleInfo, const QString& deviceName);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot ShutdownRetortHeater
          */
         /****************************************************************************/
         void ResetActiveCarbonFilterLifetime();

         /****************************************************************************/
         /*!
          *  \brief  Slot for reporting Filling Timout 2Min;
          */
         /****************************************************************************/
         void OnReportFillingTimeOut2Min();

         /****************************************************************************/
         /*!
          *  \brief  Slot for reporting Draining Timout 2Min;
          */
         /****************************************************************************/
         void OnReportDrainingTimeOut2Min();
         /****************************************************************************/
         /*!
          *  \brief  Slot for HW error occuring;
          */
         /****************************************************************************/
         void OnSystemError();
         /****************************************************************************/
         /*!
          *  \brief  Slot for restart program from HW error;
          */
         /****************************************************************************/
         void OnEnterRcRestart();

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
         *  \param  PowerFailStage
         */
        /****************************************************************************/
        virtual void OnPowerFail(const Global::PowerFailStages PowerFailStage);

        /****************************************************************************/
        /**
         *  \brief Called when a local/remote alarm raises.
         */
        /****************************************************************************/
//        void OnRaiseAlarmLocalRemote(Global::tRefType Ref, const HimalayaErrorHandler::CmdRaiseAlarm &Cmd);

         /****************************************************************************/
         /**
          *   \brief Definition/Declaration of function OnProgramAction
          *   \param Ref
          *   \param Cmd
          */
         /****************************************************************************/
        void OnProgramAction(Global::tRefType Ref, const MsgClasses::CmdProgramAction& Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnKeepCassetteCount
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdKeepCassetteCount type parameter
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
         *  \param Cmd =  MsgClasses::CmdProgramSelected type parameter
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
         *  \param Cmd =  MsgClasses::CmdQuitAppShutdown type parameter
         *
         *  \return from OnQuitAppShutdown
         */
        /****************************************************************************/
        void OnQuitAppShutdown(Global::tRefType Ref, const MsgClasses::CmdQuitAppShutdown& Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnSavedServiceInfor
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdSavedServiceInfor type parameter
         *
         *  \return from OnSavedServiceInfor
         */
        /****************************************************************************/
        void OnSavedServiceInfor(Global::tRefType Ref, const MsgClasses::CmdSavedServiceInfor & Cmd);
    public:
        /****************************************************************************/
        /**
         * @brief Constructor.
         *
         * @iparam   TheHeartBeatSource    Logging source to be used.
         */
        /****************************************************************************/
        SchedulerMainThreadController(
            Global::gSourceType TheHeartBeatSource);

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
#if 1
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_ID_DEBUG_MESSAGE,Global::tTranslatableStringList()<<message);
#endif
        }
        /****************************************************************************/
        /**
         * @brief   return current Event key
         *
         * @return  void
         */
        /****************************************************************************/
        quint32 GetEventKey(){ return m_EventKey; }

        /****************************************************************************/
        /**
         * @brief   return current Event ID when an error occurs
         *
         * @return  void
         */
        /****************************************************************************/
        ReturnCode_t GetCurErrEventID(){ return m_CurErrEventID; }
        /****************************************************************************/
        /**
         * @brief raise error to event handler
         *
         *  \iparam    EventKey
         *  \iparam    EventID
         *  \iparam    Scenario
         *  \iparam    ActionResult
         *  \iparam    Active
         */
         /****************************************************************************/
        virtual void RaiseError(const quint32 EventKey, ReturnCode_t EventID, const quint32 Scenario,
                                  const bool ActionResult, const bool Active = true)
        {
            if(EventKey == 0)
            {
                // We only update current event ID when current status is NOT error.
                SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
                if (SM_ERROR != (currentState & 0xFF))
                {
                    m_CurErrEventID = EventID;
                }
                // For filling and draining states, we need clean up the command list container at first.
                QVector<RecvCommand_t> emptyVector;
                m_RecvCommandList.swap(emptyVector);

                Global::EventObject::Instance().RaiseEvent(EventKey, EventID, Scenario, ActionResult,Active,
                                                       Global::tTranslatableStringList()<<QString("(%1 %2)").arg(EventID).arg(Scenario));
            }
            else
            {
                Global::EventObject::Instance().RaiseEvent(EventKey, EventID, Scenario, ActionResult,Active);
            }
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
         *  \param Cmd =  NetCommands::CmdSystemAction type parameter
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
         *  \param CmdPtr
         */
        /****************************************************************************/
        void PushDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t CmdPtr);

        /****************************************************************************/
        /**
         *  \brief pop Command from Q2
         *  \param PtrCmd
         *  \return from PopDeviceControlCmdQueue of bool
         */
        /****************************************************************************/
         bool PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd);

        /****************************************************************************/
        /**
         *  \brief Definition/Declaration of function PopDeviceControlCmdQueue
         *  \iparam PtrCmd
         *  \iparam CmdName
         *  \return void
         */
        /****************************************************************************/
         void PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd, const QString& CmdName);
        /****************************************************************************/
        /**
         *  \brief Get the time(in seconds) that Oven has been heated
         *  \return from GetOvenHeatingTime of qint64
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
        /*!
         *  \brief  Definition/Declaration of function GetRVTubePositionByStationID
         *
         *  \param stationID = const QString type parameter
         *
         *  \return from GetRVTubePositionByStationID
         */
        /****************************************************************************/
        RVPosition_t GetRVTubePositionByStationID(const QString& stationID);

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetRVSealPositionByStationID
         *
         *  \param stationID = const QString type parameter
         *
         *  \return from GetRVSealPositionByStationID
         */
        /****************************************************************************/
        RVPosition_t GetRVSealPositionByStationID(const QString& stationID);

        /****************************************************************************/
        /**
         *  \brief Get Scheduler Commnad Processor
         *  \return pointer to SchedulerCommandProcessor
         */
        /****************************************************************************/
        SchedulerCommandProcessorBase* GetSchedCommandProcessor() const { return m_SchedulerCommandProcessor; }

        /****************************************************************************/
        /**
         *  \brief Get Heating Strategy
         *  \return pointer to SchedulerCommandProcessor
         */
        /****************************************************************************/
        QSharedPointer<HeatingStrategy> GetHeatingStrategy() const { return mp_HeatingStrategy; }

        /****************************************************************************/
        /**
         *  \brief Get current secenario in Scheduler
         *  \return quint32 - current scenario
         */
        /****************************************************************************/
        quint32 GetCurrentScenario() const {  return m_CurrentScenario; }

        /****************************************************************************/
        /*!
         *  \brief  Check the oven lock status
         *  \return qint16, 0 - lose, 1 - open
         */
        /****************************************************************************/
        inline quint16 GetOvenLockStatus() const {    return m_OvenLidStatus; }

        /****************************************************************************/
        /**
         *  \brief  Send out Error message
         *  \param  Event Id
         *  \return void
         */
        /****************************************************************************/
        void SendOutErrMsg(ReturnCode_t EventId)
        {
            RaiseError(0,EventId, m_CurrentScenario, true);
            m_SchedulerMachine->SendErrorSignal();

        }

        /****************************************************************************/
        /**
         *  \brief  Bottle check for each station
         *  \param  bottleSeq sequence of the bottle
         *  \return bool, true - continue; false - done;
         */
        /****************************************************************************/
        bool BottleCheck(quint32 bottleSeq);

        /****************************************************************************/
        /*!
         *  \brief  Move Rotary Valve tube to the target position
         *  \param  type - type of RV moving (tubue position or sealing position)
         *  \return void
         */
        /****************************************************************************/
        void MoveRV(qint16 type);

        /****************************************************************************/
        /*!
         *  \brief  IS Rotary Valve target position
         *  \param  type - type of RV moving (tubue position or sealing position)
         *  \return bool
         */
        /****************************************************************************/
        bool IsRVRightPosition(qint16 type);

        /****************************************************************************/
        /*!
         *  \brief  Get the tpe of failed heater
         *  \param  void
         *  \return Heater type
         *
         */
        /****************************************************************************/
        HeaterType_t GetFailerHeaterType();

        /****************************************************************************/
        /*!
         *  \brief  Shutting down failed heaters
         *  \param  void
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool ShutdownFailedHeaters();

        /****************************************************************************/
        /*!
         *  \brief  Restart failed heaters
         *  \param  void
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool RestartFailedHeaters();

        /****************************************************************************/
        /*!
         *  \brief  Check temperature modules current range
         *  \param  interval - interval of time (in seconds)
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool CheckSlaveTempModulesCurrentRange(quint8 interval);

        /****************************************************************************/
        /*!
         *  \brief  Check temperature modules temperature
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool CheckSensorTempOverange();

        /****************************************************************************/
        /*!
         *  \brief  Check is cleaning progaram run
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        inline bool IsCleaningProgram() const { return m_IsCleaningProgram; }

        /****************************************************************************/
        /*!
         *  \brief  Get the program station list
         *  \return Program station list queue
         *
         */
        /****************************************************************************/
        QQueue<ProgramStationInfo_t> GetProgramStationList() const { return m_ProgramStationList; }

        /****************************************************************************/
        /*!
         *  \brief	Check if program has paraffin reagent 
         *  \return bool 
         *
         */
        /****************************************************************************/
        inline bool HasParaffinReagent() const { return m_hasParaffin;  }

        /****************************************************************************/
        /*!
         *  \brief  Filling for Rs_Tissue_Protect
         *  \param  QString StationID
         *  \return void
         */
        /****************************************************************************/
        void FillRsTissueProtect(const QString& StationID);

        /****************************************************************************/
        /*!
         *  \brief  Send Stop Filling for Rs_Tissue_Protect to GUI
         *  \param  QString StationID
         *  \return void
         */
        /****************************************************************************/
        void StopFillRsTissueProtect(const QString& StationID);

        /****************************************************************************/
        /*!
         *  \brief  Get the command list from Device Control
         *  \param  void
         *  \return Reference of received command list
         */
        /****************************************************************************/
        QVector<RecvCommand_t>& GetRecvCommandList() { return m_RecvCommandList; }

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

        /****************************************************************************/
        /*!
         *  \brief  Protocol filling
         *  \param  void
         *  \return void
         */
        /****************************************************************************/
        void Fill();

        /****************************************************************************/
        /*!
         *  \brief  slot to handle the logic on stopping filling
         */
        /****************************************************************************/
        void OnStopFill();

        /****************************************************************************/
        /*!
         *  \brief  slot to handle the logic for entrance of PSSM_PROCESSING(SOAK)
         */
        /****************************************************************************/
        void OnEnterPssmProcessing();

        /****************************************************************************/
        /*!
         *  \brief  slot to handle Draing
         */
        /****************************************************************************/
        void Drain();

        /****************************************************************************/
        /*!
         *  \brief  slot to stop draining
         */
        /****************************************************************************/
        void OnStopDrain();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot Pressure
         */
        /****************************************************************************/
        void Pressure();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot Pressure
         */
        /****************************************************************************/
        void HighPressure();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot Vaccum
         */
        /****************************************************************************/
        void Vaccum();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot RC_Drain
         */
        /****************************************************************************/
        void RCDrain();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot RcBottleCheckI
         */
        /****************************************************************************/
        void RcBottleCheckI();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot ReleasePressure
         */
        /****************************************************************************/
        void ReleasePressure();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot EnablePauseButton
         */
        /****************************************************************************/
        void EnablePauseButton();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot DisablePauseButton
         */
        /****************************************************************************/
        void DisablePauseButton();
    };

} // EONS ::Scheduler

#endif
