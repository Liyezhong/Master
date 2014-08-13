/****************************************************************************/
/*! \file RsTSensorErr3MinRetry.h
 *
 *  \brief RsTSensorErr3MinRetry class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ June 19, 2014
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
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler{

class SchedulerMainThreadController;

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_HTSensorErr_3Min_Retry
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
     */
    /****************************************************************************/
    CRsTSensorErr3MinRetry(SchedulerMainThreadController* SchedController);

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

signals:
    /****************************************************************************/
    /*!
     *  \brief	signal to release pressure
     *
     */
    /****************************************************************************/
    void ReleasePressure();

    /****************************************************************************/
    /*!
     *  \brief	signal to stop temperature control
     *
     */
    /****************************************************************************/
    void StopTempCtrlTSensor();

    /****************************************************************************/
    /*!
     *  \brief	signal to wait 3min
     *
     */
    /****************************************************************************/
    void Wait3MinTSensor();

    /****************************************************************************/
    /*!
     *  \brief	signal to start temperature control
     *
     */
    /****************************************************************************/
    void StartTempCtrlTSensor();

    /****************************************************************************/
    /*!
     *  \brief	signal to check the related device's temp
     *
     */
    /****************************************************************************/
    void CheckTSensorStatus();

    /****************************************************************************/
    /*!
     *  \brief	signal to indicate tasks are done
     *
     */
    /****************************************************************************/
    void TasksDone(bool);

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
     *  \brief  Definition/Declaration of function CheckTSensorCurrentStatus
     *
     *	\return bool
     */
    /****************************************************************************/
    bool CheckTSensorCurrentStatus();

private:
    SchedulerMainThreadController*  mp_SchedulerThreadController;    //!< Pointer to Scheduler Thread Controller
    QSharedPointer<QStateMachine>   mp_SchedulerMachine;    //!< stateMechine of RS_TSensorErr_3Min_Rety
    QSharedPointer<QState>          mp_Initial;             //!< Initial state
    QSharedPointer<QState>          mp_ReleasePressure;     //!< Release Pressure state
    QSharedPointer<QState>          mp_StopTempCtrl;        //!< Stop tempature control state
    QSharedPointer<QState>          mp_Wait3Min;            //!< wait 3min
    QSharedPointer<QState>          mp_CheckTemp;           //!< Check Device temp
    QSharedPointer<QState>          mp_StartTempCtrl;       //!< Start tempature control state
    qint64                          mp_WaitBeginTime;       //!< Wait 3Min begin time
};

}
#endif // RS_HEATING_ERR_3S_RETRY
