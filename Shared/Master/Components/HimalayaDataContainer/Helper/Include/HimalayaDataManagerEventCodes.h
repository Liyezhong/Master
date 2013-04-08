/****************************************************************************/
/*! \file HimalayaDataManagerEventCodes.h
 *
 *  \brief Definition file for class CDataContainerBase.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2012-08-10
 *  $Author:    $ N.Kamath
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
#ifndef DATAMANAGEREVENTCODES_H
#define DATAMANAGEREVENTCODES_H

#include "HimalayaEventCodes.h"

const quint32 EVENT_DM_INVALID_PROGRAM_ID          =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x001; ///< '%1' is not a valid Program ID.
const quint32 EVENT_DM_PROGRAM_NAME_NOT_UNIQUE    =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x002; ///< Program name '%1' already in use. Please choose different Program name.
const quint32 EVENT_DM_SHORT_NAME_NOT_UNIQUE      =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x003; ///< Program Short name '%1' already in use. Please choose different Program Short name.
const quint32 EVENT_DM_OVEN_NOT_FIRST_STEP      =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x004; ///< Please select oven as first step for the program '%1'.
const quint32 EVENT_DM_REAGENTID_NOT_UNIQUE     =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x005; ///< Same Reagent used in multiple steps in the program '%1'. Reagent should be exclusive to a step in the program.
const quint32 EVENT_DM_TRANS_UNLOADER_STEP_ONLY =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x006; ///< Program with only unloader or loader steps are invalid. Please add steps to program '%1'.
const quint32 EVENT_DM_CONSTRUCTION_FAILED      =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x007; ///< Container construction failed
const quint32 EVENT_DM_PROGRAM_XML_READ_FAILED  =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x008;
const quint32 EVENT_DM_REAGENT_XML_READ_FAILED  =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x009;
const quint32 EVENT_DM_PROG_SEQ_XML_READ_FAILED  =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x00A;
const quint32 EVENT_DM_STATIONS_XML_READ_FAILED  =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x00B;
const quint32 EVENT_DM_STATIONGRID_XML_READ_FAILED  =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x00C;
const quint32 EVENT_DM_STATIONGRID_VERIFICATION_FAILED  =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x00D;
const quint32 EVENT_DM_STATION_VERIFICATION_FAILED  =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x00E;
const quint32 EVENT_DM_REAGENT_VERIFICATION_FAILED  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x000F;
const quint32 EVENT_DM_PROGRAM_VERIFICATION_FAILED  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0010;
const quint32 EVENT_DM_PROG_SEQ_VERIFICATION_FAILED  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0011;
const quint32 EVENT_DM_BLG_CONF_READ_FAILED     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0012;
const quint32 EVENT_DM_BLG_CONF_VERIFICATION_FAILED     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0013;
const quint32 EVENT_DM_PROGRAM_INVALID_INDEX    = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0014;
const quint32 EVENT_DM_PROG_COUNT_EXCEEDS_LIMIT     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0015;
const quint32 EVENT_DM_NEXT_STEPID_INVALID          = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0016;
const quint32 EVENT_DM_NEXT_STEPID_LESS_THAN_ONE    = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0017;
const quint32 EVENT_DM_PROGRAM_ID_NOT_FOUND         = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0018;
const quint32 EVENT_DM_PROGSHORT_NAME_LENGTH_CHECK_FAILED = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0019;
const quint32 EVENT_DM_PROGLONG_NAME_LENGTH_CHECK_FAILED  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X001A;
const quint32 EVENT_DM_UKNOWN_PROG_COLOUR           = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X001B;
const quint32 EVENT_DM_PROG_COLOUR_NOT_UNIQUE       = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X001C;
const quint32 EVENT_DM_PROG_STEP_COUNT_EXCEEDS_LIMIT    = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X001D;
const quint32 EVENT_DM_PROG_VERFIER_INVALID_REAGENT_ID  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X001E;
const quint32 EVENT_DM_REAGENT_USED_IN_MULTIPLE_PROG_STEPS  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X001F;
const quint32 EVENT_DM_INVALID_STEP_ID                  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0020;
const quint32 EVENT_DM_INVALID_LEICA_PARALLEL_STN_CNT   = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0021;
const quint32 EVENT_DM_INTENSITY_STEP_COUNT_INVALID     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0022;
const quint32 EVENT_DM_STEP_INTENSITY_VALUE_INVALID     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0023;
const quint32 EVENT_DM_STEP_RANGE_LOW_VALUE_EMPTY       = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0024;
const quint32 EVENT_DM_STEP_RANGE_LOW_ZERO              = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0025;
const quint32 EVENT_DM_STEP_RANGE_HIGH_VALUE_EMPTY      = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0026;
const quint32 EVENT_DM_STEP_RANGE_HIGH_ZERO             = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0027;
const quint32 EVENT_DM_STEP_MIN_DURATION_EMPTY          = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0028;
const quint32 EVENT_DM_STEP_MIN_DURATION_INVALID        = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X0029;
const quint32 EVENT_DM_STEP_MAX_DURATION_INVALID        = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X002A;
const quint32 EVENT_DM_TIME_TOLERANCE_ZERO              = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X002B;
const quint32 EVENT_DM_PROG_INVALID_DURATION_FORMAT     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0X002C;
const quint32 EVENT_DM_NULL_PTR                         = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x002D;
const quint32 EVENT_DM_PROG_EDITED_LEICA_REAGENT_COUNT_INVALID         = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x002E;
const quint32 EVENT_DM_PROG_ID_NOT_UNIQUE               = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x002F;
const quint32 EVENT_DM_PROG_DELETE_FAILED_ID_NOT_FOUND  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0030;

const quint32 EVENT_DM_PROG_UPDATE_FAILED_INTERNAL_ERR  = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0031;
const quint32 EVENT_DM_PROG_ADD_FAILED_INTERNAL_ERR     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0032;
const quint32 EVENT_DM_ADJUSTMENT_CONF_READ_FAILED      = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0033;
const quint32 EVENT_DM_ADJUSTMENT_VERIFICATION_FAILED   = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0034;

const quint32 EVENT_DM_USER_PROGRAM_CONTAINS_LEICA_REAGENT   = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0035; ///< User program %1 contains the Leica reagent %2
const quint32 EVENT_DM_LEICA_PROGRAMS_CONTAIN_SAME_LEICA_REAGENT   = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0036; ///< Leica reagent %1 is used in two or more Leica programs %2
const quint32 EVENT_DM_PROG_DEL_FAILED_INTERNAL_ERR     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0037; ///< Leica reagent %1 is used in two or more Leica programs %2
const quint32 EVENT_DM_PROG_DELETE_FAILED_TEXT          = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0038;
const quint32 EVENT_DM_REAGENT_EVENTS               =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x100;
const quint32 EVENT_DM_REAGENT_NAME_NOT_UNIQUE        =   EVENT_DM_REAGENT_EVENTS + 0x001; ///< Reagent name '%1' already in use. Please choose different reagent name.
const quint32 EVENT_DM_REAGENT_SHORT_NAME_NOT_UNIQUE  =   EVENT_DM_REAGENT_EVENTS + 0x002; ///< Reagent Short name '%1' already in use. Please choose different Reagent Short name.
const quint32 EVENT_DM_REAGENT_ID_NOT_UNIQUE          =   EVENT_DM_REAGENT_EVENTS + 0x003; ///< Reagent ID '%1' already in use. Please choose different Reagent ID.
const quint32 EVENT_DM_REAGENT_ID_DOESTNOT_EXISTS   =   EVENT_DM_REAGENT_EVENTS + 0x004; ///< Reagent ID '%1' does not exists in the list.
const quint32 EVENT_DM_REAGENT_EXCEEDS_LIMIT            =   EVENT_DM_REAGENT_EVENTS + 0x005; ///< Too many reagents (max 100 allowed):
const quint32 EVENT_DM_REAGENT_INVALID_PREFIX       =   EVENT_DM_REAGENT_EVENTS + 0x006; ///< Reagent '%1' Reagent ID doesn't start with S,L,U:
const quint32 EVENT_DM_REAGENT_SHORTNAME_LENGTH_CHECK_FAILED    =   EVENT_DM_REAGENT_EVENTS + 0x007; ///< Reagent '%1' Short Name is too long (max 10 Characters)
const quint32 EVENT_DM_REAGENT_NAME_LENGTH_CHECK_FAILED     =   EVENT_DM_REAGENT_EVENTS + 0x008; ///< Reagent '%1' Name is too long (max 32 Characters)
const quint32 EVENT_DM_REAGENT_MAXSLIDES_EXCEEDS_LIMIT    =   EVENT_DM_REAGENT_EVENTS + 0x009; ///< Reagent '%1' The value for Slides max must be equal or less than 3000.
const quint32 EVENT_DM_REAGENT_USE_DAYS_EXCEEDS_LIMIT        =   EVENT_DM_REAGENT_EVENTS + 0x00A; ///< Reagent '%1' The value for Days max must be equal or less than 99.
const quint32 EVENT_DM_REAGENT_TEMP_NOTIN_LIMIT     =   EVENT_DM_REAGENT_EVENTS + 0x00B; ///< Reagent '%1' Temperature not in limit (between 37 and 75)
const quint32 EVENT_DM_REAGENT_HEATING_MODE_UNDEF   =   EVENT_DM_REAGENT_EVENTS + 0x00C; ///< Reagent '%1' Heating Start mode Undefined
const quint32 EVENT_DM_REAGENT_LIST_WRITE_FAILED    =   EVENT_DM_REAGENT_EVENTS + 0x00D; ///< ReagentList writing failed '%1' reagents
const quint32 EVENT_DM_REAGENT_LIST_READ_FAILED     =   EVENT_DM_REAGENT_EVENTS + 0x00E; ///< ReagentList reading failed '%1' reagents
const quint32 EVENT_DM_REAGENT_OPERATION_FAILED     =   EVENT_DM_REAGENT_EVENTS + 0x00F; ///< Reagent : '%1' operation failed
const quint32 EVENT_DM_REAGENT_NAME_UNIQUE        =   EVENT_DM_REAGENT_EVENTS + 0x010; ///< Reagent name must be unique.
const quint32 EVENT_DM_REAGENT_SHORT_NAME_UNIQUE  =   EVENT_DM_REAGENT_EVENTS + 0x011; ///< Short name must be unique.
//const quint32 EVENT_DM_REAGENT_MAXSLIDES_RANGE    =   EVENT_DM_REAGENT_EVENTS + 0x012; ///< The value for Slides max must be equal or less than 3000.
//const quint32 EVENT_DM_REAGENT_USE_DAYS_RANGE        =   EVENT_DM_REAGENT_EVENTS + 0x013; ///< The value for Days max must be equal or less than 99.


///< Started the number from the 200 - please dont use for other IDs
const quint32 EVENT_DM_GV_REAGENTID_EXIST_IN_PROG_NOT_IN_REAGENT_LIST         = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0200; ///< Reagent ID exists in the Program list, not in Reagent list: %1
const quint32 EVENT_DM_GV_REAGENTID_EXIST_IN_STATION_NOT_IN_REAGENT_LIST      = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0201; ///< Reagent ID exists in Station list, not in Reagenet list : %1
const quint32 EVENT_DM_GV_NO_OF_PROGRAMS_NOT_MATCHING                         = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0202; ///< No of program in Program sequece list does not match with program list
const quint32 EVENT_DM_GV_PROG_ID_EXIST_IN_STATION_NOT_IN_PROG_LIST           = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0203; ///< Program ID exists in the Program sequence list, not in Program list : %1
const quint32 EVENT_DM_GV_REAGENTID_EXIST_IN_SETTINGS_NOT_IN_REAGENT_LIST     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0204; ///< Reagent ID exist in User settings, not in the Reagent list: %1
const quint32 EVENT_DM_GV_STATIONID_EXIST_IN_STATION_NOT_IN_STATIONMATRIX     = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0205; ///< Station ID exist in Station list, not in Station matrix : %1
const quint32 EVENT_DM_GV_BASE_CONTAINER_IS_NOT_INITIALIZED                   = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0206; ///< Group verification failed. Base container is not initialized
const quint32 EVENT_DM_GV_INVALID_CONTAINER_TYPE                              = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0207; ///< Group verification failed. Unknown container type is detected
const quint32 EVENT_DM_GV_REAGENT_IS_NOT_INITIALIZED                          = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0208; ///< Group verification failed. Reagent container is not initialized
const quint32 EVENT_DM_GV_PROGRAM_IS_NOT_INITIALIZED                          = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x0209; ///< Group verification failed. Program container is not initialized
const quint32 EVENT_DM_GV_STATION_IS_NOT_INITIALIZED                          = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x020A; ///< Group verification failed. Station container is not initialized
const quint32 EVENT_DM_GV_PSEQUENCE_IS_NOT_INITIALIZED                        = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x020B; ///< Group verification failed. Program sequence container is not initialized
const quint32 EVENT_DM_GV_STATIONMATRIX_IS_NOT_INITIALIZED                    = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x020C; ///< Group verification failed. Station matrix container is not initialized
const quint32 EVENT_DM_GV_USINTERFACE_IS_NOT_INITIALIZED                      = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x020D; ///< Group verification failed. User settings container is not initialized
const quint32 EVENT_DM_REAGENTID_EXIST_IN_STATION_NOT_IN_REAGENT_LIST         = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x020E; ///< Reagent ID exists in Station list, not in Reagenet list : %1
const quint32 EVENT_DATAMANAGER_ERROR_XML_ATTRIBUTE_NOT_FOUND                 = EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x020F;

const quint32 EVENT_DM_PROGSEQ_EVENTS               =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x300;
const quint32 EVENT_DM_PROGSEQ_ID_REPEATED          =   EVENT_DM_PROGSEQ_EVENTS + 0x001; ///< Program Sequence ID '%1' already in use. Please choose different ID.
const quint32 EVENT_DM_PROGSEQ_ID_DOESTNOT_EXISTS   =   EVENT_DM_PROGSEQ_EVENTS + 0x002; ///< Program Sequence ID '%1' does not exists in the list.
const quint32 EVENT_DM_PROGSEQ_OPERATION_FAILED     =   EVENT_DM_PROGSEQ_EVENTS + 0x003; ///< Program Sequence '%1' operation failed
const quint32 EVENT_DM_PROGSEQ_MAX_COUNT_REACHED    =   EVENT_DM_PROGSEQ_EVENTS + 0x004; ///< Too many Program Sequence (max 50 allowed):
const quint32 EVENT_DM_PROGSEQ_INVALID_PREFIX       =   EVENT_DM_PROGSEQ_EVENTS + 0x005; ///< Program Sequence: '%1' Program ID shall start with L/U and should be greater than 0:
const quint32 EVENT_DM_PROGSEQ_INVALID_BLGRESULT      =   EVENT_DM_PROGSEQ_EVENTS + 0x006; ///< Program Sequence BLGResult Can't have more than 5 Error code, BLG Result can be either success or failure, not both:
const quint32 EVENT_DM_PROGSEQ_INVALID_BLGRESULT_CODE =   EVENT_DM_PROGSEQ_EVENTS + 0x007; ///< Program Sequence : '%1' : Invalid BLG result code

const quint32 EVENT_DM_BLGCONF_EVENTS                               =   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x400;
const quint32 EVENT_DM_BLGCONF_TERMCOUNT_EXCEEDS_LIMIT              =   EVENT_DM_BLGCONF_EVENTS + 0x001; ///< Blgconf : '%1' : Termination Count value is more than 10
const quint32 EVENT_DM_BLGCONF_PARALLEL_STATIONS_EXCEEDS_LIMIT      =   EVENT_DM_BLGCONF_EVENTS + 0x002; ///< Blgconf : '%1' : Maximum Parallel Stations Count value is more than 10
const quint32 EVENT_DM_BLGCONF_SHORTNAME_EXCEEDS_LENGHT             =   EVENT_DM_BLGCONF_EVENTS + 0x003; ///< Blgconf : '%1' : Rule ShortName more than 8 characters
const quint32 EVENT_DM_BLGCONF_LONGNAME_EXCEEDS_LENGHT              =   EVENT_DM_BLGCONF_EVENTS + 0x004; ///< Blgconf : '%1' : Rule Longname more than 50 characters

const quint32 EVENT_DM_USER_SETTINGS_EVENTS
=   EVENT_GROUP_HIMALAYA_DATAMANAGER + 0x500;
const quint32 EVENT_DM_ERROR_SOUND_PERIODIC_TIME_OUT_OF_RANGE
= EVENT_DM_USER_SETTINGS_EVENTS + 0X0001;
const quint32 EVENT_DM_WARN_SOUND_PERIODIC_TIME_OUT_OF_RANGE
= EVENT_DM_USER_SETTINGS_EVENTS + 0X0002;
const quint32 EVENT_DM_INFO_SOUND_PERIODIC_TIME_OUT_OF_RANGE
= EVENT_DM_USER_SETTINGS_EVENTS + 0X0003;
const quint32 EVENT_DM_INFO_SOUND_NUMBER_OUT_OF_RANGE
= EVENT_DM_USER_SETTINGS_EVENTS + 0X0004;
const quint32 EVENT_DM_INFO_SOUND_LEVEL_OUT_OF_RANGE
= EVENT_DM_USER_SETTINGS_EVENTS + 0X0005;
const quint32 EVENT_DM_PARAFFIN_TEMP_OUT_OF_RANGE
= EVENT_DM_USER_SETTINGS_EVENTS + 0X0006;
const quint32 EVENT_DM_RMS_PROCESSING_OUT_OF_RANGE
= EVENT_DM_USER_SETTINGS_EVENTS + 0X0007;
const quint32 EVENT_DM_RMS_CLEANING_OUT_OF_RANGE
= EVENT_DM_USER_SETTINGS_EVENTS + 0X0008;



#endif // DATAMANAGEREVENTCODES_H
