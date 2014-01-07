#ifndef SCENARIO_H
#define SCENARIO_H

#include <QObject>
#include "SchedulerStateMachineStates.h"

namespace Scheduler
{
class Scenario : public QObject
{
    Q_OBJECT
public:
    explicit Scenario(QObject *parent = 0);
    
signals:
    
public slots:
   qint32 GetScenarioBySchedulerState(SchedulerStateMachine_t State, QString ReagentGroup);
};
}
#endif // SCENARIO_H
