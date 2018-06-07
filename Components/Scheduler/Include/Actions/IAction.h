#ifndef IACTION_H
#define IACTION_H

#include <QString>
#include <QTime>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler {

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
    SOAKING,
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

    void SetStartTime(QTime startTime) {m_startTime = startTime;}
    QTime GetStartTime() const {return m_startTime;}

    void SetEndTime(QTime endTime) {m_endTime = endTime;}
    QTime GetEndTime() const {return m_endTime;}

    void SetFininshed(bool finished) {m_finished = finished;}
    bool IsFinished() const {return m_finished;}

    //const QString GetName() const = 0;

protected:
    QString m_reagentID;
    QString m_stationID;
    bool m_finished;
    quint32 m_durationSec;
    QTime m_startTime;
    QTime m_endTime;
    Session* mp_session;
};

}
#endif // IACTION_H
