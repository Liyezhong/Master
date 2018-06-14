#include "Scheduler/Include/InstrumentManager.h"
#include "Scheduler/Include/TPExecutor.h"
#include "Scheduler/Include/TPEvent.h"
#include "Scheduler/Include/TPTransition.h"
#include "Scheduler/Include/SessionManager.h"
#include "Scheduler/Include/SchedulerStateMachineStates.h"

namespace Scheduler{

using namespace Instrument;

InstrumentManager* InstrumentManager::m_pInstance = nullptr;
InstrumentManager::InstrumentManager(const QString& name, EventDispatcher* pParent
                                     , const QList<QString>& config, SchedulerMainThreadController* pController, SessionManager* pSessionManager)
    : IEventHandler (name, pParent),
      m_pController(pController),
      m_pSessionManager(pSessionManager),
      m_pInit(nullptr),
      m_pIdle(nullptr),
      m_pScheduling(nullptr),
      m_pConfig(config)
{

}

InstrumentManager *InstrumentManager::Create(EventDispatcher *pParent, const QList<QString> &config, SchedulerMainThreadController *pController, SessionManager *pSessionManager)
{
    if(m_pInstance != nullptr)
    {
        return m_pInstance;
    }

    auto nameItor = config.last();
    m_pInstance = new InstrumentManager(nameItor, pParent, config, pController, pSessionManager);

    return m_pInstance;
}

InstrumentManager::~InstrumentManager()
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
    if(m_pScheduling != nullptr)
    {
        delete m_pScheduling;
        m_pScheduling = nullptr;
    }
}

void InstrumentManager::Initialize()
{    
    m_pEventDispatcher->Register(QSharedPointer<IEventHandler>(this));
    CreateStateMachine();
    Start();

    auto begin = m_pConfig.begin();

    QList<QString> retorts;
    for( auto begin = m_pConfig.begin(); begin != m_pConfig.end() -1; begin ++)
    {
        retorts<<*begin;
    }
    foreach(auto retort, retorts)
    {

        if (!retort.isEmpty())
        {
            if (m_TPExecutorList.find(retort) == m_TPExecutorList.end())
            {
                qDebug()<<"**** created exector:"<<retort;
                auto executor = QSharedPointer<TPExecutor>(new TPExecutor(retort, m_pEventDispatcher, m_pController));
                executor->CreateStateMachine();
                executor->Start();
                m_TPExecutorList.insert(retort, executor);
                m_pEventDispatcher->Register(executor);
            }
        }
    }
}

TPExecutor *InstrumentManager::GetTPExecutor(const QString &Id)
{
    return m_TPExecutorList.find(Id).value().data();
}

void InstrumentManager::CreateStateMachine()
{
    m_pStateMachine =  (new QStateMachine(QState::ExclusiveStates));

    m_pInit = new Instrument::Init(this, m_pController);
    m_pIdle = new Instrument::Idle(this, m_pController);
    m_pBusy = new Instrument::Busy(this, m_pController);
    m_pScheduling = new Instrument::Scheduling(this, m_pController);
    m_pStateMachine->addState(m_pInit);
    m_pStateMachine->addState(m_pIdle);
    m_pStateMachine->addState(m_pBusy);
    m_pStateMachine->addState(m_pScheduling);
    m_pStateMachine->setInitialState(m_pInit);

    m_pInit->addTransition(new EventTransition(TPTransition_t::Done, m_pInit, m_pIdle));
    m_pInit->addTransition(new EventTransition(TPTransition_t::Self, m_pInit, nullptr));
//    m_pInit->addTransition(new EventTransition(TPTransition_t::Timeout, m_pInit, nullptr));

    m_pIdle->addTransition(new EventTransition(TPTransition_t::Load, m_pIdle, m_pScheduling));
    m_pIdle->addTransition(new EventTransition(TPTransition_t::Self, m_pIdle, nullptr));
    m_pIdle->addTransition(new EventTransition(TPTransition_t::Start, m_pIdle, m_pBusy));

    m_pScheduling->addTransition(new EventTransition(TPTransition_t::Done, m_pScheduling, m_pIdle));
    m_pScheduling->addTransition(new EventTransition(TPTransition_t::Self, m_pScheduling, nullptr));

    m_pBusy->addTransition(new EventTransition(TPTransition_t::Done, m_pBusy, m_pIdle));
    m_pBusy->addTransition(new EventTransition(TPTransition_t::Load, m_pBusy, m_pScheduling));
    m_pBusy->addTransition(new EventTransition(TPTransition_t::Self, m_pBusy, nullptr));
}

int InstrumentManager::CreateSession(const QString& retortId, const QString& protocolId)
{
    return m_pSessionManager->CreateSession(retortId, protocolId);
}

QList<Session*>& InstrumentManager::GetInitialSession()
{
    m_SessionList.clear();
    foreach(auto name, m_TPExecutorList.keys())
    {
        auto ss = m_pSessionManager->GetSessionByRetortId(name, Session::Initial);
        if(ss != nullptr)
        {
            m_SessionList << ss;
        }
    }

    return m_SessionList;
}

QList<Session*>& InstrumentManager::GetReadySession()
{
    m_SessionList.clear();
    foreach(auto name, m_TPExecutorList.keys())
    {
        auto ss = m_pSessionManager->GetSessionByRetortId(name, Session::Ready);
        if(ss != nullptr)
        {
            m_SessionList << ss;
        }
    }

    return m_SessionList;
}

void InstrumentManager::StartProtocol(const QString& retortId)
{
    m_pEventDispatcher->IncomingEvent(new TPEvent(TPTransition_t::Self, new TPEventArgs<ControlCommandType_t>(retortId, ControlCommandType_t::CTRL_CMD_START)));
    return;
}


}



