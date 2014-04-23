/****************************************************************************/
/*! \file ErrorHandlingSMBase.h
 *
 *  \brief CErrorHandlingSMBase class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.01.2014
 *   $Author:  $ Quan Zhang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef ERRORHANDLINGSMBASE_H
#define ERRORHANDLINGSMBASE_H
#include <QStateMachine>
#include "SchedulerStateMachineStates.h"

namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Base class for Error Handling State Machine
 */
/****************************************************************************/
class CErrorHandlingSMBase : public QObject
{
    Q_OBJECT
public:
    CErrorHandlingSMBase(QStateMachine* pStateMachine, QState* pParentState);
    ~CErrorHandlingSMBase();
    virtual SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);
protected:
    QStateMachine* mp_StateMachine;       ///<  Definition/Declaration of variable mp_StateMachine       ///<  Definition/Declaration of variable mp_StateMachine       ///<  Definition/Declaration of variable mp_StateMachine       ///<  Definition/Declaration of variable mp_StateMachine       ///<  Definition/Declaration of variable mp_StateMachine       ///<  Definition/Declaration of variable mp_StateMachine       ///<  Definition/Declaration of variable mp_StateMachine
    QState* mp_ParentState;       ///<  Definition/Declaration of variable mp_ParentState       ///<  Definition/Declaration of variable mp_ParentState       ///<  Definition/Declaration of variable mp_ParentState       ///<  Definition/Declaration of variable mp_ParentState       ///<  Definition/Declaration of variable mp_ParentState       ///<  Definition/Declaration of variable mp_ParentState       ///<  Definition/Declaration of variable mp_ParentState
    QList<QState*> mp_StateList;       ///<  Definition/Declaration of variable mp_StateList       ///<  Definition/Declaration of variable mp_StateList       ///<  Definition/Declaration of variable mp_StateList       ///<  Definition/Declaration of variable mp_StateList       ///<  Definition/Declaration of variable mp_StateList       ///<  Definition/Declaration of variable mp_StateList       ///<  Definition/Declaration of variable mp_StateList

};
}
#endif // ERRORHANDLINGSMBASE_H
