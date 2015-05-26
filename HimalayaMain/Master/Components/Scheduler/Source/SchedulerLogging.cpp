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
    QString Path = Global::SystemPaths::Instance().GetLogfilesPath();

    QString FileName = baseFileName + m_serialNumber + "_" +
            Global::AdjustedTime::Instance().GetCurrentDateTime().toString("yyyyMMdd") + ".log";
    QDir Dir(Path);
    QString CompleteFileName(QDir::cleanPath(Dir.absoluteFilePath(FileName)));
    bool DateLogExist = false;
    if(QFile::exists(CompleteFileName))
    {
        DateLogExist = true;
    }

    mp_DayEventLogger->Configure(DataLogging::DayEventLoggerConfig(operatingMode,
                                                        m_serialNumber,
                                                        m_SWVersion,
                                                        Path,
                                                        fileCount,
                                                        baseFileName));



    if(!DateLogExist)
    {
        QString HeaderString;
        HeaderString += QString("                            T_RV1    T_RV2    T_OvT   T_OvB1   T_OvB2");
        HeaderString += QString("   T_RtB1   T_RtB2   T_RtSd     T_LS    T_Tb1    T_Tb2");
        HeaderString += QString("    Press CurRV CurRt CurLT  Cur3  Cur5 Cur15");
        HeaderString += QString("   RvP RtLk OvLid");
        if(mp_DayEventLogger)
        {
            mp_DayEventLogger->AppendLine(HeaderString, true);
        }
    }
}


void SchedulerLogging::logSensorData(const QString& message)
{
    if (mp_DayEventLogger) {
        if(mp_DayEventLogger->CheckNewFile())
        {
            QString HeaderString;
            HeaderString += QString("                            T_RV1    T_RV2    T_OvT   T_OvB1   T_OvB2");
            HeaderString += QString("   T_RtB1   T_RtB2   T_RtSd     T_LS    T_Tb1    T_Tb2");
            HeaderString += QString("    Press CurRV CurRt CurLT  Cur3  Cur5 Cur15");
            HeaderString += QString("   RvP RtLk OvLid");
            mp_DayEventLogger->AppendLine(HeaderString, true);
        }
        QString timeStamp = mp_DayEventLogger->GetTimeStampHeader();
        mp_DayEventLogger->AppendLine(timeStamp + ";" + message, true);
    }
}


}//end of namespace Scheduler
