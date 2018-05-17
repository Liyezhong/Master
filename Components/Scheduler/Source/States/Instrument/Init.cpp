#include "Scheduler/Include/States/Instrument/Init.h"
#include <QtDebug>
#include "Scheduler/Include/SchedulerMainThreadController.h"

using namespace Scheduler;
Init::Init(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Scheduler::SchedulerCommandShPtr_t>(pHandler, controller),
     m_pSelfTest(nullptr)

{
    setObjectName("Instrument_Init_State");
    m_pSelfTest = new CProgramSelfTest(controller);
    m_pSelfTest->SetSender(pHandler->Name());

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
    IState::m_pController->GetHeatingStrategy()->SetSender(IState::m_pHandler->Name());

    m_pSelfTest->HandleWorkFlow("", DCL_ERR_FCT_CALL_FAILED);
}

bool Init::HandleEvent(TPCmdEvent<Scheduler::SchedulerCommandShPtr_t> *event, TPTransition_t& pTransition)
{
    qDebug() << "Entering Init state HandleEvent";

    auto data = event->Data();
    ReturnCode_t retCode;
    event->Data()->GetResult(retCode);

    m_pSelfTest->HandleWorkFlow(event->Data()->GetName(), retCode);

    pTransition = TPTransition_t::Unknown;

    event->SetHandled();

    return true;
}

void Init::RepeatAction(TPTransition_t& pTransition)
{
    StateBase::RepeatAction(pTransition);
    qDebug() << "RepeatAction init";

    if(IState::m_pController->IsSelfTestDone())
    {
        pTransition = TPTransition_t::Done;

        qDebug() << "Transition: TPTransition_t::Done";
    }
    m_pSelfTest->HandleWorkFlow("", DCL_ERR_FCT_CALL_FAILED);
}


void Init::onExit(QEvent *event)
{
    IState::onEntry(event);
    qDebug() << "Exitting Init state";
}
