#ifndef IACTION_H
#define IACTION_H

#include <QString>
#include <QTime>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
//#include "DeviceControl/Include/Interface/IDeviceProcessing.h"

namespace Scheduler {

class SchedulerCommandProcessorBase;
class Session;

/****************************************************************************/
/*!
 *  \brief enum for action type
 */
/****************************************************************************/
typedef enum
{
    UNKNOW_ACTION = 0,
    FILLING,
    DRAINING,
    PURGE,
    SOAKING
}ActionType_t;


class IAction
{
public:
    IAction(Session* session);
    virtual ~IAction();

    virtual void Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode) = 0;

    void SetReagentID(const QString& reagentID) {m_reagentID = reagentID;}
    QString GetReagentID() const {return m_reagentID;}

    void SetStationID(const QString& stationID) {m_stationID = stationID;}
    QString GetStationID() const {return m_stationID;}

    void SetDuration(quint32 duration) {m_durationSec = duration;}
    quint32 GetDuration() const {return m_durationSec;}

    void SetStartTime(QDateTime startTime) {m_startTime = startTime;}
    QDateTime GetStartTime() const {return m_startTime;}

    void SetEndTime(QDateTime endTime) {m_endTime = endTime;}
    QDateTime GetEndTime() const {return m_endTime;}

    void SetFininshed(bool finished) {m_finished = finished;}
    bool IsFinished() const {return m_finished;}

    ActionType_t GetActionType() const {return m_type;}

    QString GetActionName()
    {
        switch(m_type)
        {
        case FILLING:
            return "FILLING";

        case DRAINING:
            return "DRAINING";

        case PURGE:
            return "PURGE";

        case SOAKING:
            return "SOAKING";
        default:
            return "UNKNOW_ACTION";
        }
    }

protected:
    QString m_reagentID;
    QString m_stationID;
    ActionType_t m_type;
    bool m_finished;
    quint32 m_durationSec;
    QDateTime m_startTime;
    QDateTime m_endTime;
    Session* mp_session;

    //mutable DeviceControl::ReturnCode_t m_result;   ///<  Definition/Declaration of variable m_result
};

}
#endif // IACTION_H
