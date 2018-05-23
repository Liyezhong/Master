#ifndef STATEBASE_H
#define STATEBASE_H

#include <QState>
#include "Scheduler/Include/TPEvent.h"
#include "Scheduler/Include/IEventHandler.h"
#include "Scheduler/Include/TPTransition.h"

namespace Scheduler
{
class SchedulerMainThreadController;

class IState: public QState
{
public:
    IState(IEventHandler* pHandler, Scheduler::SchedulerMainThreadController* controller)
        : m_pHandler(pHandler),
          m_pController(controller),
          m_Transition(TPTransition_t::Unknown)
    {
    }
    virtual bool HandleEvent(TPEvent* e){ return false;}

protected:
    IEventHandler* m_pHandler;
    Scheduler::SchedulerMainThreadController* m_pController;
    TPTransition_t m_Transition;
};

template <class D>
class StateBase : public IState
{
public:
    StateBase(IEventHandler* pHandler, Scheduler::SchedulerMainThreadController* controller);
    D* Data(){return data;}

protected:
    void onEntry(QEvent* event) override;
    void onExit(QEvent* event) override;
    bool HandleEvent(TPEvent* event) override;

    virtual bool HandleEvent(TPEventArgs<D>* event, TPTransition_t& pTransition) = 0;
    virtual void RepeatAction(TPTransition_t& pTransition);
protected:
    D* data;
    Scheduler::SchedulerMainThreadController* m_pController;
    IEventHandler* m_pHandler;
    TPTransition_t m_Transition;
};

}



#endif // STATEBASE_H
