#include "../Include/SelfTestStateMachine.h"
#include <QDebug>
namespace Scheduler
{
SelfTestStateMachine::SelfTestStateMachine()
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


SelfTestStateMachine::~SelfTestStateMachine()
{
    delete mp_SelfTestStateMachine;
    delete mp_StInit;
    delete mp_StTempReady;
    delete mp_StStationChecking;
    delete mp_StStationCheckFinish;
    delete mp_StFinish;
}

void SelfTestStateMachine::Start()
{
    mp_SelfTestStateMachine->start();
}

void SelfTestStateMachine::NotifyTempsReady()
{
    emit TempsReady();
}

void SelfTestStateMachine::NotifyCheckStation()
{
    emit CheckStation();
}

void SelfTestStateMachine::NotifyGotCheckStationResult()
{
    emit GotStationCheckResult();
}

void SelfTestStateMachine::NotifyCheckStaionFinished()
{
    emit StationCheckFinished();
}

SelfTestStateMachine_t SelfTestStateMachine::GetCurrentState()
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
