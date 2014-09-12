/****************************************************************************/
/*! \file ProgramSelfTest.h
 *
 *  \brief CProgramSelfTest class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Aug 18th, 2014
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

#ifndef PROGRAM_SELF_TEST_H
#define PROGRAM_SELF_TEST_H

#include <QStateMachine>
#include <QSharedPointer>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

using namespace DeviceControl;

namespace Scheduler{

#define VALVE_1_INDEX (0)
#define VALVE_2_INDEX (1)
#define VALVE_STATE_OPEN (1)
#define VALVE_STATE_CLOSE (0)
#define HEATER_IGNORE (-1);
#define HEATER_UNDEF (0)
#define HEATER_220V  (1)
#define HEATER_110V  (2)
#define HEATER_DC    (3)
#define AUTO_SWITCH_DISABLE (0)
#define AUTO_SWITCH_ENABLE  (1)

class SchedulerMainThreadController;

/****************************************************************************/
/*!
 * \brief Program running State machine for Self Test
 */
/****************************************************************************/
class CProgramSelfTest : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  conctruction of CProgramSelfTest
     *  \param  SchedController = SchedulerMainThreadController
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
     *  \param  cmdName - command name
     *  \param  retCode - return code
     *  \return void
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief  whether the self test done
     *  \return bool
     */
    /****************************************************************************/
    bool IsSelfTestDone() { return m_IsSelfTestDone;   }

    /****************************************************************************/
    /*!
     *  \brief  send signal for self test
     *  \param  flag = bool
     *  \return void
     */
    /****************************************************************************/
    void SendSignalSelfTestDone(bool flag);


signals:
    /****************************************************************************/
    /*!
     *  \brief Signal for test the voltage of AC
     */
    /****************************************************************************/
    void SigACVoltageSwitch();

    /****************************************************************************/
    /*!
     *  \brief Signal for test the heating sensor of DC
     */
    /****************************************************************************/
    void SigDCHeating();

    /****************************************************************************/
    /*!
     *  \brief Signal for test the pressure sensor and pump and valve
     */
    /****************************************************************************/
    void SigPressureSensorPumpValve();

    /****************************************************************************/
    /*!
     *  \brief Signal for test the ac heating
     */
    /****************************************************************************/
    void SigACHeating();

    /****************************************************************************/
    /*!
     *  \brief Signal for tasks done
     */
    /****************************************************************************/
    void SigSelfTestDone(bool flag);

private:
    /****************************************************************************/
    /*!
     * \brief The enum of state
     */
    /****************************************************************************/
    typedef enum{
        SELFTEST_UNDF,
        SELFTEST_INIT,
        SELFTEST_AC_VOLTAGE,
        SELFTEST_DC_HEATING,
        SELFTEST_PRESSURE,
        SELFTEST_AC_HEATING
    }StateList_t;

    /****************************************************************************/
    /*!
     * \brief The enum of step of AC voltage
     */
    /****************************************************************************/
    typedef enum{
        SET_VOLTAGE_ASB3_AWITCH,
        SET_VOLTAGE_ASB5_AWITCH,
        START_HEATING_ACMODE,
        STOP_HEATING_ACMODE,
        CHECK_VOLTAGE_RANGE,
    }StepACVoltage_t;

    /****************************************************************************/
    /*!
     * \brief The enum of step of DC heating
     */
    /****************************************************************************/
    typedef enum{
        STARTHEATING_LATUBE1,
        STOPHEATING_LATUBE1,
        STARTHEATING_LATUBE2,
        STOPHEATING_LATBUBE2,
        STARTHEATING_LEVELSENSOR,
        STOPHEATING_LEVELSENSOR,
    }StepDCHeating_t;

    /****************************************************************************/
    /*!
     * \brief The enum of step of pressure pump valve
     */
    /****************************************************************************/
    typedef enum{
        START_PUMP,
        STOP_PUMP,
        START_VALVE1,
        STOP_VALVE1,
        START_VALVE2,
        STOP_VALVE2,
        START_EXHAUSTFAN,
        STOP_EXHAUSTFAN
    }StepPressurePumpValve_t;

    /****************************************************************************/
    /*!
     * \brief The enum of step of AC heating
     */
    /****************************************************************************/
    typedef enum{
        STARTHEATING_RETORTTOP,
        STOPHEATING_RETORTOP,
        STARTHEATING_RETORTBOTTOM,
        STOPHEATING_RETORTBOTTOM,
        STARTHEATING_OVENTOP,
        STOPHEATING_OVENTOP,
        STARTHEATING_OVENBOTTOM,
        STOPHEATING_OVENBOTTOM,
        STARTHEATING_RV,
        STOPHEATING_RV,
    }StepACHeating_t;

    /****************************************************************************/
    /*!
     *  \brief  Get current state
     *  \param  void
     *	\return StateList_t
     */
    /****************************************************************************/
    StateList_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for Self Test
     *  \param  cmdName - command name
     *  \param  retCode - return code
     *  \return void
     */
    /****************************************************************************/
    void HandleStateACVoltage(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for DC heating
     */
    /****************************************************************************/
    void HandleStateDCHeating();

    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for pressure pump valve
     *  \param  cmdName - command name
     *  \param  retCode - return code
     *  \return void
     */
    /****************************************************************************/
    void HandlePressure(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for AC heating
     */
    /****************************************************************************/
    void HandleStateACHeating();

private:
    SchedulerMainThreadController*  mp_SchedulerThreadController;       //!< Pointer to Scheduler Thread Controller
    QSharedPointer<QStateMachine>   mp_StateMachine;                    //!< State machine for Self-Test
    QSharedPointer<QState>          mp_Initial;                         //!< State of Initialize
    QSharedPointer<QState>          mp_StateACVoltage;                  //!< State of AC voltage test
    QSharedPointer<QState>          mp_StateDCHeating;                  //!< State of DC heating sensor
    QSharedPointer<QState>          mp_StatePressure;                   //!< State of Pressure
    QSharedPointer<QState>          mp_StateACHeating;                  //!< State of AC Heating
    StepACVoltage_t                 m_StateACVoltageStep;               //!< the step of AC voltage state
    StepDCHeating_t                 m_StateDCHeatingStep;               //!< the step of DC heating sensor
    StepPressurePumpValve_t         m_StatePressureStep;                //!< the step of pressure
    StepACHeating_t                 m_StateACHeatingStep;               //!< the step of AC heating
    qint64                          m_DelayBeginTime;                   //!< the delay start time
    quint8                          m_StartReq;                         //!< start requence
    quint8                          m_StateACVoltageStepCount;          //!< the number of AC voltage
    bool                            m_IsSelfTestDone;                   //!< whether self test done
};

}
#endif //PROGRAM_SELF_TEST_H
