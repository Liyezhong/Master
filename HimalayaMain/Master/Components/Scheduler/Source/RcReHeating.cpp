/****************************************************************************/
/*! \file RcReHeating.cpp
 *
 *  \brief CRcReHeating class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Sep 18th, 2014
 *   $Author:  $ Shufa Li
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2014 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Scheduler/Include/RcReHeating.h"
#include "Global/Include/Utils.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdIDForceDraining.h"
#include "Scheduler/Commands/Include/CmdALVaccum.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"

namespace Scheduler{

CRcReHeating::CRcReHeating(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
    ,m_LastScenario(0)
    ,m_StartReq(0)
    ,m_StartHeatingTime(0)
    ,m_StartPressureTime(0)
    ,m_IsNeedRunCleaning(false)
    ,m_DrainIsOk(false)
    ,m_HasReagent(false)
    ,m_OvenRemainingTime(0)
    ,m_RsReagentCheckStep(FORCE_DRAIN)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Init = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_StartSensorTemp = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_CheckSensorTemp = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_GetRvPosition = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_DrainCurrentReagent = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Init.data());
    mp_Init->addTransition(this, SIGNAL(SigTemperatureControlOn()), mp_StartSensorTemp.data());
    mp_StartSensorTemp->addTransition(this, SIGNAL(SigTemperatureSensorsChecking()), mp_CheckSensorTemp.data());
    mp_CheckSensorTemp->addTransition(this, SIGNAL(SigGetRVPosition()), mp_GetRvPosition.data());
    mp_GetRvPosition->addTransition(this, SIGNAL(SigDrainCurrentReagent()), mp_DrainCurrentReagent.data());
    mp_DrainCurrentReagent->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    // For error cases
    mp_StartSensorTemp->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_CheckSensorTemp->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_GetRvPosition->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());

    CONNECTSIGNALSLOT(this, SignalDrain(), mp_SchedulerThreadController, Drain());
    CONNECTSIGNALSLOT(this, SignalStopDrain(), mp_SchedulerThreadController, OnStopDrain());

    // Start up state machine
    mp_StateMachine->start();
}

CRcReHeating::~CRcReHeating()
{
    mp_StateMachine->stop();
}

CRcReHeating::StateList_t CRcReHeating::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = UNDEF;

    if (statesList.contains(mp_Init.data()))
    {
        currentState = INIT_STATE;
    }
    else if (statesList.contains(mp_StartSensorTemp.data()))
    {
        currentState = START_TEMPERATURE;
    }
    else if (statesList.contains(mp_CheckSensorTemp.data()))
    {
        currentState = CHECK_TEMPERATURE;
    }
    else if (statesList.contains(mp_GetRvPosition.data()))
    {
        currentState = GET_RV_POSOTION;
    }
    else if (statesList.contains(mp_DrainCurrentReagent.data()))
    {
        currentState = BEGIN_DRAIN;
    }
    return currentState;
}

void CRcReHeating::HandleWorkFlow(const QString &cmdName, ReturnCode_t retCode)
{
    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
    switch(currentState)
    {
        case INIT_STATE:
            emit SigTemperatureControlOn();
        case START_TEMPERATURE:
            if(true == StartHeatingSensor())
            {
                m_StartHeatingTime = QDateTime::currentMSecsSinceEpoch();
                emit SigTemperatureSensorsChecking();
            }else{
                emit TasksDone(false);
            }
            break;
        case CHECK_TEMPERATURE:
            CheckTheTemperature();
            break;
        case GET_RV_POSOTION:
            GetRvPosition(cmdName, retCode);
            break;
        case BEGIN_DRAIN:
            ProcessDraining(cmdName, retCode);
            break;
        default:
            break;
    }
}

bool CRcReHeating::StartHeatingSensor()
{
    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;
    if( 200 == m_LastScenario || 260 == m_LastScenario || (m_LastScenario >= 211 && m_LastScenario <= 257) )
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(4);
    }
    else if(203 == m_LastScenario || (281 <= m_LastScenario && m_LastScenario <= 297) )
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(4);
        ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RV");
        mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating RV,retCode:%1").arg(ret));
        if(DCL_ERR_FCT_CALL_SUCCESS != ret)
            return false;
        else
            return true;
    }
    else
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(260);
    }
    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RV");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating RV,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("LA_Tube1");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating LA_Tube1,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("LA_Tube2");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating LA_Tube2,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return true;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("OvenTop");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating oven top,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("OvenBottom");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating oven bottom,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    if(m_LastScenario >= 271 && m_LastScenario <= 277)
    {
       ret =  mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RTSide");
       mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating retort top,retCode:%1").arg(ret));
        if(DCL_ERR_FCT_CALL_SUCCESS != ret)
            return false;

        ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RTBottom");
        mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating retort bottom,retCode:%1").arg(ret));
        if(DCL_ERR_FCT_CALL_SUCCESS != ret)
            return false;
    }
    return true;
}

void CRcReHeating::CheckTheTemperature()
{
    if(200 == m_LastScenario || 260 == m_LastScenario || 283 == m_LastScenario || 293 == m_LastScenario || (m_LastScenario >= 211 && m_LastScenario <= 257))
    {
        if(mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempRV2 > RV_SENSOR2_TEMP)
        {
            emit SigGetRVPosition();
        }
        else
        {
            if(QDateTime::currentMSecsSinceEpoch() - m_StartHeatingTime > WAIT_RV_SENSOR2_TEMP * 1000)
            {
                emit TasksDone(false);
            }
        }
    }
    else if(203 == m_LastScenario || (281 <= m_LastScenario && m_LastScenario <= 297 && 283 != m_LastScenario && 293 != m_LastScenario
                                      && 285 != m_LastScenario && 295 != m_LastScenario && 287 != m_LastScenario && 297 != m_LastScenario) )
    {
        if(mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempRV2 > RV_SENSOR2_TEMP)
        {
            emit TasksDone(true);//cleaning just judge the RV sensor2 temperature
        }
        else
        {
            if(QDateTime::currentMSecsSinceEpoch() - m_StartHeatingTime > WAIT_RV_SENSOR2_TEMP * 1000)
            {
                emit TasksDone(false);
            }
        }
    }
    else
    {
        if(QFile::exists("TEST_GINA"))
        {
            if(mp_SchedulerThreadController->GetHeatingStrategy()->CheckSensorsTemp(mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor()))
            {
                emit SigGetRVPosition();
            }
        }
        else
        {
            if(0 == m_StartReq)
            {
                m_OvenRemainingTime = mp_SchedulerThreadController->GetOvenHeatingRemainingTime() * 1000;
            }
            m_StartReq++;
            if(2 == m_StartReq)
            {
                m_StartReq = 0;
            }
            if(QDateTime::currentMSecsSinceEpoch() - m_StartHeatingTime > m_OvenRemainingTime)
            {
                if(mp_SchedulerThreadController->GetHeatingStrategy()->CheckSensorsTemp(mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor()))
                {
                    emit SigGetRVPosition();
                }
                else
                {
                    emit TasksDone(false);
                }
                m_StartReq = 0;
            }
        }
    }
}

void CRcReHeating::GetRvPosition(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if(200 == m_LastScenario || 260 == m_LastScenario)
    {
        if (0 == m_StartReq)
        {
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdRVReqMoveToInitialPosition(500, mp_SchedulerThreadController));
            m_StartReq++;
        }
        else if(mp_SchedulerThreadController->IsRVRightPosition(INITIALIZE_POS))
        {
            if(m_IsNeedRunCleaning)
            {
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_POWERFAILURE_FINISH);
            }
            emit TasksDone(true);
            m_StartReq = 0;
        }
        else if("Scheduler::RVReqMoveToInitialPosition" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                emit TasksDone(false);
                m_StartReq = 0;
            }

        }
    }
    else
    {
        switch(m_RsReagentCheckStep)
        {
        case FORCE_DRAIN:
            if(0 == m_StartReq)
            {
                mp_SchedulerThreadController->LogDebug("reagent check build pressure.");
                CmdALPressure* CmdPressure = new CmdALPressure(500, mp_SchedulerThreadController);
                CmdPressure->SetTargetPressure(30.0);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(CmdPressure);
                m_StartPressureTime = QDateTime::currentMSecsSinceEpoch();
                m_StartReq++;
            }
            else if(1 == m_StartReq)
            {
                if(QDateTime::currentMSecsSinceEpoch() - m_StartPressureTime > 60 * 1000)
                {
                    m_StartReq++;
                    m_HasReagent = false;
                }
            }
            else if(2 == m_StartReq)
            {
                mp_SchedulerThreadController->LogDebug(QString("get the current pressure is:%1").arg(mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().PressureAL));
                if(qAbs(mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().PressureAL) < 5.0) //2 * GetBasePressure())
                {
                    m_HasReagent = false;
                }
                else
                {
                    m_HasReagent = true;
                }
                m_StartReq++;
            }
            else if(3 == m_StartReq)
            {
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
                m_StartReq++;
            }
            else if(4 == m_StartReq)
            {
                if("Scheduler::ALReleasePressure" == cmdName)
                {
                    if(!m_HasReagent)
                    {
                        m_RsReagentCheckStep = MOVE_INITIALIZE_POSITION;
                    }
                    else
                    {
                        m_RsReagentCheckStep = BUILD_VACUUM;
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case BUILD_VACUUM:
            if(0 == m_StartReq)
            {
                mp_SchedulerThreadController->LogDebug("Send cmd to DCL to build vacuum in PowerFailure");
                CmdALVaccum* cmd = new CmdALVaccum(500, mp_SchedulerThreadController);
                cmd->SetTargetPressure(-1.0);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
            }
            else if("Scheduler::ALVaccum" == cmdName)
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_RsReagentCheckStep = MOVE_INITIALIZE_POSITION;
                }
                else
                {
                    m_RsReagentCheckStep = FORCE_DRAIN;
                    emit TasksDone(false);
                }
                m_StartReq = 0;
            }
            break;
        case MOVE_INITIALIZE_POSITION:
            if(0 == m_StartReq)
            {
                CmdRVReqMoveToInitialPosition *cmd = new CmdRVReqMoveToInitialPosition(500, mp_SchedulerThreadController);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
            }
            else if(mp_SchedulerThreadController->IsRVRightPosition(INITIALIZE_POS))
            {
                if(!m_HasReagent)
                {
                    if(m_IsNeedRunCleaning)
                    {
                        mp_SchedulerThreadController->SetCurrentStepState(PSSM_POWERFAILURE_FINISH);
                    }
                    m_RsReagentCheckStep = FORCE_DRAIN;
                    emit TasksDone(true);
                }
                else
                {
                    m_RsReagentCheckStep = MOVE_TUBEPOSITION;
                }
                m_StartReq = 0;
            }
            else if("Scheduler::RVReqMoveToInitialPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    m_RsReagentCheckStep = FORCE_DRAIN;
                    emit TasksDone(false);
                    m_StartReq = 0;
                }
            }
            break;
        case MOVE_TUBEPOSITION:
            if(0 == m_StartReq)
            {
                if( !mp_SchedulerThreadController->MoveRV(TUBE_POS) )
                {
                    m_RsReagentCheckStep = FORCE_DRAIN;
                    emit TasksDone(false);
                    m_StartReq = 0;
                }
                else
                {
                    m_StartReq++;
                }
            }
            else if(mp_SchedulerThreadController->IsRVRightPosition(TUBE_POS))
            {
                m_StartReq = 0;
                m_RsReagentCheckStep = REALSE_PRESSRE;
            }
            else
            {
                if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                    {
                        m_RsReagentCheckStep = FORCE_DRAIN;
                        emit TasksDone(false);
                    }
                }
            }
            break;
        case REALSE_PRESSRE:
            if(0 == m_StartReq)
            {
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
                m_StartReq++;
            }
            else if("Scheduler::ALReleasePressure" == cmdName)
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    emit SigDrainCurrentReagent();
                }
                else
                {
                    emit TasksDone(false);
                }
                m_StartReq = 0;
                m_RsReagentCheckStep = FORCE_DRAIN;
            }
            break;
        default:
            break;
        }
    }
}

void CRcReHeating::ProcessDraining(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if (0 == m_StartReq)
    {
        CmdIDForceDraining* cmd  = new CmdIDForceDraining(500, mp_SchedulerThreadController);

        QString stationID = mp_SchedulerThreadController->GetCurrentStationID();
        RVPosition_t tubePos = mp_SchedulerThreadController->GetRVTubePositionByStationID(stationID);
        cmd->SetRVPosition((quint32)(tubePos));
        cmd->SetDrainPressure(40.0);
        mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
        m_StartReq ++;
        emit SignalDrain();
    }
    else if ("Scheduler::IDForceDraining" == cmdName)
    {
        emit SignalStopDrain();
        if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
        {
            m_DrainIsOk = true;
        }
        else
        {
            m_DrainIsOk = false;
        }
        m_StartReq++;
    }
    else if(2 == m_StartReq)
    {
        if(m_DrainIsOk)
        {
            if(m_IsNeedRunCleaning)
            {
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_POWERFAILURE_FINISH);
            }
            emit TasksDone(true);
        }
        else
        {
            emit TasksDone(false);
        }
        m_StartReq = 0;
    }
}

qreal CRcReHeating::GetBasePressure()
{
    qreal baseline = 1.0;
    if((m_LastReagentID == "RG1")||(m_LastReagentID == "RG2"))
    {
        baseline = 1.33;
    }
    else if((m_LastReagentID == "RG3")||(m_LastReagentID == "RG4")||(m_LastReagentID == "RG8"))
    {
        baseline = 1.05;
    }
    else if((m_LastReagentID == "RG5")||(m_LastReagentID == "RG7"))
    {
        baseline = 1.14;
    }
    else if((m_LastReagentID == "RG6"))
    {
        baseline = 0.53;
    }
    return baseline;
}

}
