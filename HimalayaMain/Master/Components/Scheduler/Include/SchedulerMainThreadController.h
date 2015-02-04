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
#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "NetCommands/Include/CmdSystemAction.h"
#include "SchedulerMachine.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "DataManager/Helper/Include/Types.h"
#include "Scheduler/Include/ProgramStatusInfor.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Global/Include/Commands/AckOKNOK.h"
#include "Scheduler/Include/SchedulerLogging.h"
#include "Scheduler/Include/EventScenarioErrorXMLInfo.h"

using namespace DeviceControl;

namespace MsgClasses
{
    class CmdKeepCassetteCount;
    class CmdProgramSelected;
    class CmdQuitAppShutdown;
    class CmdSavedServiceInfor;
    class CmdParaffinMeltPointChanged;
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

const qint64 TIME_FOR_FIX_TIME = 195;    ///< seconds for fix time

#define TIME_FOR_CLEANING_DRY_STEP            720    ///< seconds spending in dry step
#define TIME_FOR_HEATING_LEVEL_SENSOR         30     ///< seconds for heating level sensor
#define TIME_FOR_FILLING                      60     ///< seconds for filling
#define TIME_FOR_MOVE_SEAL                    3      ///< seconds for move seal
#define TIME_FOR_PRESSURE_CHECK               15     ///< seconds for pressure check before move tube
#define TIME_FOR_MOVE_TUBE                    5      ///< seconds for move tube
#define TIME_FOR_DRAIN                        60     ///< seconds for draing
#define TIME_FOR_MOVE_NEXT_TUBE               8      ///< seconds for move next tube

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
    qint64 ProposeSoakStartTime;       ///<  SoakStartTime, exclude the delay time
    qint64 SystemErrorStartTime;       ///<  Time point when system error occurring
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
    QString         ReagentGrpId;           //!< reagent group ID
    QString         StationID;              //!< the current StationID
    RVPosition_t    RvPos;                  //!< RV position
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
    LATUBE1ABNORMAL,
    LATUBE2ABNORMAL,
    RETORT,
    RETORT_NOSIGNAL,
    OVEN,
    RV,
    ASB5,
    FAN
} HeaterType_t;

/****************************************************************************/
/*!
 * \brief Definition of power failure Type
 */
/****************************************************************************/
typedef enum
{
    POWERFAILURE_INIT,
    POWERFAILURE_RUN
} PowerFailureStep_t;


/****************************************************************************/
/*!
 *  \brief struct for ProgramEndTime
 */
/****************************************************************************/
typedef struct
{
    bool    WarningFlagForTime;                         ///< the warning flag for time
    int     FirstParaffinIndex;                         ///< the first paraffin index
    quint32 PreTestTime;                                ///< the time of pretest (second unit)
    quint32 ParaffinStepsCostTime;                      ///< the paraffin steps cost time (second unit)
    quint32 Scenario260CostTime;                        ///< the scenario cost time (second unit)
    qint64  GapTime;                                    ///< the gap of step time (millisecond unit)
    qint64  StartTime;                                  ///< the start time of move tube (millisecond unit)
    qint64  EndTime;                                    ///< the end time of move seal (millisecond unit)
    qint64  UserSetEndTime;                             ///< user input the end time (millisecond unit)
    qint64  BufferTime;                                 ///< the buffer time (millisecond unit)
    qint64  TotalParaffinProcessingTime;               ///< the total paraffin processing time (second unit)
    qint64  LastParaffinProcessingTime;                ///< the last paraffin processing time (second unit)
}ProgramEndTime_t;

/****************************************************************************/
/*!
 *  \brief struct for Received command
 */
/****************************************************************************/
typedef struct
{
    DeviceControl::ReturnCode_t retCode;    //!< return Code
    QString                     cmdName;    //!< the command name
} RecvCommand_t;

/****************************************************************************/
/*!
 *  \brief struct for Slave module attributes
 */
/****************************************************************************/
typedef struct
{
    qreal CurrentMax5VDC;               //!< the current max 5VDC
    qreal Voltagerated24VDC;            //!< the voltage 24 VDC
    qreal VoltageTolerance24VDC;        //!< the voltage 24 VDC
} SlaveAttr_t;

/****************************************************************************/
/*!
 *  \brief enum for Cleaning Dry step
 */
/****************************************************************************/
typedef enum
{
    CDS_READY,
    CDS_MOVE_TO_SEALING_13,
    CDS_WAIT_HIT_POSITION,
    CDS_WAIT_HIT_TEMPERATURE,
    CDS_VACUUM,
    CDS_WAIT_HIT_PPRESSURE,
    CDS_WAITING_DRY,
    CDS_STOP_HEATING_VACUUM,
    CDS_SUCCESS,
    CDS_ERROR
}DryStepsStateMachine;

/****************************************************************************/
/*!
 *  \brief struct for Cleaning Dry
 */
/****************************************************************************/
typedef struct
{
    DryStepsStateMachine CurrentState;
    quint64 StepStartTime;
    bool warningReport;
} CleaningDry_t;

    /****************************************************************************/
    /**
     * @brief Controller class for the main scheduler thread.
     *
     */
    /****************************************************************************/
    class SchedulerMainThreadController : public Threads::ThreadController {
        Q_OBJECT

#ifdef GOOGLE_MOCK
		// add "friend class TestSchedulerController" for UT
        friend class TestSchedulerMainThreadController;
#endif

    private:
        QTimer m_TickTimer;       ///<  Definition/Declaration of variable m_TickTimer
        QMutex m_Mutex;       ///<  Definition/Declaration of variable m_Mutex
        QQueue<Global::CommandShPtr_t> m_SchedulerCmdQueue;       ///< Definition/Declaration of variable m_SchedulerCmdQueue
        QMutex m_MutexDeviceControlCmdQueue;                      ///< mutex for accessing m_DeviceControlCmdQueue
        QWaitCondition m_WaitCondition;                           ///< condition variable for DeviceControlCmdQueue
        QQueue<Scheduler::SchedulerCommandShPtr_t> m_DeviceControlCmdQueue;                     ///< Queue(Q2) for receive respond from SechedulerCommandProcessor
        QQueue<ProgramStationInfo_t> m_ProgramStationList;       ///<  Definition/Declaration of variable m_ProgramStationList

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
        QList<ProgramStationInfo_t> m_StationAndReagentList;    ///<    Definition/Declaration of variable m_StationList
        int m_ProcessCassetteCount;       ///<  Definition/Declaration of variable m_ProcessCassetteCount
        quint32 m_EventKey;                                   ///< Current Event key
        ReturnCode_t m_CurErrEventID;                         ///< Current Event ID
        quint32 m_CurrentScenario;                            ///< Current Scenario
        QSharedPointer<HeatingStrategy> mp_HeatingStrategy;   ///< Definition/Declaration of variable mp_HeatingStrategy
        Global::tRefType    m_RefCleanup;                     ///< Command reference of the cleanup command
        int m_delayTime;
        bool m_IsInSoakDelay;                                 ///< Delay in Soak
        bool m_IsPrecheckMoveRV;                            ///< precheck done move rv
        qint64 m_lastPVTime;                                  ///< Time for last PV operation
        qint8 m_ProcessingPV;                                 ///< flag to indicate P or V operation
        bool m_completionNotifierSent;                        ///< Flag to indication if program completion is sent to Gui.
        bool m_IsCleaningProgram;                             ///< cleaning program run or not
        bool m_CleanAckSentGui;                                ///< flag to indicate if cleaning ack to gui or not
        CProgramStatusInfor m_ProgramStatusInfor;              ///< Program Status Infor
        BottlePosition_t    m_CurrentBottlePosition;          ///< the current BottlePosition for bottle check
        SchedulerStateMachine_t m_CurrentStepState;           ///< The current protocol(program) step, which is used to recovery from RC_Restart
        bool m_hasParaffin;                                   ///< the program has paraffin
        bool m_IsReleasePressureOfSoakFinish;                 ///< wether release pressure when soak finish
        bool m_ReleasePressureSucessOfSoakFinish;             ///< release pressure sucess
        bool m_Is5MinPause;                                   ///< Instrument alarm when pausing exceed 5 minutes
        bool m_Is10MinPause;                                  ///< Local alarm when pausing exceed 10 minutes
        bool m_Is15MinPause;                                  ///< Remote alarm when pausing exceed 15 minutes
        QVector<SlaveAttr_t>  m_SlaveAttrList;                ///< Attribute list of Slave modules
        qint8   m_ReEnterFilling;                             ///< When restart filling, the sequence of re-entering filling
        qint64  m_TimeReEnterFilling;                         ///< Time when re-enter filling
        bool    m_CheckRemoteAlarmStatus;                     ///< flag to check m_CheckRemoteAlarmStatus
        bool    m_CheckLocalAlarmStatus;                      ///< flag to check m_CheckLocalAlarmStatus
        bool    m_DisableAlarm;                               ///< disable alarm or not
        qint8   m_PssmStepFinSeq;                             ///< sequence of PSSM_STEP_FIN stage
        QSharedPointer<EventScenarioErrXMLInfo> m_pESEXMLInfo;///< Event-Scenario-Error parser
        bool    m_IsNeedBottleCheck;                          ///< whether need bottle check
        ProgramEndTime_t m_EndTimeAndStepTime;                ///< the end tiem and step time buffer
        QVector<QString> m_UnknownErrorLogVector;             ///< the unknow error log vector
        bool    m_InternalErrorRecv;                          ///< Internal error was received
        bool    m_IsDrainDelay;                               ///< wether drain delay
        qint64  m_DrainDelayBeginTime;                        ///< drain delay begin time
        CleaningDry_t   m_CleaningDry;                        ///< Structure for cleaning dry
        bool    m_CheckOvenCover;                             ///< check the oven cover
        bool    m_TransitionPeriod;                           ///< flag to indicate transition period
        PowerFailureStep_t m_PowerFailureStep; ///< the power failure step


    private:
        SchedulerMainThreadController(const SchedulerMainThreadController&);                      ///< Not implemented.
        SchedulerMainThreadController& operator=(const SchedulerMainThreadController&);     ///< Not implemented.

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
         *  \brief pop Command from Q2
         */
        /****************************************************************************/
         bool GetNextProgramStepInformation(const QString& ProgramID, ProgramStepInfor& ProgramStepInfor, bool onlyGetFirstProgramStepIndex = false);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function GetLeftProgramStepsNeededTime
          *
          *  \param ProgramID = const QString type parameter
          *
          *  \return from GetLeftProgramStepsNeededTime
          */
         /****************************************************************************/
         quint32 GetLeftProgramStepsNeededTime(const QString& ProgramID);
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
          *  \param isLastStep
          *
          *  \return from SelectStationFromReagentID
          */
         /****************************************************************************/
         QString SelectStationFromReagentID(const QString& ReagentID,
                                           ListOfIDs_t& unusedStationIDs,
                                           QList<StationUseRecord_t>& usedStations,
                                           bool isLastStep);

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
          *  \brief  Definition/Declaration of function WhichStepHasNoSafeReagent
          *
          *  \return from IsLastStep
          */
         /****************************************************************************/
         int WhichStepHasNoSafeReagent(const QString& ProgramID);

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
          *  \brief Check no-signal error for temperature sensors
          *
          *  \param Scenario - current scenario
          *  \param HwTemp - current temperature of sensor
          *
          *  \return ture - no error, false - error was gotten
          */
         /****************************************************************************/
         bool CheckLevelSensorNoSignal(quint32 Scenario, qreal HWTemp);

         /****************************************************************************/
         /*!
          *  \brief Check if current over range error occurs in slave sensors
          *
          *  \param Scenario - current scenario
          *
          *  \return void
          */
         /****************************************************************************/
         void CheckSlaveSensorCurrentOverRange(quint32 Scenario);
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

         /****************************************************************************/
         /*!
          *  \brief HandleRmtLocAlarm
          *
          *  \param cmd alarm command
          *
          *  \return void
          */
         /****************************************************************************/
         void HandleRmtLocAlarm(quint32 cmd);

         /****************************************************************************/
         /*!
          *  \brief Construct slave module attribute list
          *
          *  \param QString moduleName - name of the specific slave module
          *
          *  \return bool, true - success, false - failure
          */
         /****************************************************************************/
         bool ConstructSlaveModuleAttrList(QString moduleName);

         /****************************************************************************/
         /*!
          *  \brief Get the stringID for ErrHandling and Service
          *
          *  \param quint32 - ErrorID
          *  \param Global::tTranslatableStringList - EventStringParList
          *  \param Global::tTranslatableStringList - EventRDStringParList
          */
         /****************************************************************************/
         void GetStringIDList(quint32 ErrorID,
                              Global::tTranslatableStringList &EventStringParList,
                              Global::tTranslatableStringList &EventRDStringParList);

         /****************************************************************************/
         /*!
          *  \brief Check active carbon filter is expired
          *
          */
         /****************************************************************************/
         void CheckCarbonFilterExpired();
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
          *  \brief  signal to resume paused/error program
          */
         /****************************************************************************/
         void NotifyResume();
private slots:
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot HandleInitState
          */
         /****************************************************************************/
         void HandleInitState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot HandleInitState
          *  \param ctrlCmd command from GUI
          *  \param cmd command from DC
          */
         /****************************************************************************/
         void HandlePowerFailure(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);

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
         void OnBackToBusy();
         /****************************************************************************/
         /*!
          *  \brief  Slot for begin to Heating RV befor filling
          */
         /****************************************************************************/
         void OnFillingHeatingRV();
         /****************************************************************************/
         /*!
          *  \brief  Slot for pretest done
          */
         /****************************************************************************/
         void OnPreTestDone();
         /****************************************************************************/
         /*!
          *  \brief  Slot to exit init State
          *
          *  \param  void
          *
          *  \return void
          */
         void OnExitedInitState();
         /****************************************************************************/
         /*!
          *  \brief  Slot for reporting error from DeviceControl
          *
          *  \iparam instanceID = Instance identifier of this function module instance
          *  \iparam usErrorGroup = Error group
          *  \iparam usErrorID = Error ID
          *  \iparam usErrorData = Additional error information
          *  \iparam timeStamp = Error time
          */
         /****************************************************************************/
         void OnReportError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData, QDateTime timeStamp);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of function UpdateStationReagentStatus
          *
          *  \return from UpdateStationReagentStatus
          */
         /****************************************************************************/
         void UpdateStationReagentStatus();
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
          * \brief Handler for acknowledge processing
          *
          * \iparam   Ref                 Acknowledge Command reference.
          * \iparam   Ack                 Command.
          */
         /****************************************************************************/
         void OnAcknowledge(Global::tRefType Ref, const Global::AckOKNOK &Ack);

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
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnParaffinMeltPointChanged
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdParaffinMeltPointChanged type parameter
         *
         *  \return from OnParaffinMeltPointChanged
         */
        /****************************************************************************/
        void OnParaffinMeltPointChanged(Global::tRefType Ref, const MsgClasses::CmdParaffinMeltPointChanged & Cmd);
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
         *  \return    bool for RaiseError
         */
         /****************************************************************************/
        /*lint -e641 */
        bool RaiseError(const quint32 EventKey, ReturnCode_t EventID, const quint32 Scenario,
                                  const bool ActionResult, const bool Active = true);

        /****************************************************************************/
        /**
         * @brief raise event to event handler
         *
         *  \iparam    Event  event id
         *  \iparam    Parameters paramter list
         */
         /****************************************************************************/
        inline void RaiseEvent(const quint32 Event, const QStringList& Parameters = QStringList())
        {

            Global::tTranslatableStringList ParList;
            for(QStringList::ConstIterator ite = Parameters.constBegin(); ite != Parameters.constEnd(); ite++)
            {
                ParList << *ite;
            }
            Global::EventObject::Instance().RaiseEvent(Event,ParList);
        }

        /****************************************************************************/
        /**
         * @brief   Set transition period
         *
         *  \return void
         */
         /****************************************************************************/
        void SetTransitionPeriod(bool flag) { m_TransitionPeriod = flag; }
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
         *  \return from GetOvenHeatingTime of quint64
         */
        /****************************************************************************/
        quint64 GetOvenHeatingTime();

        /****************************************************************************/
        /**
         *  \brief Get the time(in seconds) that Oven has been heated
         *  \return from GetOvenHeatingRemainingTime of quint64
         */
        /****************************************************************************/
        quint64 GetOvenHeatingRemainingTime();

        /****************************************************************************/
        /**
         *  \brief Init program status
         *  \return from GetOvenHeatingRemainingTime of quint64
         */
        /****************************************************************************/
        void InitProgramStatus();
        /****************************************************************************/
        /**
         *  \brief Get the time(in seconds) for melting paraffin
         *  \return from quint32
         */
        /****************************************************************************/
        quint32 GetSecondsForMeltingParaffin();

        /****************************************************************************/
        /**
         *  \brief reset the time parameter
         */
        /****************************************************************************/
        void ResetTheTimeParameter();

        /****************************************************************************/
        /**
         *  \brief Calculate the gap time(in seconds)
         *  \param IsStartTime - bool flag
         *  \param IsEndTime - bool flag
         */
        /****************************************************************************/
        void CalculateTheGapTimeAndBufferTime(bool IsStartTime, bool IsEndTime);

        /****************************************************************************/
        /**
         *  \brief Get the time(in seconds) that PreTest
         *  \return from GetPreTestTime of qint64
         */
        /****************************************************************************/
        quint32 GetPreTestTime();

        /****************************************************************************/
        /**
         *  \brief Get the RV moving steps
         *  \param firstPos - qint32
         *  \param endPos - qint32
         *  \return from GetMoveSteps of quint32
         */
        /****************************************************************************/
        quint32 GetMoveSteps(qint32 firstPos, qint32 endPos);

        /****************************************************************************/
        /**
         *  \brief Get the pressure offset for powerfailure
         *  \return from qreal
         */
        /****************************************************************************/
        qreal GetLastPressureOffset();

        /****************************************************************************/
        /**
         *  \brief Set the pressure offset for powerfailure
         *  \param pressureOffset - qreal type
         */
        /****************************************************************************/
        void SetLastPressureOffset(qreal pressureOffset);

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetCurProgramID
         *
         *  \return from GetCurProgramID
         */
        /****************************************************************************/
        QString GetCurProgramID() {return m_CurProgramID; }
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
        SchedulerCommandProcessorBase* GetSchedCommandProcessor() const { return m_SchedulerCommandProcessor;}

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
         *  \param  EventId - ReturnCodde_t
         *  \param  IsErrorMsg - bool type
         */
        /****************************************************************************/
        void SendOutErrMsg(ReturnCode_t EventId, bool IsErrorMsg = true);

        /****************************************************************************/
        /**
         *  \brief  whether need bottle check
         *  \return bool, true - continue; false - done;
         */
        /****************************************************************************/
        bool IsNeedBottleCheck(){return m_IsNeedBottleCheck;}

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
        bool MoveRV(RVPosition_type type);

        /****************************************************************************/
        /*!
         *  \brief  do cleaning dry step
         *  \param  ctrlCmd - ControlCommandType_t
         *  \param  cmd - SchedulerCommandShPtr_t
         */
        /****************************************************************************/
        void DoCleaningDryStep(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);

        /****************************************************************************/
        /*!
         *  \brief  IS Rotary Valve target position
         *  \param  type - type of RV moving (tubue position or sealing position)
         *  \return bool
         */
        /****************************************************************************/
        bool IsRVRightPosition(RVPosition_type type);

        /****************************************************************************/
        /*!
         *  \brief  Get the tpe of failed heater
         *  \return Heater type
         */
        /****************************************************************************/
        HeaterType_t GetFailerHeaterType();

        /****************************************************************************/
        /*!
         *  \brief  Shutting down failed heaters
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool ShutdownFailedHeaters();

        /****************************************************************************/
        /*!
         *  \brief  Restart failed heaters
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
         *  \param  StationID - QString
         *  \param  EnableInsufficientCheck - bool
         *  \return void
         */
        /****************************************************************************/
        void FillRsTissueProtect(const QString& StationID, bool EnableInsufficientCheck);

        /****************************************************************************/
        /*!
         *  \brief  Send Stop Filling for Rs_Tissue_Protect to GUI
         *  \param  StationID - QString
         *  \return void
         */
        /****************************************************************************/
        void StopFillRsTissueProtect(const QString& StationID);

        /****************************************************************************/
        /*!
         *  \brief  Get current station ID
         *  \return QString current station ID
         */
        /****************************************************************************/
        QString GetCurrentStationID() { return m_CurProgramStepInfo.stationID; }
        /****************************************************************************/
        /*!
         *  \brief  Set current station ID
         *  \param  StationID - QString station ID
         *  \param  ReagentGroup - QString
         *  \return void
         */
        /****************************************************************************/
        void UpdateCurProgramStepInfo(const QString& StationID, const QString& ReagentGroup)
        {
            m_CurProgramStepInfo.stationID = StationID;
            m_CurProgramStepInfo.reagentGroup = ReagentGroup;
        }
        /****************************************************************************/
        /*!
         *  \brief  Set current step state
         *  \param  stepState - chedulerStateMachine_t current step state
         */
        /****************************************************************************/
        void SetCurrentStepState(SchedulerStateMachine_t stepState) { m_CurrentStepState = stepState; }

        /****************************************************************************/
        /*!
         *  \brief  Get current step state
         *  \return Current step state
         */
        /****************************************************************************/
        SchedulerStateMachine_t GetCurrentStepState() { return m_CurrentStepState; }

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
         *  \brief  Definition/Declaration of function GetSafeReagentForSpecial just for scenario 200
         *  \param  index - int type
         *  \param  reagentGroupID - QString type
         *  \param  stationList - QList<QString>
         *  \return from bool
         */
        /****************************************************************************/
        bool GetSafeReagentForSpecial(int index, QString& reagentGroupID, QList<QString>& stationList);

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function SendPowerFailureMsg
         */
        /****************************************************************************/
        void SendPowerFailureMsg();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function SendSafeReagentFinishedCmd
         */
        /****************************************************************************/
        void SendSafeReagentFinishedCmd();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function CompleteRsAbort
         */
        /****************************************************************************/
        void CompleteRsAbort();


    public slots:

        /****************************************************************************/
        /**
         *  \brief For advancing the time
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
         *  \brief  slot to begin draining
         */
        /****************************************************************************/
        void OnBeginDrain();

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
         *  \brief  Definition/Declaration of slot RC_Drain_AtOnce
         */
        /****************************************************************************/
        void RcDrainAtOnce();

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
         *  \brief  Definition/Declaration of slot OnSelfTestDone
         *  \param  flag - bool
         */
        /****************************************************************************/
        void OnSelfTestDone(bool flag);

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

        /****************************************************************************/
        /*!
         *  \brief  Send out Tisue Protect done message to GUI
         */
        /****************************************************************************/
        void SendTissueProtectMsg();

        /****************************************************************************/
        /*!
         *  \brief  Send Oven cover open message to GUI
         */
        /****************************************************************************/
        void SendCoverLidOpenMsg();


        /****************************************************************************/
        /*!
         *  \brief  Slot for entering PSSM_STEP_FIN stage
         */
        /****************************************************************************/
        void OnEnterPssMStepFin();
        /****************************************************************************/
        /*!
         *  \brief  Slot for entering OnEnterDryStepState stage
         */
        /****************************************************************************/
        void OnEnterDryStepState();
    };

} // EONS ::Scheduler

#endif
