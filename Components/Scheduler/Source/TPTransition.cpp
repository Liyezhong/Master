#include "Scheduler/Include/TPTransition.h"
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{
EventTransition::EventTransition(const TPTransition_t& transition, IState* pHandler, IState* pTarget)
    : m_value(transition),
      m_pSource(pHandler),
      m_pTarget(pTarget)
{
    m_value = transition;
    if(m_pTarget != nullptr)
    {
        setTargetState(m_pTarget);
    }
}


bool EventTransition::eventTest(QEvent *e)
{
    if (e == nullptr
            || e->type() != (QEvent::Type)(QEvent::User + 1))
        return false;
    auto *se = static_cast<TPSMEvent*>(e);
    return (m_value == se->value);
}

void EventTransition::onTransition(QEvent* e)
{
    if(m_pSource != nullptr && m_pTarget == nullptr)
    {

        m_pSource->HandleEvent(e);
    }
}

}
