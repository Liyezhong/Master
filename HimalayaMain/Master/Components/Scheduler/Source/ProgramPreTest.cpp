/****************************************************************************/
/*! \file ProgramPreTest.cpp
 *
 *  \brief CProgramPreTest class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ June 18th, 2014
 *   $Author:  $ Songtao Yu
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
#include <QCoreApplication>

#include "Scheduler/Include/ProgramPreTest.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Include/SchedulerEventCodes.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdIDSealingCheck.h"

using namespace DeviceControl;
namespace Scheduler{
/*lint -e534 */
/*lint -e525 */
/*lint -e527 */
/*lint -e616 */

CProgramPreTest::CProgramPreTest(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
{
    // Start up state machine
    //mp_StateMachine->start();
    m_CurrentState = PRETEST_UNDEF;
    m_RTTempStartTime = 0;
    m_RTTempOffSeq = 0;
    m_RVPositioinChkSeq = 0;
    m_PressureChkSeq = 0;
    m_SetPrressureTime = 0;
    m_PressureCalibrationSeq = 0;
    m_PressureCalibrationCounter = 0;
	m_PressureDriftOffset = 0.0;
    m_PressureSealingChkSeq = 0;
    m_BottleChkFlag = true;
    m_BottleSeq = 0;
    m_MoveToTubeSeq = 0;
    m_IsLoged = 0;
    m_IsAbortRecv = false;
    m_TasksAborted = false;
}

CProgramPreTest::~CProgramPreTest()
{
}

void CProgramPreTest::Start()
{
    this->ResetVarList();
}


void CProgramPreTest::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
	qreal currentPressure = 0.0;
    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;

    qint64 now = 0;
    ReportError_t reportError1;
    memset(&reportError1, 0, sizeof(reportError1));
    ReportError_t reportError2;
    memset(&reportError2, 0, sizeof(reportError2));
    ReportError_t reportError3;
    memset(&reportError3, 0, sizeof(reportError3));
    ReportError_t reportError4;
    memset(&reportError4, 0, sizeof(reportError4));

    //Firstly, we need check if CTRL_CMD_ABORT has been received
    if (m_IsAbortRecv)
    {
        if (PRETEST_INIT == m_CurrentState || RT_TEMCTRL_ON == m_CurrentState
                || TEMPSENSORS_CHECKING == m_CurrentState || WAIT3S_RT_CURRENT == m_CurrentState
                || RT_TEMCTRL_OFF == m_CurrentState || PRESSURE_CALIBRATION == m_CurrentState)
        {
            m_TasksAborted = true;
            return;
        }
    }

    switch (m_CurrentState)
    {
    case PRETEST_INIT:
        m_CurrentState = RT_TEMCTRL_ON;
        break;
    case RT_TEMCTRL_ON:
        ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPreTest("RTSide");
        if (DCL_ERR_FCT_CALL_SUCCESS == ret)
        {
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPreTest("RTBottom");
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                m_CurrentState = TEMPSENSORS_CHECKING;
                m_RTTempStartTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                mp_SchedulerThreadController->SendOutErrMsg(ret);
            }
        }
        else
        {
            mp_SchedulerThreadController->SendOutErrMsg(ret);
        }
        break;
    case TEMPSENSORS_CHECKING:
        if(0 == m_IsLoged)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_CHECK_LEVELSENSOR);
            m_IsLoged++;
        }
        if (true == mp_SchedulerThreadController->GetHeatingStrategy()->CheckTemperatureSenseorsStatus())
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_CHECK_LEVELSENSOR_SUCCESS);
            m_CurrentState = WAIT3S_RT_CURRENT;
            m_IsLoged = 0;
        }
        break;
    case WAIT3S_RT_CURRENT:
        if(0 == m_IsLoged)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_3S);
            m_IsLoged++;
        }
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_RTTempStartTime) >= 3*1000)
        {
            m_IsLoged = 0;
            m_CurrentState = RT_TEMCTRL_OFF;
        }
        else
        {
            reportError1 = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_BOTTOM);
            reportError2 = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_SIDE);
            reportError3 = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_TOP);
            reportError4 = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_BOTTOM);
            if (reportError1.instanceID != 0 && (now-reportError1.errorTime) <= 3*1000)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE);
            }
            if (reportError2.instanceID != 0 && (now-reportError2.errorTime) <= 3*1000)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE);
            }
            if (reportError3.instanceID != 0 && (now-reportError3.errorTime) <= 3*1000)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE);
            }
            if (reportError4.instanceID != 0 && (now-reportError4.errorTime) <= 3*1000)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE);
            }
        }
    case RT_TEMCTRL_OFF:
        if (0 == m_RTTempOffSeq)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTSIDE_TEMP);
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTSide");
            if (DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTSIDE_TEMP_SUCCESS);
                m_RTTempOffSeq = 1;
            }
            else
            {
                m_RTTempOffSeq = 0;
                mp_SchedulerThreadController->SendOutErrMsg(ret); // Send out error message
            }
        }
        else
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTBOTTOM_TEMP);
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTBottom");
            if (DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTBOTTOM_TEMP_SUCCESS);
                m_RTTempOffSeq = 0;
                m_CurrentState = PRESSURE_CALIBRATION;
            }
            else
            {
                m_RTTempOffSeq = 0;
                mp_SchedulerThreadController->SendOutErrMsg(ret); // Send out error mesage
            }
        }
        break;
    case PRESSURE_CALIBRATION:
        if (0 == m_PressureCalibrationSeq)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_RELEASE_PRESSURE_CALIBRATION);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
            m_PressureCalibrationSeq++;
        }
        else if (1 == m_PressureCalibrationSeq)
        {
            if ("Scheduler::ALReleasePressure" == cmdName)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_RELEASE_PRESSURE_CALIBRATION_SUCCESS);
                m_PressureCalibrationSeq++;
                m_ReleasePressureTime = QDateTime::currentMSecsSinceEpoch();
            }
        }
        else if (2 == m_PressureCalibrationSeq)
        {
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_WAIT_20S);
                m_IsLoged++;
            }
            if ((QDateTime::currentMSecsSinceEpoch() - m_ReleasePressureTime) >= 20*1000)
            {
                m_IsLoged = 0;
                m_PressureCalibrationSeq++;
            }
        }
        else if (3 == m_PressureCalibrationSeq)
        {
            // Firstly, check if calibration has been 3 times
            if (3 == m_PressureCalibrationCounter)
            {
                m_PressureCalibrationSeq = 0;
                m_PressureCalibrationCounter = 0;
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_PRESSURESENSOR_PRECHECK_FAILED);
            }
            else if (m_PressureCalibrationCounter > 0)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_RETRY_PRESSURE_CALIBRATION);
            }

            m_PressureCalibrationCounter++;

            currentPressure = mp_SchedulerThreadController->GetSchedCommandProcessor()->ALGetRecentPressure();
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_GET_CURRENT_PRESSURE, QStringList()<<QString::number(currentPressure));

            if (qAbs(currentPressure) < 0.2) // Calibration is Not needed
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_PRESSURE_CALIBRATION_SUCCESS);
                m_PressureCalibrationSeq = 0;
                m_PressureCalibrationCounter = 0;
                m_CurrentState = RV_POSITION_CHECKING;
            }
            else if (qAbs(currentPressure) <= 2.0) //offset the calibration
            {
                m_PressureDriftOffset = m_PressureDriftOffset + currentPressure;
                mp_SchedulerThreadController->GetSchedCommandProcessor()->ALSetPressureDrift(m_PressureDriftOffset);
                mp_SchedulerThreadController->SetLastPressureOffset(m_PressureDriftOffset);
                m_PressureCalibrationSeq = 0;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_OFFSET_CALIBRATION);
            }
        }
        break;
    case RV_POSITION_CHECKING:
        if (0 == m_RVPositioinChkSeq)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_MOVETO_INITIALIZE_POSITION);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdRVReqMoveToInitialPosition(500, mp_SchedulerThreadController));
            m_RVPositioinChkSeq++;
        }
        else
        {
            if ("Scheduler::RVReqMoveToInitialPosition" == cmdName)
            {
                if (m_IsAbortRecv)
                {
                    m_RVPositioinChkSeq = 0;
                    m_TasksAborted = true;
                    break;
                }
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_RVPositioinChkSeq = 0;
                    m_CurrentState = PRESSURE_SEALING_CHECKING;
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_MOVETO_INITIALIZE_POSITION_SUCCESS);
                }
                else
                {
                    m_RVPositioinChkSeq = 0;
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
        }
        break;
    case PRESSURE_SEALING_CHECKING:
        if (0 == m_PressureSealingChkSeq)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_SEALING_TEST);
            RVPosition_t RVSealPos = mp_SchedulerThreadController->GetRVSealPositionByStationID(mp_SchedulerThreadController->GetCurrentStationID());
            CmdIDSealingCheck* cmd = new CmdIDSealingCheck(500, mp_SchedulerThreadController);
            cmd->SetSealPosition(RVSealPos);
            cmd->SetThresholdPressure(5.0);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
            m_PressureSealingChkSeq++;
        }
        else
        {
            if ("Scheduler::IDSealingCheck" == cmdName)
            {
                if (m_IsAbortRecv)
                {
                    m_PressureSealingChkSeq = 0;
                    m_TasksAborted = true;
                    break;
                }
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_SEALING_TEST_SUCCESS);
                    m_PressureSealingChkSeq = 0;
                    if(mp_SchedulerThreadController->IsNeedBottleCheck())
                    {
                        m_CurrentState = BOTTLES_CHECKING;
                    }
                    else
                    {
                        m_CurrentState = MOVE_TO_TUBE;
                    }
                }
                else
                {
                    m_PressureSealingChkSeq = 0;
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
        }
        break;
    case BOTTLES_CHECKING:
        if(0 == m_IsLoged)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK);
            m_IsLoged++;
        }
        if (true == m_BottleChkFlag)  // Send out IDBottleCheck command
        {
            if (m_IsAbortRecv)
            {
                m_TasksAborted = true;
                break;
            }
            if (mp_SchedulerThreadController->BottleCheck(m_BottleSeq))
            {
                m_BottleChkFlag = false;
            }
            else // all the bottle check (for 16 bottles) has been done
            {
                m_IsLoged = 0;
                m_BottleSeq = 0; //reset
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK_SUCCESS);
                m_CurrentState = MOVE_TO_TUBE;
            }
        }
        else // Wait for command response
        {
            if ( "Scheduler::IDBottleCheck" == cmdName)
            {
                //dequeue the current bottle
                m_BottleChkFlag = true;
                if (m_IsAbortRecv)
                {
                    m_TasksAborted = true;
                    break;
                }
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    m_BottleSeq = 0;
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
                else
                {
                    m_BottleSeq++;
                }
            }
        }
        break;
    case MOVE_TO_TUBE:
        if (0 == m_MoveToTubeSeq)
        {
            mp_SchedulerThreadController->MoveRV(TUBE_POS);
            m_MoveToTubeSeq++;
        }
        else if(mp_SchedulerThreadController->IsRVRightPosition(TUBE_POS))
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_PRETEST_SUCCESS);
			m_MoveToTubeSeq = 0;
            if (m_IsAbortRecv)
            {
                m_TasksAborted = true;
                break;
            }
            emit TasksDone();
        }
        else
        {
            if ("Scheduler::RVReqMoveToRVPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    if (m_IsAbortRecv)
                    {
                        m_TasksAborted = true;
                        break;
                    }
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
        }
        break;
    default:
        break;
	}
}

void CProgramPreTest::ResetVarList()
{
    m_CurrentState = PRETEST_INIT;
    m_RTTempStartTime = 0;
    m_RTTempOffSeq = 0;
    m_RVPositioinChkSeq = 0;
    m_PressureChkSeq = 0;
    m_SetPrressureTime = 0;
    m_PressureCalibrationSeq = 0;
    m_PressureCalibrationCounter = 0;
    m_PressureDriftOffset = 0.0;
    m_PressureSealingChkSeq = 0;
    m_BottleChkFlag = true;
    m_MoveToTubeSeq = 0;
    m_IsLoged = 0;
    m_BottleSeq = 0;
    m_IsAbortRecv = false;
    m_TasksAborted = false;
}

}
