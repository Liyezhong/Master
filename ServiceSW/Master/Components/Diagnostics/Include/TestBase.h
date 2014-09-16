/****************************************************************************/
/*! \file TestBase.h
 *
 *  \brief Declaration of Retort test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_TESTBASE_H
#define DIAGNOSTICS_TESTBASE_H

#include <QObject>
#include "MainMenu/Include/MessageDlg.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"

namespace Diagnostics {

class CTestBase : public QObject
{
    Q_OBJECT

public:
    CTestBase(QWidget *parent=NULL);
    virtual ~CTestBase(void);

    virtual int Run(void) = 0;

protected:
    void ShowMessage(QString& MessageTitle, QString& MessageText, ErrorCode_t Ret);
    void ShowWaitingDialog(QString& MessageTitle, QString& MessageText);
    void HideWaitingDialog();

private:
    QWidget        *mp_Parent;
    MainMenu::CMessageDlg *mp_WaitDlg;
};

}

#endif // DIAGNOSTICS_TESTBASE_H
