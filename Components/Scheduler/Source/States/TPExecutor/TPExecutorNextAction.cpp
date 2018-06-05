#include "Scheduler/Include/States/TPExecutor/TPExecutorNextAction.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Include/TPExecutor.h"

namespace Scheduler{
namespace TPExecutorStates{

NextAction::NextAction(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller),
      m_pSession(nullptr)
{
    setObjectName("TPExecutor_NextAction_State");
    actionNum = -1;
}

NextAction::~NextAction()
{

}

void NextAction::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
    m_pExecutor = dynamic_cast<Scheduler::TPExecutor*>(IState::m_pHandler);
    if(m_pExecutor != nullptr && actionNum < 0)
    {
        m_pSession = m_pExecutor->GetCurrentSession();
        if(m_pSession == nullptr)
        {
            return;
        }

        qDebug() << "Session " << m_pSession->GetRetortID()
                 << " Program " << m_pSession->GetProgram()->GetName();

        auto startTime = QDateTime::currentDateTime();
        foreach(auto action, m_pSession->GetActionList())
        {
            action->SetStartTime(startTime);
            startTime.addSecs(action->GetDuration());
        }

        actionNum = m_pSession->GetActionList().count();
    }

}

void NextAction::RepeatAction(TPTransition_t &pTransition)
{
    if(actionNum > 0)
    {
        m_pExecutor->SetCurrentAction(m_pSession->GetActionList().count()- actionNum);
        pTransition = TPTransition_t::Execute;
        actionNum--;
    }
    else if(m_pSession != nullptr)
    {
        pTransition = TPTransition_t::Done;
    }
}

bool NextAction::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{

}

}
}
