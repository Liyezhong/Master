#include "Scheduler/Include/IAction.h"
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/Session.h"
#include <QDebug>


using namespace DeviceControl;
namespace Scheduler{
IAction::IAction(SchedulerCommandProcessorBase* commandProcessor, Session* session, ActionType_t type)
    :mp_SchedulerCommandProcessor(commandProcessor),
     mp_session(session),
     m_type(type),
     m_stateWaitResult(false),
     m_finished(false),
     m_reagentID(""),
     m_currentState(STATE_FILLING_RVROD_HEATING)
{

}

IAction::~IAction()
{

}

void IAction::Execute(const QString& cmdName, ReturnCode_t retCode)
{
    switch (m_type)
    {
    case FILLING:
        //SetResult(pIDP->ALFilling(500,true,false));
        Fill(cmdName, retCode);
        break;
    case DRAINING:
        Drain(cmdName, retCode);
    case PURGE:
        Purge(cmdName, retCode);
    case SOAKING:
        //TO DO
        m_finished = true;
        break;
    default:
        qDebug()<<"error type for run action.";
    }
}

void IAction::Fill(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    switch (m_currentState)
    {
    case STATE_FILLING_RVROD_HEATING:
        //to do
    {
        RVPosition_t position = GetRVPosition(m_stationID, true);
        if (m_stateWaitResult)
        {
            if (IsRVRightPosition(position))
            {
                m_currentState = STATE_FILLING_LEVELSENSOR_HEATING;
                m_stateWaitResult = false;
            }
        }
        else
        {
            CmdRVReqMoveToRVPosition* moveRVcmd = new CmdRVReqMoveToRVPosition(500, mp_session->GetRetortID());
            moveRVcmd->SetRVPosition(position);
            mp_SchedulerCommandProcessor->pushCmd(moveRVcmd);
            m_stateWaitResult = true;
        }
    }
        break;
    case STATE_FILLING_LEVELSENSOR_HEATING:
        //to do
        if (m_stateWaitResult)
        {
            m_currentState = STATE_FILLING;
            m_stateWaitResult = false;
        }
        break;
    case STATE_FILLING:
        if (m_stateWaitResult)
        {
            if( "Scheduler::ALFilling" == cmdName)
            {
                qDebug()<<"************************ fill result:"<<retCode;
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_currentState = STATE_RV_MOVE_TO_SEAL;
                    m_stateWaitResult = false;
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
            CmdALFilling* fillCmd  = new CmdALFilling(500, mp_session->GetRetortID());
            mp_SchedulerCommandProcessor->pushCmd(fillCmd);
            m_stateWaitResult = true;
        }
        break;
    case STATE_RV_MOVE_TO_SEAL:

        RVPosition_t position = GetRVPosition(m_stationID, false);
        if (m_stateWaitResult)
        {
            if (IsRVRightPosition(position))
            {
                m_currentState = STATE_FILLING_RVROD_HEATING;
                m_stateWaitResult = false;
                 m_finished = true;
            }
        }
        else
        {
            CmdRVReqMoveToRVPosition* moveRVcmd = new CmdRVReqMoveToRVPosition(500, mp_session->GetRetortID());
            moveRVcmd->SetRVPosition(position);
            mp_SchedulerCommandProcessor->pushCmd(moveRVcmd);
            m_stateWaitResult = true;
        }
        break;
    }
}

void IAction::Drain(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    m_finished = true;
    return;
}

void IAction::Purge(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    m_finished = true;
    return;
}

RVPosition_t IAction::GetRVPosition(const QString& stationID, bool isTube)
{
    RVPosition_t ret = RV_UNDEF;
    bool ok = false;
    if (!stationID.isEmpty())
    {
        int position = stationID.right(stationID.length() - 1).toInt(&ok, 10);
        if (ok)
        {
            int offset = 0;
            if (isTube)
            {
                offset = 1;
            }

            if(stationID.left(1) == "S")
            {
                ret = (RVPosition_t)(position * 2 - offset);
            }
            else if(stationID.left(1) == "P")
            {
                ret = (RVPosition_t)((position + 13) * 2 - offset);
            }
        }
    }
    return ret;
}

bool IAction::IsRVRightPosition(RVPosition_t position)
{
    return true;
}

}
