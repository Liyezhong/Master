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

namespace Scheduler{

CProgramSelfTest::CProgramSelfTest()
{
    mp_SchedulerMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Initial = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_LevelSensorChecking = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_RVPositionChecking = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_PressureChecking = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_SealingChecking = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_CurrentChecking = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_BottlesChecking = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));

    mp_SchedulerMachine->setInitialState(mp_Initial.data());
    mp_Initial->addTransition(this, SIGNAL(LevelSensorChecking()), mp_LevelSensorChecking.data());
    mp_LevelSensorChecking->addTransition(this, SIGNAL(RVPositionChecking()), mp_RVPositionChecking.data());
    mp_RVPositionChecking->addTransition(this, SIGNAL(PressureChecking()), mp_PressureChecking.data());
    mp_PressureChecking->addTransition(this, SIGNAL(SealingChecking()), mp_SealingChecking.data());
    mp_SealingChecking->addTransition(this, SIGNAL(CurrentChecking()), mp_CurrentChecking.data());
    mp_CurrentChecking->addTransition(this,SIGNAL(BottlesChecking()), mp_Initial.data());
    mp_BottlesChecking->addTransition(this,SIGNAL(TasksDone(bool)), mp_Initial.data());

    //For error cases
    mp_LevelSensorChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_RVPositionChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_PressureChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_SealingChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_CurrentChecking->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());

}

CProgramSelfTest::~CProgramSelfTest()
{

}

CProgramSelfTest::StateList_t CProgramSelfTest::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = PRETEST_UNDEF;

    if (statesList.contains(mp_Initial.data()))
    {
        currentState = PRETEST_INIT;
    }
    else if (statesList.contains(mp_LevelSensorChecking.data()))
    {
        currentState = LEVELSENSOR_CHECKING;
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

void CProgramSelfTest::HandleWorkFlow(bool flag)
{
    if (false == flag)
    {
        emit TasksDone(false);
        return;
    }
    StateList_t currentState = this->GetCurrentState(mp_SchedulerMachine->configuration());

	switch (currentState)
	{
    case PRETEST_INIT:
        emit LevelSensorChecking();
        break;
    case LEVELSENSOR_CHECKING:
        emit RVPositionChecking();
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
        emit TasksDone(true);
        break;
    default:
        break;
	}
}

}
