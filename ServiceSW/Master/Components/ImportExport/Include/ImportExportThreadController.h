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
#include "DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataExport.h"
#include "DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataImport.h"
#include "DataManager/Containers/ExportConfiguration/Include/ExportConfigurationVerifier.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramListVerifier.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentListVerifier.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Include/DataStationListVerifier.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsVerifier.h"
#include "DataManager/Containers/SWVersions/Include/SWVersionListVerifier.h"
#include "Global/Include/SystemPaths.h"
#include "EncryptionDecryption/WriteArchive/Include/WriteArchive.h"
#include "EncryptionDecryption/ReadArchive/Include/ReadArchive.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardDataStationList.h"

#include <QProcess>
#include <QMutex>
#include <QWaitCondition>


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
    Q_OBJECT
    friend class TestHimalayaMasterThread;
private:    
    //DataManager::CDataManager& m_DataManager;                   ///< Store the data manager - pointer
    DataManager::CDashboardDataStationList* mp_StationList;              ///< Store the Station list
    DataManager::CDataReagentList* mp_ReagentList;              ///< Store the reagent list
    DataManager::CExportConfiguration* mp_ExportConfiguration;  ///< Store export configuration    
    QString m_CommandValue;                                     ///< Store the command value
    QString m_CommandName;                                      ///< Store the command name i.e. either user export or service export
    QString m_SerialNumber;                                     ///< Store the serial number of the device
    QString m_DeviceName;                                       ///< Store the device name
    bool m_WorkStationMode;                                     ///< Store the workstation mode
    bool m_ThreadInitialized;                                   ///< Thread initialization flag
    bool m_NewLanguageAdded;                                    ///< Store the flag for new language
    bool m_CurrentLanguageUpdated;                              ///< Store the flag for current language
    bool m_EventRaised;                                         ///< Store the event information flag for raise event
    bool m_UserExport;                                          ///< Store the user export flag
    QString m_DayRunLogDirectoryName;                           ///< Store the day run log directory name
    DataManager::CExportConfiguration m_TempExportConfiguration;  ///< Store temporary export configuration


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
     * \param[in]   FileName    File name of the file.
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
     * \param[in,out]   RMSFile         Reference of the file.
     * \param[in]       StationIDList   Stations IDs
     *
     * \return On Successful (true) or not (false)
     */
    /****************************************************************************/
    bool WriteRMSFileData(const QFile &RMSFile, const QStringList StationIDList);

    /****************************************************************************/
    /**
     * \brief Calculates the shelf life for the given reagent.
     *
     * \param[in]   p_Reagent         Pointer to Reagent class.
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
     * \param[in]       ConfigurationList    Configuration class.
     * \param[in]       TargetPath           Target path to copy the files.
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
     * \param[in]       TargetFileName    Target file name.
     * \param[in]       SourceFileName    Source file name.
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
     * \param[in] ImportType  Type of Import
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool ImportArchiveFiles(const QString &ImportType);

    /****************************************************************************/
    /**
     * \brief Adds required files to string list, so that importing can be done
     *        little easier .
     *
     * \param[in]       TypeOfImport  Type of Import
     * \param[in,out]   ListOfFiles   List of files to be imported
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
     * \param[in]       TypeOfImport  Type of Import
     * \param[in]       FileList      List of files to be imported
     * \param[in]       RamFile       Object of the RAM file
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool WriteFilesAndImportData(const QString &TypeOfImport, const QStringList &FileList,
                                 const EncryptionDecryption::RAMFile &RamFile);

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
     * \brief Initialize and verifies the data containers.
     *
     * \param[in,out] DataList          Container class
     * \param[in,out] Verifier          Verifier class for the container
     * \param[in]     Path              Path of the file
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
     * \brief Search and Mounts the USB device.
     *        It searches the sd* in the dev directory and iterates the each device
     *        and try to mounts the device
     *
     * \param[in] IsImport     Import flag
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool SearchAndMountTheDevice(bool IsImport = false);

    /****************************************************************************/
    /**
     * \brief Mount the device with a specified name in the "/mnt/USB/" folder.
     *
     * \param[in,out] Process        Process to mount the device
     * \param[in]     DeviceName     Device name to mount
     * \param[in]     IsImport       Import flag
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool MountTheSpecificDevice(const QProcess &Process, QString DeviceName,
                                bool IsImport);

    /****************************************************************************/
    /**
     * \brief Update the folder with the files from the source to target folder.
     *
     * \param[in] FileList      Files in the list
     * \param[in] TargetPath    Target path of the folder
     * \param[in] SourcePath    Source path of the folder
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
    void UnMountTheDevice();

    /****************************************************************************/
    /**
     * \brief Copy the QM files from one location to other location.
     *
     * \param[in,out] Directory  Directory location object
     * \param[in,out] Rollback   Flag for Rollback
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    void CopyQMFiles(const QDir &Directory, const bool &Rollback);

    /****************************************************************************/
    /**
     * \brief Copy the Event string files from one location to other location.
     *
     * \param[in,out] Directory  Directory location object
     * \param[in,out] Rollback   Flag for Rollback
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    void CopyEventStringFiles(const QDir &Directory, const bool &Rollback);

protected:
    /****************************************************************************/
    /**
     * \brief This method is called when the base class received the Go signal.
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
     * \brief This method is called when the base class received the Stop signal.
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
     */
    /****************************************************************************/
    virtual void OnPowerFail();
public:
    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \param[in]       TheSource       Logging source to be used.
     * \param[in]       SourceType      Source type of the thread.
     * \param[in]       CommandValue    Command data which is stored in bytearray
     */
    /****************************************************************************/    
    ImportExportThreadController(Global::gSourceType TheSource,
                                // const DataManager::CDataManager &RefDataManager,
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
     * \param[in]   TypeOfImport    Type of Import
     * \param[in]   FilePath        Path of the file to save
     *
     * \return On successful (true) or not (false)
     */
    /****************************************************************************/
    bool CreateAndUpdateContainers(const QString TypeOfImport, const QString FilePath);

signals:
    /****************************************************************************/
    /**
     * \brief Signal for starting the export process.
     * As soon as this signal received by Main it starts the Export process
     */
    /****************************************************************************/
    void StartExportProcess();

    /****************************************************************************/
    /**
     * \brief Signal for Thread finished its task.
     *  This signals main that this thread exited with some code and also updates
     *  about the language import
     *
     * \param[in]   IsTaskDone                  Task done successful or not
     * \param[in]   TypeOfImport                Type of Import
     * \param[in]   UpdatedCurrentLanguage      Flag for current language
     * \param[in]   NewLanguageAdded            Flag for new language
     *
     */
    /****************************************************************************/
    void ThreadFinished(const bool IsTaskDone, const QString &TypeOfImport,
                        bool UpdatedCurrentLanguage = false, bool NewLanguageAdded = false);


    /****************************************************************************/
    /**
     * \brief Signal for request daily run log files.
     *  This signals main that export requires the daily run log files
     *
     */
    /****************************************************************************/
    void RequestDayRunLogFileNames();

public slots:
    /****************************************************************************/
    /**
     * \brief Signal for getting the daily run log file directory name.
     *
     */
    /****************************************************************************/
    void SetDayRunLogFilesDirectoryName(const QString& DirectoryName);

};

} // end namespace ImportExport

#endif // IMPORTEXPORT_IMPORTEXPORTTHREADCONTROLLER_H
