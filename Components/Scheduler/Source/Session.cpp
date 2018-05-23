#include "Scheduler/Include/Session.h"


namespace Scheduler{

Session::Session(const QString& retortId, const CProgram* pProgram)
    : m_RetortId(retortId),
      m_pProgram(pProgram)
{
//    m_pProgramSteps = programSteps;
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
