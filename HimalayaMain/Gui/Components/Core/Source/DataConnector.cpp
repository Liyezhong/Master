/****************************************************************************/
/*! \file DataConnector.cpp
 *
 *  \brief DataConnector implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-02-24, 29-Nov-2012
 *   $Author:  $ M.Scherer, Ningu
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

#include "Core/Include/DataContainer.h"
#include "Core/Include/DataConnector.h"
#include "DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataExport.h"
#include "DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataImport.h"
#include "DataManager/Containers/UserSettings/Commands/Include/CmdAlarmToneTest.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdKeepCassetteCount.h"

#include "DataManager/Containers/UserSettings/Include/UserSettingsInterface.h"
#include "Global/Include/Commands/AckOKNOK.h"
#include "Global/Include/Commands/CmdDateAndTime.h"
#include "Global/Include/GlobalEventCodes.h"
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Reagents/Include/ReagentRMSWidget.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Include/StationXmlDefinitions.h"

#include <QApplication>
#include <QHash>
#include "Global/Include/UITranslator.h"
#include "MainMenu//Include/MsgBoxManager.h"
#include "Dashboard/Include/SplashWidget.h"

namespace Core {


/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CDataConnector::CDataConnector(MainMenu::CMainWindow *p_Parent) : DataManager::CDataContainer(),
    m_ConsumableType(KIT),mp_MainWindow(p_Parent), mp_LanguageFile(NULL), mp_OldFile(NULL),
    m_LanguageChangeCount(0),m_BottleCount(0), m_GuiInit(true), mp_RmsMessageDlg(NULL)
{
    if (m_NetworkObject.NetworkInit()) {
        THROW(EVENT_GLOBAL_ERROR_UNKNOWN_EXCEPTION);
    }

    m_NetworkObject.RegisterNetMessage<NetCommands::CmdConfigurationFile>(&CDataConnector::ConfFileHandler, this);
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdProcessState>(&CDataConnector::ProcessStateHandler, this);

    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdNewProgram>(&CDataConnector::AddNewProgramHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdProgramUpdate>(&CDataConnector::UpdateProgramHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdProgramDeleteItem>(&CDataConnector::ProgramRemoveHandler, this);

    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdReagentAdd>(&CDataConnector::AddNewReagentHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdReagentUpdate>(&CDataConnector::UpdateReagentHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdReagentRemove>(&CDataConnector::ReagentRemoveHandler, this);

    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdReagentGroupUpdate>(&CDataConnector::UpdateReagentGroupHandler, this);

    //Reagent Station Commands
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdStationChangeReagent>(&CDataConnector::UpdateStationChangeReagentHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdStationResetData>(&CDataConnector::UpdateStationResetDataHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdStationSetAsEmpty>(&CDataConnector::UpdateStationSetAsEmptyHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdStationSetAsFull>(&CDataConnector::UpdateStationSetAsFullHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdUpdateStationReagentStatus>(&CDataConnector::UpdateStationReagentStatus, this);

    // Dashboard Command Handlers
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdCurrentProgramStepInfor>(&CDataConnector::CurrentProgramStepInfoHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdProgramAcknowledge>(&CDataConnector::ProgramAcknowledgeHandler, this);

    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdStationSuckDrain>(&CDataConnector::StationParaffinBathStatusHandler, this);

    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdProgramSelectedReply>(&CDataConnector::ProgramSelectedReplyHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdRetortLockStatus>(&CDataConnector::RetortLockStatusHandler, this);

    m_NetworkObject.RegisterNetMessage<NetCommands::CmdEventStrings>(&CDataConnector::EventStringHandler, this);
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdExecutionStateChanged>(&CDataConnector::ExecutionStateHandler, this);
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdLanguageFile>(&CDataConnector::LanguageFileHandler, this);
    m_NetworkObject.RegisterNetMessage<MsgClasses::CmdChangeUserSettings>(&CDataConnector::SettingsUpdateHandler, this);
    //m_NetworkObject.RegisterNetMessage<MsgClasses::CmdAlarmToneTest>(&CDataConnector::TestAlarmHandler, this);


    //EventHandlercomamnds
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdEventReport>(&CDataConnector::EventReportHandler, this);
    //user menu functions
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdChangeUserLevelReply>(&CDataConnector::UserAuthenticationHandler, this);
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdChangeAdminPasswordReply>(&CDataConnector::AdminPasswordChangeHandler, this);
    //Event view commands
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdDayRunLogReplyFile>(&CDataConnector::DayRunLogReplyFileHandler, this);
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdDayRunLogReply>(&CDataConnector::DayRunLogReplyHandler, this);
    m_NetworkObject.RegisterNetMessage<NetCommands::CmdGuiInit>(&CDataConnector::OnCmdGuiInit, this);
    // Signal/slot for DateAndTime reporting
    CONNECTSIGNALSLOT(&m_NetworkObject, SigDateAndTime(QDateTime), this, SetDateTime(QDateTime));
    // Signal/slot for timeout reporting
    CONNECTSIGNALSLOT(&m_NetworkObject, SigCmdTimeout(Global::tRefType, const QString &),
                      this, OnCmdTimeout(Global::tRefType, const QString &));
    // Signal indicating a valid connection to Master
    CONNECTSIGNALSLOT(&m_NetworkObject, SigMasterConnected(const QString &), this, OnMasterConnected(const QString &));
    // Signal indicating a disconnection from Master
    CONNECTSIGNALSLOT(&m_NetworkObject, SigMasterDisconnected(const QString &), this, OnMasterDisconnected(const QString &));

    CONNECTSIGNALSLOT(mp_MainWindow, onChangeEvent(), this, RetranslateUI());

    CONNECTSIGNALSLOT(this, DeviceConfigurationUpdated(), this, OnDeviceConfigurationUpdated());
    CONNECTSIGNALSLOT(mp_MainWindow, CurrentTabChanged(int), this, OnCurrentTabChanged(int));

    mp_MessageDlg = new MainMenu::CMessageDlg(mp_MainWindow);
    mp_MessageDlg->SetTitle(tr("Communication Error"));
    mp_MessageDlg->SetText(tr("The changes could not be saved."));
    mp_MessageDlg->SetIcon(QMessageBox::Critical);
    mp_MessageDlg->SetButtonText(1, tr("Ok"));
    mp_MessageDlg->setModal(true);
    mp_MessageDlg->HideButtons();
    m_UpdateProgramColor = false;
    m_AddProgramWithUpdateColor = false;

    mp_BlgScanWaitDialog = new MainMenu::CWaitDialog(mp_MainWindow);
    mp_WaitDialog = new MainMenu::CWaitDialog(mp_MainWindow);
    mp_WaitDialog->SetDialogTitle(tr("Startup"));
    mp_WaitDialog->SetText(tr("Initializing device communication ..."));
    mp_WaitDialog->HideAbort();
    mp_WaitDialog->show();
    CONNECTSIGNALSLOT(mp_WaitDialog, Timeout(), mp_MessageDlg, Show());

    // MsgBox Manager
    mp_MesgBoxManager  = new MainMenu::CMsgBoxManager(mp_MainWindow,SettingsInterface);
    (void)connect(mp_MesgBoxManager, SIGNAL(EventReportAck(NetCommands::ClickedButton_t,Global::tRefType, quint64)),
                      this, SLOT(OnEventReportAck(NetCommands::ClickedButton_t,Global::tRefType, quint64)));

    mp_SplashWidget = new SplashWidget();
    CONNECTSIGNALSLOT(this, ProgramStartReady(), this, OnProgramStartReady());

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDataConnector::~CDataConnector()
{
    try {
        delete mp_MesgBoxManager;
        delete mp_WaitDialog;
        delete mp_BlgScanWaitDialog;
        delete mp_MessageDlg;
        delete mp_LanguageFile;
    }
    catch (...) {
        //To please lint warnings
    }
}

/****************************************************************************/
/*!
 *  \brief Sets time and and date of the program
 *
 *  \iparam DateTime = New date and time
 */
/****************************************************************************/
void CDataConnector::SetDateTime(QDateTime DateTime)
{
    DEBUGWHEREAMI;
    // set new time
    Global::AdjustedTime::Instance().AdjustToDateTime(DateTime);
}

/****************************************************************************/
/*!
 *  \brief Called if a command sent to master times out.
 *
 * Called if a command sent to master times out. \todo check if it can be removed
 *
 * \param[in]   Ref             Reference of command which timed out.
 * \param[in]   CommandName     Name of command which timed out.
 */
/****************************************************************************/
void CDataConnector::OnCmdTimeout(Global::tRefType Ref, const QString &CommandName)
{
    DEBUGWHEREAMI;
    qDebug() << "Ref = " << Ref << " Name = " << CommandName;
}

/****************************************************************************/
/*!
 *  \brief  This slot is called as soon as connection with Master is established
 *
 *  \param[in] name = client name
 */
/****************************************************************************/
void CDataConnector::OnMasterConnected(const QString &name)
{
    Q_UNUSED(name)
    // hide the "waiting for connection" dialog:
    mp_WaitDialog->accept();
    /// \todo: do whatever else needs to be done on this event
}

/****************************************************************************/
/*!
 *  \brief  This slot is called as soon as connection to Master is lost
 *
 *  \param[in] name = client name
 */
/****************************************************************************/
void CDataConnector::OnMasterDisconnected(const QString &name)
{
    Q_UNUSED(name)
    // show the "waiting for reconnect" dialog:
    mp_WaitDialog->SetDialogTitle(tr("Disconnect"));
    mp_WaitDialog->SetText(tr("Reinitializing device communication ..."));
    mp_WaitDialog->show();
    /// \todo: do whatever needs to be done on this event
}

/****************************************************************************/
/**
 * \brief Acknowledge for a date and time transaction received.
 *
 * Acknowledge received.
 *
 * \param[in]   Ref     Acknowledge reference.
 * \param[in]   Ack     The acknowledge.
 */
/****************************************************************************/
void CDataConnector::OnAckDateAndTime(Global::tRefType Ref, const Global::AckOKNOK &Ack)
{
    Q_UNUSED(Ref);
    mp_WaitDialog->accept();
    if (Ack.GetStatus() == true) {
        emit DateTimeAcked();
    }
    else {
        if (Ack.GetText().length() > 0) {
            switch(Ack.GetType()) {
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
            mp_MessageDlg->SetText(Ack.GetText());
        }
        else {
            mp_MessageDlg->SetTitle(tr("Communication Error"));
            mp_MessageDlg->SetText(tr("Communication Error"));
            mp_MessageDlg->SetIcon(QMessageBox::Critical);
        }
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->show();
    }
}

/****************************************************************************/
/**
 * \brief Acknowledge received for transmissions consisting of two commands
 *
 * Acknowledge received.
 *
 * \param[in]   Ref     Acknowledge reference.
 * \param[in]   Ack     The acknowledge.
 */
/****************************************************************************/
void CDataConnector::OnAckTwoPhase(Global::tRefType Ref, const Global::AckOKNOK &Ack)
{
    Q_UNUSED(Ref);
    qDebug()<<"\n\n Ack.GetStatus() = "<< Ack.GetStatus();
    // just accept the wait dialog
    mp_WaitDialog->accept();
    if (Ack.GetStatus() == false) {
        ShowMessageDialog(Ack.GetType(), Ack.GetText());
    }    
}

/****************************************************************************/
/**
 * \brief Acknowledge received for transmissions consisting of two commands
 *
 * Acknowledge received.
 *
 * \param[in]   Ref     Acknowledge reference.
 * \param[in]   Ack     The acknowledge.
 */
/****************************************************************************/
void CDataConnector::OnAck(Global::tRefType Ref, const Global::AckOKNOK &Ack)
{
    Q_UNUSED(Ref);
    if (Ack.GetStatus() == false) {
        mp_WaitDialog->accept();
    }   
}
/****************************************************************************/
/*!
 *  \brief Sends data and time to the master
 *
 *  \iparam DateTime = Date and time to be sent
 */
/****************************************************************************/
void CDataConnector::SendDateTime(QDateTime DateTime)
{
    Global::CmdDateAndTime Command(5000, DateTime);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckDateAndTime, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Sends updated Dashboard Station Change Reagent data to the master
 *
 *  \iparam p_Reagent = Reagent data object
 */
/****************************************************************************/
void CDataConnector::SendStationChangeReagent(const QString& StationId, const QString& ReagentId)
{
    MsgClasses::CmdStationChangeReagent Command(1000, StationId, ReagentId);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();

}
void CDataConnector::SendStationResetData(const QString& StationId)
{
    MsgClasses::CmdStationResetData Command(1000, StationId);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

void CDataConnector::SendStationSetAsEmpty(const QString StationId)
{
    MsgClasses::CmdStationSetAsEmpty Command(1000, StationId);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

void CDataConnector::SendStationSetAsFull(const QString& StationId)
{
    MsgClasses::CmdStationSetAsFull Command(1000, StationId);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

void CDataConnector::SendRMSChanged(Global::RMSOptions_t rmsOption)
{

}

/****************************************************************************/
/*!
 *  \brief Sends updated Reagent data to the master
 *
 *  \iparam p_Reagent = Reagent data object
 */
/****************************************************************************/
void CDataConnector::SendReagentGroupUpdate(DataManager::CReagentGroup &ReagentGroup)
{
    MsgClasses::CmdReagentGroupUpdate Command(1000, ReagentGroup.GetGroupID(), ReagentGroup.GetGroupColor());
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Auto hide message box
 */
/****************************************************************************/
void CDataConnector::OnCancelMessageBoxDisplay()
{    
    if (mp_RmsMessageDlg) {
        if (mp_RmsMessageDlg->isVisible() ) {
            mp_RmsMessageDlg->accept();
            delete mp_RmsMessageDlg;
            mp_RmsMessageDlg = NULL;
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Sends updated Reagent data to the master
 *
 *  \iparam p_Reagent = Reagent data object
 */
/****************************************************************************/
void CDataConnector::SendReagentUpdate(DataManager::CReagent &Reagent)
{
#if 0
    QByteArray ByteArray;
    Reagent.SerializeContent(&ByteArray);

    NetCommands::CmdReagentUpdate Command(5000, ByteArray);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
#endif
    QByteArray ByteArray;
    QDataStream ReagentDataStream(&ByteArray,QIODevice::ReadWrite);
    ReagentDataStream << Reagent;
    MsgClasses::CmdReagentUpdate Command(1000, ReagentDataStream);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Sends new Reagent data to the master
 *
 *  \iparam Reagent = Reagent data object
 */
/****************************************************************************/
void CDataConnector::SendReagentAdd(DataManager::CReagent &Reagent)
{
    QByteArray ByteArray;
    QDataStream ReagentDataStream(&ByteArray,QIODevice::ReadWrite);
    ReagentDataStream << Reagent;
    MsgClasses::CmdReagentAdd Command(1000, ReagentDataStream);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnReagentAck, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
 }

/****************************************************************************/
/*!
 *  \brief Sends new Reagent data to the master
 *
 *  \iparam ReagentID = Reagent data object
 */
/****************************************************************************/
void CDataConnector::SendReagentRemove(QString ReagentID)
{
    MsgClasses::CmdReagentRemove Command(1000, ReagentID);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Sends updated Program data to the master
 *
 *  \iparam Program = Program data object
 */
/****************************************************************************/
void CDataConnector::SendProgramUpdate(DataManager::CProgram &Program)
{
      QByteArray ByteArray;
      QDataStream ProgramDataStream(&ByteArray,QIODevice::ReadWrite);
      ProgramDataStream << Program;
      MsgClasses::CmdProgramUpdate Command(5000, ProgramDataStream, ProgramDataStream);
      Command.SetProgramColorReplaced(false);
      m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnProgramAck, this);
       m_UpdateProgramColor = false;
      mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
      mp_WaitDialog->SetText(tr("Saving Settings ..."));
      mp_WaitDialog->SetTimeout(10000);
      mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Sends updated Program data to the master
 *
 *  \iparam ColorReplacedProgram = Program data object
 *  \iparam ColorAssignedProgram = Program data object
 */
/****************************************************************************/
void CDataConnector::SendProgramColorUpdate(DataManager::CProgram &ColorReplacedProgram,DataManager::CProgram &ColorAssignedProgram)
{
      QByteArray ByteArray;
      QByteArray ByteArrayNext;
      QDataStream ProgramDataStream(&ByteArray,QIODevice::ReadWrite);
      QDataStream ColorAssignedProgramDataStream(&ByteArrayNext,QIODevice::ReadWrite);
      ProgramDataStream << ColorReplacedProgram ;    
      ColorAssignedProgramDataStream << ColorAssignedProgram;
      (void)ProgramDataStream.device()->reset();
      MsgClasses::CmdProgramUpdate Command(5000, ProgramDataStream, ColorAssignedProgramDataStream);
      Command.SetProgramColorReplaced(true);
      m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnProgramAck, this);
      m_UpdateProgramColor = true;
      mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
      mp_WaitDialog->SetText(tr("Saving Settings ..."));
      mp_WaitDialog->SetTimeout(10000);
      mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Sends new program data to the master
 *
 *  \iparam Program = Program data object
 */
/****************************************************************************/
void CDataConnector::SendProgramAdd(DataManager::CProgram &Program)
{
    QByteArray ByteArray;
    QDataStream ProgramDataStream(&ByteArray,QIODevice::ReadWrite);
    ProgramDataStream << Program;
    (void)ProgramDataStream.device()->reset();
    MsgClasses::CmdNewProgram Command(5000, ProgramDataStream);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnProgramAck, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Sends new program data to the master
 *
 *  \iparam ProgramID = Program data object
 */
/****************************************************************************/
void CDataConnector::SendProgramRemove(QString &ProgramID)
{
    MsgClasses::CmdProgramDeleteItem Command(1000, ProgramID);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnProgramAck, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}
/****************************************************************************/
/*!
 *  \brief Handles incoming configuration Program commands
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::AddNewProgramHandler(Global::tRefType Ref, const MsgClasses::CmdNewProgram &Command)
{
    bool Result = false;
    DataManager::CProgram Program;
    QDataStream ProgramDataStream(&const_cast<QByteArray &>(Command.GetProgramData()), QIODevice::ReadWrite);
    ProgramDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));

    ProgramDataStream >> Program;


    Result = ProgramList->AddProgram(&Program);
    if (Result) {
        ProgramList->GetNextFreeProgID(true);
        emit ProgramsUpdated();
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
}

/****************************************************************************/
/*!
 *  \brief Handles incoming configuration Program commands
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::UpdateProgramHandler(Global::tRefType Ref, const MsgClasses::CmdProgramUpdate &Command)
{
    bool Result = false;
    bool ColorAssignedResult = false;
    DataManager::CProgram Program;
    DataManager::CProgram ColorAssignedProgram;
    QDataStream ProgramDataStream(&const_cast<QByteArray &>(Command.GetProgramData()), QIODevice::ReadWrite);
    ProgramDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    ProgramDataStream >> Program;
    Result = ProgramList->UpdateProgram(&Program);

    if (m_UpdateProgramColor == true) {
        QDataStream ColorAssignedProgramDataStream(&const_cast<QByteArray &>(Command.GetColorAssignedProgramData()), QIODevice::ReadWrite);
        ColorAssignedProgramDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
        (void)ColorAssignedProgramDataStream.device()->reset();
        ColorAssignedProgramDataStream >> ColorAssignedProgram;
        ColorAssignedResult = ProgramList->UpdateProgram(&ColorAssignedProgram);
        if (Result && ColorAssignedResult) {
            emit ProgramsUpdated();
        }
        m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result && ColorAssignedResult));
    }
    else {
        if (Result) {
            emit ProgramsUpdated();
        }
        m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
    }
    return;
}

/****************************************************************************/
/*!
 *  \brief Handles incomming ProgramRemove cmd
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::ProgramRemoveHandler(Global::tRefType Ref,
                                       const MsgClasses::CmdProgramDeleteItem &Command)
{
    bool Result = true;
    QString ProgramId = Command.GetItemId();
    Result = ProgramList-> DeleteProgram(ProgramId);
    if (Result) {
        emit ProgramsUpdated();
    }
    else {
        Result = false;
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
    return;
}

/****************************************************************************/
/*!
 *  \brief Handles incoming configuration Reagents commands
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::AddNewReagentHandler(Global::tRefType Ref, const MsgClasses::CmdReagentAdd &Command)
{
    bool Result = true;
    DataManager::CReagent Reagent;
    QDataStream ReagentDataStream(&const_cast<QByteArray &>(Command.GetReagentData()), QIODevice::ReadWrite);
    ReagentDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    ReagentDataStream >> Reagent;
    Result = ReagentList->AddReagent(&Reagent);
    if (Result) {
        emit ReagentsUpdated();
        ReagentList->GetNextFreeReagentID(true);
        mp_WaitDialog->accept();
        qDebug()<<"ReagentsUpdated emitted";
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
    return;
}
/****************************************************************************/
/*!
 *  \brief Handles incoming configuration Reagent commands
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::UpdateReagentHandler(Global::tRefType Ref, const MsgClasses::CmdReagentUpdate &Command)
{
    bool Result = true;
    DataManager::CReagent Reagent;
    QDataStream ReagentDataStream(&const_cast<QByteArray &>(Command.GetReagentData()), QIODevice::ReadWrite);
    ReagentDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    ReagentDataStream >> Reagent;
    Result = ReagentList->UpdateReagent(&Reagent);
    if (Result) {
        mp_WaitDialog->accept();
        emit ReagentsUpdated();
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
    return;
}

/****************************************************************************/
/*!
 *  \brief Handles incoming configuration Reagent commands
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::UpdateReagentGroupHandler(Global::tRefType Ref, const MsgClasses::CmdReagentGroupUpdate &Command)
{
    bool Result = true;
    DataManager::CReagentGroup* pReagentGroup = ReagentGroupList->GetReagentGroup(Command.GroupId());
    if (pReagentGroup)
        pReagentGroup->SetGroupColor(Command.GroupColor());
    else
        Result = false;
    if(Result){
        mp_WaitDialog->accept();
        emit ReagentGroupUpdated();
        emit ReagentsUpdated();
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
    return;
}


/****************************************************************************/
/*!
 *  \brief Handles incoming configuration Dashboard Station commands
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::UpdateStationChangeReagentHandler(Global::tRefType Ref,
                                                       const MsgClasses::CmdStationChangeReagent &Command)
{
    bool Result = true;
    DataManager::CDashboardStation* pDashboardStation = DashboardStationList->GetDashboardStation(Command.StationID());
    if (pDashboardStation)
    {
        pDashboardStation->SetDashboardReagentID(Command.ReagentID());
        switch (Reagents::CReagentRMSWidget::RMSPROCESSINGOPTION) {
            default:
                 QString("");
            case Global::RMS_CASSETTES:
                pDashboardStation->SetDashboardReagentActualCassettes(0);
                break;
            case Global::RMS_CYCLES:
                pDashboardStation->SetDashboardReagentActualCycles(0);
                break;
         }
        pDashboardStation->SetDashboardReagentStatus("Empty");
    }
    else
        Result = false;
    if(Result){
        mp_WaitDialog->accept();
        emit DashboardStationChangeReagent(Command.StationID());
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
}


void CDataConnector::UpdateStationResetDataHandler(Global::tRefType Ref, const MsgClasses::CmdStationResetData &Command)
{
    bool Result = true;
    DataManager::CDashboardStation* pDashboardStation = DashboardStationList->GetDashboardStation(Command.StationID());
     if (pDashboardStation) {
        pDashboardStation->SetDashboardReagentExchangeDate(QDate::currentDate()) ;
        switch (Reagents::CReagentRMSWidget::RMSPROCESSINGOPTION) {
            default:
                 QString("");
            case Global::RMS_CASSETTES:
                pDashboardStation->SetDashboardReagentActualCassettes(0);
                break;
            case Global::RMS_CYCLES:
                pDashboardStation->SetDashboardReagentActualCycles(0);
                break;
         }
     }
     else
        Result = false;
     if(Result){
        mp_WaitDialog->accept();
        emit DashboardStationChangeReagent(Command.StationID());
     }
     m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
}

void CDataConnector::UpdateStationSetAsEmptyHandler(Global::tRefType Ref, const MsgClasses::CmdStationSetAsEmpty &Command)
{
    bool Result = true;
    DataManager::CDashboardStation* pDashboardStation = DashboardStationList->GetDashboardStation(Command.StationID());
    if (pDashboardStation)
    {
        pDashboardStation->SetDashboardReagentStatus("Empty");
    }
    else
        Result = false;
    if(Result){
        mp_WaitDialog->accept();
        emit DashboardStationChangeReagent(Command.StationID());
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
}

void CDataConnector::UpdateStationSetAsFullHandler(Global::tRefType Ref, const MsgClasses::CmdStationSetAsFull &Command)
{
    bool Result = true;
    DataManager::CDashboardStation* pDashboardStation = DashboardStationList->GetDashboardStation(Command.StationID());
    if (pDashboardStation){
        pDashboardStation->SetDashboardReagentStatus("Full");
        pDashboardStation->SetDashboardReagentExchangeDate(QDate::currentDate()) ;
        switch (Reagents::CReagentRMSWidget::RMSPROCESSINGOPTION) {
            default:
                 QString("");
            case Global::RMS_CASSETTES:
                pDashboardStation->SetDashboardReagentActualCassettes(0);
                break;
            case Global::RMS_CYCLES:
                 pDashboardStation->SetDashboardReagentActualCycles(0);
                 break;
         }
    }
    else
        Result = false;
    if(Result){
        mp_WaitDialog->accept();
        emit DashboardStationChangeReagent(Command.StationID());
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
}

void CDataConnector::UpdateStationReagentStatus(Global::tRefType Ref, const MsgClasses::CmdUpdateStationReagentStatus &Command)
{
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    const QStringList& Ids = Command.StationIDs();
    for (int i = 0; i < Ids.count(); i++)
    {
        DataManager::CDashboardStation* pDashboardStation = DashboardStationList->GetDashboardStation(Ids[i]);
        if (pDashboardStation)
        {
            if(Command.CassetteCount() > 0)
            {

                pDashboardStation->SetDashboardReagentActualCassettes(
                            pDashboardStation->GetDashboardReagentActualCassettes() + Command.CassetteCount());

            }
            else
            {
                pDashboardStation->SetDashboardReagentActualCycles(
                            pDashboardStation->GetDashboardReagentActualCycles() + 1);
            }
        }
        else
        {
            qDebug() << "UpdateStationReagentStatus Failed.";
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Handles incoming ReagentRemove cmd
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::ReagentRemoveHandler(Global::tRefType Ref,
                                       const MsgClasses::CmdReagentRemove &Command)
{
    bool Result = false;
    bool ResultUpdateStation = false;
    ResultUpdateStation = DashboardStationList->UpdateStationsByReagentDelete(Command.GetReagentID());
    Result = ReagentList->DeleteReagent(Command.GetReagentID());
    if (Result && ResultUpdateStation) {
        mp_WaitDialog->accept();
        emit DashboardStationsUpdated();
        emit ReagentsUpdated();
    }
    else {
        Result = false;
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
    return;
}
/****************************************************************************/
/*!
 *  \brief Handles incoming configuration file commands
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = The command
 */
/****************************************************************************/
void CDataConnector::ConfFileHandler(Global::tRefType Ref, const NetCommands::CmdConfigurationFile &Command)
{
    bool Result = true;

    QDataStream DataStream(const_cast<QByteArray *>(&Command.GetFileContent()), QIODevice::ReadWrite);
    (void)DataStream.device()->reset();

    switch (Command.GetFileType()) {

        case NetCommands::PROGRAM:
            DataStream >> *ProgramList;
            ProgramList->SetDataVerificationMode(false);
            emit ProgramsUpdated();
            break;
        case NetCommands::REAGENT:
            DataStream >> *ReagentList;
            ReagentList->SetDataVerificationMode(false);
            emit ReagentsUpdated();
            break;
        case NetCommands::REAGENTGROUP:            
            DataStream >> *ReagentGroupList;
            emit ReagentGroupUpdated();
            break;
        case NetCommands::STATION:
            DataStream >> *DashboardStationList;
            emit DashboardStationsUpdated();
            break;

        case NetCommands::REAGENTGROUPCOLOR:
              DataStream >> *ReagentGroupColorList;
              emit ReagentGroupColorUpdated();
              break;

        case NetCommands::USER_SETTING:
            DataStream >> *SettingsInterface;
            SettingsInterface->SetDataVerificationMode(false);
            emit UserSettingsUpdated();
            break;
        case NetCommands::DEVICE_CONFIGURATION:
            DataStream >> *DeviceConfigurationInterface;
            DeviceConfigurationInterface->SetDataVerificationMode(false);
            qDebug()<<"Receive NetCommands::DEVICECONFIGURATION \n";
            emit DeviceConfigurationUpdated();
            break;
        default:
            Result = false;
            break;
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
    mp_WaitDialog->accept();


    /*! Device Configuration is the last among all the XML's sent from Main. We
     *  inform main that GUI is intialized.
     */
    if (m_GuiInit && m_GuiAllConfigurationReceived && Command.GetFileType() == NetCommands::DEVICE_CONFIGURATION) {
        qDebug()<<"Sending GUI INIT COMMAND \n";
        //end of initialization
        m_GuiInit = false;
        NetCommands::CmdGuiInit Cmd(2000, true);
        m_NetworkObject.SendCmdToMaster(Cmd, &CDataConnector::OnAckTwoPhase, this);

        //Enable it later
        //mp_SplashWidget->move(80, 50);
        //mp_SplashWidget->exec();
    }
    return;
}

void CDataConnector::OnProgramStartReady()
{
    mp_SplashWidget->accept();
}

/****************************************************************************/
/*!
 *  \brief Handles incoming Process state commands
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = Process State command
 */
/****************************************************************************/
void CDataConnector::ProcessStateHandler(Global::tRefType Ref, const NetCommands::CmdProcessState &Command)
{
    bool Result = false;
    if (Command.GetProcessState()) {
        Result = mp_MainWindow->SetStatusIcons(MainMenu::CMainWindow::ProcessRunning);

    }
    else {
        Result = mp_MainWindow->UnsetStatusIcons(MainMenu::CMainWindow::ProcessRunning);
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
    mp_WaitDialog->accept();
    return;
}

/****************************************************************************/
/*!
 *  \brief Sends the Updated/Modified settings to the master
 *
 *  \iparam Settings = Settings reference
 */
/****************************************************************************/
void CDataConnector::SendUpdatedSettings(DataManager::CUserSettings &Settings)
{
    QByteArray ByteArray;
    QDataStream SettingsDataStream(&ByteArray, QIODevice::ReadWrite);
    SettingsDataStream << Settings;
    (void)SettingsDataStream.device()->reset();
    MsgClasses::CmdChangeUserSettings Command(5000, SettingsDataStream);    
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnUserSettingsAck, this);
    mp_WaitDialog->SetDialogTitle("Device Communication");
    mp_WaitDialog->SetText("Saving Settings ...");
    mp_WaitDialog->show();
}
/****************************************************************************/
/*!
 *  \brief Sends the TestToneAlarm settings to the master
 *
 *  \iparam Volume = Volume increments
 *  \iparam Sound = Sound type
 *  \iparam Type =
 */
/****************************************************************************/
void CDataConnector::SendCmdPlayTestToneAlarm(quint8 Volume, quint8 Sound, bool Type)
{
    qDebug()<< Volume << Sound << Type;

    MsgClasses::CmdAlarmToneTest Command(5000, Volume, Sound, Type);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle("Device Communication");
    mp_WaitDialog->SetText("Request Sent");
    mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CDataConnector::RetranslateUI()
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
 *  \brief This slot is called when  DeviceConfigurationUpdated signal is emitted.
 */
/****************************************************************************/
void CDataConnector::OnDeviceConfigurationUpdated()
{
    bool WorkStationMode = DeviceConfigurationInterface->GetDeviceConfiguration()->GetBoolValue("WorkstationMode");
    if (WorkStationMode) {
        emit UpdateMode("Workstation");
    }
    else {
        emit UpdateMode("Himalaya");
    }
}
/****************************************************************************/
/*!
 *  \brief This slot is called when  ---- is emitted.
 */
/****************************************************************************/
void CDataConnector::SendDataImportExport(const QString Name, const QStringList &ParamList)
{
    QByteArray ByteArray;
    QString DialogTitle;
    QString DialogText;
    if (Name.contains("Export")) {
        // update the parameter list in the byte array
        foreach (QString ListValue, ParamList) {
            ByteArray.append(ListValue);
        }
        if (ParamList.contains("User")) {
            DialogTitle = "User Export";
            DialogText = "Exporting user data ...";
        }
        else {
            DialogTitle = "Service Export";
            DialogText = "Exporting service data ...";
        }
        MsgClasses::CmdDataExport Command(20000, ByteArray);
        m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    }
    else {
        MsgClasses::CmdDataImport Command(Global::Command::MAXTIMEOUT, ByteArray);
        m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
        DialogTitle = "Import";
        DialogText = "Importing data ...";
    }


    mp_WaitDialog->SetDialogTitle(tr(DialogTitle.toAscii()));
    mp_WaitDialog->SetText(tr(DialogText.toAscii()));

    mp_WaitDialog->show();

}

/****************************************************************************/
/*!
 *  \brief Handles incoming Event String command
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = Event String command
 */
/****************************************************************************/
void CDataConnector::EventStringHandler(Global::tRefType Ref, const NetCommands::CmdEventStrings &Command)
{
    QByteArray EventsStringsData = Command.GetEventsStringsData();
    QDataStream DataStream(&EventsStringsData, QIODevice::ReadWrite);
    Global::tTranslations Translations;

    QDataStream::Status oldStatus = DataStream.status();
    DataStream.resetStatus();
    Translations.clear();

    quint32 n;
    DataStream >> n;

    for (quint32 i = 0; i < n; ++i)
    {
        if (DataStream.status() != QDataStream::Ok)
        {
            break;
        }
        QLocale::Language Key;
        Global::tLanguageData T;
        quint32 num = static_cast<quint32>(Key);
        DataStream >> num >> T;
        Key = static_cast<QLocale::Language>(num);
        Translations.insertMulti(Key, T);
    }

    if (DataStream.status() != QDataStream::Ok)
        Translations.clear();
    if (oldStatus != QDataStream::Ok)
        DataStream.setStatus(oldStatus);

    QHashIterator<QLocale::Language, Global::tLanguageData> j(Translations);
    while (j.hasNext())
    {
        j.next();
        //qDebug() << j.key() << ": " << j.value() << endl;
        Global::UITranslator::TranslatorInstance().SetLanguageData(j.key(), j.value(), false, false);

    }
    // set default language
    Global::UITranslator::TranslatorInstance().SetDefaultLanguage(SettingsInterface->GetUserSettings()->GetLanguage());
    // set fallback language
    Global::UITranslator::TranslatorInstance().SetFallbackLanguage(QLocale::English);
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
}

/****************************************************************************/
/*!
 *  \brief Handles CmdExecution State , informs Status screen about
 *         changes in execution state
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = Event String command
 */
/****************************************************************************/
void CDataConnector::ExecutionStateHandler(Global::tRefType Ref, const NetCommands::CmdExecutionStateChanged &Command)
{
    if (Command.m_Stop) {
        // Stop timers in rack button and dialogs associated with it.
        qDebug()<<" \n\n PROCESSING STOPPED, STOPPING TIMERS" <<Global::AdjustedTime::Instance().GetCurrentDateTime();
        emit StopTimers();
    }
    else {
        //Start timers, before starting calculate the updated endtimes.
        qDebug()<<"Starting Timers"<< Global::AdjustedTime::Instance().GetCurrentDateTime();
        emit ReCalculateEndTimes();
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
}

/****************************************************************************/
/*!
 *  \brief Handles CmdLanguageFile
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = Event String command
 */
/****************************************************************************/
void CDataConnector::LanguageFileHandler(Global::tRefType Ref, const NetCommands::CmdLanguageFile &Command)
{
    mp_LanguageFile = new QFile(QString("languagefile%1.qm").arg(m_LanguageChangeCount));
    if (!mp_LanguageFile->isOpen()) {
        mp_LanguageFile->open(QIODevice::ReadWrite);
    }
    if (mp_LanguageFile->write(Command.GetLanguageData()) != Command.GetLanguageData().size()) {
        m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(false));
        mp_LanguageFile->remove();
        delete mp_LanguageFile;
        mp_LanguageFile = NULL;
        return;
    }
    mp_LanguageFile->flush();

    if (!m_Translator.load(mp_LanguageFile->fileName())) {
        qDebug()<<"\n\nTranslations failed";
        m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(false));
        mp_LanguageFile->remove();
        delete mp_LanguageFile;
        mp_LanguageFile = NULL;
        return;
    }
    qApp->installTranslator(&m_Translator);
    m_LanguageChangeCount++;
    if (m_LanguageChangeCount != 1) {
        if (mp_OldFile) {
            mp_OldFile->remove();
            delete mp_OldFile;
            mp_OldFile = mp_LanguageFile;
        }
    }
    else {
        mp_OldFile = mp_LanguageFile;
    }
    // change the language whenever the new language file receives from Main
    Global::UITranslator::TranslatorInstance().SetDefaultLanguage(SettingsInterface->GetUserSettings()->GetLanguage());
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
}

/****************************************************************************/
/*!
 *  \brief Handles CmdChangeUserSettings
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = Change User Settings command
 */
/****************************************************************************/
void CDataConnector::SettingsUpdateHandler(Global::tRefType Ref, const MsgClasses::CmdChangeUserSettings &Command)
{
    QByteArray SettingsData(const_cast<QByteArray &>(Command.GetUserSettings()));
    QDataStream SettingsDataStream(&SettingsData, QIODevice::ReadWrite);
    SettingsDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    DataManager::CUserSettings Settings;
    SettingsDataStream >> Settings;
    bool Result = false;
    if (SettingsInterface) {
        Result = SettingsInterface->UpdateUserSettings(&Settings);
        if (Result) {
            emit UserSettingsUpdated();
        }
    }
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
}
///****************************************************************************/
///*!
// *  \brief Handles CmdAlarmToneTest
// *
// *  \iparam Ref = Command reference
// *  \iparam Command = Change User Settings command
// */
///****************************************************************************/
//void CDataConnector::TestAlarmHandler(Global::tRefType Ref, const MsgClasses::CmdAlarmToneTest &Command)
//{
//    QByteArray SettingsData(const_cast<QByteArray &>(Command.GetUserSettings()));
//    QDataStream SettingsDataStream(&SettingsData, QIODevice::ReadWrite);
//    SettingsDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
//    DataManager::CUserSettings Settings;
//    SettingsDataStream >> Settings;
//    bool Result = false;
//    if (SettingsInterface) {
//        Result = SettingsInterface->UpdateUserSettings(&Settings);
//        if (Result) {
//            emit UserSettingsUpdated();
//        }
//    }
//    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(Result));
//}

/****************************************************************************/
/****************************************************************************/
/*!
 *  \brief CmdEventReport Handler
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = EventReport Command
 */
/****************************************************************************/
void CDataConnector::EventReportHandler(Global::tRefType Ref, const NetCommands::CmdEventReport &Command)
{
    QByteArray EventReportData(const_cast<QByteArray &>(Command.GetEventReportData()));
    QDataStream EventDataStream(&EventReportData, QIODevice::ReadWrite);
    EventDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    mp_MesgBoxManager->Manage(EventDataStream, Ref);
}

/****************************************************************************/
/*!
 *  \brief  Send Ack To CmdEventReport
 *  \iparam ClickedButton = Button clicked
 *  \iparam CmdRef = Command reference
 *  \iparam EventKey  = Event key
 */
/****************************************************************************/
void CDataConnector::OnEventReportAck(NetCommands::ClickedButton_t ClickedButton, Global::tRefType CmdRef, quint64 EventKey)
{
    m_NetworkObject.SendAckToMaster(CmdRef, NetCommands::CmdAcknEventReport(EventKey, ClickedButton));
}

/****************************************************************************/
/*!
 *  \brief Send the user level information to the Main
 *
 *  \iparam DataStream = Data stream refernce
 */
/****************************************************************************/
void CDataConnector::SendUserLevel(QDataStream &DataStream)
{
    qint32 UserLevel;
    QString Password;
    (void)DataStream.device()->reset();
    DataStream >> UserLevel >> Password;
    (void)DataStream.device()->reset();
    qDebug()<<" Password = "<< DataStream.device()->readAll();
    NetCommands::CmdChangeUserLevel Command(1000, (Global::GuiUserLevel)UserLevel, Password);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
}

/****************************************************************************/
/*!
 *  \brief Send the change password information to the Main
 *
 *  \iparam DataStream = Data stream refernce
 */
/****************************************************************************/
void CDataConnector::SendChangepassword(QDataStream &DataStream)
{
    QString Type;
    QString Password;
    (void)DataStream.device()->reset();
    DataStream >> Type >> Password;
    NetCommands::CmdChangeAdminPassword Command(1000, Type, Password);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
}

/****************************************************************************/
/*!
 *  \brief User authentication handler
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = User authentication Command
 */
/****************************************************************************/
void CDataConnector::UserAuthenticationHandler(Global::tRefType Ref, const NetCommands::CmdChangeUserLevelReply &Command)
{
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    emit UserAuthentication(Command.GetUserAuthenticatedLevel());
}

/****************************************************************************/
/*!
 *  \brief admin password change handler
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = CmdChangeAdminPassword class object
 */
/****************************************************************************/
void CDataConnector::AdminPasswordChangeHandler(Global::tRefType Ref, const NetCommands::CmdChangeAdminPasswordReply &Command)
{
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    emit ChangeAdminPassword(Command.GetCommandType());
}

/****************************************************************************/
/*!
 *  \brief Day run log reply handler for the daily run log file names
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = CmdDayRunLogReply class object
 */
/****************************************************************************/
void CDataConnector::DayRunLogReplyHandler(Global::tRefType Ref, const NetCommands::CmdDayRunLogReply &Command)
{
    Q_UNUSED(Ref);
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    emit UpdateDayRunLogFileNames(Command.GetFileNames());
}
/****************************************************************************/
/*!
 *  \brief Day run log file handler for the daily run log file
 *
 *  \iparam Ref = Command reference
 *  \iparam Command = CmdDayRunLogReplyFile class object
 */
/****************************************************************************/
void CDataConnector::DayRunLogReplyFileHandler(Global::tRefType Ref, const NetCommands::CmdDayRunLogReplyFile &Command)
{
    Q_UNUSED(Ref);
    mp_WaitDialog->accept();
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    QDataStream FileContentDataStream(const_cast<QByteArray *>(&Command.GetFileData()), QIODevice::ReadWrite);
    FileContentDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    emit UpdateDayRunLogFileContent(FileContentDataStream);
}
/****************************************************************************/
/*!
 *  \brief Send the selected day run log file to Main
 *
 *  \iparam FileName = Name of the file
 */
/****************************************************************************/
void CDataConnector::SendSelectedDayRunLogFile(const QString &FileName)
{
    Global::GuiUserLevel userRole = (Global::GuiUserLevel)MainMenu::CMainWindow::GetCurrentUserRole();
    NetCommands::CmdDayRunLogRequestFile Command(3000, FileName, userRole);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Log Files"));
    mp_WaitDialog->SetText(tr("Getting Daily run Log file ..."));
    mp_WaitDialog->show();
}

void CDataConnector::SendProgramAction(const QString& ProgramID,
                                       DataManager::ProgramActionType_t ActionType,
                                       const QDateTime& ProgramEndDateTime)
{
    MsgClasses::CmdProgramAction Command(1000, ProgramID, ActionType, ProgramEndDateTime);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

void CDataConnector::SendProgramSelected(const QString& ProgramID, int ParaffinStepIndex)
{
    MsgClasses::CmdProgramSelected Command(1000, ProgramID, ParaffinStepIndex);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

void CDataConnector::SendRetortLock(bool IsLock)
{
    MsgClasses::CmdRetortLock Command(1000, IsLock);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

void CDataConnector::SendKeepCassetteCount(int CassetteCount)
{
    MsgClasses::CmdKeepCassetteCount Command(1000, CassetteCount);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
    mp_WaitDialog->SetDialogTitle(tr("Device Communication"));
    mp_WaitDialog->SetText(tr("Saving Settings ..."));
    mp_WaitDialog->SetTimeout(10000);
    mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief RequestDayRunLogFileNames
 */
/****************************************************************************/
void CDataConnector::RequestDayRunLogFileNames()
{
    NetCommands::CmdDayRunLogRequest Command(1000);
    m_NetworkObject.SendCmdToMaster(Command, &CDataConnector::OnAckTwoPhase, this);
}
/****************************************************************************/
/*!
 *  \brief OnCmdGuiInit
 */
/****************************************************************************/
void CDataConnector::OnCmdGuiInit(Global::tRefType Ref, const NetCommands::CmdGuiInit &Command)
{
    //Set GUI Init Flag to true. This will be set to false when last xml file is received from Master
    qDebug()<<"NetCommands::CmdGuiInit Received \n";
    m_GuiAllConfigurationReceived = true;
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
}
/****************************************************************************/
/*!
 *  \brief OnUserSettingsAck
 */
/****************************************************************************/
void CDataConnector::OnUserSettingsAck(Global::tRefType Ref, const Global::AckOKNOK &Ack)
{
    Q_UNUSED(Ref);
    qDebug()<<"\n\n Ack.GetStatus() = "<< Ack.GetStatus();
    // just accept the wait dialog
    mp_WaitDialog->accept();
    if (Ack.GetStatus() == false) {
        ShowMessageDialog(Ack.GetType(), Ack.GetText());
        if (mp_MessageDlg->exec() == (int)QDialog::Accepted ) {
            emit RevertChangedUserSettings();
        }
    }
}
/****************************************************************************/
/*!
 *  \brief OnProgramAck
 */
/****************************************************************************/
void CDataConnector::OnProgramAck(Global::tRefType Ref, const Global::AckOKNOK &Ack)
{
    Q_UNUSED(Ref);
    qDebug()<<"\n\n Ack.GetStatus() = "<< Ack.GetStatus();
    // just accept the wait dialog
    mp_WaitDialog->accept();
    if (Ack.GetStatus() == false) {
        ShowMessageDialog(Ack.GetType(), Ack.GetText());
        if (mp_MessageDlg->exec() == (int)QDialog::Accepted ) {
            emit RevertChangedProgram();
        }
    }
}
/****************************************************************************/
/*!
 *  \brief OnReagentAck
 */
/****************************************************************************/
void CDataConnector::OnReagentAck(Global::tRefType Ref, const Global::AckOKNOK &Ack)
{
    Q_UNUSED(Ref);
    qDebug()<<"\n\n Ack.GetStatus() = "<< Ack.GetStatus();
    // just accept the wait dialog
    mp_WaitDialog->accept();
    if (Ack.GetStatus() == false) {
        ShowMessageDialog(Ack.GetType(), Ack.GetText());
        if (mp_MessageDlg->exec() == (int)QDialog::Accepted ) {
            emit RevertChangedReagent();
        }
    }
}
/****************************************************************************/
/*!
 *  \brief OnCurrentTabChanged
 */
/****************************************************************************/
void CDataConnector::OnCurrentTabChanged(int CurrentTabIndex)
{
    m_CurrentTabIndex = CurrentTabIndex;
}
/****************************************************************************/
/*!
 *  \brief ShowMessageDialog
 */
/****************************************************************************/
void CDataConnector::ShowMessageDialog(Global::GUIMessageType MessageType, QString MessageText)
{
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
    mp_MessageDlg->show();
}

void CDataConnector::CurrentProgramStepInfoHandler(Global::tRefType Ref, const MsgClasses::CmdCurrentProgramStepInfor & Command)
{
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    emit CurrentProgramStepInforUpdated(Command);
}

void CDataConnector::ProgramAcknowledgeHandler(Global::tRefType Ref, const MsgClasses::CmdProgramAcknowledge& Command)
{
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    switch( Command.AcknownedgeType())
    {
        case DataManager::PROGRAM_READY:
        {
             emit ProgramStartReady();
        }
        break;
        case DataManager::PROGRAM_WILL_COMPLETE:
        {
             emit ProgramWillComplete();
        }
        break;
        case DataManager::PROGRAM_ABORT_FINISHED:
        {
             emit ProgramAborted();
        }
        break;
        case DataManager::PROGRAM_ABORT_BEGIN:
        {
             emit ProgramBeginAbort();
        }
        break;
        case DataManager::PROGRAM_RUN_FINISHED:
        {
             emit ProgramCompleted();
        }
        break;
        case DataManager::PROGRAM_RUN_BEGIN:
        {
             emit ProgramRunBegin();
             mp_MainWindow->SetStatusIcons(MainMenu::CMainWindow::ProcessRunning);
        }
        break;
        case DataManager::PROGRAM_PAUSE_FINISHED:
        {
              mp_MainWindow->UnsetStatusIcons(MainMenu::CMainWindow::ProcessRunning);
        }
        break;

        default:
        {
            qDebug() << "Do Nothing";
        }
    }
}

void CDataConnector::ProgramSelectedReplyHandler(Global::tRefType Ref, const MsgClasses::CmdProgramSelectedReply & Command)
{
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    emit ProgramSelectedReply(Command);
}

void CDataConnector::RetortLockStatusHandler(Global::tRefType Ref, const MsgClasses::CmdRetortLockStatus & Command)
{
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    emit RetortLockStatusChanged(Command);
}


void CDataConnector::StationParaffinBathStatusHandler(Global::tRefType Ref, const MsgClasses::CmdStationSuckDrain & Command)
{
    m_NetworkObject.SendAckToMaster(Ref, Global::AckOKNOK(true));
    emit StationSuckDrain(Command);
}


} // end namespace Core

