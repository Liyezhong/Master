/****************************************************************************/
/*! \file Include/DataLoggingSources.h
 *
 *  \brief Definition of data logging sources for Himalaya.
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

#ifndef DATALOGGINGSOURCES_H
#define DATALOGGINGSOURCES_H

#include <../Include/PlatformEventCodes.h>
#include <Global/Include/GlobalDefines.h>

// here all logging sources:
const Global::gSourceType HEARTBEAT_SOURCE_MASTERTHREAD   = EVENT_GROUP_SOURCENAME + 0x0001;  ///< Himalaya master thread
const Global::gSourceType HEARTBEAT_SOURCE_DATALOGGING    = EVENT_GROUP_SOURCENAME + 0x0002;  ///< Data logging
const Global::gSourceType HEARTBEAT_SOURCE_EVENTHANDLER   = EVENT_GROUP_SOURCENAME + 0x0003;  ///< Event handler
const Global::gSourceType HEARTBEAT_SOURCE_DEVICECMDPROC  = EVENT_GROUP_SOURCENAME + 0x0004;  ///< Device command processor
const Global::gSourceType HEARTBEAT_SOURCE_AXEDA          = EVENT_GROUP_SOURCENAME + 0x0005;  ///< Axeda
const Global::gSourceType HEARTBEAT_SOURCE_GUI            = EVENT_GROUP_SOURCENAME + 0x0006;  ///< Gui
const Global::gSourceType HEARTBEAT_SOURCE_SCHEDULER      = EVENT_GROUP_SOURCENAME + 0x0007;  ///< Scheduler
const Global::gSourceType HEARTBEAT_SOURCE_IMPORTEXPORT   = EVENT_GROUP_SOURCENAME + 0x0008;  ///< ImportExport Thread
const Global::gSourceType HEARTBEAT_SOURCE_EXPORT         = EVENT_GROUP_SOURCENAME + 0x0009;  ///< Export
const Global::gSourceType HEARTBEAT_SOURCE_SEPIA          = EVENT_GROUP_SOURCENAME + 0x000A;  ///< Export
const Global::gSourceType HEARTBEAT_SOURCE_BLG            = EVENT_GROUP_SOURCENAME + 0x000B;  ///< BLG
const Global::gSourceType HEARTBEAT_SOURCE_SCHEDULER_MAIN = EVENT_GROUP_SOURCENAME + 0x000C;  ///< Scheduler
#endif // DATALOGGINGSOURCES_H
