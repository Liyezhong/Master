/****************************************************************************/
/*! \file RsStandbyWithTissue.h
 *
 *  \brief RsStandby class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 22.04.2014
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
#ifndef RSSTANDBY_WITH_TISSUE_H
#define RSSTANDBY_WITH_TISSUE_H
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "ErrorHandlingSMBase.h"
#include <QStateMachine>

namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_Standby_WithTissue
 */
/****************************************************************************/
class  CRsStandbyWithTissue : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function HeatingStrategy
     *
     *  \param pStateMachine = pointer QStateMachine
     *  \param pParentState = pointer QState
     *
     */
    /****************************************************************************/
    CRsStandbyWithTissue (QStateMachine* pStateMachine, QState* pParentState);

    /****************************************************************************/
    /*!
     *  \brief  desctruction CRsStandbyWithTissue
     */
    /****************************************************************************/
    ~CRsStandbyWithTissue();

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
     *  \brief  Definition/Declaration of function RTStopTempCtrl
     *
     */
    /****************************************************************************/
    void RTStopTempCtrl(DeviceControl::RTTempCtrlType_t);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ShutdownFailedHeater
     *
     */
    /****************************************************************************/
    void ShutdownFailedHeater();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ReleasePressure
     *
     */
    /****************************************************************************/
    void ReleasePressure();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function TasksDone
     *
     */
    /****************************************************************************/
    void TasksDone(bool);

private:
    QState *mp_Initial;                 //!< the current state
    QState *mp_RTBottomStopTempCtrl;    //!< RT Bottom stop tempature control state
    QState *mp_RTSideStopTempCtrl;      //!< RT Top stop tempatrue control state
    QState *mp_ShutdownFailedHeater;    //!< shutdown failed heater state
    QState *mp_ReleasePressure;         //!< release pressure
};
}
#endif // RSSTANDBY_WITH_TISSUE_H
