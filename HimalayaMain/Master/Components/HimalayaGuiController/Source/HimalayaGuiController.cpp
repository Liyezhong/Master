/****************************************************************************/
/*! \file HimalayaGuiController.cpp
 *
 *  \brief Implementation file for class HimalayaGuiController.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.12.2010
 *   $Author:  $ Y.Novak
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

#include <HimalayaGuiController/Include/HimalayaGuiController.h>
#include <../Include/DataLoggingSources.h>
#include <Global/Include/SystemPaths.h>
#include <Global/Include/Utils.h>

#include <Global/Include/Commands/AckOKNOK.h>
#include <Global/Include/Commands/CmdDateAndTime.h>
#include <NetCommands/Include/CmdConfigurationFile.h>
#include <HimalayaDataContainer/Containers/ReagentGroups/Commands/Include/CmdReagentGroupUpdate.h>
#include <HimalayaDataContainer/Containers/Reagents/Commands/Include/CmdReagentUpdate.h>
#include <HimalayaDataContainer/Containers/Reagents/Commands/Include/CmdReagentAdd.h>
#include <HimalayaDataContainer/Containers/Reagents/Commands/Include/CmdReagentRemove.h>
#include <DataManager/Containers/UserSettings/Commands/Include/CmdChangeUserSettings.h>
#include <NetCommands/Include/CmdProcessState.h>
#include <HimalayaDataContainer/Containers/Programs/Commands/Include/CmdProgramUpdate.h>
#include <HimalayaDataContainer/Containers/Programs/Commands/Include/CmdNewProgram.h>
#include <HimalayaDataContainer/Containers/Programs/Commands/Include/CmdProgramDeleteItem.h>

#include <NetCommands/Include/CmdEventStrings.h>
#include <NetCommands/Include/CmdExecutionStateChanged.h>
#include <NetCommands/Include/CmdLanguageFile.h>
#include <NetCommands/Include/CmdScheduledStations.h>
#include <NetCommands/Include/CmdExternalProcessState.h>

#include <NetCommands/Include/CmdEventReport.h>
#include <NetCommands/Include/CmdChangeAdminPassword.h>
#include <NetCommands/Include/CmdChangeAdminPasswordReply.h>
#include <NetCommands/Include/CmdChangeUserLevel.h>
#include <NetCommands/Include/CmdChangeUserLevelReply.h>

#include <NetCommands/Include/CmdDayRunLogReply.h>
#include <NetCommands/Include/CmdDayRunLogReplyFile.h>
#include <NetCommands/Include/CmdDayRunLogRequest.h>
#include <NetCommands/Include/CmdDayRunLogRequestFile.h>
#include <NetCommands/Include/CmdAcknEventReport.h>
#include <NetCommands/Include/CmdGuiInit.h>
#include "DataManager/Containers/UserSettings/Commands/Include/CmdAlarmToneTest.h"

#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdStationChangeReagent.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdStationResetData.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdStationSetAsFull.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdStationSetAsEmpty.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdUpdateStationReagentStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortLock.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortLockStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdParaffinBathStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramEndDateTime.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramStartReady.h"



namespace HimalayaGui {

/****************************************************************************/
/**
 * \brief Constructor.
 *
 * \param[in]   TheHeartBeatSource    Logging source to be used.
 */
/****************************************************************************/
HimalayaGuiController::HimalayaGuiController(Global::gSourceType TheHeartBeatSource) :
    ExternalProcessController::ExternalProcessController(GUI_PROCESS_NAME, TheHeartBeatSource),
    m_myGuiDevice(NULL)
{
}

/****************************************************************************/
/**
 * \brief Destructor.
 */
/****************************************************************************/
HimalayaGuiController::~HimalayaGuiController() {
    try {
        if (m_myGuiDevice != NULL) {
            delete m_myGuiDevice;
        }
        m_myGuiDevice = NULL;
    } catch(...) {
        // to please PCLint...
    }
}

/****************************************************************************/
/**
 * \brief Create and configure needed objects.
 *
 * If something goes wrong, the already created objects must be deleted.
 */
/****************************************************************************/
void HimalayaGuiController::CreateAndInitializeObjects() {

    qDebug() << "Himalaya-GUI: initializing objects...";

    QString path = Global::SystemPaths::Instance().GetSettingsPath() + NetworkBase::CMH_PATH_TO_SETTINGS;
    m_myGuiDevice = new ExternalProcessControl::ExternalProcessDevice(NetworkBase::NSE_TYPE_NORMAL_GUI, GUI_PROCESS_NAME, path, this, this);

    // register GuiDevice with Base Class
    ExternalProcessController::RegisterExternalProcessDevice(m_myGuiDevice);

    // register acknowledge processing and timeout functors
    RegisterThreadAcksAndTimeouts();

    // register OnDataChanged processing functions
    /// \todo implement

    // continue with initialization
    ExternalProcessController::CreateAndInitializeObjects();
}

/****************************************************************************/
/**
 * \brief This function registers all command acknowledges
 *        used by this thread controller
 */
/****************************************************************************/
void HimalayaGuiController::RegisterThreadAcksAndTimeouts()
{
    // register commands for sending to external process
    RegisterCommandForProcessing<NetCommands::CmdConfigurationFile, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdConfigurationFile>, this);

    RegisterAcknowledgeForProcessing<Global::AckOKNOK, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendAckToExternalProcess<Global::AckOKNOK>, this);

    RegisterCommandForProcessing<NetCommands::CmdProcessState, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdProcessState>, this);

    RegisterCommandForProcessing<NetCommands::CmdEventStrings, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdEventStrings>, this);

    RegisterCommandForProcessing<NetCommands::CmdExecutionStateChanged, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdExecutionStateChanged>, this);

    RegisterCommandForProcessing<NetCommands::CmdLanguageFile, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdLanguageFile>, this);
    RegisterCommandForProcessing<MsgClasses::CmdChangeUserSettings, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdChangeUserSettings>, this);
    RegisterCommandForProcessing<NetCommands::CmdScheduledStations, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdScheduledStations>, this);

    // user level functions
    RegisterCommandForProcessing<NetCommands::CmdChangeUserLevelReply, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdChangeUserLevelReply>, this);
    RegisterCommandForProcessing<NetCommands::CmdChangeAdminPasswordReply, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdChangeAdminPasswordReply>, this);

    // Event view commands for Daily run log file which need to be sent to GUI
    RegisterCommandForProcessing<NetCommands::CmdDayRunLogReply, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdDayRunLogReply>, this);
    RegisterCommandForProcessing<NetCommands::CmdDayRunLogReplyFile, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdDayRunLogReplyFile>, this);
    RegisterCommandForProcessing<NetCommands::CmdGuiInit, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdGuiInit>, this);

    // Register commands for receiving from external process
    RegisterExternalMessage<Global::CmdDateAndTime, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<Global::CmdDateAndTime>, this);
    RegisterExternalMessage<MsgClasses::CmdChangeUserSettings, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdChangeUserSettings>, this);


    //Dashboard
    RegisterExternalMessage<MsgClasses::CmdProgramAction, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdProgramAction>, this);

    RegisterCommandForProcessing<MsgClasses::CmdRetortStatus, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdRetortStatus>, this);

    RegisterCommandForProcessing<MsgClasses::CmdParaffinBathStatus, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdParaffinBathStatus>, this);

    RegisterCommandForProcessing<MsgClasses::CmdStationStatus, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdStationStatus>, this);

    RegisterCommandForProcessing<MsgClasses::CmdProgramEndDateTime, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdProgramEndDateTime>, this);

    RegisterExternalMessage<MsgClasses::CmdProgramEndDateTime, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdProgramEndDateTime>, this);

    RegisterCommandForProcessing<MsgClasses::CmdCurrentProgramStepInfor, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdCurrentProgramStepInfor>, this);

    RegisterCommandForProcessing<MsgClasses::CmdProgramStartReady, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdProgramStartReady>, this);

    //Retort Lock/Unlock
    RegisterExternalMessage<MsgClasses::CmdRetortLock, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdRetortLock>, this);

    RegisterCommandForProcessing<MsgClasses::CmdRetortLockStatus, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdRetortLockStatus>, this);

    //  Staions
    RegisterExternalMessage<MsgClasses::CmdStationChangeReagent, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdStationChangeReagent>, this);

    RegisterCommandForProcessing<MsgClasses::CmdStationChangeReagent, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdStationChangeReagent>, this);

    RegisterExternalMessage<MsgClasses::CmdStationResetData, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdStationResetData>, this);

    RegisterCommandForProcessing<MsgClasses::CmdStationResetData, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdStationResetData>, this);

    RegisterExternalMessage<MsgClasses::CmdStationSetAsFull, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdStationSetAsFull>, this);

    RegisterCommandForProcessing<MsgClasses::CmdStationSetAsFull, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdStationSetAsFull>, this);

    RegisterExternalMessage<MsgClasses::CmdStationSetAsEmpty, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdStationSetAsEmpty>, this);

    RegisterCommandForProcessing<MsgClasses::CmdStationSetAsEmpty, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdStationSetAsEmpty>, this);

    //  reagent group
    RegisterExternalMessage<MsgClasses::CmdReagentGroupUpdate, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdReagentGroupUpdate>, this);
    RegisterCommandForProcessing<MsgClasses::CmdReagentGroupUpdate, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdReagentGroupUpdate>, this);

    //  Update reagent status
    RegisterExternalMessage<MsgClasses::CmdUpdateStationReagentStatus, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdUpdateStationReagentStatus>, this);

    RegisterCommandForProcessing<MsgClasses::CmdUpdateStationReagentStatus, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdUpdateStationReagentStatus>, this);

    //  Reagents
    RegisterExternalMessage<MsgClasses::CmdReagentAdd, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdReagentAdd>, this);
    RegisterCommandForProcessing<MsgClasses::CmdReagentAdd, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdReagentAdd>, this);

    RegisterExternalMessage<MsgClasses::CmdReagentRemove, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdReagentRemove>, this);
    RegisterCommandForProcessing<MsgClasses::CmdReagentRemove, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdReagentRemove>, this);

    RegisterExternalMessage<MsgClasses::CmdReagentUpdate, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdReagentUpdate>, this);
    RegisterCommandForProcessing<MsgClasses::CmdReagentUpdate, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdReagentUpdate>, this);

  
    //  Program
    RegisterExternalMessage<MsgClasses::CmdProgramUpdate, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdProgramUpdate>, this);

    RegisterExternalMessage<MsgClasses::CmdNewProgram, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdNewProgram>, this);

    RegisterExternalMessage<MsgClasses::CmdProgramDeleteItem, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdProgramDeleteItem>, this);

    RegisterCommandForProcessing<MsgClasses::CmdProgramUpdate, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdProgramUpdate>, this);

    RegisterCommandForProcessing<MsgClasses::CmdNewProgram, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdNewProgram>, this);
    RegisterCommandForProcessing<MsgClasses::CmdProgramDeleteItem, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<MsgClasses::CmdProgramDeleteItem>, this);


    // commands for Import and Export
    RegisterExternalMessage<MsgClasses::CmdDataExport, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdDataExport>, this);
    RegisterExternalMessage<MsgClasses::CmdDataImport, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdDataImport>, this);

    // User level functions for the different users
    RegisterExternalMessage<NetCommands::CmdChangeAdminPassword, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<NetCommands::CmdChangeAdminPassword>, this);
    RegisterExternalMessage<NetCommands::CmdChangeUserLevel, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<NetCommands::CmdChangeUserLevel>, this);


    // Event view commands for Daily run log file which receives from GUI
    RegisterExternalMessage<NetCommands::CmdDayRunLogRequest, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<NetCommands::CmdDayRunLogRequest>, this);

    RegisterExternalMessage<NetCommands::CmdDayRunLogRequestFile, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardCmdFromExternalProcess<NetCommands::CmdDayRunLogRequestFile>, this);

    // register acknowledges for receiving from external process
    RegisterExternalMessage<Global::AckOKNOK, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardAckFromExternalProcess<Global::AckOKNOK>, this);

    RegisterExternalMessage<NetCommands::CmdAcknEventReport, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::ForwardAckFromExternalProcess<NetCommands::CmdAcknEventReport>, this);
    


    RegisterCommandForProcessing<NetCommands::CmdEventReport, HimalayaGui::HimalayaGuiController>
            (&HimalayaGuiController::SendCmdToExternalProcess<NetCommands::CmdEventReport>,this);


    RegisterExternalMessage<NetCommands::CmdConfigurationFile, HimalayaGui::HimalayaGuiController>
                (&HimalayaGuiController::ForwardCmdFromExternalProcess<NetCommands::CmdConfigurationFile>, this);


    RegisterExternalMessage<NetCommands::CmdGuiInit, HimalayaGui::HimalayaGuiController>
                (&HimalayaGuiController::ForwardCmdFromExternalProcess<NetCommands::CmdGuiInit>, this);


    RegisterExternalMessage<MsgClasses::CmdAlarmToneTest, HimalayaGui::HimalayaGuiController>
                (&HimalayaGuiController::ForwardCmdFromExternalProcess<MsgClasses::CmdAlarmToneTest>, this);

}

/****************************************************************************/
/**
 * \brief This method is called when the base class received the \ref Go signal.
 *
 * This means that everything is fine and normal operation started.
 * We are running in our own thread now.
 */
/****************************************************************************/
void HimalayaGuiController::OnGoReceived() {
    /// \todo implement

    /// \todo sort out the path to binaries & the rest of constants...
    qDebug() << (QString)("Himalaya-GUI: OnGo in GUI controller");
    ExternalProcessController::OnGoReceived();
}

/****************************************************************************/
/**
 * \brief This method is called when GUI is connected and ready to work.
 *
 *        This means we are in the "Working" state.
 */
/****************************************************************************/
void HimalayaGuiController::OnReadyToWork() {
    qDebug() << "HimalayaGuiController: ready to work called.";

    if (m_myGuiDevice == NULL) {
        /// \todo log error
        return;
    }

    // start sending DateAndTime (supply Zero to use the default period)
    if (!m_myGuiDevice->StartDateTimeSync(0)) {
        /// \todo: handle error.
    }
    emit SendConfigurationFiles();

    NetCommands::CmdExternalProcessState* commandPtr(new NetCommands::CmdExternalProcessState(5000, NetCommands::HIMALAYA_GUI_PROCESS, true));
    Q_ASSERT(commandPtr);

    Global::tRefType Ref = GetNewCommandRef();

    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

}

void HimalayaGuiController::OnStopWorking()
{
    NetCommands::CmdExternalProcessState* commandPtr(new NetCommands::CmdExternalProcessState(5000, NetCommands::HIMALAYA_GUI_PROCESS, false));
    Q_ASSERT(commandPtr);

    Global::tRefType Ref = GetNewCommandRef();

    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}


/****************************************************************************/
/**
 * \brief This method is called when the base class received the \ref Stop signal.
 *
 * This means that normal operation will stop after processing this signal.
 * We are still running in our own thread.\n
 */
/****************************************************************************/
void HimalayaGuiController::OnStopReceived() {
    /// \todo implement
    qDebug() << (QString)("Himalaya-GUI: OnStop received.");
    ExternalProcessController::OnStopReceived();

}

/****************************************************************************/
/**
 * \brief Power will fail shortly.
 *
 * Power will fail shortly.
 */
/****************************************************************************/
void HimalayaGuiController::OnPowerFail() {
    /// \todo implement
}

/****************************************************************************/
/**
 * \brief Cleanup and destroy the created objects.
 */
/****************************************************************************/
void HimalayaGuiController::CleanupAndDestroyObjects() {
    // destroy all objects
    /// \todo implement
    qDebug() << (QString)("Himalaya-GUI: CleanupAndDestroyObjects called.");
    ExternalProcessController::CleanupAndDestroyObjects();
}

/****************************************************************************/
/**
 * \brief Handler for command timeouts.
 *
 * \param[in]       Ref         The command reference.
 * \param[in]       CmdName     Name of command.
 */
/****************************************************************************/
void HimalayaGuiController::OnCmdTimeout(Global::tRefType Ref, const QString &CmdName)
{
    Q_UNUSED(Ref)
    Q_UNUSED(CmdName)
    /// \todo: implement
}

} // end namespace HimalayaGui

