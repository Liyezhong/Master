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
        RTBOTTOM_STOP_TEMPCTRL,
        RTSIDE_STOP_TEMPCTRL,
        SHUTDOWN_FAILD_HEATER,
        RELEASE_PRESSURE
    } StateList_t;
public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor of class CRsStandbyWithTissue
     *
     *  \param SchedController = pointer to SchedulerMainThreadController
     *
     */
    /****************************************************************************/
    CRsStandbyWithTissue (SchedulerMainThreadController* SchedController);

    /****************************************************************************/
    /*!
     *  \brief  Desctruction of class CRsStandbyWithTissue
     */
    /****************************************************************************/
    ~CRsStandbyWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Get current state
     *
     *  \param statesList = QSet<QAbstractState*>
     *
     *	\return Internal state list
     */
    /****************************************************************************/
    CRsStandbyWithTissue::StateList_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for RS_Standby_WithTissue
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

signals:
    /****************************************************************************/
    /*!
     *  \brief  Signal for stopping RTSide temperature control
     *
     */
    /****************************************************************************/
    void StopRTSideTempCtrl();

    /****************************************************************************/
    /*!
     *  \brief  Signal for shutting down failer heaters
     *
     */
    /****************************************************************************/
    void ShutdownFailedHeater();

    /****************************************************************************/
    /*!
     *  \brief  Signal for releasing pressure
     *
     */
    /****************************************************************************/
    void ReleasePressure();

    /****************************************************************************/
    /*!
     *  \brief  Signal for Tasks done
     *  \param  bool ture- tasks done successfully, false - tasks failed
     *
     */
    /****************************************************************************/
    void TasksDone(bool);
private slots:

    /****************************************************************************/
    /*!
     *  \brief  Slot for releasing pressure
     *  \param  void
     *  \return void
     *
     */
    /****************************************************************************/
    void OnReleasePressure();

private:
    SchedulerMainThreadController* mp_SchedulerController;  //!< Pointer to SchedulerMainThreadController
    QSharedPointer<QStateMachine>   mp_StateMachine;        //!< State machine for RS_Standby_WithTissue
    QSharedPointer<QState> mp_RTBottomStopTempCtrl;         //!< RT Bottom stop tempature control state
    QSharedPointer<QState> mp_RTSideStopTempCtrl;           //!< RT Top stop tempatrue control state
    QSharedPointer<QState> mp_ShutdownFailedHeater;         //!< Shutdown failed heater state
    QSharedPointer<QState> mp_ReleasePressure;              //!< Release pressure
};
}
#endif // RSSTANDBY_WITH_TISSUE_H
