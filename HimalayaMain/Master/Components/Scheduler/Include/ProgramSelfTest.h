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
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler{

class SchedulerMainThreadController;

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
    CProgramSelfTest(SchedulerMainThreadController* SchedController);

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
     *  \param  cmdName - command name
     *  \param  retCode - return code
	 *
     *  \return void
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

signals:

    /****************************************************************************/
    /*!
     *  \brief Signal for checking the status of all temperature sensors
     *
     */
    /****************************************************************************/
    void TemperatureSensorsChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal for current and voltage status checking
     *
     */
    /****************************************************************************/
    void RTTemperatureControlOff();


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
    SchedulerMainThreadController *mp_SchedulerThreadController;    //!< Pointer to Scheduler Thread Controller
    QSharedPointer<QStateMachine>   mp_StateMachine;                //!< State machine for Pre-Test
    QSharedPointer<QState> mp_Initial;                              //!< Initial state
    QSharedPointer<QState> mp_TemperatureSensorsChecking;           //!< Temperature sensors status checking state
    QSharedPointer<QState> mp_RTTempCtrlOff;                        //!< Current checking state
    QSharedPointer<QState> mp_RVPositionChecking;                   //!< Rotary Valve position checking state
    QSharedPointer<QState> mp_PressureChecking;                     //!< Pressure checking state
    QSharedPointer<QState> mp_SealingChecking;                      //!< Sealing checking state
    QSharedPointer<QState> mp_BottlesChecking;                      //!< Bottle checking state


    //!< state list of the state machine
    typedef enum
    {
        PRETEST_UNDEF,
        PRETEST_INIT,
        TEMPSENSORS_CHECKING,
        RT_TEMCTRL_OFF,
        RV_POSITION_CHECKING,
        PRESSURE_CHECKING,
        SEALING_CHECKING,
        BOTTLES_CHECKING
    } StateList_t;
	
	quint32	m_RTTempOffSeq;											//!< Sequence of RT sensors temperature off
	quint32	m_RVPositioinChkSeq;									//!< Sequence of RV position checking 
    quint32	m_PressureChkSeq;										//!< Sequence of Pressure checking
    qint64  m_SetPrressureTime;                                     //!< Time for pressure checking
    quint32 m_SealingChkSeq;                                        //!< Sequence of Sealing checking
    bool    m_BottleChkFlag;                                        //!< Flag to indicate sending out command or geting command response.
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
