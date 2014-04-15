/****************************************************************************/
/*! \file RsRvGetOriginalPositionAgain.h
 *
 *  \brief CRsRvGetOriginalPositionAgain class definition.
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
#ifndef RSRVGETORIGINALPOSITIONAGAIN_H
#define RSRVGETORIGINALPOSITIONAGAIN_H
#include "ErrorHandlingSMBase.h"
#include <QStateMachine>

namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_RV_GetOriginalPositionAgain
 */
/****************************************************************************/
class  CRsRvGetOriginalPositionAgain : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    CRsRvGetOriginalPositionAgain (QStateMachine* pStateMachine, QState* pParentState);
    ~CRsRvGetOriginalPositionAgain();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

signals:
    void RvMoveToInitPosition();
    void RvMoveToInitPositionFinished();
    void OnRvMoveToInitPosition();

private:
    QState *mp_RvMovingToInitialPosition;
};
}
#endif // RSRVGETORIGINALPOSITIONAGAIN_H

