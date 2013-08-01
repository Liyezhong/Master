/****************************************************************************/
/*! \file HimalayaErrorHanlder/Source/HimalayaAlarmHandler.cpp
 *
 *  \brief Definition file for class HimalayaAlarmHandler.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-05
 *  $Author:    $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include <HimalayaErrorHandler/Include/HimalayaAlarmHandler.h>
#include <QDebug>
#include <QProcess>

namespace HimalayaErrorHandler {

/****************************************************************************/
HimalayaAlarmHandler::HimalayaAlarmHandler(quint16 timeout, QObject *parent)
    :Global::AlarmHandler(timeout, parent)
    , m_volume(1)
    , m_processPlay(NULL)
    , m_processSetVolume(NULL)
    , m_alarmToneTimer(NULL)
{
    qDebug() << "HimalayaAlarmHandler::emitAlarm" ;
    m_mutex = new QMutex();

    m_Timer = new QTimer(this);
    m_Timer->setInterval(timeout);

    m_alarmToneTimer = new QTimer(this);
    connect(m_alarmToneTimer, SIGNAL(timeout()), this, SLOT(StopPlayAlarm()));

    connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_Timer->start();
}

/****************************************************************************/
HimalayaAlarmHandler::~HimalayaAlarmHandler()
{
    delete m_Timer;
    delete m_mutex;
    delete m_alarmToneTimer;
}

void HimalayaAlarmHandler::onTimeout()
{
    m_mutex->lock();
    m_Timer->stop();

    if (!(m_errorList.size() == 0))
    {
        qDebug() << "HimalayaAlarmHandler::emitAlarm" ;
        emitAlarm(Global::ALARM_ERROR);
    }
    else
    {
        if (!(m_warningList.size() == 0))
        {
            qDebug() << "HimalayaAlarmHandler::emitAlarm" ;
            emitAlarm(Global::ALARM_WARNING);
        }
    }

    bool bHasAlarm = false;
    QHash<quint64, QPair<Global::AlarmType, Global::AlarmPosType> >::iterator i;
    for (i = m_errorList.begin(); i != m_errorList.end(); ++i)
    {
       if(i.value().second == Global::ALARMPOS_LOCAL)
       {
           emit FireAlarmLocalRemote(true);
           bHasAlarm = true;
           break;
       }
       else if(i.value().second == Global::ALARMPOS_REMOTE)//include local Alarm
       {
           emit FireAlarmLocalRemote(true);
           bHasAlarm = true;
           break;
       }
    }

    if (!bHasAlarm)
    {
        for (i = m_warningList.begin(); i != m_warningList.end(); ++i)
        {
            if(i.value().second == Global::ALARMPOS_LOCAL)
            {
                emit FireAlarmLocalRemote(true);
                break;
            }
            else if(i.value().second == Global::ALARMPOS_REMOTE)//include local Alarm
            {
                emit FireAlarmLocalRemote(true);
                break;
            }
        }
    }

    m_mutex->unlock();
    m_Timer->start();
}

void HimalayaAlarmHandler::emitAlarm (Global::AlarmType alarmType, bool Active, QString Filename, quint8 AlarmVolume  )
{
    QString soundFile = ((Active) ? m_soundList.value(alarmType, NULL) : Filename);
    if (soundFile.length() == 0)
    {
        qDebug() << "HimalayaAlarmHandler::emitAlarm" << "No sound file defined for alarm type " << alarmType;
        return;
    }

    double volumeLevel = ((Active) ? (( 0.1 + 0.15*m_volume )*100) : ((0.1 + 0.15*AlarmVolume )*100));   // m_volume=6 => volumeLevel=1

    if (!m_processPlay)
    {
        m_processPlay = new QProcess();
        qDebug() << "Creating a new Process";
    }
    else
    {
        if (m_processPlay->state() == QProcess::Running)
        {
            qDebug() << "Process Running";
            m_processPlay->kill();
            qDebug()<<"Process Killed";
            m_processPlay->waitForFinished();
        }
    }

    qDebug() << "ogg123" + /*QString::number(volumeLevel, 'g', 1) + " " +*/ soundFile;
    QString SetVolume= "amixer sset 'PCM',0 " + QString::number(volumeLevel) +"%";
    m_processSetVolume = new QProcess();
    m_processSetVolume->start(SetVolume);
    QString program = "ogg123 " + /*QString::number(volumeLevel, 'g', 1) + " " +*/ soundFile;
    m_processPlay->start(program);


}
bool HimalayaAlarmHandler::playTestTone(bool AlarmTypeFlag, quint8 AlarmNumber, quint8 AlarmVolume)
{
    if ((!(m_errorList.size() == 0)) || (!(m_warningList.size() == 0)))
    {
        qDebug() << "Process is already running";
        return false;
    }

    else
    {   connect(m_processPlay,SIGNAL(finished(int)),this,SLOT(StopPlayAlarm()));
        m_alarmToneTimer->start(3000);

        quint8 number=AlarmNumber;
        QString FileName = "";

        if ( !AlarmTypeFlag )
        {
            QString FileName = Global::SystemPaths::Instance().GetSoundPath() + "/Alarm" + QString::number(number) + ".ogg";
            qDebug() << FileName;
            emitAlarm(Global::ALARM_ERROR, false, FileName, AlarmVolume);
        }
        else
        {
            QString FileName = Global::SystemPaths::Instance().GetSoundPath() + "/Note" + QString::number(number) + ".ogg";
            qDebug() << FileName;
            emitAlarm(Global::ALARM_WARNING, false, FileName, AlarmVolume);
        }
    }


    return true;
}

void HimalayaAlarmHandler::StopPlayAlarm()
{
    if (!m_processPlay)
        return;
        
    if ( m_processPlay->state() == QProcess::NotRunning)
    {
        qDebug() << "Process completed...";
        m_alarmToneTimer->stop();
        disconnect(m_processPlay,SIGNAL(finished(int)),this,SLOT(StopPlayAlarm()));
    }
    else if (m_processPlay->state() == QProcess::Running)
    {

        qDebug() << "Process Running...";
        m_processPlay->kill();
        qDebug()<<"Process Killed....";
        m_processPlay->waitForFinished();
        m_alarmToneTimer->stop();
        disconnect(m_processPlay,SIGNAL(finished(int)),this,SLOT(StopPlayAlarm()));

    }

//    m_alarmToneTimer = NULL;


}

void HimalayaAlarmHandler::setTimeout(quint16 timeout)
{
    m_Timer->stop();
    m_mutex->lock();

    m_Timer->setInterval(timeout);

    m_mutex->unlock();
    m_Timer->start();
}

void HimalayaAlarmHandler::emitSetTimeout(quint16 timeout)
{
    emit sigSetTimeout(timeout);
}

void HimalayaAlarmHandler::setVolume(Global::AlarmType alarmType, quint8 volume)
{
    m_mutex->lock();
    m_volumeList.insert(alarmType,volume);
    m_mutex->unlock();
}

void HimalayaAlarmHandler::setSoundNumber(Global::AlarmType alarmType, int number)
{
    //Global::SystemPaths SysPath;
    Q_UNUSED(number);
    QString fileName = "";

    if (alarmType == Global::ALARM_ERROR)
    {
        fileName =  Global::SystemPaths::Instance().GetSoundPath() + "/Alarm" + QString::number(number) + ".ogg";
    }
    else
    {
        fileName =  Global::SystemPaths::Instance().GetSoundPath() + "/Note" + QString::number(number) + ".ogg";
    }
    if (fileName.length() > 0)
    {
        setSoundFile(alarmType, fileName);
        qDebug() << "HimalayaAlarmHandler SoundFile is" << fileName;
    }

}

void HimalayaAlarmHandler::setSoundFile(Global::AlarmType alarmType, QString fileName)
{
    if (fileName.length() == 0)
    {
        qDebug() << "HimalayaAlarmHandler::setSoundFile, no file specified";
        return;
    }

    m_mutex->lock();
    m_soundList.insert(alarmType, fileName);
    m_mutex->unlock();
}

void HimalayaAlarmHandler::setAlarm(quint64 eventKey, Global::AlarmType alarmType, Global::AlarmPosType alarmPosType, bool active)
{
    m_mutex->lock();
    if (active)
    {
        if (alarmType == Global::ALARM_ERROR)
        {
           m_errorList.insert(eventKey,  qMakePair(alarmType, alarmPosType));
        }
        else if (alarmType == Global::ALARM_WARNING)
        {
          m_warningList.insert(eventKey,  qMakePair(alarmType, alarmPosType));
        }

    }
    else
    {
        if (alarmType == Global::ALARM_ERROR)
        {
            m_errorList.remove(eventKey);
        }
        else if (alarmType == Global::ALARM_WARNING)
        {
           m_warningList.remove(eventKey);
        }

    }
    m_mutex->unlock();
}

void HimalayaAlarmHandler::reset()
{
    m_mutex->lock();
    m_errorList.clear();
    m_mutex->unlock();
}

} // end namespace HimalayaErrorHandler
