// ================================================================================================
/*! @file Master/Components/Scheduler/Include/SchedulerStateHandler.h
 *
 *  @brief
 *  Definition of class Scheduler::Schedule.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2018-03-06
 *  $Author:    $ Dixiong Li
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
#ifndef SCHEDULERSTATEHANDLER_H
#define SCHEDULERSTATEHANDLER_H

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

namespace DataManager
{
    class CDataManager;
    class CReagent;
}

namespace Scheduler {

class CSchedulerStateMachine;
class SchedulerMainThreadController;
class SchedulerCommandProcessorBase;

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
 *  \brief  Definition/Declaration of class ProgramStepInfor
 */
/****************************************************************************/

typedef struct {
    QString stationID;       ///<  Definition/Declaration of variable stationID
    QString nextStationID;   ///< Definition/Declaration of variable stationID
    qint64 durationInSeconds;       ///<  Definition/Declaration of variable durationInSeconds
    int temperature;       ///<  Definition/Declaration of variable temperature
    bool isPressure;       ///<  Definition/Declaration of variable isPressure
    bool isVacuum;       ///<  Definition/Declaration of variable isVacuum
    QString reagentGroup;       ///<  Definition/Declaration of variable reagentGroup
} ProgramStepInfor;

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
 *  \brief struct for ProgramEndTime
 */
/****************************************************************************/
typedef struct
{
    bool    WarningFlagForStepTime;                     ///< the warning flag for step time
    bool    WarningFlagForBufferTime;                   ///< the warning flag for buffer time
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
 *  \brief enum for Cleaning Dry step
 */
/****************************************************************************/
typedef enum
{
    CDS_READY,
    CDS_MOVE_TO_SEALING_13,
    CDS_WAIT_HIT_POSITION,
    CDS_WAIT_HIT_TEMPERATURE,
    CDS_MOVE_TO_INIT_POS,
    CDS_VACUUM,
    CDS_WAIT_HIT_PPRESSURE,
    CDS_WAITING_DRY,
    CDS_STOP_VACUUM,
    CDS_STOP_HEATING,
    CDS_WAIT_COOLDWON,
    CDS_MOVE_TO_TUBE_13,
    CDS_WAIT_HIT_TUBE_13,
    CDS_SUCCESS,
    CDS_ERROR
}DryStepsStateMachine;

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
 *  \brief struct for Cleaning Dry
 */
/****************************************************************************/
typedef struct
{
    DryStepsStateMachine CurrentState;      //!< the current state
    quint64 StepStartTime;                  //!< the step start time
    bool warningReport;                     //!< the warning report
} CleaningDry_t;

class CSchedulerStateHandler : public QObject
{
    Q_OBJECT
private:

    SchedulerMainThreadController *mp_SchedulerThreadController;       ///<  Definition/Declaration of variable mp_SchedulerThreadController
    CSchedulerStateMachine* m_SchedulerMachine;
    SchedulerCommandProcessorBase*  m_SchedulerCommandProcessor;       ///<  Definition/Declaration of variable m_SchedulerCommandProcessor
    DataManager::CDataManager       *mp_DataManager;       ///<  Definition/Declaration of variable mp_DataManager

    SchedulerStateMachine_t m_CurrentStepState;           ///< The current protocol(program) step, which is used to recovery from RC_Restart
    bool    m_IsWaitHeatingRV;                            ///< wait heating RV
    bool    m_IsProcessing;                               ///< in processing
    bool    m_IsFirstProcessingForDelay;                  ///< is first enter processing for delay time
    bool m_IsCleaningProgram;                             ///< cleaning program run or not
    bool m_CleanAckSentGui;                               ///< flag to indicate if cleaning ack to gui or not
    CProgramStatusInfor m_ProgramStatusInfor;             ///< Program Status Infor
    int m_CurProgramStepIndex;                            ///<  Definition/Declaration of variable m_CurProgramStepIndex
    int m_FirstProgramStepIndex;                          ///<  Definition/Declaration of variable m_CurProgramStepIndex
    QString m_ReagentExpiredFlag;                         ///< Reagent expired flag
    bool    m_bWaitToPauseCmdYes;                         ///< flag to indicate if PauseYes command is selected
    bool    m_bWaitToPause;                               ///< Wait to be Paused
    ProgramEndTime_t m_EndTimeAndStepTime;                ///< the end tiem and step time buffer
    bool m_completionNotifierSent;                        ///< Flag to indication if program completion is sent to Gui.
    SchedulerTimeStamps_t m_TimeStamps;                   ///<  Definition/Declaration of variable m_TimeStamps
    bool m_IsInSoakDelay;                                 ///< Delay in Soak
    ProgramStepInfor m_CurProgramStepInfo;                ///<  Definition/Declaration of variable m_CurProgramStepInfo
    qint8 m_ProcessingPV;                                 ///< flag to indicate P or V operation
    qint64 m_lastPVTime;                                  ///< Time for last PV operation
    qint8 m_PssmStepFinSeq;                               ///< sequence of PSSM_STEP_FIN stage
    int m_ProcessCassetteCount;                           ///<  Definition/Declaration of variable m_ProcessCassetteCount
    int m_ProcessCassetteNewCount;                        ///<  Definition/Declaration of variable m_ProcessCassetteNewCount
    bool m_IsReleasePressureOfSoakFinish;                 ///< wether release pressure when soak finish
    bool m_IsTakeSpecimen;                                ///< wether take of specimen
    bool m_Is5MinPause;                                   ///< Instrument alarm when pausing exceed 5 minutes
    bool m_Is10MinPause;                                  ///< Local alarm when pausing exceed 10 minutes
    bool m_Is15MinPause;                                  ///< Remote alarm when pausing exceed 15 minutes
    bool m_IsResumeFromPause;                             ///< flag to indicate if we are resuming from pause state
    bool m_TransitionPeriod;                              ///< flag to indicate transition period
    bool m_hasParaffin;                                   ///< the program has paraffin
    qreal m_PressureAL;                                   ///<  Definition/Declaration of variable m_PressureAL
    qreal m_TempRTBottom;                                 ///<  Definition/Declaration of variable m_TempRTBottom
    qreal m_TempRTSide;                                   ///<  Definition/Declaration of variable m_TempRTSide
    qreal m_TempOvenBottom;                               ///<  Definition/Declaration of variable m_TempOvenBottom
    qreal m_TempOvenTop;                                  ///<  Definition/Declaration of variable m_TempOvenTop
    QString m_CurReagnetName;                             ///<  Definition/Declaration of variable m_CurReagnetName
    QString m_CurProgramID;                               ///<  Definition/Declaration of variable m_CurProgramID
    QString m_NewProgramID;                               ///<  Definition/Declaration of variable m_NewProgramID
    qint64 m_delayTime;                                   ///< Delay time set by user
    QStringList m_UsedStationIDs;                         ///< in a whole of program processing
    RVPosition_t m_PositionRV;                            ///<  Definition/Declaration of variable m_PositionRV
    QList<QString> m_StationList;                         ///<  Definition/Declaration of variable m_StationList
    QList<ProgramStationInfo_t> m_StationAndReagentList;  ///<  Definition/Declaration of variable m_StationList
    CleaningDry_t   m_CleaningDry;                        ///< Structure for cleaning dry
    quint32 m_CurrentScenario;                            ///< Current Scenario
    quint32 m_CountTheLogSenserData;
    bool m_IsErrorStateForHM;
    quint32 m_CheckTheHardwareStatus;
//    quint32 m_RemoteAlarmPreviousStatus;
//    quint32 m_LocalAlarmPreviousStatus;
    quint32 m_RetortLockStatus;
    quint32 m_OvenLidStatus;

    QString m_RetortName;

public:
    CSchedulerStateHandler(QString RetortName, SchedulerMainThreadController* SchedulerThreadController, DataManager::CDataManager* dataManager);
    virtual ~CSchedulerStateHandler();

    QString GetRetortName() {return m_RetortName;}
    void SetRetortName(const QString& RetortName) {m_RetortName = RetortName;}

    /****************************************************************************/
    /*!
     *  \brief  IS Rotary Valve target position
     *  \param  type - type of RV moving (tubue position or sealing position)
     *  \return bool
     */
    /****************************************************************************/
    bool IsRVRightPosition(RVPosition_type type);

    /****************************************************************************/
    /**
     *  \brief Calculate the gap time(in seconds)
     *  \param IsStartTime - bool flag
     *  \param IsEndTime - bool flag
     */
    /****************************************************************************/
    void CalculateTheGapTimeAndBufferTime(bool IsStartTime, bool IsEndTime);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetRVTubePositionByStationID
     *
     *  \param stationID = const QString type parameter
     *
     *  \return from GetRVTubePositionByStationID
     */
    /****************************************************************************/
    static RVPosition_t GetRVTubePositionByStationID(const QString& stationID);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetRVSealPositionByStationID
     *
     *  \param stationID = const QString type parameter
     *
     *  \return from GetRVSealPositionByStationID
     */
    /****************************************************************************/
    static RVPosition_t GetRVSealPositionByStationID(const QString& stationID);

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
     *  \brief  Check whether it is the first time to use the specified current reagent group or not
     *  \param  reagentGroupID = the specified current reagent group
     *  \return true indicates the first time to use the current reagent group,
     *     otherwise return false
     *
     */
    /****************************************************************************/
    bool IsFirstTimeUseCurReagent(const QString& reagentGroupID);

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
     *  \brief pop Command from Q2
     */
    /****************************************************************************/
    bool GetNextProgramStepInformation(const QString& ProgramID, ProgramStepInfor& ProgramStepInfor, bool onlyGetFirstProgramStepIndex = false);

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
     *  \brief  Definition/Declaration of function GetSafeReagentStationList
     *  \param  curReagentGroupID = the current used reagent group
     *  \param  excludeCurStation =  whether the current station shall be excluded or not?
     *  \param  firstTimeUseReagent = When the error is happening, is it first time to use the current reagent?
     *  \param  the gotten safe reagent list
     *
     *  \return from IsLastStep
     */
    /****************************************************************************/
    bool GetSafeReagentStationList(const QString& curReagentGroupID, bool excludeCurStation, bool firstTimeUseReagent, QList<QString>& stationList);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function WhichStepHasNoSafeReagent
     *
     *  \return from IsLastStep
     */
    /****************************************************************************/
    int WhichStepHasNoSafeReagent(const QString& ProgramID);


    /****************************************************************************/
    /**
     *  \brief reset the time parameter
     */
    /****************************************************************************/
    void ResetTheTimeParameter();

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
     *  \brief  Definition/Declaration of function CompleteRsAbort
     */
    /****************************************************************************/
    void CompleteRsAbort();

    /****************************************************************************/
    /*!
     *  \brief  Check is there clearing reagent used in the current program?
     *  \return true indicates clearing reagent is existing in the current program, otherwise return false
     *
     */
    /****************************************************************************/
    bool CurProgramHasClearingReagent();

    /****************************************************************************/
    /*!
     *  \brief  get the type of reagent before the current program step
     *  \return reagent group
     *
     */
    /****************************************************************************/
    QString ReagentGroupOfLastStep();

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

    void SetNewProgramID(const QString ProgramID) { m_NewProgramID = ProgramID; }

    /****************************************************************************/
    /*!
     *  \brief Get ReagentID for the Current program step
     *
     *  \return ReagentID for the Current program step
     *
     */
    /****************************************************************************/
    QString ReagentIdOfCurProgramStep();

    /****************************************************************************/
    /**
     *  \brief Get the time(in seconds) that PreTest
     *  \return from GetPreTestTime of qint64
     */
    /****************************************************************************/
    quint32 GetPreTestTime();

    void HandleStateCommand(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd);

    /****************************************************************************/
    /**
     *  \brief Get current secenario in Scheduler
     *  \return quint32 - current scenario
     */
    /****************************************************************************/
    quint32 GetCurrentScenario() const {  return m_CurrentScenario; }

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
     *  \brief  Get last reagent group
     *  \return QString last reagent group
     */
    /****************************************************************************/
    QString GetLastReagentGroup() { return m_ProgramStatusInfor.GetLastReagentGroup(); }

    /****************************************************************************/
    /*!
     *  \brief  Send Bottle Check Reply command to GUI
     *  \return bool from IsRetortContaminted
     */
    /****************************************************************************/
    bool IsRetortContaminted() {return m_ProgramStatusInfor.IsRetortContaminted(); }

    void Start() { m_SchedulerMachine->Start(); }

    /****************************************************************************/
    /*!
     *  \brief	Check if program has paraffin reagent
     *  \return bool
     *
     */
    /****************************************************************************/
    inline bool HasParaffinReagent() const { return m_hasParaffin;  }

    void ProgramSelectedReply(Global::tRefType Ref, const QString& ProgramID, int paraffinStepIndex);

    QString CurStateName(SchedulerStateMachine_t state);

private:
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

    int CurStepIndexForSafeReagent();

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
     *  \brief  Definition/Declaration of function GetLeftProgramStepsNeededTime
     *
     *  \param ProgramID = const QString type parameter
     *  \param BeginProgramStepID = int type  default parameter
     *
     *  \return from GetLeftProgramStepsNeededTime
     */
    /****************************************************************************/
    quint32 GetLeftProgramStepsNeededTime(const QString& ProgramID, int BeginProgramStepID = 0);

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
     *  \brief  Definition/Declaration of function UpdateStationReagentStatus
     *  \iparam  bOnlyNew = only add new added cassette numbers
     *  \return from UpdateStationReagentStatus
     */
    /****************************************************************************/
    void UpdateStationReagentStatus(bool bOnlyNew);

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
     *  \brief  Get the type of safe reagent
     *  \param  curReagentGroupID = the current used reagent group
     *  \param  firstTimeUseReagent = When the error is happening, is it first time to use the current reagent?
     *
     *  \return the type of safe reagent
     */
    /****************************************************************************/
    QString GetSafeReagentType(const QString& curReagentGroupID, bool firstTimeUseReagent);

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
     *  \brief  according to the specified reagent group to get the stations in the current prorgam
     *  \param  specifiedReagentGroup = the specified reagent group
     *  \param  stationList = station containers
     *
     *  \return NULL
     */
    /****************************************************************************/
    void GetSpecifiedStations(const QString& specifiedReagentGroup, QList<QString>& stationList);

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
     *  \brief  Definition/Declaration of function GetScenarioBySchedulerState
     *
     *  \param State = SchedulerStateMachine_t type parameter
     *  \param ReagentGroup =  QString type parameter
     *
     *  \return from GetScenarioBySchedulerState
     */
    /****************************************************************************/
    qint32 GetScenarioBySchedulerState(SchedulerStateMachine_t State, QString ReagentGroup);

    void HardwareMonitor(const QString&);

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

public slots:
     /****************************************************************************/
     /*!
      *  \brief  Protocol filling
      */
     /****************************************************************************/
     void Fill();

     /****************************************************************************/
     /*!
      *  \brief  slot to handle Draing
      */
     /****************************************************************************/
     void Drain();

     /****************************************************************************/
     /*!
      *  \brief  slot to handle the logic on stopping filling
      */
     /****************************************************************************/
     void OnStopFill();

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
     *  \brief  slot to handle the logic for entrance of PSSM_PROCESSING(SOAK)
     */
    /****************************************************************************/
    void OnEnterPssmProcessing();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot RC_Drain
     */
    /****************************************************************************/
    void RCDrain();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot RC_ForceDrain
     */
    /****************************************************************************/
    void RCForceDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot RC_Drain_AtOnce
     */
    /****************************************************************************/
    void RcDrainAtOnce();

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
     *  \brief  On enter idle state
     */
    /****************************************************************************/
    void OnEnterIdleState();

    /****************************************************************************/
    /*!
     *  \brief  On exit idle state
     */
    /****************************************************************************/
    void OnExitIdleState();

    /****************************************************************************/
    /*!
     *  \brief  Slot for entering OnEnterDryStepState stage
     */
    /****************************************************************************/
    void OnEnterDryStepState();

    /****************************************************************************/
    /*!
     *  \brief  Slot for begin to Heating RV befor filling
     */
    /****************************************************************************/
    void OnFillingHeatingRV();

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
      *  \brief  Definition/Declaration of slot AllStop
      */
     /****************************************************************************/
     void AllStop();

     /****************************************************************************/
     /*!
      *  \brief  Slot for begin to Heating RV befor filling Level sensor
      */
     /****************************************************************************/
     void OnEnterHeatingLevelSensor();
};
}

#endif // SCHEDULERSTATEHANDLER_H
