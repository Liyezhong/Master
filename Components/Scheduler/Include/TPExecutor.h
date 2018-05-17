#ifndef TPEXECUTOR_H
#define TPEXECUTOR_H

#include <QObject>
#include "Scheduler/Include/IEventHandler.h"

namespace Scheduler{
class InstrumentManager;

class TPExecutor: public IEventHandler
{
public:
    TPExecutor(const QString& name, EventDispatcher* pEventDispatcher);

    bool HandleEvent(QEvent* event) override;

    void CreateStateMachine() override;
    Q_DISABLE_COPY(TPExecutor)
};
}

#endif // TPEXECUTOR_H
