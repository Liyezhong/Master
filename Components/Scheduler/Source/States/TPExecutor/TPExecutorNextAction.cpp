#include "Scheduler/Include/States/TPExecutor/TPExecutorNextAction.h"


namespace Scheduler{
namespace TPExecutorStates{

NextAction::NextAction(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller)
{
    setObjectName("TPExecutor_NextAction_State");
    actionNum = 3;
}

NextAction::~NextAction()
{

}

void NextAction::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
    qDebug() << "Action count: " << actionNum;

}

void NextAction::RepeatAction(TPTransition_t &pTransition)
{
    if(actionNum > 0)
    {
        actionNum--;
        pTransition = TPTransition_t::Execute;
    }
    else
    {
        pTransition = TPTransition_t::Done;
    }
}

bool NextAction::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{

}

}
}
