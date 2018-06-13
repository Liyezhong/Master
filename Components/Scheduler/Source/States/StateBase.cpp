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
bool StateBase<D>::HandleEvent(TPEvent *event)
{
    auto wrappedEvent = dynamic_cast<TPEvent*>(event);
    if(wrappedEvent != nullptr)
    {
        auto eventArgs = dynamic_cast<TPEventArgs<D>*>(wrappedEvent->EventArgs());

        if(eventArgs != nullptr)
        {
            qDebug() << objectName() << "HandleEvent";

            HandleEvent(eventArgs, m_Transition);

            if(m_Transition != TPTransition_t::Unknown)
            {
                machine()->postEvent(new TPEvent(m_Transition, nullptr));
            }
            return eventArgs->Handled();
        }

        if(wrappedEvent->Value() == TPTransition_t::Timeout)
        {
//            qDebug() << objectName() << "RepeatAction";
            RepeatAction(m_Transition);
            if(m_Transition != TPTransition_t::Unknown)
            {
                machine()->postEvent(new TPEvent(m_Transition, nullptr));
            }
            return true;
        }
    }


    return true;
}

template <class D>
void StateBase<D>::RepeatAction(TPTransition_t& pTransition)
{
    qDebug(StatesDebug) << "RepeatAction in state " << objectName() << "==> transition: " << m_Transition;
    pTransition = TPTransition_t::Unknown;
}

template <class D>
void StateBase<D>::onEntry(QEvent *event)
{
//    setObjectName(typeid(this).name());
    auto e = dynamic_cast<TPEvent*>(event);

    if(e != nullptr)
    {
        qDebug() << "Enter state " << objectName() << "==> transition: " << e->Value();
    }
}

template <class D>
void StateBase<D>::onExit(QEvent *event)
{
    qDebug() << "Exit state " << objectName() << "==> transition: " << m_Transition;
}



template class StateBase<Scheduler::SchedulerCommandShPtr_t>;
template class StateBase<Global::CommandShPtr_t>;
template class StateBase<ControlCommandType_t>;

