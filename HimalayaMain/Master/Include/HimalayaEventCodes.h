/****************************************************************************/
/*! \file Include/HimalayaEventCodes.h
 *
 *  \brief Definition of event groups for event codes.
 *
 *  Version:    0.1
 *  Date:       2010-07-12
 *  Author:     J.Bugariu
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

#ifndef HIMALAYAEVENTCODES_H
#define HIMALAYAEVENTCODES_H

#include <QtGlobal>

/// \todo JB: check what is still needed...
const quint32 EVENT_GROUP_HIMALAYA   = 0x02;        ///< Event group for Himalaya.
const quint32 EVENT_GROUP_HIMALAYA_MAIN             = ((EVENT_GROUP_HIMALAYA << 8) + 0x01) << 16;   ///< Event group for main program.
const quint32 EVENT_GROUP_HIMALAYA_VERIFIER         = ((EVENT_GROUP_HIMALAYA << 8) + 0x02) << 16;   ///< Event group for verifier.
const quint32 EVENT_GROUP_HIMALAYA_SCHEDULER        = ((EVENT_GROUP_HIMALAYA << 8) + 0x03) << 16;   ///< Event group for himalaya scheduler.
const quint32 EVENT_GROUP_HIMALAYA_DATAMANAGER      = ((EVENT_GROUP_HIMALAYA << 8) + 0x04) << 16;   ///< Event group for himalaya DataManager.
const quint32 EVENT_GROUP_HIMALAYA_BLG              = ((EVENT_GROUP_HIMALAYA << 8) + 0x05) << 16;   ///< Event group for himalaya BLG.
const quint32 EVENT_GROUP_HIMALAYA_IMPORTEXPORT     = ((EVENT_GROUP_HIMALAYA << 8) + 0x06) << 16;   ///< Event group for himalaya Import/Export.
const quint32 EVENT_GROUP_HIMALAYA_RMS              = ((EVENT_GROUP_HIMALAYA << 8) + 0x07) << 16;   ///< Event group for himalaya RMS.
const quint32 EVENT_GROUP_HIMALAYA_SEPIA            = ((EVENT_GROUP_HIMALAYA << 8) + 0x08) << 16;   ///< Event group for himalaya Sepia connection.

const quint32 EVENT_RACK_INSERTED                   = EVENT_GROUP_HIMALAYA_MAIN + 0x0001;
const quint32 EVENT_RACK_REMOVED                    = EVENT_GROUP_HIMALAYA_MAIN + 0x0002;
const quint32 EVENT_HIMALAYA_START                  = EVENT_GROUP_HIMALAYA_MAIN + 0x0003;
const quint32 EVENT_PASSWORD_INCORRECT              = EVENT_GROUP_HIMALAYA_MAIN + 0x0004; ///< Incorrect password
const quint32 EVENT_PASSWORD_NOT_MATCHING           = EVENT_GROUP_HIMALAYA_MAIN + 0x0005; ///< New Password and confirmation password are not matching
const quint32 EVENT_PASSWORD_CHANGED_SUCCESSFULLY   = EVENT_GROUP_HIMALAYA_MAIN + 0x0006; ///< Password changed successfully
const quint32 EVENT_PROCESS_HIMALAYA_GUI_STOPPED    = EVENT_GROUP_HIMALAYA_MAIN + 0x0007;
const quint32 EVENT_PROCESS_SEPIA_STOPPED           = EVENT_GROUP_HIMALAYA_MAIN + 0x0008;
//EventCodes for ProgramStartable Manager
const quint32 EVENT_STATIONS_DEFECT                 = EVENT_GROUP_HIMALAYA_MAIN + 0x0009;
const quint32 EVENT_PROCESS_SEPIA_CONNECTED         = EVENT_GROUP_HIMALAYA_MAIN + 0x000A;
const quint32 EVENT_PROCESS_HIMALAYA_GUI_CONNECTED  = EVENT_GROUP_HIMALAYA_MAIN + 0x000B;
const quint32 EVENT_DI_WATER_NOT_AVAILABLE          = EVENT_GROUP_HIMALAYA_MAIN + 0x000C;
const quint32 EVENT_REAGENT_NOT_PRESENT_IN_BATHLAYOUT       = EVENT_GROUP_HIMALAYA_MAIN + 0x000D;
const quint32 EVENT_REAGENT_EXPIRED_PROG_NOT_STARTABLE      = EVENT_GROUP_HIMALAYA_MAIN + 0x000E;
const quint32 EVENT_5_SLIDE_REAGENT_USED_FOR_NORMAL_RACK    = EVENT_GROUP_HIMALAYA_MAIN + 0x000F;
const quint32 EVENT_REMOVE_RACK_PROG_NOT_STARTABLE  =   EVENT_GROUP_HIMALAYA_MAIN + 0x0010;
const quint32 EVENT_INITIALIZATION_FAILED           = EVENT_GROUP_HIMALAYA_MAIN + 0x0011;
const quint32 EVENT_5_SLIDE_RACK_WITH_LAST_STEP_TRANSFER = EVENT_GROUP_HIMALAYA_MAIN + 0x0012;
const quint32 EVENT_HIMALAYA_DM_GV_FAILED                = EVENT_GROUP_HIMALAYA_MAIN + 0x0013; ///< Group verification is failed
const quint32 EVENT_INCORRECT_RACK_ORIENTATION           = EVENT_GROUP_HIMALAYA_MAIN + 0x0014;
const quint32 EVENT_HOOD_OPEN_NO_RACK_PROCESSING        = EVENT_GROUP_HIMALAYA_MAIN + 0x0015; ///< As the hood is still open, we ignore racks placed into drawers (because when hood is closed again, we read them againa anyway
// event codes for date time
const quint32 EVENT_DATE_TIME_CANNOT_BE_MORE_THAN_24_HOURS       = EVENT_GROUP_HIMALAYA_MAIN + 0x0016;
const quint32 EVENT_TEST_1                              =EVENT_GROUP_HIMALAYA_MAIN + 0x0018; ///< As the hood is still open, we ignore racks placed into drawers (because when hood is closed again, we read them againa anyway
const quint32 EVENT_UNKNOWN_DRAWER_RFID                 = EVENT_GROUP_HIMALAYA_MAIN + 0x0019; ///< RFID tag read by drawer is illegible

// log user activities of the data manager starts from 0x0200
/// don't start the number for events(EVENT_GROUP_HIMALAYA_MAIN) from 0x0200
const quint32 EVENT_USER_ACTIVITY_PROGRAM_ADDED             = EVENT_GROUP_HIMALAYA_MAIN + 0x0200; ///< Program %1 added
const quint32 EVENT_USER_ACTIVITY_PROGRAM_DELETED           = EVENT_GROUP_HIMALAYA_MAIN + 0x0201; ///< Program %1 deleted
const quint32 EVENT_USER_ACTIVITY_PROGRAM_UPDATE            = EVENT_GROUP_HIMALAYA_MAIN + 0x0202; ///< Program %1 updated
const quint32 EVENT_USER_ACTIVITY_PROGRAM_MODIFY_LEICA      = EVENT_GROUP_HIMALAYA_MAIN + 0x0203; ///< Leica Program %1 updated
const quint32 EVENT_USER_ACTIVITY_PROGRAM_COLOR             = EVENT_GROUP_HIMALAYA_MAIN + 0x0204; ///< Color for program %1 changed
const quint32 EVENT_USER_ACTIVITY_REAGENT_ADDED             = EVENT_GROUP_HIMALAYA_MAIN + 0x0205; ///< Reagent %1 added
const quint32 EVENT_USER_ACTIVITY_REAGENT_DELETED           = EVENT_GROUP_HIMALAYA_MAIN + 0x0206; ///< Reagent %1 deleted
const quint32 EVENT_USER_ACTIVITY_REAGENT_UPDATED           = EVENT_GROUP_HIMALAYA_MAIN + 0x0207; ///< Reagent %1 updated
const quint32 EVENT_USER_ACTIVITY_US_LANGUAGE_CHANGED       = EVENT_GROUP_HIMALAYA_MAIN + 0x0208; ///< Language set to %1
const quint32 EVENT_USER_ACTIVITY_US_DATE_TIME_CHANGED      = EVENT_GROUP_HIMALAYA_MAIN + 0x0209; ///< Date/ Time changed: %1
const quint32 EVENT_USER_ACTIVITY_US_OVEN_TEMP_CHANGED      = EVENT_GROUP_HIMALAYA_MAIN + 0x020A; ///< Oventemperature changed to %1
const quint32 EVENT_USER_ACTIVITY_US_NETWORK_SETTINGS_ADDED = EVENT_GROUP_HIMALAYA_MAIN + 0x020B; ///< Network Settings changed
const quint32 EVENT_USER_ACTIVITY_OTHER_WHITE_RACK_INSERTED = EVENT_GROUP_HIMALAYA_MAIN + 0x020C; ///< White rack assigned to program %1
const quint32 EVENT_USER_ACTIVITY_OTHER_ADMIN_LOGIN         = EVENT_GROUP_HIMALAYA_MAIN + 0x020D; ///< Administrator Login
const quint32 EVENT_USER_ACTIVITY_OTHER_STANDARD_LOGIN      = EVENT_GROUP_HIMALAYA_MAIN + 0x020E; ///< User Login
const quint32 EVENT_USER_ACTIVITY_OTHER_PASSWORD_CHANGED    = EVENT_GROUP_HIMALAYA_MAIN + 0x020F; ///< Administartor Password changed

//Event codes for the BLG process events
const quint32 EVENT_BLG_GENERATION_SUCCESS                      = EVENT_GROUP_HIMALAYA_BLG + 0x0001;
const quint32 EVENT_BLG_READING_INPUTFILE_FAILED                = EVENT_GROUP_HIMALAYA_BLG + 0x0002;
const quint32 EVENT_BLG_USED_PROGRAM_LIST_EMPTY                 = EVENT_GROUP_HIMALAYA_BLG + 0x0003;
const quint32 EVENT_BLG_EMPTY_BLGRULES_LIST                     = EVENT_GROUP_HIMALAYA_BLG + 0x0004;
const quint32 EVENT_BLG_WRITING_OUTPUT_FILES_FAILED             = EVENT_GROUP_HIMALAYA_BLG + 0x0005;
const quint32 EVENT_BLG_BATHLAYOUT_SELECTION_FAILED_TREE_EMPTY  = EVENT_GROUP_HIMALAYA_BLG + 0x0008;
const quint32 EVENT_BLG_GENERATION_FAILURE                      = EVENT_GROUP_HIMALAYA_BLG + 0x000a;
const quint32 EVENT_BLG_MAIN_READING_STATIONS_FAILED            = EVENT_GROUP_HIMALAYA_BLG + 0x000b;
const quint32 EVENT_BLG_MAIN_STATIONS_VERIFY_FAILED             = EVENT_GROUP_HIMALAYA_BLG + 0x000c;
const quint32 EVENT_BLG_MAIN_READING_PROG_SEQ_FAILED            = EVENT_GROUP_HIMALAYA_BLG + 0x000d;
const quint32 EVENT_BLG_MAIN_PROG_SEQ_VERIFY_FAILED             = EVENT_GROUP_HIMALAYA_BLG + 0x000e;
const quint32 EVENT_BLG_MAIN_ERROR_STATION_NULL_POINTER         = EVENT_GROUP_HIMALAYA_BLG + 0x000f;
const quint32 EVENT_BLG_MAIN_ERROR_CONTAINER_NULL_POINTER       = EVENT_GROUP_HIMALAYA_BLG + 0x0010;

// Event codes for the Import Controller
const quint32 EVENT_IMPORTEXPORT_NO_USB                     = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0001;
const quint32 EVENT_IMPORT_DIRECTORY_CREATION_FAILED        = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0002;
const quint32 EVENT_IMPORT_NO_USB_DIRECTORY_EXIST           = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0003;
const quint32 EVENT_IMPORT_NO_FILES_TO_IMPORT               = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0004;
const quint32 EVENT_IMPORT_ARCHIVE_FILE_FORMAT_NOT_PROPER   = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0005;
const quint32 EVENT_IMPORT_TAMPERED_ARCHIVE_FILE            = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0006;
const quint32 EVENT_IMPORT_REQUIRED_FILES_NOT_AVAILABLE     = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0007;
const quint32 EVENT_IMPORT_SWVERSION_NOT_MATCHING           = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0008;
const quint32 EVENT_IMPORT_INIT_CONTAINER_FAILED            = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0009;
const quint32 EVENT_IMPORT_VERIFICATION_CONTAINER_FAILED    = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x000A;
const quint32 EVENT_IMPORT_ROLLBACK_FAILED                  = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x000B;
const quint32 EVENT_IMPORT_UPDATE_ROLLBACK_FAILED           = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x000C;
const quint32 EVENT_IMPORT_NEW_EVENTSTRING_FILE_MISSING     = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x000D;
const quint32 EVENT_IMPORT_SUCCESS                          = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x000E;
const quint32 EVENT_IMPORT_SUCCESS_LANGUAGE                 = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x000F;
const quint32 EVENT_IMPORT_FAILED                           = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0010;
const quint32 EVENT_IMPORTEXPORT_THREADRUNNING              = EVENT_GROUP_HIMALAYA_IMPORTEXPORT + 0x0011; ///< ImportExport thread already running

//Event codes for RMS module
const quint32 EVENT_RMS_BOTTLE_SCAN_TIMEOUT         =   EVENT_GROUP_HIMALAYA_RMS + 0x0001; ///< Bottle not scanned in time.
const quint32 EVENT_RMS_KIT_SCAN_TIMEOUT            =   EVENT_GROUP_HIMALAYA_RMS + 0x0002; ///< Kit not scanned in time.
const quint32 EVENT_RMS_CONSUMABLES_ALREADY_USED    =   EVENT_GROUP_HIMALAYA_RMS + 0x0003; ///< Consumables scanned already.
const quint32 EVENT_RMS_KIT_BEST_BEFORE_EXPIRED     =   EVENT_GROUP_HIMALAYA_RMS + 0x0004; ///< Kit's best before date expired.
const quint32 EVENT_RMS_INVALID_KIT_PROG_ID         =   EVENT_GROUP_HIMALAYA_RMS + 0x0005; ///< Kit does not belong to program id.
const quint32 EVENT_RMS_WRITE_STATIONS_FAILED       =   EVENT_GROUP_HIMALAYA_RMS + 0x0006; ///< New RMS data verfication failed.
const quint32 EVENT_RMS_INVALID_SCAN_ORDER          =   EVENT_GROUP_HIMALAYA_RMS + 0x0007; ///< Invalid scan order.
const quint32 EVENT_RMS_KIT_DATA_ERROR              =   EVENT_GROUP_HIMALAYA_RMS + 0x0008; ///< Kit data error.
const quint32 EVENT_RMS_BOTTLE_DATA_ERROR           =   EVENT_GROUP_HIMALAYA_RMS + 0x0009; ///< Bottle data error.
const quint32 EVENT_RMS_CONSUMABLES_RFID_SCAN_ERROR =   EVENT_GROUP_HIMALAYA_RMS + 0x000A; ///< Kit scan error.


const quint32 EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER           =   EVENT_GROUP_HIMALAYA_RMS + 0x000E; ///< Data container pointer is null.
const quint32 EVENT_RMS_ERROR_UPDATE_STATION                        =   EVENT_GROUP_HIMALAYA_RMS + 0x000F; ///< Error in updating station.
const quint32 EVENT_RMS_NO_STATION_PRESENT                          =   EVENT_GROUP_HIMALAYA_RMS + 0x0010; ///< Station is not present.
const quint32 EVENT_RMS_NO_REAGENT_PRESENT                          =   EVENT_GROUP_HIMALAYA_RMS + 0x0011; ///< Reagent is not present.
const quint32 EVENT_RMS_ERROR_UPDATE_REAGENT                        =   EVENT_GROUP_HIMALAYA_RMS + 0x0012; ///< Error in updating reagent.
const quint32 EVENT_RMS_REAGENT_ERROR_NULL_POINTER                  =   EVENT_GROUP_HIMALAYA_RMS + 0x0013; ///< Reagent pointer is null.
const quint32 EVENT_RMS_LEICA_REAGENT_BEST_BEFORE_DATE_EXPIRED      =   EVENT_GROUP_HIMALAYA_RMS + 0x0014; ///< Leica reagent best before date expired.
const quint32 EVENT_RMS_REAGENT_ID_NOT_VALID                        =   EVENT_GROUP_HIMALAYA_RMS + 0x0015; ///< Reagent ID not valid.
const quint32 EVENT_RMS_STATION_ID_NOT_VALID                        =   EVENT_GROUP_HIMALAYA_RMS + 0x0016; ///< Station ID not valid.
const quint32 EVENT_RMS_REAGENT_ID_NOT_PRESENT                      =   EVENT_GROUP_HIMALAYA_RMS + 0x0017; ///< Reagent ID not present.
const quint32 EVENT_RMS_LOT_NUMBER_NOT_PRESENT_IN_KIT               =   EVENT_GROUP_HIMALAYA_RMS + 0x0018; ///< Lot number does'nt exists in Kit.
const quint32 EVENT_RMS_ERROR_CONFIRM_LEICA_REAGENT                 =   EVENT_GROUP_HIMALAYA_RMS + 0x0019; ///< Confirm Leica reagent failed.
const quint32 EVENT_RMS_ERROR_CONFIRM_BATHLAYOUT                    =   EVENT_GROUP_HIMALAYA_RMS + 0x001A; ///< Confirm Bathlayout failed.
const quint32 EVENT_RMS_ERROR_RESET_STATION_RMS                     =   EVENT_GROUP_HIMALAYA_RMS + 0x001B; ///< Reset station failed.
const quint32 EVENT_RMS_ERROR_UPDATE_REAGENT_CONTAINER              =   EVENT_GROUP_HIMALAYA_RMS + 0x001C; ///< Update reagent container failed.
const quint32 EVENT_RMS_ERROR_UPDATE_STATION_CONTAINER              =   EVENT_GROUP_HIMALAYA_RMS + 0x001D; ///< Update station container failed.
const quint32 EVENT_RMS_INVALID_LEICA_CONSUMABLE                    =   EVENT_GROUP_HIMALAYA_RMS + 0x001E; ///< Invalid Leica consumable.
const quint32 EVENT_RMS_ERROR_UPDATE_CONTAINERS                     =   EVENT_GROUP_HIMALAYA_RMS + 0x001F; ///< Error in updating containers.
const quint32 EVENT_RMS_STATIONLIST_VERIFICATION_FAILED             =   EVENT_GROUP_HIMALAYA_RMS + 0x0020; ///< Stationlist verification failed.
const quint32 EVENT_RMS_ERROR_UPDATE_LEICA_REAGENT                  =   EVENT_GROUP_HIMALAYA_RMS + 0x0021; ///< Update Leica reagent failed.



#endif // HIMALAYAEVENTCODES_H
