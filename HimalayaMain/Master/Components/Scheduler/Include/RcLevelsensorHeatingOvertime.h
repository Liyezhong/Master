/****************************************************************************/
/*! \file	CRcLevelSensorHeatingOvertime.h
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
     CRcLevelSensorHeatingOvertime(QStateMachine* pStateMachine, QState* pParentState);
    ~CRcLevelSensorHeatingOvertime();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);
    void OnHandleWorkFlow(bool flag);

signals:
    void RestartLevelSensorTempControl();
    void TasksDone(bool);

private:
    QState *mp_Initial;
    QState *mp_RestartLevelSensorTempControl;
};
}
#endif // RC_LEVELSENSOR_HEATING_OVERTIME_H
