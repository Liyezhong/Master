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

namespace Scheduler{

CProgramSelfTest::CProgramSelfTest(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Initial = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_TemperatureSensorsChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RVPositionChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_PressureChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_SealingChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_BottlesChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_CurrentVoltageChecking = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Initial.data());
    mp_Initial->addTransition(this, SIGNAL(TemperatureSensorsChecking()), mp_TemperatureSensorsChecking.data());
    mp_TemperatureSensorsChecking->addTransition(this, SIGNAL(CurrentVoltageChecking()), mp_CurrentVoltageChecking.data());
    mp_CurrentVoltageChecking->addTransition(this,SIGNAL(RVPositionChecking()), mp_Initial.data());
    mp_RVPositionChecking->addTransition(this, SIGNAL(PressureChecking()), mp_PressureChecking.data());
    mp_PressureChecking->addTransition(this, SIGNAL(SealingChecking()), mp_SealingChecking.data());
    mp_SealingChecking->addTransition(this, SIGNAL(BottlesChecking()), mp_BottlesChecking.data());
    mp_BottlesChecking->addTransition(this,SIGNAL(TasksDone(bool)), mp_CurrentVoltageChecking.data());


    // For error cases
    mp_TemperatureSensorsChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_CurrentVoltageChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_RVPositionChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_PressureChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_SealingChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());

    // Start up state machine
    mp_StateMachine->start();

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
    else if (statesList.contains(mp_CurrentVoltageChecking.data()))
    {
        currentState = CURRENTVOLTAGE_CHECKING;
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

void CProgramSelfTest::HandleWorkFlow()
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
            emit CurrentVoltageChecking();
        }
        break;
    case CURRENTVOLTAGE_CHECKING:
        if (true == mp_SchedulerThreadController->GetSchedCommandProcessor()->GetCurrentVoltageStatus())
        {
            emit RVPositionChecking();
        }
        else
        {
            emit TasksDone(false);
        }
        break;
    case RV_POSITION_CHECKING:
        emit PressureChecking();
        break;
    case PRESSURE_CHECKING:
        emit SealingChecking();
        break;
    case SEALING_CHECKING:
        emit BottlesChecking();
        break;
    case BOTTLES_CHECKING:
        emit CurrentVoltageChecking();
        break;
    default:
        break;
	}
}

}
