/****************************************************************************/
/*! \file WrapperUtils.h
 *
 *  \brief  Definition file for class WrapperUtils
 *
 *  \version  0.1
 *  \date     2012-03-23
 *  \author   T.Scheck
 *
 *  \b Description:
 *
 *   N/A
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2012 by Leica Biosystems Nussloch GmbH. All rights reserved
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 */
/****************************************************************************/

#ifndef WRAPPERUTILS_H
#define WRAPPERUTILS_H

#include <QObject>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperBase.h"


/****************************************************************************/
/*!  \brief Class which collects utility functionality for the Script-API.
 *
 */
/****************************************************************************/
class WrapperUtils : public WrapperBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!  \brief Constructor
     *
     *  \iparam   Parent   Optional pointer to parent for QObject Hierarchy.
     *
     */
    /****************************************************************************/
    inline WrapperUtils(QObject *Parent): WrapperBase("Script", Parent) {}

public slots:
    void Pause(quint32 MilliSeconds);
    void AbortPause();
    void Stop();
    void SendTestSignal(qint32 value);
#ifdef PRE_ALFA_TEST
    void SetMonitorT(quint32 MilliSeconds);
    void PlaySound(quint8 SoundType);
    void LogToFile(const QString &FolderName,  const QString &FileName, bool Clear);
    void SwitchMonitor(bool Enable);
    quint32 ReadLoop(quint32 loopIndex);
    void WriteLoop(quint32 loopIndex, quint32 loopCounter);

#endif

    /****************************************************************************/
    /*!  \brief Get wrapper API version
     *
     *   \return String with version number
     */
    /****************************************************************************/
    QString GetVersion() { return "0.1"; }


signals:
    /****************************************************************************/
    /*!
     *   \brief  Signal: send stop command to ScriptEngine.
     */
    /****************************************************************************/
    void StopCalled();

    /****************************************************************************/
    /*!
     *   \brief  Signal: TestSignal for tests with QtScript slots.
     */
    /****************************************************************************/
    void TestSignal(qint32);
#ifdef PRE_ALFA_TEST
    void OnMonitorT(quint32);
    void OnClearLogUtils(void);
    void OnWriteLogToFileUtils(QString FileName);
    void OnSwitchMonitor(bool Enable);
#endif

};

#endif

// vi: set ts=4 sw=4 et:

