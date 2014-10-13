/****************************************************************************/
/*! \file OvenHeatingTestEmpty.cpp
 *
 *  \brief Implementation of Heating test empty.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
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


#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWaitingStatus.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include <QCoreApplication>
#include <QTimer>
#include <QObject>

#include "Diagnostics/Include/Oven/OvenHeatingTestEmpty.h"

#define QCOREAPPLICATION_EXEC(TIMEOUT) \
{ \
    QTimer timer; \
    timer.setSingleShot(true); \
    timer.setInterval(TIMEOUT); \
    timer.start(); \
    (void)QObject::connect(&timer, SIGNAL(timeout()), QCoreApplication::instance(), SLOT(quit())); \
    (void)QCoreApplication::exec(); \
}

namespace Diagnostics {

namespace Oven {

CHeatingTestEmpty::CHeatingTestEmpty(CDiagnosticMessageDlg *dlg)
    : CTestBase()
    , mp_dlg(dlg)
{
}

CHeatingTestEmpty::~CHeatingTestEmpty(void)
{
}

int CHeatingTestEmpty::Run(void)
{
    qDebug() << "Oven Heating test empty starts!";
    return StartHeating();
}

int CHeatingTestEmpty::StartHeating(void)
{
    qDebug() << "Oven Heating test empty: start heating!";

    QString title((tr(" Rotary Valve Movement Test")));
    QString text;

    int ret;

    qreal OvenTempTop(0);
    qreal OvenTempSensor1(0);
    qreal OvenTempSensor2(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("HeatingTestEmpty");

    qreal DiffTemp = p_TestCase->GetParameter("OvenDiffTemp").toFloat();
    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();

    (void)dev->OvenStopHeating();

    ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);
    qDebug()<<"OvenGetTemp ---- "<<OvenTempSensor1<<"  "<<OvenTempSensor2;

    if (ret != RETURN_OK || qAbs(OvenTempSensor1-OvenTempSensor2) > DiffTemp) {
        text = QString(tr("Paraffin Oven Heating Test (Empty) failed. "
                          "Temperature sensors are out of specification. Please "
                          "check resistance of temperature sensors to verify, "
                          "exchange paraffin oven module and repeat this test."));
        mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    qreal OvenTopTargetTemp = p_TestCase->GetParameter("OvenTopTargetTemp").toFloat();
    qreal OvenBottomTargetTemp = p_TestCase->GetParameter("OvenBottomTargetTemp").toFloat();
    if (OvenTempSensor1 >= OvenTopTargetTemp || OvenTempSensor2 >= OvenBottomTargetTemp) {
        text = QString(tr("Please remove any paraffin bath present in "
                          "the paraffin oven. Then please leave the oven cover"
                          "opened to speed up the cooling process."));
        ret = mp_dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            return RETURN_ERR_FAIL;

          // if abort
          // return main display
//        text = QString(tr(""));
        mp_dlg->ShowWaitingDialog(title, text);
        // show a waiting dialog
         // wait a few minutes later, read temp
        int t = p_TestCase->GetParameter("t").toInt();

        for (int i = 0; i < t; i += 5) {
            QCOREAPPLICATION_EXEC(5);
            qreal OvenTempSensor1Cur;
            qreal OvenTempSensor2Cur;
            ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
            if (OvenTempSensor1 - OvenTempSensor1Cur >= 10 && OvenTempSensor2 - OvenTempSensor2Cur >= 10) {
                ret = RETURN_OK;
                break;
            }
            qDebug() << "OvenGetTemp ---- " << OvenTempSensor1Cur << "  " << OvenTempSensor2Cur;
        }

        if (ret != RETURN_OK) {
            mp_dlg->HideWaitingDialog();
            text = tr("Paraffin Oven Heating Test "
                      "(Empty) failed. ASB5 is damaged."
                      "Exchange it and repeat this test.");
            mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
            return RETURN_ERR_FAIL;
        }

        mp_dlg->HideWaitingDialog();
        text = tr("Please close oven cover.");
        ret = mp_dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            return RETURN_ERR_FAIL;
    } else {
        text = tr("Please make sure there are no "
                  "paraffin baths present in the paraffin oven."
                  "Verify the oven surfaces are dry and clean, "
                  "and the oven cover is closed.");
        ret = mp_dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            return RETURN_ERR_FAIL;
    }

    //<<<

    text = tr("Used Time: 00:00:00\n"
              "Estimated Duration: 00:00:00\n"
              "Target Temperature(℃ ): X"
              "Current Temperature (Top)(℃ ):"
              "Current Temperature (Bottom 1)(℃ )"
              "Current Temperature (Bottom 2)(℃ )");
    mp_dlg->ShowWaitingDialog(title, text);

    ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);
    if (ret != RETURN_OK)
        return RETURN_ERR_FAIL;

    OvenTempSensor1 += 10;
    OvenTempSensor2 += 10;
    (void)dev->OvenStartHeating(OvenTempSensor1, OvenTempSensor2);
    int t1 = p_TestCase->GetParameter("t1").toInt();

    for (int i = 0; i < t1; i += 5) {
        QCOREAPPLICATION_EXEC(5);
        qreal OvenTempSensor1Cur;
        qreal OvenTempSensor2Cur;
        ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
        if (OvenTempSensor1Cur > OvenTempSensor1 && OvenTempSensor2Cur > OvenTempSensor2) {
            ret = RETURN_OK;
            break;
        }
        mp_dlg->ShowWaitingDialog(title, text);
        qDebug() << "OvenGetTemp ---- " << OvenTempSensor1Cur << "  " << OvenTempSensor2Cur;
    }

    if (ret != RETURN_OK) {
        text = tr("Paraffin Oven Heating Test (Empty) failed. Temperature did "
                  "not reach TM[current temperature +10℃ ] within t1 mins?."
                  "Root cause might be damaged ASB5 or Paraffin Oven Module."
                  "Sequentially check resistance of heaters and function of ASB5."
                  "Exchange defective part accordingly and repeat this test.");
        mp_dlg->HideWaitingDialog();
        mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    int t2 = p_TestCase->GetParameter("t2").toInt();

    for (int i = 0; i < t2; i += 5) {
        QCOREAPPLICATION_EXEC(5);
        qreal OvenTempSensor1Cur;
        qreal OvenTempSensor2Cur;
        ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
        if (qAbs(OvenTempSensor1Cur - OvenTempSensor1) > 10  || qAbs(OvenTempSensor2Cur - OvenTempSensor2) > 10) {
            ret = RETURN_ERR_FAIL;
            break;
        }
        // todo
        // text =
        mp_dlg->ShowWaitingDialog(title, text);
        qDebug() << "OvenGetTemp ---- " << OvenTempSensor1Cur << "  " << OvenTempSensor2Cur;
    }

    mp_dlg->HideWaitingDialog();
    if (ret != RETURN_OK) {
        text = tr("Paraffin Oven Heating Test (Empty) failed."
                  "ASB5 is damaged. Exchange it and repeat this test.");
        mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr("Paraffin Oven Heating Test (Empty) "
              "successful. Please re-insert the paraffin "
              "baths and close the oven cover.");
    mp_dlg->ShowMessage(title, text, RETURN_OK);
    //>>>

    // close
    (void)dev->OvenStopHeating();

    p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SOvenPreTest");

    OvenTempSensor1 = p_TestCase->GetParameter("OvenTopTargetTemp").toFloat();
    OvenTempSensor2 = p_TestCase->GetParameter("OvenBottomTargetTemp").toFloat();

    return dev->OvenStartHeating(OvenTempSensor1, OvenTempSensor2);
}

void CHeatingTestEmpty::Succeed(void)
{
//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_SUCCESS);
//    qDebug() << "Oven Heating test empty succeeded!";

//    // display success message
//    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
//    dlg->SetTitle(tr("Oven Heating Test - empty"));
//    dlg->SetIcon(QMessageBox::Information);
//    dlg->SetText(tr("Heating test empty SUCCEEDED!"));
//    dlg->HideButtons();
//    dlg->SetButtonText(1, tr("OK"));

//    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

//    /// \todo: connect the message dialog accepted signal
//    ///        with the test's stop heating slots */
//    // (void)connect(dlg, SIGNAL(accept()), TEST, SLOT(STOP_HEATING_SLOT()) );

//    dlg->exec();

//    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingTestEmpty::Fail(void)
{
//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_FAILURE);
//    qDebug() << "Oven Heating test empty failed!";

//    // display failure message
//    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
//    dlg->SetTitle(tr("Oven Heating Test - empty"));
//    dlg->SetIcon(QMessageBox::Critical);
//    dlg->SetText(tr("Heating test empty FAILED!"));
//    dlg->HideButtons();
//    dlg->SetButtonText(1, tr("OK"));

//    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

//    /// \todo: connect the message dialog accepted signal
//    ///        with the test's stop heating slots */
//    // (void)connect(dlg, SIGNAL(accept()), TEST, SLOT(STOP_HEATING_SLOT()) );

//    dlg->exec();

//    delete dlg;

//    /// \todo: log here **************************************/
}

} // namespace Oven

} // namespace Diagnostics
