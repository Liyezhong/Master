/****************************************************************************/
/*! \file RsTSensorErr3MinRetry.h
 *
 *  \brief RsTSensorErr3MinRetry class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ June 19th, 2014
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

#ifndef RS_TSENSOR_ERR_3MIN_RETRY_H
#define RS_TSENSOR_ERR_3MIN_RETRY_H

#include <QStateMachine>
#include <QSharedPointer>
#include <QCoreApplication>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler{

class SchedulerMainThreadController;
class CSchedulerStateMachine;

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_TSensorErr_3Min_Retry
 */
/****************************************************************************/
class  CRsTSensorErr3MinRetry : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of class CRsTSensorErr3MinRetry
     *  \param SchedController is pointer of class SchedulerMainThreadController
     *  \param StateMachine is pointer of class CSchedulerStateMachine
     */
    /****************************************************************************/
    CRsTSensorErr3MinRetry(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine);

    /****************************************************************************/
    /*!
     *  \brief  desctruction CRsTSensorErr3MinRetry
     */
    /****************************************************************************/
    ~CRsTSensorErr3MinRetry();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function HandleWorkFlow
     *  \param  cmdName is QString
     *  \param  retCode is ReturnCode_t
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
    //!< state list of the state machine
    typedef enum{
        RS_TSENSORERR3MINRETRY_UNDEF,
        RS_TSENSORERR3MINRETRY_INIT,
        RS_TSENSORERR3MINRETRY_RELEASEPRESSURE,
        RS_TSENSORERR3MINRETRY_STOPTEMPCTRL,
        RS_TSENSORERR3MINRETRY_WAIT3MIN,
        RS_TSENSORERR3MINRETRY_STARTTEMPCTRL,
        RS_TSENSORERR3MINRETRY_CHECKSTATUS
    }StateList_t;

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CheckTSensorCurrentStatus
     *
     *	\return bool
     */
    /****************************************************************************/
    bool CheckTSensorCurrentStatus();

private:
    SchedulerMainThreadController*  mp_SchedulerThreadController;    //!< Pointer to Scheduler Thread Controller
    CSchedulerStateMachine*         mp_SchedulerMachine;    //!< stateMechine of RS_TSensorErr_3Min_Rety
    StateList_t                     m_CurrentState;         //!< the current state
    qint64                          m_WaitBeginTime;       //!< Wait 3Min begin time
    QString m_Sender;
};

}
#endif // RS_HEATING_ERR_3S_RETRY
