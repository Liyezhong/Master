/****************************************************************************/
/*! \file RsStandbyWithTissue.h
 *
 *  \brief RsStandby class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ April 22nd, 014
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
#include <QStateMachine>
#include <QSharedPointer>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler{

class SchedulerMainThreadController;

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_Standby_WithTissue
 */
/****************************************************************************/
class  CRsStandbyWithTissue : public QObject
{
    Q_OBJECT

    //!< state list of the state machine
    typedef enum
    {
        UNDEF,
        INIT,
        RTBOTTOM_STOP_TEMPCTRL,
        RTSIDE_STOP_TEMPCTRL,
        SHUTDOWN_FAILD_HEATER,
        RELEASE_PRESSURE
    } StateList_t;
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function HeatingStrategy
     *
     *  \param SchedController = pointer SchedulerMainThreadController
     *
     */
    /****************************************************************************/
    CRsStandbyWithTissue (SchedulerMainThreadController* SchedController);

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
    CRsStandbyWithTissue::StateList_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function HandleWorkFlow
     *
     *  \param flag = bool
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(bool flag);

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
    SchedulerMainThreadController* mp_SchedulerController;  //!< Pointer to SchedulerMainThreadController
    QSharedPointer<QStateMachine>   mp_StateMachine;        //!< State machine for RS_Standby_WithTissue
    QSharedPointer<QState> mp_Initial;                      //!< Initial state
    QSharedPointer<QState> mp_RTBottomStopTempCtrl;         //!< RT Bottom stop tempature control state
    QSharedPointer<QState> mp_RTSideStopTempCtrl;           //!< RT Top stop tempatrue control state
    QSharedPointer<QState> mp_ShutdownFailedHeater;         //!< Shutdown failed heater state
    QSharedPointer<QState> mp_ReleasePressure;              //!< Release pressure
};
}
#endif // RSSTANDBY_WITH_TISSUE_H
