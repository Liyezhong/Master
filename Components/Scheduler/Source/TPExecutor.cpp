#include "Scheduler/Include/TPExecutor.h"
#include "Scheduler/Include/TPEvent.h"
#include "Scheduler/Include/Actions/IAction.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Include/SessionManager.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"

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
    m_pSession = nullptr;
    m_pAction = -1;
    if(pController != nullptr)
    {
        m_pSessionManager = pController->GetSessionManager();
    }
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
    auto tpEvent = dynamic_cast<TPEvent*>(event);
    if(tpEvent != nullptr && m_pStateMachine->isRunning())
    {
        qDebug() << tpEvent->toString();
        qDebug() << objectName();
        foreach(auto state, m_pStateMachine->configuration())
        {
            auto pCurrent = dynamic_cast<IState*>(state);

            // Busy state is a sub state machine, ignore it
            if(!pCurrent->objectName().contains("TPExecutor_Busy_State"))
            {
                return pCurrent->HandleEvent(tpEvent);
            }
        }
    }

    return false;
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

Session *TPExecutor::GetCurrentSession()
{
    m_pSession =  m_pSessionManager->GetSessionByRetortId(objectName(), Session::Ready);
    return m_pSession;
}

int TPExecutor::GetCurrentAction()
{
    return m_pAction;
}

void TPExecutor::SetCurrentAction(int pAction)
{
    m_pAction = pAction;
}

}
