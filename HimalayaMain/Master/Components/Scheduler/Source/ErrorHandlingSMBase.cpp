#include "../Include/ErrorHandlingSMBase.h"

namespace Scheduler{


CErrorHandlingSMBase::CErrorHandlingSMBase(QStateMachine* pStateMachine, QState* pParentState)
{
    if(pStateMachine)
    {
        mp_StateMachine = pStateMachine;
    }
    if(pParentState)
    {
        mp_ParentState = pParentState;
    }

}

CErrorHandlingSMBase::~CErrorHandlingSMBase()
{
    mp_StateMachine = NULL;
    mp_ParentState = NULL;
}

SchedulerStateMachine_t CErrorHandlingSMBase::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_ParentState))
    {
        currentState = SM_ERROR;
    }

    return currentState;
}


}
