/************************************************************************************************/

/*!\file Include/HimalayaEventCodes.h

* \brief The file is Created by script from EventConfig.csv, Do not modified manually!

*/

/************************************************************************************************/

#ifndef HIMALAYAEVENTCODES_H

#define HIMALAYAEVENTCODES_H

const quint32 EVENT_DATE_TIME_CANNOT_BE_MORE_THAN_24_HOURS	= 33619990;	///< Can not change the date and time more than 24 hours of the current system date time is %1.


const quint32 EVENT_DM_GV_BASE_CONTAINER_IS_NOT_INITIALIZED	= 33817094;	///< Group verification failed. Base container is not initialized.


const quint32 EVENT_DM_GV_INVALID_CONTAINER_TYPE	= 33817095;	///< Group verification failed. Container type is not valid.

const quint32 EVENT_DM_GV_PROGRAM_IS_NOT_INITIALIZED	= 33817097;	///< Group verification failed. Program container is not initialized.

const quint32 EVENT_DM_GV_REAGENTID_EXIST_IN_PROG_NOT_IN_REAGENT_LIST	= 33817088;	///< Reagent ID exists in Program list but not in Reagent list: %1

const quint32 EVENT_DM_GV_REAGENTID_EXIST_IN_SETTINGS_NOT_IN_REAGENT_LIST	= 33817092;	///< Reagent ID exist in User settings but not in Reagent list: %1

const quint32 EVENT_DM_GV_REAGENT_IS_NOT_INITIALIZED	= 33817096;	///< Group verification failed. Reagent container is not initialized.

const quint32 EVENT_DM_GV_STATIONID_EXIST_IN_STATION_NOT_IN_STATIONMATRIX	= 33817093;	///< Station ID exist in Station list but not in Station matrix : %1

const quint32 EVENT_DM_GV_STATION_IS_NOT_INITIALIZED	= 33817098;	///< Group verification failed. Station container is not initialized.

const quint32 EVENT_DM_GV_USINTERFACE_IS_NOT_INITIALIZED	= 33817101;	///< Group verification failed. User settings container is not initialized.

const quint32 EVENT_DM_INCOMPATIBLE_STEP_REAGENT_GROUP	= 33818114;	///< Program '%1' Reagents  for Step %2 and Step %3 are not compatible.

const quint32 EVENT_DM_INFO_SOUND_LEVEL_OUT_OF_RANGE	= 33817861;	///<        ///<  Definition/Declaration of variable EVENT_DM_INFO_SOUND_LEVEL_OUT_OF_RANGE

const quint32 EVENT_DM_INFO_SOUND_NUMBER_OUT_OF_RANGE	= 33817860;	///<        ///<  Definition/Declaration of variable EVENT_DM_INFO_SOUND_NUMBER_OUT_OF_RANGE

const quint32 EVENT_DM_INFO_SOUND_PERIODIC_TIME_OUT_OF_RANGE	= 33817859;	///<        ///<  Sound periodic parameter is out of range

const quint32 EVENT_DM_INVALID_PROGRAM_ID	= 33816577;	///< %1' is not a valid Program ID.

const quint32 EVENT_SERVICE_INFO_NOT_FOUND	= 33816578;	///<  //!< Error in retrieving Service information


const quint32 EVENT_DM_INVALID_STEP_ID	= 33816608;	///< Step ID %2 in Program %1 is invalid

const quint32 EVENT_DM_PARAFFIN_TEMP_OUT_OF_RANGE	= 33817862;	///<        ///<  Definition/Declaration of variable EVENT_DM_PARAFFIN_TEMP_OUT_OF_RANGE

const quint32 EVENT_DM_PROG_ADD_FAILED_INTERNAL_ERR	= 33816626;	///< Internal Software Error. Adding rogram failed. Please contact service.

const quint32 EVENT_DM_PROG_COUNT_EXCEEDS_LIMIT	= 33816597;	///< Program list contains more than %1 programs. Only %1 programs allowed

const quint32 EVENT_DM_PROG_DELETE_FAILED_ID_NOT_FOUND	= 33816624;	///< Delete program failed due to invalid Program ID %1.

const quint32 EVENT_DM_PROG_DEL_FAILED_INTERNAL_ERR	= 33816631;	///< Internal Software Error. Program delete failed. Please contact service.

const quint32 EVENT_DM_PROG_ID_NOT_UNIQUE	= 33816623;	///< Program ID %1 associated with Program %2 is not unique .

const quint32 EVENT_DM_PROG_INVALID_DURATION_FORMAT	= 33816620;	///< Program step(s) in Program %1 contains invalid duration format

const quint32 EVENT_DM_PROG_NAME_LENGTH_CHECK_FAILED	= 33818117;	///< Length of the Program name exceeds limit for the program '%1'. Valid Range '%2' - '%3' characters.

const quint32 EVENT_DM_PROG_NAME_NOT_UNIQUE	= 33818113;	///< Program name '%1' already in use. Please choose different Program name.

const quint32 EVENT_DM_PROGRAM_INVALID_INDEX	= 33816596;	///< Invalid Index in program hash table.

const quint32 EVENT_DM_PROGRAM_VERIFICATION_FAILED	= 33816592;	///< Verifying Programs.xml failed.

const quint32 EVENT_DM_PROGRAM_XML_READ_FAILED	= 33816584;	///< Reading Programs.xml failed.

const quint32 EVENT_DM_PROG_STEP_COUNT_EXCEEDS_LIMIT	= 33816605;	///< Number of steps in Program %1 exceeds limit. Max 50 steps allowed.

const quint32 EVENT_DM_PROG_STEP_COUNT_LESS_LIMIT	= 33818116;	///< Program '%1' must have '%2' step(s) at least.

const quint32 EVENT_DM_PROG_STEP_DURATION_EXCEED_LIMIT	= 33818118;	///< Duration in Step '%1' of Program '%2' exceeds limits, validated range : '%3' - '%4'.

const quint32 EVENT_DM_PROG_STEP_TEMP_EXCEED_LIMIT	= 33818119;	///< Temperature in Step '%1' of Program '%2' exceeds limits, validated range : '%3' - '%4' (Celsius).

const quint32 EVENT_DM_PROG_VERFIER_INVALID_REAGENT_ID	= 33816606;	///< Program Verifier found invalid Reagent ID %2 in the program %1.


const quint32 EVENT_DM_REAGENT_LIST_READ_FAILED	= 33816846;	///< ReagentList reading failed '%1' reagents

const quint32 EVENT_DM_REAGENT_LIST_WRITE_FAILED	= 33816845;	///< ReagentList writing failed '%1' reagents

const quint32 EVENT_DM_REAGENT_NAME_LENGTH_CHECK_FAILED	= 33816840;	///< Reagent '%1' Name is too long (max 32 Characters)


const quint32 EVENT_DM_RMS_CLEANING_OUT_OF_RANGE	= 33817864;	///<        ///<  Definition/Declaration of variable EVENT_DM_RMS_CLEANING_OUT_OF_RANGE

const quint32 EVENT_DM_RMS_PROCESSING_OUT_OF_RANGE	= 33817863;	///<        ///<  Definition/Declaration of variable EVENT_DM_RMS_PROCESSING_OUT_OF_RANGE


const quint32 EVENT_DM_USER_PROGRAM_NAME_CONTAINS_LEICA	= 33818115;	///< User Program name can not contain 'leica' word.

const quint32 EVENT_EXPORT_NO_ENOUGH_SPACE_ON_SD_CARD	= 50331649;	///< Export failed. No enough space on SD card.

const quint32 EVENT_EXPORT_NO_ENOUGH_SPACE_ON_USB_CARD	= 50331672;	///< Export failed. No enough space on USB storage.

const quint32 EVENT_GLOBAL_ERROR_FALLBACK_LANGUAGE	= 16842764;	///< Could not set fallback language to '%1'.

const quint32 EVENT_GLOBAL_ERROR_FILE_REMOVE	= 16842767;	///< Error deleting file %1.

const quint32 EVENT_GLOBAL_ERROR_FILE_SEEK	= 16842771;	///< Error seeking in file %1.

const quint32 EVENT_GLOBAL_ERROR_FILE_WRITE	= 16842768;	///< Error writing data: File %1. %2 bytes of %3 written.


const quint32 EVENT_GLOBAL_ERROR_READ_CONFIG_FILE	= 16842762;	///< Error reading config file %1.


const quint32 EVENT_GLOBAL_ERROR_TIME_OFFSET_TOO_LARGE	= 16842772;	///< Time offset is too large '%1' seconds. Allowed: '%2' seconds.

const quint32 EVENT_GLOBAL_POWER_FAIL	= 16846860;	///< Power fail.

const quint32 EVENT_GLOBAL_SET_DATE_TIME	= 16846858;	///< Setting date / time to '%1'.

const quint32 EVENT_GLOBAL_UNDEFINED	= 16908287;	///< undefined. Used for initialisations

const quint32 EVENT_HIMALAYA_START	= 33619971;	///< PRIMARIS started (serial number is %1 and type is %2)


const quint32 EVENT_IMPORT_ARCHIVE_FILE_FORMAT_NOT_PROPER	= 33947653;	///< Import failed. Format of import file is wrong

const quint32 EVENT_IMPORTEXPORT_NO_USB	= 33947649;	///< No USB devices found in the system to import files

const quint32 EVENT_IMPORTEXPORT_THREADRUNNING	= 33947665;	///< ImportExport thread already running
const quint32 EVENT_IMPORTEXPORT_SIGNALSLOTERROR = 33947668; ///< Not able to connect to signal slot

const quint32 EVENT_IMPORT_FAILED	= 33947664;	///< Import failed

const quint32 EVENT_IMPORT_INIT_CONTAINER_FAILED	= 33947657;	///< Import failed. Container initialization failed

const quint32 EVENT_IMPORT_NEW_EVENTSTRING_FILE_MISSING	= 33947661;	///< Files for language import are not available

const quint32 EVENT_IMPORT_NO_FILES_TO_IMPORT	= 33947652;	///< Import failed. There are no files to Import the data

const quint32 EVENT_IMPORT_REQUIRED_FILES_NOT_AVAILABLE	= 33947655;	///< Import failed. Required files are not available to Import

const quint32 EVENT_IMPORT_ROLLBACK_FAILED	= 33947659;	///< Import failed. Rollback failed

const quint32 EVENT_IMPORT_SUCCESS	= 33947662;	///< Import successful. Please generate new bathlayout

const quint32 EVENT_IMPORT_SUCCESS_LANGUAGE	= 33947663;	///< Import successful. Language files are updated

const quint32 EVENT_IMPORT_SWVERSION_NOT_MATCHING	= 33947656;	///< Import failed. SW versions are not matching to Import the files

const quint32 EVENT_IMPORT_UPDATE_ROLLBACK_FAILED	= 33947660;	///< Import failed. Updating rollback failed

const quint32 EVENT_IMPORT_VERIFICATION_CONTAINER_FAILED	= 33947658;	///< Import failed. Verification of the container failed

const quint32 EVENT_PASSWORD_CHANGED_SUCCESSFULLY	= 33619974;	///< Password changed successfully.

const quint32 EVENT_PASSWORD_INCORRECT	= 33619972;	///< Login failed. Incorrect password.


const quint32 EVENT_PASSWORD_NOT_MATCHING	= 33619973;	///< Unable to change the password. Passwords are not matching.


const quint32 EVENT_PROCESS_HIMALAYA_GUI_CONNECTED	= 33619978;	///< GUI Connected

const quint32 EVENT_PROCESS_HIMALAYA_GUI_STOPPED	= 33619975;	///< GUI has stopped

const quint32 EVENT_STATIONS_DEFECT	= 33619977;	///< The Following Station(s) are defect: %n. Programs using defective stations will no longer be startable.


const quint32 STR_DM_PROGRAM_LEICA_OVERNIGHT	= 50331650;	///< LeicaOvernight

const quint32 STR_DM_PROGRAM_LEICA_BIOPSIE	= 50331651;	///< LeicaBiopsie

const quint32 STR_DM_PROGRAM_LEICA_CLEANINGPROGRAM	= 50331652;	///< CleaningProgram

const quint32 STR_DM_REAGENT_GROUP_Fixation	= 50331653;	///< Fixation

const quint32 STR_DM_REAGENT_GROUP_Water	= 50331654;	///< Water

const quint32 STR_DM_REAGENT_GROUP_Dehydrating_diluted	= 50331655;	///< Dehydrating,diluted

const quint32 STR_DM_REAGENT_GROUP_Dehydrating_absolute	= 50331656;	///< Dehydrating,absolute

const quint32 STR_DM_REAGENT_GROUP_Clearing	= 50331657;	///< Clearing

const quint32 STR_DM_REAGENT_GROUP_Paraffin	= 50331658;	///< Paraffin

const quint32 STR_DM_REAGENT_GROUP_Cleaning_Solvent	= 50331659;	///< Cleaning Solvent

const quint32 STR_DM_REAGENT_GROUP_Cleaning_Alcohol	= 50331660;	///< Cleaning Alcohol

const quint32 STR_DM_REAGENT_Formalin	= 50331661;	///< Formalin

const quint32 STR_DM_REAGENT_Ethanol_70	= 50331662;	///< Ethanol 70%

const quint32 STR_DM_REAGENT_Ethanol_80	= 50331663;	///< Ethanol 80%

const quint32 STR_DM_REAGENT_Ethanol_90	= 50331664;	///< Ethanol 90%

const quint32 STR_DM_REAGENT_Ethanol_95	= 50331665;	///< Ethanol 95%

const quint32 STR_DM_REAGENT_Ethanol_100	= 50331666;	///< Ethanol 100%

const quint32 STR_DM_REAGENT_Xylene	= 50331667;	///< Xylene

const quint32 STR_DM_REAGENT_Paraffin	= 50331668;	///< Paraffin

const quint32 STR_DM_REAGENT_Procssing_Water	= 50331669;	///< Procssing Water

const quint32 STR_DM_REAGENT_Cleaningxylene	= 50331670;	///< Cleaning xylene

const quint32 STR_DM_REAGENT_Cleaningalcohol	= 50331671;	///< Cleaning alcohol

const quint32 STR_SCHDEULER_MAIN_CONTROLLER_STATE	= 50331674;	///< Scheduler main controller state: %1

const quint32 STR_INIT	= 50331675;	///< INIT

const quint32 STR_IDLE	= 50331676;	///< IDLE

const quint32 STR_RUN	= 50331677;	///< RUN

const quint32 STR_ERROR	= 50331678;	///< ERROR

const quint32 STR_UNEXPECTED_STATE	= 50331679;	///< unexpected state

const quint32 STR_SCHDEULER_RECEIVE_MASTER_ACTION_COMMAND	= 50331680;	///< Scheduler receive action command from GUI: %1

const quint32 STR_PROGRAM_STEP_STATE	= 50331681;	///< Current program step state: %1

const quint32 STR_START_PROGRAM	= 50331682;	///< Start program %1

const quint32 STR_CURRENT_PROGRAM_NAME_STEP_REAGENT_LEFTTIME	= 50331683;	///< Program name %1 current step %2 current reagent %3 left time %4s

const quint32 STR_FINISH_PROGRAM	= 50331684;	///< Finish program %1

const quint32 STR_PROGRAM_STEP_STATE_START	= 50331685;	///< step initializing

const quint32 STR_PROGRAM_STEP_STATE_HEAT_LEVEL_SENSOR_1	= 50331686;	///< heating level sensor 1

const quint32 STR_PROGRAM_STEP_STATE_HEAT_LEVEL_SENSOR_2	= 50331687;	///< heating level sensor 2

const quint32 STR_PROGRAM_STEP_STATE_MOVE_TUBE_TO_FILL	= 50331688;	///< moving to tube to fill

const quint32 STR_PROGRAM_STEP_STATE_MOVE_TUBE_TO_DRAIN	= 50331689;	///< moving to tube to draing

const quint32 STR_PROGRAM_STEP_STATE_FILL	= 50331690;	///< filling

const quint32 STR_PROGRAM_STEP_STATE_SEAL	= 50331691;	///< sealing

const quint32 STR_PROGRAM_STEP_STATE_SOAK	= 50331692;	///< soaking

const quint32 STR_PROGRAM_STEP_STATE_DRAIN	= 50331693;	///< draining

const quint32 STR_PROGRAM_STEP_STATE_READY_TO_DRAIN	= 50331694;	///< ready to drain

const quint32 STR_PROGRAM_STEP_STATE_FINISH	= 50331695;	///< finished

const quint32 STR_PROGRAM_STEP_STATE_ERROR	= 50331696;	///< Error

const quint32 STR_PROGRAM_STEP_STATE_PAUSE	= 50331697;	///< pause

const quint32 STR_PROGRAM_STEP_STATE_PAUSE_DRAIN	= 50331698;	///< pause during drain

const quint32 STR_PROGRAM_STEP_STATE_ABORTED	= 50331699;	///< aborted

const quint32 STR_PROGRAM_STEP_STATE_ABORTING	= 50331700;	///< aborting

const quint32 STR_PROGRAM_STEP_STATE_UNKNOWN	= 50331701;	///< unknown

const quint32 STR_PROGRAM_SELFTEST_START	= 50331702;	///< start selftest

const quint32 STR_PROGRAM_SELFTEST_CHECK_TEMP_PRESSURE	= 50331703;	///< checking temperature and pressure

const quint32 STR_PROGRAM_SELFTEST_CHECK_BOTTLE	= 50331704;	///< checking bottle %1

const quint32 STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT	= 50331705;	///< bottle check result: %1

const quint32 STR_PROGRAM_SELFTEST_FINISH	= 50331706;	///< selftest finished

const quint32 STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_OK	= 50331707;	///< Ok

const quint32 STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_NOT_FULL	= 50331708;	///< not full

const quint32 STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_BLOCKAGE	= 50331709;	///< blockage

const quint32 STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_EMPTY	= 50331710;	///< empty

const quint32 STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_ERROR	= 50331711;	///< error

const quint32 STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_TIMEOUT	= 50331712;	///< timeout

const quint32 STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_UNEXPECTED	= 50331713;	///< unexpected

const quint32 STR_PROGRAM_COMMAND_START_PROGRAM	= 50331714;	///< start program

const quint32 STR_PROGRAM_COMMAND_PAUSE_PROGRAM	= 50331715;	///< pause program

const quint32 STR_PROGRAM_COMMAND_ABORT_PROGRAM	= 50331716;	///< abort program

const quint32 STR_PROGRAM_COMMAND_DRAIN	= 50331717;	///< drain

const quint32 EVENT_SERVICE_OPERATIONTIME_OVERDUE	= 50331718;	///< Please call the service engineer for maintenance!

const quint32 EVENT_SERVICE_ACTIVECARBONTIME_OVERDUE_WARNING	= 50331719;	///< The Active carbon filter is overdue, please replace a new Active carbon filter!

const quint32 STR_EXECUTE_COMMAND	= 50331720;	///< Scheduler execute command %1

const quint32 STR_EXECUTE_COMMAND_RESULT	= 50331721;	///< Scheduler execute command %1 result %2


const quint32 EVENT_IMPORT_TAMPERED_ARCHIVE_FILE	= 50331759;	///< Import failed. Archive file is tampered

const quint32 EVENT_IMPORT_DEVICE_NAME_NOT_MATCHING	= 50331760;	///< Import failed. SW versions are not matching to Import the files

const quint32 EVENT_IMPORT_TYPEOFIMPORTNOTVALID	= 50331761;	///< Type of import is not valid

const quint32 EVENT_IMPORTEXPORT_IMPORT_NO_USB	= 50331762;	///< No USB devices found in the system to import files

const quint32 EVENT_IMPORTEXPORT_EXPORT_NO_USB	= 50331763;	///< No USB devices found in the system to export files

const quint32 EVENT_SERVICE_OPERATIONTIME_RESET	= 50331764;	///< Reset michine operation time

const quint32 EVENT_SERVICE_ACTIVECARBONTIME_RESET	= 50331765;	///< Reset Active carbon filter operation time

const quint32 EVENT_DEVICE_ALARM_PAUSE_5MINTUES = 50331766;	   ///< instrument alarm when pausing exceed 5 minutes

const quint32 EVENT_LOCAL_ALARM_PAUSE_10MINTUES = 50331767;	   ///< local alarm when pausing exceed 10 minustes

const quint32 EVENT_REMOTE_ALARM_PAUSE_15MINTUES = 50331768;   ///< remote alarm when pausing exceed 15 minutes

const quint32 EVENT_WAIT_FILLING_FINISH = 50331769;   ///< when press "Pause", message "Please wait for filling to be completed, then try a again!"

const quint32 EVENT_WAIT_DRAINING_FINISH = 50331770;   ///< when press "Pause", message "Please wait for draining to be completed, then try a again!"

const quint32 EVENT_SERVICE_ACTIVECARBONTIME_OVERDUE_ALARM	= 50331771;	///< The Active carbon filter is overdue, please replace a new Active carbon filter!

#endif // HIMALAYAEVENTCODES_H

