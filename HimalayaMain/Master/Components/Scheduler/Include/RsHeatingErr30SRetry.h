/****************************************************************************/
/*! \file RsHeatingErr3SRetry.h
 *
 *  \brief CRsHeatingErr3SRetry class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ June 6th, 2014
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
#ifndef RS_HEATING_ERR_30S_RETRY_H
#define RS_HEATING_ERR_30S_RETRY_H
#include <QSharedPointer>
#include <QStateMachine>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"


namespace Scheduler{

class SchedulerMainThreadController;

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_HeatingErr_30S_Retry
 */
/****************************************************************************/
class  CRsHeatingErr30SRetry : public QObject
{
    Q_OBJECT

    //!< state list of the state machine
    typedef enum
    {
        UNDEF,
        SHUTDOWN_FAILD_HEATER,
        WAIT_FOR_10SECONDS,
        RESTART_FAILED_HEATER,
        CHECK_MODULE_CURRENT
    } StateList_t;

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of class CRsHeatingErr30SRetry
     *
     *  \param SchedController = pointer to SchedulerMainThreadController
     *
     */
    /****************************************************************************/
    CRsHeatingErr30SRetry(SchedulerMainThreadController* SchedController);

    /****************************************************************************/
    /*!
     *  \brief  desctruction CRsHeatingErr3SRetry
     */
    /****************************************************************************/
    ~CRsHeatingErr30SRetry();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetCurrentState
     *
     *  \param statesList = QSet<QAbstractState*>
     *
     *	\return StateList_t
     */
    /****************************************************************************/
    StateList_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for RS_Standby_WithTissue
     *
     *  \param void
     *
     */
    /****************************************************************************/
    void HandleWorkFlow();

signals:
    /****************************************************************************/
    /*!
     *  \brief	signal to wait for 3 seconds
     *
     */
    /****************************************************************************/
    void WaitFor10Seconds();

    /****************************************************************************/
    /*!
     *  \brief	signal to restart failed heater
     *
     */
    /****************************************************************************/
    void RestartFailedHeater();

    /****************************************************************************/
    /*!
     *  \brief  Signal for checking RTBottom temperature
     *
     */
    /****************************************************************************/
    void CheckTempModuleCurrernt();

    /****************************************************************************/
    /*!
     *  \brief	signal to retry above 3 steps
     *
     */
    /****************************************************************************/
    void Retry();


    /****************************************************************************/
    /*!
     *  \brief	signal to indicate tasks are done
     *
     */
    /****************************************************************************/
    void TasksDone(bool);
private:
    SchedulerMainThreadController* mp_SchedulerController;  //!< Pointer to SchedulerMainThreadController
    QSharedPointer<QStateMachine>  mp_StateMachine;         //!< State machine for RS_Standby_WithTissue
    QSharedPointer<QState> mp_ShutdownFailedHeater;         //!< Shutdown failed heater state
    QSharedPointer<QState> mp_WaitFor10Seconds;              //!< Wait for 3 seconds state
    QSharedPointer<QState> mp_RestartFailedHeater;          //!< Restart failed heater
    QSharedPointer<QState> mp_CheckTempModuleCurrent;       //!< Check current of the temperature module
    qint64 m_ShutdownHeaterTime;                            //!< Time of shutting down heater
    qint64  m_StartTime;                                    //!< Start up time
    quint32 m_Counter;                                      //!< counter for retry times
};
}
#endif // RS_HEATING_ERR_3S_RETRY
