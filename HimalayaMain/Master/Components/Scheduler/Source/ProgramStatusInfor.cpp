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
#include <QDebug>
namespace Scheduler{

#define TIME_1_HOUR (1 * 60 * 60 * 1000)
#define TIME_12_HOURS (12 * 60 * 60 * 1000)
#define TIME_15_HOURS (15 * 60 * 60 * 1000)
#define DATE_TIME_FORMAT "yyyyMMdd-hhmmss"

CProgramStatusInfor::CProgramStatusInfor()
{
    m_LastHeatingOn = false;
    m_LastTimeUpdateHeatingTime = 0;
    m_MaxMeltTime = TIME_15_HOURS;
    m_RemainTime = m_MaxMeltTime;
}

CProgramStatusInfor::~CProgramStatusInfor()
{
    /*lint -e1551 */
    m_Status.clear();
    m_StatusFile.close();
}

bool CProgramStatusInfor::InitProgramStatus(quint32 ParaffinMeltPoint)
{
   quint64 HeatingTime = 0;
   quint64 UnHeatingTime = 0;
   bool ret = true;
   m_MaxMeltTime = TIME_15_HOURS;
   if(ParaffinMeltPoint < 64)
   {
       m_MaxMeltTime = TIME_12_HOURS;
   }
   if(CalculateTime(HeatingTime,UnHeatingTime))
   {
       if(UnHeatingTime > 4 * TIME_1_HOUR)
       {
           m_RemainTime = m_MaxMeltTime;
       }
       else
       {
           if(m_MaxMeltTime - HeatingTime + 2 * UnHeatingTime + TIME_1_HOUR > m_MaxMeltTime)
           {
               m_RemainTime = m_MaxMeltTime;
           }
           else
           {
               m_RemainTime = 2 * UnHeatingTime + TIME_1_HOUR;
           }
       }
   }
   else
   {
       ret = false;
   }

   m_Status.insert("HeatingOvenSlice","");
   SetStatus("HeatingOvenSlice","");

   if(QFile::exists("TEST_ISSAC")) // only for testing time of paraffin
   {
       QFile Test("TEST_ISSAC");
       if (Test.open(QIODevice::ReadOnly | QIODevice::Text))
       {
           QTextStream in(&Test);
           if(!in.atEnd())
           {
               QString line = in.readLine();
               bool ok = false;
               quint32 time = line.toUInt(&ok);
               if(ok && time > 0)
               {
                   m_MaxMeltTime = time * 60 * 1000;
                   m_RemainTime = m_MaxMeltTime;
               }
           }
           Test.close();
       }
   }
   return ret;
}

void CProgramStatusInfor::ResetOvenHeatingTime(quint32 ParaffinMeltPoint, bool Reset)
{
    m_MaxMeltTime = TIME_15_HOURS;
    if(ParaffinMeltPoint < 64)
    {
        m_MaxMeltTime = TIME_12_HOURS;
    }
    if(Reset)
    {
        m_Status.insert("HeatingOvenSlice","");
        SetStatus("HeatingOvenSlice","");
    }

    if(QFile::exists("TEST_ISSAC")) // only for testing time of paraffin
    {
        QFile Test("TEST_ISSAC");
        if (Test.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&Test);
            if(!in.atEnd())
            {
                QString line = in.readLine();
                bool ok = false;
                quint32 time = line.toUInt(&ok);
                if(ok && time > 0)
                {
                    m_MaxMeltTime = time * 60 * 1000;
                }
            }
            Test.close();
        }
    }
    m_RemainTime = m_MaxMeltTime;
}

void CProgramStatusInfor::SetLastRVPosition(const DeviceControl::RVPosition_t& rvpos)
{
    /*lint -e641 */
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


void CProgramStatusInfor::SetLastReagentGroup(QString rgb)
{
    SetStatus("LastReagentGroup",rgb);
}

QString CProgramStatusInfor::GetLastReagentGroup()
{
    return GetStatus("LastReagentGroup");
}
void CProgramStatusInfor::SetProgramID(const QString& ProgramID)
{
    SetStatus("ProgramID",ProgramID);
}

QString CProgramStatusInfor::GetProgramId()
{
    return GetStatus("ProgramID");
}

void CProgramStatusInfor::SetStepID(int StepID)
{
     SetStatus("ProgramStepID",QString::number(StepID));
}

int CProgramStatusInfor::GetStepID()
{
    bool ok = false;
    int step = -1;
    if(!GetStatus("ProgramStepID").isEmpty())
    {
        step = GetStatus("ProgramStepID").toInt(&ok);
    }
    return step;
}

void CProgramStatusInfor::SetErrorFlag(int ErrFlag)
{
    SetStatus("ErrorFlag", QString::number(ErrFlag));
}

int CProgramStatusInfor::GetErrorFlag()
{
    bool ok = false;
    int flag = 0;
    if(!GetStatus("ErrorFlag").isEmpty())
    {
        flag = GetStatus("ErrorFlag").toInt(&ok);
    }
    return flag;
}

void CProgramStatusInfor::SetStationList(const QList<QString>&  List)
{
    QString Value = "";
    QList<QString>::const_iterator ite = List.constBegin();
    for(;ite != List.constEnd();ite++)
    {
        Value += (*ite + ",");
    }
    (void)Value.remove(Value.length() - 1);
    SetStatus("StationList",Value);
}

QList<QString> CProgramStatusInfor::GetStationList()
{
    QString Value = GetStatus("StationList");
    QList<QString> List;
    QStringList StrList = Value.split(",");
    for(int i = 0; i < StrList.length(); i++)
    {
        List.push_back(StrList.at(i));
    }
    return List;
}

void CProgramStatusInfor::SetScenario(const quint32 Scenario)
{
    SetStatus("Scenario", QString::number(Scenario));
}

quint32 CProgramStatusInfor::GetScenario()
{
    QString value = GetStatus("Scenario");
    bool ok = false;
    quint32 scen = 0;
    if(!value.isEmpty())
    {
        scen = value.toUInt(&ok);
    }
    if(!ok)
    {
        scen = 0;
    }
    return scen;
}

void CProgramStatusInfor::SetPressureDriftOffset(qreal PressureDriftOffset)
{
    SetStatus("LastPressureDriftOffset", QString::number(PressureDriftOffset));
}

qreal CProgramStatusInfor::GetPressureDriftOffset()
{
    QString value = GetStatus("LastPressureDriftOffset");
    bool ok = false;
    qreal pre = 0.0;
    if(!value.isEmpty())
    {
        pre = value.toFloat(&ok);
    }
    if(!ok)
    {
        pre = 0.0;
    }
    return pre;
}

bool CProgramStatusInfor::IsProgramFinished()
{
    quint32 scenario = GetScenario();
    return  (GetProgramId().isEmpty() ||
            (GetStepID() < 0) ||
            scenario <= 4);
}

void CProgramStatusInfor::SetProgramFinished()
{
    SetLastReagentGroup("");
    SetProgramID("");
}

quint64 CProgramStatusInfor::GetOvenHeatingTime()
{
    if(m_MaxMeltTime <= m_RemainTime)
    {
        return 0;
    }
    else
    {
        return m_MaxMeltTime - m_RemainTime;
    }
}

quint64 CProgramStatusInfor::GetRemaingTimeForMeltingParffin()
{
    return m_RemainTime;
}


void CProgramStatusInfor::UpdateOvenHeatingTime(quint64 Time, bool IsHeatingOn)
{
    if(((Time - m_LastTimeUpdateHeatingTime) < 60 * 1000)
            && (IsHeatingOn == m_LastHeatingOn)) // record by one minutes
    {
        return;
    }
    QString key="HeatingOvenSlice";
    QString value = m_Status.value(key);
    QStringList Slices;
    if(!value.isEmpty())
    {
        Slices = value.split(",");
    }
    if(!IsHeatingOn)
    {
        m_RemainTime = m_MaxMeltTime;
    }
    if(Slices.length() == 0 || (!m_LastHeatingOn && IsHeatingOn)) // if 1. first boot; 2. the heating status from OFF to ON.
    {

        Slices.append(QDateTime::fromMSecsSinceEpoch(Time).toString(DATE_TIME_FORMAT));
        Slices.append(QDateTime::fromMSecsSinceEpoch(Time).toString(DATE_TIME_FORMAT));
        m_LastTimeUpdateHeatingTime = Time;
        SetStatus(key, Slices.join(","));
    }
    else if(m_LastHeatingOn && IsHeatingOn) // if the heating status from ON to ON
    {
        Slices.replace(Slices.length() - 1,QDateTime::fromMSecsSinceEpoch(Time).toString(DATE_TIME_FORMAT));
        if(m_LastTimeUpdateHeatingTime != 0)
        {
            if(m_RemainTime > (Time - m_LastTimeUpdateHeatingTime))
            {
                m_RemainTime -= (Time - m_LastTimeUpdateHeatingTime);
            }
            else
            {
                m_RemainTime = 0;
            }
        }
        m_LastTimeUpdateHeatingTime = Time;
        SetStatus(key, Slices.join(","));
    }
    m_LastHeatingOn = IsHeatingOn;
}

bool CProgramStatusInfor::CalculateTime(quint64& HeatingTime, quint64& UnHeatingTime)
{
    /*lint -e550 */
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
       (void)m_Status.insert(Line.split(":").at(0),Line.split(":").at(1));
       Line = FileStream.readLine().simplified();
    }
   m_StatusFile.close();

    quint64 TimeLimit = QDateTime::currentMSecsSinceEpoch() - m_MaxMeltTime;
    QString value = m_Status.value("HeatingOvenSlice");
    QStringList Slices;
    if(!value.isEmpty())
    {
        Slices = value.split(",");
    }
    //bool ok = false;
    quint64 Start = 0;
    quint64 End = 0;
    for(int i = 0; i < Slices.length() / 2;)
    {
        Start = QDateTime::fromString(Slices.at(2 * i),DATE_TIME_FORMAT).toMSecsSinceEpoch();
        End = QDateTime::fromString(Slices.at(2 * i + 1),DATE_TIME_FORMAT).toMSecsSinceEpoch();
        if(End < TimeLimit)
        {
            Slices.removeAt(2 * i);
            Slices.removeAt(2 * i);
            Start = End = TimeLimit;
        }
        else if(End >= TimeLimit && Start < TimeLimit)
        {
            Slices.replace(2 * i ,QDateTime::fromMSecsSinceEpoch(TimeLimit).toString(DATE_TIME_FORMAT));;
            HeatingTime += (End - TimeLimit);
            Start = TimeLimit;
            i++;
        }
        else
        {
            HeatingTime += (End - Start);
            if(i > 0)
            {
                UnHeatingTime += (Start - QDateTime::fromString(Slices.at(2 * i - 1),DATE_TIME_FORMAT).toMSecsSinceEpoch());
            }
            i++;
        }
    }
    if(Start == 0 || End == 0) // empty value for HeatingOvenSlice
    {
        UnHeatingTime = m_MaxMeltTime;
        HeatingTime = 0;
    }
    else
    {
      UnHeatingTime += (QDateTime::currentMSecsSinceEpoch() - End);
    }
    return true;
}

bool CProgramStatusInfor::IsRetortContaminted()
{
    QString ReagentGroup = GetLastReagentGroup();
    return !ReagentGroup.isEmpty();
}
void CProgramStatusInfor::SetStatus(const QString& key, const QString& value)
{
    QString Cmd = "";
    if(m_Status.contains(key))
    {
        Cmd = QString("sed -i s#^%1:.*#%1:%2# %3").arg(key).arg(value).arg("../Settings/ProgramStatus.txt");
    }
    else
    {
        Cmd = QString("echo %1:%2 >> %3").arg(key).arg(value).arg("../Settings/ProgramStatus.txt");
    }
//    FlushFile();
    system(Cmd.toStdString().c_str());
    (void)m_Status.insert(key,value);
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
        (void)StatusfileItr.next();
        QString Key1 = StatusfileItr.key();
        QString Value1 = m_Status.value(Key1);
        FileStream << Key1 << ":" << Value1 << "\n" << left;
    }
    (void)m_StatusFile.flush();
    (void)fsync(m_StatusFile.handle());
    m_StatusFile.close();
}
} //end of scheduler name space
