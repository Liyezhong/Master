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
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "Diagnostics/Include/InitialSystem/ACVoltageTest.h"
#include "Diagnostics/Include/InitialSystem/LTubePreTest.h"
#include "Diagnostics/Include/InitialSystem/MainsRelayTest.h"
#include "Diagnostics/Include/InitialSystem/OvenPreTest.h"
#include "Diagnostics/Include/InitialSystem/RetortPreTest.h"
#include "Diagnostics/Include/InitialSystem/RVPreTest.h"
#include "Diagnostics/Include/InitialSystem/InitialSystemCheck.h"

namespace Diagnostics {

CInitialSystem::CInitialSystem(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    mp_Ui(new Ui::CInitialSystem)
{
    mp_Ui->setupUi(GetContentFrame());

    this->SetDialogTitle(tr("Initial System Check"));

    QPixmap PixMapCheck(QString::fromUtf8(":/Large/CheckBoxLarge/CheckBox-enabled-large.png"));
    QPixmap PixMapPass(QString(":/Large/CheckBoxLarge/CheckBox-Checked_large_green.png"));
    QPixmap PixMapFail(QString(":/Large/CheckBoxLarge/CheckBox-Crossed_large_red.png"));

    m_PixmapCheck = PixMapCheck.scaled(QSize(40,40),Qt::KeepAspectRatio, Qt::FastTransformation);
    m_PixmapPass  = PixMapPass.scaled(QSize(40,40),Qt::KeepAspectRatio, Qt::FastTransformation);
    m_PixmapFail  = PixMapFail.scaled(QSize(40,40),Qt::KeepAspectRatio, Qt::FastTransformation);

    mp_Ui->ovenCheckLabel->setPixmap(m_PixmapCheck);
    mp_Ui->liquidCheckLabel->setPixmap(m_PixmapCheck);
    mp_Ui->rvCheckLabel->setPixmap(m_PixmapCheck);
    mp_Ui->retortCheckLabel->setPixmap(m_PixmapCheck);

    mp_Ui->ovenFrame->setBackgroundRole(QPalette::Dark);
    mp_Ui->liquidFrame->setBackgroundRole(QPalette::Dark);
    mp_Ui->rvFrame->setBackgroundRole(QPalette::Dark);
    mp_Ui->retortFrame->setBackgroundRole(QPalette::Dark);

    mp_Ui->mainRelayGroup->setFixedSize(776,122);
    mp_Ui->ovenGroup->setFixedSize(388,152);
    mp_Ui->rvGroup->setFixedSize(388,152);
    mp_Ui->liquidGroup->setFixedSize(388,152);
    mp_Ui->retortGroup->setFixedSize(388,152);

    CONNECTSIGNALSLOT(mp_Ui->mainDisplayBtn, clicked(), this, close());

    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(15000);
    timer.start();
    CONNECTSIGNALSLOT(&timer, timeout(), this, StartCheck());

    //CONNECTSIGNALSLOT(mp_Ui->retortHeatingBtn, clicked(), this, UpdateMainRelayStatus());
    //CONNECTSIGNALSLOT(mp_Ui->retortHeatingBtn, clicked(), this, UpdateOvenTestStatus());
}

CInitialSystem::~CInitialSystem()
{
    delete mp_Ui;
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
    mp_Ui->mrTestLabel->setText(tr("Mains relay self test..."));
    Core::CServiceUtils::delay(3000);
    InitialSystem::CMainsRelayTest MainsRelayTest(this);
    if (MainsRelayTest.Run() == RETURN_OK) {
        mp_Ui->mrChcekLable->setPixmap(m_PixmapPass);
    }
    else {
        mp_Ui->mrChcekLable->setPixmap(m_PixmapFail);
        return;
    }

    mp_Ui->voltageTestLabel->setText(tr("AC voltage auto-switch self test..."));
    Core::CServiceUtils::delay(3000);
    InitialSystem::CACVoltageTest ACVoltageTest(this);
    if (ACVoltageTest.Run() == RETURN_OK) {
        mp_Ui->voltageCheckLabel->setPixmap(m_PixmapPass);
    }
    else {
        mp_Ui->voltageCheckLabel->setPixmap(m_PixmapFail);
        return;
    }
}

void CInitialSystem::UpdateMainRelayStatus()
{
    //mp_Ui->mainRelayGroup->setFixedSize(782, 116);
    mp_Ui->mrTestLabel->setText(tr("Mains relay self test..."));
    mp_Ui->voltageTestLabel->setText(tr("AC voltage auto-switch self test..."));
    mp_Ui->preTestLabel->setText(tr("Pre test..."));

    //QPixmap CheckMap(QString::fromUtf8(":/Large/CheckBoxLarge/CheckBox-enabled-large.png"));
    QPixmap PixMapPass(QString(":/Large/CheckBoxLarge/CheckBox-Checked_large_green.png"));
    //QPixmap PixMapFail(QString(":/Large/CheckBoxLarge/CheckBox-Crossed_large_red.png"));
    QPixmap SetPixmap;
    SetPixmap = PixMapPass.scaled(QSize(40,40),Qt::KeepAspectRatio, Qt::FastTransformation);

    mp_Ui->mrChcekLable->setPixmap(SetPixmap);
    mp_Ui->voltageCheckLabel->setPixmap(SetPixmap);
    //mp_Ui->preCheckLable->setPixmap(SetPixmap);
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


