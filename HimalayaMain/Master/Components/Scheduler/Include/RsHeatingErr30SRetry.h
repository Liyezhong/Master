/****************************************************************************/
/*! \file RsHeatingErr3SRetry.h
 *
 *  \brief RsHeatingErr3SRetry class definition.
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
class  CRsHeatingErr3SRetry : public CErrorHandlingSMBase
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
    CRsHeatingErr3SRetry(QStateMachine* pStateMachine, QState* pParentState);

    /****************************************************************************/
    /*!
     *  \brief  desctruction CRsHeatingErr3SRetry
     */
    /****************************************************************************/
    ~CRsHeatingErr3SRetry();

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
     *  \brief	signal to start temperature control 
     *
     */
    /****************************************************************************/
    void StartTempCtrl();

    /****************************************************************************/
    /*!
     *  \brief	signal to start temperature control	 
     *
     */
    /****************************************************************************/
    void StoptempCtrl();

    /****************************************************************************/
    /*!
     *  \brief	signal to indicate tasks are done
     *
     */
    /****************************************************************************/
    void TasksDone(bool);

private:
    QState *mp_Initial;                 //!< Initial state
    QState *mp_StartTempCtrl;			//!< Start tempature control state
    QState *mp_StopTempCtrl;			//!< Stop tempature control state
};
}
#endif // RS_HEATING_ERR_3S_RETRY
