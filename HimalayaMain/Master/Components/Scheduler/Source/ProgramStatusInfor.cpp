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

CProgramStatusInfor::CProgramStatusInfor()
{
    m_LastHeatingOn = false;
    m_LastTimeUpdateHeatingTime = 0;
}

CProgramStatusInfor::~CProgramStatusInfor()
{
    /*lint -e1551 */
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
       (void)m_Status.insert(Line.split(":").at(0),Line.split(":").at(1));
       Line = FileStream.readLine().simplified();
    }
   m_StatusFile.close();
   return true;
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
}

quint64 CProgramStatusInfor::GetOvenHeatingTime(quint32 ParaffinMeltingPoint)
{
    /*lint -e550 */
    quint64 TimeLimit = 12 * 60 * 60 * 1000;
    quint64 HeatingTime = 0;
    quint64 UnHeatingTime = 0;
    if(ParaffinMeltingPoint >= 64)
    {
        TimeLimit = 15 * 60 * 60 * 1000;
    }
    TimeLimit = QDateTime::currentMSecsSinceEpoch() - TimeLimit;
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
        Start = QDateTime::fromString(Slices.at(2 * i),"yyyyMMdd-hhmmss").toMSecsSinceEpoch();
        End = QDateTime::fromString(Slices.at(2 * i + 1),"yyyyMMdd-hhmmss").toMSecsSinceEpoch();
        if(End < TimeLimit)
        {
            Slices.removeAt(2 * i);
            Slices.removeAt(2 * i);
            Start = End = TimeLimit;
        }
        else if(End >= TimeLimit && Start < TimeLimit)
        {
            Slices.replace(2 * i ,QDateTime::fromMSecsSinceEpoch(TimeLimit).toString("yyyyMMdd-hhmmss"));;
            HeatingTime += (End - TimeLimit);
            Start = TimeLimit;
            i++;
        }
        else
        {
            HeatingTime += (End - Start);
            if(i > 0)
            {
                UnHeatingTime += (Start - QDateTime::fromString(Slices.at(2 * i - 1),"yyyyMMdd-hhmmss").toMSecsSinceEpoch());
            }
            i++;
        }
    }
    SetStatus("HeatingOvenSlice", Slices.join(","));
    return HeatingTime;
}

quint64 CProgramStatusInfor::GetRemaingTimeForMeltingParffin(quint32 ParaffinMeltingPoint)
{
    quint64 TimeLimit = 12 * 60 * 60 * 1000;
    quint64 HeatingTime = 0;
    quint64 UnHeatingTime = 0;
    if(ParaffinMeltingPoint >= 64)
    {
        TimeLimit = 15 * 60 * 60 * 1000;
    }
    quint64 TimeForMelting = TimeLimit;
    TimeLimit = QDateTime::currentMSecsSinceEpoch() - TimeLimit;
    QString value = m_Status.value("HeatingOvenSlice");
    QStringList Slices;
    if(!value.isEmpty())
    {
        Slices = value.split(",");
    }
    quint64 Start = 0;
    quint64 End = 0;
    for(int i = 0; i < Slices.length() / 2;)
    {
        Start = QDateTime::fromString(Slices.at(2 * i),"yyyyMMdd-hhmmss").toMSecsSinceEpoch();
        End = QDateTime::fromString(Slices.at(2 * i + 1),"yyyyMMdd-hhmmss").toMSecsSinceEpoch();
        if(End < TimeLimit)
        {
            Slices.removeAt(2 * i);
            Slices.removeAt(2 * i);
            Start = End = TimeLimit;
        }
        else if(End >= TimeLimit && Start < TimeLimit)
        {
            Slices.replace(2 * i ,QDateTime::fromMSecsSinceEpoch(TimeLimit).toString("yyyyMMdd-hhmmss"));
            HeatingTime += (End - TimeLimit);
            Start = TimeLimit;
            i++;
        }
        else
        {
            HeatingTime += (End - Start);
            if(i > 0)
            {
                UnHeatingTime += (Start - QDateTime::fromString(Slices.at(2 * i - 1),"yyyyMMdd-hhmmss").toMSecsSinceEpoch());
            }
            i++;
        }
    }
    UnHeatingTime += (QDateTime::currentMSecsSinceEpoch() - End);
    SetStatus("HeatingOvenSlice", Slices.join(","));

    if(TimeForMelting - HeatingTime + UnHeatingTime <= 4 * 60 * 60 * 1000)
    {
        return 2 * (TimeForMelting - HeatingTime + UnHeatingTime) / 1000;
    }
    else
    {
        return TimeForMelting/1000;
    }
}

void CProgramStatusInfor::UpdateOvenHeatingTime(quint64 Time, bool IsHeatingOn, bool ResetFlag)
{
    if(((QDateTime::currentMSecsSinceEpoch() - m_LastTimeUpdateHeatingTime) < 60 * 1000)
            && !ResetFlag && (IsHeatingOn == m_LastHeatingOn)) // record by one minutes
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
    if(ResetFlag) // reset oven heating to 0
    {
        Slices.clear();
    }
    if(Slices.length() == 0 || (!m_LastHeatingOn && IsHeatingOn)) // if 1. first boot; 2. the heating status from OFF to ON.
    {

        Slices.append(QDateTime::fromMSecsSinceEpoch(Time).toString("yyyyMMdd-hhmmss"));
        Slices.append(QDateTime::fromMSecsSinceEpoch(Time).toString("yyyyMMdd-hhmmss"));
        m_LastTimeUpdateHeatingTime = QDateTime::currentMSecsSinceEpoch();
        SetStatus(key, Slices.join(","));
    }
    else if(m_LastHeatingOn && IsHeatingOn) // if the heating status from ON to ON
    {
        Slices.replace(Slices.length() - 1,QDateTime::fromMSecsSinceEpoch(Time).toString("yyyyMMdd-hhmmss"));
        m_LastTimeUpdateHeatingTime = QDateTime::currentMSecsSinceEpoch();
        SetStatus(key, Slices.join(","));
    }
    m_LastHeatingOn = IsHeatingOn;
}

bool CProgramStatusInfor::IsRetortContaminted()
{
    QString ReagentGroup = GetLastReagentGroup();
    return !ReagentGroup.isEmpty();
    /*return ((!ReagentGroup.isEmpty()) &&
            (ReagentGroup.compare("RG2") != 0) &&   //water
            (ReagentGroup.compare("RG7") != 0) &&   //Cleaning Solvent
            (ReagentGroup.compare("RG8") != 0));*/    //Cleaning Alcohol
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
    qDebug()<<Cmd;
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
