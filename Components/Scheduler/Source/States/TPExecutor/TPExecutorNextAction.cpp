#include "Scheduler/Include/States/TPExecutor/TPExecutorNextAction.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Include/TPExecutor.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "Scheduler/Include/SchedulerLogging.h"

namespace Scheduler{
namespace TPExecutorStates{

NextAction::NextAction(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller),
      m_pSession(nullptr)
{
    setObjectName("TPExecutor_NextAction_State");
    actionNum = -1;
}

NextAction::~NextAction()
{

}

void NextAction::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
    m_pExecutor = dynamic_cast<Scheduler::TPExecutor*>(IState::m_pHandler);
    if(m_pExecutor != nullptr && actionNum < 0)
    {
        m_pSession = m_pExecutor->GetCurrentSession();
        if(m_pSession == nullptr)
        {
            return;
        }

        qDebug() << "Session " << m_pSession->GetRetortID()
                 << " Program " << m_pSession->GetProgram()->GetName();

        auto startTime = QDateTime::currentDateTime();
        foreach(auto action, m_pSession->GetActionList())
        {
            action->SetStartTime(startTime);
            startTime.addSecs(action->GetDuration());
        }

        actionNum = m_pSession->GetActionList().count();
        SchedulerLogging::getInstance().Log4DualRetort(m_pSession->GetRetortID(), QString("******************Program %1 started.").arg(m_pSession->GetProgram()->GetName()));
    }

}

void NextAction::RepeatAction(TPTransition_t &pTransition)
{
    if(actionNum > 0)
    {
        int actionIndex = m_pSession->GetActionList().count()- actionNum;
        m_pExecutor->SetCurrentAction(actionIndex);
        m_pSession->UpdateCurrentActionIndex(actionIndex);
        pTransition = TPTransition_t::Execute;
        actionNum--;
    }
    else if(m_pSession != nullptr)
    {
        actionNum--;
        pTransition = TPTransition_t::Done;
    }
}

void NextAction::onExit(QEvent *event)
{
    if(actionNum < 0)
    {
        MsgClasses::CmdProgramAcknowledge* commandPtrFinish
                (new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_NO_CONTAMINATED, IState::m_pHandler->objectName()));
        Q_ASSERT(commandPtrFinish);
        Global::tRefType Ref = IState::m_pController->GetNewCommandRef();
        IState::m_pController->SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));

        SchedulerLogging::getInstance().Log4DualRetort(m_pSession->GetRetortID(), QString("******************Finished program %1.").arg(m_pSession->GetProgram()->GetName()));
    }
}

bool NextAction::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{
    qDebug()<<"*********** handle next action."<<pTransition;
}

}
}
