#include "Scheduler/Include/Session.h"
#include "Global/Include/SystemPaths.h"
#include <unistd.h>


namespace Scheduler{

Session::Session(const QString& retortId, CProgram* pProgram)
    : m_RetortId(retortId),
      m_pProgram(pProgram),
      m_pProgramStepInfo(new ProgramStepInfor{}),
      m_pSchedule(nullptr)
{
    m_pProgram = pProgram;
    m_pProgramStepInfo = new ProgramStepInfor{};

    m_ActionList.clear();
    m_Status = SessionStatus_t::Initial;
    m_SessionID = QDateTime::currentMSecsSinceEpoch();
}

Session::~Session()
{
    if(m_pProgramStepInfo != nullptr)
    {
        delete m_pProgramStepInfo;
        m_pProgramStepInfo = nullptr;
    }

    if(m_pSchedule != nullptr)
    {
        delete m_pSchedule;
        m_pSchedule = nullptr;
    }
}

QString Session::toString()
{
    QString output;
    output.append(QString("Program ID:%1\n").arg(m_pProgram->GetID()));
    output.append(QString("Session status:%1\n").arg(m_Status));
    output.append("*********** Action List **************\n");
    foreach(auto action, m_ActionList)
    {
        output.append(action.data()->toString());
        output.append("\n");
    }
    output.append("*********** End **************");

    return output;
}

void Session::Log2File()
{
    QString Path = Global::SystemPaths::Instance().GetLogfilesPath();
    QDir Dir(Path);
    QString FileName = "Session_" + QString::number(m_SessionID)+ ".txt";
    QString CompleteFile(QDir::cleanPath(Dir.absoluteFilePath(FileName)));

    //if(QFile::exists(CompleteFile))
    QFile logFile(CompleteFile);
    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        //Log error
        return;
    }

    logFile.write(toString().toUtf8());
    logFile.flush() || (fsync(logFile.handle()) != -1);
    logFile.close();
}

void Session::RemoveFile()
{
    QString Path = Global::SystemPaths::Instance().GetLogfilesPath();
    QDir Dir(Path);
    QString FileName = "Session_" + QString::number(m_SessionID)+ ".txt";
    QString CompleteFile(QDir::cleanPath(Dir.absoluteFilePath(FileName)));

    if (QFile::exists(CompleteFile))
    {
        QFile::remove(CompleteFile);
    }
}

}
