#include "Scheduler/Include/InstrumentManager.h"
#include "Scheduler/Include/TPExecutor.h"
#include "Scheduler/Include/TPEvent.h"
#include "Scheduler/Include/TPTransition.h"

namespace Scheduler{
InstrumentManager::InstrumentManager(const QString& name, EventDispatcher* pParent, SchedulerMainThreadController* pController)
    : IEventHandler (name, pParent),
      m_pController(pController),
      m_pInit(nullptr),
      m_pIdle(nullptr)
{

}

void InstrumentManager::Initialize(QList<QString> retorts)
{
    m_pEventDispatcher->Register(QSharedPointer<IEventHandler>(this));
    foreach(auto retort, retorts)
    {

        if (!retort.isEmpty())
        {
//            qDebug()<<"Get retort name:"<<retort;
            if (m_TPExecutorList.find(retort) == m_TPExecutorList.end())
            {
                auto executor = QSharedPointer<TPExecutor>(new TPExecutor(retort, m_pEventDispatcher));
                m_TPExecutorList.insert(retort, executor);
                m_pEventDispatcher->Register(executor);
            }

        }
    }
}

void InstrumentManager::Start()
{
    if(m_pStateMachine != nullptr && !m_pStateMachine->isRunning())
    {
        m_pStateMachine->start();
    }
}

void InstrumentManager::Stop()
{
    if(m_pStateMachine != nullptr && m_pStateMachine->isRunning())
    {
        m_pStateMachine->stop();
    }
}



void InstrumentManager::CreateStateMachine()
{
    m_pStateMachine =  (new QStateMachine(QState::ExclusiveStates));

    m_pInit = new Init(this, m_pController);
    m_pIdle = new Idle(this, m_pController);
    m_pBusy = new Busy(this, m_pController);
    m_pScheduling = new Scheduling(this, m_pController);
    m_pStateMachine->addState(m_pInit);
    m_pStateMachine->addState(m_pIdle);
    m_pStateMachine->addState(m_pBusy);
    m_pStateMachine->addState(m_pScheduling);
    m_pStateMachine->setInitialState(m_pInit);

    m_pInit->addTransition(new EventTransition(TPTransition_t::Done, m_pInit, m_pIdle));
    m_pInit->addTransition(new EventTransition(TPTransition_t::Self, m_pInit, nullptr));

    m_pIdle->addTransition(new EventTransition(TPTransition_t::Load, m_pIdle, m_pScheduling));
    m_pIdle->addTransition(new EventTransition(TPTransition_t::Self, m_pIdle, nullptr));
    m_pIdle->addTransition(new EventTransition(TPTransition_t::Start, m_pIdle, m_pBusy));

    m_pScheduling->addTransition(new EventTransition(TPTransition_t::Done, m_pScheduling, m_pIdle));
    m_pScheduling->addTransition(new EventTransition(TPTransition_t::Self, m_pScheduling, nullptr));

    m_pBusy->addTransition(new EventTransition(TPTransition_t::Done, m_pBusy, m_pIdle));
    m_pBusy->addTransition(new EventTransition(TPTransition_t::Self, m_pBusy, nullptr));
}

bool InstrumentManager::HandleEvent(QEvent* event)
{
    auto tpsmEvent = dynamic_cast<TPSMEvent*>(event);
    if(tpsmEvent != nullptr && m_pStateMachine->isRunning())
    {
        qDebug() << tpsmEvent->toString();

        m_pStateMachine->postEvent(event);
    }
    return true;
}

}



