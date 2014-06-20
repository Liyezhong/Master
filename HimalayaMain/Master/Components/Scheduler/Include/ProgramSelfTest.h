/****************************************************************************/
/*! \file ProgramSelfTest.h
 *
 *  \brief CProgramSelfTest class definition.
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
#ifndef PROGRAM_SELF_TEST_H
#define PROGRAM_SELF_TEST_H

#include <QStateMachine>
#include <QSharedPointer>

namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Program running State machine for Self Test 
 */
/****************************************************************************/
class  CProgramSelfTest : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of CProgramSelfTest
     *
     *  \param none
     *
     */
    /****************************************************************************/
    CProgramSelfTest();

    /****************************************************************************/
    /*!
     *  \brief  desctruction of CProgramSelfTest
     */
    /****************************************************************************/
    ~CProgramSelfTest();


    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for Self Test 
     *
     *  \param  flag = bool
	 *
     *  \return void
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(bool flag);

signals:

    /****************************************************************************/
    /*!
     *  \brief Signal for level sensor temperature checking
     *
     */
    /****************************************************************************/
    void LevelSensorChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal for Rotary Valve position checking
     *
     */
    /****************************************************************************/
    void RVPositionChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal for pressure checking
     *
     */
    /****************************************************************************/
    void PressureChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal for sealing checking
     *
     */
    /****************************************************************************/
    void SealingChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal fo current status checking
     *
     */
    /****************************************************************************/
    void CurrentChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal fo bottles status checking
     *
     */
    /****************************************************************************/
    void BottlesChecking();


    /****************************************************************************/
    /*!
     *  \brief Signal for tasks done 
     *
     */
    /****************************************************************************/
    void TasksDone(bool);

private:
    QSharedPointer<QStateMachine>   mp_SchedulerMachine;    //!< State machine for Pre-Test
    QSharedPointer<QState> mp_Initial;                      //!< Initial state
    QSharedPointer<QState> mp_LevelSensorChecking;          //!< Level Sensor temperature checking state
    QSharedPointer<QState> mp_RVPositionChecking;           //!< Rotary Valve position checking state
    QSharedPointer<QState> mp_PressureChecking;             //!< Pressure checking state
    QSharedPointer<QState> mp_SealingChecking;              //!< Sealing checking state
    QSharedPointer<QState> mp_CurrentChecking;              //!< Current checking state
    QSharedPointer<QState> mp_BottlesChecking;              //!< Bottle checking state

    //!< state list of the state machine
    typedef enum
    {
        PRETEST_UNDEF,
        PRETEST_INIT,
        LEVELSENSOR_CHECKING,
        RV_POSITION_CHECKING,
        PRESSURE_CHECKING,
        SEALING_CHECKING,
        BOTTLES_CHECKING
    } StateList_t;
private:
    /****************************************************************************/
    /*!
     *  \brief  Get current state
     *
     *  \param  void
     *
     *	\return StateList_t
     */
    /****************************************************************************/
    StateList_t GetCurrentState(QSet<QAbstractState*> statesList);
};
}
#endif // PROGRAM_SELF_TEST_H
