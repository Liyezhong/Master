#include "../Include/ProgramStepStateMachine.h"

namespace Scheduler
{

ProgramStepStateMachine::ProgramStepStateMachine()
{
    mp_ProgramStepStateMachine = new QStateMachine();
    mp_PssmInit = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToHeatLevelSensorS1 = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToHeatLevelSensorS2 = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToTubeBefore = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToTubeAfter = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToSeal = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToFill = new QState(mp_ProgramStepStateMachine);
    mp_PssmReadyToDrain = new QState(mp_ProgramStepStateMachine);
    mp_PssmSoak = new QState(mp_ProgramStepStateMachine);
    mp_PssmFinish = new QState(mp_ProgramStepStateMachine);
    mp_PssmError = new QState(mp_ProgramStepStateMachine);
    mp_ProgramStepStateMachine->setInitialState(mp_PssmInit);
    mp_PssmInit->addTransition(this, SIGNAL(TempsReady()), mp_PssmReadyToHeatLevelSensorS1);
    mp_PssmReadyToHeatLevelSensorS1->addTransition(this, SIGNAL(LevelSensorTempS1Ready()), mp_PssmReadyToHeatLevelSensorS2);
    mp_PssmReadyToHeatLevelSensorS2->addTransition(this, SIGNAL(LevelSensorTempS2Ready()), mp_PssmReadyToTubeBefore);
    mp_PssmReadyToTubeBefore->addTransition(this, SIGNAL(HitTubeBefore()), mp_PssmReadyToFill);
    mp_PssmReadyToFill->addTransition(this, SIGNAL(FillFinished()), mp_PssmReadyToSeal);
    mp_PssmReadyToSeal->addTransition(this, SIGNAL(HitSeal()), mp_PssmSoak);
    mp_PssmSoak->addTransition(this, SIGNAL(SoakFinished()), mp_PssmReadyToTubeAfter);
    mp_PssmReadyToTubeAfter->addTransition(this, SIGNAL(HitTubeAfter()), mp_PssmReadyToDrain);
    mp_PssmReadyToDrain->addTransition(this, SIGNAL(DrainFinished()), mp_PssmFinish);
    mp_PssmInit->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToHeatLevelSensorS1->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToHeatLevelSensorS2->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToTubeBefore->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToTubeAfter->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToSeal->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToFill->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmReadyToDrain->addTransition(this, SIGNAL(Error()), mp_PssmError);
    mp_PssmSoak->addTransition(this, SIGNAL(Error()), mp_PssmError);

    connect(mp_PssmInit, SIGNAL(entered()), this, SIGNAL(OnInit()));
    connect(mp_PssmReadyToHeatLevelSensorS1, SIGNAL(entered()), this, SIGNAL(OnHeatLevelSensorTempS1()));
    connect(mp_PssmReadyToHeatLevelSensorS2, SIGNAL(entered()), this, SIGNAL(OnHeatLevelSensorTempS2()));
    connect(mp_PssmReadyToTubeBefore, SIGNAL(entered()), this, SIGNAL(OnMoveToTubeBefore()));
    connect(mp_PssmReadyToTubeAfter, SIGNAL(entered()), this, SIGNAL(OnMoveToTubeAfter()));
    connect(mp_PssmReadyToSeal, SIGNAL(entered()), this, SIGNAL(OnMoveToSeal()));

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
    delete  mp_PssmFinish;
    delete  mp_PssmError;
    delete  mp_ProgramStepStateMachine;
}

void ProgramStepStateMachine::Start()
{
    mp_ProgramStepStateMachine->start();
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

void ProgramStepStateMachine::NotifyError()
{
    emit Error();
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
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmFinish))
    {
        return PSSM_FINISH;
    }
    else if(mp_ProgramStepStateMachine->configuration().contains(mp_PssmError))
    {
        return PSSM_ERROR;
    }
}

}
