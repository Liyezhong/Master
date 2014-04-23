/****************************************************************************/
/*! \file RsStandby.h
 *
 *  \brief CRsStandby class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 18.03.2014
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
#ifndef RSSTANDBY_H
#define RSSTANDBY_H
#include "ErrorHandlingSMBase.h"
#include <QStateMachine>

namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_Standby
 */
/****************************************************************************/
class  CRsStandby : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    CRsStandby (QStateMachine* pStateMachine, QState* pParentState);
    ~CRsStandby();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ReleasePressure
     */
    /****************************************************************************/
    void ReleasePressure();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ShutdownFailedHeater
     */
    /****************************************************************************/
    void ShutdownFailedHeater();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ShutdownFailedHeaterFinished
     */
    /****************************************************************************/
    void ShutdownFailedHeaterFinished();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnReleasePressure
     */
    /****************************************************************************/
    void OnReleasePressure();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnShutdownFailedHeater
     */
    /****************************************************************************/
    void OnShutdownFailedHeater();

private:
    QState *mp_ReleasePressure;       ///<  Definition/Declaration of variable mp_ReleasePressure
    QState *mp_ShutdownFailedHeater;       ///<  Definition/Declaration of variable mp_ShutdownFailedHeater
};
}

#endif // RSSTANDBY_H
