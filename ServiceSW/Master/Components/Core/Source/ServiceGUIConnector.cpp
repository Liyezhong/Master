/****************************************************************************/
/*! \file ServiceGUIConnector.cpp
 *
 *  \brief ServiceGUIConnector implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-27
 *   $Author:  $ Srivathsa HH
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Core/Include/ServiceGUIConnector.h"
#include <QApplication>
#include <QHash>
#include "Global/Include/UITranslator.h"
#include "MainMenu//Include/MsgBoxManager.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include <Global/Include/SystemPaths.h>
#include <Core/Include/CMessageString.h>

namespace Core {


/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CServiceGUIConnector::CServiceGUIConnector(MainMenu::CMainWindow *p_Parent)
    : mp_MainWindow(p_Parent)
    , mp_LanguageFile(NULL)
    , mp_OldFile(NULL)
    , m_GuiInit(true)
    , m_MessageDlg(false)
    , m_Archive(true)
    , mp_ModuleList(0)
    , m_CurrentTabIndex(0)
    , mp_SettingsInterface(0)
    , mp_ModuleListArchive(NULL)
    , mp_DeviceConfigurationInterface(NULL)
    , m_Language(PlatformService::DEUTSCH)
    , mp_ServiceParameters(NULL)
{
    CONNECTSIGNALSLOT(mp_MainWindow, onChangeEvent(), this, RetranslateUI());

    mp_MessageDlg = new MainMenu::CMessageDlg(mp_MainWindow);
    mp_MessageDlg->SetButtonText(1, tr("Ok"));
    mp_MessageDlg->setModal(true);
    mp_MessageDlg->HideButtons();

    mp_WaitDialog = new MainMenu::CWaitDialog(mp_MainWindow);
    mp_WaitDialog->SetDialogTitle(tr("Please Wait"));
    mp_WaitDialog->SetText(tr("Requested Operation in Progress ..."));

    if(!connect(mp_MessageDlg, SIGNAL(accepted()), this, SLOT(onMessageDialogClosed()))){
        qDebug() << "CStartup: cannot connect 'accepted' signal";
    }

    CONNECTSIGNALSLOT(mp_WaitDialog, Timeout(), mp_MessageDlg, Show());

    // MsgBox Manager
    mp_MesgBoxManager  = new MainMenu::CMsgBoxManager(mp_MainWindow, mp_SettingsInterface);
    (void)connect(mp_MesgBoxManager, SIGNAL(EventReportAck(NetCommands::ClickedButton_t,Global::tRefType, quint64)),
                      this, SLOT(OnEventReportAck(NetCommands::ClickedButton_t,Global::tRefType, quint64)));
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CServiceGUIConnector::~CServiceGUIConnector()
{
    try {
        delete mp_MesgBoxManager;
        delete mp_WaitDialog;
        delete mp_MessageDlg;
        delete mp_LanguageFile;
    }
    catch (...) {
        //To please lint warnings
    }
}


/****************************************************************************/
/*!
 *  \brief This slot updates the Modulelist
 */
/****************************************************************************/
void CServiceGUIConnector::SetModuleListContainer(ServiceDataManager::CModuleDataList *ModuleList, ServiceDataManager::CInstrumentHistory *ModuleListArchive)
{
    mp_ModuleList = ModuleList;
    mp_ModuleListArchive = ModuleListArchive;
    emit ModuleListChanged();
}

void CServiceGUIConnector::SetDeviceConfigurationInterface(
        DataManager::CDeviceConfigurationInterface *DeviceConfigInterface)
{
    mp_DeviceConfigurationInterface = DeviceConfigInterface;

    emit DeviceConfigurationInterfaceChanged();
}

void CServiceGUIConnector::SetUserSettingInterface(
        DataManager::CUserSettingsInterface *UserSettingInterface)
{
    mp_SettingsInterface = UserSettingInterface;

    emit UserSettingInterfaceChanged();
}

/****************************************************************************/
/*!
 *  \brief This slot updates the Modulelist
 *  \iparam Module = Module date container
 */
/****************************************************************************/
void CServiceGUIConnector::SendModuleUpdate(ServiceDataManager::CModule &Module)
{
    if (!mp_ModuleList) {
        qDebug()<<"CServiceGuiConnector::SendModuleUpdate invalid module list.";
        return;
    }
    (void)mp_ModuleList->UpdateModule(&Module);
    if (!mp_ModuleList->Write()) {
        qDebug()<<"CServiceGUIConnector: SendModuluUpdate to file failed.";
    }
    qDebug() << Module.GetSerialNumber() << endl;
    qDebug() << "Module Updated";
}

bool CServiceGUIConnector::UpdateInstrumentHistory(ServiceDataManager::CModuleDataList& ModuleList)
{
    bool Result(false);

    if(mp_MainWindow->GetSaMUserMode() == "Service") {
        try {
            CHECKPTR(mp_ModuleListArchive)

            if (m_Archive) {
                (void) mp_ModuleListArchive->AddModuleListInfo(mp_ModuleList);
                Result = mp_ModuleListArchive->Write();
                m_Archive = false;
                if (!Result) {
                    return Result;
                }
            }
        }
        CATCHALL();
    }
    try {
        CHECKPTR(mp_ModuleList)

        *mp_ModuleList = ModuleList;
        QDateTime DateTime = QDateTime::currentDateTime();
        QString CurrentDateTime = DateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
        mp_ModuleList->SetModuleTimeStamp(CurrentDateTime);

            /* Emit a signal to update module list */
        Result = mp_ModuleList->Write();
    }
    CATCHALL();

    return Result;
}

/****************************************************************************/
/*!
 *  \brief This slot updates the Device configuration
 *  \iparam DeviceConfiguration = pointer of device configuration
 */
/****************************************************************************/
void CServiceGUIConnector::SendDeviceConfigurationUpdate(DataManager::CDeviceConfiguration* DeviceConfiguration)
{
    if (mp_DeviceConfigurationInterface) {
        (void)mp_DeviceConfigurationInterface->UpdateDeviceConfiguration(DeviceConfiguration);
        if (!mp_DeviceConfigurationInterface->Write()) {
            qDebug()<<"CServiceGUIConnector: UpdateDeviceConfigurationSN to file failed.";
        }
    }
}

/****************************************************************************/
/*!
 *  \brief This slot updates the Modulelist
 *  \iparam Module = Module date container
 */
/****************************************************************************/
void CServiceGUIConnector::SendAddModule(ServiceDataManager::CModule &Module)
{
    if (mp_ModuleList) {
        (void)mp_ModuleList->AddModule(&Module);
        qDebug() << Module.GetSerialNumber() << endl;
        qDebug() << "Module Updated";
    }
}

/****************************************************************************/
/*!
 *  \brief onMessageDialogClosed
 */
/****************************************************************************/
void CServiceGUIConnector::onMessageDialogClosed()
{
    m_MessageDlg = false;
}
/****************************************************************************/
/*!
 *  \brief OnCurrentTabChanged
 *  \iparam CurrentTabIndex = Index of current tab
 */
/****************************************************************************/
void CServiceGUIConnector::OnCurrentTabChanged(int CurrentTabIndex)
{
    m_CurrentTabIndex = CurrentTabIndex;
}

/****************************************************************************/
/*!
 *  \brief Show Busy Dialog
 *  \iparam MessageText = message string
 *  \iparam HideAbort = flag of hide dialog
 */
/****************************************************************************/
void CServiceGUIConnector::ShowBusyDialog(QString MessageText, bool HideAbort)
{
    if(m_MessageDlg)
    {
        return;
    }
    mp_WaitDialog->SetText(MessageText);
    mp_WaitDialog->show();
    mp_WaitDialog->setModal(false);
    if(HideAbort)
    {
        mp_WaitDialog->HideAbort();
    }
}

/****************************************************************************/
/*!
 *  \brief Show Busy Dialog
 */
/****************************************************************************/
void CServiceGUIConnector::ShowBusyDialog()
{
    if(m_MessageDlg)
    {
        return;
    }
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(10000);
    timer.start();
    CONNECTSIGNALSLOT(&timer, timeout(), this, HandleTimeout());
    mp_WaitDialog->SetText(Service::CMessageString::MSG_DIAGNOSTICS_REQUESTED_OPERATION);
    mp_WaitDialog->show();
    mp_WaitDialog->HideAbort();
}

/****************************************************************************/
/*!
 *  \brief Hide Busy Dialog
 */
/****************************************************************************/
void CServiceGUIConnector::HideBusyDialog()
{
     (void)mp_WaitDialog->close();
}

/****************************************************************************/
/*!
 *  \brief Slot to Get PopUp Response
 *  \iparam button = index of button
 */
/****************************************************************************/
void CServiceGUIConnector::onPopUpButtonClicked(qint32 button)
{
    if (m_LoopSetAction.isRunning()) {
        m_LoopSetAction.exit(button);
    }
}

/****************************************************************************/
/*!
 *  \brief ShowMessageDialog
 *  \iparam MessageType = The message type
 *  \iparam MessageText = The message string
 *  \iparam NeedClickFlag = The flag of need click
 */
/****************************************************************************/
void CServiceGUIConnector::ShowMessageDialog(Global::GUIMessageType MessageType, QString MessageText, bool NeedClickFlag)
{
    qDebug()<<"ShowMessageDialog -- "<<MessageText;

    m_MessageDlg = true;
    mp_MessageDlg->hide();

    if (MessageText.length() > 0) {
        switch (MessageType) {
        case Global::GUIMSGTYPE_ERROR:
            mp_MessageDlg->SetIcon(QMessageBox::Critical);
            mp_MessageDlg->SetTitle(Service::CMessageString::MSG_TITLE_ERROR);
            break;
        case Global::GUIMSGTYPE_INFO:
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->SetTitle(Service::CMessageString::MSG_TITLE_INFO);
            break;
        case Global::GUIMSGTYPE_WARNING:
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(Service::CMessageString::MSG_TITLE_WARN);
            break;
        default:
            break;
        }
        mp_MessageDlg->SetText(MessageText);
    }
    else {
        mp_MessageDlg->SetTitle(Service::CMessageString::MSG_TITLE_COMMUNCATION_ERROR);
        mp_MessageDlg->SetText(Service::CMessageString::MSG_TITLE_COMMUNCATION_ERROR);
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
    }
    mp_MessageDlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_OK);
    //mp_MessageDlg->setModal(false);

    if (NeedClickFlag) {
        mp_MessageDlg->setModal(true);
        (void)mp_MessageDlg->exec();
        (void)mp_MessageDlg->close();
        delete mp_MessageDlg;
        mp_MessageDlg = NULL;

        mp_MessageDlg = new MainMenu::CMessageDlg(mp_MainWindow);
        mp_MessageDlg->SetButtonText(1,Service::CMessageString::MSG_BUTTON_OK);
        mp_MessageDlg->setModal(true);
        mp_MessageDlg->HideButtons();
    }
    else {
        mp_MessageDlg->show();
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CServiceGUIConnector::RetranslateUI()
{
   mp_WaitDialog->SetDialogTitle(QApplication::translate("Core::CDataConnector", "Device Communication", 0, QApplication::UnicodeUTF8));
   mp_WaitDialog->SetText(QApplication::translate("Core::CDataConnector", "Saving Settings ...", 0, QApplication::UnicodeUTF8));
   mp_WaitDialog->SetDialogTitle(QApplication::translate("Core::CDataConnector", "Disconnect", 0, QApplication::UnicodeUTF8));
   mp_WaitDialog->SetDialogTitle(QApplication::translate("Core::CDataConnector", "Startup", 0, QApplication::UnicodeUTF8));
   mp_WaitDialog->SetText(QApplication::translate("Core::CDataConnector", "Initializing device communication ...", 0, QApplication::UnicodeUTF8));
   mp_WaitDialog->SetText(QApplication::translate("Core::CDataConnector", "Reinitializing device communication ...", 0, QApplication::UnicodeUTF8));
   mp_MessageDlg->SetDialogTitle(QApplication::translate("Core::CDataConnector", "Communication Error", 0, QApplication::UnicodeUTF8));
   mp_MessageDlg->SetText(QApplication::translate("Core::CDataConnector", "The changes could not be saved.", 0, QApplication::UnicodeUTF8));
   mp_MessageDlg->SetButtonText(1,QApplication::translate("Core::CDataConnector", "Ok", 0, QApplication::UnicodeUTF8));
}

void CServiceGUIConnector::SetLanguage(PlatformService::Languages_t SelectedLanguage)
{
    QDir Directory(Global::SystemPaths::Instance().GetTranslationsPath());
    Directory.setFilter(QDir::AllEntries);
    QStringList FileNames;
    QFileInfoList List = Directory.entryInfoList();
    for (int i = 0; i < List.size(); i++) {
        QFileInfo fileInfo = List.at(i);
        FileNames.append(fileInfo.fileName());
    }

    if (m_Language != SelectedLanguage) {
        if (SelectedLanguage == PlatformService::US_ENGLISH) {
            if (FileNames.contains("HimalayaService_en.qm")) {
                DataManager::CTestCaseGuide::Instance().Clear();
                (void)DataManager::CTestCaseGuide::Instance().InitData(Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseGuide_en.xml");

                QString FilePath = Global::SystemPaths::Instance().GetTranslationsPath() + "/HimalayaService_en.qm";
                (void) m_Translator.load(FilePath);
                qApp->installTranslator(&m_Translator);
            } else {
                mp_MessageDlg->SetTitle(QApplication::translate("ServiceUpdates::CLanguageWidget", "Language files",
                                                                0, QApplication::UnicodeUTF8));
                mp_MessageDlg->SetText(QApplication::translate("ServiceUpdates::CLanguageWidget",
                                       "Translation files are missing.", 0, QApplication::UnicodeUTF8));
                mp_MessageDlg->SetButtonText(1, QApplication::translate("ServiceUpdates::CLanguageWidget", "Ok",
                                                                        0, QApplication::UnicodeUTF8));
                mp_MessageDlg->HideButtons();
                mp_MessageDlg->SetIcon(QMessageBox::Critical);
                mp_MessageDlg->Show();
            }
        } else {
            if (FileNames.contains("HimalayaService_zh.qm")) {
                DataManager::CTestCaseGuide::Instance().Clear();
                (void)DataManager::CTestCaseGuide::Instance().InitData(Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseGuide_zh.xml");

                QString FilePath = Global::SystemPaths::Instance().GetTranslationsPath() + "/HimalayaService_zh.qm";
                (void) m_Translator.load(FilePath);
                qApp->installTranslator(&m_Translator);
            } else {
                mp_MessageDlg->SetTitle(QApplication::translate("ServiceUpdates::CLanguageWidget", "Language files",
                                                                0, QApplication::UnicodeUTF8));
                mp_MessageDlg->SetText(QApplication::translate("ServiceUpdates::CLanguageWidget",
                                       "Translation files are missing.", 0, QApplication::UnicodeUTF8));
                mp_MessageDlg->SetButtonText(1, QApplication::translate("ServiceUpdates::CLanguageWidget",
                                                                        "Ok", 0, QApplication::UnicodeUTF8));
                mp_MessageDlg->HideButtons();
                mp_MessageDlg->SetIcon(QMessageBox::Critical);
                mp_MessageDlg->Show();
            }           
        }
        m_Language = SelectedLanguage;
    }
}

/****************************************************************************/
/*!
 *  \brief Sets time and and date of the program
 *
 *  \iparam DateTime = New date and time
 */
/****************************************************************************/
void CServiceGUIConnector::SetDateTime(const QDateTime& DateTime)
{
    //    DEBUGWHEREAMI;

        // set new time
        QString CurrentDateTime =  DateTime.toString("d MMM yyyy hh:mm:ss");
    //        QString DateCommand = QString("echo himalaya | sudo -S date -s \"%1\"").arg(CurrentDateTime);
        QString DateCommand = QString("date -s \"%1\"").arg(CurrentDateTime);

        (void) system(DateCommand.toStdString().c_str());
    //        (void) system("echo himalaya | sudo -S hwclock -w");
        (void) system("hwclock -w");

        Global::AdjustedTime::Instance().AdjustToDateTime(DateTime);
}

/****************************************************************************/
/*!
 *  \brief Send event report command to Main
 */
/****************************************************************************/
void CServiceGUIConnector::SendEventReportCommand(const quint32 EventCode,
                                            const Global::tTranslatableStringList &EventStringList,
                                            const bool EventStatus, quint32 EventKey,
                                            const Global::AlternateEventStringUsage AltStringUsage)
{
    Q_UNUSED(EventKey);
    Q_UNUSED(EventStatus);
    Q_UNUSED(EventCode);
    Q_UNUSED(EventStringList);
    Q_UNUSED(AltStringUsage);

}

/****************************************************************************/
/*!
 *  \brief  Send Ack To CmdEventReport
 *  \iparam ClickedButton = Button clicked
 *  \iparam CmdRef = Command reference
 *  \iparam EventKey  = Event key
 */
/****************************************************************************/
void CServiceGUIConnector::OnEventReportAck(NetCommands::ClickedButton_t ClickedButton, Global::tRefType CmdRef, quint64 EventKey)
{
    Q_UNUSED(ClickedButton);
    Q_UNUSED(CmdRef);
    Q_UNUSED(EventKey);
    //m_NetworkObject.SendAckToMaster(CmdRef, NetCommands::CmdAcknEventReport(EventKey, ClickedButton));
}

ServiceDataManager::CModuleDataList* CServiceGUIConnector::GetModuleListContainer(void)
{
    return mp_ModuleList;
}

ServiceDataManager::CInstrumentHistory* CServiceGUIConnector::GetModuleListArchiveContainer(void)
{
    return mp_ModuleListArchive;
}

DataManager::CDeviceConfigurationInterface *
CServiceGUIConnector::GetDeviceConfigInterface(void)
{
    return mp_DeviceConfigurationInterface;
}

DataManager::CUserSettingsInterface *
CServiceGUIConnector::GetUserSettingInterface(void)
{
    return mp_SettingsInterface;
}

/****************************************************************************/
/*!
 *  \brief This slot updates the ServiceParameters object
 *  \iparam ServiceParameters = Service Parameters pointer
 */
/****************************************************************************/
void CServiceGUIConnector::SetServiceParametersContainer(DataManager::CServiceParameters *ServiceParameters)
{
    mp_ServiceParameters = ServiceParameters;
    emit ServiceParametersChanged();
}

/****************************************************************************/
/*!
 *  \brief  To get the ServiceParameters datacontainer
 *  \return ServiceParameters pointer
 */
/****************************************************************************/
DataManager::CServiceParameters* CServiceGUIConnector::GetServiceParameters()
{
    if(mp_ServiceParameters != NULL)
    {
        return mp_ServiceParameters;
    }
    return NULL;
}

/****************************************************************************/
/*!
 *  \brief Slot to update the ServiceParameters xml file
 *  \iparam ServiceParameters = Service Parameters object
 */
/****************************************************************************/
void CServiceGUIConnector::ServiceParametersUpdates(DataManager::CServiceParameters *ServiceParameters)
{
    if (ServiceParameters) {
       //delete mp_ServiceParameters;
       mp_ServiceParameters = ServiceParameters;
       if (!mp_ServiceParameters->Write()) {
           qDebug()<<"CServiceGUIConnector: Service Parameter updates to file failed.";
       }
    }
    //emit UpdateServiceParameters(mp_ServiceParameters);
}

/****************************************************************************/
/*!
 *  \brief Show ShowBasicColorTestDialog
 */
/****************************************************************************/
void CServiceGUIConnector::HandleTimeout()
{
    (void) mp_WaitDialog->close();
    ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Service::CMessageString::MSG_DEVICECOMMAND_TIMEOUT);
}


} // end namespace Core

