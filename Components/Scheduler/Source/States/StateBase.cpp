#include "Scheduler/Include/States/StateBase.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"

using namespace Scheduler;
template <class D>
StateBase<D>::StateBase(IEventHandler* pHandler, Scheduler::SchedulerMainThreadController* controller)
    :IState(pHandler, controller),
     data(nullptr)

{

}


template <class D>
bool StateBase<D>::HandleEvent(QEvent *event)
{
    auto wrappedEvent = dynamic_cast<TPSMEvent*>(event);
    if(wrappedEvent != nullptr)
    {
        auto eventArgs = dynamic_cast<TPCmdEvent<D>*>(wrappedEvent->m_pData);

        if(eventArgs != nullptr)
        {

            HandleEvent(eventArgs, m_Transition);
            if(m_Transition != TPTransition_t::Unknown)
            {
                machine()->postEvent(new TPSMEvent(m_Transition, nullptr));
            }
            return true;
        }

        if(wrappedEvent->value == TPTransition_t::Self)
        {
            RepeatAction(m_Transition);
            if(m_Transition != TPTransition_t::Unknown)
            {
                machine()->postEvent(new TPSMEvent(m_Transition, nullptr));
            }
            return true;
        }
    }


    return true;
}

template <class D>
void StateBase<D>::RepeatAction(TPTransition_t& pTransition)
{
    qDebug() << "RepeatAction";
    pTransition = TPTransition_t::Unknown;
}

template <class D>
void StateBase<D>::onEntry(QEvent *event)
{
//    setObjectName(typeid(this).name());
    auto e = dynamic_cast<TPSMEvent*>(event);

    if(e != nullptr)
    {
        qDebug() << "Enter state " << objectName() << "==> transition: " << e->value;
    }
}

template <class D>
void StateBase<D>::onExit(QEvent *event)
{
    qDebug() << "Exit state " << objectName() << "==> transition: " << m_Transition;
}



template class StateBase<Scheduler::SchedulerCommandShPtr_t>;
template class StateBase<Global::CommandShPtr_t>;
