/****************************************************************************/
/*! \file InitialSystemCheck.cpp
 *
 *  \brief Implementation of Initial system check..
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-15
 *   $Author:  $ Sunny.Qu
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

#include "Diagnostics/Include/InitialSystem/InitialSystemCheck.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Core/Include/ServiceUtils.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include "Diagnostics/Include/InitialSystem/ACVoltageTest.h"
#include "Diagnostics/Include/InitialSystem/LTubePreTest.h"
#include "Diagnostics/Include/InitialSystem/MainsRelayTest.h"
#include "Diagnostics/Include/InitialSystem/OvenPreTest.h"
#include "Diagnostics/Include/InitialSystem/RetortPreTest.h"
#include "Diagnostics/Include/InitialSystem/RVPreTest.h"
#include "Diagnostics/Include/SelectMeltingPointDialog.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include <QDebug>

namespace Diagnostics {

namespace InitialSystem {

CInitialSystemCheck::CInitialSystemCheck(Core::CServiceGUIConnector *p_DataConnector, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_DataConnector(p_DataConnector),
      m_ParaffinMeltPoint(0),
      m_IsEmptyInRetort(false),
      m_IsParaffinInRetort(false)
{

}

CInitialSystemCheck::~CInitialSystemCheck(void)
{
}

int CInitialSystemCheck::Run(void)
{   
     int Ret = 0;

     Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("Start Initial System Check."));

    CMainsRelayTest *MainsRelayTest = new CMainsRelayTest(mp_Parent);
    qDebug()<<"Start MainsRelayTest";

    Ret = MainsRelayTest->Run();

//    Ret = RETURN_OK; // only for test

    emit RefreshStatusToGUI(Service::INITIAL_MAINS_RELAY, Ret);
    delete MainsRelayTest;

    if (Ret != RETURN_OK) {
        return Ret;
    }
    ServiceDeviceProcess::Instance()->Pause(1000);

    qDebug()<<"Start ACVoltageTest";

    CACVoltageTest *ACVoltageTest = new CACVoltageTest(mp_Parent);
    Ret = ACVoltageTest->Run();

//    Ret = RETURN_OK; // only for test

    emit RefreshStatusToGUI(Service::INITIAL_AC_VOLTAGE, Ret);
    delete ACVoltageTest;

    if (Ret != RETURN_OK) {
        return Ret;
    }
    ServiceDeviceProcess::Instance()->Pause(1000);

    ConfirmParaffinBath();
    ConfirmRetortCondition();

    (void)ServiceDeviceProcess::Instance()->MainRelaySetOnOff(true);
    qDebug()<<"Start oven pre-test";


    COvenPreTest *OvenPreTest = new COvenPreTest(mp_Parent);
    Ret = OvenPreTest->Run();
    emit RefreshStatusToGUI(Service::INITIAL_OVEN, Ret);
    if (Ret == RETURN_OK) {
        OvenPreTest->StartPreHeating(m_ParaffinMeltPoint);
        emit RefreshHeatingStatus(Service::INITIAL_OVEN);
    }
    delete OvenPreTest;
    ServiceDeviceProcess::Instance()->Pause(1000);

    qDebug()<<"Start liquid tube pre-test";

    CLTubePreTest *LTubePreTest = new CLTubePreTest(mp_Parent);
    Ret = LTubePreTest->Run();
    emit RefreshStatusToGUI(Service::INITIAL_LIQUID_TUBE, Ret);
    if (Ret == RETURN_OK) {
        LTubePreTest->StartPreHeating();
        emit RefreshHeatingStatus(Service::INITIAL_LIQUID_TUBE);
    }
    delete LTubePreTest;
    ServiceDeviceProcess::Instance()->Pause(1000);

    qDebug()<<"Start rotary valve pre-test";

    CRVPreTest *RVPreTest = new CRVPreTest(mp_Parent);
    Ret = RVPreTest->Run();
    emit RefreshStatusToGUI(Service::INITIAL_ROTARY_VALVE, Ret);
    if (Ret == RETURN_OK) {
        RVPreTest->StartPreHeating();
        emit RefreshHeatingStatus(Service::INITIAL_ROTARY_VALVE);
    }
    delete RVPreTest;

    ServiceDeviceProcess::Instance()->Pause(1000);

    qDebug()<<"Start retort pre-test";

    if (m_IsParaffinInRetort) {
        CRetortPreTest *RetortPreTest = new CRetortPreTest(mp_Parent);
        Ret = RetortPreTest->Run();
        emit RefreshStatusToGUI(Service::INITIAL_RETORT, Ret);
        delete RetortPreTest;
    }

    emit PreTestFinished();

    return Ret;
}

void CInitialSystemCheck::RetortPreHeating()
{
    QString Text = "This function will heat the retort to melt excessive paraffin. "
            "Please visually observe the paraffin melting status in retort. As soon "
            "the sides of paraffin block are molten, remove the block manually and "
            "ensure that existing specimen are provided to the customer. Then "
            "change to the \"Diagnostic_Retort_Reagent Drain\" function and drain "
            "remaining paraffin back to original paraffin bath. Thereafter flush the retort";

    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);

    dlg->SetTitle("Retort");
    dlg->SetIcon(QMessageBox::Question);
    dlg->SetText(Text);
    dlg->HideCenterButton();
    dlg->SetButtonText(1, tr("Ok"));
    dlg->SetButtonText(3, tr("Abort"));

    if (dlg->exec() == 0) {
        delete dlg;
        return;
    }
    delete dlg;

    CRetortPreTest *RetortPreTest = new CRetortPreTest(mp_Parent);
    RetortPreTest->StartPreHeating(m_ParaffinMeltPoint);
    emit RefreshHeatingStatus(Service::INITIAL_RETORT);
    delete RetortPreTest;
}

void CInitialSystemCheck::SelectRetortCondition(quint64 Id)
{
    Q_UNUSED(Id);
    m_IsEmptyInRetort = true;
}

void CInitialSystemCheck::ConfirmParaffinBath(void)
{
    int ParaffinBath = mp_DataConnector->GetUserSettingInterface()->GetUserSettings()->GetTemperatureParaffinBath();

    QString Text = QString("Current paraffin melting point is %1\260C. If correct, press 'Yes'. If incorrect,"
                           " press 'No', then select the correct paraffin melting point").arg(ParaffinBath);
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);

    dlg->SetTitle(MSG_TITLE);
    dlg->SetIcon(QMessageBox::Question);
    dlg->SetText(Text);
    dlg->HideCenterButton();
    dlg->SetButtonText(1, tr("Yes"));
    dlg->SetButtonText(3, tr("No"));
    dlg->setModal(true);

    if (dlg->exec() == 0) {
        CSelectMeltingPointDialog* SelectDlg = new CSelectMeltingPointDialog(ParaffinBath, mp_Parent);
        (void)SelectDlg->exec();

        ParaffinBath = SelectDlg->GetMeltingPoint();
        delete SelectDlg;
    }

    delete dlg;

    m_ParaffinMeltPoint = ParaffinBath;

    DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SGlobal")->SetParameter("PMeltingPoint",
                                                                                          QString::number(ParaffinBath));

    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("Paraffin melting point is selected to be %1 centigrade.").arg(m_ParaffinMeltPoint));

    qDebug()<<"InitialSystemCheck Paraffin melting point :"<<m_ParaffinMeltPoint;
}

void CInitialSystemCheck::ConfirmRetortCondition(void)
{
    MainMenu::CMessageDlg* dlg = new MainMenu::CMessageDlg(mp_Parent);

    dlg->SetTitle(tr("Retort Condition"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Select the content/condition within the retort"));
    dlg->SetButtonText(1, tr("Empty"));
    dlg->SetButtonText(2, tr("Other Reagent"));
    dlg->SetButtonText(3, tr("Paraffin"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(quint64), this, SelectRetortCondition(quint64));
    dlg->setModal(true);
    int Ret = dlg->exec();
    if (Ret == 0) {
        m_IsParaffinInRetort = true;
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() <<
                                                   QString("The content/condition within the retort is selected to be Paraffin."));
    }
    else if (m_IsEmptyInRetort) {
        Ret = 2;
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() <<
                                                   QString("The content/condition within the retort is selected to be Empty."));
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() <<
                                                   QString("The content/condition within the retort is selected to be Other Reagent."));
    }

    DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SGlobal")->SetParameter("RetortCondition",
                                                                                          QString::number(Ret));
    qDebug()<<"InitialSystemCheck is Paraffin in Retort :"<<Ret;
}

} // namespace InitialSystem

} // namespace Diagnostics
