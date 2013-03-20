/****************************************************************************/
/*! \file HimalayaErrorHanlder/Include/HimalayaAlarmHandler.h
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

#ifndef HIMALAYAERRORHANLDER_HIMALAYAALARMHANDLER_H
#define HIMALAYAERRORHANLDER_HIMALAYAALARMHANDLER_H

#include <Global/Include/AlarmHandler.h>

namespace Himalaya
{
    class HimalayaMasterThreadController;
}

namespace HimalayaErrorHandler {

/****************************************************************************/
/**
 * \brief Alarm handler for Himalaya application.
 */
/****************************************************************************/
class HimalayaAlarmHandler : public Global::AlarmHandler
{
    Q_OBJECT
public:
    friend class Himalaya::HimalayaMasterThreadController;

    HimalayaAlarmHandler(quint16 timeout, QObject *parent);
    ~HimalayaAlarmHandler();

    /**
      @brief Includes or removes an alarm into the queue of raised alarms
      */
    void setAlarm(quint64 eventKey, Global::AlarmType alarmType, Global::AlarmPosType alarmPosType, bool active);

    /**
      @brief Removes all alarms from alarm queue
      */
    void reset();

    /**
      @brief in non-MasterThreadController, this function should be called
     */
    void emitSetTimeout(quint16 timeout);
    /**
      @brief Specifies the alarm volume related to a specific alarm type
      *1 = lowest
      *6 = normal
      *9 = highest
      */
    void setVolume(Global::AlarmType alarmType, quint8 volume);

    /**
      @brief Specifies the sound file to be played when an alarm of type alarmType is raised
      */
    void setSoundFile(Global::AlarmType alarmType, QString fileName);

    /**
      @brief Specifies the sound-id related to a specific alarm type
      */
    void setSoundNumber(Global::AlarmType alarmType, int number);
    //void emitAlarm( bool AlarmTypeFlag, quint8 Volume, quint8 Sound, bool Active = false, Global::AlarmType alarmType = Global::ALARM_NONE);
    bool playTestTone(bool AlarmTypeFlag, quint8 Volume, quint8 Sound);

protected:
    /**
      @brief Specifies the timeout, i.e. the time interval inbetween two alarm sounds,only MasterThreadController
      */
    void setTimeout(quint16 timeout);

private:
    QHash< quint64, QPair<Global::AlarmType, Global::AlarmPosType> > m_errorList;
    QHash<quint64, QPair<Global::AlarmType, Global::AlarmPosType> > m_warningList;
    QHash<Global::AlarmType, QString> m_soundList;
    QHash<Global::AlarmType, quint8> m_volumeList;
    quint8 m_volume;
    QTimer* m_Timer;
    QMutex* m_mutex;
    QProcess* m_processPlay;
    QProcess* m_processSetVolume;
    QTimer *m_alarmToneTimer;
    //QString m_soundPath;
    void emitAlarm (Global::AlarmType alarmType, bool Active = true, QString Filename = "", quint8 Volume = 1);


private slots:
    void onTimeout();
    void StopPlayAlarm();
signals:
    void sigSetTimeout(quint16 timeout);
    void FireAlarmLocalRemote(bool isLocalAlarm);
}; // end class

} // end namespace HimalayaErrorHandler

#endif // HIMALAYAERRORHANLDER_HIMALAYAALARMHANDLER_H
