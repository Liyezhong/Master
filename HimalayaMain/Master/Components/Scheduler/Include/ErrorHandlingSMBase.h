#ifndef ERRORHANDLINGSMBASE_H
#define ERRORHANDLINGSMBASE_H
#include <QStateMachine>
#include "SchedulerStateMachineStates.h"

namespace Scheduler{


class CErrorHandlingSMBase : public QObject
{
    Q_OBJECT
public:
    CErrorHandlingSMBase(QStateMachine* pStateMachine, QState* pParentState);
    ~CErrorHandlingSMBase();
    virtual SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);
protected:
    QStateMachine* mp_StateMachine;
    QState* mp_ParentState;
    QList<QState*> mp_StateList;

};
}
#endif // ERRORHANDLINGSMBASE_H
