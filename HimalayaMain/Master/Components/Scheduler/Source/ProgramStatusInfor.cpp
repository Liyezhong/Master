/****************************************************************************/
/*! \file ProgramStatusInfor.cpp
 *
 *  \brief ProgramStatusInfor class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Aug 18th, 2014
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

#include <QTextStream>
#include <QStringList>
#include <QDateTime>
#include "Scheduler/Include/ProgramStatusInfor.h"
#include <unistd.h>

CProgramStatusInfor::CProgramStatusInfor()
{
}

CProgramStatusInfor::~CProgramStatusInfor()
{
    m_Status.clear();
    m_StatusFile.close();
}

bool CProgramStatusInfor::ReadProgramStatusFile()
{
    if(!m_StatusFile.isOpen())
    {
        m_StatusFile.setFileName("../Settings/ProgramStatus.txt");
        if(!m_StatusFile.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            return false;
        }
    }
   QTextStream FileStream(&m_StatusFile);
   QString Line = FileStream.readLine().simplified();
   m_Status.clear();
   while(!Line.isNull())
   {
       m_Status.insert(Line.split(":").at(0),Line.split(":").at(1));
       Line = FileStream.readLine().simplified();
    }
   m_StatusFile.close();
   return true;
}

void CProgramStatusInfor::SetLastRVPosition(const DeviceControl::RVPosition_t& rvpos)
{
    SetStatus("LastRVPosition",QString::number(rvpos));
}

DeviceControl::RVPosition_t CProgramStatusInfor::GetLastRVPosition()
{
    QString value = GetStatus("LastRVPosition");
    bool ok = false;
    DeviceControl::RVPosition_t pos =  (DeviceControl::RVPosition_t)(value.toUInt(&ok));
    if(!ok)
    {
        pos = DeviceControl::RV_UNDEF;
    }
    return pos;
}

void CProgramStatusInfor::SetProgramID(const QString& ProgramID)
{
    SetStatus("ProgramID",ProgramID);
}

QString CProgramStatusInfor::GetProgramId()
{
    return GetStatus("ProgramID");
}

void CProgramStatusInfor::SetStepID(const QString& StepID)
{
     SetStatus("ProgramStepID",StepID);
}

QString CProgramStatusInfor::GetStepID()
{
    return GetStatus("ProgramStepID");
}

void CProgramStatusInfor::SetScenario(const quint32 Scenario)
{
    SetStatus("Scenario", QString::number(Scenario));
}

quint32 CProgramStatusInfor::GetScenario()
{
    QString value = GetStatus("Scenario");
    bool ok = false;
    quint32 scen =  value.toUInt(&ok);
    if(!ok)
    {
        scen = 0;
    }
    return scen;
}

bool CProgramStatusInfor::IsProgramFinished()
{
    return GetProgramId().isEmpty();
}

void CProgramStatusInfor::SetProgramFinished()
{
    SetStatus("ProgramID","");
}

quint64 CProgramStatusInfor::GetOvenHeatingTime(quint32 ParaffinMeltingPoint)
{
    quint64 TimeLimit = 12 * 60 * 60 * 1000;
    quint64 HeatingTime = 0;
    if(ParaffinMeltingPoint >= 64)
    {
        TimeLimit = 15 * 60 * 60 * 1000;
    }
    TimeLimit = QDateTime::currentMSecsSinceEpoch() - TimeLimit;
    QString value = m_Status.value("HeatingOvenSlice");
    QStringList Slices = value.split(",");
    bool ok = false;
    quint64 Start = 0;
    quint64 End = 0;
    for(int i = 0; i < Slices.length() / 2;)
    {
        Start = Slices.at(2 * i).toULongLong(&ok);
        End = Slices.at(2 * i + 1).toULongLong(&ok);
        if(End < TimeLimit)
        {
            Slices.removeAt(2 * i);
            Slices.removeAt(2 * i + 1);
        }
        else if(End >= TimeLimit && Start < TimeLimit)
        {
            Slices.replace(2 * i ,QString::number(TimeLimit));
            HeatingTime += End - TimeLimit;
            i++;
        }
        else
        {
            HeatingTime += End - Start;
            i++;
        }
    }
    SetStatus("HeatingOvenSlice", Slices.join(","));
    return HeatingTime;
}

void CProgramStatusInfor::UpdateOvenHeatingTime(quint64 Time, bool StartFlag, bool ResetFlag)
{
    QString key="HeatingOvenSlice";
    QString value = m_Status.value(key);
    QStringList Slices;
    if(!value.isEmpty())
    {
        Slices = value.split(",");
    }
    if(ResetFlag) // reset oven heating to 0
    {
        Slices.clear();
    }
    if(StartFlag)
    {
        Slices.append(QString::number(Time));
        Slices.append(QString::number(Time));
    }
    else
    {
        Slices.replace(Slices.length() - 1,QString::number(Time));
    }
    SetStatus(key, Slices.join(","));
}

bool CProgramStatusInfor::IsRetortContaminted()
{
    return true;
}
void CProgramStatusInfor::SetStatus(const QString& key, const QString& value)
{
    m_Status.insert(key,value);
    FlushFile();
}

QString CProgramStatusInfor::GetStatus(const QString &key)
{
    return m_Status.value(key);
}

void CProgramStatusInfor::FlushFile()
{
    if(!m_StatusFile.isOpen())
    {
        m_StatusFile.setFileName("../Settings/ProgramStatus.txt");
        if(!m_StatusFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
        {
            return;
        }
    }
    QTextStream FileStream(&m_StatusFile);
    FileStream.setFieldAlignment(QTextStream::AlignLeft);

    QMapIterator<QString, QString> StatusfileItr(m_Status);
    while (StatusfileItr.hasNext()) {
        StatusfileItr.next();
        QString Key1 = StatusfileItr.key();
        QString Value1 = m_Status.value(Key1);
        FileStream << Key1 << ":" << Value1 << "\n" << left;
    }
    m_StatusFile.flush();
    fsync(m_StatusFile.handle());
    m_StatusFile.close();
}
