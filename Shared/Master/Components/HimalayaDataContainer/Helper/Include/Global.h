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

#include <QString>


namespace DataManager {


 typedef enum  {
        PROGRAM_START = 0,
        PROGRAM_PAUSE,
        PROGRAM_ABORT
    } ProgramActionType_t;
    
typedef enum  {
        RETORT_EMPTYCLEAN = 0,
        RETORT_SCUKING,
        RETORT_DRAINING,
        RETORT_FULL,
        RETORT_CONTAMINATED
    } RetortStatusType_t;
    
typedef enum  {
        PARAFFINBATH_EMPTY = 0,
        PARAFFINBATH_SCUKING,
        PARAFFINBATH_DRAINING,
        PARAFFINBATH_FULL,
        PARAFFINBATH_EXPIRED,
        PARAFFINBATH_NOTUSED
    } StationParaffinBathStatusType_t;
    

const QString LEICA("leica");

/****************************************************************************/
/*
 *  \brief  Check if the system is now idle,
 *          which means no processing is running.
 */
/****************************************************************************/
bool IsSystemIdle(void);

/****************************************************************************/
/*
 *  \brief  Set the system idle status.
 */
/****************************************************************************/
void SetSystemIdle(bool idle);



}

#endif // DATAMANAGER_GLOBAL_H
