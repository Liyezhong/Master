/****************************************************************************/
/*! \file RsFillingAfterFlush.h
 *
 *  \brief CRsFillingAfterFlush class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ July 22th, 2014
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

#ifndef RS_FILLINGAFTERFLUSH_H
#define RS_FILLINGAFTERFLUSH_H

#include <QStateMachine>
#include <QSharedPointer>
#include <QCoreApplication>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerStateHandler.h"

namespace Scheduler
{

/****************************************************************************/
/*!
 * \brief Program running State machine for RsFillingAfterFlush
 */
/****************************************************************************/
class CRsFillingAfterFlush : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of CRsFillingAfterFlush
     *
     *  \param SchedController from SchedulerMainThreadController
     *  \param StateMachine from CSchedulerStateMachine
     *
     */
    /****************************************************************************/
    CRsFillingAfterFlush(SchedulerMainThreadController* SchedController, CSchedulerStateHandler* SchedulerStatehandler, CSchedulerStateMachine* StateMachine);

    /****************************************************************************/
    /*!
     *  \brief  desctruction of CRsFillingAfterFlush
     */
    /****************************************************************************/
    ~CRsFillingAfterFlush();

    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for RsFillingAfterFlush
     *  \param  cmdName - command name
     *  \param  retCode - return code
     *  \return void
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
    typedef enum
    {
        RSFILLINGAFTERFLUSH_UNDEF,
        RSFILLINGAFTERFLUSH_INIT,
        RSFILLINGAFTERFLUSH_PRESSURE,
        RSFILLINGAFTERFLUSH_WAIT,
        RSFILLINGAFTERFLUSH_HEATINGLEVELSENSOR,
        RSFILLINGAFTERFLUSH_CHECKLEVELSENSORTEMP,
        RSFILLINGAFTERFLUSH_FILLING,
        RSFILLINGAFTERFLUSH_MOVETOSEALING,
        RSFILLINGAFTERFLUSH_RELEASEPRESSURE
    } StateList_t;

private:
    SchedulerMainThreadController*   mp_SchedulerThreadController;  //!< Pointer to Scheduler Thread Controller
    CSchedulerStateMachine*          mp_StateMachine;               //!< State machine for FillingAfterFlush
    CSchedulerStateHandler*          mp_SchedulerStateHandler;
    StateList_t                      m_CurrentState;                //!< Current state
    qint64                           m_StartTime;                    //!< The begin time
    quint8                           m_MoveToSealingSeq;             //!< Sequence of moving to seaing position
    QString m_Sender;
};

}

#endif
