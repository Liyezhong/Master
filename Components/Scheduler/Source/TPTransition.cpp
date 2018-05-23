#include "Scheduler/Include/TPTransition.h"
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{
EventTransition::EventTransition(const TPTransition_t& transition, QState* pHandler, QState* pTarget)
    : m_pSource(pHandler),
      m_pTarget(pTarget),
      m_value(transition)

{
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
    auto *se = dynamic_cast<TPEvent*>(e);
    return (m_value == se->Value());
}

void EventTransition::onTransition(QEvent* e)
{
    if(m_pSource != nullptr)
    {
        auto *se = dynamic_cast<TPEvent*>(e);
        auto* pState = dynamic_cast<IState*>(m_pSource);

        if(pState != nullptr)
            pState->HandleEvent(se);
    }
}

}
