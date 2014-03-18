#if 0
#include "../Include/SelfTestStateMachine.h"

namespace Scheduler
{
CSelfTestStateMachine::CSelfTestStateMachine()
{
    mp_SelfTestStateMachine = new QStateMachine();
    mp_StInit = new QState(mp_SelfTestStateMachine);
    mp_StTempReady = new QState(mp_SelfTestStateMachine);
    mp_StStationChecking= new QState(mp_SelfTestStateMachine);
    mp_StStationCheckFinish = new QState(mp_SelfTestStateMachine);
    mp_StFinish = new QFinalState(mp_SelfTestStateMachine);

    mp_SelfTestStateMachine->setInitialState(mp_StInit);
    mp_StInit->addTransition(this, SIGNAL(TempsReady()), mp_StTempReady);
    mp_StTempReady->addTransition(this, SIGNAL(CheckStation()), mp_StStationChecking);
    mp_StStationChecking->addTransition(this, SIGNAL(GotStationCheckResult()), mp_StStationCheckFinish);
    mp_StStationCheckFinish->addTransition(this, SIGNAL(CheckStation()), mp_StStationChecking);
    mp_StStationCheckFinish->addTransition(this, SIGNAL(StationCheckFinished()), mp_StFinish);
}


CSelfTestStateMachine::~SelfTestStateMachine()
{
    delete mp_StInit;
    delete mp_StTempReady;
    delete mp_StStationChecking;
    delete mp_StStationCheckFinish;
    delete mp_StFinish;
    delete mp_SelfTestStateMachine;
}

void CSelfTestStateMachine::Start()
{
    mp_SelfTestStateMachine->start();
}

void CSelfTestStateMachine::Stop()
{
    mp_SelfTestStateMachine->stop();
}

void CSelfTestStateMachine::NotifyTempsReady()
{
    emit TempsReady();
}

void CSelfTestStateMachine::NotifyCheckStation()
{
    emit CheckStation();
}

void CSelfTestStateMachine::NotifyGotCheckStationResult()
{
    emit GotStationCheckResult();
}

void CSelfTestStateMachine::NotifyCheckStaionFinished()
{
    emit StationCheckFinished();
}

SelfTestStateMachine_t CSelfTestStateMachine::GetCurrentState()
{
    if(mp_SelfTestStateMachine->configuration().contains(mp_StInit))
    {
        return SELF_TEST_INIT;
    }
    else if(mp_SelfTestStateMachine->configuration().contains(mp_StTempReady))
    {
        return SELF_TEST_TEMP_READY;
    }
    else if(mp_SelfTestStateMachine->configuration().contains(mp_StStationChecking))
    {
        return SELF_TEST_BOTTLE_CHECKING;
    }
    else if(mp_SelfTestStateMachine->configuration().contains(mp_StStationCheckFinish))
    {
        return SELF_TEST_BOTTLE_CHECK_FINISH;
    }
    else if(mp_SelfTestStateMachine->configuration().contains(mp_StFinish))
    {
        return SELF_TEST_FINISH;
    }
    else
    {
        return SELF_TEST_UNDEF;
    }
}

}
#endif
