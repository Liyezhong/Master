/****************************************************************************/
/*! \file FirmwareUpdate.cpp
 *
 *  \brief FirmwareUpdate implementation file
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-07-10
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Main/Include/HimalayaServiceEventCodes.h"
#include "ServiceUpdates/Include/FirmwareUpdate.h"
#include "DataManager/Containers/SWVersions/Include/SWVersionList.h"
#include "Global/Include/SystemPaths.h"

#include "Global/Include/Utils.h"

#include "ui_FirmwareUpdate.h"

namespace ServiceUpdates {

CFirmwareUpdate::CFirmwareUpdate(QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CFirmwareUpdate)
{
    mp_Ui->setupUi(this);

    RetranslateUI();

    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(380,500);

    mp_TableWidget->horizontalHeader()->show();

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->horizontalHeader()->resizeSection(0, 100);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 200);
    mp_TableWidget->horizontalHeader()->resizeSection(2, 200);

    mp_TableWidget->verticalHeader()->resizeSection(0,55);
    mp_TableWidget->verticalHeader()->resizeSection(1,55);

    mp_TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->SetContent(mp_TableWidget);

    InitData();

    CONNECTSIGNALSLOT(mp_Ui->updateBtn,
                      clicked(),
                      this,
                      UpdateFirmware());
}

CFirmwareUpdate::~CFirmwareUpdate(void)
{
    try
    {
        delete mp_TableWidget;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

void CFirmwareUpdate::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            mp_Ui->retranslateUi(this);
            this->RetranslateUI();
            break;
        default:
            break;
    }
}

void CFirmwareUpdate::InitData()
{
    DataManager::CSWVersionList SWVersionList;
    SWVersionList.SetDataVerificationMode(false);
    QString FirmWarePath = Global::SystemPaths::Instance().GetFirmwarePath();
    if (!SWVersionList.Read(FirmWarePath + QDir::separator() + "SW_Version.xml")) {
        qDebug()<<"CFirmwareUpdate:GetVersionInfo read SW_Version.xml file failed.";
        return;
    }

    for(int i = 0; i < SWVersionList.GetNumberOfSWDetails(); ++i) {
        DataManager::CSWDetails SlaveInfo;
        SWVersionList.GetSWDetails(i, SlaveInfo);
        QString Slave = SlaveInfo.GetSWName().remove(".bin");
        QString CurrentVersion = "CurrentVersion";
        QString LatestVersion = SlaveInfo.GetSWVersion();
        if (SlaveInfo.GetSWType() == FIRMWARE) {
            QDir tempDir;
            if (tempDir.exists(FirmWarePath + QDir::separator() + SlaveInfo.GetSWName())) {
                AddItem(Slave, CurrentVersion, LatestVersion);
            }
        }
    }
}

void CFirmwareUpdate::AddItem(QString& Slave, QString& CurrentVersion, QString& LatestVersion)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(Slave);
    ItemList << new QStandardItem(CurrentVersion);
    ItemList << new QStandardItem(LatestVersion);

    m_Model.appendRow(ItemList);
}

QString CFirmwareUpdate::GetVersionInfo(QString& Slave)
{
    DataManager::CSWVersionList SWVersionList;
    SWVersionList.SetDataVerificationMode(false);
    QString VersionConfigFile = Global::SystemPaths::Instance().GetFirmwarePath() + QDir::separator() + "SW_Version.xml";
    if (!SWVersionList.Read(VersionConfigFile)) {
        qDebug()<<"CFirmwareUpdate:GetVersionInfo read SW_Version.xml file failed.";
        return "";
    }
    DataManager::CSWDetails* SlaveInfo =  SWVersionList.GetSWDetails(Slave);
    if (SlaveInfo) {
        return SlaveInfo->GetSWVersion();
    }
    else {
        qDebug()<<"CFirmwareUpdate:GetVersionInfo failed for "<<Slave;
        return "";
    }
}

void CFirmwareUpdate::UpdateFirmware(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_SERVICEUPDATES_FIRMWARE_UPDATE);
    // to update firmware
}

void CFirmwareUpdate::RetranslateUI()
{
    m_Model.setHorizontalHeaderLabels(QStringList() << QApplication::translate("ServiceUpdates::CFirmwareUpdate",
                                                       "Slave", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("ServiceUpdates::CFirmwareUpdate",
                                                       "Current Version", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("ServiceUpdates::CFirmwareUpdate",
                                                       "Latest Version", 0, QApplication::UnicodeUTF8));
}

} // end namespace ServiceUpdates
