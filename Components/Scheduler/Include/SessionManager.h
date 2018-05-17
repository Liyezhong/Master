#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include "HimalayaDataManager/Include/DataManager.h"


namespace Scheduler{
class Session;
class SessionManager
{
public:
    SessionManager(DataManager::CDataManager* pDataManager);

    int CreateSession(quint32 protocoId, const QString& retortId);
    void DestroySession(int sessionId);
    Session* GetSessionById(int sessionId);
private:
    QHash<int, QSharedPointer<Session>> m_SessionList;
    DataManager::CDataManager* m_pDataManager;
    static int SeqNo;
    Q_DISABLE_COPY(SessionManager)
};
}
#endif // SESSIONMANAGER_H
