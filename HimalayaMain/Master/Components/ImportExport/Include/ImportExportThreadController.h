/****************************************************************************/
/*! \file ImportExportThreadController.h
 *
 *  \brief Definition file for class ImportExportThreadController.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-07-02
 *  $Author:    $ Raju
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

#ifndef IMPORTEXPORT_IMPORTEXPORTTHREADCONTROLLER_H
#define IMPORTEXPORT_IMPORTEXPORTTHREADCONTROLLER_H

#define HIMALAYA_IMPORT_EXPORT

#include "Threads/Include/ThreadController.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataExport.h"
#include "DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataImport.h"
#include "DataManager/Containers/ExportConfiguration/Include/ExportConfigurationVerifier.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramListVerifier.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentListVerifier.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Include/DataStationListVerifier.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsVerifier.h"
#include "DataManager/Containers/SWVersions/Include/SWVersionListVerifier.h"
#include "Global/Include/SystemPaths.h"
#include "ImportExport/WriteArchive/Include/WriteArchive.h"
#include "ImportExport/ReadArchive/Include/ReadArchive.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardDataStationList.h"


//lint -sem(ImportExport::ImportExportThreadController::DoCleanUpObjects,cleanup)


namespace ImportExport {

const qint32 TWENTY_FOUR_HOURS_IN_SECONDS = 86400; ///< const for 24 hours value in seconds

// forward declaration
class CmdDataExport;

/****************************************************************************/
/**
 * \brief Class for ImportExportThread Controller.
 *        This class used for to Import the data and export the data
 */
/****************************************************************************/
class ImportExportThreadController : public Threads::ThreadController {
    friend class TestImportExportThreadController;
    Q_OBJECT
private:    
    DataManager::CDataManager& m_DataManager;                   ///< Store the data manager - pointer
    DataManager::CDashboardDataStationList* mp_StationList;              ///< Store the Station list
    DataManager::CDataReagentList* mp_ReagentList;              ///< Store the reagent list
    DataManager::CExportConfiguration* mp_ExportConfiguration;  ///< Store export configuration    
    QString m_CommandValue;                                     ///< Store the command value
    quint32 m_NoOfLogFiles;                                     ///< Store the no of log files value for service export
    QString m_CommandName;                                      ///< Store the command name i.e. either user export or service export
    QString m_SerialNumber;                                     ///< Store the serial number of the device
    QString m_DeviceName;                                       ///< Store the device name
    bool m_WorkStationMode;                                     ///< Store the workstation mode
    bool m_ThreadInitialized;                                   ///< Thread initialization flag
    bool m_NewLanguageAdded;                                    ///< Store the flag for new language
    bool m_CurrentLanguageUpdated;                              ///< Store the flag for current language
    bool m_EventRaised;                                         ///< Store the event information flag for raise event
    bool m_UserExport;                                          ///< Store the user export flag
    bool m_TranslationsFolderUpdatedFiles;                      ///< Translations folder updated with the files
    bool m_RemoteCareRequest;                                   ///< Remote care request flag
    QString m_DayRunLogDirectoryName;                           ///< Store the day run log directory name
    DataManager::CExportConfiguration m_TempExportConfiguration;  ///< Store temporary export configuration
    quint32 m_EventCode;                                        ///< Store the event code
    QString m_EventLogFileName;                                 ///< Store the event log file name
    QString m_TargetFileName;                                   ///< Store the target file name


    /****************************************************************************/
    /**
     * \brief Constructor.
     */
    /****************************************************************************/
    ImportExportThreadController();

    /****************************************************************************/
    /**
     * \brief Copy Constructor.
     */
    /****************************************************************************/
    ImportExportThreadController(const ImportExportThreadController &);

    /****************************************************************************/
    /**
     * \brief Assignment Operator which copies from rhs to lhs.
     */
    /****************************************************************************/
    const ImportExportThreadController & operator = (const ImportExportThreadController &);

    /****************************************************************************/
    /**
     * \brief Create required directories before proceeding further otherwise the
     *        thread end up with error
     */
    /****************************************************************************/
    void CreateRequiredDirectories();
	
	/****************************************************************************/
    /**
     * \brief Do pre tasks before starting the process.
     *        Creates the RMS_Status.csv file and TempConfiguration.xml file and
     *        copy required files for export from one location to other location
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool DoPretasks();

    /****************************************************************************/
    /**
     * \brief Create the temporary export configuration and write all the required
     *        files in a temporary directory.
     *
     * \return On Successful (true) or not (false)
     */
    /****************************************************************************/
    bool WriteTempExportConfigurationAndFiles();

    /****************************************************************************/
    /**
     * \brief Update the user export configuration and create the temporary
     *        export configuration.
     */
    /****************************************************************************/
    void UpdateUserExportConfigurationAndWriteFile();

    /****************************************************************************/
    /**
     * \brief Creates the RMS file.     
     *
     * \iparam       FileName    File name of the file.
     *
     * \return On Successful (true) or not (false)
     */
    /****************************************************************************/
    bool CreateRMSFile(const QString FileName);

    /****************************************************************************/
    /**
     * \brief Creates the sorted station IDs from station container.
     *
     * \return List of sorted station IDs
     */
    /****************************************************************************/
    QStringList SortedStationIDs();

    /****************************************************************************/
    /**
     * \brief Write the data into RMS_status.csv file. Information of Station and
     *        Reagent data will be written.
     *
     * \oparam           RMSFile         Reference of the file.
     * \iparam           StationIDList   Stations IDs
     *
     * \return On Successful (true) or not (false)
     */
    /****************************************************************************/
    bool WriteRMSFileData(const QFile &RMSFile, const QStringList StationIDList);

    /****************************************************************************/
    /**
     * \brief Calculates the shelf life for the given reagent.
     *
     * \iparam       p_Reagent         Pointer to Reagent class.
     *
     * \return Shelf life value
     */
    /****************************************************************************/
    QString CalculateShelfLife(const DataManager::CReagent* p_Reagent);

    /****************************************************************************/
    /**
     * \brief Copy the configuration files from one location to other location.
     *        Configuration list contains all the file list, so read all the files
     *        from the list and copy the files to target location
     *
     * \iparam           ConfigurationList    Configuration class.
     * \iparam           TargetPath           Target path to copy the files.
     *
     * \return On Successful (true) or not (false)
     */
    /****************************************************************************/
    bool CopyConfigurationFiles(const DataManager::CConfigurationList &ConfigurationList,
                                QString TargetPath);

    /****************************************************************************/
    /**
     * \brief Check the existence of the file and copy the files to Target place
     *        Sometimes Target folder does not delete all the files, so check
     *        the existence of the file first before copying, if it exists delete
     *        and copy the new file
     *
     * \iparam           TargetFileName    Target file name.
     * \iparam           SourceFileName    Source file name.
     *
     * \return On Successful (true) or not (false)
     */
    /****************************************************************************/
    bool CheckTheExistenceAndCopyFile(QString TargetFileName, QString SourceFileName);

    /****************************************************************************/
    /**
     * \brief Cleans up the objects.
     */
    /****************************************************************************/
    void DoCleanUpObjects();
	
	/****************************************************************************/
    /**
     * \brief Imports files from the archive file.
     *
     * \iparam    ImportType  Type of Import
     * \iparam    FileName    Name of the file
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool ImportArchiveFiles(const QString &ImportType, QString FileName);

    /****************************************************************************/
    /**
     * \brief Adds required files to string list, so that importing can be done
     *        little easier .
     *
     * \iparam           TypeOfImport  Type of Import
     * \oparam           ListOfFiles   List of files to be imported
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool AddFilesForImportType(const QString &TypeOfImport, const QStringList &ListOfFiles);

    /****************************************************************************/
    /**
     * \brief Writes the files in a temporary loaction and imports the data into
     *        data containers.
     *
     * \iparam           TypeOfImport  Type of Import
     * \iparam           FileList      List of files to be imported
     * \iparam           RamFile       Object of the RAM file
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool WriteFilesAndImportData(const QString &TypeOfImport, const QStringList &FileList,
                                 const ImportExport::RAMFile &RamFile);

    /****************************************************************************/
    /**
     * \brief Compare software versions which is available in settings folder
     * and Import folder.
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool CompareSoftwareVersions();

    /****************************************************************************/
    /**
     * \brief Check for the files updated or not.
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool CheckForFilesUpdateError();

    /****************************************************************************/
    /**
     * \brief Update the settings folder with rollback folder configuration files.
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool UpdateSettingsWithRollbackFolder();

    /****************************************************************************/
    /**
     * \brief Resets the station and program sequence containers data.
     *        Reset the StartCapablefalg and BLGResult in Program sequence container
     *        Reset the Actual slides, Program ID, step ID and Reagent ID(Except for
     *        the Loader stations) in stations container
     * \iparam     TypeOfImport  Type of Import
     */
    /****************************************************************************/
    void ResetTheStationPSContainers(const QString TypeOfImport);

    /****************************************************************************/
    /**
     * \brief Initialize and verifies the data containers.
     *
     * \oparam         DataList          Container class
     * \oparam         Verifier          Verifier class for the container
     * \iparam         Path              Path of the file
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    template<class DataListClass, class VerifierClass>
    bool InitializeAndVerifyContainer(const DataListClass &DataList,
                                      const VerifierClass &Verifier, QString Path) {
        // remove the const cast
        DataListClass& Container = const_cast<DataListClass&>(DataList);
        VerifierClass& ContainerVerifier = const_cast<VerifierClass&>(Verifier);
        // register this verifier object in the data container (=> dependency injection)
        Container.AddVerifier(dynamic_cast<DataManager::IVerifierInterface*>(&ContainerVerifier));
        // set the data verification false before loading the data
        Container.SetDataVerificationMode(false);

        // read the file
        if (!Container.Read(Path)) {
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_INIT_CONTAINER_FAILED);
            return false;
        }
        // make the data verification true so that the container is verified
        Container.SetDataVerificationMode(true);

        // verify the data
        if (!Container.VerifyData()) {
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_VERIFICATION_CONTAINER_FAILED);
            return false;
        }
        // if everything goes well then retun success
        return true;
    }    

    /****************************************************************************/
    /**
     * \brief Imports Leica programs.
     *        Imports the new Leica programs to Program and program sequence
     *        container and updates for an existing Leica program which is present
     *        in program container
     *
     * \oparam         ProgramList  Program list Container class
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool ImportLeicaPrograms(const DataManager::CDataProgramList &ProgramList);

    /****************************************************************************/
    /**
     * \brief Imports Leica reagents and User Leica Reagents.
     *        Imports the new Leica reagents and new Leica user reagents to Reagent
     *        container and updates for an existing Leica reagent and new Leica reagent
     *        which is present in reagent container. Leica user reagents range is from
     *        U1 to U99
     *
     * \oparam         ReagentList  Reagent list Container class
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool ImportLeicaUserReagents(const DataManager::CDataReagentList &ReagentList);

    /****************************************************************************/
    /**
     * \brief Mounts the USB device.
     *
     *
     * \iparam     IsImport     Import flag
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool MountDevice(bool IsImport = false);


    /****************************************************************************/
    /**
     * \brief Update the folder with the files from the source to target folder.
     *
     * \iparam     FileList      Files in the list
     * \iparam     TargetPath    Target path of the folder
     * \iparam     SourcePath    Source path of the folder
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool UpdateFolderWithFiles(QStringList FileList, QString TargetPath,
                               QString SourcePath);

    /****************************************************************************/
    /**
     * \brief Write the configuration files in the settings folder.
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool WriteFilesInSettingsFolder();

    /****************************************************************************/
    /**
     * \brief Import the language files.
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool ImportLanguageFiles();

    /****************************************************************************/
    /**
     * \brief Unmount the device if the device is mounted.
     */
    /****************************************************************************/
    void UnMountDevice();

    /****************************************************************************/
    /**
     * \brief Copy the QM files from one location to other location.
     *
     * \oparam         Directory  Directory location object
     * \oparam         Rollback   Flag for Rollback
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    void CopyQMFiles(const QDir &Directory, const bool &Rollback);

    /****************************************************************************/
    /**
     * \brief Copy the Event string files from one location to other location.
     *
     * \oparam         Directory  Directory location object
     * \oparam         Rollback   Flag for Rollback
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    void CopyEventStringFiles(const QDir &Directory, const bool &Rollback);

protected:
    /****************************************************************************/
    /**
     * \brief This method is called when the base class received the \ref Go signal.
     *
     * This means that everything is fine and normal operation started.
     * We are running in our own thread now.\n
     * We do not have anything special to do, since objects are already configured
     * and ready for normal operation.
     */
    /****************************************************************************/
    virtual void OnGoReceived();
    /****************************************************************************/
    /**
     * \brief This method is called when the base class received the \ref Stop signal.
     *
     * This means that normal operation will stop after processing this signal.
     * We are still running in our own thread.\n
     * We do not have anything special to do.
     */
    /****************************************************************************/
    virtual void OnStopReceived();
    /****************************************************************************/
    /**
     * \brief Power will fail shortly.
     *
     * We try to log the power fail into the event logger and close all open files.
     * After that we switch to a PowerFail state where only events are processed
     * and dumped to console.
     *
     * \iparam PowerFailStage   Power fail stages
     */
    /****************************************************************************/
    virtual void OnPowerFail();
public:
    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \iparam           ThreadID        Unique Thread ID
     * \oparam           RefDataManager  Take the reference of the main data manager
     * \iparam           SourceType      Source type of the thread.
     * \iparam           CommandValue    Command data which is stored in bytearray
     */
    /****************************************************************************/    
    ImportExportThreadController(Global::gSourceType TheSource,
                                 const DataManager::CDataManager &RefDataManager,
                                 QString SourceType, QByteArray CommandValue);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~ImportExportThreadController();

    /****************************************************************************/
    /**
     * \brief Create and configure the loggers.
     *
     * If something goes wrong, the already created loggers are deleted.
     */
    /****************************************************************************/
    virtual void CreateAndInitializeObjects();

    /****************************************************************************/
    /**
     * \brief Cleanup and destroy the loggers.
     */
    /****************************************************************************/
    virtual void CleanupAndDestroyObjects();

    /****************************************************************************/
    /**
     * \brief Creates the containers and update the containers.
     *
     * \iparam       TypeOfImport    Type of Import
     * \iparam       FilePath        Path of the file to save
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool CreateAndUpdateContainers(const QString TypeOfImport, const QString FilePath);


    /****************************************************************************/
    /**
     * \brief Sets the event log file name.
     *
     * \iparam       FileName    Event log file name
     *
     */
    /****************************************************************************/
    void SetEventLogFileName(QString FileName) {
        m_EventLogFileName = FileName;
    }

signals:
    /****************************************************************************/
    /**
     * \brief Signal for starting the export process.
     * As soon as this signal received by Main it starts the Export process
     */
    /****************************************************************************/
    void StartExportProcess(QString FileName);

    /****************************************************************************/
    /**
     * \brief Signal for Thread finished its task.
     *  This signals main that this thread exited with some code and also updates
     *  about the language import
     *
     * \iparam       IsTaskDone                  Task done successful or not
     * \iparam       ImportTypeList              List of import types
     * \iparam       EventCode                   Event code
     * \iparam       UpdatedCurrentLanguage      Flag for current language
     * \iparam       NewLanguageAdded            Flag for new language
     *
     */
    /****************************************************************************/
    void ThreadFinished(const bool IsTaskDone, QStringList ImportTypeList, quint32 EventCode,
                        bool UpdatedCurrentLanguage = false, bool NewLanguageAdded = false);


    /****************************************************************************/
    /**
     * \brief Signal for request daily run log files.
     *  This signals main that export requires the daily run log files
     *
     */
    /****************************************************************************/
    void RequestDayRunLogFileNames();

    /****************************************************************************/
    /**
     * \brief Signal for selecting required files to import.
     *  This signals main that import detected multiple files, Main requests
     *  to display selection screen in GUI side
     *
     */
    /****************************************************************************/
    void RequestFileSelectionToImport(QStringList);

public slots:
    /****************************************************************************/
    /**
     * \brief Slot for getting the daily run log file directory name.
     *
     * \iparam DirectoryName - Name of the directory
     *
     */
    /****************************************************************************/
    void SetDayRunLogFilesDirectoryName(const QString& DirectoryName);

    /****************************************************************************/
    /**
     * \brief Slot for importing files.
     *
     * \iparam FileList - List of files
     *
     */
    /****************************************************************************/
    void StartImportingFiles(const QStringList FileList);

};

} // end namespace ImportExport

#endif // IMPORTEXPORT_IMPORTEXPORTTHREADCONTROLLER_H
