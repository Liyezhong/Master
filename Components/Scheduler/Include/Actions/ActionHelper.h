#ifndef ACTIONHELPER_H
#define ACTIONHELPER_H

#include <QString>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler {

class SchedulerCommandProcessorBase;

class ActionHelper
{

public:
    ActionHelper(SchedulerCommandProcessorBase* commandProcessor);
    virtual ~ActionHelper();

    DeviceControl::RVPosition_t GetRVPosition(const QString& stationID, bool isTube);
    bool IsRVRightPosition(DeviceControl::RVPosition_t position, const QString& retortID);

protected:
    SchedulerCommandProcessorBase* mp_SchedulerCommandProcessor;
};
}

#endif // ACTIONHELPER_H
