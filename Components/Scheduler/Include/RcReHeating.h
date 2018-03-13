/****************************************************************************/
/*! \file RcReHeating.h
 *
 *  \brief CRcReHeating class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Sep 18th, 2014
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

#ifndef RCREHEATING_H
#define RCREHEATING_H

#include <QStateMachine>
#include <QSharedPointer>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler{

#define RV_SENSOR2_TEMP (40)                    //!< RV sensor2 temperature
#define WAIT_RV_SENSOR2_TEMP (3000)             //!< Wait RV Sensor2 time
#define WAIT_PARAFFIN_TEMP_TIME (900)           //!< Wait paraffin timeout

class SchedulerMainThreadController;
class CSchedulerStateMachine;
/****************************************************************************/
/*!
 * \brief Program running State machine for RcReheating
 */
/****************************************************************************/
class CRcReHeating : public QObject
{
#ifdef GOOGLE_MOCK
    friend class TestRcReheating;
#endif
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  conctruction of CRcReHeating
     *  \param  SchedController = SchedulerMainThreadController
     *  \param  StateMachine = CSchedulerStateMachine
     */
    /****************************************************************************/
    CRcReHeating(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine);

    /****************************************************************************/
    /*!
     *  \brief  desconctruction of CRcReHeating
     */
    /****************************************************************************/
    ~CRcReHeating();

    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for ReHeating
     *  \param  cmdName - command name
     *  \param  retCode - return code
     *  \return void
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief  function of StartHeatingWithScenario
     *  \param  Scenario = qint32
     *  \return void
     */
    /****************************************************************************/
    inline void SetScenario(quint32 Scenario){m_LastScenario = Scenario;}

    /****************************************************************************/
    /*!
     *  \brief  function of SetStationID
     *  \param  StationID - QString
     */
    /****************************************************************************/
    inline void SetStationID(const QString& StationID) { m_LastStationID = StationID;}

    /****************************************************************************/
    /*!
     *  \brief  function of Set5MinTimeOut
     *  \param  value = bool
     *  \return void
     */
    /****************************************************************************/
    inline void Set5MinTimeOut(bool value)  {m_Is5MinTimeOut = value;}

private:
    /****************************************************************************/
    /*!
     *  \brief  function of HandleInint
     */
    /****************************************************************************/
    void HandleInint();

    /****************************************************************************/
    /*!
     *  \brief  function of HandlePressureCalibration
     *  \param  cmdName = QString
     */
    /****************************************************************************/
    void HandlePressureCalibration(const QString &cmdName);

    /****************************************************************************/
    /*!
     *  \brief  function of StartHeatingWithScenario
     *  \return bool
     */
    /****************************************************************************/
    bool StartHeatingSensor();

    /****************************************************************************/
    /*!
     *  \brief  function of CheckTheTemperature
     */
    /****************************************************************************/
    void CheckTheTemperature();

    /****************************************************************************/
    /*!
     *  \brief  function of GetRvPosition
     *  \param  cmdName = QString
     *  \param  retCode = DeviceControl::ReturnCode_t
     */
    /****************************************************************************/
    void GetRvPosition(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief  function of ProcessDraingin
     *  \param  cmdName = QString
     *  \param  retCode = DeviceControl::ReturnCode_t
     */
    /****************************************************************************/
    void ProcessDraining(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief  conctruction of CRcReHeating
     *  \param  rhs = CRcReHeating
     */
    /****************************************************************************/
    CRcReHeating(const CRcReHeating& rhs);
    /****************************************************************************/
    /*!
     *  \brief  operator= conctruction of CRcReHeating
     *  \param  rhs = CRcReHeating
     */
    /****************************************************************************/
    CRcReHeating& operator=(const CRcReHeating& rhs);

private:
    /****************************************************************************/
    /*!
     * \brief The enum of step of ReHeating step
     */
    /****************************************************************************/
    typedef enum{
        UNDEF,
        INIT_STATE,
        PRESSURE_CALIBRATION,
        START_TEMPERATURE,
        CHECK_TEMPERATURE,
        GET_RV_POSOTION,
        BEGIN_DRAIN
    }StateList_t;

private:
    SchedulerMainThreadController*  mp_SchedulerThreadController;       //!< Pointer to Scheduler Thread Controller
    CSchedulerStateMachine*         mp_StateMachine;                    //!< pointer to scheduler state machine
    StateList_t                     m_CurrentStep;                      //!< the current step
    qint32                          m_LastScenario;                     //!< last scenario of power failure
    qint32                          m_StartReq;                         //!< count start command
    qint64                          m_StartHeatingTime;                 //!< the start heating time
    qint64                          m_StartPressureTime;                //!< the start pressure time
    bool                            m_Is5MinTimeOut;                    //!< flag to indicat if RC_ReHeating was triggered by 5 minutes timeout
    QString                         m_LastStationID;                    //!< last Station ID
    quint8                          m_CountTheEffectiveTemp;            //!< cout the effecitive temperature
    quint32                         m_PressureCalibrationSeq;           //!< Sequence of Pressure Calibration
    qreal                           m_PressureDriftOffset;              //!< Pressure calibration offset
    quint8                          m_PressureCalibrationCounter;       //!< Counter of Pressure Calibratoin
    qint64                          m_ReleasePressureTime;              //!< Time of releasing pressure
    quint8                          m_IsLoged;                          //!< Whether loged
    bool                            m_IsMeltingMsgSend;                 //!< flag for melting message sent or not
};

}

#endif
