/****************************************************************************/
/*! \file HeatingTestEmpty.h
 *
 *  \brief Declaration of Heating test empty.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-13
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H
#define DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
#include <QThread>

namespace Diagnostics {

namespace Oven {

class CHeatingTestEmptyThread;
struct heatingTestStatus {
   int UsedTime;
   int EDTime;
   qreal OvenTempTop;
   qreal OvenTempSensor1;
   qreal OvenTempSensor2;
   qreal OvenTargetTemp;

   qreal OvenTempSensor1Target;
   qreal OvenTempSensor2Target;
   qreal TempOffset;
   qreal TempOffsetRange;
};

class CHeatingTestEmpty : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CHeatingTestEmpty(CDiagnosticMessageDlg *_dlg);
    ~CHeatingTestEmpty(void);

    int Run(void);

signals:
    void notifyClose(void);

private Q_SLOTS:
    void AbortDialog(bool status);
    void ShowWaitingDialog(QByteArray status);
    void Clean(void);
    void ShowMessageBoxFail(QString msg);
    void ShowMessageBoxSuccess(QString msg);

private:
    CDiagnosticMessageDlg *dlg;
    DiagnosticsManufacturing::CHeatingTestDialog *waitDialog;
    CHeatingTestEmptyThread *heatingTestEmptyThread;
    QThread Thread;
};

class CHeatingTestEmptyThread : public QThread
{
    Q_OBJECT

public:
    void run();
signals:
    void freshWaitingDlg(QByteArray status);
    void ShowMessageBoxFail(QString msg);
    void ShowMessageBoxSuccess(QString msg);

public:
    struct heatingTestStatus status;
    int t1;
    int t2;

private:
    void sendBuf(struct heatingTestStatus *s);
};

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H
