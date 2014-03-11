/****************************************************************************/
/*! \file DataConnector.cpp
 *
 *  \brief DataConnector implementation.
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
#include <Global/Include/SystemPaths.h>

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
    , mp_ModuleList(0)
    , mp_SettingsInterface(0)
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
void CServiceGUIConnector::SetModuleListContainer(DataManager::CModuleDataList *ModuleList)
{
    mp_ModuleList = ModuleList;
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
 */
/****************************************************************************/
void CServiceGUIConnector::SendModuleUpdate(DataManager::CModule &Module)
{
    mp_ModuleList->UpdateModule(&Module);
    qDebug() << Module.GetSerialNumber() << endl;
    qDebug() << "Module Updated";
}

/****************************************************************************/
/*!
 *  \brief This slot updates the Modulelist
 */
/****************************************************************************/
void CServiceGUIConnector::SendAddModule(DataManager::CModule &Module)
{
    mp_ModuleList->AddModule(&Module);
    qDebug() << Module.GetSerialNumber() << endl;
    qDebug() << "Module Updated";
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
 */
/****************************************************************************/
void CServiceGUIConnector::OnCurrentTabChanged(int CurrentTabIndex)
{
    m_CurrentTabIndex = CurrentTabIndex;
}

/****************************************************************************/
/*!
 *  \brief Show Busy Dialog
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
 *  \brief Hide Busy Dialog
 */
/****************************************************************************/
void CServiceGUIConnector::HideBusyDialog()
{
     mp_WaitDialog->close();
}

/****************************************************************************/
/*!
 *  \brief Slot to Get PopUp Response
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
 */
/****************************************************************************/
void CServiceGUIConnector::ShowMessageDialog(Global::GUIMessageType MessageType, QString MessageText)
{
    m_MessageDlg = true;
    if (MessageText.length() > 0) {
        switch (MessageType) {
        case Global::GUIMSGTYPE_ERROR:
            mp_MessageDlg->SetIcon(QMessageBox::Critical);
            mp_MessageDlg->SetTitle(tr("Error"));
            break;
        case Global::GUIMSGTYPE_INFO:
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->SetTitle(tr("Information"));
            break;
        case Global::GUIMSGTYPE_WARNING:
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(tr("Warning"));
            break;
        }
        mp_MessageDlg->SetText(MessageText);
    }
    else {
        mp_MessageDlg->SetTitle(tr("Communication Error"));
        mp_MessageDlg->SetText(tr("Communication Error"));
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
    }
    mp_MessageDlg->SetButtonText(1, tr("OK"));
    //mp_MessageDlg->setModal(false);
    mp_MessageDlg->show();
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

/****************************************************************************/
/*!
 *  \brief Sets time and and date of the program
 *
 *  \iparam DateTime = New date and time
 */
/****************************************************************************/
void CServiceGUIConnector::SetDateTime(QDateTime DateTime)
{
    // set new time
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
    //m_NetworkObject.SendAckToMaster(CmdRef, NetCommands::CmdAcknEventReport(EventKey, ClickedButton));
}

DataManager::CModuleDataList* CServiceGUIConnector::GetModuleListContainer(void)
{
    return mp_ModuleList;
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

} // end namespace Core

