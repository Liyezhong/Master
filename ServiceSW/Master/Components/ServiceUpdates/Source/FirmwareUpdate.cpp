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


#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include "ui_FirmwareUpdate.h"

namespace ServiceUpdates {

CFirmwareUpdate::CFirmwareUpdate(Core::CServiceGUIConnector *p_DataConnector, QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CFirmwareUpdate)
    , mp_DataConnector(p_DataConnector)
    , mp_Module(NULL)
    , m_Result(false)
{
    mp_Ui->setupUi(this);
    RetranslateUI();
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(380,500);

    mp_TableWidget->horizontalHeader()->show();

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->horizontalHeader()->resizeSection(0, 105);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 150);
    mp_TableWidget->horizontalHeader()->resizeSection(2, 150);
    mp_TableWidget->horizontalHeader()->resizeSection(3, 100);

    mp_TableWidget->verticalHeader()->resizeSection(0,55);
    mp_TableWidget->verticalHeader()->resizeSection(1,55);

    mp_TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->SetContent(mp_TableWidget);

    //InitData();

    CONNECTSIGNALSLOT(mp_Ui->updateBtn, clicked(), this, UpdateFirmware());
    CONNECTSIGNALSLOTGUI(this, UpdateModule(ServiceDataManager::CModule&), mp_DataConnector, SendModuleUpdate(ServiceDataManager::CModule&));
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

void CFirmwareUpdate::UpdateGUI()
{
    QString latestVersion = "";
    ServiceDataManager::CSubModule* SlaveModule = NULL;

    if (mp_DataConnector->GetModuleListContainer()) {
        mp_Module = mp_DataConnector->GetModuleListContainer()->GetModule("Main Control");
    }   
    if (mp_Module) {
        m_Model.clear();
        RetranslateUI();
        /*
        for (int i = 1; i < m_Model.rowCount(); ++i) {
            m_Model.removeRow(i);
        }*/
        for (int j = 0; j < mp_Module->GetNumberofSubModules(); ++j) {
            SlaveModule = mp_Module->GetSubModuleInfo(j);
            if (SlaveModule->GetSubModuleName().contains("ASB")) {
                QString SlaveName = SlaveModule->GetSubModuleName();
                QString SlaveVersion = SlaveModule->GetParameterInfo("SoftwareVersion")->ParameterValue;
                AddItem(SlaveName, SlaveVersion, latestVersion);
            }
        }
    }

    RefreshLatestVersion();
}

void CFirmwareUpdate::AddItem(QString& Slave, QString& CurrentVersion, QString& LatestVersion)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(Slave);
    ItemList << new QStandardItem(CurrentVersion);
    ItemList << new QStandardItem(LatestVersion);

    QPixmap SetPixmap;
    QPixmap PixMap(QString(":/Large/CheckBoxLarge/CheckBox-enabled-large.png"));
    if (!PixMap.isNull())
        SetPixmap = (PixMap.scaled(QSize(45,45),Qt::KeepAspectRatio, Qt::FastTransformation));

    QStandardItem *item = new QStandardItem;
    item->setData(SetPixmap, (int) Qt::DecorationRole);
    ItemList << item;

    m_Model.appendRow(ItemList);
}

QStandardItem* CFirmwareUpdate::FindLastVersionItem(QString& Slave)
{
    QStandardItem* item = NULL;
    for(int i=0; i<m_Model.rowCount(); i++) {
        if (m_Model.item(i, 0)->text() == Slave) {
            item = m_Model.item(i, 2);
            break;
        }
    }
    return item;
}

void CFirmwareUpdate::RefreshLatestVersion()
{
    DataManager::CSWVersionList SWVersionList;
    SWVersionList.SetDataVerificationMode(false);
    QString FirmWarePath = Global::SystemPaths::Instance().GetFirmwarePath();

    if (!SWVersionList.Read(FirmWarePath + QDir::separator() + "FW_Version.xml")) {
        qDebug()<<"CFirmwareUpdate:GetVersionInfo read config file failed. the file is"<<FirmWarePath + QDir::separator() + "FW_Version.xml";
        return;
    }

    QString CurrentVersion = "";
    for(int i = 0; i < SWVersionList.GetNumberOfSWDetails(); ++i) {
        DataManager::CSWDetails SlaveInfo;
        SWVersionList.GetSWDetails(i, SlaveInfo);
        QString Slave = SlaveInfo.GetSWName().remove(".bin");
        QString LatestVersion = SlaveInfo.GetSWVersion();
        if (SlaveInfo.GetSWType() == FIRMWARE) {
            QDir tempDir;
            if (tempDir.exists(FirmWarePath + QDir::separator() + SlaveInfo.GetSWName())) {
                QStandardItem* item = FindLastVersionItem(Slave);
                if (item) {
                    item->setText(LatestVersion);
                }
                else {
                    AddItem(Slave, CurrentVersion, LatestVersion);
                }
            }
        }
    }
}

void CFirmwareUpdate::UpdateFirmware(void)
{
    mp_Ui->updateBtn->setEnabled(false);

    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_SERVICEUPDATES_FIRMWARE_UPDATE);
    // to update firmware

    QList<Service::ModuleTestCaseID> TestCaseList;
    TestCaseList.append(Service::FIRMWARE_UPDATE);
    TestCaseList.append(Service::FIRMWARE_GET_SLAVE_INFO);


    Service::ModuleTestCaseID Id = Service::FIRMWARE_UPDATE;
    QString SlaveTypes[] = {"3", "5", "15"};
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    QString TestCaseName1 = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::FIRMWARE_GET_SLAVE_INFO);
    DataManager::CTestCase* p_TestCase1 = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName1);
    p_TestCase1->SetParameter("TestType", "UpdateFirmware");

    int num = 0;
    for(int i=0; i<m_Model.rowCount(); i++) {
        if (m_Model.item(i, 1)->text() == m_Model.item(i, 2)->text()) {
            num++;
            continue;
        }

        QString FirmWarePath = Global::SystemPaths::Instance().GetFirmwarePath() + QDir::separator() + m_Model.item(i, 0)->text() + ".bin";

        qDebug()<<"Firmware Path = "<< FirmWarePath;

        p_TestCase->SetParameter("Path", FirmWarePath);
        p_TestCase->SetParameter("SlaveType", SlaveTypes[i]);
        p_TestCase->SetParameter("Index", QString("%1").arg(i));

        p_TestCase1->SetParameter("SlaveType", SlaveTypes[i]);

        emit BeginModuleTest(Service::FIRMWARE, TestCaseList);

        if (m_Result) {
            m_Model.item(i, 1)->setText(m_Model.item(i, 2)->text());
        }

        emit UpdateModule(*mp_Module);
    }

    if (num == 3) {

        MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(this);
        dlg->SetTitle(QApplication::translate("ServiceUpdates::CFirmwareUpdate", "Information Message",
                                                        0, QApplication::UnicodeUTF8));
        dlg->SetIcon(QMessageBox::Information);
        dlg->SetButtonText(1, QApplication::translate("ServiceUpdates::CFirmwareUpdate", "Ok",
                                                                0, QApplication::UnicodeUTF8));

        dlg->SetText(QApplication::translate("ServiceUpdates::CFirmwareUpdate", "The current firmware version has been latest",
                                             0, QApplication::UnicodeUTF8));
        dlg->setModal(true);
        dlg->HideButtons();
        dlg->exec();
        delete dlg;
    }

    mp_Ui->updateBtn->setEnabled(true);
}

/****************************************************************************/
/*!
 *  \brief Set Result for Update
 *  \iparam Index = ASB Index
 *  \iparam Result = Result of the test
 */
/****************************************************************************/
void CFirmwareUpdate::SetUpdateResult(int Index, bool Result)
{
    QPixmap PixMapPass(QString(":/Large/CheckBoxLarge/CheckBox-Checked_large_green.png"));
    QPixmap PixMapFail(QString(":/Large/CheckBoxLarge/CheckBox-Crossed_large_red.png"));
    QPixmap SetPixMap;

    m_Result = Result;

    if (Result) {
        if (!PixMapPass.isNull())
            SetPixMap = (PixMapPass.scaled(45,45,Qt::KeepAspectRatio));
    }
    else {
        if (!PixMapFail.isNull())
            SetPixMap = (PixMapFail.scaled(45,45,Qt::KeepAspectRatio));
    }

    (void) m_Model.setData(m_Model.index(Index, 3), SetPixMap, (int) Qt::DecorationRole);

}

void CFirmwareUpdate::RetranslateUI()
{
    m_Model.setHorizontalHeaderLabels(QStringList() << QApplication::translate("ServiceUpdates::CFirmwareUpdate",
                                                       "Slave", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("ServiceUpdates::CFirmwareUpdate",
                                                       "Current Version", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("ServiceUpdates::CFirmwareUpdate",
                                                       "Latest Version", 0, QApplication::UnicodeUTF8)
                                                    << "");
}

} // end namespace ServiceUpdates
