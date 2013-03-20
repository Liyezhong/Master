/****************************************************************************/
/*! \file ProgramStep.cpp
 *
 *  \brief Implementation file for class CProgramStep.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2011-08-01
 *  $Author:    $ F. Toth, N.Kamath
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QStringList>
#include <QDebug>

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "DataManager/Helper/Include/Helper.h"
#include "HimalayaDataContainer/Containers/Programs/Include/ProgramStep.h"


namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Default Constructor
 */
/****************************************************************************/
CProgramStep::CProgramStep():
    m_StepID("INVALID"), m_ReagentID("0"),
    m_MinDuration(0), m_MaxDuration(0), m_Exclusive(false),
    m_NumberOfParallelStations(0), m_Intensity(0), m_NumberOfSteps(9), m_RangeLow(30), m_RangeHigh(60)
{
    m_StationIDList.clear();
}

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam StepID
 */
/****************************************************************************/
CProgramStep::CProgramStep(QString StepID):
    m_StepID(StepID), m_ReagentID("0"),
    m_MinDuration(0), m_MaxDuration(0), m_Exclusive(false), m_NumberOfParallelStations(0), m_Intensity(0),
    m_NumberOfSteps(9), m_RangeLow(30), m_RangeHigh(60)
{
    m_StationIDList.clear();
}

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam StepID = program ID
 *  \iparam StationsType = e.g. DRY,REAGENT,OVEN,HEATED,WATER etc
 *  \iparam ReagentID = Id of the reagent used for the program step
 *  \iparam MinDuration = Minimum duration of the program step [s]
 *  \iparam MaxDuration = Tolerance for program step
 *  \iparam Exclusive   = Reagent is exclusive to program step
 *  \iparam ParallelStations = total number of parallel stations
 *  \iparam Intensity = Intensity value for Leica programs
 */
/****************************************************************************/
CProgramStep::CProgramStep(QString StepID, QString ReagentID, int MinDuration, int MaxDuration, bool Exclusive,
                           int ParallelStations, int Intensity):
    m_StepID(StepID),
    m_ReagentID(ReagentID),
    m_MinDuration(MinDuration),
    m_MaxDuration(MaxDuration),
    m_Exclusive(Exclusive),
    m_NumberOfParallelStations(ParallelStations),
    m_Intensity(Intensity),
    m_NumberOfSteps(9),
    m_RangeLow(30),
    m_RangeHigh(60)
{
    m_StationIDList.clear();
}

/****************************************************************************/
/*!
 *  \brief Copy Constructor
 *
 *  \iparam SourceProgramStep
 */
/****************************************************************************/
CProgramStep::CProgramStep(const CProgramStep &SourceProgramStep)
{
    m_StationIDList.clear();
    *this = SourceProgramStep;
}

/****************************************************************************/
/*!
 *  \brief Function to retrieve the minimum duration of the program step
 *
 *  \return returns the minimum duration in sec
 */
/****************************************************************************/
QString CProgramStep::GetMinDuration()
{
    if (m_Intensity != 0) {
        m_MinDuration = CalculateMinDuration();
    }
    return Helper::ConvertSecondsToTimeString(m_MinDuration);
}

/****************************************************************************/
/*!
 *  \brief Sets the minimum program step. e.g. "0d 1h 5m 5s"
 *
 *  \iparam Value = e.g. "0d 1h 5m 5s"
 */
/****************************************************************************/
void CProgramStep::SetMinDuration(const QString Value)
{
    m_MinDuration = Helper::ConvertTimeStringToSeconds(Value);

    return;
}

/****************************************************************************/
/*!
 *  \brief Returns the maximum duration of the program step
 *
 *  \return Maximum duration of program step in seconds
 */
/****************************************************************************/
int CProgramStep::GetMaxDurationInSeconds() const
{
    if (m_MaxDuration < 0) {
        return -1;
    } else {
        return (1 + m_MaxDuration / 100.0) * m_MinDuration;
    }
}

/****************************************************************************/
/*!
 *  \brief Sets the maximum program step duration
 *
 *  \iparam Value = e.g. "100%"
 */
/****************************************************************************/
void CProgramStep::SetMaxDurationInPercent(const QString Value)
{
    if (Value.endsWith("%")) {
        m_MaxDurationString = Value;
        QString StringHelper = Value;
        StringHelper.chop(1);
        bool ok;
        int Percent = StringHelper.toInt(&ok);
        if(!ok) {
            qDebug() << "##### CProgramStep::SetMaxDurationInPercent failed. Old value is still active.";
        } else {
            m_MaxDuration = Percent;
        }
    } else if (Value == "INF") {
        //m_MaxDuration = -1;
        m_MaxDuration = 100;  // value for unlimited max duration not needed (2011/06/07)
        m_MaxDurationString = "100%";
        qDebug() << "-----WARNING: CProgramStep::SetMaxDurationInPercent. INF not allowed anymore for maxDuration.";
    } else {
        qDebug() << "##### CProgramStep::SetMaxDurationInPercent failed. Invalid string value. Old value is still active.";
    }

    return;
}


/****************************************************************************/
/*!
 *  \brief Calculates the Minimum Duration When Intensity is non zero
 *
 *  \iparam void
 *
 *  \return The Minimum duration
 */
/****************************************************************************/
int CProgramStep::CalculateMinDuration()
{
    int Resolution;
    if ((m_Intensity == 0) || (m_NumberOfSteps == 0) || (m_RangeLow == 0) || (m_RangeHigh == 0)) {
        qDebug() << "The calculation parameters are zero's -- ERROR";
        return 0;
    }
    //calculating the resolution of each step
    Resolution = qRound((m_RangeHigh-m_RangeLow)/m_NumberOfSteps);

    return (m_Intensity*Resolution);
}

/****************************************************************************/
/*!
*  \brief Retrieves Range Low of the reagent used for the Leica Program
*
*  \return Number of Steps
*/
/****************************************************************************/

QString CProgramStep::GetRangeLowInString() const
{
    return Helper::ConvertSecondsToTimeString(m_RangeLow);
}

/****************************************************************************/
/*!
*  \brief Retrieves Range Low of the reagent used for the Leica Program
*
*  \return Range Low
*/
/****************************************************************************/

int CProgramStep::GetRangeLow() const
{
    return m_RangeLow;
}

/****************************************************************************/
/*!
 *  \brief Sets Range low for a Leica program
 *
 *  \iparam Value = Range low value
 */
/****************************************************************************/
void CProgramStep::SetRangeLow(const QString Value)
{
    m_RangeLow = Helper::ConvertTimeStringToSeconds(Value);
    return;
}

/****************************************************************************/
/*!
*  \brief Retrieves Range High of the reagent used for the Leica Program
*
*  \return Range High in String
*/
/****************************************************************************/

QString CProgramStep::GetRangeHighInString() const
{
    return Helper::ConvertSecondsToTimeString(m_RangeHigh);
}

/****************************************************************************/
/*!
*  \brief Retrieves Range High of the reagent used for the Leica Program
*
*  \return Range High
*/
/****************************************************************************/

int CProgramStep::GetRangeHigh() const
{
    return m_RangeHigh;
}

/****************************************************************************/
/*!
 *  \brief Sets Range High for a Leica program
 *
 *  \iparam Value = Range High value
 */
/****************************************************************************/
void CProgramStep::SetRangeHigh(const QString Value)
{
        m_RangeHigh = Helper::ConvertTimeStringToSeconds(Value);
        return;
}

/****************************************************************************/
/*!
 *  \brief Writes from the CProgramStep object to a IODevice.
 *
 *  \iparam XmlStreamWriter = XML stream to write into.
 *  \iparam CompleteData = bool type if true writes Complete data of object
 *
 *  \return True or False
 */
/****************************************************************************/
bool CProgramStep::SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData)
{
    bool Result = true;
    XmlStreamWriter.writeStartElement("Step");
    XmlStreamWriter.writeAttribute("ID", GetStepID());
    XmlStreamWriter.writeAttribute("ReagentID", GetReagentID());

    if (GetExclusive()) {
        XmlStreamWriter.writeAttribute("Exclusive", "true");
    } else {
        XmlStreamWriter.writeAttribute("Exclusive", "false");
    }

    XmlStreamWriter.writeAttribute("ParallelStations",QString::number(GetNumberOfParallelStations()));
    XmlStreamWriter.writeAttribute("MinDuration", GetMinDuration());

    QString StringValue;
    (void)StringValue.setNum(GetMaxDurationInPercent()); //to suppress lint-534
    StringValue.append("%");
    XmlStreamWriter.writeAttribute("MaxDuration", StringValue);
    XmlStreamWriter.writeAttribute("Intensity",QString::number(GetIntensity()));
    XmlStreamWriter.writeAttribute("NumberOfSteps",QString::number(GetNumberOfSteps()));
    XmlStreamWriter.writeAttribute("RangeLow",GetRangeLowInString());
    XmlStreamWriter.writeAttribute("RangeHigh",GetRangeHighInString());

    if (CompleteData) {
        QString StationIDs;
        for (qint32 I = 0; I < m_StationIDList.count(); I++) {
            StationIDs.append(m_StationIDList.at(I));
            StationIDs.append(",");
        }
        XmlStreamWriter.writeAttribute("StationIDList", StationIDs);
    }
    XmlStreamWriter.writeEndElement(); //Step
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Reads from the IODevice to CProgramStep object.
 *
 *  \iparam XmlStreamReader  = XML stream to read from
 *  \iparam CompleteData = bool type if true gives Complete data of object
 *
 *  \return True or False
 */
/****************************************************************************/
bool CProgramStep::DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData)
{
    // get attribute ID
    if (!XmlStreamReader.attributes().hasAttribute("ID")) {
        qDebug() << "### attribute <ID> is missing => abort reading";
        return false;
    }
    SetStepID(XmlStreamReader.attributes().value("ID").toString());

    // get reagent ID
    if (!XmlStreamReader.attributes().hasAttribute("ReagentID")) {
        qDebug() << "### attribute <ReagentID> is missing => abort reading";
        return false;
    }
    SetReagentID(XmlStreamReader.attributes().value("ReagentID").toString());

    // Exclusive
    if (!XmlStreamReader.attributes().hasAttribute("Exclusive")) {
        qDebug() << "### attribute <Exclusive> is missing => abort reading";
        return false;
    }
    if (XmlStreamReader.attributes().value("Exclusive").toString().toUpper() == "TRUE") {
        SetExclusive(true);
    } else {
        SetExclusive(false);
    }

    if (!XmlStreamReader.attributes().hasAttribute("ParallelStations")) {
        qDebug() << "### attribute <ParallelStations> is missing => abort reading";
        return false;
    }
    SetNumberOfParallelStations(XmlStreamReader.attributes().value("ParallelStations").toString().toInt());

    // MinDuration
    if (!XmlStreamReader.attributes().hasAttribute("MinDuration")) {
        qDebug() << "### attribute <MinDuration> is missing => abort reading";
        return false;
    }
    SetMinDuration(XmlStreamReader.attributes().value("MinDuration").toString());

    // MaxDuration
    if (!XmlStreamReader.attributes().hasAttribute("MaxDuration")) {
        qDebug() << "### attribute <MaxDuration> is missing => abort reading";
        return false;
    }
    SetMaxDurationInPercent(XmlStreamReader.attributes().value("MaxDuration").toString());

    if (!XmlStreamReader.attributes().hasAttribute("Intensity")) {
        qDebug() << "### attribute <Intensity> is missing => abort reading";
        return false;
    }
    SetIntensity(XmlStreamReader.attributes().value("Intensity").toString().toInt());

    if (!XmlStreamReader.attributes().hasAttribute("NumberOfSteps")) {
        qDebug() << "### attribute <NumberOfSteps> is missing => abort reading";
        return false;
    }
    SetNumberOfSteps(XmlStreamReader.attributes().value("NumberOfSteps").toString().toInt());

    // Range Low
    if (!XmlStreamReader.attributes().hasAttribute("RangeLow")) {
        qDebug() << "### attribute <RangeLow> is missing => abort reading";
        return false;
    }
    SetRangeLow(XmlStreamReader.attributes().value("RangeLow").toString());

    // Range High
    if (!XmlStreamReader.attributes().hasAttribute("RangeHigh")) {
        qDebug() << "### attribute <RangeHigh> is missing => abort reading";
        return false;
    }
    SetRangeHigh(XmlStreamReader.attributes().value("RangeHigh").toString());

    if (CompleteData) {
        // read complete data from the xml
        if (!ReadCompleteData(XmlStreamReader)) {
            return false;
        }
    }

    return true;
}


/****************************************************************************/
/*!
 *  \brief Reads the complete information from the xml stream.
 *
 *  \iparam XmlStreamReader  = XML stream to read from
 *
 *  \return True or False
 */
/****************************************************************************/
bool CProgramStep::ReadCompleteData(QXmlStreamReader& XmlStreamReader)
{

    if (!XmlStreamReader.attributes().hasAttribute("StationIDList")) {
        qDebug()<<"### attribute <StationIDList> is missing => abort reading";
        return false;
    }
    //retrieve station id list
    QStringList StationIDList = XmlStreamReader.attributes().value("StationIDList").toString().split(",",  QString::SkipEmptyParts);
    SetStationIDList(StationIDList);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Output Stream Operator which streams data
 *
 *  \iparam OutDataStream = Instance of the QDataStream
 *  \iparam ProgramStep = CProgramStep class object
 *
 *  \return Output Stream
 */
/****************************************************************************/
QDataStream& operator << (QDataStream& OutDataStream, const CProgramStep& ProgramStep) {

    QXmlStreamWriter XmlStreamWriter;
    XmlStreamWriter.setDevice(OutDataStream.device());
    CProgramStep *p_TempProgStep = const_cast<CProgramStep*>(&ProgramStep);
    if (!p_TempProgStep->SerializeContent(XmlStreamWriter, true)) {
        qDebug() << "CProgram::Operator Streaming (SerializeContent) failed.";
        // throws an exception
        THROWARG(Global::EVENT_GLOBAL_UNKNOWN_STRING_ID, Global::tTranslatableStringList() << FILE_LINE);
    }

    return OutDataStream;
}

/****************************************************************************/
/*!
 *  \brief Input stream Operator which reads the data from Input parameter.
 *
 *  \iparam InDataStream = Instance of the DataStream
 *  \iparam ProgramStep = CProgramStep class object
 *
 *  \return Input Stream
 */
/****************************************************************************/
QDataStream& operator >> (QDataStream& InDataStream, CProgramStep& ProgramStep) {

    QXmlStreamReader XmlStreamReader;
    XmlStreamReader.setDevice(InDataStream.device());
    if (!Helper::ReadNode(XmlStreamReader, QString("Step"))) {
        qDebug() << "CProgram::Operator Streaming (DeSerializeContent) Node not found: Reagent";
        THROWARG(Global::EVENT_GLOBAL_UNKNOWN_STRING_ID, Global::tTranslatableStringList() << FILE_LINE);
    }

    if (!ProgramStep.DeserializeContent(XmlStreamReader, true)) {
        qDebug() << "CProgram::Operator Streaming (DeSerializeContent) failed.";
        // throws an exception
        THROWARG(Global::EVENT_GLOBAL_UNKNOWN_STRING_ID, Global::tTranslatableStringList() << FILE_LINE);
    }

    return InDataStream;
}

/****************************************************************************/
/*!
 *  \brief Assignment Operator which copies from rhs to lhs.
 *
 *  \iparam SourceProgramStep = CProgramStep class object
 *
 *  \return CProgramStep Class Object
 */
/****************************************************************************/
CProgramStep& CProgramStep::operator = (const CProgramStep& SourceProgramStep) {

    // make sure not same object
    if (this != &SourceProgramStep) {
        QByteArray TempByteArray;
        QDataStream DataStream(&TempByteArray, QIODevice::ReadWrite);
        DataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
        TempByteArray.clear();
        DataStream << SourceProgramStep;
        (void)DataStream.device()->reset();
        DataStream >> *this;
    }
    return *this;
}

}  // namespace DataManager
