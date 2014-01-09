#include "../Include/RCReport.h"

namespace Scheduler{

CRCReport::CRCReport(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_RCReport = new QState(pParentState);
        QState *pInitState = (QState*)pParentState->initialState();
        pInitState->addTransition(this, SIGNAL(RCReport()), mp_RCReport);
        connect(mp_RCReport, SIGNAL(entered()), this, SIGNAL(OnRCReport()));
    }
}

CRCReport::~CRCReport()
{
    delete mp_RCReport;
    mp_RCReport = NULL;
}

SchedulerStateMachine_t CRCReport::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_RCReport))
    {
        currentState = SM_ERR_RC_REPORT;
    }

    return currentState;
}
}
