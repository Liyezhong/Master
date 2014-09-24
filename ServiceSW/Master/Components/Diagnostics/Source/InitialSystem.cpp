/****************************************************************************/
/*! \file InitialSystem.cpp
 *
 *  \brief Implementation of Initial system check.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-10
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

#include "Diagnostics/Include/InitialSystem.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include <QDebug>

#include "ui_InitialSystem.h"

#include "Global/Include/Utils.h"
#include "Core/Include/ServiceUtils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Diagnostics/Include/InitialSystem/InitialSystemCheck.h"

namespace Diagnostics {

CInitialSystem::CInitialSystem(Core::CServiceGUIConnector *p_DataConnector, QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent),
    //mp_DataConnector(p_DataConnector),
    mp_Ui(new Ui::CInitialSystem),
    m_IsHeatingTimerStart(false),
    m_OvenHeating(false),
    m_LiquidHeating(false),
    m_RVHeating(false),
    m_RetortHeating(false)
{
    mp_Ui->setupUi(GetContentFrame());

    this->SetDialogTitle(InitialSystem::MSG_TITLE);

    QPixmap PixMapCheck(QString::fromUtf8(":/Large/CheckBoxLarge/CheckBox-enabled-large.png"));
    QPixmap SetPixmap = PixMapCheck.scaled(QSize(40,40),Qt::KeepAspectRatio, Qt::FastTransformation);

    mp_Ui->ovenCheckLabel->setPixmap(SetPixmap);
    mp_Ui->liquidCheckLabel->setPixmap(SetPixmap);
    mp_Ui->rvCheckLabel->setPixmap(SetPixmap);
    mp_Ui->retortCheckLabel->setPixmap(SetPixmap);

    mp_Ui->ovenFrame->setBackgroundRole(QPalette::Dark);
    mp_Ui->liquidFrame->setBackgroundRole(QPalette::Dark);
    mp_Ui->rvFrame->setBackgroundRole(QPalette::Dark);
    mp_Ui->retortFrame->setBackgroundRole(QPalette::Dark);

    mp_Ui->mainRelayGroup->setFixedSize(776,122);
    mp_Ui->ovenGroup->setFixedSize(388,152);
    mp_Ui->rvGroup->setFixedSize(388,152);
    mp_Ui->liquidGroup->setFixedSize(388,152);
    mp_Ui->retortGroup->setFixedSize(388,152);

    mp_WaitDlg = new MainMenu::CMessageDlg(p_Parent);
    mp_WaitDlg->SetTitle(InitialSystem::MSG_TITLE);
    mp_WaitDlg->SetIcon(QMessageBox::Information);
    mp_WaitDlg->SetText(tr("System is initializing..."));
    mp_WaitDlg->HideAllButtons();
    mp_WaitDlg->setModal(true);

    mp_Ui->retortHeatingBtn->setEnabled(false);
    mp_Ui->mainDisplayBtn->setEnabled(false);

    mp_InitialSystemCheck = new InitialSystem::CInitialSystemCheck(p_DataConnector, p_Parent);

    CONNECTSIGNALSLOT(mp_InitialSystemCheck, RefreshStatusToGUI(Service::InitialSystemTestType, int),
                      this, OnRefreshStatus(Service::InitialSystemTestType, int));

    CONNECTSIGNALSLOT(mp_InitialSystemCheck, RefreshHeatingStatus(Service::InitialSystemTestType),
                      this, OnRefreshHeatingStatus(Service::InitialSystemTestType));

    CONNECTSIGNALSLOT(mp_Ui->mainDisplayBtn, clicked(), this, close());
    CONNECTSIGNALSLOT(mp_Ui->retortHeatingBtn, clicked(), mp_InitialSystemCheck, RetortPreHeating());

    mp_HeatingTimer = new QTimer;
    mp_HeatingTimer->setSingleShot(false);
    mp_HeatingTimer->setInterval(1000);
    CONNECTSIGNALSLOT(mp_HeatingTimer, timeout(), this, UpdateHeatingStatus());
    QTimer::singleShot(1000, this, SLOT(StartCheck()));
}

CInitialSystem::~CInitialSystem()
{
    try {
        delete mp_Ui;
        delete mp_WaitDlg;
        delete mp_InitialSystemCheck;
        delete mp_HeatingTimer;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CInitialSystem::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type())
    {
    case QEvent::LanguageChange:
    {
        mp_Ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

void CInitialSystem::StartCheck()
{
    qDebug()<<"Start Initial System Check..........";
    mp_WaitDlg->show();

    while(1) {
        if (ServiceDeviceProcess::Instance()->IsInitialized()) {
            mp_WaitDlg->hide();
            break;
        }
        Core::CServiceUtils::delay(1000);
    }

    mp_Ui->mrTestLabel->setText(tr("Mains relay self test..."));
    (void)mp_InitialSystemCheck->Run();
}

void CInitialSystem::UpdateHeatingStatus()
{
    qDebug()<<"Update Heating Status....";
    if (m_OvenHeating) {
        qDebug()<<"Pre-Heating Oven....";
        UpdateOvenHeatingStatus();
    }
    if (m_LiquidHeating) {
        qDebug()<<"Pre-Heating Liquid tube....";
        UpdateLHeatingStatus();
    }
    if (m_RVHeating) {
        qDebug()<<"Pre-Heating rotary valve....";
        UpdateRVHeatingStatus();
    }
    if (m_RetortHeating) {
        qDebug()<<"Pre-Heating Retort....";
        UpdateRetortStatus();
    }
}

void CInitialSystem::OnRefreshStatus(Service::InitialSystemTestType Type, int Ret)
{
    QPixmap PixMapPass(QString(":/Large/CheckBoxLarge/CheckBox-Checked_large_green.png"));
    QPixmap PixMapFail(QString(":/Large/CheckBoxLarge/CheckBox-Crossed_large_red.png"));
    QPixmap SetPixmap;

    if (Ret == RETURN_OK) {
        SetPixmap = PixMapPass.scaled(QSize(40,40),Qt::KeepAspectRatio, Qt::FastTransformation);
    }
    else {
        SetPixmap = PixMapFail.scaled(QSize(40,40),Qt::KeepAspectRatio, Qt::FastTransformation);
    }

    switch (Type) {
    case Service::INITIAL_MAINS_RELAY:
        mp_Ui->mrChcekLable->setPixmap(SetPixmap);

        if (Ret == RETURN_OK) {
            mp_Ui->voltageTestLabel->setText(tr("AC voltage auto-switch self test..."));
        }
        else {
            mp_Ui->mainDisplayBtn->setEnabled(true);
        }
        break;
    case Service::INITIAL_AC_VOLTAGE:
        mp_Ui->voltageCheckLabel->setPixmap(SetPixmap);

        if (Ret == RETURN_OK) {
            mp_Ui->preTestLabel->setText(tr("Pre-test ..."));
        }
        else {
            mp_Ui->mainDisplayBtn->setEnabled(true);
        }
        break;
    case Service::INITIAL_OVEN:
        mp_Ui->ovenCheckLabel->setPixmap(SetPixmap);
        break;
    case Service::INITIAL_LIQUID_TUBE:
        mp_Ui->liquidCheckLabel->setPixmap(SetPixmap);
        break;
    case Service::INITIAL_ROTARY_VALVE:
        mp_Ui->rvCheckLabel->setPixmap(SetPixmap);
        break;
    case Service::INITIAL_RETORT:
        mp_Ui->retortCheckLabel->setPixmap(SetPixmap);
        if (Ret == RETURN_OK) {
            mp_Ui->retortHeatingBtn->setEnabled(true);
        }
        mp_Ui->mainDisplayBtn->setEnabled(true);
        break;
    default:
        qDebug()<<"invalid initial test module.";
        return;
    }
}

void CInitialSystem::UpdateOvenHeatingStatus()
{
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    QString TargetTemp = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SOvenPreTest")->GetParameter("TargetTemp");
    qreal CurrentTemp(0);
    qreal CurrentTempB1(0);
    qreal CurrentTempB2(0);
    quint16 CurrentOvenTempT(0);
    quint16 CurrentOvenTempB(0);

    (void)p_DevProc->OvenGetTemp(&CurrentTemp, &CurrentTempB1, &CurrentTempB2);
    (void)p_DevProc->OvenGetCurrent(&CurrentOvenTempT, &CurrentOvenTempB);

    mp_Ui->ovenTargetTempLabel->setText(tr("Target Temperature(\260C):"));
    mp_Ui->ovenCurTemplabel->setText(tr("Current Temperature(top)(\260C):"));
    mp_Ui->ovenBtmLabel1->setText(tr("Current Temperature(bottom1)(\260C):"));
    mp_Ui->ovenBtmLable2->setText(tr("Current Temperature(bottom2)(\260C):"));
    mp_Ui->ovenCurrentLabel->setText(tr("Paraffin Oven Current(mA):"));

    mp_Ui->ovenTargetTempValue->setText(TargetTemp);
    mp_Ui->ovenCurTempValue->setText(QString::number(CurrentTemp));
    mp_Ui->ovenBtmValue1->setText(QString::number(CurrentTempB1));
    mp_Ui->ovenBtmValue2->setText(QString::number(CurrentTempB2));
    mp_Ui->ovenCurrentValue->setText(QString::number(CurrentOvenTempT));
}

void CInitialSystem::UpdateLHeatingStatus()
{
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    QString TargetTemp = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SLTubePreTest")->GetParameter("PreHeatingTargetTemp");
    quint16 TubeCurrent(0);
    qreal TubeTemp(0);

    (void)p_DevProc->LiquidTubeGetCurrent(&TubeCurrent);
    (void)p_DevProc->LiquidTubeGetTemp(&TubeTemp);

    mp_Ui->LTargetTmpLabel->setText(tr("Target Temperature(\260C):"));
    mp_Ui->LCurrentTempLabel->setText(tr("Current Temperature(\260C):"));
    mp_Ui->LTubeTempLabel->setText(tr("Liquid Heating Tube Current(mA):"));   

    mp_Ui->LTargetTmpValue->setText(TargetTemp);
    mp_Ui->LCurrentTempValue->setText(QString::number(TubeTemp));
    mp_Ui->LTubeTempValue->setText(QString::number(TubeCurrent));
}

void CInitialSystem::UpdateRVHeatingStatus()
{
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    QString TargetTempS1 = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRVPreTest")->GetParameter("PreHeatingTargetTemp");
    QString TargetTempS2 = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SGlobel")->GetParameter("PMeltingPoint");
    qreal CurrentTempS1(0);
    qreal CurrentTempS2(0);
    quint16 RVCurrent(0);

    (void)p_DevProc->RVGetTemp(&CurrentTempS1, &CurrentTempS2);
    (void)p_DevProc->RVGetCurrent(&RVCurrent);

    mp_Ui->rvTargetTempS1Label->setText(tr("Target Temperature(Sensor1)(\260C):"));
    mp_Ui->rvTargetTempS2Label->setText(tr("Target Temperature(Sensor2)(\260C):"));
    mp_Ui->rvCurTempS1Label->setText(tr("Current Temperature(Sensor1)(\260C):"));
    mp_Ui->rvCurTempS2Label->setText(tr("Current Temperature(Sensor2)(\260C):"));
    mp_Ui->rvCurrentLabel->setText(tr("Rotary valve Current(mA):"));

    mp_Ui->rvTargetTempS1Value->setText(TargetTempS1);
    mp_Ui->rvTargetTempS2Value->setText(TargetTempS2);
    mp_Ui->rvCurTempS1Value->setText(QString::number(CurrentTempS1));
    mp_Ui->rvCurTempS2Value->setText(QString::number(CurrentTempS2));
    mp_Ui->rvCurrentValue->setText(QString::number(RVCurrent));
}

void CInitialSystem::UpdateRetortStatus()
{
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    QString TargetTemp = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortPreTest")->GetParameter("TargetTemp");
    qDebug()<<"get retort Target temp :"<<TargetTemp;
    qreal CurrentTempS(0);
    qreal CurrentTempB1(0);
    qreal CurrentTempB2(0);
    quint16 RetortCurrent(0);

    (void)p_DevProc->RetortGetTemp(&CurrentTempS, &CurrentTempB1, &CurrentTempB2);
    (void)p_DevProc->RetortGetCurrent(&RetortCurrent, NULL);

    mp_Ui->retortTargetTempLabel->setText(tr("Target Temperature(\260C):"));
    mp_Ui->retortCurTempSLabel->setText(tr("Current Temperature(Side)(\260C):"));
    mp_Ui->retortCurTempB1Label->setText(tr("Current Temperature(Bottom1)(\260C):"));
    mp_Ui->retortCurTempB2Label->setText(tr("Current Temperature(Bottom2)(\260C):"));
    mp_Ui->retortCurrentLabel->setText(tr("Retort Current(mA):"));

    mp_Ui->retortTargetTempValue->setText(TargetTemp);
    mp_Ui->retortCurTempSValue->setText(QString::number(CurrentTempS));
    mp_Ui->retortCurTempB1Value->setText(QString::number(CurrentTempB1));
    mp_Ui->retortCurTempB2Value->setText(QString::number(CurrentTempB2));
    mp_Ui->retortCurrentValue->setText(QString::number(RetortCurrent));
}

void CInitialSystem::OnRefreshHeatingStatus(Service::InitialSystemTestType Type)
{
    if (!m_IsHeatingTimerStart) {
        mp_HeatingTimer->start();
        m_IsHeatingTimerStart = true;
    }

    switch (Type) {
    case Service::INITIAL_OVEN:
        m_OvenHeating = true;
        break;
    case Service::INITIAL_LIQUID_TUBE:
        m_LiquidHeating = true;
        break;
    case Service::INITIAL_ROTARY_VALVE:
        m_RVHeating = true;
        break;
    case Service::INITIAL_RETORT:
        m_RetortHeating = true;
        break;
    default:
        qDebug()<<"invalid initial test module.";
        return;
    }
}

} // namespace Diagnostics


