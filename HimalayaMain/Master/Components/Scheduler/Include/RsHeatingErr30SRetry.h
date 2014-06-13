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
#ifndef RS_HEATING_ERR_3S_RETRY_H
#define RS_HEATING_ERR_3S_RETRY_H
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "ErrorHandlingSMBase.h"
#include <QStateMachine>


namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_HeatingErr_3S_Retry
 */
/****************************************************************************/
class  CRsHeatingErr30SRetry : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of class CRsHeatingErr4SRetry 
     *
     *  \param pStateMachine = pointer QStateMachine
     *  \param pParentState = pointer QState
     *
     */
    /****************************************************************************/
    CRsHeatingErr30SRetry(QStateMachine* pStateMachine, QState* pParentState);

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
     *	\return SchedulerStateMachine_t
     */
    /****************************************************************************/
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function OnHandleWorkFlow
     *
     *  \param flag = bool
     *
     */
    /****************************************************************************/
    void OnHandleWorkFlow(bool flag);

signals:
    /****************************************************************************/
    /*!
     *  \brief	signal to stop temperature control	 
     *
     */
    /****************************************************************************/
    void StopTempCtrl();

    /****************************************************************************/
    /*!
     *  \brief	signal to start temperature control 
     *
     */
    /****************************************************************************/
    void StartTempCtrl();

    /****************************************************************************/
    /*!
     *  \brief	signal to check the related device's status 
     *
     */
    /****************************************************************************/
    void CheckDevStatus();

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
    QState *mp_Initial;                 							//!< Initial state
    QState *mp_StopTempCtrl;										//!< Stop tempature control state
    QState *mp_StartTempCtrl;										//!< Start tempature control state
    QState *mp_CheckDevStatus;										//!< Check Device Status
    QState *mp_Retry;                                               //!< Retry (at most 10)
	quint32 m_counter;												//!< counter for retry times
};
}
#endif // RS_HEATING_ERR_3S_RETRY
