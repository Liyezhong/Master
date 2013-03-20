/****************************************************************************/
/*! \file DataContainerGlobal.h
 *
 *  \brief Global definition file for Data Manager.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-01-09
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

#ifndef DATAMANAGER_DATACONTAINERGLOBAL_H
#define DATAMANAGER_DATACONTAINERGLOBAL_H

namespace DataManager {

typedef enum
{
    UNDEFINED = -1,
    PROGRAMS,
    REAGENTS,
    STATIONS,
    REAGENT_GROUP,
    USER_SETTINGS
} DataContainerType_t;

}

#endif // DATAMANAGER_DATACONTAINERGLOBAL_H
