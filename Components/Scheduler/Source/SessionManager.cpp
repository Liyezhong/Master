#include "Scheduler/Include/SessionManager.h"
#include "Scheduler/Include/Session.h"


using namespace DataManager;
namespace Scheduler{

int SessionManager::SessionNo = -1;
SessionManager::SessionManager(CDataManager* pDataManager)
    :m_pDataManager(pDataManager)
{
    m_SessionList.clear();
}

int SessionManager::CreateSession(const QString& protocoId, const QString &retortId)
{
    auto pProgram = m_pDataManager->GetProgramList()->GetProgram(protocoId);
    if(pProgram == nullptr)
        return -1;

    QList<QSharedPointer<const CProgramStep>> programSteps;

    for (int i = 0; i < pProgram->GetNumberOfSteps(); i++)
    {
        auto pProgramStep = pProgram->GetProgramStep(i);
        programSteps << QSharedPointer<const CProgramStep>(pProgramStep);
    }

    auto session = new Session(retortId, pProgram);
    m_SessionList.insert(SessionNo++, QSharedPointer<Session>(session));

    return SessionNo;
}

void SessionManager::DestroySession(int sessionId)
{
    m_SessionList.remove(sessionId);
}

Session const* SessionManager::GetSessionById(int sessionId)
{
    return m_SessionList.find(sessionId).value().data();
}


}
