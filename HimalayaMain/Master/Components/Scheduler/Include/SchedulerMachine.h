/****************************************************************************/
/*! \file SchedulerMachine.h
 *
 *  \brief SchedulerStateMachine class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.01.2014
 *   $Author:  $ Quan Zhang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/


#ifndef SCHEDULERMACHINE_H
#define SCHEDULERMACHINE_H

#include <QStateMachine>
#include <QFinalState>
#include <QSharedPointer>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Include/SchedulerStateMachineStates.h"

namespace Scheduler{

class CRsStandby;
class CRsStandbyWithTissue;
class CRsHeatingErr30SRetry;
class CRsPressureOverRange3SRetry;
class CRsTSensorErr3MinRetry;
class CRcLevelSensorHeatingOvertime;
class CRcRestart;
class CRcReport;
class CProgramPreTest;
class CProgramSelfTest;
class SchedulerMainThreadController;
class SchedulerCommandProcessorBase;
class CRsFillingAfterFlush;
class CRsTissueProtect;

/****************************************************************************/
/*!
 * \brief Scheduler State Machine Class
 */
/****************************************************************************/
class CSchedulerStateMachine : public QObject
{
    Q_OBJECT
private:
    SchedulerMainThreadController *mp_SchedulerThreadController;                        ///<  Definition/Declaration of variable mp_SchedulerThreadController
    SchedulerStateMachine_t m_CurrentState;                                             ///<  Definition/Declaration of variable m_CurrentState
    QSharedPointer<QStateMachine> mp_SchedulerMachine;                                  ///<  Scheduler state machine
    SchedulerStateMachine_t m_PreviousState;                                            ///<  Definition/Declaration of variable m_PreviousState

	// Layer one states
    QSharedPointer<QState> mp_InitState;                                                ///<  Initial state
    QSharedPointer<QState> mp_IdleState;                                                ///<  Idle state
    QSharedPointer<QState> mp_BusyState;                                                ///<  Busy state
    QSharedPointer<QState> mp_ErrorState;                                               ///<  Error state

    // Layer two states (for Init state)
    QSharedPointer<QState> mp_SelfTestState;                                            ///<  Self Test state

	// Layer two states (for Busy state)
    QSharedPointer<QState> mp_PssmInitState;                                            ///<  Busy State's sub state: Init state
    QSharedPointer<QState> mp_PssmPreTestState;                                         ///<  Busy State's sub state: Pre-test state
    QSharedPointer<QState> mp_PssmFillingHeatingRVState;                                ///<  Busy State's sub state: heating RV Rod ro not
    QSharedPointer<QState> mp_PssmFillingLevelSensorHeatingState;                       ///<  Busy State's sub state: Filling-LevelSensor_Heating
    QSharedPointer<QState> mp_PssmFillingState;                                         ///<  Busy State's sub state: Filling
    QSharedPointer<QState> mp_PssmRVMoveToSealState;                                    ///<  Busy State's sub state: RV move to Seal position
    QSharedPointer<QState> mp_PssmProcessingState;                                      ///<  Busy State's sub state: Processing or Soak state
    QSharedPointer<QState> mp_PssmProcessingSRState;                                    ///<  Busy State's sub state: Processing or Soak state (for Safe Reagent)
    QSharedPointer<QState> mp_PssmRVMoveToTubeState;                                    ///<  Busy State's sub state: RV move to tube position state
    QSharedPointer<QState> mp_PssmDrainingState;                                        ///<  Busy State's sub state: Draining state
    QSharedPointer<QState> mp_PssmRVPosChangeState;                                     ///<  Busy State's sub state: RV move to next tube state
    QSharedPointer<QState> mp_PssmError;                                                ///<  Busy State's sub state: Step error state
    QSharedPointer<QState> mp_PssmPause;                                                ///<  Busy State's sub state: Pause state
    QSharedPointer<QState> mp_PssmPauseDrain;                                           ///<  Busy State's sub state: Pause drain state
    QSharedPointer<QState> mp_PssmAborting;                                             ///<  Busy State's sub state: Aborting state
    QSharedPointer<QState> mp_PssmAborted;                                              ///<  Busy State's sub state: Aborted state
    QSharedPointer<QState> mp_PssmStepFinish;                                           ///<  Busy State's sub state: step finish state
    QSharedPointer<QState> mp_PssmCleaningDryStep;                                      ///<  Busy State's sub state: cleaning dry step
    QSharedPointer<QFinalState> mp_PssmProgramFinish;                                   ///<  Busy State's sub state: Program Finished state

	// Layer two states (for Error State)
    QSharedPointer<QState> mp_ErrorWaitState;                                           ///<  Error State's sub state: error wait state
    QSharedPointer<QState> mp_ErrorRsStandbyState;                                      ///<  Error State's sub state: handle RS_STandby related logic
    QSharedPointer<QState> mp_ErrorRsStandbyWithTissueState;                            ///<  Error State's sub state: handle RS_STandby_WithTissue related logic
    QSharedPointer<QState> mp_ErrorRcLevelSensorHeatingOvertimeState;                   ///<  Error State's sub state: handle RC_Levelsensor_Heating_Overtime related logic
    QSharedPointer<QState> mp_ErrorRcRestartState;                                      ///<  Error State's sub state: handle RC_Restart related logic
    QSharedPointer<QState> mp_ErrorRsHeatingErr30SRetryState;                           ///<  Error State's sub state: handle Rs_HeatingErr30S_Retry related logic
    QSharedPointer<QState> mp_ErrorRsPressureOverRange3SRetryState;                     ///<  Error State's sub state: handle Rs_PressureOverRange_3SRetry related logic
    QSharedPointer<QState> mp_ErrorRsTSensorErr3MinRetryState;                          ///<  Error State's sub state: handle Rs_TSensorErr3Min_Retry related logic
    QSharedPointer<QState> mp_ErrorRsRVGetOriginalPositionAgainState;                   ///<  Error State's sub state: handle Rs_RV_GetOriginalPositionAgain related logic
    QSharedPointer<QState> mp_RcPressure;                                               ///<  Error State's sub state: for RC_Pressure;
    QSharedPointer<QState> mp_RcVacuum;                                                 ///<  Error State's sub state: for RC_Vacuum;
    QSharedPointer<QState> mp_RcFilling;                                                ///<  Error State's sub state: for RC_Filling;
    QSharedPointer<QState> mp_RcDraining;                                               ///<  Error State's sub state: for RC_Draining;
    QSharedPointer<QState> mp_RsDrainAtOnce;                                            ///<  Error State's sub state: for RS_DrainAtOnce
    QSharedPointer<QState> mp_RcBottleCheckI;                                           ///<  Error State's sub state: for RC_BottleCheck_I
    QSharedPointer<QState> mp_ErrorRsFillingAfterFlushState;                            ///<  Error State's sub state: for RS_FillingAfterFlush
    QSharedPointer<QState> mp_ErrorRsCheckBlockageState;                                ///<  Error State's sub state: for RS_Check_Blockage
    QSharedPointer<QState> mp_ErrorRsPauseState;                                        ///<  Error State's sub state: for RS_Pause
    QSharedPointer<QState> mp_ErrorRsRVWaitintTempUpState;                              ///<  Error State's sub state: for RS_RV_WaitingTempUp
    QSharedPointer<QState> mp_ErrorRsTissueProtectState;                                ///<  Error State's sub state: for RS_Tissue_Protect
    QSharedPointer<QState> mp_ErrorRcCheckRTLockState;                                  ///<  Error State's sub state: for RC_Check_RTLock

    //State machines for Run handling
    QSharedPointer<CProgramSelfTest> mp_ProgramSelfTest;                                ///< state machine for self-test
    QSharedPointer<CProgramPreTest> mp_ProgramPreTest;                                  ///< state machine for Pre-test

    // State machines for Error handling
    QSharedPointer<CRsStandbyWithTissue> mp_RsStandby;                                  ///<  State machine for RS_Standby
    QSharedPointer<CRsStandbyWithTissue> mp_RsStandbyWithTissue;						///<  State machine for RS_StandbyWithTissue
    QSharedPointer<CRsHeatingErr30SRetry> mp_RsHeatingErr30SRetry;                      ///<  State machine for RS_HeatingErr_30S_Retry
    QSharedPointer<CRsPressureOverRange3SRetry> mp_RsPressureOverRange3SRetry;          ///<  State machine for RS_PressureOverRange_3SRetry
    QSharedPointer<CRsTSensorErr3MinRetry> mp_RsTSensorErr3MinRetry;                    ///<  State machine for Rs_TSensorErr3MinRetry
    QSharedPointer<CRsFillingAfterFlush> mp_RsFillingAfterFlush;                        ///<  State machine for Rs_FillingAfterFlush
    QSharedPointer<CRsTissueProtect> mp_RsTissueProtect;                                ///<  State machine for Rs_Tissue_Protect

    typedef enum
    {
        RESTART_LEVELSENSOR,
        CHECK_TEMPERATURE,
        STOP_LEVELSENSOR,
        WAIT2MIN
    } LEVELSENSOR_RESTART_STAGE_t;

    LEVELSENSOR_RESTART_STAGE_t m_RestartLevelSensor;                                   ///< State restart Level sensor sub state
    qint64                      m_LevelSensorWaitTime;                                  ///< The restart Level Sensor begin time

    typedef enum
    {
        MOVE_TO_INITIAL_POS,
        CHECK_INITIAL_POS_RESULT
    } GET_RV_ORIGINALPOS_t;

    GET_RV_ORIGINALPOS_t m_RVGetOriginalPosition;                                      ///< Get Original Position sub state
    qint64               m_RVOrgPosCmdTime;                                            ///< Get Original Position begin time

    typedef enum
    {
        HEATING_LEVELSENSOR,
        CHECK_FILLINGTEMP,
        RC_FILLING,
        CHECK_SEALING_POS,
        RELEASE_PRESSURE
    }RC_FILLING_t;
    RC_FILLING_t m_RcFilling;                                                        ///< RC_Filling sub state

    typedef enum
    {
        BUILD_HIGHPRESSURE,
        WAIT_30S,
        CHECK_PRESSURE
    }RS_CHECK_BLOCKAGE_t;
    RS_CHECK_BLOCKAGE_t m_RsCheckBlockage;                                              ///<  The Rs_Check_Blockage sub state
    qint64              m_RsCheckBlockageStartTime;                                     ///<  The Rs_Check_Blockage begin time

    qint64              m_RsPauseCount;                                                 ///<  The number of RS_Pause
    qint64              m_RsPauseStartTime;                                             ///<  The RS_Pause begin time

    typedef enum
    {
        STOP_HEATING,
        START_HEATING,
        CHECK_RVTEMP
    }RS_RV_WAITINGTEMPUP_t;
    RS_RV_WAITINGTEMPUP_t m_RsRVWaitingTempUp;                                          ///< The RS_RV_WaitingTempUp sub state
    qint64                m_RsRVWaitingTempUpTime;                                      ///< The RS_RV_WaitingTempUp begin time
    qint8                 m_RcPressureSeq;                                              ///< Sequence of RC_Pressure
    qint64                m_RcPressureDelayTime;                                        ///< Delay time for RC_Pressure

private:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetDeviceName
     *  \return from QString
     */
    /****************************************************************************/
    QString GetDeviceName();

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CSchedulerStateMachine
     *
     *  \param  SchedulerThreadController
     *
     *  \return from void
     */
    /****************************************************************************/
    CSchedulerStateMachine(SchedulerMainThreadController* SchedulerThreadController);
    ~CSchedulerStateMachine();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function Start
     *
     *  \return from Start
     */
    /****************************************************************************/
    void Start();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function Stop
     *
     *  \return from Stop
     */
    /****************************************************************************/
    void Stop();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendSchedulerInitComplete
     *
     *  \return from SendSchedulerInitComplete
     */
    /****************************************************************************/
    void SendSchedulerInitComplete();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendRunSignal
     *
     *  \return from SendRunSignal
     */
    /****************************************************************************/
    void SendRunSignal();

   /****************************************************************************/
   /*!
    *  \brief  Send signal "RunPreTest" to Scheduler
    *
    *  \return void
    */
   /****************************************************************************/
   void SendRunPreTest();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendRunComplete
     *
     *  \return from SendRunComplete
     */
    /****************************************************************************/
    void SendRunComplete();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeFillingRVRodHeating
     *
     *  \return from SendResumeFillingRVRodHeating
     */
    /****************************************************************************/
    void SendResumeFillingRVRodHeating();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeFillingLevelSensorHeating
     *
     *  \return from SendResumeFillingLevelSensorHeating
     */
    /****************************************************************************/
    void SendResumeFillingLevelSensorHeating();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeFiling
     *
     *  \return from SendResumeFiling
     */
    /****************************************************************************/
    void SendResumeFiling();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeRVMoveToSeal
     *
     *  \return from SendResumeRVMoveToSeal
     */
    /****************************************************************************/
    void SendResumeRVMoveToSeal();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeProcessing
     *
     *  \return from SendResumeProcessing
     */
    /****************************************************************************/
    void SendResumeProcessing();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeProcessingSR
     *
     *  \return from SendResumeProcessingSR
     */
    /****************************************************************************/
    void SendResumeProcessingSR();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeRVMoveTube
     *
     *  \return from SendResumeRVMoveTube
     */
    /****************************************************************************/
    void SendResumeRVMoveTube();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeDraining
     *
     *  \return from SendResumeDraining
     */
    /****************************************************************************/
    void SendResumeDraining();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendResumeRVPosChange
     *
     *  \return from SendResumeRVPosChange
     */
    /****************************************************************************/
    void SendResumeRVPosChange();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SendErrorSignal
     *
     *  \return from SendErrorSignal
     */
    /****************************************************************************/
    void SendErrorSignal();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStInitOK
     *
     *  \return from NotifyStInitOK
     */
    /****************************************************************************/
    void NotifyStInitOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStTempOK
     *
     *  \return from NotifyStTempOK
     */
    /****************************************************************************/
    void NotifyStTempOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStCurrentOK
     *
     *  \return from NotifyStCurrentOK
     */
    /****************************************************************************/
    void NotifyStCurrentOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStVoltageOK
     *
     *  \return from NotifyStVoltageOK
     */
    /****************************************************************************/
    void NotifyStVoltageOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStRVPositionOK
     *
     *  \return from NotifyStRVPositionOK
     */
    /****************************************************************************/
    void NotifyStRVPositionOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStPressureOK
     *
     *  \return from NotifyStPressureOK
     */
    /****************************************************************************/
    void NotifyStPressureOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStSealingOK
     *
     *  \return from NotifyStSealingOK
     */
    /****************************************************************************/
    void NotifyStSealingOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStGetStationcheckResult
     *
     *  \return from NotifyStGetStationcheckResult
     */
    /****************************************************************************/
    void NotifyStGetStationcheckResult();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStStationLeft
     *
     *  \return from NotifyStStationLeft
     */
    /****************************************************************************/
    void NotifyStStationLeft();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStStationOK
     *
     *  \return from NotifyStStationOK
     */
    /****************************************************************************/
    void NotifyStStationOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStDone
     *
     *  \return from NotifyStDone
     */
    /****************************************************************************/
    void NotifyStDone();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyTempsReady
     *
     *  \return from NotifyTempsReady
     */
    /****************************************************************************/
    void NotifyTempsReady();

    /****************************************************************************/
    /*!
     *  \brief  Notify level sensor heating is ready
     *  \param  void
     *  \return void
     */
    /****************************************************************************/
    void NotifyLevelSensorHeatingReady();

    /****************************************************************************/
    /*!
     *  \brief  Notify RV Rod heating is ready
     *  \param  void
     *  \return void
     */
    /****************************************************************************/
    void NotifyRVRodHeatingReady();

    /****************************************************************************/
    /*!
     *  \brief  Notify RV moving to Sealing position is ready
     *  \param  void
     *  \return void
     */
    /****************************************************************************/
    void NotifyRVMoveToSealReady();

    /****************************************************************************/
    /*!
     *  \brief  Notify RV moving to tube position is ready
     *  \param  void
     *  \return void
     */
    /****************************************************************************/
    void NotifyRVMoveToTubeReady();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyHitTubeBefore
     *
     *  \return from NotifyHitTubeBefore
     */
    /****************************************************************************/
    void NotifyHitTubeBefore();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyFillFinished
     *
     *  \return from NotifyFillFinished
     */
    /****************************************************************************/
    void NotifyFillFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyHitSeal
     *
     *  \return from NotifyHitSeal
     */
    /****************************************************************************/
    void NotifyHitSeal();
    /****************************************************************************/
    /*!
     *  \brief  Notify processing finished
     *  \param  void
     *  \return void
     */
    /****************************************************************************/
    void NotifyProcessingFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyHitTubeAfter
     *
     *  \return from NotifyHitTubeAfter
     */
    /****************************************************************************/
    void NotifyHitTubeAfter();

    /****************************************************************************/
    /*!
     *  \brief  Notify Drain finished
     *  \param  void
     *  \return void
     */
    /****************************************************************************/
    void NotifyDrainFinished();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStepFinished
     *
     *  \return from NotifyStepFinished
     */
    /****************************************************************************/
    void NotifyStepFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyProgramFinished
     *
     *  \return from NotifyProgramFinished
     */
    /****************************************************************************/
    void NotifyProgramFinished();

    /****************************************************************************/
    /*!
     *  \brief  notify to enter cleaning dry step
     *
     *  \return void
     */
    /****************************************************************************/
    void NotifyEnterCleaningDryStep();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyStepProgramFinished
     *
     *  \return from NotifyStepProgramFinished
     */
    /****************************************************************************/
    void NotifyStepProgramFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyError
     *
     *  \return from NotifyError
     */
    /****************************************************************************/
    void NotifyError();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyPause
     *
     *  \param PreviousState = SchedulerStateMachine_t type parameter
     *
     *  \return from NotifyPause
     */
    /****************************************************************************/
    void NotifyPause(SchedulerStateMachine_t PreviousState);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyResumeDrain
     *
     *  \return from NotifyResumeDrain
     */
    /****************************************************************************/
    void NotifyResumeDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyAbort
     *
     *  \return from NotifyAbort
     */
    /****************************************************************************/
    void NotifyAbort();

    /****************************************************************************/
    /*!
     *  \brief Enter RS_RV_GetOriginalPostionAgain
     *
     *  \return void
     */
    /****************************************************************************/
    void EnterRsRVGetOriginalPositionAgain();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyRsRvMoveToInitPositionFinished
     *
     *  \return from NotifyRsRvMoveToInitPositionFinished
     */
    /****************************************************************************/
    void NotifyRsRvMoveToInitPositionFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyRcReport
     *
     *  \return from NotifyRcReport
     */
    /****************************************************************************/
    void NotifyRcReport();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyRsReleasePressure
     *
     *  \return from NotifyRsReleasePressure
     */
    /****************************************************************************/
    void NotifyRsReleasePressure();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyRsShutdownFailedHeater
     *
     *  \return from NotifyRsShutdownFailedHeater
     */
    /****************************************************************************/
    void NotifyRsShutdownFailedHeater();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyRsShutdownFailedHeaterFinished
     *
     *  \return from NotifyRsShutdownFailedHeaterFinished
     */
    /****************************************************************************/
    void NotifyRsShutdownFailedHeaterFinished();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function UpdateCurrentState
     *
     *  \param currentState = SchedulerStateMachine_t type parameter
     *
     *  \return from UpdateCurrentState
     */
    /****************************************************************************/
    void UpdateCurrentState(SchedulerStateMachine_t currentState);

    /****************************************************************************/
    /*!
     *  \brief Enter to Rs_Standby sub state machine
     *
     *  \return void
     */
    /****************************************************************************/
    void EnterRsStandBy();

    /****************************************************************************/
    /*!
     *  \brief Enter to Rs_HeatingErr30SRetry sub state machine
     *
     *  \return void
     */
    /****************************************************************************/
    void EnterRsHeatingErr30SRetry();

    /****************************************************************************/
    /*!
     *  \brief Enter to Rs_PressureOverRange_3SRetry sub state machine
     *
     *  \return void
     */
    /****************************************************************************/
    void EnterRsPressureOverRange3SRetry();

    /****************************************************************************/
    /*!
     *  \brief Enter to Rs_TSensorErr_3min_Retry sub state machine
     *
     *  \return void
     */
    /****************************************************************************/
    void EnterRsTSensorErr3MinRetry();

    /****************************************************************************/
    /*!
     *  \brief Enter to Rs_Standby_WithTissue sub state machine
     *
     *  \return void
     */
    /****************************************************************************/
    void EnterRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief Enter to Rc_Levelsensor_Heating_Overtime sub state machine
     *
     *  \return void
     */
    /****************************************************************************/
    void EnterRcLevelsensorHeatingOvertime();

    /****************************************************************************/
    /*!
     *  \brief Enter to Rc_Restart sub state machine
     *
     *  \return void
     */
    /****************************************************************************/
    void EnterRcRestart();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyRcPressure
     *
     *  \return from NotifyRcPressure
     */
    /****************************************************************************/
    void EnterRcPressure();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyVacuum
     *
     *  \return from NotifyRcVacuum
     */
    /****************************************************************************/
    void EnterRcVacuum();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyRcFilling
     *
     *  \return from NotifyRcFilling
     */
    /****************************************************************************/
    void EnterRcFilling();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifyRcDraining
     *
     *  \return from NotifyRcDraining
     */
    /****************************************************************************/
    void EnterRcDraining();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnterRsDrainAtOnce
     *
     *  \return from EnterRsDrainAtOnce
     */
    /****************************************************************************/
    void EnterRsDrainAtOnce();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnterRcBottleCheckI
     *
     *  \return from EnterRcBottleCheckI
     */
    /****************************************************************************/
    void EnterRcBottleCheckI();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnterRsFillingAfterFlush
     *
     *  \return from EnterRsFillingAfterFlush
     */
    /****************************************************************************/
    void EnterRsFillingAfterFlush();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnterRsCheckBlockage
     *
     *  \return from EnterRsCheckBlockage
     */
    /****************************************************************************/
    void EnterRsCheckBlockage();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnterRsPause
     *
     *  \return from EnterRsPause
     */
    /****************************************************************************/
    void EnterRsPause();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnterRsRVWaitingTempUp
     *
     *  \return from EnterRsRVWaitingTempUp
     */
    /****************************************************************************/
    void EnterRsRVWaitingTempUp();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnterRsTissueProtect
     *
     *  \return from EnterRsTissueProtect
     */
    /****************************************************************************/
    void EnterRsTissueProtect();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnterRcCheckRTLock
     *
     *  \return from EnterRcCheckRTLock
     */
    /****************************************************************************/
    void EnterRcCheckRTLock();

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Program Pre-Test
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandlePssmPreTestWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode); 

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Program Self-Test
     *  \param cmdName - command name
     *  \param retCode - return code
     *  \return void
     */
    /****************************************************************************/
    void HandleSelfTestWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Rs_Standby
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsStandByWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Rs_HeatingERR30SRetry
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsHeatingErr30SRetry(const QString& cmdName, DeviceControl::ReturnCode_t retCode);


    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Rs_PressureOverRange_3SRetry
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsPressureOverRange3SRetry(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for RS_TSensorErr3MinRetry
     *  \param cmdName - command name
     *  \param retCode - return code
     *  \return void
     */
    /****************************************************************************/
    void HandleRsTSensorErr3MinRetry(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Rs_Standby_WithTissue
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsStandByWithTissueWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Rs_Tissue_Protect
     *
     *  \param ctrlCmd - control command
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsTissueProtectWorkFlow(ControlCommandType_t ctrlCmd, const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Rc_Check_RTLock
     *
     *  \param void
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRcCheckRTLockWorkFlow();

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Rc_Levelsensor_Heating_Overtime
     *
     *  \param void
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRcLevelSensorHeatingOvertimeWorkFlow();

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for Rs_RV_GetOriginalPositionAgain
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsRVGetOriginalPositionAgainWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRcPressureWorkFlow
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRcPressureWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRcVacuumWorkFlow
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRcVacuumWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRcFillingWorkFlow
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRcFillingWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRcDrainingWorkFlow
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleDrainingWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRcBottleCheckIWorkFlow
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRcBottleCheckIWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRsFillingAfterFlushWorkFlow
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsFillingAfterFlushWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRsCheckBlockageWorkFlow
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsCheckBlockageWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRsPauseWorkFlow
     *
     *  \return void
     */
    /****************************************************************************/
    void HandleRsPauseWorkFlow();

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for HandleRsRVWaitingTempUpWorkFlow
     *  \param cmdName - command name
     *  \param retCode - return cod
     *  \return void
     */
    /****************************************************************************/
    void HandleRsRVWaitingTempUpWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetCurrentState
     *
     *  \return from GetCurrentState
     */
    /****************************************************************************/
    SchedulerStateMachine_t GetCurrentState();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetPreviousState
     *
     *  \return from GetPreviousState
     */
    /****************************************************************************/
    SchedulerStateMachine_t GetPreviousState();

private slots:

    /****************************************************************************/
    /*!
     *  \brief	Slot to handle the actions when all the tasks are done.
     *  \param	bool flag to indicate if the whole execution succeeds or not
     *  \return	void
     */
    /****************************************************************************/
    void OnTasksDone(bool flag);

    /****************************************************************************/
    /*!
     *  \brief	Slot to handle the actions when RS_Tissue_Protect is done.
     *  \param	bool flag to indicate if the whole execution succeeds or not
     *  \return	void
     */
    /****************************************************************************/
    void OnTasksDoneRSTissueProtect(bool flag);

    /****************************************************************************/
    /*!
     *  \brief	Slot to enter RV_Move_To_Seal state.
     *  \param	void
     *  \return	void
     */
    /****************************************************************************/
    void OnRVMoveToSeal();

    /****************************************************************************/
    /*!
     *  \brief	Slot to enter RV_Move_To_Tube state.
     *  \param	void
     *  \return	void
     */
    /****************************************************************************/
    void OnRVMoveToTube();

    /****************************************************************************/
    /*!
     *  \brief	Slot to enter RV_Move_To_next_Tube state.
     *  \param	void
     *  \return	void
     */
    /****************************************************************************/
    void OnRVMoveToNextTube();

    /****************************************************************************/
    /*!
     *  \brief  Slot to react for program resuming
     *
     *  \param  void
     *
     *  \return void
     */
    /****************************************************************************/
    void OnNotifyResume();
signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SchedulerInitComplete
     */
    /****************************************************************************/
    void SchedulerInitComplete();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal RunSignal
     */
    /****************************************************************************/
    void RunSignal();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal RunSelfTest
    */
   /****************************************************************************/
   void RunPreTest();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeFillingRVRodHeating
    */
   /****************************************************************************/
   void ResumeFillingRVRodHeating();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeFillingLevelSensorHeating
    */
   /****************************************************************************/
   void ResumeFillingLevelSensorHeating();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeFiling
    */
   /****************************************************************************/
   void ResumeFiling();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeRVMoveToSeal
    */
   /****************************************************************************/
   void ResumeRVMoveToSeal();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeProcessing
    */
   /****************************************************************************/
   void ResumeProcessing();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeProcessingSR
    */
   /****************************************************************************/
   void ResumeProcessingSR();
   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeRVMoveTube
    */
   /****************************************************************************/
   void ResumeRVMoveTube();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeDraining
    */
   /****************************************************************************/
   void ResumeDraining();

   /****************************************************************************/
   /*!
    *  \brief  Definition/Declaration of signal ResumeRVPosChange
    */
   /****************************************************************************/
   void ResumeRVPosChange();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal RunComplete
     */
    /****************************************************************************/
    void RunComplete();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal RecoverToPause
     */
    /****************************************************************************/
    void RecoverToPause();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ErrorSignal
     */
    /****************************************************************************/
    void ErrorSignal();

    /****************************************************************************/
    /*!
     *  \brief signal to enter RS_StandBy state
     */
    /****************************************************************************/
    void SigEnterRsStandBy();

    /****************************************************************************/
    /*!
     *  \brief signal to enter RC_Levelsensor_Heating_Overtime state
     */
    /****************************************************************************/
    void SigEnterRcLevelsensorHeatingOvertime();

    /****************************************************************************/
    /*!
     *  \brief signal to enter RS_StandBy_WithTissue state
     */
    /****************************************************************************/
    void SigEnterRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief signal to enter RS_RV_GetOriginalPositionAgain state
     */
    /****************************************************************************/
    void SigEnterRsRVGetOriginalPositionAgain();

    /****************************************************************************/
    /*!
     *  \brief signal to enter RC_Restart
     */
    /****************************************************************************/
    void SigEnterRcRestart();

    /****************************************************************************/
    /*!
     *  \brief signal to enter RS_HeatingErr30SRetry
     */
    /****************************************************************************/
    void SigEnterRsHeatingErr30SRetry();

    /****************************************************************************/
    /*!
     *  \brief signal to enter RS_PressureOverRange_3SRetry
     */
    /****************************************************************************/
    void SigEnterRsPressureOverRange3SRetry();

    /****************************************************************************/
    /*!
     *  \brief signal to enter Rs_TSensorErr3MinRetry
     */
    /****************************************************************************/
    void SigEnterRSTSensorErr3MinRetry();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStInitOK
     */
    /****************************************************************************/
    void sigStInitOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStTempOK
     */
    /****************************************************************************/
    void sigStTempOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStCurrentOK
     */
    /****************************************************************************/
    void sigStCurrentOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStVoltageOK
     */
    /****************************************************************************/
    void sigStVoltageOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStRVPositionOK
     */
    /****************************************************************************/
    void sigStRVPositionOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStPressureOK
     */
    /****************************************************************************/
    void sigStPressureOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStSealingOK
     */
    /****************************************************************************/
    void sigStSealingOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStGetStationcheckResult
     */
    /****************************************************************************/
    void sigStGetStationcheckResult();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStStationLeft
     */
    /****************************************************************************/
    void sigStStationLeft();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStStationOK
     */
    /****************************************************************************/
    void sigStStationOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStDone
     */
    /****************************************************************************/
    void sigStDone();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigTempsReady
     */
    /****************************************************************************/
    void sigTempsReady();

    /****************************************************************************/
    /*!
     *  \brief  Signal for level sensor heating ready
     */
    /****************************************************************************/
    void sigLevelSensorHeatingReady();

    /****************************************************************************/
    /*!
     *  \brief  Signal for RV Rod heating ready
     */
    /****************************************************************************/
    void sigRVRodHeatingReady();

    /****************************************************************************/
    /*!
     *  \brief  Signal for RV moving to Sealing position ready
     */
    /****************************************************************************/
    void sigRVMoveToSealReady();

    /****************************************************************************/
    /*!
     *  \brief  Signal for RV moving to tube position ready
     */
    /****************************************************************************/
    void sigRVMoveToTubeReady();

    /****************************************************************************/
    /*!
     *  \brief  Signal for filling completes
     */
    /****************************************************************************/
    void sigFillingComplete();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigHitTubeBefore
     */
    /****************************************************************************/
    void sigHitTubeBefore();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigFillFinished
     */
    /****************************************************************************/
    void sigFillFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigHitSeal
     */
    /****************************************************************************/
    void sigHitSeal();
    /****************************************************************************/
    /*!
     *  \brief  Signal for Processing(soak) finished
     */
    /****************************************************************************/
    void sigProcessingFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigHitTubeAfter
     */
    /****************************************************************************/
    void sigHitTubeAfter();
    /****************************************************************************/
    /*!
     *  \brief  Signal for Draining finished
     */
    /****************************************************************************/
    void sigDrainFinished();

    /****************************************************************************/
    /*!
     *  \brief  Signal for RV position change ready
     */
    /****************************************************************************/
    void sigRVPosChangeReady();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStepFinished
     */
    /****************************************************************************/
    void sigStepFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigProgramFinished
     */
    /****************************************************************************/
    void sigProgramFinished();
    /****************************************************************************/
    /*!
     *  \brief  signal for enter cleaning dry step
     */
    /****************************************************************************/
    void sigEnterDryStep();


    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigProgramFinished
     */
    /****************************************************************************/
    void sigStepProgramFinished();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigError
     */
    /****************************************************************************/
    void sigError();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigPause
     */
    /****************************************************************************/
    void sigPause();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToSelftest
     */
    /****************************************************************************/
    void sigResumeToPreTest();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToInit
     */
    /****************************************************************************/
    void sigResumeToInit();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeFromErrorToBegin
     */
    /****************************************************************************/
    void sigResumeFromErrorToBegin();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToHeatLevelSensorS1
     */
    /****************************************************************************/
    void sigResumeToHeatLevelSensorS1();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToHeatLevelSensorS2
     */
    /****************************************************************************/
    void sigResumeToHeatLevelSensorS2();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToReadyToFill
     */
    /****************************************************************************/
    void sigResumeToReadyToFill();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToSoak
     */
    /****************************************************************************/
    void sigResumeToSoak();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToStepFinished
     */
    /****************************************************************************/
    void sigResumeToStepFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToReadyToTubeAfter
     */
    /****************************************************************************/
    void sigResumeToReadyToTubeAfter();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigAbort
     */
    /****************************************************************************/
    void sigAbort();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigRsRvMoveToInitPosition
     */
    /****************************************************************************/
    void sigRsRvMoveToInitPosition();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigRsRvMoveToInitPositionFinished
     */
    /****************************************************************************/
    void sigRsRvMoveToInitPositionFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigRcReport
     */
    /****************************************************************************/
    void sigRcReport();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigReleasePressure
     */
    /****************************************************************************/
    void sigReleasePressure();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigShutdownFailedHeater
     */
    /****************************************************************************/
    void sigShutdownFailedHeater();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigShutdownFailedHeaterFinished
     */
    /****************************************************************************/
    void sigShutdownFailedHeaterFinished();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigReleasePressureAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigReleasePressureAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigShutdownFailedHeaterAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigShutdownFailedHeaterAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigShutdownFailedHeaterFinishedAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigShutdownFailedHeaterFinishedAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigRTBottomStopTempCtrlAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigRTBottomStopTempCtrlAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigRTTopStopTempCtrlAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigRTTopStopTempCtrlAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnInit
     */
    /****************************************************************************/
    void sigOnInit();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnHeatLevelSensorTempS1
     */
    /****************************************************************************/
    void sigOnHeatLevelSensorTempS1();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnHeatLevelSensorTempS2
     */
    /****************************************************************************/
    void sigOnHeatLevelSensorTempS2();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnMoveToTubeBefore
     */
    /****************************************************************************/
    void sigOnMoveToTubeBefore();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnMoveToTubeAfter
     */
    /****************************************************************************/
    void sigOnMoveToTubeAfter();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnMoveToSeal
     */
    /****************************************************************************/
    void sigOnMoveToSeal();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnFill
     */
    /****************************************************************************/
    void sigOnFill();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnStopFill
     */
    /****************************************************************************/
    void sigOnStopFill();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnSoak
     */
    /****************************************************************************/
    void sigOnSoak();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnDrain
     */
    /****************************************************************************/
    void sigOnDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnStopDrain
     */
    /****************************************************************************/
    void sigOnStopDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnAborting
     */
    /****************************************************************************/
    void sigOnAborting();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnAborted
     */
    /****************************************************************************/
    void sigOnAborted();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnPause
     */
    /****************************************************************************/
    void sigOnPause();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnPauseDrain
     */
    /****************************************************************************/
    void sigOnPauseDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRsRvMoveToInitPosition
     */
    /****************************************************************************/
    void sigOnRsRvMoveToInitPosition();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRcReport
     */
    /****************************************************************************/
    void sigOnRcReport();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRsReleasePressure
     */
    /****************************************************************************/
    void sigOnRsReleasePressure();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRsShutdownFailedHeater
     */
    /****************************************************************************/
    void sigOnRsShutdownFailedHeater();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRVPositionCheck
     */
    /****************************************************************************/
    void sigOnRVPositionCheck();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnSealingCheck
     */
    /****************************************************************************/
    void sigOnSealingCheck();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRsReleasePressureAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigOnRsReleasePressureAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRsShutdownFailedHeaterAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigOnRsShutdownFailedHeaterAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRsRTBottomStopTempCtrlAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigOnRsRTBottomStopTempCtrlAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigOnRsRTTopStopTempCtrlAtRsStandByWithTissue
     */
    /****************************************************************************/
    void sigOnRsRTTopStopTempCtrlAtRsStandByWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRcPressure
     */
    /****************************************************************************/
    void SigRcPressure();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRcVacuum
     */
    /****************************************************************************/
    void SigRcVacuum();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRcFilling
     */
    /****************************************************************************/
    void SigRcFilling();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRcFillingBegin
     */
    /****************************************************************************/
    void SigRcFillingBegin();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRcDraining
     */
    /****************************************************************************/
    void SigRcDraining();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRsDrainAtOnce
     */
    /****************************************************************************/
    void SigRsDrainAtOnce();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRcBottleCheckI
     */
    /****************************************************************************/
    void SigRcBottleCheckI();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRsFillingAfterFlush
     */
    /****************************************************************************/
    void SigRsFillingAfterFlush();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigEnterRsTissueProtect
     */
    /****************************************************************************/
    void SigEnterRsTissueProtect();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigEnterRcCheckRTLock
     */
    /****************************************************************************/
    void SigEnterRcCheckRTLock();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRsCheckBlockage
     */
    /****************************************************************************/
    void SigRsCheckBlockage();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRsPause
     */
    /****************************************************************************/
    void SigRsPause();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigRsPause
     */
    /****************************************************************************/
    void SigRsRVWaitingTempUp();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigStateChange
     */
    /****************************************************************************/
    void sigStateChange();

    /****************************************************************************/
    /*!
     *  \brief  Signal for entering idle state
     */
    /****************************************************************************/
    void sigEnterIdleState();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal sigResumeToProcessing
     */
    /****************************************************************************/
    void sigResumeToProcessing();
};
}

#endif // SCHEDULERMACHINE_H
