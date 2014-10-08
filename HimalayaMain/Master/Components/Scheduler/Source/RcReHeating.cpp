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

namespace Scheduler{

CRcReHeating::CRcReHeating(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
    ,m_LastScenario(0)
    ,m_StartReq(0)
    ,m_StartHeatingTime(0)
    ,m_RcReHeatingStep(START_TEMPERATURE)
    ,m_IsNeedRunCleaning(false)
    ,m_DrainIsOk(false)
{
    CONNECTSIGNALSLOT(this, SignalDrain(), mp_SchedulerThreadController, Drain());
    CONNECTSIGNALSLOT(this, SignalStopDrain(), mp_SchedulerThreadController, OnStopDrain());
}

CRcReHeating::~CRcReHeating()
{
}

void CRcReHeating::HandleWorkFlow(const QString &cmdName, ReturnCode_t retCode)
{
    switch(m_RcReHeatingStep)
    {
        case START_TEMPERATURE:
            if(true == StartHeatingSensor())
            {
                m_RcReHeatingStep = CHECK_TEMPERATURE;
                m_StartHeatingTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                emit TasksDone(false);
            }
            break;
        case CHECK_TEMPERATURE:
            if(true == CheckTheTemperature())
            {
                if(200 == m_LastScenario || 260 == m_LastScenario)
                {
                    if(m_IsNeedRunCleaning)
                    {
                        mp_SchedulerThreadController->SetCurrentStepState(PSSM_PROGRAM_FINISH);
                    }
                    emit TasksDone(true);
                }
                else
                {
                    m_RcReHeatingStep = BEGIN_DRAIN;
                }
            }
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
    if( 200 == m_LastScenario || 260 == m_LastScenario || (m_LastScenario >= 211 && m_LastScenario <= 257) )
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(4);
    }
    else
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(260);
    }
    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;
    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RV");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating ProcessParaffinStep start heating RV,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("LA_Tube1");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating ProcessParaffinStep start heating LA_Tube1,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("LA_Tube2");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating ProcessParaffinStep start heating LA_Tube2,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return true;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("OvenTop");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating ProcessParaffinStep start heating oven top,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("OvenBottom");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating ProcessParaffinStep start heating oven bottom,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    if(m_LastScenario >= 271 && m_LastScenario <= 277)
    {
       ret =  mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RTSide");
       mp_SchedulerThreadController->LogDebug(QString("In RcReHeating ProcessParaffinStep start heating retort top,retCode:%1").arg(ret));
        if(DCL_ERR_FCT_CALL_SUCCESS != ret)
            return false;

        ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RTBottom");
        mp_SchedulerThreadController->LogDebug(QString("In RcReHeating ProcessParaffinStep start heating retort bottom,retCode:%1").arg(ret));
        if(DCL_ERR_FCT_CALL_SUCCESS != ret)
            return false;
    }
    return true;
}

bool CRcReHeating::CheckTheTemperature()
{
    bool ret = false;
    if(200 == m_LastScenario || 260 == m_LastScenario || (m_LastScenario >= 211 && m_LastScenario <= 257))
    {
        if(mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempRV2 > RV_SENSOR2_TEMP)
        {
            ret =true;
        }
        else
        {
            if(QDateTime::currentMSecsSinceEpoch() - m_StartHeatingTime > WAIT_RV_SENSOR2_TEMP * 1000)
                emit TasksDone(false);
        }
    }
    else
    {
        if(QDateTime::currentMSecsSinceEpoch() - m_StartHeatingTime > mp_SchedulerThreadController->GetOvenHeatingTime() * 1000)
        {
            ret = true;
        }
    }
    return ret;
}

void CRcReHeating::ProcessDraining(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if (0 == m_StartReq)
    {
        mp_SchedulerThreadController->MoveRV(0);
        m_StartReq++;
    }
    else if(1 == m_StartReq)
    {
        if( mp_SchedulerThreadController->IsRVRightPosition(0) )
        {
            mp_SchedulerThreadController->LogDebug("Moving to tube OK");
            m_StartReq++;
        }
    }
    else if(2 == m_StartReq)
    {
        emit SignalDrain();
        m_StartReq++;
    }
    else if(3 == m_StartReq)
    {
        if( "Scheduler::ALDraining" == cmdName)
        {
            emit SignalStopDrain();

            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                m_DrainIsOk = true;
            }
            m_StartReq ++;
        }
    }
    else if(4 == m_StartReq)
    {
        if(m_IsNeedRunCleaning)
        {
            mp_SchedulerThreadController->SetCurrentStepState(PSSM_PROGRAM_FINISH);
        }
        if(m_DrainIsOk)
        {
            emit TasksDone(true);
        }
        else
        {
            emit TasksDone(false);
        }
        m_StartReq = 0;
    }
}


}
