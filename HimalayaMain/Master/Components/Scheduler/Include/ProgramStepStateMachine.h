/****************************************************************************/
/*! \file ProgramStepStateMachine.h
 *
 *  \brief CProgramStepStateMachine class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.01.2014
 *   $Author:  $ Quan Zhang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2014 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef PROGRAMSTEPSTATEMACHINE_H
#define PROGRAMSTEPSTATEMACHINE_H


#include <QStateMachine>
#include <QFinalState>
#include "SchedulerStateMachineStates.h"
namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CProgramStepStateMachine
 */
/****************************************************************************/
class CProgramStepStateMachine : public QObject
{
    Q_OBJECT
public:
    CProgramStepStateMachine(QState* pParentState, QState* pErrorState);
    ~CProgramStepStateMachine();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function Start
     *
     *  \return from Start
     */
    /****************************************************************************/
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
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function Stop
     *
     *  \return from Stop
     */
    /****************************************************************************/
    void Stop();

signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StInitOK
     */
    /****************************************************************************/
    void StInitOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StTempOK
     */
    /****************************************************************************/
    void StTempOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StCurrentOK
     */
    /****************************************************************************/
    void StCurrentOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StVoltageOK
     */
    /****************************************************************************/
    void StVoltageOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StRVPositionOK
     */
    /****************************************************************************/
    void StRVPositionOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StPressureOK
     */
    /****************************************************************************/
    void StPressureOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StSealingOK
     */
    /****************************************************************************/
    void StSealingOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StGetStationcheckResult
     */
    /****************************************************************************/
    void StGetStationcheckResult();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StStationLeft
     */
    /****************************************************************************/
    void StStationLeft();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StStationOK
     */
    /****************************************************************************/
    void StStationOK();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StDone
     */
    /****************************************************************************/
    void StDone();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SelfTestSig
     */
    /****************************************************************************/
    void PreTestSig();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal CleaningMoveRVPosSig
     */
    /****************************************************************************/
    void CleaningMoveRVPosSig();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal TempsReady
     */
    /****************************************************************************/
    void TempsReady();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal LevelSensorTempS1Ready
     */
    /****************************************************************************/
    void LevelSensorTempS1Ready();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal LevelSensorTempS2Ready
     */
    /****************************************************************************/
    void LevelSensorTempS2Ready();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal HitTubeBefore
     */
    /****************************************************************************/
    void HitTubeBefore();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal FillFinished
     */
    /****************************************************************************/
    void FillFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal HitSeal
     */
    /****************************************************************************/
    void HitSeal();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SoakFinished
     */
    /****************************************************************************/
    void SoakFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal HitTubeAfter
     */
    /****************************************************************************/
    void HitTubeAfter();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal DrainFinished
     */
    /****************************************************************************/
    void DrainFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StepFinished
     */
    /****************************************************************************/
    void StepFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramFinished
     */
    /****************************************************************************/
    void ProgramFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal Error
     */
    /****************************************************************************/
    void Error();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal Pause
     */
    /****************************************************************************/
    void Pause();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeToSelftest
     */
    /****************************************************************************/
    void ResumeToSelftest();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeFromErrorToBegin
     */
    /****************************************************************************/
    void ResumeFromErrorToBegin();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeToInit
     */
    /****************************************************************************/
    void ResumeToInit();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeToHeatLevelSensorS1
     */
    /****************************************************************************/
    void ResumeToHeatLevelSensorS1();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeToHeatLevelSensorS2
     */
    /****************************************************************************/
    void ResumeToHeatLevelSensorS2();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeToReadyToFill
     */
    /****************************************************************************/
    void ResumeToReadyToFill();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeToSoak
     */
    /****************************************************************************/
    void ResumeToSoak();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeToStepFinished
     */
    /****************************************************************************/
    void ResumeToStepFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ResumeToReadyToTubeAfter
     */
    /****************************************************************************/
    void ResumeToReadyToTubeAfter();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal Abort
     */
    /****************************************************************************/
    void Abort();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnInit
     */
    /****************************************************************************/
    void OnInit();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnHeatLevelSensorTempS1
     */
    /****************************************************************************/
    void OnHeatLevelSensorTempS1();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnHeatLevelSensorTempS2
     */
    /****************************************************************************/
    void OnHeatLevelSensorTempS2();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnMoveToTubeBefore
     */
    /****************************************************************************/
    void OnMoveToTubeBefore();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnMoveToTubeAfter
     */
    /****************************************************************************/
    void OnMoveToTubeAfter();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnMoveToSeal
     */
    /****************************************************************************/
    void OnMoveToSeal();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnFill
     */
    /****************************************************************************/
    void OnFill();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnStopFill
     */
    /****************************************************************************/
    void OnStopFill();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnSoak
     */
    /****************************************************************************/
    void OnSoak();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnDrain
     */
    /****************************************************************************/
    void OnDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnStopDrain
     */
    /****************************************************************************/
    void OnStopDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnAborting
     */
    /****************************************************************************/
    void OnAborting();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnAborted
     */
    /****************************************************************************/
    void OnAborted();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnPause
     */
    /****************************************************************************/
    void OnPause();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnPauseDrain
     */
    /****************************************************************************/
    void OnPauseDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnStateExited
     */
    /****************************************************************************/
    void OnStateExited();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnSealingCheck
     */
    /****************************************************************************/
    void OnSealingCheck();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnRVPositionCheck
     */
    /****************************************************************************/
    void OnRVPositionCheck();

private:
<<<<<<< HEAD
    QState* mp_PssmSelfTest;    ///<  Definition/Declaration of variable mp_PssmSelfTest
    QState* mp_PssmStInit;       ///<  Definition/Declaration of variable mp_PssmStInit
    QState* mp_PssmStTempChecking;       ///<  Definition/Declaration of variable mp_PssmStTempChecking
    QState* mp_PssmStCurrentChecking;       ///<  Definition/Declaration of variable mp_PssmStCurrentChecking
    QState* mp_PssmStVoltageChecking;       ///<  Definition/Declaration of variable mp_PssmStVoltageChecking
    QState* mp_PssmStRVPositionChecking;       ///<  Definition/Declaration of variable mp_PssmStRVPositionChecking
    QState* mp_PssmStPressureChecking;       ///<  Definition/Declaration of variable mp_PssmStPressureChecking
    QState* mp_PssmStSealingChecking;       ///<  Definition/Declaration of variable mp_PssmStSealingChecking
    QState* mp_PssmStStationChecking;       ///<  Definition/Declaration of variable mp_PssmStStationChecking
    QState* mp_PssmStStationCheckFinish;       ///<  Definition/Declaration of variable mp_PssmStStationCheckFinish
    QState* mp_PssmStDone;       ///<  Definition/Declaration of variable mp_PssmStDone
    QState* mp_PssmInit;       ///<  Definition/Declaration of variable mp_PssmInit
    QState* mp_PssmReadyToHeatLevelSensorS1;       ///<  Definition/Declaration of variable mp_PssmReadyToHeatLevelSensorS1
    QState* mp_PssmReadyToHeatLevelSensorS2;       ///<  Definition/Declaration of variable mp_PssmReadyToHeatLevelSensorS2
    QState* mp_PssmReadyToTubeBefore;       ///<  Definition/Declaration of variable mp_PssmReadyToTubeBefore
    QState* mp_PssmReadyToTubeAfter;       ///<  Definition/Declaration of variable mp_PssmReadyToTubeAfter
    QState* mp_PssmReadyToSeal;       ///<  Definition/Declaration of variable mp_PssmReadyToSeal
    QState* mp_PssmReadyToFill;       ///<  Definition/Declaration of variable mp_PssmReadyToFill
    QState* mp_PssmReadyToDrain;       ///<  Definition/Declaration of variable mp_PssmReadyToDrain
    QState* mp_PssmSoak;       ///<  Definition/Declaration of variable mp_PssmSoak
    QState* mp_PssmPause;       ///<  Definition/Declaration of variable mp_PssmPause
    QState* mp_PssmPauseDrain;       ///<  Definition/Declaration of variable mp_PssmPauseDrain
    QState* mp_PssmAborting;       ///<  Definition/Declaration of variable mp_PssmAborting
    QState* mp_PssmAborted;       ///<  Definition/Declaration of variable mp_PssmAborted
    QState* mp_PssmStepFinish;       ///<  Definition/Declaration of variable mp_PssmStepFinish
    QFinalState* mp_PssmProgramFinish;       ///<  Definition/Declaration of variable mp_PssmProgramFinish
    QState* mp_PssmError;       ///<  Definition/Declaration of variable mp_PssmError
    SchedulerStateMachine_t m_PreviousState;       ///<  Definition/Declaration of variable m_PreviousState
||||||| merged common ancestors
    QState* mp_PssmSelfTest;
    QState* mp_PssmStInit;       ///<  Definition/Declaration of variable mp_PssmStInit
    QState* mp_PssmStTempChecking;       ///<  Definition/Declaration of variable mp_PssmStTempChecking
    QState* mp_PssmStCurrentChecking;       ///<  Definition/Declaration of variable mp_PssmStCurrentChecking
    QState* mp_PssmStVoltageChecking;       ///<  Definition/Declaration of variable mp_PssmStVoltageChecking
    QState* mp_PssmStRVPositionChecking;       ///<  Definition/Declaration of variable mp_PssmStRVPositionChecking
    QState* mp_PssmStPressureChecking;       ///<  Definition/Declaration of variable mp_PssmStPressureChecking
    QState* mp_PssmStSealingChecking;       ///<  Definition/Declaration of variable mp_PssmStSealingChecking
    QState* mp_PssmStStationChecking;       ///<  Definition/Declaration of variable mp_PssmStStationChecking
    QState* mp_PssmStStationCheckFinish;       ///<  Definition/Declaration of variable mp_PssmStStationCheckFinish
    QState* mp_PssmStDone;       ///<  Definition/Declaration of variable mp_PssmStDone
    QState* mp_PssmInit;       ///<  Definition/Declaration of variable mp_PssmInit
    QState* mp_PssmReadyToHeatLevelSensorS1;       ///<  Definition/Declaration of variable mp_PssmReadyToHeatLevelSensorS1
    QState* mp_PssmReadyToHeatLevelSensorS2;       ///<  Definition/Declaration of variable mp_PssmReadyToHeatLevelSensorS2
    QState* mp_PssmReadyToTubeBefore;       ///<  Definition/Declaration of variable mp_PssmReadyToTubeBefore
    QState* mp_PssmReadyToTubeAfter;       ///<  Definition/Declaration of variable mp_PssmReadyToTubeAfter
    QState* mp_PssmReadyToSeal;       ///<  Definition/Declaration of variable mp_PssmReadyToSeal
    QState* mp_PssmReadyToFill;       ///<  Definition/Declaration of variable mp_PssmReadyToFill
    QState* mp_PssmReadyToDrain;       ///<  Definition/Declaration of variable mp_PssmReadyToDrain
    QState* mp_PssmSoak;       ///<  Definition/Declaration of variable mp_PssmSoak
    QState* mp_PssmPause;       ///<  Definition/Declaration of variable mp_PssmPause
    QState* mp_PssmPauseDrain;       ///<  Definition/Declaration of variable mp_PssmPauseDrain
    QState* mp_PssmAborting;       ///<  Definition/Declaration of variable mp_PssmAborting
    QState* mp_PssmAborted;       ///<  Definition/Declaration of variable mp_PssmAborted
    QState* mp_PssmStepFinish;       ///<  Definition/Declaration of variable mp_PssmStepFinish
    QFinalState* mp_PssmProgramFinish;       ///<  Definition/Declaration of variable mp_PssmProgramFinish
    QState* mp_PssmError;       ///<  Definition/Declaration of variable mp_PssmError
    SchedulerStateMachine_t m_PreviousState;       ///<  Definition/Declaration of variable m_PreviousState
=======
    QState* mp_PssmInit;                        ///< Program state Init
    QState* mp_PssmPreTest;                     ///< program state PreTest
    QState* mp_PssmReadyToHeatLevelSensorS1;    ///<  Definition/Declaration of variable mp_PssmReadyToHeatLevelSensorS1
    QState* mp_PssmReadyToHeatLevelSensorS2;    ///<  Definition/Declaration of variable mp_PssmReadyToHeatLevelSensorS2
    QState* mp_PssmReadyToTubeBefore;           ///<  Definition/Declaration of variable mp_PssmReadyToTubeBefore
    QState* mp_PssmReadyToTubeAfter;            ///<  Definition/Declaration of variable mp_PssmReadyToTubeAfter
    QState* mp_PssmReadyToSeal;                 ///<  Definition/Declaration of variable mp_PssmReadyToSeal
    QState* mp_PssmReadyToFill;                 ///<  Definition/Declaration of variable mp_PssmReadyToFill
    QState* mp_PssmReadyToDrain;                ///<  Definition/Declaration of variable mp_PssmReadyToDrain
    QState* mp_PssmSoak;                        ///<  Definition/Declaration of variable mp_PssmSoak
    QState* mp_PssmPause;                       ///<  Definition/Declaration of variable mp_PssmPause
    QState* mp_PssmPauseDrain;                  ///<  Definition/Declaration of variable mp_PssmPauseDrain
    QState* mp_PssmAborting;                    ///<  Definition/Declaration of variable mp_PssmAborting
    QState* mp_PssmAborted;                     ///<  Definition/Declaration of variable mp_PssmAborted
    QState* mp_PssmStepFinish;                  ///<  Definition/Declaration of variable mp_PssmStepFinish
    QFinalState* mp_PssmProgramFinish;          ///<  Definition/Declaration of variable mp_PssmProgramFinish
    QState* mp_PssmError;                       ///<  Definition/Declaration of variable mp_PssmError
    SchedulerStateMachine_t m_PreviousState;    ///<  Definition/Declaration of variable m_PreviousState
>>>>>>> Initial version of Refacting Scheduler

};











}

#endif // PROGRAMSTEPSTATEMACHINE_H
