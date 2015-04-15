/****************************************************************************/
/*! \file Diagnostics/Source/MainControl.cpp
 *
 *  \brief Implementation of Main control test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-09
 *   $Author:  $ Dixiong Li
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

#include "Diagnostics/Include/MainControl.h"

#include <QDebug>

#include "ui_MainControl.h"

#include "MainMenu/Include/MessageDlg.h"
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "Diagnostics/Include/MainControl/ASBTest.h"

namespace Diagnostics {

CMainControl::CMainControl(QWidget *p_Parent) :
    QWidget(p_Parent),
    ui(new Ui::CMainControl)
{
    qDebug() << "Diagnostics::CMainControl::CMainControl";

    ui->setupUi(this);

    (void)connect(ui->testASB3,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartASB3Test()) );

    (void)connect(ui->testASB5,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartASB5Test()) );

    (void)connect(ui->testASB15,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartASB15Test()) );
}

CMainControl::~CMainControl()
{
    delete ui;
}


void CMainControl::LogResult(QString &TestName, int RetError)
{
    if (RetError == RETURN_OK)
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is successful.").arg(TestName));
    else if (RetError == RETURN_ABORT)
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is aborted.").arg(TestName));
    else
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is failed.").arg(TestName));

}


void CMainControl::StartASB3Test(void)
{
    QString TestName = QString("%1 Test").arg(ui->testASB3->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));


    emit SetGUITabEnable(false);
    MainControl::CASBTest Test(Slave_3, this);

    int Ret = Test.Run();

    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CMainControl::StartASB5Test(void)
{
    QString TestName = QString("%1 Test").arg(ui->testASB5->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));


    emit SetGUITabEnable(false);
    MainControl::CASBTest Test(Slave_5, this);

    int Ret = Test.Run();

    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CMainControl::StartASB15Test(void)
{
    QString TestName = QString("%1 Test").arg(ui->testASB15->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    MainControl::CASBTest Test(Slave_15, this);

    int Ret = Test.Run();

    LogResult(TestName, Ret);


    emit SetGUITabEnable(true);
}
/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CMainControl::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type())
    {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

} // namespace Diagnostics

