/****************************************************************************/
/*! \file ServiceDeviceController/Include/DeviceProcessor/ServiceUtils.h
 *
 *  \brief Definition file for class ServiceUtils.
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

#ifndef CORE_SERVICE_UTILS_H
#define CORE_SERVICE_UTILS_H


#include <Global/Include/GlobalDefines.h>
#include <QTime>

namespace Core {

/****************************************************************************/
class CServiceUtils : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /**
     * \brief Delay function
     * \iparam   MilliSeconds = Seconds in milliSeconds
     */
    /****************************************************************************/
    static void delay(qint32 MilliSeconds);
};

} // end namespace DeviceCommandProcessor

#endif // DEVICECOMMANDPROCESSOR_SERVICE_UTILS_H
