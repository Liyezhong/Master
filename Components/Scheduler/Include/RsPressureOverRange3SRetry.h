/****************************************************************************/
/*! \file RsPressureOverRange3SRetry.h
 *
 *  \brief CRsPressureOverRange3SRetry class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ July 24th, 2014
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
#ifndef RS_PRESSURE_OVERRANGE_3S_RETRY_H
#define RS_PRESSURE_OVERRANGE_3S_RETRY_H
#include <QSharedPointer>
#include <QStateMachine>
#include <QCoreApplication>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"


namespace Scheduler{

class SchedulerMainThreadController;
class CSchedulerStateMachine;

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_PressuerOverRange_3S_Retry
 */
/****************************************************************************/
class  CRsPressureOverRange3SRetry : public QObject
{
    Q_OBJECT

    //!< state list of the state machine
    typedef enum
    {
        UNDEF,
        CHECK_PRESSURE,
        WAIT_FOR_1S,
		RELEASE_PRESSURE 
    } StateList_t;

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of class CRsPressureOverRange3SRetry
     *
     *  \param SchedController = pointer to SchedulerMainThreadController
     *  \param StateMachine = pointer to CSchedulerStateMachine
     *
     */
    /****************************************************************************/
    CRsPressureOverRange3SRetry(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine);

    /****************************************************************************/
    /*!
     *  \brief  desctruction CRsPressureOverRange3SRetry
     */
    /****************************************************************************/
    ~CRsPressureOverRange3SRetry();

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
    CSchedulerStateMachine*  mp_StateMachine;               //!< State machine for RS_Standby_WithTissue
    StateList_t              m_CurrentState;                //!< The current state
    qint64  m_CheckPressureTime;                            //!< time for checking pressure
    quint32 m_Counter;                                      //!< counter for retry times
};
}
#endif // RS_PRESSURE_OVERRANGE_3S_RETRY
