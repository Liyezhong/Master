/****************************************************************************/
/*! \file ServiceUtils.cpp
 *
 *  \brief Implementation file for class ServiceUtils.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-12-24
 *  $Author:    $ Srivathsa HH
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

#include "Core/Include/ServiceUtils.h"
#include <QCoreApplication>

#include <unistd.h>

namespace Core {

/****************************************************************************/
/**
 * \brief Delay function
 * \iparam   MilliSeconds = Seconds in milliSeconds
 */
/****************************************************************************/
void CServiceUtils::delay(qint32 MilliSeconds)
{
    QTime dieTime= QTime::currentTime().addMSecs(MilliSeconds);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

} // end namespace DeviceControl
