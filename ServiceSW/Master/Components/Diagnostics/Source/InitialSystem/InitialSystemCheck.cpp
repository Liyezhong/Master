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

#include "Diagnostics/Include/InitialSystem/ACVoltageTest.h"
#include "Diagnostics/Include/InitialSystem/LTubePreTest.h"
#include "Diagnostics/Include/InitialSystem/MainsRelayTest.h"
#include "Diagnostics/Include/InitialSystem/OvenPreTest.h"
#include "Diagnostics/Include/InitialSystem/RetortPreTest.h"
#include "Diagnostics/Include/InitialSystem/RVPreTest.h"
#include "Diagnostics/Include/SelectMeltingPointDialog.h"

#include <QDebug>

namespace Diagnostics {

namespace InitialSystem {

CInitialSystemCheck::CInitialSystemCheck(Core::CServiceGUIConnector *p_DataConnector, QWidget *parent)
    : CTestBase(parent),
      mp_Parent(parent),
      mp_DataConnector(p_DataConnector),
      m_ParaffinMeltPoint(0),
      m_IsParaffinInRetort(false)
{

}

CInitialSystemCheck::~CInitialSystemCheck(void)
{
}

int CInitialSystemCheck::Run(void)
{   
     int Ret = 0;

    CMainsRelayTest *MainsRelayTest = new CMainsRelayTest(mp_Parent);
    qDebug()<<"Start MainsRelayTest";

    Ret = MainsRelayTest->Run();

    //Ret = RETURN_OK; // only for test

    emit RefreshStatusToGUI(Service::INITIAL_MAINS_RELAY, Ret);
    delete MainsRelayTest;

    if (Ret != RETURN_OK) {
        return Ret;
    }
    ServiceDeviceProcess::Instance()->Pause(1000);

    qDebug()<<"Start ACVoltageTest";

    CACVoltageTest *ACVoltageTest = new CACVoltageTest(mp_Parent);
    Ret = ACVoltageTest->Run();

    //Ret = RETURN_OK; // only for test

    emit RefreshStatusToGUI(Service::INITIAL_AC_VOLTAGE, Ret);
    delete ACVoltageTest;

    if (Ret != RETURN_OK) {
        return Ret;
    }
    ServiceDeviceProcess::Instance()->Pause(1000);

    ConfirmParaffinBath();
    ConfirmRetortCondition();

    ServiceDeviceProcess::Instance()->MainRelaySetOnOff(true);
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

    CRetortPreTest *RetortPreTest = new CRetortPreTest(mp_Parent);
    Ret = RetortPreTest->Run();
    emit RefreshStatusToGUI(Service::INITIAL_RETORT, Ret);
    delete RetortPreTest;

    return Ret;
}

void CInitialSystemCheck::RetortPreHeating()
{
    CRetortPreTest *RetortPreTest = new CRetortPreTest(mp_Parent);
    RetortPreTest->StartPreHeating(m_ParaffinMeltPoint);
    emit RefreshHeatingStatus(Service::INITIAL_RETORT);
}

void CInitialSystemCheck::ConfirmParaffinBath(void)
{
    int ParaffinBath = mp_DataConnector->GetUserSettingInterface()->GetUserSettings()->GetTemperatureParaffinBath();
    QString Text = QString("Current paraffin melting point is %1. If correct, press 'Yes'. If incorrect,press 'No', then select the correct paraffinmelting point").arg(ParaffinBath);
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);

    dlg->SetTitle(MSG_TITLE);
    dlg->SetIcon(QMessageBox::Information);
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

    dlg->setModal(true);
    if (dlg->exec() == 0) {
        m_IsParaffinInRetort = true;
    }
    else {
        m_IsParaffinInRetort = false;
    }

    qDebug()<<"InitialSystemCheck is Paraffin in Retort :"<<m_IsParaffinInRetort;
}

} // namespace InitialSystem

} // namespace Diagnostics
