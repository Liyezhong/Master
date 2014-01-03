#include "../Include/RsRvGetOriginalPositionAgain.h"

namespace Scheduler{

CRsRvGetOriginalPositionAgain::CRsRvGetOriginalPositionAgain(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_RvMovingToInitialPosition = new QState(pParentState);
        QState *pInitState = (QState*)pParentState->initialState();
        pInitState->addTransition(this, SIGNAL(RvMoveToInitPosition()), mp_RvMovingToInitialPosition);
        mp_RvMovingToInitialPosition->addTransition(this, SIGNAL(RvMoveToInitPositionFinished()), pInitState);
        connect(mp_RvMovingToInitialPosition, SIGNAL(entered()), this, SIGNAL(OnRvMoveToInitPosition()));
    }
}

CRsRvGetOriginalPositionAgain::~CRsRvGetOriginalPositionAgain()
{
    delete mp_RvMovingToInitialPosition;
    mp_RvMovingToInitialPosition = NULL;
}

SchedulerStateMachine_t CRsRvGetOriginalPositionAgain::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_RvMovingToInitialPosition))
    {
        currentState = SM_ERR_RS_RV_MOVING_TO_INIT_POS;
    }

    return currentState;
}
}
