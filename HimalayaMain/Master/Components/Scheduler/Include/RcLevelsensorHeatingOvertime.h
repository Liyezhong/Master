/****************************************************************************/
/*! \file	RcLevelsensorHeatingOvertime.h
 *
 *  \brief	state machine class for recovery RC_LevelSensor_Heating_Overtime 
 *
 *   $Version: $ 0.1
 *   $Date:    $ May 23rd, 2014
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
#ifndef RC_LEVELSENSOR_HEATING_OVERTIME_H
#define RC_LEVELSENSOR_HEATING_OVERTIME_H
#include "ErrorHandlingSMBase.h"
#include <QStateMachine>

namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RC_LevelSensor_Heating_Overtime 
 */
/****************************************************************************/
class  CRcLevelSensorHeatingOvertime : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CRcLevelSensorHeatingOvertime
     *
     *  \param pStateMachine = pointer QStateMachine
     *  \param pParentState = pointer QState
     *
     */
    /****************************************************************************/
     CRcLevelSensorHeatingOvertime(QStateMachine* pStateMachine, QState* pParentState);

     /****************************************************************************/
     /*!
      *  \brief  Definition/Declaration of function ~CRcLevelSensorHeatingOvertime
      *
      */
     /****************************************************************************/
    ~CRcLevelSensorHeatingOvertime();

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
     *  \param flag
     *
     */
    /****************************************************************************/
    void OnHandleWorkFlow(bool flag);

signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function RestartLevelSensorTempControl
     *
     */
    /****************************************************************************/
    void RestartLevelSensorTempControl();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function TasksDone
     *
     */
    /****************************************************************************/
    void TasksDone(bool);

private:
    QState *mp_Initial;                         //!< the current state
    QState *mp_RestartLevelSensorTempControl;   //!< the restart levelSensor temperature control
};
}
#endif // RC_LEVELSENSOR_HEATING_OVERTIME_H
