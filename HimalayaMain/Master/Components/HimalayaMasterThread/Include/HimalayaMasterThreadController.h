/****************************************************************************/
/*! \file HimalayaMasterThread/Include/HimalayaMasterThreadController.h
 *
 *  \brief Definition file for class HimalayaMasterThreadController.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2010-07-12, 2012-07-12
 *  $Author:    $ J.Bugariu, Raju
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

#ifndef HIMALAYA_HIMALAYAMASTERTHREADCONTROLLER_H
#define HIMALAYA_HIMALAYAMASTERTHREADCONTROLLER_H

#include <Threads/Include/MasterThreadController.h>
#include <PasswordManager/Include/PasswordManager.h>
#include <DataManager/Containers/UserSettings/Include/UserSettings.h>
#include <../Include/DataLoggingSources.h>
#include <Global/Include/Commands/CmdDateAndTime.h>
#include <Global/Include/AlarmHandler.h>
#include <Global/Include/GlobalExitCodes.h>
#include <Global/Include/Commands/AckOKNOK.h>
#include <Global/Include/Commands/CmdEventUpdate.h>
#include <HimalayaDataManager/Include/DataManager.h>
#include "ImportExport/Include/ImportExportThreadController.h"
#include <NetCommands/Include/CmdSystemAction.h>
#include <NetCommands/Include/CmdChangeAdminPassword.h>
#include <NetCommands/Include/CmdChangeAdminPasswordReply.h>
#include <NetCommands/Include/CmdChangeUserLevel.h>
#include <NetCommands/Include/CmdChangeUserLevelReply.h>
#include <NetCommands/Include/CmdDayRunLogReply.h>
#include <NetCommands/Include/CmdExportDayRunLogReply.h>
#include <NetCommands/Include/CmdDayRunLogReplyFile.h>
#include <NetCommands/Include/CmdDayRunLogRequest.h>
#include <NetCommands/Include/CmdExportDayRunLogRequest.h>
#include <NetCommands/Include/CmdDayRunLogRequestFile.h>
#include <NetCommands/Include/CmdEventStrings.h>
#include <NetCommands/Include/CmdDataUpdate.h>
#include <NetCommands/Include/CmdGuiInit.h>
#include <HimalayaMasterThread/Include/ProgramStartableFlagManager.h>
#include <NetCommands/Include/CmdConfigurationFile.h>


namespace EventHandler {
    class EventObject;
}

//Forward declarations
namespace NetCommands {
    class CmdExternalProcessState;
    class CmdSystemState;
}



namespace DataManager {
    class DataManager;
}

namespace Himalaya {

enum CommandChannelSelector_t {
    GUI,
    EVENTHANDLER,
    DATALOGGING,
    SCHEDULER,
    SCHEDULERMAIN,
    DEVICE_CMD_PROCESSOR
};

typedef enum  {
    UNDEFINED_HIMALAYA_THREAD,
    HIMALAYA_GUI_THREAD,
    SCHEDULER_THREAD,
    SCHEDULER_MAIN_THREAD,
    DEVICE_CMD_PROCESSOR_THREAD,
    EXPORT_CONTROLLER_THREAD,
    IMPORT_EXPORT_THREAD
} HimalayaThreads_t;

/****************************************************************************/
/**
 * \brief Controller for master thread for Himalaya application.
 *
 * This is the controller for master thread. It has to create other controllers
 * and threads, initialize them, start and stop them and finally destroy them.
 * \warning This class is not thread safe!
 */
/****************************************************************************/
class HimalayaMasterThreadController : public Threads::MasterThreadController {
    Q_OBJECT
private:
    PasswordManager::CPasswordManager    m_PasswordManager;                      ///< The password manager.

    Threads::CommandChannel         m_CommandChannelGui;                    ///< Command channel for Gui thread controller.
    Threads::CommandChannel         m_CommandChannelSchedulerMain;          ///< Command channel for scheduler main thread controller.
    Threads::CommandChannel         m_CommandChannelEventHandler;
    Threads::CommandChannel         m_CommandChannelExport;                 ///< Command channel for Export
    Threads::CommandChannel         m_CommandChannelImportExport;           ///< Command channel for Import Export thread
    Global::tRefType                m_ImportExportCommandRef;               ///< Store the command reference to return the command data once export process is exited
    DataManager::CDataManager       *mp_DataManager;                        ///< The himalaya DataManager
    Threads::CommandChannel*        mp_ImportExportAckChannel;              ///< Store the command reference to return the command channel data once export process is exited
    bool                            m_ExportProcessIsFinished;              ///< Store export process flag value
    bool                            m_ImportExportThreadIsRunning;          ///< Store ImportExport thread flag value


    bool m_Simulation;  //!  Enable/disable simulation thread controller. \todo Remove later
    ProgramStartableManager         m_ProgramStartableManager;              ///< Object Managing Program Startablity
    Global::GuiUserLevel            m_AuthenticatedLevel;                   ///< The current user authenticated level
    bool                            m_ControllerCreationFlag;               ///< True if controllers are created, False if not
    /****************************************************************************/
    HimalayaMasterThreadController(const HimalayaMasterThreadController &);                     ///< Not implemented.
    const HimalayaMasterThreadController & operator = (const HimalayaMasterThreadController &); ///< Not implemented.
    /****************************************************************************/
    /**
     * \brief Register commands.
     */
    /****************************************************************************/
    virtual void RegisterCommands();
    /****************************************************************************/
    /**
     * \brief Set new date and time offset.
     *
     * \param[in]   Ref                 Command reference.
     * \param[in]   Cmd                 Command.
     * \param[in]   AckCommandChannel   Command channel for acknowledges.
     */
    /****************************************************************************/
    void SetDateTime(Global::tRefType Ref, const Global::CmdDateAndTime &Cmd, Threads::CommandChannel &AckCommandChannel);
    /****************************************************************************/
    /**
     * \brief Acknowledge of type AckOKNOK received.
     *
     * \param[in]   Ref     Command reference.
     * \param[in]   Ack     Acknowledge.
     */
    /****************************************************************************/
    void OnAckOKNOK(Global::tRefType Ref, const Global::AckOKNOK &Ack);
    /****************************************************************************/


    /**
     * \brief Start statemachine.
     *
     * Start own statemachine and start to work.
     *
     */
    /****************************************************************************/
    virtual void StartStatemachine();

    /****************************************************************************/
    /**
     * \brief Start the import export thread.
     *
     * \param Ref - Refernce of the command argument
     * \param Cmd - Command class
     * \param AckCommandChannel - Channel class for the command
     *
     */
    /****************************************************************************/
    template<class CommandData>
    void ImportExportDataFileHandler(Global::tRefType Ref, const CommandData &Cmd,
                        Threads::CommandChannel &AckCommandChannel) {
        if (!m_ImportExportThreadIsRunning) {
            m_ImportExportThreadIsRunning = true;
            // store the command reference so that once Export process is finished then
            // this reference need to be sent to the acknowledgement channel
            m_ImportExportCommandRef = Ref;
            // save the command channel, so that once the work is completed then ack can be sent to the same channel
            mp_ImportExportAckChannel = &AckCommandChannel;

            // create and connect scheduler controller
            ImportExport::ImportExportThreadController *ImportExportThreadController
                    = new ImportExport::ImportExportThreadController(HEARTBEAT_SOURCE_IMPORTEXPORT, *mp_DataManager,
                                                                     CommandData::NAME,
                                                                     (const_cast<CommandData&>(Cmd)).GetCommandData());
            try {
                // connect the siganl slot mechanism to create the process.
                CONNECTSIGNALSLOT(ImportExportThreadController, StartExportProcess(), this, StartExportProcess());
            }
            catch (...){
                SendAcknowledgeNOK(Ref, AckCommandChannel, "Unable to connect to signal slot");
                m_ImportExportThreadIsRunning = false;
                return;
            }

            try {
                // connect the siganl slot mechanism to create the containers for the Import.
                CONNECTSIGNALSLOT(ImportExportThreadController,
                              ThreadFinished(const bool, const QString &, bool, bool), this,
                              ImportExportThreadFinished(const bool, const QString &, bool, bool));
            }
            catch (...){
                SendAcknowledgeNOK(Ref, AckCommandChannel, "Unable to connect to signal slot");
                m_ImportExportThreadIsRunning = false;
                return;
            }

            try {
                // connect the siganl slot mechanism to create the containers for the Import.
                CONNECTSIGNALSLOT(ImportExportThreadController,
                              RequestDayRunLogFileNames(), this,
                              RequestDayRunLogFileNames());
            }
            catch (...){
                SendAcknowledgeNOK(Ref, AckCommandChannel, "Unable to connect to signal slot");
                m_ImportExportThreadIsRunning = false;
                return;
            }

            try {
                // connect the siganl slot mechanism to set directory name.
                CONNECTSIGNALSLOT(this,
                              DayRunLogDirectoryName(const QString &), ImportExportThreadController,
                              SetDayRunLogFilesDirectoryName(const QString &));
            }
            catch (...){
                SendAcknowledgeNOK(Ref, AckCommandChannel, "Unable to connect to signal slot");
                m_ImportExportThreadIsRunning = false;
                return;
            }




            //ImportExportThreadController->setDataContainer(&mp_DataManager);
            AddAndConnectController(ImportExportThreadController, &m_CommandChannelImportExport,
                                    static_cast<int>(IMPORT_EXPORT_THREAD));            
            // start the export process
            StartSpecificThreadController(static_cast<int>(IMPORT_EXPORT_THREAD));
        }
        else {
            // send negative acknowledge
            SendAcknowledgeNOK(Ref, AckCommandChannel, "Thread is already running");
        }
    }





    /****************************************************************************/
    /**
     * \brief Used for the export process which in turn return provides inputs to
     *        ImportExport thread controller. This handler fires when the datalogging
     *        thread replies it
     *
     * \param Ref - Refernce of the command argument
     * \param Cmd - Command class
     * \param AckCommandChannel - Channel class for the command
     *
     */
    /****************************************************************************/
    void ExportDayRunLogHandler(Global::tRefType Ref, const NetCommands::CmdExportDayRunLogReply &Cmd,
                             Threads::CommandChannel &AckCommandChannel);

    /************************************************************************************/
    /*!
     *  \brief Change of the user level
     *  \iparam Ref = Command reference
     *  \iparam Cmd = The command
     *  \iparam AckCommandChannel = command channel
    */
    /************************************************************************************/
    void ChangeUserLevelHandler(Global::tRefType Ref, const NetCommands::CmdChangeUserLevel &Cmd,
                             Threads::CommandChannel &AckCommandChannel);

    /************************************************************************************/
    /*!
     *  \brief Change of the password for the Admin user
     *  \iparam Ref = Command reference
     *  \iparam Cmd = The command
     *  \iparam AckCommandChannel = command channel
    */
    /************************************************************************************/
    void ChangeAdminPasswordHandler(Global::tRefType Ref, const NetCommands::CmdChangeAdminPassword &Cmd,
                             Threads::CommandChannel &AckCommandChannel);


    /****************************************************************************/
    /**
     * \brief Information to the Target about an event
     *
     * \param Ref - Refernce of the command argument
     * \param Cmd - Command class
     * \param AckCommandChannel - Channel class for the command
     *
     */
    /****************************************************************************/
    void EventCmdSystemAction(Global::tRefType Ref, const NetCommands::CmdSystemAction &Cmd,
                            Threads::CommandChannel &AckCommandChannel);

    /****************************************************************************/
    /**
     * \brief Update the GUI language files (qm files from translation directory)
     */
    /****************************************************************************/
    bool UpdateSupportedGUILanguages();



    /****************************************************************************/
    /*!
     *  \brief  Inform GUI and scheduler status of external status
     *  \iparam Ref = Command reference
     *  \iparam Cmd = Command
     *  \AckCommandChannel = Command channel to send acknowledge
     */
    /****************************************************************************/
    void ExternalProcessConnectionHandler(Global::tRefType Ref, const NetCommands::CmdExternalProcessState &Cmd, Threads::CommandChannel &AckCommandChannel);

    /****************************************************************************/
    /*!
     *  \brief   This handler is called on reception of "CmdGuiInit", The remaining
                 threads are created on reception of this command.
     *  \iparam Ref = Command reference
     *  \iparam Cmd = Command
     *  \AckCommandChannel = Command channel to send acknowledge
     */
    /****************************************************************************/
    void OnCmdGuiInitHandler(Global::tRefType Ref, const NetCommands::CmdGuiInit &Cmd, Threads::CommandChannel &AckCommandChannel);

    /****************************************************************************/
    /*!
     *  \brief  when system state changed, Master should update and keep
     *          the curent system state.
     *  \iparam Ref = Command reference
     *  \iparam Cmd = Command
     *  \iparam AckCommandChannel = Command channel to send acknowledge
     */
    /****************************************************************************/
    void OnCmdSysState(Global::tRefType Ref, const NetCommands::CmdSystemState &Cmd,
                          Threads::CommandChannel& AckCommandChannel);

protected:
    /****************************************************************************/
    /**
     * \brief Create GUI controllers, which inturn starts GUI process .
     *
     * \note All the XML files are sent to GUI.
     */
    /****************************************************************************/
    virtual void InitializeGUI();
    /****************************************************************************/
    /**
     * \brief Create all controllers and threads.
     *
     * First of all base class' method is called then own controllers are created.
     */
    /****************************************************************************/
    virtual void CreateControllersAndThreads();
    /****************************************************************************/
    /**
     * \brief Power will fail shortly.
     */
    /****************************************************************************/
    virtual void OnPowerFail();
    /****************************************************************************/
    /**
     * \brief Initiate the shutdown process.
     *
     * Initiate the shutdown process. Make some project specific tasks the call
     * \ref Shutdown to shut down software.
     */
    /****************************************************************************/
    virtual void InitiateShutdown();

    virtual void SendContainersTo(Threads::CommandChannel &CommandChannel);

    virtual bool IsCommandAllowed(const Global::CommandShPtr_t &Cmd);

    /****************************************************************************/
    /**
     * \brief This method is called when the base received the \ref Go signal.
     *
     * This means that everything is fine and normal operation started.
     * We are running in our own thread.\n
     * We create all controllers used (the according method in derived classes
     * \ref CreateControllersAndThreads is also called), then initialize the controllers
     * and finally attach them to the corresponding threads and start them.\n
     * If something goes wrong, the master thread (and application) will be stopped.
     */
    /****************************************************************************/
    virtual void OnGoReceived();
    /****************************************************************************/
    virtual void CreateAlarmHandler();
public:
    /****************************************************************************/
    /**
     * \brief Default Constructor.
     */
    /****************************************************************************/
    HimalayaMasterThreadController();
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~HimalayaMasterThreadController();

    /****************************************************************************/
    /**
     * \brief Create and initialize used objects.
     *
     * We try to read the hardware configuration file.
     */
    /****************************************************************************/
    void CreateAndInitializeObjects();


    /****************************************************************************/
    /**
     * \brief Return the command channel object requested
     * \iparam CommandChannelSelector = Command channel to return
     * \return CommandChannel object
     */
    /****************************************************************************/
    inline Threads::CommandChannel & GetCommandChannel(CommandChannelSelector_t CommandChannelSelector) {
        switch (CommandChannelSelector) {
            case GUI:
                return m_CommandChannelGui;
            case EVENTHANDLER:
                return m_CommandChannelEventHandler;
            case SCHEDULERMAIN:
                return m_CommandChannelSchedulerMain;

            case DATALOGGING:
                return m_CommandChannelDataLogging;
            default:
                return m_CommandChannelGui;
        }
    }
   const DataManager::CDataManagerBase *GetDataManager() { return mp_DataManager; }

   /****************************************************************************/
    /**
     * \brief Send the language file to GUI
     *
     * \param FileName - Name of the file
     *
     * \return On successful (True) or not (false)
     */
    /****************************************************************************/
    bool SendLanguageFileToGUI(QString FileName);

    /****************************************************************************/
    /**
     * \brief Send command to the required channel.
     *
     * \param Container - Container class
     * \param ContainerVerifier - Verifier class for the container
     * \param Path - path of the file
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    template<class DataList>
    bool SendConfigurationFile(DataList *Container,NetCommands::FileType_t FileType) {
        // initialize the byte array
        QByteArray *p_ByteArray = new QByteArray();
        // clear the byte array
        p_ByteArray->clear();
        // create the xml stream in the read write mode
        QDataStream XmlStream(p_ByteArray, QIODevice::ReadWrite);
        // set the stream version
        XmlStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
        // stream the container
        XmlStream << *Container;
        /// \todo please add if any  logevent is required or not
        // send the command to the required channel
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, FileType, XmlStream)), m_CommandChannelGui);

        // delete the byte array
        delete p_ByteArray;
        return true;
    }

private slots:
      void SendXML();
      /****************************************************************************/
      /**
       * \brief Starts the export process.
       */
      /****************************************************************************/
      void StartExportProcess();

      /****************************************************************************/
      /**
       * \brief Requests the day run log file names.
       */
      /****************************************************************************/
      void RequestDayRunLogFileNames();

      void SendStateChange(QString state);
      /****************************************************************************/
      /**
       * \brief Slot for the export process.
       *
       * \param Name - Name of the error string
       * \param ExitCode - exit code for the process
       */
      /****************************************************************************/
      void ExportProcessExited(const QString &Name, int ExitCode);

      /****************************************************************************/
      /**
       * \brief Slot for the removing the on demand threads from master thread.
       *
       */
      /****************************************************************************/
      void RemoveAndDestroyObjects();


      /****************************************************************************/
      /**
       * \brief Slot for the creating containers for the type of Import.
       *
       * \param IsImport - Import is successful or not
       * \param TypeOfImport - type of import
       * \param UpdatedCurrentLanguage - Flag for the current language updated or not
       * \param NewLanguageAdded - Flag for the new language is added or not
       */
      /****************************************************************************/
      void ImportExportThreadFinished(const bool IsImport, const QString &TypeOfImport,
                                      bool UpdatedCurrentLanguage = false,
                                      bool NewLanguageAdded = false);     
      void SetAlarmHandlerTimeout(quint16 timeout);
      void OnFireAlarmLocalRemote(bool isLocalAlarm);
signals:
      /****************************************************************************/
      /**
       * \brief Signal for setting the daily run log file directory name.
       *
       * \param Name - Name of the directory
       */
      /****************************************************************************/
      void DayRunLogDirectoryName(const QString &Name);

}; // end class HimalayaMasterThreadController

} // end namespace Himalaya

#endif // HIMALAYA_HIMALAYAMASTERTHREADCONTROLLER_H
