#ifndef BUSY_H
#define BUSY_H
#include "Scheduler/Include/States/Instrument/Idle.h"


namespace Scheduler{
namespace Instrument{

class Busy: public Idle
{
public:
    Busy(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Busy();

protected:
    void onEntry(QEvent* event) override;

};
}
}

#endif // BUSY_H
