/****************************************************************************/
/*! \file SchedulerLogging.cpp
 *
 *  \brief SchedulerLogging class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Oct. 28th, 2014
 *   $Author:  $ lyg
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2014 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Scheduler/Include/SchedulerLogging.h"
#include "Global/Include/SystemPaths.h"

namespace Scheduler{

SchedulerLogging::SchedulerLogging():
    mp_DayEventLogger(NULL),
    m_serialNumber(""),
    m_SWVersion(""),
    m_BufferSize(0)
{
       m_LoggingBuffer.clear();
}

SchedulerLogging::~SchedulerLogging(){}



void SchedulerLogging::InitLog(const QString& serialNumber, const QString& SWVersion)
{
    m_serialNumber = serialNumber;
    m_SWVersion = SWVersion;
    mp_DayEventLogger = new DataLogging::DayEventLogger(0, "DataLogging", "hd");
    QString operatingMode = "production";
    int fileCount = 5;
    QString baseFileName = "SensorData_";
    mp_DayEventLogger->Configure(DataLogging::DayEventLoggerConfig(operatingMode,
                                                        m_serialNumber,
                                                        m_SWVersion,
                                                        Global::SystemPaths::Instance().GetLogfilesPath(),
                                                        fileCount,
                                                        baseFileName));
}


void SchedulerLogging::logSensorData(const QString& message)
{
    if (mp_DayEventLogger) {
        mp_DayEventLogger->CheckNewFile();
        QString timeStamp = mp_DayEventLogger->GetTimeStampHeader();
        mp_DayEventLogger->AppendLine(timeStamp + ";" + message, true);
    }
}


}//end of namespace Scheduler
