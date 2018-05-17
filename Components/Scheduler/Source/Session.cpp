#include "Session.h"


namespace Scheduler{

Session::Session(const QString& retortId, QSharedPointer<CProgram> pProgram, const QList<QSharedPointer<const CProgramStep>>& programSteps)
    : m_RetortId(retortId)
{
    m_pProgram = pProgram.data();
    m_pProgramSteps = programSteps;
    m_pProgramStepInfo = new ProgramStepInfor{};

    m_ActionList.clear();
}

Session::~Session()
{
    if(m_pProgramStepInfo != nullptr)
    {
        delete m_pProgramStepInfo;
        m_pProgramStepInfo = nullptr;
    }
}

}
