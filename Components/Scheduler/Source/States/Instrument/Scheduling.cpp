#include "Scheduler/Include/States/Instrument/Scheduling.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "Scheduler/Include/ProtocolParser.h"
#include "Scheduler/Include/InstrumentManager.h"

#include "Scheduler/Include/Session.h"
#include "Scheduler/Include/Actions/IAction.h"
#include <QTime>

namespace Scheduler{
namespace Instrument{

Scheduling::Scheduling(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller),
      commandPtr(nullptr)
{
    setObjectName("Instrument_Scheduling_State");
}

Scheduling::~Scheduling()
{

}

void Scheduling::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
    ProtocolParser* parser = IState::m_pController->GetProtocolParser();
    parser->GenerateActionList(((InstrumentManager*)IState::m_pHandler)->GetInitialSession().first());
    commandPtr = nullptr;
    ref = -1;

    machine()->postDelayedEvent(new TPEvent(TPTransition_t::Done), 500);
}

void Scheduling::onExit(QEvent *event)
{
    if(commandPtr != nullptr)
    {
        IState::m_pController->SendCommand(ref, Global::CommandShPtr_t(commandPtr));
    }
}

void Scheduling::RepeatAction(TPTransition_t &pTransition)
{

}

bool Scheduling::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{    
    pTransition = TPTransition_t::Done;
//    Session *base;
//    Session *select;
//    pSelectedcmd->GetRetortId()
    return false;
}


// Session *base;  @base had being operating.
// Session *target; @target will be operating.
void Scheduling::Operating(Session *base, Session *target)
{

    QTime startTime = QTime::currentTime();
    startTime = startTime.addSecs(2);  // add offset 2 seconds

    //  1. Only one program to run.
    if (base == nullptr) {
        target->SetProgramStartTime(startTime);
        return;
    }

    target->SetProgramStartTime(startTime);

    // 2. There are two program, to do the second program.
    auto baseActionList = base->GetActionList();
    QSharedPointer<IAction> targetFormalinSoaking;

    auto baseActionIndex = 0;
    auto targetActionIndex = 0;

    for (;baseActionIndex < base->GetActionList().size() && targetActionIndex < target->GetActionList().size();) {
      baseActionIndex = GetSameStationSoakingActionIndex(baseActionIndex, base);
      if (base->GetActionList()[baseActionIndex]->GetStartTime()
              <= target->GetActionList()[targetActionIndex]->GetStartTime()) {
          if (base->GetActionList()[baseActionIndex]->GetStationID()
                  == target->GetActionList()[targetActionIndex]->GetStationID()) {
          // 1. base's station id == target's station id
          //    change to next soaking action.
            baseActionIndex++;
          } else {
          // 2. base's station id != target's station id
            int endIndex = GetSameStationActionEndIndex(targetActionIndex, target->GetActionList()[targetActionIndex]->GetStationID(), target);
            if (target->GetActionList()[targetActionIndex]->GetStepEndTime()
                    < base->GetActionList()[baseActionIndex]->GetEndTime()) {

            }
            int baseNextSoakingActionIndex = baseActionIndex;
            for (;;) {
                baseNextSoakingActionIndex = GetSameStationNextSoakingActionIndex(baseNextSoakingActionIndex, base);
                if (base->GetActionList()[baseNextSoakingActionIndex]->GetStartTime()
                        < target->GetActionList()[targetActionIndex]->GetStepEndTime()) {
                    if (base->GetActionList()[baseNextSoakingActionIndex]->GetEndTime()
                            > target->GetActionList()[targetActionIndex]->GetStepEndTime()) {
                        baseActionIndex = GetSameStationNextSoakingActionIndex(baseNextSoakingActionIndex, base);
                        break;
                    }
                    if (base->GetActionList()[baseNextSoakingActionIndex]->GetEndTime()
                            < target->GetActionList()[targetActionIndex]->GetStepEndTime()) {
                        // FIXME

                    }
                }

            }
            targetActionIndex++;
          }
      } else {
        int offsetTime = base->GetActionList()[baseActionIndex]->GetStepStartTime().second()
                - target->GetActionList()[targetActionIndex]->GetStartTime().second();
        offsetTime++; // FIXME
        target->GetActionList()[0]->SetOffsetSecond(target->GetActionList()[0]->GetOffsetSecond() + offsetTime);
      }
    }
}

// Get step index by station id;
int  Scheduling::GetSameStationActionStartIndex(int index, QString stationID, Session *target)
{
    auto targetActionList = target->GetActionList();
    for (int i = index; i < targetActionList.size(); i++) {
        if (targetActionList[i].data()->GetStationID() == stationID)
            return i;
    }
    return -1;
}

int  Scheduling::GetSameStationActionEndIndex(int index, QString stationID, Session *target)
{
    int si = GetSameStationActionStartIndex(index, stationID, target);
    if (si == -1)
        return -1;

    auto targetActionList = target->GetActionList();

    for (int i = si + 1; i < targetActionList.size(); i++) {
        if (targetActionList[i].data()->GetStationID() != stationID)
            return i - 1;
    }
    return targetActionList.size() - 1;
}

int  Scheduling::GetSameStationSoakingActionIndex(int index, Session *target)
{
    for (int i = index; i < target->GetActionList().size(); i++) {
        if (target->GetActionList()[i].data()->GetActionType()
                == Scheduler::ActionType_t::SOAKING)
            return i;
    }
    return -1;
}

int  Scheduling::GetSameStationNextSoakingActionIndex(int index, Session *target)
{
    return GetSameStationSoakingActionIndex(index + 1, target);
}

}
}
