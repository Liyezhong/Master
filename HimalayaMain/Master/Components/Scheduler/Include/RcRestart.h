/****************************************************************************/
/*! \file RCRestart.h
 *
 *  \brief CRCRestart class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ June 3rd, 2014
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
#ifndef RCRESTART_H
#define RCRESTART_H

#include "ErrorHandlingSMBase.h"
#include <QStateMachine>

namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Error Handling State Machine for RC_Restart
 */
/****************************************************************************/
class  CRcRestart : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of class CRCRestart 
     *
     *  \param pStateMachine = pointer QStateMachine
     *  \param pParentState = pointer QState
     *
     */
    /****************************************************************************/
    CRcRestart(QStateMachine* pStateMachine, QState* pParentState);

    /****************************************************************************/
     /*! 
      *  \brief Destructor of class CRCRestart 
      *
      */
     /****************************************************************************/
    ~CRcRestart();

    /****************************************************************************/
     /*! 
      *  \brief Get current state of state machine 
      *
      */
     /****************************************************************************/
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function OnHandleWorkFlow
     *
     *
     */
    /****************************************************************************/
    void OnHandleWorkFlow(bool flag);
signals:
    /****************************************************************************/
    /*! 
     *  \brief send out recovery signal 
     *
     */
    /****************************************************************************/
	void Recover();
    /****************************************************************************/
    /*! 
     *  \brief send out Tasks Done signal 
     *
     */
    /****************************************************************************/
	void TasksDone(bool);

private:
	QState *mp_Initial;		//!< Initial state
	QState *mp_Recover;		//!< Recover to normal state 
};

}
#endif // RCRESTART_H
