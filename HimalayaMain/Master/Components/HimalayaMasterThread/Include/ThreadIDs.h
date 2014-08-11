/****************************************************************************/
/*! \file ThreadIDs.h
 *
 *  \brief Definition of data logging sources for Colorado.
 *
 *  Version:    0.1
 *  Date:       2013-12-27
 *  Author:     N.Kamath
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN
 *
 *  (C) Copyright 2010 by Leica Biosystems SH CN. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DATALOGGINGSOURCES_H
#define DATALOGGINGSOURCES_H

#include <Global/Include/GlobalDefines.h>

// here all logging sources:


const quint32 THREAD_ID_DEVICECMDPROC  = 0x0101;  ///< Device command processor

const quint32 THREAD_ID_GUI            = 0x0102;  ///< Gui
const quint32 THREAD_ID_SCHEDULER      = 0x0103;  ///< Scheduler

#endif // DATALOGGINGSOURCES_H
