#if 0
/****************************************************************************/
/*! \file SelfTestStateMachine.h
 *
 *  \brief SelfTestStateMachine class definition.
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
#ifndef SELFTESTSTATEMACHINE_H
#define SELFTESTSTATEMACHINE_H

#include <QStateMachine>
#include <QFinalState>

namespace Scheduler{

typedef enum
{
SELF_TEST_INIT,
SELF_TEST_TEMP_READY,
SELF_TEST_BOTTLE_CHECKING,
SELF_TEST_BOTTLE_CHECK_FINISH,
SELF_TEST_FINISH,
SELF_TEST_UNDEF
}SelfTestStateMachine_t;

class CSelfTestStateMachine : public QObject
{
    Q_OBJECT
public:
    CSelfTestStateMachine();
    ~CSelfTestStateMachine();
    CSelfTestStateMachine_t GetCurrentState();

    void Start();
    void Stop();
    void NotifyTempsReady();
    void NotifyCheckStation();
    void NotifyGotCheckStationResult();
    void NotifyCheckStaionFinished();

signals:
    void TempsReady();
    void CheckStation();
    void GotStationCheckResult();
    void StationCheckFinished();

private slots:
    //void OnStateChanged();

private:
    QStateMachine* mp_SelfTestStateMachine;
    QState* mp_StInit;
    QState* mp_StTempReady;
    QState* mp_StStationChecking;
    QState* mp_StStationCheckFinish;
    QFinalState* mp_StFinish;

};

}


#endif // SELFTESTSTATEMACHINE_H
#endif
