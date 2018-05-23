#include "Scheduler/Include/TPExecutor.h"
#include "Scheduler/Include/TPEvent.h"

using namespace Scheduler::TPExecutorStates;
namespace Scheduler {

TPExecutor::TPExecutor(const QString& name, EventDispatcher* pEventDispatcher, SchedulerMainThreadController* pController)
    : IEventHandler(name, pEventDispatcher),
      m_pController(pController),
      m_pInit(nullptr),
      m_pIdle(nullptr),
      m_pBusy(nullptr),
      m_pNextAction(nullptr),
      m_pSoaking(nullptr),
      m_pExecuting(nullptr)

{

}

TPExecutor::~TPExecutor()
{
    if(m_pInit != nullptr)
    {
        delete m_pInit;
        m_pInit = nullptr;
    }
    if(m_pIdle != nullptr)
    {
        delete m_pIdle;
        m_pIdle = nullptr;
    }
    if(m_pBusy != nullptr)
    {
        delete m_pBusy;
        m_pBusy = nullptr;
    }
    if(m_pSoaking != nullptr)
    {
        delete m_pSoaking;
        m_pSoaking = nullptr;
    }
    if(m_pNextAction != nullptr)
    {
        delete m_pNextAction;
        m_pNextAction = nullptr;
    }
    if(m_pExecuting != nullptr)
    {
        delete m_pExecuting;
        m_pExecuting = nullptr;
    }
}

bool TPExecutor::HandleEvent(QEvent* event)
{
//    auto tpEvent = dynamic_cast<TPEvent*>(event);
//    if(tpEvent != nullptr && tpEvent->EventArgs() != nullptr && m_pStateMachine->isRunning())
//    {
//        qDebug() << tpEvent->toString();
//        auto pCurrent = dynamic_cast<IState*>(*m_pStateMachine->configuration().begin());
//        return pCurrent->HandleEvent(tpEvent);
//    }

    return IEventHandler::HandleEvent(event);
}

void TPExecutor::CreateStateMachine()
{
    m_pStateMachine =  new QStateMachine(QState::ExclusiveStates);

    m_pInit = new Init(this, m_pController);
    m_pIdle = new Idle(this, m_pController);


    m_pStateMachine->addState(m_pInit);
    m_pStateMachine->addState(m_pIdle);
    m_pBusy = new Busy(this, m_pController);
    m_pBusy->setObjectName("TPExecutor_Busy_State");

    m_pStateMachine->addState(m_pBusy);
    m_pStateMachine->setInitialState(m_pInit);

    m_pInit->addTransition(new EventTransition(TPTransition_t::Initial, m_pInit, m_pIdle));
    m_pInit->addTransition(new EventTransition(TPTransition_t::Self, m_pInit, nullptr));

    m_pIdle->addTransition(new EventTransition(TPTransition_t::Start, m_pIdle, m_pBusy));
    m_pIdle->addTransition(new EventTransition(TPTransition_t::Self, m_pIdle, nullptr));;

    m_pBusy->addTransition(new EventTransition(TPTransition_t::Done, m_pBusy, m_pIdle));
    m_pBusy->addTransition(new EventTransition(TPTransition_t::Self, m_pBusy, nullptr));

    m_pSoaking = new Soaking(this, m_pController);
    m_pNextAction = new NextAction(this, m_pController);
    m_pExecuting = new Executing(this, m_pController);
    m_pSoaking->setParent(m_pBusy);
    m_pNextAction->setParent(m_pBusy);
    m_pExecuting->setParent(m_pBusy);
    m_pBusy->setInitialState(m_pNextAction);

    m_pNextAction->addTransition(new EventTransition(TPTransition_t::Execute, m_pNextAction, m_pExecuting));
    m_pNextAction->addTransition(new EventTransition(TPTransition_t::Timeout, m_pNextAction, nullptr));

    m_pExecuting->addTransition(new EventTransition(TPTransition_t::ActionDone, m_pExecuting, m_pNextAction));
    m_pExecuting->addTransition(new EventTransition(TPTransition_t::Timeout, m_pExecuting, nullptr));;

//    m_pSoaking->addTransition(new EventTransition(TPTransition_t::SoakingDone, m_pSoaking, m_pNextAction));
//    m_pSoaking->addTransition(new EventTransition(TPTransition_t::Timeout, m_pSoaking, nullptr));

}

}
