#include "Scheduler/Include/TPExecutor.h"

namespace Scheduler {

TPExecutor::TPExecutor(const QString& name, EventDispatcher* pEventDispatcher)
    : IEventHandler(name, pEventDispatcher)
{
}

bool TPExecutor::HandleEvent(QEvent* event)
{
//    if(event->type() == TPEventType)
//    {
//        auto evt = dynamic_cast<QStateMachine::WrappedEvent*>(event);
//        if(evt != nullptr)
//            return true;

//        auto eventArgs = static_cast<TPCmdEvent<Global::CommandShPtr_t>*>(evt->object());

//        if(eventArgs != nullptr && eventArgs->Data().GetPointerToUserData()->GetName() == "MsgClasses::CmdProgramAction")
//        {
//             qDebug()<<"Handled Event type: " << eventArgs->Data()->GetName() << "by Handler: " << m_Name;
//             eventArgs->SetHandled();
//             return true;
//        }
//    }

    return IEventHandler::HandleEvent(event);
}

void TPExecutor::CreateStateMachine()
{

}

}
