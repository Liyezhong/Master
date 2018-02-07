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
#include <QCoreApplication>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler{

class SchedulerMainThreadController;
class CSchedulerStateMachine;

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
        SHUTDOWN_FAILED_HEATER,
        RTBOTTOM_STOP_TEMPCTRL,
        RTSIDE_STOP_TEMPCTRL,
        CHECK_TEMPMODULE_CURRENT,
        RELEASE_PRESSURE
    } StateList_t;

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor of class CRsStandbyWithTissue
     *  \param  type - quint8
     *  \param  SchedController = pointer to SchedulerMainThreadController
     *  \param  StateMachine = pointer to CSchedulerStateMachine
     *
     */
    /****************************************************************************/
    CRsStandbyWithTissue (SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine, quint8 type=0);

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

    /****************************************************************************/
    /*!
     *  \brief start up state machine
     */
    /****************************************************************************/
    void Start();

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
    CSchedulerStateMachine*        mp_StateMachine;         //!< State machine for RS_Standby_WithTissue
    StateList_t                    m_CurrentState;          //!< The current state
    qint64                m_StartCheckingTime;              //!< start-up time for checking temperature module current status
    quint8                m_StandbyType;                    //!< RS_Sandby_WithTissue or RS_Standby.
};
}
#endif // RSSTANDBY_WITH_TISSUE_H
