#include "Scheduler/Include/Session.h"


namespace Scheduler{

Session::Session(const QString& retortId, CProgram* pProgram)
    : m_RetortId(retortId), m_currentActionIndex(-1)
{
    m_pProgram = pProgram;
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
