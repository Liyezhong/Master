/****************************************************************************/
/*! \file RsFillingAfterFlush.h
 *
 *  \brief CRsFillingAfterFlush class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ July 22th, 2014
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

#include "Scheduler/Include/RsFillingAfterFlush.h"
#include "Global/Include/Utils.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"

namespace Scheduler{
/*lint -e534 */
/*lint -e1551 */
/*lint -e616 */

CRsFillingAfterFlush::CRsFillingAfterFlush(SchedulerMainThreadController* SchedController, CSchedulerStateHandler* SchedulerStatehandler, CSchedulerStateMachine* StateMachine)
    :mp_SchedulerThreadController(SchedController)
    ,mp_SchedulerStateHandler(SchedulerStatehandler)
    ,mp_StateMachine(StateMachine)
{
    m_CurrentState = RSFILLINGAFTERFLUSH_INIT;
    m_StartTime = 0;
    m_MoveToSealingSeq = 0;
}

void CRsFillingAfterFlush::Start()
{
    m_CurrentState = RSFILLINGAFTERFLUSH_INIT;
    m_StartTime = 0;
    m_MoveToSealingSeq = 0;
}

CRsFillingAfterFlush::~CRsFillingAfterFlush()
{
}


void CRsFillingAfterFlush::HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qint64 nowTime = 0;
    quint16 retValue = 0;
    qreal tempLevelSensor = 0.0;

    switch(m_CurrentState)
    {
    case RSFILLINGAFTERFLUSH_INIT:
        mp_SchedulerStateHandler->HighPressure();
        m_CurrentState = RSFILLINGAFTERFLUSH_PRESSURE;
        break;
    case RSFILLINGAFTERFLUSH_PRESSURE:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: In set pressure state");
        if("Scheduler::ALPressure" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                mp_StateMachine->OnTasksDone(false);
            }
            else
            {
                m_StartTime = QDateTime::currentMSecsSinceEpoch();
                m_CurrentState = RSFILLINGAFTERFLUSH_WAIT;
            }
        }
        break;
    case RSFILLINGAFTERFLUSH_WAIT:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: In waiting state");
        nowTime = QDateTime::currentMSecsSinceEpoch();
        if(nowTime - m_StartTime >= 30 * 1000)
        {
            m_CurrentState = RSFILLINGAFTERFLUSH_HEATINGLEVELSENSOR;
        }
        break;
    case RSFILLINGAFTERFLUSH_HEATINGLEVELSENSOR:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: In heating Levelsensor temperature state");
        if(DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControl("LevelSensor"))
        {
            m_StartTime = 0;
            m_MoveToSealingSeq = 0;
            mp_StateMachine->OnTasksDone(false);
        }
        else
        {
            m_CurrentState = RSFILLINGAFTERFLUSH_CHECKLEVELSENSORTEMP;
        }
        break;
    case RSFILLINGAFTERFLUSH_CHECKLEVELSENSORTEMP:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: In check Levelsensor temperature state");
        tempLevelSensor = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempALLevelSensor;
        retValue = mp_SchedulerThreadController->GetHeatingStrategy()->CheckTemperatureOverTime("LevelSensor",tempLevelSensor);
        if (0 == retValue)
        {
            // Do nothing
        }
        else if (1 == retValue)
        {
            m_StartTime = 0;
            m_MoveToSealingSeq = 0;
            mp_StateMachine->OnTasksDone(false);
        }
        else if (2 == retValue)
        {
            mp_SchedulerStateHandler->Fill();
            m_CurrentState = RSFILLINGAFTERFLUSH_FILLING;
        }
        break;
    case RSFILLINGAFTERFLUSH_FILLING:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: In filling state");
        if( "Scheduler::ALFilling" == cmdName)
        {
            mp_SchedulerStateHandler->OnStopFill();
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                m_StartTime = 0;
                m_MoveToSealingSeq = 0;
                mp_StateMachine->OnTasksDone(false);
            }
            else
            {
                mp_SchedulerStateHandler->MoveRV(SEAL_POS);
                m_CurrentState = RSFILLINGAFTERFLUSH_MOVETOSEALING;
            }
        }
        break;
    case RSFILLINGAFTERFLUSH_MOVETOSEALING:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: In move to sealing state");
        if (0 == m_MoveToSealingSeq)
        {
            if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
            {
                if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    m_StartTime = 0;
                    m_MoveToSealingSeq = 0;
                    mp_StateMachine->OnTasksDone(false);
                }
                else
                {
                    m_MoveToSealingSeq++;
                }
            }
        }
        else
        {
            if (true == mp_SchedulerStateHandler->IsRVRightPosition(SEAL_POS))
            {

                m_MoveToSealingSeq = 0;
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, m_Sender));
                m_CurrentState = RSFILLINGAFTERFLUSH_RELEASEPRESSURE;
            }
            else
            {
                // Do nothing, just wait
            }

        }
        break;
    case RSFILLINGAFTERFLUSH_RELEASEPRESSURE:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: In release pressure state");
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                mp_SchedulerStateHandler->SetCurrentStepState(PSSM_PROCESSING);
                m_StartTime = 0;
                m_MoveToSealingSeq = 0;
                mp_StateMachine->OnTasksDone(true);
            }
            else
            {
                m_StartTime = 0;
                m_MoveToSealingSeq = 0;
                mp_StateMachine->OnTasksDone(false);
            }
        }

    default:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: Get the default branch");
        break;
    }
}



}

