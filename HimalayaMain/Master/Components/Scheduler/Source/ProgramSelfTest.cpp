/****************************************************************************/
/*! \file ProgramSelfTest.cpp
 *
 *  \brief CPorogramSelfTest class definition.
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
#include "Scheduler/Include/ProgramSelfTest.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdIDSealingCheck.h"

using namespace DeviceControl;
namespace Scheduler{

CProgramSelfTest::CProgramSelfTest(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Initial = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_TemperatureSensorsChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RTTempCtrlOff = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RVPositionChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_PressureChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_SealingChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_BottlesChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Initial.data());
    mp_Initial->addTransition(this, SIGNAL(TemperatureSensorsChecking()), mp_TemperatureSensorsChecking.data());
    mp_TemperatureSensorsChecking->addTransition(this, SIGNAL(CurrentVoltageChecking()), mp_RTTempCtrlOff.data());
    mp_RTTempCtrlOff->addTransition(this,SIGNAL(RVPositionChecking()), mp_RVPositionChecking.data());
    mp_RVPositionChecking->addTransition(this, SIGNAL(PressureChecking()), mp_PressureChecking.data());
    mp_PressureChecking->addTransition(this, SIGNAL(SealingChecking()), mp_SealingChecking.data());
    mp_SealingChecking->addTransition(this, SIGNAL(BottlesChecking()), mp_BottlesChecking.data());
    mp_BottlesChecking->addTransition(this,SIGNAL(TasksDone(bool)), mp_Initial.data());


    // For error cases
    mp_TemperatureSensorsChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_RTTempCtrlOff->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_RVPositionChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_PressureChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_SealingChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());

    // Start up state machine
    mp_StateMachine->start();

    m_RTTempOffSeq = 0;
    m_RVPositioinChkSeq = 0;
    m_PressureChkSeq = 0;
    m_SetPrressureTime = 0;
    m_BottleChkFlag = true;
}

CProgramSelfTest::~CProgramSelfTest()
{
    mp_StateMachine->stop();
}

CProgramSelfTest::StateList_t CProgramSelfTest::GetCurrentState(QSet<QAbstractState*> statesList)
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
    else if (statesList.contains(mp_RTTempCtrlOff.data()))
    {
        currentState = RT_TEMCTRL_OFF;
    }
    else if (statesList.contains(mp_RVPositionChecking.data()))
    {
        currentState = RV_POSITION_CHECKING;
    }
    else if(statesList.contains(mp_PressureChecking.data()))
    {
        currentState = PRESSURE_CHECKING;
    }
    else if(statesList.contains(mp_SealingChecking.data()))
    {
        currentState = SEALING_CHECKING;
    }
    else if(statesList.contains(mp_BottlesChecking.data()))
    {
        currentState = BOTTLES_CHECKING;
    }

    return currentState;
}

void CProgramSelfTest::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());

	switch (currentState)
	{
    case PRETEST_INIT:
        emit TemperatureSensorsChecking();
        break;
    case TEMPSENSORS_CHECKING:
        if (true == mp_SchedulerThreadController->GetHeatingStrategy()->CheckTemperatureSenseorsStatus())
        {
            emit RTTemperatureControlOff();
        }
        else
        {
            // Do nothing - Heating Strategy has handled all related error cases.
        }
        break;
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
                    emit PressureChecking();
                }
                else
                {
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
            else
            {
                // Do nothing, just wait for the command response.
            }
        }
        break;
    case PRESSURE_CHECKING:
        if (0 == m_PressureChkSeq)
        {
            RVPosition_t sealPos = mp_SchedulerThreadController->GetSealingPosition();
            CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, mp_SchedulerThreadController);
            cmd->SetRVPosition(sealPos);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
            m_PressureChkSeq++;
        }
        else if (1 == m_PressureChkSeq)
        {
            if ("Scheduler::RVReqMoveToRVPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_PressureChkSeq++;
                }
                else
                {
                   mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
            else
            {
                // Do nothing, just wait for the command response.
            }
        }
        else if (2 == m_PressureChkSeq)
        {
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALPressure(500, mp_SchedulerThreadController));
            m_SetPrressureTime = QDateTime::currentMSecsSinceEpoch();
            m_PressureChkSeq++;
        }
        else if (3 == m_PressureChkSeq)
        {
            if ("Scheduler::ALPressure" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_PressureChkSeq++;
                }
                else
                {
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
            else
            {
                // Do nothing, just wait for the command response
            }
        }
        else if (4 == m_PressureChkSeq) // Check if building pressure reaches 30KPa(5KPa drift) in 30 seconds.
        {
            if (std::abs(mp_SchedulerThreadController->GetRecentPressure()-30.0) >= 5.0)
            {
                qint64 now = QDateTime::currentMSecsSinceEpoch();
                if ((now - m_SetPrressureTime) > 30*1000) // time out 30 seconds
                {
                    mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_PRESSURESENSOR_PRECHECK_FAILED);
                }
            }
            else
            {
                m_SetPrressureTime = QDateTime::currentMSecsSinceEpoch();
                m_PressureChkSeq++;
            }
        }
        else if (5 == m_PressureChkSeq) // Pressure check - threshold time 5 seconds
        {
            qint64 now = QDateTime::currentMSecsSinceEpoch();
            if ((now - m_SetPrressureTime) <= 5*1000)
            {
                if (std::abs(mp_SchedulerThreadController->GetRecentPressure()-30.0) >= 5.0)
                {
                   mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_PRESSURESENSOR_PRECHECK_FAILED);
                }
                else
                {
                    // Do nothing, just wait for next tick
                }
            }
            else
            {
                emit SealingChecking();
            }
        }
        break;
    case SEALING_CHECKING:
        if (0 == m_SealingChkSeq)
        {
            CmdIDSealingCheck* cmd = new CmdIDSealingCheck(500, mp_SchedulerThreadController);
            cmd->SetThresholdPressure(5.0);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
        }
        else
        {
            if ("Scheduler::IDSealingCheck" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    emit BottlesChecking();
                }
                else
                {
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
            if (mp_SchedulerThreadController->BottleCheck())
            {
                m_BottleChkFlag = false;
            }
            else
            {
                emit TasksDone(true);
            }
        }
        else // Wait for command response
        {
            if ( "Scheduler::IDBottleCheck" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
                else
                {
                    m_BottleChkFlag = true;
                }
            }
        }
        break;
    default:
        break;
	}
}

}
