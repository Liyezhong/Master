/****************************************************************************/
/*! \file SchedulerLogging.h
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
#ifndef SCHEDULERLOGGING_H
#define SCHEDULERLOGGING_H
#include <DataLogging/Include/DayEventLogger.h>
namespace Scheduler{


/****************************************************************************/
/**
 * \brief This class logs all the logging messages i.e. scheduling results.
 */
/****************************************************************************/
class SchedulerLogging
{
public:
    /****************************************************************************/
    /**
     * \brief
     *
     * \return logInstance
     */
    /****************************************************************************/
    static SchedulerLogging& getInstance()
    {
        static SchedulerLogging logInstance;
        return logInstance;
    }

    /****************************************************************************/
    /**
     * Destructor
     */
    /****************************************************************************/
    ~SchedulerLogging();


//    void LogThreadId(QString threadName, long lwp);

    /****************************************************************************/
    /**
     * \brief   Initializes logging process
     *
     * \param   serialNumber - the serial number of Instrument
     * \param   SWVersion - software version
     */
    /****************************************************************************/
    void InitLog(const QString& serialNumber,const QString& SWVersion);

    /****************************************************************************/
    /**
     * \brief log sensor data
     *
     * \param message string of sensor value.
     *
     */
    /****************************************************************************/
    void logSensorData(const QString& message);


private:
    /****************************************************************************/
    /**
     * \brief Default Constructor
     */
    /****************************************************************************/
    SchedulerLogging();
    /****************************************************************************/
    /**
     * \brief Copy Constructor
     *
     * \iparam rOther
     */
    /****************************************************************************/
    SchedulerLogging(SchedulerLogging const& rOther);
    /****************************************************************************/
    /**
     * \brief Assignment Operator
     *
     * \iparam rhs
     */
    /****************************************************************************/
    void operator=(SchedulerLogging const& rhs);
    DataLogging::DayEventLogger *mp_DayEventLogger;
    QString m_serialNumber;
    QString m_SWVersion;
    QStringList m_LoggingBuffer;
    quint32 m_BufferSize;

};

}//end of namespace Scheduler

#endif
