#ifndef IACTION_H
#define IACTION_H

#include <QString>
#include <QTime>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
//#include "DeviceControl/Include/Interface/IDeviceProcessing.h"

namespace Scheduler {

class SchedulerCommandProcessorBase;

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
    ACTION_SUM //must be last one
}ActionType_t;


typedef enum
{
    STATE_UNDEFIN = 0,
    STATE_FILLING_RVROD_HEATING,
    STATE_FILLING_LEVELSENSOR_HEATING,
    STATE_FILLING,
    STATE_RV_MOVE_TO_SEAL
}ActionState_t;


class IAction
{
public:
    IAction(SchedulerCommandProcessorBase* commandProcessor, const QString RetortID, ActionType_t type);
    virtual ~IAction();

    virtual void Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

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

private:
    void Fill(const QString& cmdName, DeviceControl::ReturnCode_t retCode);
    void Drain();
    void Purge();

    DeviceControl::RVPosition_t GetRVPosition(const QString& stationID, bool isTube);
protected:
    QString m_retortID;
    QString m_reagentID;
    QString m_stationID;
    ActionType_t m_type;
    bool m_stateWaitResult;
    bool m_finished;
    quint32 m_durationSec;
    QTime m_startTime;
    QTime m_endTime;
    ActionState_t m_currentState;
    SchedulerCommandProcessorBase* mp_SchedulerCommandProcessor;

    //mutable DeviceControl::ReturnCode_t m_result;   ///<  Definition/Declaration of variable m_result
};

}
#endif // IACTION_H
