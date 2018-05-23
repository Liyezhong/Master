#include "Scheduler/Include/SessionManager.h"
#include "Scheduler/Include/Session.h"
#include <QSharedPointer>

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
    auto* pProgram = m_pDataManager->GetProgramList()->GetProgram(protocoId);
    QList<QSharedPointer<const CProgramStep>> programSteps;

    auto session = new Session(retortId, const_cast<CProgram*>(pProgram));
    m_SessionList.insert(SessionNo++,QSharedPointer<Session>(session));

    return SessionNo;
}

void SessionManager::DestroySession(int sessionId)
{
    m_SessionList.remove(sessionId);
}

Session *SessionManager::GetSessionById(int sessionId)
{
    return m_SessionList.find(sessionId).value().data();
}


}
