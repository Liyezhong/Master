#ifndef RSRVGETORIGINALPOSITIONAGAIN_H
#define RSRVGETORIGINALPOSITIONAGAIN_H
#include "ErrorHandlingSMBase.h"
#include <QStateMachine>

namespace Scheduler{

class  CRsRvGetOriginalPositionAgain : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    CRsRvGetOriginalPositionAgain (QStateMachine* pStateMachine, QState* pParentState);
    ~CRsRvGetOriginalPositionAgain();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

signals:
    void RvMoveToInitPosition();
    void RvMoveToInitPositionFinished();
    void OnRvMoveToInitPosition();

private:
    QState *mp_RvMovingToInitialPosition;
};
}
#endif // RSRVGETORIGINALPOSITIONAGAIN_H

