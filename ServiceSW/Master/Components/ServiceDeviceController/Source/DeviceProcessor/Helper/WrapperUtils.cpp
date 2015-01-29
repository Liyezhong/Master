/****************************************************************************/
/*! \file WrapperUtils.cpp
 *
 *  \brief  Implementation file for class WrapperUtils
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
#include <QTimer>
#include <QEventLoop>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperUtils.h"
#include <QProcess>
#include "Global/Include/SystemPaths.h"
#include <QtDebug>

//lint -e1039

/****************************************************************************/
/*!
 *   \brief Script-API: pause for a timespan given in ms
 *
 *   pause for 2 seconds: utils.Pause(2000)
 *
 *  \iparam   MilliSeconds   timespan in ms
 *
 */
/****************************************************************************/
void WrapperUtils::Pause(quint32 MilliSeconds)
{
#ifdef PRE_ALFA_TEST
    Log(tr(">>>"));
#else
    Log(tr("Pausing for %1 ms...").arg(MilliSeconds));
#endif

    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(MilliSeconds);
    timer.start();
    QEventLoop loop;
    (void)connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    (void)loop.exec();
}

void WrapperUtils::AbortPause()
{
//    m_loop.exit(1);
}

/****************************************************************************/
/*!  \brief  Script-API: stop script engine (doesn't work with Qt 4.6.x)
 */
/****************************************************************************/
void WrapperUtils::Stop()
{
    emit StopCalled();
}

/****************************************************************************/
/*!
 *   \brief Script-API: sends a signal "TestSignal" for test purposes
 *
 *  \iparam   value   integer parameter for the emitted signal
 *
 */
/****************************************************************************/
void WrapperUtils::SendTestSignal(qint32 value)
{
    Log(tr("Sending value %1.").arg(value));
    emit TestSignal(value);
}

void WrapperUtils::SetMonitorT(quint32 MilliSeconds)
{
    emit OnMonitorT(MilliSeconds);
}

void WrapperUtils::PlaySound(quint8 SoundType)
{

    QProcess Proc;
    QString ProcName = "aplay";
    QString SndFile;
    QStringList arguments;

    switch(SoundType)
    {
    case 0:
    {
        SndFile = Global::SystemPaths::Instance().GetSettingsPath() + "/Error.wav";
        arguments << SndFile;
    }
        break;
    case 1:
    {
        SndFile = Global::SystemPaths::Instance().GetSettingsPath() + "/Ok.wav";
        arguments << SndFile;
    }
        break;
    default:
        return;
    }

    Proc.start(ProcName, arguments );

    (void)Proc.waitForFinished();
}

void WrapperUtils::LogToFile(const QString &FolderName,  const QString &FileName, bool Clear)
{
    if((Clear)&&((FileName == "NA")||(FolderName == "NA")))
    {
        emit OnClearLogUtils();
    }
    else
    {
        if((FolderName.length() != 0)&&(FileName.length() != 0))
        {
            QString Path = QCoreApplication::applicationDirPath();
            //Log(tr("current path...:%1").arg(Path));
            Path += "/"+FolderName;
            QString BashCmd="mkdir "+ Path;
            (void)system(BashCmd.toStdString().c_str());

            //QString FileName = FolderName + "/Log_" + QDateTime::currentDateTime().toString("MM_dd_hh_mm_ss");
            QString FullName = Path + "/" + FileName;
            emit OnWriteLogToFileUtils(FullName);
            if(Clear)
            {
                emit OnClearLogUtils();
            }
        }
        else
        {
            Log(tr("You input empty folder name, please re-enter"));
        }
    }
}

void WrapperUtils::SwitchMonitor(bool Enable)
{
    emit OnSwitchMonitor(Enable);
}

quint32 WrapperUtils::ReadLoop(quint32 loopIndex)
{
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/LoopCounter.txt";
    quint32 succeedCounter = 0;
    quint32 failedCounter = 0;
    FILE* pFile;
    if ( (pFile = fopen(FileName.toStdString().c_str(), "r")) == NULL)
    {
        //Log(tr("Cannot open LoopCounter.txt.\n"));
        return 0;
    }

    char Buf[200];
    memset(Buf, 0, sizeof(Buf));
    if(fread(Buf, 1, 200, pFile) > 0 )
    {
        QString Content = QString::fromAscii(Buf, -1);
        QStringList StrList = Content.split(";");
        if(StrList.size() >= 2)
        {
            succeedCounter = StrList.at(0).toInt();
            failedCounter = StrList.at(1).toInt();
        }
        QString msg = tr("%1;%2;\n").arg(succeedCounter).arg(failedCounter);
        //Log(msg);
    }
    fclose(pFile);
    if(loopIndex == 0)
    {
        return succeedCounter;
    }
    else
    {
        return failedCounter;
    }
}

void WrapperUtils::WriteLoop(quint32 loopIndex, quint32 loopCounter)
{
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/LoopCounter.txt";
    quint32 succeedCounter = 0;
    quint32 failedCounter = 0;
    if(loopIndex == 0)
    {
        succeedCounter = loopCounter;
        failedCounter = ReadLoop(1);
    }
    else
    {
        succeedCounter = ReadLoop(0);
        failedCounter = loopCounter;
    }
    QString msg = tr("%1;%2;\n").arg(succeedCounter).arg(failedCounter);
    FILE* pFile = fopen(FileName.toStdString().c_str(), "w+");
    if (pFile) {
        fprintf(pFile, "%s", msg.toStdString().c_str());
        (void)fflush(pFile);
        fclose(pFile);
    }
    (void)system("sync");
}



// vi: set ts=4 sw=4 et:

