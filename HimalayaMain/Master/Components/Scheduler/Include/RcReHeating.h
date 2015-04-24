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
#define WAIT_RV_SENSOR2_TEMP (1800)             //!< Wait RV Sensor2 time
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
     *  \brief  function of StartHeatingWithScenario
     *  \return qint32
     */
    /****************************************************************************/
    inline quint32 GetScenario(){return m_LastScenario;}

    /****************************************************************************/
    /*!
     *  \brief  function of SetReagentID
     *  \param  ReagentID - QString
     */
    /****************************************************************************/
    inline void SetReagentID(const QString& ReagentID) { m_LastReagentID = ReagentID;}

    /****************************************************************************/
    /*!
     *  \brief  function of GetReagentID
     *  \return QString
     */
    /****************************************************************************/
    QString GetReagentID();

    /****************************************************************************/
    /*!
     *  \brief  function of SetNeedRunCleaning
     *  \param  value = bool
     *  \return void
     */
    /****************************************************************************/
    inline void SetNeedRunCleaning(bool value)  {m_IsNeedRunCleaning = value;}

    /****************************************************************************/
    /*!
     *  \brief  function of GetNeedRunCleaning
     *  \return bool
     */
    /****************************************************************************/
    inline bool GetNeedRunCleaning()    {return m_IsNeedRunCleaning;}

private:
    /****************************************************************************/
    /*!
     *  \brief  function of HandleInint
     */
    /****************************************************************************/
    void HandleInint();

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
        START_TEMPERATURE,
        CHECK_TEMPERATURE,
        GET_RV_POSOTION,
        BEGIN_DRAIN
    }StateList_t;

    /****************************************************************************/
    /*!
     * \brief The enum of step of reagent check
     */
    /****************************************************************************/
    typedef enum
    {
        FORCE_DRAIN,
        BUILD_VACUUM,
        MOVE_INITIALIZE_POSITION,
        MOVE_TUBEPOSITION,
        REALSE_PRESSURE
    }RS_REAGENTCHECK_t;

private:
    SchedulerMainThreadController*  mp_SchedulerThreadController;       //!< Pointer to Scheduler Thread Controller
    CSchedulerStateMachine*         mp_StateMachine;                    //!< pointer to scheduler state machine
    StateList_t                     m_CurrentStep;                      //!< the current step
    qint32                          m_LastScenario;                     //!< last scenario of power failure
    qint32                          m_StartReq;                         //!< count start command
    qint64                          m_StartHeatingTime;                 //!< the start heating time
    qint64                          m_StartPressureTime;                //!< the start pressure time
    bool                            m_IsNeedRunCleaning;                //!< is need enter cleaning program
    RS_REAGENTCHECK_t               m_RsReagentCheckStep;               //!< the Rs_ReagentCheck state step
    QString                         m_LastReagentID;                    //!< last reagent ID
};

}

#endif
