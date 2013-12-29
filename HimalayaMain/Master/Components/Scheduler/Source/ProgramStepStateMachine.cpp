#include "../Include/ProgramStepStateMachine.h"
#include "EventHandler/Include/CrisisEventHandler.h"
#include <QHistoryState>

namespace Scheduler
{

ProgramStepStateMachine::ProgramStepStateMachine()
{
    m_PreviousState = PSSM_UNDEF;

    mp_ProgramStepStateMachine = new QStateMachine();
    mp_PssmSelfTest = new QState(mp_ProgramStepStateMachine);
    mp_PssmStInit = new QState(mp_PssmSelfTest);
    mp_PssmStTempChecking = new QState(mp_PssmSelfTest);
    mp_PssmStCurrentChecking = new QState(mp_PssmSelfTest);
    mp_PssmStVoltageChecking = new QState(mp_PssmSelfTest);
    mp_PssmStRVPositionChecking = new QState(mp_PssmSelfTest);
    mp_PssmStPressureChecking = new QState(mp_PssmSelfTest);
    mp_PssmStSealingChecking = new QState(mp_PssmSelfTest);
    mp_PssmStStationChecking = new QState(mp_PssmSelfTest);
    mp_PssmStStationCheckFinish = new QState(mp_PssmSelfTest);
    mp_PssmStDone = new QState(mp_PssmSelfTest);
    QHistoryState *stHistory = new QHistoryState(mp_PssmSelfTest);



    mp_PssmInit = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToHeatLevelSensorS1 = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToHeatLevelSensorS2 = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToTubeBefore = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToTubeAfter = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToSeal = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToFill = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToDrain = new QState(mp_ProgramStepStateMachine);
    mp_PssmSoak = new QState(mp_ProgramStepStateMachine);
    mp_PssmStepFinish = new QState(mp_ProgramStepStateMachine);
    mp_PssmError = new QState(mp_ProgramStepStateMachine);
    mp_PssmPause = new QState(mp_ProgramStepStateMachine);
    mp_PssmPauseDrain = new QState(mp_ProgramStepStateMachine);
    mp_PssmAborting = new QState(mp_ProgramStepStateMachine);
    mp_PssmAborted = new QState(mp_ProgramStepStateMachine);
    mp_PssmProgramFinish = new QFinalState(mp_ProgramStepStateMachine);

    mp_PssmSelfTest->setInitialState(mp_PssmStInit);

    mp_ProgramStepStateMachine->setInitialState(mp_PssmSelfTest);
    mp_PssmSelfTest->addTransition(this, SIGNAL(StDone()), mp_PssmInit);
    mp_PssmStInit->addTransition(this, SIGNAL(StInitOK()), mp_PssmStTempChecking);
    mp_PssmStTempChecking->addTransition(this, SIGNAL(StTempOK()), mp_PssmStCurrentChecking);
    mp_PssmStCurrentChecking->addTransition(this, SIGNAL(StCurrentOK()), mp_PssmStVoltageChecking);
    mp_PssmStVoltageChecking->addTransition(this, SIGNAL(StVoltageOK()), mp_PssmStRVPositionChecking);
    mp_PssmStRVPositionChecking->addTransition(this, SIGNAL(StRVPositionOK()), mp_PssmStPressureChecking);
    mp_PssmStPressureChecking->addTransition(this, SIGNAL(StPressureOK()), mp_PssmStSealingChecking);
    mp_PssmStSealingChecking->addTransition(this, SIGNAL(StSealingOK()), mp_PssmStStationChecking);
    mp_PssmStStationChecking->addTransition(this, SIGNAL(StGetStationcheckResult()), mp_PssmStStationCheckFinish);
    mp_PssmStStationCheckFinish->addTransition(this, SIGNAL(StStationLeft()), mp_PssmStStationChecking);
    mp_PssmStStationCheckFinish->addTransition(this, SIGNAL(StStationOK()), mp_PssmStDone);

    mp_PssmSelfTest->addTransition(this, SIGNAL(Pause()), mp_PssmPause);
    mp_PssmPause->addTransition(this, SIGNAL(ResumeToSelftest()), stHistory);
    mp_PssmSelfTest->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmError->addTransition(this, SIGNAL(ResumeToSelftest()), stHistory);//verify later
    mp_PssmSelfTest->addTransition(this, SIGNAL(abort()), mp_PssmAborted);

    mp_PssmInit->addTransition(this, SIGNAL(TempsReady()), mp_PssmReadyToHeatLevelSensorS1);
    mp_PssmReadyToHeatLevelSensorS1->addTransition(this, SIGNAL(LevelSensorTempS1Ready()), mp_PssmReadyToHeatLevelSensorS2);
    mp_PssmReadyToHeatLevelSensorS2->addTransition(this, SIGNAL(LevelSensorTempS2Ready()), mp_PssmReadyToTubeBefore);
    mp_PssmReadyToTubeBefore->addTransition(this, SIGNAL(HitTubeBefore()), mp_PssmReadyToFill);
    mp_PssmReadyToFill->addTransition(this, SIGNAL(FillFinished()), mp_PssmReadyToSeal);
    mp_PssmReadyToSeal->addTransition(this, SIGNAL(HitSeal()), mp_PssmSoak);
    mp_PssmSoak->addTransition(this, SIGNAL(SoakFinished()), mp_PssmReadyToTubeAfter);
    mp_PssmReadyToTubeAfter->addTransition(this, SIGNAL(HitTubeAfter()), mp_PssmReadyToDrain);
    mp_PssmReadyToDrain->addTransition(this, SIGNAL(DrainFinished()), mp_PssmStepFinish);
    mp_PssmStepFinish->addTransition(this, SIGNAL(ProgramFinished()), mp_PssmProgramFinish);
    mp_PssmStepFinish->addTransition(this, SIGNAL(StepFinished()), mp_PssmInit);

    mp_PssmInit->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToHeatLevelSensorS1->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToHeatLevelSensorS2->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToTubeBefore->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToTubeAfter->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToSeal->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToFill->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToDrain->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmSoak->addTransition(this, SIGNAL(Error()), mp_PssmError);

    mp_PssmInit->addTransition(this, SIGNAL(Pause()), mp_PssmPause);
    mp_PssmReadyToHeatLevelSensorS1->addTransition(this, SIGNAL(Pause()), mp_PssmPause);
    mp_PssmReadyToHeatLevelSensorS2->addTransition(this, SIGNAL(Pause()), mp_PssmPause);
    mp_PssmReadyToTubeBefore->addTransition(this, SIGNAL(Pause()), mp_PssmPause);
    mp_PssmReadyToSeal->addTransition(this, SIGNAL(Pause()), mp_PssmPause);
    mp_PssmReadyToFill->addTransition(this, SIGNAL(Pause()), mp_PssmPause);
    mp_PssmSoak->addTransition(this, SIGNAL(Pause()), mp_PssmPause);
    mp_PssmStepFinish->addTransition(this, SIGNAL(Pause()), mp_PssmPause);

    mp_PssmReadyToTubeAfter->addTransition(this, SIGNAL(Pause()), mp_PssmPauseDrain);
    mp_PssmReadyToDrain->addTransition(this, SIGNAL(Pause()), mp_PssmPauseDrain);

    mp_PssmInit->addTransition(this, SIGNAL(Abort()), mp_PssmAborted);
    mp_PssmReadyToHeatLevelSensorS1->addTransition(this, SIGNAL(Abort()), mp_PssmAborted);
    mp_PssmReadyToHeatLevelSensorS2->addTransition(this, SIGNAL(Abort()), mp_PssmAborted);
    mp_PssmReadyToTubeBefore->addTransition(this, SIGNAL(Abort()), mp_PssmAborted);
    mp_PssmStepFinish->addTransition(this, SIGNAL(Abort()), mp_PssmAborted);
    mp_PssmAborting->addTransition(this, SIGNAL(Abort()), mp_PssmAborted);

    mp_PssmReadyToFill->addTransition(this, SIGNAL(Abort()), mp_PssmAborting);
    mp_PssmReadyToSeal->addTransition(this, SIGNAL(Abort()), mp_PssmAborting);
    mp_PssmSoak->addTransition(this, SIGNAL(Abort()), mp_PssmAborting);
    mp_PssmReadyToTubeAfter->addTransition(this, SIGNAL(Abort()), mp_PssmAborting);
    mp_PssmReadyToDrain->addTransition(this, SIGNAL(Abort()), mp_PssmAborting);


    mp_PssmPause->addTransition(this, SIGNAL(ResumeToInit()), mp_PssmInit);
    mp_PssmPause->addTransition(this, SIGNAL(ResumeToHeatLevelSensorS1()), mp_PssmReadyToHeatLevelSensorS1);
    mp_PssmPause->addTransition(this, SIGNAL(ResumeToHeatLevelSensorS2()), mp_PssmReadyToHeatLevelSensorS2);
    mp_PssmPause->addTransition(this, SIGNAL(ResumeToReadyToFill()), mp_PssmReadyToFill);
    mp_PssmPause->addTransition(this, SIGNAL(ResumeToSoak()), mp_PssmSoak);
    mp_PssmPause->addTransition(this, SIGNAL(ResumeToStepFinished()), mp_PssmStepFinish);
    mp_PssmPause->addTransition(this, SIGNAL(Abort()), mp_PssmAborted);

    mp_PssmPauseDrain->addTransition(this, SIGNAL(ResumeToReadyToTubeAfter()), mp_PssmReadyToTubeAfter);
    mp_PssmPauseDrain->addTransition(this, SIGNAL(Abort()), mp_PssmAborting);


    connect(mp_PssmInit, SIGNAL(entered()), this, SIGNAL(OnInit()));
    connect(mp_PssmReadyToHeatLevelSensorS1, SIGNAL(entered()), this, SIGNAL(OnHeatLevelSensorTempS1()));
    connect(mp_PssmReadyToHeatLevelSensorS2, SIGNAL(entered()), this, SIGNAL(OnHeatLevelSensorTempS2()));
    connect(mp_PssmReadyToTubeBefore, SIGNAL(entered()), this, SIGNAL(OnMoveToTubeBefore()));
    connect(mp_PssmReadyToTubeAfter, SIGNAL(entered()), this, SIGNAL(OnMoveToTubeAfter()));
    connect(mp_PssmReadyToSeal, SIGNAL(entered()), this, SIGNAL(OnMoveToSeal()));
    connect(mp_PssmReadyToFill, SIGNAL(entered()), this, SIGNAL(OnFill()));
    connect(mp_PssmSoak, SIGNAL(entered()), this, SIGNAL(OnSoak()));
    connect(mp_PssmReadyToDrain, SIGNAL(entered()), this, SIGNAL(OnDrain()));
    connect(mp_PssmAborting, SIGNAL(entered()), this, SIGNAL(OnAborting()));
    connect(mp_PssmAborted, SIGNAL(entered()), this, SIGNAL(OnAborted()));
    //connect(mp_PssmPause, SIGNAL(entered()), this, SIGNAL(OnPause()));
    //connect(mp_PssmPauseDrain, SIGNAL(entered()), this, SIGNAL(OnPauseDrain()));

    connect(mp_PssmInit, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmReadyToHeatLevelSensorS1, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmReadyToHeatLevelSensorS2, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmReadyToTubeBefore, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmReadyToTubeAfter, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmReadyToSeal, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmReadyToFill, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmSoak, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmReadyToDrain, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmStepFinish, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmProgramFinish, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmError, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmPause, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmPauseDrain, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmAborted, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_PssmAborting, SIGNAL(entered()), this, SLOT(OnStateChanged()));

    connect(mp_PssmReadyToFill,SIGNAL(exited()),this, SIGNAL(OnStopFill()));
    connect(mp_PssmReadyToDrain,SIGNAL(exited()),this,SIGNAL(OnStopDrain()));

}

ProgramStepStateMachine::~ProgramStepStateMachine()
{
    delete  mp_PssmInit;
    delete  mp_PssmReadyToHeatLevelSensorS1;
    delete  mp_PssmReadyToHeatLevelSensorS2;
    delete  mp_PssmReadyToTubeBefore;
    delete  mp_PssmReadyToTubeAfter;
    delete  mp_PssmReadyToSeal;
    delete  mp_PssmReadyToFill;
    delete  mp_PssmReadyToDrain;
    delete  mp_PssmSoak;
    delete  mp_PssmStepFinish;
    delete  mp_PssmProgramFinish;
    delete  mp_PssmError;
    delete  mp_PssmPause;
    delete  mp_PssmPauseDrain;
    delete  mp_PssmAborted;
    delete  mp_PssmAborting;
    delete  mp_ProgramStepStateMachine;
}

void ProgramStepStateMachine::Start()
{
    mp_ProgramStepStateMachine->start();
}

void ProgramStepStateMachine::Stop()
{
    mp_ProgramStepStateMachine->stop();
}

void ProgramStepStateMachine::NotifyStInitOK()
{
    emit StInitOK();
}

void ProgramStepStateMachine::NotifyStTempOK()
{
    emit StTempOK();
}

void ProgramStepStateMachine::NotifyStCurrentOK()
{
    emit StCurrentOK();
}

void ProgramStepStateMachine::NotifyStVoltageOK()
{
    emit StVoltageOK();
}

void ProgramStepStateMachine::NotifyStRVPositionOK()
{
    emit StRVPositionOK();
}

void ProgramStepStateMachine::NotifyStPressureOK()
{
    emit StPressureOK();
}

void ProgramStepStateMachine::NotifyStSealingOK()
{
    emit StSealingOK();
}

void ProgramStepStateMachine::NotifyStGetStationcheckResult()
{
    emit StGetStationcheckResult();
}

void ProgramStepStateMachine::NotifyStStationLeft()
{
    emit StStationLeft();
}

void ProgramStepStateMachine::NotifyStStationOK()
{
    emit StStationOK();
}

void ProgramStepStateMachine::NotifyStDone()
{
    emit StDone();
}

void ProgramStepStateMachine::NotifyTempsReady()
{
    emit TempsReady();
}

void ProgramStepStateMachine::NotifyLevelSensorTempS1Ready()
{
    emit LevelSensorTempS1Ready();
}

void ProgramStepStateMachine::NotifyLevelSensorTempS2Ready()
{
    emit LevelSensorTempS2Ready();
}

void ProgramStepStateMachine::NotifyHitTubeBefore()
{
    emit HitTubeBefore();
}

void ProgramStepStateMachine::NotifyFillFinished()
{
    emit FillFinished();
}

void ProgramStepStateMachine::NotifyHitSeal()
{
    emit HitSeal();
}

void ProgramStepStateMachine::NotifySoakFinished()
{
    emit SoakFinished();
}

void ProgramStepStateMachine::NotifyHitTubeAfter()
{
    emit HitTubeAfter();
}

void ProgramStepStateMachine::NotifyDrainFinished()
{
    emit DrainFinished();
}

void ProgramStepStateMachine::NotifyStepFinished()
{
    emit StepFinished();
}

void ProgramStepStateMachine::NotifyProgramFinished()
{
    emit ProgramFinished();
}

void ProgramStepStateMachine::NotifyError()
{
    emit Error();
}

void ProgramStepStateMachine::NotifyPause(ProgramStepStateMachine_t PreviousState)
{
   m_PreviousState = PreviousState;
   emit Pause();
}

void ProgramStepStateMachine::NotifyAbort()
{
    m_PreviousState = GetCurrentState();
    emit Abort();
}

void ProgramStepStateMachine::NotifyResumeToSelftest()
{
    emit ResumeToSelftest();
}

void ProgramStepStateMachine::NotifyResume()
{
    //emit Resume(m_PreviousState);
    if( m_PreviousState == (PSSM_INIT))
    {
        emit ResumeToInit();
    }
    else if(m_PreviousState == (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1))
    {
        emit ResumeToHeatLevelSensorS1();
    }
    else if(m_PreviousState == (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2))
    {
        emit ResumeToHeatLevelSensorS2();
    }
    else if(m_PreviousState == (PSSM_READY_TO_TUBE_BEFORE))
    {
        emit ResumeToReadyToFill();
    }
    else if(m_PreviousState == (PSSM_READY_TO_FILL))
    {
        emit ResumeToReadyToFill();
    }
    else if(m_PreviousState == (PSSM_READY_TO_SEAL))
    {
        emit ResumeToSoak();
    }
    else if(m_PreviousState == (PSSM_SOAK))
    {
        emit ResumeToSoak();
    }
    else if(m_PreviousState == (PSSM_STEP_FINISH))
    {
        emit ResumeToStepFinished();
    }
    else if(m_PreviousState == (PSSM_ERROR))
    {
        //str = " PSSM_ERROR";
    }
    else
    {
        //should not enter here
    }
    m_PreviousState = PSSM_PAUSE;
}

void ProgramStepStateMachine::NotifyResumeDrain()
{
    if(m_PreviousState == (PSSM_READY_TO_TUBE_AFTER))
    {
        emit ResumeToReadyToTubeAfter();
    }
    else if(m_PreviousState == (PSSM_READY_TO_DRAIN))
    {
        emit ResumeToReadyToTubeAfter();
    }
    m_PreviousState = PSSM_PAUSE_DRAIN;
}

ProgramStepStateMachine_t ProgramStepStateMachine::GetCurrentState()
{
    if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmInit))
    {
        return PSSM_INIT;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmReadyToHeatLevelSensorS1))
    {
        return PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmReadyToHeatLevelSensorS2))
    {
        return PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmReadyToTubeBefore))
    {
        return PSSM_READY_TO_TUBE_BEFORE;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmReadyToTubeAfter))
    {
        return PSSM_READY_TO_TUBE_AFTER;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmReadyToFill))
    {
        return PSSM_READY_TO_FILL;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmReadyToSeal))
    {
        return PSSM_READY_TO_SEAL;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmSoak))
    {
        return PSSM_SOAK;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmReadyToDrain))
    {
        return PSSM_READY_TO_DRAIN;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStepFinish))
    {
        return PSSM_STEP_FINISH;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmProgramFinish))
    {
        return PSSM_PROGRAM_FINISH;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmError))
    {
        return PSSM_ERROR;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmPause))
    {
        return PSSM_PAUSE;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmPauseDrain))
    {
        return PSSM_PAUSE_DRAIN;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmAborting))
    {
        return PSSM_ABORTING;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmAborted))
    {
        return PSSM_ABORTED;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmSelfTest))
    {
        if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStInit))
        {
            return PSSM_ST_INIT;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStTempChecking))
        {
            return PSSM_ST_TEMP_CHECKING;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStCurrentChecking))
        {
            return PSSM_ST_CURRENT_CHECKING;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStVoltageChecking))
        {
            return PSSM_ST_VOLTAGE_CHECKING;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStRVPositionChecking))
        {
            return PSSM_ST_RV_POSITION_CHECKING;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStPressureChecking))
        {
            return PSSM_ST_PRESSURE_CHECKING;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStSealingChecking))
        {
            return PSSM_ST_SEALING_CHECKING;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStStationChecking))
        {
            return PSSM_ST_STATION_CHECKING;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStStationCheckFinish))
        {
            return PSSM_ST_STATION_CHECK_FINISH;
        }
        else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmStDone))
        {
            return PSSM_ST_DONE;
        }
        //return PSSM_ST; //this is the parent state
    }
    else
    {
        return PSSM_UNDEF;
    }
}

ProgramStepStateMachine_t ProgramStepStateMachine::GetPreviousState()
{
    return m_PreviousState;
}

void ProgramStepStateMachine::OnStateChanged()
{
    ProgramStepStateMachine_t curState = GetCurrentState();
    QString str = "";
    quint32 strid = STR_PROGRAM_STEP_STATE_UNKNOWN;
    if( curState == (PSSM_INIT))
    {
        str = " PSSM_INIT";
        strid = STR_PROGRAM_STEP_STATE_START;
    }
    else if(curState == (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1))
    {
         str = " PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1";
         strid = STR_PROGRAM_STEP_STATE_HEAT_LEVEL_SENSOR_1;
    }
    else if(curState == (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2))
    {
         str = " PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2";
         strid = STR_PROGRAM_STEP_STATE_HEAT_LEVEL_SENSOR_2;
    }
    else if(curState == (PSSM_READY_TO_TUBE_BEFORE))
    {
         str = " PSSM_READY_TO_TUBE_BEFORE";
         strid = STR_PROGRAM_STEP_STATE_MOVE_TUBE_TO_FILL;
    }
    else if(curState == (PSSM_READY_TO_TUBE_AFTER))
    {
         str = " PSSM_READY_TO_TUBE_AFTER";
         strid = STR_PROGRAM_STEP_STATE_MOVE_TUBE_TO_DRAIN;
    }
    else if(curState == (PSSM_READY_TO_FILL))
    {
         str = " PSSM_READY_TO_FILL";
         strid = STR_PROGRAM_STEP_STATE_FILL;
    }
    else if(curState == (PSSM_READY_TO_SEAL))
    {
         str = " PSSM_READY_TO_SEAL";
         strid = STR_PROGRAM_STEP_STATE_SEAL;
    }
    else if(curState == (PSSM_SOAK))
    {
         str = " PSSM_SOAK";
         strid = STR_PROGRAM_STEP_STATE_SOAK;
    }
    else if(curState == (PSSM_READY_TO_DRAIN))
    {
         str = " PSSM_READY_TO_DRAIN";
         strid = STR_PROGRAM_STEP_STATE_DRAIN;
    }
    else if(curState == (PSSM_STEP_FINISH))
    {
         str = " PSSM_STEP_FINISH";
         strid = STR_PROGRAM_STEP_STATE_FINISH;//todo: update later
    }
    else if(curState == (PSSM_PROGRAM_FINISH))
    {
         str = " PSSM_PROGRAM_FINISH";
         strid = STR_PROGRAM_STEP_STATE_FINISH;//todo: update later
    }
    else if(curState == (PSSM_ERROR))
    {
         str = " PSSM_ERROR";
         strid = STR_PROGRAM_STEP_STATE_ERROR;
    }
    else if(curState == PSSM_PAUSE)
    {
         str = " PSSM_PAUSE";
         strid = STR_PROGRAM_STEP_STATE_PAUSE;
    }
    else if(curState == PSSM_PAUSE_DRAIN)
    {
         str = " PSSM_PAUSE_DRAIN";
         strid = STR_PROGRAM_STEP_STATE_PAUSE_DRAIN;
    }
    else if(curState == PSSM_ABORTED)
    {
         str = " PSSM_ABORTED";
         strid = STR_PROGRAM_STEP_STATE_ABORTED;
    }
    else if(curState == PSSM_ABORTING)
    {
         str = " PSSM_ABORTING";
         strid = STR_PROGRAM_STEP_STATE_ABORTING;
    }
    else
    {
        //todo: log error here
         str = "UNKNOWEN STATE";
         strid = STR_PROGRAM_STEP_STATE_UNKNOWN;
    }
    qDebug() << "Program step state machine enter:" << str;
    LOG_STR_ARG(STR_PROGRAM_STEP_STATE,Global::tTranslatableStringList()<<Global::TranslatableString(strid));
}

}
