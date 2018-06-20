#include "Scheduler/Include/Actions/DrainAction.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Include/SchedulerLogging.h"
#include <QDebug>

using namespace DeviceControl;
namespace Scheduler{

DrainAction::DrainAction(SchedulerCommandProcessorBase* commandProcessor, Session* session, bool isLastStep):
    IAction(session),
    ActionHelper(commandProcessor),
    m_currentState(STATE_DRAINING_RVROD),
    m_stateWaitResult(false),
    m_isLastStep(isLastStep)
{
    m_type = DRAINING;
}

DrainAction::~DrainAction()
{

}

void DrainAction::Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    RVPosition_t position = RV_POSITION_UNDEF;
    qDebug()<<"************************ drain action:"<<m_currentState;
    switch (m_currentState)
    {
    case STATE_DRAINING_RVROD:
        position = GetRVPosition(m_stationID);
        if (m_stateWaitResult)
        {
            if (IsRVRightPosition(position, mp_session->GetRetortID()))
            {
                m_currentState = STATE_DRAINING;
                m_stateWaitResult = false;
            }
        }
        else
        {
            SchedulerLogging::getInstance().Log4DualRetort(mp_session->GetRetortID(), QString("Execute drain action: reagent:%1, station:%2, seq:%3")
                                                           .arg(m_reagentID).arg(m_stationID).arg(m_currentState));
            CmdRVReqMoveToRVPosition* moveRVcmd = new CmdRVReqMoveToRVPosition(500, mp_session->GetRetortID());
            moveRVcmd->SetRVPosition(position);
            mp_SchedulerCommandProcessor->pushCmd(moveRVcmd);
            m_stateWaitResult = true;
        }
        break;
    case STATE_DRAINING:
        if (m_stateWaitResult)
        {
            if( "Scheduler::ALDraining" == cmdName)
            {
                qDebug()<<"************************ drain result:"<<retCode;
                SchedulerLogging::getInstance().Log4DualRetort(mp_session->GetRetortID(), QString("Drain finished, result:%1").arg(retCode));
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_currentState = STATE_DRAINING_RVROD;
                    m_stateWaitResult = false;
                    m_finished = true;
                }
                else
                {
                    //retry
                    return;
                }
            }
        }
        else
        {
            CmdALDraining* drainCmd  = new CmdALDraining(500, mp_session->GetRetortID());
            mp_SchedulerCommandProcessor->pushCmd(drainCmd);
            m_stateWaitResult = true;
        }
        break;
    }
}

}
