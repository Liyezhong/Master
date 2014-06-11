/****************************************************************************/
/*! \file ImportExportThreadController.cpp
 *
 *  \brief Implementation file for class ImportExportThreadController.
 *
 *  \b Description:
 *         1. Searches and Mounts the device for the Export and Import.
 *         2. Do the pre-tasks (Identifies which files needs to be exported) before
 *            creating the export process
 *         3. Read the Import file and takes a copy of the updated files in Rollback folder
 *         4. Updates the data containers (Programs.xml, ProgramsSequence.xml, Reagents.xml,
 *            UserSettings.xml and Stations.xml)
 *         5. Creates the temporary directories
 *         6. Updates the Rollback folder before importing and after importing the data
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

#include "ImportExport/Include/ImportExportThreadController.h"
#include "Global/Include/Utils.h"
#include "Global/Include/GlobalEventCodes.h"
#include <QProcess>

namespace ImportExport {

// constants for the different files
const QString FILENAME_REAGENTS             = "HimalayaReagents.xml"; ///< const for the reagent file name
const QString FILENAME_PROGRAMS             = "HimalayaPrograms.xml"; ///< const for the programs file name
const QString FILENAME_REAGENT_GROUPS       = "HimalayaReagentGroups.xml"; ///< const for the reagent groups file name
const QString FILENAME_REAGENT_GROUP_COLORS = "HimalayaReagentGroupColors.xml"; ///< const for the reagent groups file name
const QString FILENAME_STATIONS             = "HimalayaStations.xml"; ///< const for the stations file name
const QString FILENAME_USERSETTINGS         = "UserSettings.xml"; ///< const for the user settings file name
const QString FILENAME_SWVERSION            = "SW_Version.xml"; ///< const for the user settings file name
const QString FILENAME_QM                   = "Himalaya_*.qm"; ///< const for the QM file name
const QString FILENAME_EVENTSTRING          = "HimalayaEventStrings_*.xml"; ///< const for the event string file name
const QString FILENAME_EXPORTCONFIG         = "ExportConfiguration.xml"; ///< const for the export configuration file name
const QString FILENAME_RMSSTATUS            = "RMS_Status.csv"; ///< const for the RMS status file name
const QString FILENAME_TEMPEXPORTCONFIG     = "TempExportConfiguration.xml"; ///< const for the temporary export configuration file name
const QString FILTER_EXTENSION_ANY          = "*.*"; ///< const for the any file names
const QString FILTER_EXTENSION_LOG          = "*.log"; ///< const for the any file names
const QString FILENAME_DAILYRUNLOG          = "DailyRun*.log"; ///< const for the dialy run log file name
const QString FILENAME_COLEVENTSTRINGS      = "HimalayaEventStrings_"; ///< const for the himalaya event string file name

// constant for command name
const QString COMMAND_NAME_IMPORT           = "Import"; ///< constant for the command name

// constants for directory names
const QString DIRECTORY_IMPORT              = "Import"; ///< constant for the Import directory name
const QString DIRECTORY_EXPORT              = "Export"; ///< constant for the Export directory name
const QString DIRECTORY_SETTINGS            = "Settings"; ///< constant for the settings directory name
const QString DIRECTORY_ROLLBACK            = "Rollback"; ///< constant for the Rollback directory name
const QString DIRECTORY_TEMPORARY           = "Temporary"; ///< constant for the Temporary directory name
const QString DIRECTORY_USB                 = "USB"; ///< constant for the USB directory name
const QString DIRECTORY_TRANSLATIONS        = "Translations"; ///< constant for the Translations directory name
const QString DIRECTORY_LOGFILES            = "Logfiles"; ///< constant for the Logfiles directory name

// constants for system directory paths
//const QString DIRECTORY_MNT                 = "/mnt"; ///< constant for the mnt directory
const QString DIRECTORY_DEV                 = "/dev"; ///< constant for the dev directory
const QString DIRECTORY_SH                  = "/bin/sh"; ///< constant for the shell directory
const QString DIRECTORY_SD                  = "sd"; ///< constant for the sd directory
const QString DEVICE_FILE_SD                = "/dev/mmpblkpk0"; ///< constant for sd card device file name

// constants for the file type
const QString FILETYPE_LPKG                 = "*.lpkg"; ///< constant for the lpkg file type
const QString FILETYPE_QM                   = "*.qm"; ///< constant for the qm file type
const QString FILETYPE_XML                  = ".xml"; ///< constant for the xml file type

// constants for the type of Import
const QString TYPEOFIMPORT_SERVICE          = "Service"; ///< constant for the 'Service' of Import
const QString TYPEOFIMPORT_USER             = "User"; ///< constant for the 'User' of Import
const QString TYPEOFIMPORT_LEICA            = "Leica"; ///< constant for the 'Leica' of Import
const QString TYPEOFIMPORT_LANGUAGE         = "Language"; ///< constant for the 'Language' of Import

// constant for strings
const QString STRING_UNDEFINED              = "UNDEFINED"; ///< constant for the UNDEFINED string
const QString STRING_EXPIRED                = "Expired"; ///< constant for the Expired station
const QString STRING_H                      = "H"; ///< constant string for Heated stations
const QString STRING_L                      = "L"; ///< constant string for Loading stations
const QString STRING_R                      = "R"; ///< constant string for regular stations
const QString STRING_U                      = "U"; ///< constant string for unloading stations
const QString STRING_T                      = "T"; ///< constant string for time
const QString STRING_SPACE                  = " "; ///< constant string for space
const QString STRING_NEWLINE                = "\n"; ///< constant string for new line
const QString STRING_YES                    = "Yes"; ///< constant string for Yes
const QString STRING_NO                     = "No"; ///< constant string for No
const QString STRING_PERCENTNUMBER          = "%1"; ///< constant string for percent one
const QString STRING_NEGNUMBER              = "-1"; ///< constant string for minus one
const QString STRING_MIUNS                  = "-"; ///< constant string for minus
const QString STRING_WORKSTATION            = "workstation"; ///< constant string for workstation
const QString STRING_STANDALONE             = "standalone"; ///< constant string for standalone

// constant for delimiters
const QString DELIMITER_STRING_UNDERSCORE   = "_"; ///< constant for the delimiter underscore
const QString DELIMITER_STRING_COMMA        = ","; ///< constant for the delimiter comma

// constant for fromat strings
const QString FORMAT_DATE                   = "dd.MM.yyyy"; ///< constant for the date format
const QString FORMAT_TIME                   = "hh:mm"; ///< constant for the time format

// constants for CSV
const QString CSV_STRING_FORMATVERSION      = "Format Version: 1\nFileName: RMS_Status.csv\n"; ///< constant string format version
const QString CSV_STRING_TIMESTAMP          = "TimeStamp: "; ///< constant string for Timestamp
const QString CSV_STRING_OPERATINGMODE      = "OperatingMode: "; ///< constant string for operating mode
const QString CSV_STRING_SERIALNUMBER       = "SerialNumber: "; ///< constant string for serial number
const QString CSV_STRING_COLHEADER          = "Station ID,Reagent,RMS,Remaining slides,Slides max,Vial shelf life,Best before date,Lot No,Exclusive\n"; ///< constant string for the column header

// constants for commands
const QString COMMAND_MKDIR                 = "mkdir "; ///< constant string for the command 'mkdir'
const QString COMMAND_MOUNT                 = "mount "; ///< constant string for the command 'mount'
const QString COMMAND_RM                    = "rm "; ///< constant string for the command 'rm'
const QString COMMAND_RMDIR                 = "rmdir "; ///< constant string for the command 'rmdir'
const QString COMMAND_ARGUMENT_C            = "-c"; ///< constant string for the command argument for shell '-c'
const QString COMMAND_ARGUMENT_R            = "-rf"; ///< constant string for the command argument for recursive files '-r'

// constants for wildcharacters
const QString WILDCHAR_ASTRIK               = "*"; ///< constant for wild char

const char FILEMODE_WRITE                   = 'w'; ///< constant for the file write mode
const char CHAR_L                           = 'L'; ///< constant for the Leica reagents
const char CHAR_DOT                         = '.'; ///< constant for the dot
const char CHAR_UNDERSCORE                  = '_'; ///< constant for the underscore

// constants for the bytes - these bytes represents that the CSV file will write in Unicode format
const QString CSV_START_BYTES               = "\xEF\xBB\xBF"; ///< constant for EF

/****************************************************************************/
ImportExportThreadController::ImportExportThreadController(Global::gSourceType TheSource,
                                                           const DataManager::CDataManager &RefDataManager,
                                                           QString SourceType,
                                                           QByteArray CommandValue) :
    Threads::ThreadController(TheSource, SourceType),
    m_DataManager(const_cast<DataManager::CDataManager&>(RefDataManager)),
    mp_StationList(NULL),
    mp_ReagentList(NULL),
    mp_ExportConfiguration(NULL),
    m_CommandName(SourceType),
    m_SerialNumber(STRING_UNDEFINED),
    m_DeviceName(STRING_UNDEFINED),
    m_WorkStationMode(false),
    m_ThreadInitialized(false),
    m_NewLanguageAdded(false),
    m_CurrentLanguageUpdated(false),
    m_EventRaised(false),
    m_UserExport(false),
    m_RemoteCareRequest(false),
    m_DayRunLogDirectoryName(""),
    m_EventCode(0),
    m_EventLogFileName(""),
    m_TargetFileName("") {

    QString StringData(CommandValue);

    m_CommandValue.clear();
    // always the first one will be the command type
    m_CommandValue = StringData.split("_").value(0);
    // default value
    m_NoOfLogFiles = 5;
    if (StringData.split("_").count() > 1) {
        m_NoOfLogFiles = StringData.split("_").value(1).toInt();
    }
    /*lint -e1401 */
}


/****************************************************************************/
ImportExportThreadController::~ImportExportThreadController() {
    try {
        // clear the objects
        DoCleanUpObjects();
    }
    catch (...) {
        // do nothing
    }
}

/****************************************************************************/
void ImportExportThreadController::CreateAndInitializeObjects() {
    // initialize the station list and reagent list
    // Station list and reagent list requires only when the Export functionality is called by Main

    if (!m_CommandName.contains(COMMAND_NAME_IMPORT)) {
        // do a deep copy of the data container
        if (mp_StationList == NULL) {
            mp_StationList = new DataManager::CDashboardDataStationList();
        }
        // copy the station list
        *mp_StationList = *m_DataManager.GetStationList();

        // do a deep copy of the data.
        if (mp_ReagentList == NULL) {
            mp_ReagentList = new DataManager::CDataReagentList();
        }
        *mp_ReagentList = *m_DataManager.GetReagentList();
    }

    // get the device configuration so that we can use the device name
    // and serial number of the device for the Export component
    DataManager::CDeviceConfigurationInterface *p_DeviceInterface = NULL;
    p_DeviceInterface = m_DataManager.GetDeviceConfigurationInterface();

    if (p_DeviceInterface != NULL) {
        DataManager::CDeviceConfiguration *p_DeviceConfiguration = NULL;
        p_DeviceConfiguration = p_DeviceInterface->GetDeviceConfiguration();

        if (p_DeviceConfiguration != NULL) {
            m_DeviceName = p_DeviceConfiguration->GetValue("DeviceName");
            m_SerialNumber = p_DeviceConfiguration->GetValue("SerialNumber");
            m_WorkStationMode = p_DeviceConfiguration->GetBoolValue("WorkstationMode");
        }
    }
}

/****************************************************************************/
void ImportExportThreadController::CleanupAndDestroyObjects() {
    // call stop received event
    OnStopReceived();
}

/****************************************************************************/
void ImportExportThreadController::OnGoReceived() {

    QString TypeOfImport;    

    if (!m_ThreadInitialized) {
        m_ThreadInitialized = true;
        CreateRequiredDirectories();


        if (!m_CommandName.contains(COMMAND_NAME_IMPORT)) {
            bool ErrorInThreadExecution = true;
            // write the files in settings directory
            (void)WriteFilesInSettingsFolder();

            if (MountDevice()) {
                // do pre-tasks before emitting the Export process signal
                if (DoPretasks()) {

                    // for user export we need to know the daily run log files
                    if (!m_UserExport) {
                        // clear the objects
                        DoCleanUpObjects();                        
                        // emit the export process signal
                        emit StartExportProcess(m_TargetFileName);
                    }
                    ErrorInThreadExecution = false;
                }                
            }            

            if (ErrorInThreadExecution) {
                // if the event is not raised then display an error due to any reason
                if (!m_EventRaised) {
                    Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_FAILED, true);
                    m_EventCode = Global::EVENT_EXPORT_FAILED;
                }
                // emit the thread finished flag - with error code
                emit ThreadFinished(false, QStringList(), m_EventCode);
            }
        }
        else {
            // mount the USB device
            if (MountDevice(true, "*.lpkg")) {
                // to store the files in the directory
                QStringList DirFileNames;
                QDir Dir(Global::DIRECTORY_MNT_STORAGE + QDir::separator() + DIRECTORY_IMPORT);
                // get the lpkg files from the directory
                DirFileNames = Dir.entryList(QStringList(FILETYPE_LPKG), QDir::Files, QDir::Name | QDir::Time);
                switch(DirFileNames.count()) {
                    case 0:
                        m_EventRaised = true;
                        Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_NO_FILES_TO_IMPORT);
                        m_EventCode = EVENT_IMPORT_NO_FILES_TO_IMPORT;
                        emit ThreadFinished(false, QStringList(), m_EventCode, m_CurrentLanguageUpdated, m_NewLanguageAdded);
                        break;
                    case 1:
                        // get the return code
                        StartImportingFiles(DirFileNames);
                        break;
                    default:
                        // sort the files
                        emit RequestFileSelectionToImport(DirFileNames);
                        break;
                }

            }
            else{
                emit ThreadFinished(false, QStringList() << TypeOfImport, m_EventCode, m_CurrentLanguageUpdated, m_NewLanguageAdded);
            }
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_IMPORTEXPORT_THREADRUNNING, true);
        m_EventCode = EVENT_IMPORTEXPORT_THREADRUNNING;
        emit ThreadFinished(false, QStringList(), m_EventCode, m_CurrentLanguageUpdated, m_NewLanguageAdded);
    }    
}

/****************************************************************************/
void ImportExportThreadController::OnStopReceived() {
    // unmount the device
    UnMountDevice();

    QString DirectoryName;
    // this temporary directory will be used everybody so dont use the command line arguments
    if (!m_CommandName.contains(COMMAND_NAME_IMPORT)) {
        DirectoryName = DIRECTORY_EXPORT;
    }
    else {
        DirectoryName = DIRECTORY_IMPORT;

    }

    QStringList Options;
    // -c option for shell and remove directory
    Options << COMMAND_ARGUMENT_C << COMMAND_RM + COMMAND_ARGUMENT_R + STRING_SPACE
               + Global::SystemPaths::Instance().GetTempPath()
               + QDir::separator() + DirectoryName;
    // execute the process with "/bin/sh"
    // remove all the files from the temporary directory "rm -rf Temporary/Import" or "rm -rf Temporary/Export"
    (void)QProcess::execute(DIRECTORY_SH, Options);
    // this is to make sure all the xpresobjects are cleared.
    DoCleanUpObjects();
    // reset the thread initialization flag
    m_ThreadInitialized = false;
}

/****************************************************************************/
void ImportExportThreadController::OnPowerFail(const Global::PowerFailStages PowerFailStage) {
    // call stop received event
    Q_UNUSED(PowerFailStage)
    OnStopReceived();
}

/****************************************************************************/
void ImportExportThreadController::CreateRequiredDirectories() {
    // make sure that all the directories available to do further - Otherwise end up with error
    QDir DirectoryExist;
    // check the existence of the rollback directory
    if (!DirectoryExist.exists(Global::SystemPaths::Instance().GetRollbackPath())) {
        // create the Rollback folder, if not exists
        (void)QProcess::execute(COMMAND_MKDIR + Global::SystemPaths::Instance().GetRollbackPath());  //to avoid lint-534
    }
    // check the existence of the Temporary directory
    if (!DirectoryExist.exists(Global::SystemPaths::Instance().GetTempPath())) {
        // create the Translations directory, if not exists
        (void)QProcess::execute(COMMAND_MKDIR + Global::SystemPaths::Instance().GetTempPath());  //to avoid lint-534

    }

    // check the existence of the Rollback/Settings directory
    if (!DirectoryExist.exists(Global::SystemPaths::Instance().GetRollbackPath()
                               + QDir::separator() + DIRECTORY_SETTINGS)) {
        // create the Rollback/Settings directory, if not exists
        (void)QProcess::execute(COMMAND_MKDIR + Global::SystemPaths::Instance().GetRollbackPath()
                              + QDir::separator() + DIRECTORY_SETTINGS);  //to avoid lint-534
    }

    // check the existence of the Rollback/Translations directory
    if (!DirectoryExist.exists(Global::SystemPaths::Instance().GetRollbackPath()
                               + QDir::separator() + DIRECTORY_TRANSLATIONS)) {
        // create the Rollback/Translations folder, if not exists
        (void)QProcess::execute(COMMAND_MKDIR + Global::SystemPaths::Instance().GetRollbackPath()
                              + QDir::separator() + DIRECTORY_TRANSLATIONS);  //to avoid lint-534
    }
}

/****************************************************************************/
bool ImportExportThreadController::DoPretasks() {

    // Create the export configuration object
    mp_ExportConfiguration =  new DataManager::CExportConfiguration();
    mp_ExportConfiguration->SetDataVerificationMode(false);

    if (mp_ExportConfiguration->Read(Global::SystemPaths::Instance().GetSettingsPath()
                                     + QDir::separator() + FILENAME_EXPORTCONFIG)) {

        if (!m_RemoteCareRequest) {
            // set the target directory path "../mnt_storage"
            mp_ExportConfiguration->SetTargetDir(Global::DIRECTORY_MNT_STORAGE);
        }
        else {
            // set the target directory path "../RemoteCare"
            mp_ExportConfiguration->SetTargetDir(Global::SystemPaths::Instance().GetRemoteCarePath());
        }

        // create the Export directory in "Temporary" folder
        if (QProcess::execute(COMMAND_MKDIR + Global::SystemPaths::Instance().GetTempPath()
                              + QDir::separator() + DIRECTORY_EXPORT) >= 0) {
            // form the trget file name format is devicename_typeofexport_serialnumber_datetimestamp
            QString TargetFileName = m_DeviceName + DELIMITER_STRING_UNDERSCORE;

            if (m_CommandValue.compare(TYPEOFIMPORT_USER) == 0) {
                mp_ExportConfiguration->SetUserConfigurationFlag(true);
                TargetFileName += TYPEOFIMPORT_USER + DELIMITER_STRING_UNDERSCORE;
            }
            else if (m_CommandValue.compare(TYPEOFIMPORT_SERVICE) == 0) {                
                mp_ExportConfiguration->SetServiceConfigurationFlag(true);
                // concatenate other informtion to the target file name
                TargetFileName += TYPEOFIMPORT_SERVICE + DELIMITER_STRING_UNDERSCORE;
            }
            else {
                return false;
            }

            // concatenate other information to the target file name, dont add the time stamp here. This will be done by
            // export process
            TargetFileName += m_SerialNumber + DELIMITER_STRING_UNDERSCORE + STRING_PERCENTNUMBER;
            m_TargetFileName = TargetFileName;
            mp_ExportConfiguration->SetTargetFileName(TargetFileName);

//            QString ExportDirPath = Global::SystemPaths::Instance().GetTempPath() + QDir::separator() + DIRECTORY_EXPORT;

            // create RMS file
/*
            if (!CreateRMSFile(ExportDirPath + QDir::separator() + FILENAME_RMSSTATUS)) {
                Global::EventObject::Instance().RaiseEvent(EVENT_EXPORT_UNABLE_TO_CREATE_FILE_RMS_STATUS);
                return false;
            }            
*/
            if (!WriteTempExportConfigurationAndFiles()) {
                return false;
            }
        }
        else {
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_DIRECTORY_CREATION_FAILED, true);
            m_EventCode = Global::EVENT_EXPORT_DIRECTORY_CREATION_FAILED;
            m_EventRaised = true;
            return false;
        }
    }
    else {
        return false;
    }
    return true;
}

/****************************************************************************/
bool ImportExportThreadController::WriteTempExportConfigurationAndFiles() {

    if (mp_ExportConfiguration != NULL) {
        // do a deep copy of the data
        m_TempExportConfiguration = *mp_ExportConfiguration;
    }
    // set the destination directory as temp "Temporary/Export"
    m_TempExportConfiguration.SetSourceDir(Global::SystemPaths::Instance().GetTempPath() + QDir::separator() + DIRECTORY_EXPORT);

    if (m_TempExportConfiguration.GetUserConfigurationFlag()) {
        emit RequestDayRunLogFileNames(m_TempExportConfiguration.GetSourceDir());
        m_UserExport = true;
    }

    if (m_TempExportConfiguration.GetServiceConfigurationFlag()) {
        // copy all the files in a temporary directory
        if (!CopyConfigurationFiles(mp_ExportConfiguration->GetServiceConfiguration().GetServiceConfigurationList(),
                                    m_TempExportConfiguration.GetSourceDir())) {
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_FILES_NOT_COPIED, true);
            m_EventCode = Global::EVENT_EXPORT_FILES_NOT_COPIED;
            m_EventRaised = true;
            return false;
        }
        // write the Export configuration file
        if (!m_TempExportConfiguration.Write(m_TempExportConfiguration.GetSourceDir() + QDir::separator() + FILENAME_TEMPEXPORTCONFIG)) {
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_UNABLE_TO_CREATE_FILE_TEMP_EXPORTCONFIGURATION, true);
            m_EventCode = Global::EVENT_EXPORT_UNABLE_TO_CREATE_FILE_TEMP_EXPORTCONFIGURATION;
            m_EventRaised = true;
            return false;
        }
    }    

    return true;
}


/****************************************************************************/
void ImportExportThreadController::SetDayRunLogFilesDirectoryName(const QString &DirectoryName) {
    m_DayRunLogDirectoryName = DirectoryName;
    UpdateUserExportConfigurationAndWriteFile();
}

/****************************************************************************/
void ImportExportThreadController::UpdateUserExportConfigurationAndWriteFile() {
    bool ErrorInExecution = false;    
    if (mp_ExportConfiguration != NULL) {
        if (m_TempExportConfiguration.GetUserConfigurationFlag()) {
            // copy all the files in a temporary directory
            if (CopyConfigurationFiles(mp_ExportConfiguration->GetUserConfiguration().GetUserConfigurationList(),
                                       m_TempExportConfiguration.GetSourceDir())) {
                // set the folder name so that all the files can be iterated
                m_TempExportConfiguration.GetUserConfiguration().GetUserReportList().SetGroupFileName(m_DayRunLogDirectoryName);
                if (!CopyConfigurationFiles(mp_ExportConfiguration->GetUserConfiguration().GetUserReportList(),
                                            m_TempExportConfiguration.GetSourceDir())) {
                    Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_FILES_NOT_COPIED, true);
                    m_EventCode = Global::EVENT_EXPORT_FILES_NOT_COPIED;
                    m_EventRaised = true;
                    ErrorInExecution = true;
                }
            }
            else {
                ErrorInExecution = true;
            }
        }

        // write the Export configuration file
        if (!m_TempExportConfiguration.Write(m_TempExportConfiguration.GetSourceDir() + QDir::separator() + FILENAME_TEMPEXPORTCONFIG)) {
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_UNABLE_TO_CREATE_FILE_TEMP_EXPORTCONFIGURATION, true);
            m_EventCode = Global::EVENT_EXPORT_UNABLE_TO_CREATE_FILE_TEMP_EXPORTCONFIGURATION;
            m_EventRaised = true;
            ErrorInExecution = true;
        }
    }
    else {
        ErrorInExecution = true;
    }

    if (!ErrorInExecution) {
        // clear the objects
        DoCleanUpObjects();
        // emit the export process signal
        emit StartExportProcess(m_TargetFileName);
    }

    if (ErrorInExecution) {
        // if the event is not raised then display an error due to any reason
        if (!m_EventRaised) {
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_FAILED, true);
            m_EventCode = Global::EVENT_EXPORT_FAILED;
        }
        // emit the thread finished flag - with error code
        emit ThreadFinished(false, QStringList(), m_EventCode);
    }

}


/****************************************************************************/
bool ImportExportThreadController::CreateRMSFile(const QString FileName) {
    // check whether station list exists or not
    // this is prerequiste to create the RMS file
    if (mp_StationList != NULL && mp_ReagentList != NULL) {
        // create the RMS file
        QFile RMSFile(FileName);
        // open the file in write mode
        if (RMSFile.open(QIODevice::WriteOnly)) {            
            if (RMSFile.write(QByteArray().insert(0, CSV_START_BYTES)) == -1) {
                // file will be closed automatically when function is exited because QFile is not a pointer
                // this is for safety
                RMSFile.close();
                return false;
            }
            QString FileData;
            // write starting information to the file
            FileData = CSV_STRING_FORMATVERSION + CSV_STRING_TIMESTAMP +
                       QDateTime::currentDateTime().toString(Qt::ISODate).replace(STRING_T, STRING_SPACE) + STRING_NEWLINE +
                       CSV_STRING_OPERATINGMODE + (m_WorkStationMode ? STRING_WORKSTATION : STRING_STANDALONE) +
                       STRING_NEWLINE + CSV_STRING_SERIALNUMBER + m_SerialNumber + STRING_NEWLINE + CSV_STRING_COLHEADER;

            if (RMSFile.write(qPrintable(FileData)) == -1) {
                // file will be closed automatically when function is exited because QFile is not a pointer
                // this is for safety
                RMSFile.close();
                return false;
            }

            if (!WriteRMSFileData(RMSFile, SortedStationIDs())) {
                // file will be closed automatically when function is exited because QFile is not a pointer
                // this is for safety
                RMSFile.close();
                return false;
            }
            // close the file once the data is written into the file need not require to flush it, close take cares of it
            RMSFile.close();
        }
        else {
            return false;
        }
    }
    return true;
}

/****************************************************************************/
QStringList ImportExportThreadController::SortedStationIDs() {
    // need to sort all the station IDs it should be like first heated stations, regular stations
    // Loading stations and Unloading stations
    // write the contents of the file
    QStringList StationIDList;
    // check whether station list exists or not
    // this is prerequiste to create sorted station ids
#ifndef HIMALAYA_IMPORT_EXPORT
    if (mp_StationList != NULL) {
        QStringList StringNames;
        // store the station ID starting letters so it is easy to sort the data
        StringNames << STRING_H << STRING_R << STRING_L << STRING_U;

        for (int StringCount = 0; StringCount < StringNames.count(); StringCount++) {
            QStringList SortedIDs;
            for (int StationCount = 0; StationCount < mp_StationList->GetNumberOfStations(); StationCount++) {
                DataManager::CDashboardStation* p_Station = const_cast<DataManager::CDashboardStation*>(mp_StationList->GetStation(StationCount));
                if (p_Station != NULL) {
                    // check the station ID starting letter with the string name
                    if (p_Station->GetStationID().left(1).compare(StringNames.value(StringCount)) == 0) {
                        SortedIDs << p_Station->GetStationID();
                    }
                }
            }
            SortedIDs.sort();
            StationIDList << SortedIDs;
            SortedIDs.clear();
        }
    }
#endif
    return StationIDList;
}

/****************************************************************************/
bool ImportExportThreadController::WriteRMSFileData(const QFile &RMSFile,
                                                    const QStringList StationIDList) {
    // check whether station list exists or not
    // this is prerequiste to create the RMS file
    Q_UNUSED(RMSFile)
    Q_UNUSED(StationIDList)
#ifndef HIMALAYA_IMPORT_EXPORT
    if (mp_StationList != NULL && mp_ReagentList != NULL) {
        QString FileData;

        for (int StationIDCount = 0; StationIDCount < StationIDList.count(); StationIDCount++) {
            DataManager::CDashboardStation* p_Station = const_cast<DataManager::CDashboardStation*>
                    (mp_StationList->GetStation(StationIDList.value(StationIDCount)));

            if (p_Station != NULL) {
                FileData = p_Station->GetStationID() + DELIMITER_STRING_COMMA;
                // get the reagent data from the station reagent ID
                DataManager::CReagent* p_Reagent = const_cast<DataManager::CReagent*>
                        (mp_ReagentList->GetReagent(p_Station->GetReagentID()));

                if (p_Reagent != NULL) {
                    FileData += p_Reagent->GetLongName() + DELIMITER_STRING_COMMA;
                }
                else {
                    FileData += STRING_MIUNS + DELIMITER_STRING_COMMA;
                }

                FileData += QString::number(p_Station->GetReagentStatusValue()) + DELIMITER_STRING_COMMA;
                // get the remaining slides
                if (p_Reagent != NULL) {
                    FileData += QString::number(p_Reagent->GetMaxSlides() - p_Station->GetNumberOfActualSlides())
                            + DELIMITER_STRING_COMMA + QString::number(p_Reagent->GetMaxSlides()) + DELIMITER_STRING_COMMA;
                }
                else {
                    // if the reagent does not exist then add nothing exists (i.e. "-")
                    FileData += STRING_MIUNS + DELIMITER_STRING_COMMA + STRING_MIUNS + DELIMITER_STRING_COMMA;
                }                

                QString ShelfLife = CalculateShelfLife(p_Reagent);
                // for the loader and unloader station the shelf life is '-'
                if (p_Station->GetStationID().at(0) == 'U' || p_Station->GetStationID().at(0) == 'L') {
                    ShelfLife = STRING_MIUNS;
                }

                // get the vial shelf life, best before date, lot number
                FileData += ShelfLife + DELIMITER_STRING_COMMA +
                        ((p_Station->GetBestBeforeDateInString().compare("1900-01-01") == 0)
                         ? STRING_MIUNS : p_Station->GetBestBeforeDate().toString(FORMAT_DATE))
                        + DELIMITER_STRING_COMMA + (p_Station->GetLotNumber().compare(STRING_UNDEFINED) == 0 ?
                                                        STRING_MIUNS + DELIMITER_STRING_COMMA : p_Station->GetLotNumber()
                                                        + DELIMITER_STRING_COMMA);
                // get the exclusivenss
                FileData += (p_Station->IsStationExclusive() ? STRING_YES + STRING_NEWLINE : STRING_NO + STRING_NEWLINE);
                // write the data in the file
                // qprintable is the function which converts the string to byte array
                if ((const_cast<QFile&>(RMSFile)).write(qPrintable(FileData)) == -1) {
                    return false;
                }
            }
        }
    }
#endif
    return true;
}

/****************************************************************************/
QString ImportExportThreadController::CalculateShelfLife(const DataManager::CReagent* p_Reagent) {
    Q_UNUSED(p_Reagent)
    // This has to be done using the following formula:
    // the current time has to be added to (MaxTime - ActualTime)
#ifndef HIMALAYA_IMPORT_EXPORT
    QDateTime CurrentDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    QString ShelfLife = STRING_MIUNS;

    if (p_Reagent != NULL) {
        QDateTime ShelfDateTime = CurrentDateTime.addSecs(p_Reagent->GetMaxTimeInSeconds());
        //Get number of seconds since epoch (i.e. from 1970-01-01T00:00:00)
        uint CurrentTimeFromEpoch = CurrentDateTime.toTime_t();
        uint ShelTimeFromEpoch = ShelfDateTime.toTime_t();
        //Calculate time difference between current time and end time
        qint32 TimeDifference = ShelTimeFromEpoch - CurrentTimeFromEpoch;

        // check the time difference.
        if (TimeDifference <= 0) {
            ShelfLife = STRING_EXPIRED;
        }
        else  if (TimeDifference < TWENTY_FOUR_HOURS_IN_SECONDS) {
            ShelfLife = ShelfDateTime.date().toString(FORMAT_DATE) + STRING_SPACE + STRING_SPACE
                    + ShelfDateTime.time().toString(FORMAT_TIME);
        }
        else {
            ShelfLife = ShelfDateTime.date().toString(FORMAT_DATE);
        }
    }
    return ShelfLife;

#endif
     return QString("");
}

/****************************************************************************/
bool ImportExportThreadController::CopyConfigurationFiles(const DataManager::CConfigurationList &ConfigurationList,
                                                          QString TargetPath) {
    // remove the constant cast
    DataManager::CConfigurationList& Configuration = const_cast<DataManager::CConfigurationList&>(ConfigurationList);

    foreach (QString FileName, Configuration.GetFileList()) {
        // if the file is RMS_Status.csv file then dont copy, because this file will be doesnt exist in the source directory
        // and this file created in tempporary directory
        if (FileName.compare(FILENAME_RMSSTATUS) != 0) {
            if (mp_ExportConfiguration != NULL) {

                // check whether file exists in the "Settings" directory before start copying the files
                if (!QFile::exists(mp_ExportConfiguration->GetSourceDir() + QDir::separator() + FileName)) {
                    qDebug() << "File does not exist";
                    return false;
                }

                // file name can contain folder name also, create folder name
                if (FileName.contains(QDir::separator())) {
                    QString DirectoryName(FileName.mid(0, FileName.indexOf(QDir::separator())));
                    QDir Directory(TargetPath + QDir::separator() + DirectoryName);
                    // create directory if not exists
                    if (!Directory.exists()) {
                        (void)Directory.mkdir(Directory.absolutePath());
                    }
                }

                // copy the files and QDir::seperator uses generic symbol depending on the OS i.e. "/" for unix and for windows "\"
                if (!CheckTheExistenceAndCopyFile(TargetPath + QDir::separator() + FileName, mp_ExportConfiguration->GetSourceDir() +
                                                 QDir::separator() + FileName)) {
                    return false;
                }
            }
            else {
                return false;
            }
        }
    }

    // check the group flag, if it is true then start copying the files from the directory
    if (Configuration.GetGroupListFlag()) {
        // for log directory
        QDir LogDirectory;
        QString LogFiles;
        bool UserLogDirectory = false;


        if (Configuration.GetGroupFileName().compare(DIRECTORY_LOGFILES, Qt::CaseInsensitive) == 0) {
            (void)LogDirectory.setPath(Global::SystemPaths::Instance().GetLogfilesPath()); //to avoid lint-534
            LogFiles = FILTER_EXTENSION_ANY;

        }
        else {
            return true;
        }

        if (LogDirectory.exists()) {            
            // create one more directory to save the LogFiles
            if (QProcess::execute(COMMAND_MKDIR + TargetPath + QDir::separator()
                                  + Configuration.GetGroupFileName()) >= 0) {

                QStringList LogFilesList = LogDirectory.entryList(QStringList() << LogFiles, QDir::Files);
                if (!UserLogDirectory) {
                    if (m_EventLogFileName.compare("") != 0) {
                        // for event log files we need to copy only the specified number of files
                        QStringList EventFileList = LogDirectory.entryList
                                (QStringList() << m_EventLogFileName + FILTER_EXTENSION_LOG,
                                 QDir::Files, QDir::Name | QDir::Time);

                        for (int Counter = m_NoOfLogFiles; Counter < EventFileList.count(); Counter++) {
                            (void)LogFilesList.removeOne(EventFileList.value(Counter));
                        }
                    }
                }

                foreach (QString FileName, LogFilesList) {

                    // copy the files and QDir::seperator uses generic symbol depending on the OS i.e. "/" for unix and for windows "\"
                    if (!CheckTheExistenceAndCopyFile(TargetPath + QDir::separator() + Configuration.GetGroupFileName() +
                                                     QDir::separator() + FileName, LogDirectory.absolutePath() +
                                                     QDir::separator() + FileName)) {
                        return false;
                    }
                }
            }
            // if it is a DailyRunLog folder then delete the folder after copying the data
            if (UserLogDirectory) {
                QStringList Options;
                // -c option for shell and remove directory
                Options << COMMAND_ARGUMENT_C << COMMAND_RM + COMMAND_ARGUMENT_R + STRING_SPACE +
                           LogDirectory.absolutePath();
                // execute the process with "/bin/sh"
                // remove all the files from the "rm -rf DailyRun"
                (void)QProcess::execute(DIRECTORY_SH, Options);  //to avoid lint-534
            }
        }
        else {
            return false;
        }
    }
    return true;
}

/****************************************************************************/
bool ImportExportThreadController::CheckTheExistenceAndCopyFile(QString TargetFileName, QString SourceFileName) {
    // if some reason the Export directory is not deleted then files cannot be copied
    // so delete the files which are in Export directory
    if (QFile::exists(TargetFileName)) {
        (void)QFile::remove(TargetFileName); //to avoid lint-534
    }

    if (!QFile::copy(SourceFileName, TargetFileName)) {
        qDebug() << "Unable to copy the files";
        Global::EventObject::Instance().RaiseEvent(EVENT_EXPORT_NO_ENOUGH_SPACE_ON_SD_CARD, false);
        return false;
    }
    return true;
}

/****************************************************************************/
void ImportExportThreadController::DoCleanUpObjects() {
    /// don't delete the m_DataManager pointer, this is a reference object from the Main thread
    // destroy all the objects
    // destroy the station list container
    if (mp_StationList != NULL) {
        try {
            delete mp_StationList;
        }
        catch(...) {
            // nothing to do
        }
    }
    mp_StationList = NULL;

    // destroy reagent list container
    if (mp_ReagentList != NULL) {
        try {
            delete mp_ReagentList;
        }
        catch(...) {
            // nothing to do
        }
    }
    mp_ReagentList = NULL;

    // destroy export configuration container
    if (mp_ExportConfiguration != NULL) {
        try {
            delete mp_ExportConfiguration;
        }
        catch(...) {
            // nothing to do
        }
    }
    mp_ExportConfiguration = NULL;
}

/****************************************************************************/
void ImportExportThreadController::StartImportingFiles(const QStringList FileList) {

    QStringList ImportTypeList;
    QString TypeOfImport;
    bool IsImport = true;

    ImportTypeList.clear();

    // multiple files can be imported. At max three files can be imported.
    for (qint32 Counter = 0; Counter < FileList.count(); Counter++) {
        if (!ImportArchiveFiles(TypeOfImport, FileList.value(Counter))) {
            IsImport = false;
            break;
        }
        ImportTypeList.append(TypeOfImport);
    }

    if (IsImport && ImportTypeList.count() > 0) {
        // if everything goes well then update the files and take a backup of the files
        if (!ImportTypeList.contains(TYPEOFIMPORT_LANGUAGE)) {
            // before updating take a back-up of the configuration files
            QStringList fileList;
            fileList << FILENAME_PROGRAMS << FILENAME_REAGENTS << FILENAME_STATIONS
                     << FILENAME_USERSETTINGS;
            // check for the files updation error
            if (!CheckForFilesUpdateError()) {
                IsImport = false;
            }
            // update the Rollback folder with latest files after the Import is successful
            if (!UpdateFolderWithFiles(fileList, Global::SystemPaths::Instance().GetRollbackPath()
                                       + QDir::separator() + DIRECTORY_SETTINGS + QDir::separator(),
                                       Global::SystemPaths::Instance().GetSettingsPath()
                                       + QDir::separator())) {
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_UPDATE_ROLLBACK_FAILED, true);
                m_EventCode = EVENT_IMPORT_UPDATE_ROLLBACK_FAILED;
                m_EventRaised = true;
                IsImport = false;
            }
        }
        if (ImportTypeList.contains(TYPEOFIMPORT_LANGUAGE)) {
            // before updating take a back-up of the configuration files
            QStringList fileList;
            // store the file names in the list "Colorado_*.qm" , "ColoradoEventStrings_*.xml"
            fileList << FILENAME_QM << FILENAME_EVENTSTRING;
            // Update the rollback folder after the import is done
            if (!UpdateFolderWithFiles(fileList, Global::SystemPaths::Instance().GetRollbackPath()
                                       + QDir::separator() + DIRECTORY_TRANSLATIONS + QDir::separator(),
                                       Global::SystemPaths::Instance().GetTranslationsPath()
                                       + QDir::separator())) {
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_UPDATE_ROLLBACK_FAILED, true);
                m_EventCode = EVENT_IMPORT_UPDATE_ROLLBACK_FAILED;
                m_EventRaised = true;
                IsImport = false;
            }
        }
    }

    if (!IsImport  && ImportTypeList.count() > 0) {
        if (!ImportTypeList.contains(TYPEOFIMPORT_LANGUAGE)) {
            (void)UpdateSettingsWithRollbackFolder();
        }
        if (ImportTypeList.contains(TYPEOFIMPORT_LANGUAGE) && m_TranslationsFolderUpdatedFiles) {
            QStringList fileList;
            // store the file names in the list "Colorado_*.qm" , "ColoradoEventStrings_*.xml"
            fileList << FILENAME_QM << FILENAME_EVENTSTRING;
            m_CurrentLanguageUpdated = false;
            m_NewLanguageAdded = false;

            // copy all the files from rollback folder
            if (!UpdateFolderWithFiles(fileList, Global::SystemPaths::Instance().GetRollbackPath()
                                       + QDir::separator() + DIRECTORY_TRANSLATIONS + QDir::separator(),
                                       Global::SystemPaths::Instance().GetTranslationsPath()
                                       + QDir::separator())) {
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_UPDATE_ROLLBACK_FAILED, true);
                m_EventCode = EVENT_IMPORT_UPDATE_ROLLBACK_FAILED;
                m_EventRaised = true;
                IsImport = false;
            }
        }
    }

    // emit the thread finished flag
    emit ThreadFinished(IsImport, ImportTypeList, m_EventCode, m_CurrentLanguageUpdated, m_NewLanguageAdded);

}

/****************************************************************************/
bool ImportExportThreadController::ImportArchiveFiles(const QString &ImportType, QString FileName) {
    // remove the const cast
    QString& TypeOfImport = const_cast<QString&>(ImportType);
    // this is used for Import command
    bool IsImported = true;
    // we use different keys for Leica and Viewer (Viewer with value 1)
    QByteArray KeyBytes(ImportExport::Constants::KEYFILESIZE, 0);
    KeyBytes[2 * ImportExport::Constants::HASH_SIZE - 1] = 1;
    // create the RAM file
    ImportExport::RAMFile RFile;
    // to store the file list
    QStringList FileList;
    QDir Dir(Global::DIRECTORY_MNT_STORAGE + QDir::separator() + DIRECTORY_IMPORT);

    // check the file format - consider the first file
    if (FileName.split(DELIMITER_STRING_UNDERSCORE).count() > 4) {
        // check whether device name matches with the file name
        if (FileName.split("_").value(0).compare(m_DeviceName, Qt::CaseInsensitive) == 0) {
            // second value of the file name will be the type of Import
            TypeOfImport = FileName.split("_").value(1);
            // every type of Import will have SW_version.xml by default
            FileList << FILENAME_SWVERSION;

            IsImported = AddFilesForImportType(TypeOfImport, FileList);

            if (IsImported) {
                try {
                    // read the archive file - add try catch
                    ReadArchive(qPrintable(Dir.absoluteFilePath(FileName)), &RFile,
                                "Import", KeyBytes, FileList, Global::SystemPaths::Instance().GetTempPath()
                                + QDir::separator() + DIRECTORY_IMPORT + QDir::separator());
                }
                catch (...) {
                    m_EventCode = EVENT_IMPORT_TAMPERED_ARCHIVE_FILE;
                    IsImported = false;
                }
            }
        }
        else {
            // device name is not matching
            m_EventCode = EVENT_IMPORT_DEVICE_NAME_NOT_MATCHING;
            IsImported = false;
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_ARCHIVE_FILE_FORMAT_NOT_PROPER, true);
        m_EventCode = EVENT_IMPORT_ARCHIVE_FILE_FORMAT_NOT_PROPER;
        m_EventRaised = true;
        IsImported = false;
    }
    qDebug() << QDir::currentPath();

    if (IsImported) {
        // check the type of Import, for the language file we cannot predict how many files can be present
        // so we hard code the value at least two should be present one is SW_Version.xml and other one can
        // be either QM or event string XML file
        if (!WriteFilesAndImportData(TypeOfImport, FileList, RFile)) {
            IsImported = false;
        }
    }

    return IsImported;
}

/****************************************************************************/
bool ImportExportThreadController::AddFilesForImportType(const QString &TypeOfImport, const QStringList &ListOfFiles) {

    QStringList& FileList = const_cast<QStringList&>(ListOfFiles);
    // check the type of Import
    if (TypeOfImport.compare(TYPEOFIMPORT_SERVICE, Qt::CaseInsensitive) == 0 ||
            TypeOfImport.compare(TYPEOFIMPORT_USER, Qt::CaseInsensitive) == 0) {
        // for the type of import "user" or "Service"
        FileList << FILENAME_REAGENTS << FILENAME_REAGENT_GROUPS << FILENAME_REAGENT_GROUP_COLORS
                 << FILENAME_PROGRAMS << FILENAME_STATIONS << FILENAME_USERSETTINGS;
    }
    else if (TypeOfImport.compare(TYPEOFIMPORT_LEICA, Qt::CaseInsensitive) == 0) {
        // for the type of import "Leica"
        FileList << FILENAME_REAGENTS << FILENAME_REAGENT_GROUPS << FILENAME_REAGENT_GROUP_COLORS << FILENAME_USERSETTINGS;
    }
    else if (TypeOfImport.compare(TYPEOFIMPORT_LANGUAGE, Qt::CaseInsensitive) == 0) {
        // for the type of import "Language"
        FileList << FILENAME_QM << FILENAME_EVENTSTRING;
    }
    else {
        m_EventCode = EVENT_IMPORT_TYPEOFIMPORTNOTVALID;
        return false;
        //ErrorString = "Type of Import is not valid";
    }
    return true;
}


/****************************************************************************/
bool ImportExportThreadController::WriteFilesAndImportData(const QString &TypeOfImport, const QStringList &FileList,
                                                           const ImportExport::RAMFile &RamFile) {
    bool RequiredFilesImported = false;
    ImportExport::RAMFile RFile = const_cast<ImportExport::RAMFile&>(RamFile);
    // check the type of Import
    if (TypeOfImport.compare(TYPEOFIMPORT_LANGUAGE, Qt::CaseInsensitive) == 0) {
        if (RFile.getFiles().count() >= 2) {
            RequiredFilesImported = true;
        }
    }
    else {
        // for other import check for the file count
        if (RFile.getFiles().count() == FileList.count()) {
            RequiredFilesImported = true;
        }
    }
    // check file count
    if (RequiredFilesImported) {
        // create the temporary directory
        if (QProcess::execute(COMMAND_MKDIR + Global::SystemPaths::Instance().GetTempPath()
                              + QDir::separator() + DIRECTORY_IMPORT) >= 0) {
            // write all the files
            foreach(QString KeyName, RFile.getFiles().uniqueKeys()) {
                foreach (QByteArray FileData, RFile.getFiles().values(KeyName)) {
                    if (KeyName.contains(DIRECTORY_IMPORT)) {
                        // try to create the files
                        try {
                            // remove the full file path
                            ImportExport::FailSafeOpen FileWrite(KeyName, FILEMODE_WRITE);
                            FileWrite.write(FileData);
                            FileWrite.close();
                        }
                        catch (...) {
                            // got a exception
                            return false;
                        }
                    }
                }
            }
            // check the return code and creates the containers -- Short-circuit evaluation - check for present
            // software versions with imported software versions
            if (CompareSoftwareVersions()) {
                if (TypeOfImport.compare(TYPEOFIMPORT_LANGUAGE, Qt::CaseInsensitive) == 0) {
                    if (ImportLanguageFiles()) {
                        // if everything is success then return code is positive
                        return true;
                    }
                }
                else {
                    // create the containers and do verification
                    if ((CreateAndUpdateContainers(TypeOfImport, QString(Global::SystemPaths::Instance().GetTempPath()
                                                                          + QDir::separator() + DIRECTORY_IMPORT)))) {
                        // if everything is success then return code is positive
                        return true;
                    }
                }
            }
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_REQUIRED_FILES_NOT_AVAILABLE, true);
        m_EventCode = EVENT_IMPORT_REQUIRED_FILES_NOT_AVAILABLE;
        m_EventRaised = true;
    }
    return false;
}

/****************************************************************************/
bool ImportExportThreadController::CreateAndUpdateContainers(const QString TypeOfImport,
                                                             const QString FilePath) {
    // check the type of Import
    if ((TypeOfImport.compare(TYPEOFIMPORT_SERVICE, Qt::CaseInsensitive) == 0) ||
            (TypeOfImport.compare(TYPEOFIMPORT_USER, Qt::CaseInsensitive) == 0) ||
            (TypeOfImport.compare(TYPEOFIMPORT_LEICA, Qt::CaseInsensitive) == 0)) {
        // initialize the Reagents container
        DataManager::CDataReagentList ImportReagentList;
        DataManager::CDataReagentListVerifier ImportReagentListVerifier;
        // check whether container initialized with all the data properly or not
        if (!InitializeAndVerifyContainer(ImportReagentList, ImportReagentListVerifier, FilePath + QDir::separator() +
                                 FILENAME_REAGENTS)) {            
            qDebug() << "Initialization failed - Reagents";
            return false;
        }

        //check Regent Group container
        DataManager::CDataReagentGroupList ImportReagentGroupList;
        DataManager::CDataReagentListVerifier ImportReagentGroupListVerifier;
        if (!InitializeAndVerifyContainer(ImportReagentGroupList, ImportReagentGroupListVerifier, FilePath + QDir::separator() +
                                 FILENAME_REAGENT_GROUPS)) {
            qDebug() << "Initialization failed - Reagent Groups";
            return false;
        }

        //check Regent Group color container
        DataManager::CReagentGroupColorList ImportReagentGroupColorList;
        ImportReagentGroupColorList.SetDataVerificationMode(false);
        (void)ImportReagentGroupColorList.Read(FilePath + QDir::separator() + FILENAME_REAGENT_GROUP_COLORS);

        //check Station container
        DataManager::CDashboardDataStationList ImportDashboardDataStationList;
        ImportDashboardDataStationList.SetDataVerificationMode(false);
        (void)ImportDashboardDataStationList.Read(FilePath + QDir::separator() + FILENAME_STATIONS);

        // initialize the Programs container
        DataManager::CDataProgramList ImportProgramList;
        DataManager::CDataProgramListVerifier ImportProgramListVerifier;
        // check whether container initialized with all the data properly or not
        if (!InitializeAndVerifyContainer(ImportProgramList, ImportProgramListVerifier, FilePath + QDir::separator() +
                                 FILENAME_PROGRAMS)) {
            qDebug() << "Initialization failed - Programs";
            return false;
        }

        // initialize the User settings container
        DataManager::CUserSettingsInterface ImportUSInterface;
        DataManager::CUserSettingsVerifier ImportUSInterfaceVerifier;
        // check whether container initialized with all the data properly or not
        if (!InitializeAndVerifyContainer(ImportUSInterface, ImportUSInterfaceVerifier, FilePath + QDir::separator() +
                                 FILENAME_USERSETTINGS)) {
            qDebug() << "Initialization failed - User settings";
            return false;
        }


        // before updating take a back-up of the configuration files
        QStringList FileList;
        (void)AddFilesForImportType(TypeOfImport,FileList);

        // save the data containers before copying to "Rollback" folder
        if (!WriteFilesInSettingsFolder()) {
            return false;
        }
        // updated the rollback folder so that before overwriting we can have a backup
        if (!UpdateFolderWithFiles(FileList, Global::SystemPaths::Instance().GetRollbackPath()
                                   + QDir::separator() + DIRECTORY_SETTINGS + QDir::separator(),
                                   Global::SystemPaths::Instance().GetSettingsPath()
                                   + QDir::separator())) {
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_UPDATE_ROLLBACK_FAILED);
            return false;
        }
        /// \todo need to write code for the special verfiers
        // for the type of Leica import Program sequence is not required

        *(m_DataManager.GetReagentList()) = ImportReagentList;
        *(m_DataManager.GetReagentGroupList()) = ImportReagentGroupList;
        *(m_DataManager.GetReagentGroupColorList()) = ImportReagentGroupColorList;
        *(m_DataManager.GetStationList()) = ImportDashboardDataStationList;
        *(m_DataManager.GetProgramList()) = ImportProgramList;
        *(m_DataManager.GetUserSettingsInterface()->GetUserSettings()) = *(ImportUSInterface.GetUserSettings());
        // update the user settings in the Main container
//        if (!m_DataManager.GetUserSettingsInterface()->UpdateUserSettings(&UserSettings)) {
//            return false;
//        }

        // check for the files updation error
        if (!CheckForFilesUpdateError()) {
            return false;
        }
        // update the Rollback folder with latest files after the Import is successful
        if (!UpdateFolderWithFiles(FileList, Global::SystemPaths::Instance().GetRollbackPath()
                                   + QDir::separator() + DIRECTORY_SETTINGS + QDir::separator(),
                                   Global::SystemPaths::Instance().GetSettingsPath()
                                   + QDir::separator())) {
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_UPDATE_ROLLBACK_FAILED);
            return false;
        }
    }
    else {
        // unknown Import is found
    }
    return true;
}



/****************************************************************************/
bool ImportExportThreadController::CheckForFilesUpdateError() {
    bool Rollback = false;
    // short circuit evaluation - evaluates the first one if false then exits from the if statement
    if (!(m_DataManager.GetReagentList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                                             + QDir::separator() + FILENAME_REAGENTS))) {
        Rollback = true;
    }

    //
    if (!(m_DataManager.GetReagentGroupList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                                             + QDir::separator() + FILENAME_REAGENT_GROUPS))) {
        Rollback = true;
    }

    //
    if (!(m_DataManager.GetReagentGroupColorList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                                             + QDir::separator() + FILENAME_REAGENT_GROUP_COLORS))) {
        Rollback = true;
    }

    // short circuit evaluation - evaluates the first one if false then exits from the if statement
    if (!Rollback && !(m_DataManager.GetProgramList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                                             + QDir::separator() + FILENAME_PROGRAMS))) {
        Rollback = true;
    }

    // short circuit evaluation - evaluates the first one if false then exits from the if statement
    if (!Rollback && !(m_DataManager.GetStationList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                                             + QDir::separator() + FILENAME_STATIONS))) {
        Rollback = true;
    }
    // short circuit evaluation - evaluates the first one if false then exits from the if statement
    if (!Rollback && !(m_DataManager.GetUserSettingsInterface()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                                                       + QDir::separator() + FILENAME_USERSETTINGS))) {
        Rollback = true;
    }

    // if rollback is true then put back all original files and exit
    if (Rollback) {
        if (!UpdateSettingsWithRollbackFolder()) {
            return false;
        }
    }

    return true;
}

/****************************************************************************/
bool ImportExportThreadController::UpdateSettingsWithRollbackFolder() {

    QStringList FileList;

    (void)AddFilesForImportType(QString("Service"), FileList);

    // update the settings folder with the rollback folder files
    if (!UpdateFolderWithFiles(FileList, Global::SystemPaths::Instance().GetSettingsPath() + QDir::separator(),
                               Global::SystemPaths::Instance().GetRollbackPath() + QDir::separator()
                               + DIRECTORY_SETTINGS + QDir::separator())) {
        Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_ROLLBACK_FAILED, true);
        m_EventCode = EVENT_IMPORT_ROLLBACK_FAILED;
        m_EventRaised = true;
        return false;
    }

    // save the data containers before copying to "Rollback" folder
    if (!m_DataManager.GetReagentList()->Read(Global::SystemPaths::Instance().GetSettingsPath()
                                              + QDir::separator() + FILENAME_REAGENTS)) {
        /// this never happens, but if the xml is having some problem
        return false;
    }
    if (!m_DataManager.GetProgramList()->Read(Global::SystemPaths::Instance().GetSettingsPath()
                                              + QDir::separator() + FILENAME_PROGRAMS)) {
        /// this never happens, but if the xml is having some problem
        return false;
    }
/*
    if (!m_DataManager.GetProgramSequenceList()->Read(Global::SystemPaths::Instance().GetSettingsPath()
                                                      + QDir::separator() + FILENAME_PROGRAMSEQUENCE)) {
        /// this never happens, but if the xml is having some problem
        return false;
    }
*/
    if (!m_DataManager.GetStationList()->Read(Global::SystemPaths::Instance().GetSettingsPath()
                                              + QDir::separator() + FILENAME_STATIONS)) {
        /// this never happens, but if the xml is having some problem
        return false;
    }
    if (!m_DataManager.GetUserSettingsInterface()->Read(Global::SystemPaths::Instance().GetSettingsPath()
                                                        + QDir::separator() + FILENAME_USERSETTINGS)) {
        /// this never happens, but if the xml is having some problem
        return false;
    }

    return true;
}

/****************************************************************************/
bool ImportExportThreadController::CompareSoftwareVersions() {

    // initialize the SW version container
    DataManager::CSWVersionList ImportSWVersionList;
    DataManager::CSWVersionListVerifier ImportSWVLVerifier;
    // check whether container initialized with all the data properly or not
    if (!InitializeAndVerifyContainer(ImportSWVersionList, ImportSWVLVerifier, Global::SystemPaths::Instance().GetTempPath()
                             + QDir::separator() + DIRECTORY_IMPORT + QDir::separator() + FILENAME_SWVERSION)) {
        qDebug() << "Initialization failed from the import folder - SW version";
        return false;
    }
    // now read the container from the settings folder
    // initialize the SW version container
    DataManager::CSWVersionList SettingsSWVersionList;
    DataManager::CSWVersionListVerifier SettingsSWVLVerifier;
    // check whether container initialized with all the data properly or not
    if (!InitializeAndVerifyContainer(SettingsSWVersionList, SettingsSWVLVerifier, Global::SystemPaths::Instance().GetSettingsPath()
                             + QDir::separator() + FILENAME_SWVERSION)) {
        qDebug() << "Initialization failed from the settings folder - SW version";
        return false;
    }
    // iterate each item and check the SW version numbers
    for (int SWDetailsCount = 0; SWDetailsCount < SettingsSWVersionList.GetNumberOfSWDetails(); SWDetailsCount++) {
        DataManager::CSWDetails SettingsSWDetails;
        if (SettingsSWVersionList.GetSWDetails(SWDetailsCount, SettingsSWDetails)) {
            if (SettingsSWDetails.GetSWType() == MASTERSOFTWARE) {
                // check for the SW name exist in the imported files
                DataManager::CSWDetails* ImportSWDetails = const_cast<DataManager::CSWDetails*>
                        (ImportSWVersionList.GetSWDetails(SettingsSWDetails.GetSWName()));
                if (ImportSWDetails != NULL) {
                    if (ImportSWDetails->GetSWVersionNumber() != SettingsSWDetails.GetSWVersionNumber()) {
                        Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_SWVERSION_NOT_MATCHING, true);
                        m_EventCode = EVENT_IMPORT_SWVERSION_NOT_MATCHING;
                        m_EventRaised = true;
                        qDebug() << "SW version is not matching with the Imported file version number :: "
                                 << ImportSWDetails->GetSWVersionNumber();
                        return false;
                    }
                }
                else {
                    Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_SWVERSION_NOT_MATCHING, true);
                    m_EventCode = EVENT_IMPORT_SWVERSION_NOT_MATCHING;
                    m_EventRaised = true;
                    qDebug() << "SW name does not exist in the SW version list" << SettingsSWDetails.GetSWName();
                    return false;
                }
            }
        }
    }
    // on successful return true
    return true;
}



/****************************************************************************/
bool ImportExportThreadController::ImportLeicaPrograms(const DataManager::CDataProgramList &ProgramList) {

    Q_UNUSED(ProgramList)
#ifndef HIMALAYA_IMPORT_EXPORT

    // remove the constant cast
    DataManager::CDataProgramList& ImportProgramList = const_cast<DataManager::CDataProgramList&>(ProgramList);
    // get the Leica Programs
    DataManager::CDataProgramList* p_DataProgramList = m_DataManager.GetProgramList();

    if (p_DataProgramList != NULL) {
        // check for the program ID
        for (int ProgramCount = 0; ProgramCount < ImportProgramList.GetNumberOfPrograms();
             ProgramCount++) {
            bool ExecuteLoop = false;
            DataManager::CProgram ImportProgram;

            if (ImportProgramList.GetProgram((unsigned int)ProgramCount, ImportProgram)) {
                DataManager::CProgram* p_Program = const_cast<DataManager::CProgram*>
                        (p_DataProgramList->GetProgram(ImportProgram.GetID()));

                // if the program exist then update the program in the container
                if (p_Program != NULL) {
                    // check whether program is updated in the container or not
                    if (p_DataProgramList->UpdateProgram(&ImportProgram)) {
                        ExecuteLoop = true;
                    }
                }
                else {

                    // add new program in the list - but the program shall be inserted after the Leica programs
                    if (p_DataProgramList->AddLeicaProgram(&ImportProgram)) {
                        DataManager::CProgramSequenceList* p_ProgramSeqList = m_DataManager.GetProgramSequenceList();

                        if (p_ProgramSeqList != NULL) {
                            // create the program sequence step
                            DataManager::CProgramSequence ProgramSequence;
                            // set the program id
                            ProgramSequence.SetID(ImportProgram.GetID());
                            // check whether the Program is added to the list
                            if (p_ProgramSeqList->AddLeicaProgramSequenceStep(&ProgramSequence)) {
                                ExecuteLoop = true;
                            }
                        }                        
                    }                    
                }
            }
            // check whether loop is required to exit
            if (!ExecuteLoop) {
                return false;
            }
        }
    }
    else {
        return false;
    }
#endif
    return true;
}

/****************************************************************************/
bool ImportExportThreadController::ImportLeicaUserReagents(const DataManager::CDataReagentList &ReagentList) {

    Q_UNUSED(ReagentList)
#ifndef HIMALAYA_IMPORT_EXPORT
    // remove the constant cast
    DataManager::CDataReagentList& ImportReagentList = const_cast<DataManager::CDataReagentList&>(ReagentList);
    // get the Leica and User Reagents
    DataManager::CDataReagentList* p_DataReagentList = m_DataManager.GetReagentList();

    if (p_DataReagentList != NULL) {
        // check for the Reagent ID
        for (int ReagentCount = 0; ReagentCount < ImportReagentList.GetNumberOfReagents(); ReagentCount++) {
            DataManager::CReagent ImportReagent;

            if (ImportReagentList.GetReagent((unsigned int)ReagentCount, ImportReagent)) {
                if (ImportReagent.GetReagentType() == LEICA_REAGENT || ImportReagent.GetReagentType() == USER_REAGENT) {
                    // used for updating or adding the reagents
                    bool DoFurther = true;

                    if (ImportReagent.GetReagentType() == USER_REAGENT) {
                        // check if the user Leica reagent ID value more than 99 then don't update the reagents
                        if (ImportReagent.GetID().mid(1).toInt() > 100) {
                            DoFurther = false;
                        }
                    }

                    // do further steps if the above condition satisfies
                    if (DoFurther) {
                        DataManager::CReagent* p_Reagent = const_cast<DataManager::CReagent*>
                                (p_DataReagentList->GetReagent(ImportReagent.GetID()));

                        // if the Reagent exist then update the Reagent in the container
                        if (p_Reagent != NULL) {
                            // update the Max slides and max time only for the User Reagents
                            if (ImportReagent.GetReagentType() == USER_REAGENT) {
                                p_Reagent->SetMaxSlides(ImportReagent.GetMaxSlides());
                                p_Reagent->SetMaxTime(ImportReagent.GetMaxTime());
                            }
                            p_Reagent->SetShortName(ImportReagent.GetShortName());
                            p_Reagent->SetLongName(ImportReagent.GetLongName());
                            // check whether Reagent is updated in the container or not
                            if (!p_DataReagentList->UpdateReagent(p_Reagent)) {
                                return false;

                            }
                        }
                        else {
                            // add new reagent in the list
                            if (!p_DataReagentList->AddReagent(&ImportReagent)) {
                                return false;
                            }
                        }
                    }
                }
            }
            else {
                return false;
            }
        }
    }
    else {
        return false;
    }
#endif
    return true;
}


/****************************************************************************/
bool ImportExportThreadController::MountDevice(bool IsImport, QString FileExtensionName) {
    QString FileName("");
    if (IsImport) {
        FileName = m_DeviceName + DELIMITER_STRING_UNDERSCORE + FileExtensionName;
    }

    // check for the file existence in the mounted device.
    qint32 MountedValue = Global::MountStorageDevice(FileName);

    switch (MountedValue) {
        default:
            m_EventRaised = true;
            if (m_CommandName.contains(COMMAND_NAME_IMPORT)) {
                // log the event code
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORTEXPORT_IMPORT_NO_USB);
                m_EventCode = EVENT_IMPORTEXPORT_IMPORT_NO_USB;
            }
            else {
                // log the event code
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORTEXPORT_EXPORT_NO_USB);
                m_EventCode = EVENT_IMPORTEXPORT_EXPORT_NO_USB;
            }
            break;

        case 3:
            if (IsImport) {
                m_EventRaised = true;
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_NO_FILES_TO_IMPORT);
                m_EventCode = EVENT_IMPORT_NO_FILES_TO_IMPORT;
            }
            break;

        case 0:
            return true;
    }
    return false;
}



/****************************************************************************/
bool ImportExportThreadController::UpdateFolderWithFiles(QStringList FileList, QString TargetPath,
                                                         QString SourcePath) {
    // copy all the files to a rollback folder
    foreach (QString FileName, FileList) {
        // search in the file name  "*."
        if (FileName.contains(WILDCHAR_ASTRIK + QString(CHAR_DOT))) {
            QDir SourceDirectory(SourcePath);
            foreach (QString WildCharFileName, SourceDirectory.entryList(QStringList() << FileName, QDir::Files)) {
                // remove the file then try to write it
                (void)QFile::remove(TargetPath + WildCharFileName); //to avoid lint-534
                // not able to take backup of the files
                if (!QFile::copy(SourcePath + WildCharFileName,
                                 TargetPath + WildCharFileName)) {
                    /// this never happens - If it happens something wrong with file writing in the flash
                    //ErrorString = "Something wrong with the USB flash";
                    return false;
                }
            }
        }
        else {
            // remove the file then try to write it
            (void)QFile::remove(TargetPath + FileName); //to avoid lint-534
            // not able to take backup of the files
            if (!QFile::copy(SourcePath + FileName,
                             TargetPath + FileName)) {
                /// this never happens - If it happens something wrong with file writing in the flash
                //ErrorString = "Something wrong with the USB flash";
                return false;
            }
        }
    }
    return true;
}

/****************************************************************************/
bool ImportExportThreadController::WriteFilesInSettingsFolder() {

    if (!m_DataManager.GetReagentList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                               + QDir::separator() + FILENAME_REAGENTS)) {
        /// this may happen if the flash does not have more space or flash is having some problem
        //ErrorString = "Unable to write the file in Settings folder";
        return false;
    }
    if (!m_DataManager.GetProgramList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                               + QDir::separator() + FILENAME_PROGRAMS)) {
        /// this may happen if the flash does not have more space or flash is having some problem
        //ErrorString = "Unable to write the file in Settings folder";
        return false;
    }
/*
    if (!m_DataManager.GetProgramSequenceList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                                       + QDir::separator() + FILENAME_PROGRAMSEQUENCE)) {
        /// this may happen if the flash does not have more space or flash is having some problem
        //ErrorString = "Unable to write the file in Settings folder";
        return false;
    }
*/
    if (!m_DataManager.GetStationList()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                               + QDir::separator() + FILENAME_STATIONS)) {
        /// this may happen if the flash does not have more space or flash is having some problem
        //ErrorString = "Unable to write the file in Settings folder";
        return false;
    }
    if (!m_DataManager.GetUserSettingsInterface()->Write(Global::SystemPaths::Instance().GetSettingsPath()
                                                         + QDir::separator() + FILENAME_USERSETTINGS)) {
        /// this may happen if the flash does not have more space or flash is having some problem
        //ErrorString = "Unable to write the file in Settings folder";
        return false;
    }
    return true;
}

/****************************************************************************/
bool ImportExportThreadController::ImportLanguageFiles() {
    // take a copy of the files
    QDir ImportDirectory(Global::SystemPaths::Instance().GetTempPath() + QDir::separator() + DIRECTORY_IMPORT);

    if (ImportDirectory.entryList(QStringList() << FILENAME_QM).count() > 0
            || ImportDirectory.entryList(QStringList() <<  FILENAME_EVENTSTRING).count() > 0) {

        QStringList FileList;
        // store the file names in the list "Himalaya_*.qm" , "HimalayaEventStrings_*.xml"
        FileList << FILENAME_QM << FILENAME_EVENTSTRING;

        // delete all the files from translations directory "Translations/*"
        if (QProcess::execute(COMMAND_RM + Global::SystemPaths::Instance().GetRollbackPath()
                              + QDir::separator() + DIRECTORY_TRANSLATIONS + QDir::separator() + WILDCHAR_ASTRIK) < 0) {
            /// never goes here log the data if required
        }
        // Update the rollback folder "Rollback/Translations/*"
        if (!UpdateFolderWithFiles(FileList, Global::SystemPaths::Instance().GetRollbackPath()
                                   + QDir::separator() + DIRECTORY_TRANSLATIONS + QDir::separator(),
                                   Global::SystemPaths::Instance().GetTranslationsPath()
                                   + QDir::separator())) {
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_UPDATE_ROLLBACK_FAILED, true);
            m_EventCode = EVENT_IMPORT_UPDATE_ROLLBACK_FAILED;
            m_EventRaised = true;
            return false;
        }

        bool Rollback = false;
        // copy the QM files
        CopyQMFiles(ImportDirectory, Rollback);
        // check the rollback is requires or not
        if (!Rollback) {
            // copy the event string files
            CopyEventStringFiles(ImportDirectory, Rollback);
        }
        // check the rollback flag
        if (Rollback) {
            // if the rollback is going to happen then reset the flags
            m_NewLanguageAdded = false;
            m_CurrentLanguageUpdated = false;
            // delete all the files from translations directory
            if (QProcess::execute(COMMAND_RM + Global::SystemPaths::Instance().GetRollbackPath()
                                  + QDir::separator() + DIRECTORY_TRANSLATIONS + QDir::separator() + WILDCHAR_ASTRIK) < 0) {
                /// never goes here log the data if required
            }
            // Update the translations folder from rollback folder
            if (!UpdateFolderWithFiles(FileList, Global::SystemPaths::Instance().GetTranslationsPath()
                                       + QDir::separator(), Global::SystemPaths::Instance().GetRollbackPath()
                                       + QDir::separator() + DIRECTORY_TRANSLATIONS + QDir::separator())) {
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_ROLLBACK_FAILED, true);
                m_EventCode = EVENT_IMPORT_ROLLBACK_FAILED;
                m_EventRaised = true;
                return false;
            }
            return false;
        }
        m_TranslationsFolderUpdatedFiles = true;
        return true;
    }
    else {
        return false;
    }
}

/****************************************************************************/
void ImportExportThreadController::UnMountDevice() {
    Global::UnMountStorageDevice();
}

/****************************************************************************/
void ImportExportThreadController::CopyQMFiles(const QDir &Directory, const bool &Rollback) {
    // remove the const cast
    bool& IsRollback = const_cast<bool&>(Rollback);
    // check the user settings and the language
    QLocale::Language CurrentLanguage = QLocale::English;
    DataManager::CUserSettingsInterface* p_UserSettingsInterface = m_DataManager.GetUserSettingsInterface();
    if (p_UserSettingsInterface != NULL) {
        if (p_UserSettingsInterface->GetUserSettings(false) != NULL) {
            // get the current language
            CurrentLanguage = p_UserSettingsInterface->GetUserSettings(false)->GetLanguage();
        }
    }
    // "Himalaya_*.qm"
    foreach (QString FileName, Directory.entryList(QStringList() <<  FILENAME_QM)) {
        // Store the language name
        QString LangNameInFile(FileName);
        // remove the extension "Himalaya_de"
        LangNameInFile.truncate(LangNameInFile.lastIndexOf(CHAR_DOT));
        // only take the language name "de"
        (void)LangNameInFile.remove(0, LangNameInFile.indexOf(CHAR_UNDERSCORE) + 1); //to avoid lint-534

        // check the language updated
        if (!m_CurrentLanguageUpdated) {
            // check the current language file is updated
            if (CurrentLanguage == QLocale(LangNameInFile).language()) {
                m_CurrentLanguageUpdated = true;
            }
        }

        // check the file existence
        if (QFile::exists(Global::SystemPaths::Instance().GetTranslationsPath() + QDir::separator() + FileName)) {
            // remove the file before copying it
            if (!QFile::remove(Global::SystemPaths::Instance().GetTranslationsPath() + QDir::separator() + FileName)) {
                IsRollback = true;
                break;
            }
            // copy the files
            if (!QFile::copy(Directory.absolutePath() + QDir::separator() + FileName,
                             Global::SystemPaths::Instance().GetTranslationsPath() + QDir::separator() + FileName)) {
                IsRollback = true;
                break;
            }
        }
        else {
            // "HimalayaEventStrings_de.xml"
            QString EventFileName(FILENAME_COLEVENTSTRINGS + LangNameInFile + FILETYPE_XML);
            // check whether event string XML file exists or not
            if (!QFile::exists(Directory.absolutePath() + QDir::separator() + EventFileName)) {
                // copy the QM file
                if (!QFile::copy(Directory.absolutePath() + QDir::separator() + FileName,
                                 Global::SystemPaths::Instance().GetTranslationsPath() + QDir::separator() + FileName)) {
                    IsRollback = true;
                    break;
                }
                // copy the event file
                if (!QFile::copy(Directory.absolutePath() + QDir::separator() + EventFileName,
                                 Global::SystemPaths::Instance().GetTranslationsPath() + QDir::separator() + EventFileName)) {
                    IsRollback = true;
                    break;
                }
                // new language is addded
                m_NewLanguageAdded = true;
            }
            else {
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_NEW_EVENTSTRING_FILE_MISSING);
            }
        }
    }
}

/****************************************************************************/
void ImportExportThreadController::CopyEventStringFiles(const QDir &Directory, const bool &Rollback) {
    // remove the const cast
    bool& IsRollback = const_cast<bool&>(Rollback);
    // "HimalayaEventStrings_*.xml"
    foreach (QString FileName, Directory.entryList(QStringList() << FILENAME_EVENTSTRING)) {
        // check the file existence
        if (QFile::exists(Global::SystemPaths::Instance().GetTranslationsPath() + QDir::separator() + FileName)) {
            // remove the file before copying it
            if (!QFile::remove(Global::SystemPaths::Instance().GetTranslationsPath() + QDir::separator() + FileName)) {
                IsRollback = true;
                break;
            }
            // copy the files
            if (!QFile::copy(Directory.absolutePath() + QDir::separator() + FileName,
                             Global::SystemPaths::Instance().GetTranslationsPath() + QDir::separator() + FileName)) {
                IsRollback = true;
                break;
            }
        }
    }
}

}
