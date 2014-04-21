/****************************************************************************/
/*! \file Global.h
 *
 *  \brief Definition file for global variables and methods.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-01-25
 *  $Author:    $ R. Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DATAMANAGER_GLOBAL_H
#define DATAMANAGER_GLOBAL_H



namespace DataManager {

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ReagentStatusType_t
 */
/****************************************************************************/
 typedef enum  {
        PROGRAM_START = 0,
        PROGRAM_PAUSE,
        PROGRAM_ABORT,
        PROGRAM_DRAIN,
        PROGRAM_UNDEFINED
    } ProgramActionType_t;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum LockType_t
 */
/****************************************************************************/
typedef enum  {
    RETORT_LOCK = 0,
    PARAFFIN_BATH_LOCK,
    LOCKTYPE_UNDEFINED
   } LockType_t;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ProgramStatusType_t
 */
/****************************************************************************/
typedef enum  {
        PROGRAM_STATUS_COMPLETED = 0,
        PROGRAM_STATUS_ABORTED,
        PROGRAM_STATUS_PAUSED
   } ProgramStatusType_t;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ProgramAcknownedgeType_t
 */
/****************************************************************************/
typedef enum  {
        PROGRAM_READY = 0,
        PROGRAM_WILL_COMPLETE,
        PROGRAM_CLEANING_FINISHED,

        PROGRAM_RUN_BEGIN,
        PROGRAM_RUN_FINISHED,

        PROGRAM_PAUSE_BEGIN,
        PROGRAM_PAUSE_FINISHED,

        PROGRAM_ABORT_BEGIN,
        PROGRAM_ABORT_FINISHED,
        PROGRAM_SELFTEST_FAILED

    } ProgramAcknownedgeType_t;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum RetortStatusType_t
 */
/****************************************************************************/
typedef enum  {
        RETORT_EMPTYCLEAN = 0,
        RETORT_SCUKING,
        RETORT_DRAINING,
        RETORT_FULL,
        RETORT_CONTAMINATED,
        RETORT_UNDEFINED
    } RetortStatusType_t;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ReagentStatusType_t
 */
/****************************************************************************/
typedef enum  {
        CONTAINER_STATUS_EMPTY = 0,
        CONTAINER_STATUS_SCUKING,
        CONTAINER_STATUS_DRAINING,
        CONTAINER_STATUS_FULL,
        CONTAINER_STATUS_EXPIRED,
        CONTAINER_STATUS_CONTAMINATED,
        CONTAINER_STATUS_NOTUSED
    } ContainerStatusType_t;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ReagentStatusType_t
 */
/****************************************************************************/
typedef enum {
    RESETOPERATIONHOURS_WHOLEMACHINEOPERATION = 0,
    RESETOPERATIONHOURS_ACTIVECARBONFILTER,
    RESETOPERATIONHOURS_UNDEFINED
} ResetOperationHoursType_t;
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ReagentStatusType_t
 */
/****************************************************************************/
typedef enum {
    QUITAPPSHUTDOWNACTIONTYPE_QUITAPP = 0,
    QUITAPPSHUTDOWNACTIONTYPE_SHUTDOWN,
    QUITAPPSHUTDOWNACTIONTYPE_UNDEFINED
} QuitAppShutdownActionType_t;

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ReagentStatusType_t
 */
/****************************************************************************/
typedef enum  {
    REAGENT_STATUS_NOT_IN_STATION = 0, ///< Reagent is newly created, but has not been placed into a station.
    REAGENT_STATUS_NORMAL, ///< Reagent is in a station, and can be used normally.
    REAGENT_STATUS_EXPIRED, ///< Reagent is in a station, but expired.
    REAGENT_STATUS_UNDEFINE ///< Undefined.
} ReagentStatusType_t;


/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function LEICA
 *
 *  \return from LEICA
 */
/****************************************************************************/
const QString LEICA("leica");

/****************************************************************************/
/*!
 *  \brief  Check if the system is now idle,
 *          which means no processing is running.
 */
/****************************************************************************/
bool IsSystemIdle(void);

/****************************************************************************/
/*!
 *  \brief  Set the system idle status.
 */
/****************************************************************************/
void SetSystemIdle(bool idle);



}

#endif // DATAMANAGER_GLOBAL_H
