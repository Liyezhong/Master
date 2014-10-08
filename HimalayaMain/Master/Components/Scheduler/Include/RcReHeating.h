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

class SchedulerMainThreadController;
/****************************************************************************/
/*!
 * \brief Program running State machine for RcReheating
 */
/****************************************************************************/
class CRcReHeating : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  conctruction of CRcReHeating
     *  \param  SchedController = SchedulerMainThreadController
     */
    /****************************************************************************/
    CRcReHeating(SchedulerMainThreadController* SchedController);

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
     *  \brief  function of StartHeatingWithScenario
     *  \return bool
     */
    /****************************************************************************/
    bool StartHeatingSensor();

    /****************************************************************************/
    /*!
     *  \brief  function of CheckTheTemperature
     *  \return bool
     */
    /****************************************************************************/
    bool CheckTheTemperature();

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

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal for tasks done
     *  \param flag - bool
     *
     */
    /****************************************************************************/
    void TasksDone(bool flag);

    /****************************************************************************/
    /*!
     *  \brief Signal for tasks done
     *
     */
    /****************************************************************************/
    void SignalDrain();

    /****************************************************************************/
    /*!
     *  \brief Signal for tasks done
     *
     */
    /****************************************************************************/
    void SignalStopDrain();

private:
    /****************************************************************************/
    /*!
     * \brief The enum of step of ReHeating step
     */
    /****************************************************************************/
    typedef enum{
        START_TEMPERATURE,
        CHECK_TEMPERATURE,
        BEGIN_DRAIN
    }ReHeatingStep_t;

    SchedulerMainThreadController*  mp_SchedulerThreadController;       //!< Pointer to Scheduler Thread Controller
    ReHeatingStep_t                 m_RcReHeatingStep;                  //!< The step of special scenario
    qint32                          m_LastScenario;                     //!< last scenario of power failure
    qint32                          m_StartReq;                         //!< count start command
    qint64                          m_StartHeatingTime;                 //!< the start heating time
    bool                            m_IsNeedRunCleaning;                //!< is need enter cleaning program
    bool                            m_DrainIsOk;                        //!< is drain OK
};

}

#endif
