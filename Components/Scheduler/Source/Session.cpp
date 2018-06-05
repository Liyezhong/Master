#include "Scheduler/Include/Session.h"


namespace Scheduler{

Session::Session(const QString& retortId, CProgram* pProgram)
    : m_RetortId(retortId),
      m_pProgram(pProgram),
      m_pProgramStepInfo(new ProgramStepInfor{}),
      m_pSchedule(nullptr)
{
    m_ActionList.clear();
    m_Status = SessionStatus_t::Initial;
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

}
