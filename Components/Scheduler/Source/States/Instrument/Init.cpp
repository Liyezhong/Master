#include "Scheduler/Include/States/Instrument/Init.h"
#include <QtDebug>
#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler{
namespace Instrument{

Init::Init(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Scheduler::SchedulerCommandShPtr_t>(pHandler, controller),
     m_pSelfTest(nullptr)

{
    setObjectName("Instrument_Init_State");
//    m_pSelfTest = new CProgramSelfTest(controller);
//    m_pSelfTest->SetSender(pHandler->objectName());
}

Init::~Init()
{
    if(m_pSelfTest != nullptr)
    {
        delete m_pSelfTest;
        m_pSelfTest = nullptr;
    }
}


void Init::onEntry(QEvent *event)
{
    StateBase<Scheduler::SchedulerCommandShPtr_t>::onEntry(event);
    addTransition(new EventTransition(TPTransition_t::Timeout, this));
//    IState::m_pController->GetHeatingStrategy()->SetSender(IState::m_pHandler->objectName());

//    m_pSelfTest->HandleWorkFlow("", DCL_ERR_FCT_CALL_FAILED);
}

bool Init::HandleEvent(TPEventArgs<Scheduler::SchedulerCommandShPtr_t> *event, TPTransition_t& pTransition)
{
    return false;
}

void Init::RepeatAction(TPTransition_t& pTransition)
{
    StateBase::RepeatAction(pTransition);

    pTransition = TPTransition_t::Done;
}


void Init::onExit(QEvent *event)
{
    IState::onExit(event);

}
}
}
