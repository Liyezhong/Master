#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QSharedPointer>

#include "HimalayaDataContainer/Containers/Programs/Include/ProgramStep.h"
#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"
#include "Scheduler/Include/Actions/IAction.h"
#include <QTime>

using namespace DataManager;

namespace Scheduler{

typedef struct {
    QString stationID;       ///<  Definition/Declaration of variable stationID
    QString nextStationID;   ///< Definition/Declaration of variable stationID
    qint64 durationInSeconds;       ///<  Definition/Declaration of variable durationInSeconds
    int temperature;       ///<  Definition/Declaration of variable temperature
    bool isPressure;       ///<  Definition/Declaration of variable isPressure
    bool isVacuum;       ///<  Definition/Declaration of variable isVacuum
    QString reagentGroup;       ///<  Definition/Declaration of variable reagentGroup
} ProgramStepInfor;


typedef struct Schedule
{
public:
    Schedule(QDateTime startTime,  int actionNum)
    {
        StartTime = startTime;
        ActionNum = actionNum;
    }

    int GetNextAction() {return ActionIndex++; }

private:
    int ActionIndex;
    int ActionNum;
    QDateTime StartTime;
}Schedule_t;

class Session
{
public:
    Session(const QString& retortId, CProgram* pProgram);
    ~Session();

    const QString& GetRetortID() {return m_RetortId;}
    CProgram* GetProgram() {return m_pProgram;}

    QList<QSharedPointer<const CProgramStep>> GetProgramStep() {return m_pProgramSteps;}

    ProgramStepInfor* GetProgramStepInfo(){return m_pProgramStepInfo;}

    QList<QSharedPointer<IAction>>& GetActionList(){return m_ActionList;}
    
    Schedule* GetSchedule() const {return m_pSchedule;}

    void SetSchedule(Schedule* pSchedule) {m_pSchedule = pSchedule;}

    typedef enum Status
    {
        Initial,
        Ready,
        Running,
        Completed,
        Error
    }SessionStatus_t;
        
    void SetStatus(const SessionStatus_t& status){m_Status = status;}
    SessionStatus_t GetStatus() const { return m_Status;}

    int GetCurrentActionIndex() const {return m_currentActionIndex;}
    void UpdateCurrentActionIndex(int index) {m_currentActionIndex = index;}

    int GetProposedTime()
    {
        int duration = -1;
        if(m_ActionList.isEmpty())
            return duration;

        foreach(auto action, m_ActionList)
        {
            duration += action->GetDuration();
        }

        return duration;
    }
	
		QTime GetProgramStartTime() {return programStartTime;}
	
    QTime GetProgramEndTime()
    {
        return m_ActionList[m_ActionList.size() - 1].data()->GetEndTime();
    }

    void SetProgramStartTime(QTime t)
    {
        programStartTime = t;
    }

    qint64 SessionID() const {return m_SessionID;}

    QString toString();

    void Log2File();
    void RemoveFile();

    Q_DISABLE_COPY(Session)
private:
    int m_currentActionIndex;
    QString m_RetortId;
    CProgram* m_pProgram;
    QList<QSharedPointer<const CProgramStep>> m_pProgramSteps;
    QList<QSharedPointer<IAction>> m_ActionList;
public:
    QTime programStartTime;
    ProgramStepInfor* m_pProgramStepInfo;
    SessionStatus_t m_Status;
    Schedule* m_pSchedule;
    qint64 m_SessionID;
};
}
#endif // SESSION_H
