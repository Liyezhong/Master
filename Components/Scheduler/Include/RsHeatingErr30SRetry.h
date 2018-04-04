/****************************************************************************/
/*! \file RsHeatingErr30SRetry.h
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
#include <QCoreApplication>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"


namespace Scheduler{

class SchedulerMainThreadController;
class CSchedulerStateMachine;

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
        HEATINGERROR_30SRetry_INIT,
        RELEASE_PRESSURE,
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
     *  \param StateMachine = pointer to CSchedulerStateMachine
     *
     */
    /****************************************************************************/
    CRsHeatingErr30SRetry(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine);

    /****************************************************************************/
    /*!
     *  \brief  desctruction CRsHeatingErr3SRetry
     */
    /****************************************************************************/
    ~CRsHeatingErr30SRetry();

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for RS_Standby_WithTissue
     *
     *  \param  cmdName - command name
     *  \param  retCode - return code
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

    /****************************************************************************/
    /*!
     *  \brief start up state machine
     */
    /****************************************************************************/
    void Start();

private:
    SchedulerMainThreadController* mp_SchedulerController;  //!< Pointer to SchedulerMainThreadController
    CSchedulerStateMachine*        mp_StateMachine;         //!< State machine for RS_HeatingErr_30SRetry
    StateList_t                    m_CurrentState;          //!< Current state
    qint64 m_ShutdownHeaterTime;                            //!< Time of shutting down heater
    qint64  m_StartTime;                                    //!< Start up time
    QString m_Sender;
};
}
#endif // RS_HEATING_ERR_3S_RETRY
