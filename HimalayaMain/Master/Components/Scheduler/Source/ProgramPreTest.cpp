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
#include "Scheduler/Include/ProgramPreTest.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdIDSealingCheck.h"

using namespace DeviceControl;
namespace Scheduler{

CProgramPreTest::CProgramPreTest(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Initial = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_TemperatureSensorsChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_Wait3SRTCurrent = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RTTempCtrlOff = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RVPositionChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
	mp_PressureCalibration = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_PressureSealingChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_BottlesChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_MoveToTube = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Initial.data());
    mp_Initial->addTransition(this, SIGNAL(CleaningMoveToTube()), mp_MoveToTube.data());

    mp_Initial->addTransition(this, SIGNAL(TemperatureSensorsChecking()), mp_TemperatureSensorsChecking.data());
    mp_TemperatureSensorsChecking->addTransition(this, SIGNAL(Wait3SecondsRTCurrent()), mp_Wait3SRTCurrent.data());
    mp_Wait3SRTCurrent->addTransition(this, SIGNAL(RTTemperatureControlOff()), mp_RTTempCtrlOff.data());
    mp_RTTempCtrlOff->addTransition(this,SIGNAL(RVPositionChecking()), mp_RVPositionChecking.data());
	mp_RVPositionChecking->addTransition(this, SIGNAL(PressureCalibration()), mp_PressureCalibration.data());
    mp_PressureCalibration->addTransition(this,SIGNAL(PressureSealingChecking()), mp_PressureSealingChecking.data());
    mp_PressureSealingChecking->addTransition(this, SIGNAL(BottlesChecking()), mp_BottlesChecking.data());
    mp_BottlesChecking->addTransition(this,SIGNAL(MoveToTube()), mp_MoveToTube.data());
    mp_MoveToTube->addTransition(this, SIGNAL(TasksDone()), mp_Initial.data());

    // Start up state machine
    mp_StateMachine->start();

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
}

CProgramPreTest::~CProgramPreTest()
{
    mp_StateMachine->stop();
}

CProgramPreTest::StateList_t CProgramPreTest::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = PRETEST_UNDEF;

    if (statesList.contains(mp_Initial.data()))
    {
        currentState = PRETEST_INIT;
    }
    else if (statesList.contains(mp_TemperatureSensorsChecking.data()))
    {
        currentState = TEMPSENSORS_CHECKING;
    }
    else if (statesList.contains(mp_Wait3SRTCurrent.data()))
    {
        currentState = WAIT3S_RT_CURRENT;
    }
    else if (statesList.contains(mp_RTTempCtrlOff.data()))
    {
        currentState = RT_TEMCTRL_OFF;
    }
    else if (statesList.contains(mp_RVPositionChecking.data()))
    {
        currentState = RV_POSITION_CHECKING;
    }
    else if(statesList.contains(mp_PressureCalibration.data()))
    {
        currentState = PRESSURE_CALIBRATION;
    }
    else if(statesList.contains(mp_PressureSealingChecking.data()))
    {
        currentState = PRESSURE_SEALING_CHECKING;
    }
    else if(statesList.contains(mp_BottlesChecking.data()))
    {
        currentState = BOTTLES_CHECKING;
    }
    else if (statesList.contains(mp_MoveToTube.data()))
    {
        currentState = MOVE_TO_TUBE;
    }

    return currentState;
}

void CProgramPreTest::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
	qreal currentPressure = 0.0;

    qint64 now = 0;
    ReportError_t reportError1;
    memset(&reportError1, 0, sizeof(reportError1));
    ReportError_t reportError2;
    memset(&reportError2, 0, sizeof(reportError2));
    ReportError_t reportError3;
    memset(&reportError3, 0, sizeof(reportError3));
    ReportError_t reportError4;
    memset(&reportError4, 0, sizeof(reportError4));

	switch (currentState)
	{
    case PRETEST_INIT:
        if(mp_SchedulerThreadController->IsCleaningProgram())
        {
            emit CleaningMoveToTube();
        }
        else
        {
            emit TemperatureSensorsChecking();
            m_RTTempStartTime = QDateTime::currentMSecsSinceEpoch();
        }
        break;
    case TEMPSENSORS_CHECKING:
        if (true == mp_SchedulerThreadController->GetHeatingStrategy()->CheckTemperatureSenseorsStatus())
        {
            mp_SchedulerThreadController->LogDebug("Pre-Test: Temperature checking of sensors passed");
            emit Wait3SecondsRTCurrent();
        }
        else
        {
            // Do nothing - Heating Strategy has handled all related error cases.
        }
        break;
    case WAIT3S_RT_CURRENT:
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_RTTempStartTime) >= 3*1000)
        {
            emit RTTemperatureControlOff();
        }
        else
        {
            reportError1 = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_BOTTOM);
            reportError2 = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_SIDE);
            reportError3 = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_TOP);
            reportError4 = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_BOTTOM);
            if (reportError1.instanceID != 0 && (now-reportError1.errorTime.toMSecsSinceEpoch()) <= 3*1000)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_RETORT_BOTTOM_HEATING_ELEMENT_FAILED);
            }
            if (reportError2.instanceID != 0 && (now-reportError2.errorTime.toMSecsSinceEpoch()) <= 3*1000)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_RETORT_SIDETOP_HEATING_ELEMENT_FAILED);
            }
            if (reportError3.instanceID != 0 && (now-reportError3.errorTime.toMSecsSinceEpoch()) <= 3*1000)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_WAXBATH_TOP_HEATINGPAD_CURRENT_OUTOFRANGE);
            }
            if (reportError4.instanceID != 0 && (now-reportError4.errorTime.toMSecsSinceEpoch()) <= 3*1000)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE);
            }
        }
    case RT_TEMCTRL_OFF:
        if (0 == m_RTTempOffSeq)
        {
            ReturnCode_t ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTSide");
            if (DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                m_RTTempOffSeq = 1;
            }
            else
            {
                mp_SchedulerThreadController->SendOutErrMsg(ret); // Send out error message
            }
        }
        else
        {
            ReturnCode_t ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTBottom");
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_RTTempOffSeq = 0;
                emit RVPositionChecking();
            }
            else
            {
                mp_SchedulerThreadController->SendOutErrMsg(ret); // Send out error mesage
            }
        }
        break;
    case RV_POSITION_CHECKING:
        if (0 == m_RVPositioinChkSeq)
        {
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdRVReqMoveToInitialPosition(500, mp_SchedulerThreadController));
            m_RVPositioinChkSeq++;
        }
        else
        {
            if ("Scheduler::RVReqMoveToInitialPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    mp_SchedulerThreadController->LogDebug("Pre-Test: RVReqMoveToInitialPosition passed");
                    m_RVPositioinChkSeq = 0; //reset to initial value
                    emit PressureCalibration();
                }
                else
                {
                    m_RVPositioinChkSeq = 0; //reset to initial value
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
            else
            {
                // Do nothing, just wait for the command response.
            }
        }
        break;
    case PRESSURE_CALIBRATION:
        if (0 == m_PressureCalibrationSeq)
        {
            mp_SchedulerThreadController->LogDebug(QString("Pre-Test: Enter Pressure Calibration"));
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
            m_PressureCalibrationSeq++;
        }
        else if (1 == m_PressureCalibrationSeq)
        {
            if ("Scheduler::ALReleasePressure" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_PressureCalibrationSeq++;
                    m_ReleasePressureTime = QDateTime::currentMSecsSinceEpoch();
                }
                else
                {
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
        }
        else if (2 == m_PressureCalibrationSeq)
        {
            if ((QDateTime::currentMSecsSinceEpoch() - m_ReleasePressureTime) >= 20*1000)
            {
                m_PressureCalibrationSeq++;
            }
            else
            {
                // Do nothing, just wait for time out
            }
        }
        else if (3 == m_PressureCalibrationSeq)
        {
            currentPressure = mp_SchedulerThreadController->GetSchedCommandProcessor()->ALGetRecentPressure();
            mp_SchedulerThreadController->LogDebug(QString("Pre-Test: Pressure Calibration, current pressure is :%1f").arg(currentPressure));
            if (qAbs(currentPressure) > 1.5) //Reitry, at most 3 times
            {
                m_PressureCalibrationSeq = 0;
                m_PressureCalibrationCounter++;
                if (3 == m_PressureCalibrationCounter)
                {
                    mp_SchedulerThreadController->LogDebug("Pre-Test: Pressure Calibration failed");
                    mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_PRESSURESENSOR_PRECHECK_FAILED);
                }
                else
                {
                    mp_SchedulerThreadController->LogDebug("Pre-Test: Pressure Calibration,  pressure value is too large, retry");
                }
            }
            else if (qAbs(currentPressure) < 0.2) // Calibration is Not needed
            {
                mp_SchedulerThreadController->LogDebug("Pre-Test: Pressure Calibration passed");
                m_PressureCalibrationSeq = 0;
                emit PressureSealingChecking();
            }
            else //offset the calibration
            {
                m_PressureDriftOffset = m_PressureDriftOffset + currentPressure;
                mp_SchedulerThreadController->GetSchedCommandProcessor()->ALSetPressureDrift(m_PressureDriftOffset);
                m_PressureCalibrationSeq = 0;
            }
        }
        break;
    case PRESSURE_SEALING_CHECKING:
        if (0 == m_PressureSealingChkSeq)
        {
            CmdIDSealingCheck* cmd = new CmdIDSealingCheck(500, mp_SchedulerThreadController);
            cmd->SetThresholdPressure(5.0);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
            m_PressureSealingChkSeq++;
        }
        else
        {
            if ("Scheduler::IDSealingCheck" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    mp_SchedulerThreadController->LogDebug("Pre-Test: IDSealingCheck passed");
					m_PressureSealingChkSeq = 0;
                    emit BottlesChecking();
                }
                else
                {
                    mp_SchedulerThreadController->LogDebug(QString("Pre-Test, Return code is: %1").arg(retCode));
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
            else
            {
                // Do nothing, just wait for the command response.
            }
        }
        break;
    case BOTTLES_CHECKING:
        if (true == m_BottleChkFlag)  // Send out IDBottleCheck command
        {
            if (mp_SchedulerThreadController->BottleCheck(m_BottleSeq))
            {
                m_BottleChkFlag = false;
            }
            else // all the bottle check (for 16 bottles) has been done
            {
                mp_SchedulerThreadController->LogDebug("Pre-Test: IDBottleCheck passed");
                m_BottleSeq = 0; //reset
                emit MoveToTube();
            }
        }
        else // Wait for command response
        {
            if ( "Scheduler::IDBottleCheck" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    m_BottleSeq = 0; //reset
                    m_BottleChkFlag = true; //reset
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
                else
                {
                    //dequeue the current bottle
                    m_BottleSeq++;
                    m_BottleChkFlag = true;
                }
            }
        }
        break;
    case MOVE_TO_TUBE:
        if (0 == m_MoveToTubeSeq)
        {
            mp_SchedulerThreadController->MoveRV(0);
            m_MoveToTubeSeq++;
        }
        else if(mp_SchedulerThreadController->IsRVRightPosition(0))
        {
            mp_SchedulerThreadController->LogDebug("Pre-Test: Moving to tube passed");
            mp_SchedulerThreadController->LogDebug("Pre-Test Done");
			m_MoveToTubeSeq = 0;
            emit TasksDone();
        }
        else
        {
            if ("Scheduler::RVReqMoveToRVPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
        }
        break;
    default:
		mp_SchedulerThreadController->LogDebug("Pre-Test: Get the default branch");
        break;
	}
}

}
