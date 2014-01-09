#ifndef RCREPORT_H
#define RCREPORT_H

#include "ErrorHandlingSMBase.h"
#include <QStateMachine>

namespace Scheduler{

class  CRCReport : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    CRCReport (QStateMachine* pStateMachine, QState* pParentState);
    ~CRCReport();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

signals:
    void RCReport();
    void OnRCReport();

private:
    QState *mp_RCReport;
};
}


#endif // RCREPORT_H
