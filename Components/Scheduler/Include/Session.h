#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QSharedPointer>

#include "HimalayaDataContainer/Containers/Programs/Include/ProgramStep.h"
#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"
#include "IAction.h"

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


class Session
{
public:
    Session(const QString& retortId, CProgram* pProgram);
    ~Session();

    const QString& GetRetortID() const {return m_RetortId;}
    CProgram* GetProgram() {return m_pProgram;}

    QList<QSharedPointer<const CProgramStep>> GetProgramStep() {return m_pProgramSteps;}

    ProgramStepInfor* GetProgramStepInfo(){return m_pProgramStepInfo;}

    QList<QSharedPointer<IAction>>& GetActionList(){return m_ActionList;}

    Q_DISABLE_COPY(Session)
private:
    QString m_RetortId;
    CProgram* m_pProgram;
    QList<QSharedPointer<const CProgramStep>> m_pProgramSteps;
    QList<QSharedPointer<IAction>> m_ActionList;
    ProgramStepInfor* m_pProgramStepInfo;
};
}
#endif // SESSION_H
