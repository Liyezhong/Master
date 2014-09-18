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

#include <QDebug>

#include "ui_InitialSystem.h"

#include "Global/Include/Utils.h"
#include "Core/Include/ServiceUtils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Diagnostics/Include/InitialSystem/InitialSystemCheck.h"

namespace Diagnostics {

CInitialSystem::CInitialSystem(Core::CServiceGUIConnector *p_DataConnector, QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    //mp_DataConnector(p_DataConnector),
    mp_Ui(new Ui::CInitialSystem)
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

    mp_WaitDlg = new MainMenu::CMessageDlg(parent);
    mp_WaitDlg->SetTitle(InitialSystem::MSG_TITLE);
    mp_WaitDlg->SetIcon(QMessageBox::Information);
    mp_WaitDlg->SetText(tr("System is initializing..."));
    mp_WaitDlg->HideAllButtons();
    mp_WaitDlg->setModal(true);

    mp_InitialSystemCheck = new InitialSystem::CInitialSystemCheck(p_DataConnector, parent);

    CONNECTSIGNALSLOT(mp_InitialSystemCheck, RefreshStatusToGUI(Service::InitialSystemTestType, int),
                      this, OnRefreshStatus(Service::InitialSystemTestType, int));

    CONNECTSIGNALSLOT(mp_Ui->mainDisplayBtn, clicked(), this, close());

    mp_StartTimer = new QTimer;
    mp_StartTimer->setSingleShot(true);
    mp_StartTimer->setInterval(1000);
    mp_StartTimer->start();
    CONNECTSIGNALSLOT(mp_StartTimer, timeout(), this, StartCheck());

}

CInitialSystem::~CInitialSystem()
{
    delete mp_Ui;
    delete mp_StartTimer;
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
    mp_WaitDlg->show();

    while(1) {
        if (ServiceDeviceProcess::Instance()) {
            mp_WaitDlg->hide();
            break;
        }
        Core::CServiceUtils::delay(1000);
    }

    mp_Ui->mrTestLabel->setText(tr("Mains relay self test..."));
    mp_InitialSystemCheck->Run();

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
        break;
    case Service::INITIAL_AC_VOLTAGE:
        mp_Ui->voltageCheckLabel->setPixmap(SetPixmap);
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
        break;
    default:
        qDebug()<<"invalid initial test module.";
        return;
    }
}

void CInitialSystem::UpdateOvenTestStatus()
{
    QPixmap PixMapPass(QString(":/Large/CheckBoxLarge/CheckBox-Checked_large_green.png"));
    QPixmap PixMapFail(QString(":/Large/CheckBoxLarge/CheckBox-Crossed_large_red.png"));
    QPixmap SetPixmap1, SetPixmap2;
    SetPixmap1 = (PixMapPass.scaled(40,40,Qt::KeepAspectRatio));
    SetPixmap2 = (PixMapFail.scaled(40,40,Qt::KeepAspectRatio));

    mp_Ui->ovenCheckLabel->setPixmap(SetPixmap1);
    mp_Ui->liquidCheckLabel->setPixmap(SetPixmap2);

    mp_Ui->targTempLabel->setText(tr("Target Temperature(\260C):"));
    mp_Ui->currentTemplabel->setText(tr("Current Temperature(top)(\260C):"));
    mp_Ui->bottomLabel1->setText(tr("Current Temperature(bottom1)(\260C):"));
    mp_Ui->bottomLable2->setText(tr("Current Temperature(bottom2)(\260C):"));
    mp_Ui->ovenToplabel->setText(tr("Paraffin Oven Current (top) (mA):"));
    mp_Ui->ovenBottomLabel->setText(tr("Paraffin Oven Current (bottom) (mA):"));

    mp_Ui->targetTempValue->setText(tr("65"));
    mp_Ui->currentTempValue->setText(tr("35"));
    mp_Ui->bottomValue1->setText(tr("37"));
    mp_Ui->bottomValue2->setText(tr("37"));
    mp_Ui->ovenTopValue->setText(tr("536"));
    mp_Ui->ovenBottomValue->setText(tr("537"));
}

void CInitialSystem::UpdateLiquidTestStatus()
{

}

void CInitialSystem::UpdateRVTestStatus()
{

}

void CInitialSystem::UpdateRetortTestStatus()
{

}

} // namespace Diagnostics


