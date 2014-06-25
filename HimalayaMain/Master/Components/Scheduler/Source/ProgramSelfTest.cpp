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
    mp_PressureSealingChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_BottlesChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Initial.data());
    mp_Initial->addTransition(this, SIGNAL(TemperatureSensorsChecking()), mp_TemperatureSensorsChecking.data());
    mp_TemperatureSensorsChecking->addTransition(this, SIGNAL(RTTemperatureControlOff()), mp_RTTempCtrlOff.data());
    mp_RTTempCtrlOff->addTransition(this,SIGNAL(RVPositionChecking()), mp_RVPositionChecking.data());
    mp_RVPositionChecking->addTransition(this, SIGNAL(PressureSealingChecking()), mp_PressureSealingChecking.data());
    mp_PressureSealingChecking->addTransition(this, SIGNAL(BottlesChecking()), mp_BottlesChecking.data());
    mp_BottlesChecking->addTransition(this,SIGNAL(TasksDone(bool)), mp_Initial.data());


    // For error cases
    mp_TemperatureSensorsChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_RTTempCtrlOff->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_RVPositionChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_PressureSealingChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());

    // Start up state machine
    mp_StateMachine->start();

    m_RTTempOffSeq = 0;
    m_RVPositioinChkSeq = 0;
    m_PressureChkSeq = 0;
    m_SetPrressureTime = 0;
    m_PressureSealingChkSeq = 0;
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
    else if(statesList.contains(mp_PressureSealingChecking.data()))
    {
        currentState = PRESSURE_SEALING_CHECKING;
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
                    emit PressureSealingChecking();
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
