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

    int CreateSession(const QString &retortId, const QString& protocoId);
    void DestroySession(int sessionId);
    Session* GetSessionById(int sessionId);
    Session* GetSessionByRetortId(const QString& retrotId);
private:

    QHash<int, QSharedPointer<Session>> m_SessionList;
    DataManager::CDataManager* m_pDataManager;
    static int SessionNo;
    Q_DISABLE_COPY(SessionManager)
};
}
#endif // SESSIONMANAGER_H
