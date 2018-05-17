#include "SessionManager.h"
#include "Session.h"
#include <QSharedPointer>

using namespace DataManager;
namespace Scheduler{

int SessionManager::SeqNo = -1;
SessionManager::SessionManager(CDataManager* pDataManager)
    :m_pDataManager(pDataManager)
{
    m_SessionList.clear();
}

int SessionManager::CreateSession(quint32 protocoId, const QString &retortId)
{
    auto pProgram = m_pDataManager->GetProgramList()->GetProgram(protocoId);
    QList<QSharedPointer<const CProgramStep>> programSteps;

    for (int i = 0; i < pProgram->GetNumberOfSteps(); i++)
    {
        auto pProgramStep = pProgram->GetProgramStep(i);
        programSteps << QSharedPointer<const CProgramStep>(pProgramStep);
    }

    auto session = new Session(retortId, QSharedPointer<CProgram>(pProgram), programSteps);
    m_SessionList.insert(SeqNo++, QSharedPointer<Session>(session));

    return SeqNo;
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
