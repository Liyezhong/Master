/****************************************************************************/
/*! \file Diagnostics/Source/Retort.cpp
 *
 *  \brief Implementation of Retort test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-27
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

#include "Diagnostics/Include/Retort.h"

#include <QDebug>

#include "ui_Retort.h"

#include "Global/Include/Utils.h"
#include "Diagnostics/Include/Retort/LidLockTest.h"
#include "Diagnostics/Include/Retort/LevelSensorDetectingTest.h"
#include "Diagnostics/Include/Retort/RetortHeatingTestEmpty.h"
#include "Diagnostics/Include/Retort/RetortHeatingTestWithWater.h"
#include "Diagnostics/Include/Retort/DrainReagentTest.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Main/Include/HimalayaServiceEventCodes.h"


namespace Diagnostics {

CRetort::CRetort(QWidget *p_Parent) :
    QWidget(p_Parent),
    mp_Ui(new Ui::CRetort)
{
    mp_Ui->setupUi(this);

    mp_MessageDlg = new CDiagnosticMessageDlg(this);

    mp_Ui->testLevelSensorDetection->setEnabled(false);
    mp_Ui->testHeatingWater->setEnabled(false);

    (void)connect(mp_Ui->testLidLock,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartLidLockTest()) );

    (void)connect(mp_Ui->testDrainReagent,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartDrainReagentTest()) );

    (void)connect(mp_Ui->testLevelSensorDetection,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartLevelSensorDetectionTest()) );

    (void)connect(mp_Ui->testHeatingEmpty,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingTestEmpty()) );

    (void)connect(mp_Ui->testHeatingWater,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingTestWithWater()) );
}

CRetort::~CRetort()
{
    try {
        delete mp_Ui;
        delete mp_MessageDlg;
    }
    catch (...) {

    }
}

void CRetort::LogResult(QString &TestName, ErrorCode_t RetError)
{
    if (RetError == RETURN_OK)
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is successful.").arg(TestName));
    else if (RetError == RETURN_ABORT)
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is aborted.").arg(TestName));
    else
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is failed.").arg(TestName));

}

void CRetort::OnEnableTestButton()
{
    mp_Ui->testLevelSensorDetection->setEnabled(true);
    mp_Ui->testHeatingWater->setEnabled(true);
}

void CRetort::StartLidLockTest(void)
{
    QString TestName = QString("Retort %1").arg(mp_Ui->testLidLock->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    Retort::CLidLockTest Test(mp_MessageDlg, this);
    ErrorCode_t ret = (ErrorCode_t)Test.Run();
    LogResult(TestName, ret);

    emit SetGUITabEnable(true);
}

void CRetort::StartDrainReagentTest(void)
{
    QString TestName = QString("Retort %1").arg(mp_Ui->testDrainReagent->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    Retort::CDrainReagentTest Test(mp_MessageDlg, this);

    ErrorCode_t ret = (ErrorCode_t)Test.Run();

    LogResult(TestName, ret);
    emit SetGUITabEnable(true);
}

void CRetort::StartLevelSensorDetectionTest(void)
{
    QString TestName = QString("Retort %1").arg(mp_Ui->testLevelSensorDetection->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));


    emit SetGUITabEnable(false);
    Retort::CLevelSensorDetectingTest Test(mp_MessageDlg, this);

    ErrorCode_t ret = (ErrorCode_t)Test.Run();

    LogResult(TestName, ret);

    emit SetGUITabEnable(true);
}

void CRetort::StartHeatingTestEmpty(void)
{
    QString TestName = QString("Retort %1").arg(mp_Ui->testHeatingEmpty->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));


    emit SetGUITabEnable(false);
    Retort::CHeatingTestEmpty test(mp_MessageDlg);

    ErrorCode_t ret = (ErrorCode_t)test.Run();

    LogResult(TestName, ret);

    emit SetGUITabEnable(true);
}

void CRetort::StartHeatingTestWithWater(void)
{
    QString TestName = QString("Retort %1").arg(mp_Ui->testHeatingWater->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    Retort::CHeatingTestWithWater test(mp_MessageDlg);

    ErrorCode_t ret = (ErrorCode_t)test.Run();
    LogResult(TestName, ret);

    emit SetGUITabEnable(true);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CRetort::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

} // namespace Diagnostics
