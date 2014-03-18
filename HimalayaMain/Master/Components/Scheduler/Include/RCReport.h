/****************************************************************************/
/*! \file CRCReport.h
 *
 *  \brief CRCReport class definition.
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
#ifndef RCREPORT_H
#define RCREPORT_H

#include "ErrorHandlingSMBase.h"
#include <QStateMachine>

namespace Scheduler{

/****************************************************************************/
/*!
 * \brief Error Handling State Machine for RC_Report
 */
/****************************************************************************/
class  CRCReport : public CErrorHandlingSMBase
{
    Q_OBJECT
public:
    CRCReport (QStateMachine* pStateMachine, QState* pParentState);
    ~CRCReport();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

signals:
    void RCReport();
    void OnRCReport();

private:
    QState *mp_RCReport;
};
}


#endif // RCREPORT_H
