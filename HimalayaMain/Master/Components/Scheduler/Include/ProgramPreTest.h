/****************************************************************************/
/*! \file ProgramPreTest.h
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
#ifndef PROGRAM_PRE_TEST_H
#define PROGRAM_PRE_TEST_H

#include <QStateMachine>
#include <QSharedPointer>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Include/SchedulerStateMachineStates.h"

namespace Scheduler{

class SchedulerMainThreadController;

/****************************************************************************/
/*!
 * \brief Program running State machine for Pre Test
 */
/****************************************************************************/
class  CProgramPreTest : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of CProgramPreTest
     *  \param SchedController - SchedulerMainThreadController
     *
     */
    /****************************************************************************/
    CProgramPreTest(SchedulerMainThreadController* SchedController);

    /****************************************************************************/
    /*!
     *  \brief  desctruction of CProgramPreTest
     */
    /****************************************************************************/
    ~CProgramPreTest();

    /****************************************************************************/
    /*!
     *  \brief start up state machine
     */
    /****************************************************************************/
    void Start();

    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for Pre Test
     *
     *  \param  cmdName - command name
     *  \param  retCode - return code
	 *
     *  \return void
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief  Reset variable list in the state machine
     *
     */
    /****************************************************************************/
    void ResetVarList();

public slots:
    /****************************************************************************/
    /*!
     *  \brief  slot for receiving ABORT control command
     *
     */
    /****************************************************************************/
    void OnRecvAbort() { m_IsAbortRecv = true; }

signals:

    /****************************************************************************/
    /*!
    *  \brief Signal for start the RT temperature sensors
    *
    */
   /****************************************************************************/
   void RTTemperatureControlOn();

    /****************************************************************************/
    /*!
     *  \brief Signal for checking the status of all temperature sensors
     *
     */
    /****************************************************************************/
    void TemperatureSensorsChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal to wait to 3 seconds
     *
     */
    /****************************************************************************/
    void Wait3SecondsRTCurrent();

    /****************************************************************************/
    /*!
     *  \brief Signal to turn off RT Side and Bottom
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
     *  \brief Signal for pressure calibration
     *
     */
    /****************************************************************************/
    void PressureCalibration();


    /****************************************************************************/
    /*!
     *  \brief Signal for pressure checking
     *
     */
    /****************************************************************************/
    void PressureSealingChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal for bottles status checking
     *
     */
    /****************************************************************************/
    void BottlesChecking();

    /****************************************************************************/
    /*!
     *  \brief Signal for moving to tube position
     *
     */
    /****************************************************************************/
    void MoveToTube();

    /****************************************************************************/
    /*!
     *  \brief Signal for tasks done
     *
     */
    /****************************************************************************/
    void TasksDone();

    /****************************************************************************/
    /*!
     *  \brief signal for moving to tube when current program is cleaning program
     *
     */
    /****************************************************************************/
    void CleaningMoveToTube();

    /****************************************************************************/
    /*!
     *  \brief signal to indicate that tasks were aborted
     *
     */
    /****************************************************************************/
    void TasksAborted();

private:
    SchedulerMainThreadController *mp_SchedulerThreadController;    //!< Pointer to Scheduler Thread Controller
    QSharedPointer<QStateMachine>   mp_StateMachine;                //!< State machine for Pre-Test
    QSharedPointer<QState> mp_Initial;                              //!< Initial state
    QSharedPointer<QState> mp_RTTempCtrlOn;                         //!< start RT temperature
    QSharedPointer<QState> mp_TemperatureSensorsChecking;           //!< Temperature sensors status checking state
    QSharedPointer<QState> mp_Wait3SRTCurrent;                      //!< Wait for 3 seconds to see if current error was raised
    QSharedPointer<QState> mp_RTTempCtrlOff;                        //!< Turn off Retort Side and Bottom
    QSharedPointer<QState> mp_RVPositionChecking;                   //!< Rotary Valve position checking state
    QSharedPointer<QState> mp_PressureCalibration;                  //!< Pressure Calibration state
    QSharedPointer<QState> mp_PressureSealingChecking;              //!< Pressure test and sealing checking state
    QSharedPointer<QState> mp_BottlesChecking;                      //!< Bottle checking state
    QSharedPointer<QState> mp_MoveToTube;                           //!< Moving to tube state


    //!< state list of the state machine
    typedef enum
    {
        PRETEST_UNDEF,
        PRETEST_INIT,
        RT_TEMCTRL_ON,
        TEMPSENSORS_CHECKING,
        WAIT3S_RT_CURRENT,
        RT_TEMCTRL_OFF,
        RV_POSITION_CHECKING,
		PRESSURE_CALIBRATION,
        PRESSURE_SEALING_CHECKING,
        BOTTLES_CHECKING,
        MOVE_TO_TUBE
    } StateList_t;
	
    qint64  m_RTTempStartTime;                                      //!< Start time for turning on RT Tempeture control On
	quint32	m_RTTempOffSeq;											//!< Sequence of RT sensors temperature off
	quint32	m_RVPositioinChkSeq;									//!< Sequence of RV position checking 
    quint32	m_PressureChkSeq;										//!< Sequence of Pressure test and Sealing checking
    qint64  m_SetPrressureTime;                                     //!< Time for pressure checking
    quint32 m_PressureCalibrationSeq;                               //!< Sequence of Pressure Calibration
    qreal   m_PressureDriftOffset;                                  //!< Pressure calibration offset
    quint8  m_PressureCalibrationCounter;                           //!< Counter of Pressure Calibratoin
    qint64  m_ReleasePressureTime;                                  //!< Time of releasing pressure
    quint32 m_PressureSealingChkSeq;                                //!< Sequence of Sealing checking
    bool    m_BottleChkFlag;                                        //!< Flag to indicate sending out command or geting command response.
    quint32 m_BottleSeq;                                            //!< Sequence of the bottle
    quint32 m_MoveToTubeSeq;                                        //!< Sequence of moving to tube
    quint8  m_IsLoged;                                              //!< Whether loged
    bool    m_IsAbortRecv;                                          //!< Flag to indicate if CTRL_CMD_ABORT was received
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
#endif // PROGRAM_PRE_TEST_H
