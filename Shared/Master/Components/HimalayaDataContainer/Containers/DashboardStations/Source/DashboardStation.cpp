/****************************************************************************/
/*! \file DashboardStation.cpp
 *
 *  \brief DashboardStation class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 16-Jan-2013
 *   $Author:  $ Santhosha Kumar Raju
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

#include <QDebug>
#include <QString>
#include <QBuffer>

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardStation.h"
#include <DataManager/Helper/Include/DataManagerEventCodes.h>
#include "HimalayaDataContainer/Helper/Include/HimalayaDataManagerEventCodes.h"

//lint -sem(DataManager::CDashboardStation::CopyFromOther, initializer)

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Default Constructor for DashboardStation class.
*/
/****************************************************************************/
CDashboardStation::CDashboardStation() :
    m_DashboardStationID("0"),
    m_DashboardStationName("UNDEFINED"),
    m_IsParaffinBath(false),
    m_ReagentID("-1"),
    m_ReagentExchangeDate(QDate::fromString("1986-01-01", "yyyy-MM-dd")),
    m_ReagentActualCassettes(0),
    m_ReagentActualCycles(0),
    m_ReagentStatus("Empty"),
    m_DashboardStationStatus(INVALID_STATUS),
    m_DashboardStationType(INVALID_STATION)

{
}

/****************************************************************************/
/*!
 *  \brief Parameterized constructor for DashboardStation class.
 *
 *  \iparam ID = Unique ID of station
 */
/****************************************************************************/
CDashboardStation::CDashboardStation(const QString ID)
{
    m_DashboardStationID = ID;
    m_DashboardStationName = "UNDEFINED";
    m_ReagentID = "-1";
    m_DashboardStationStatus = INVALID_STATUS;
    m_DashboardStationType = INVALID_STATION;
    m_IsParaffinBath = false;
    m_ReagentExchangeDate = QDate::fromString("1986-01-01", "yyyy-MM-dd");
    m_ReagentActualCassettes = 0;
    m_ReagentActualCycles = 0;
    m_ReagentStatus = "Empty";
}

/****************************************************************************/
/*!
 *  \brief Copy Constructor
 *
 *  \iparam DashboardStation = Instance of the CDashboardStation class
 *
 *  \return
 */
/****************************************************************************/
CDashboardStation::CDashboardStation(const CDashboardStation& ExistingStation)
{
    CopyFromOther(ExistingStation);
}

void CDashboardStation::CopyFromOther(const CDashboardStation& Station)
{
    CDashboardStation& OtherStation = const_cast<CDashboardStation &>(Station);
    m_DashboardStationID = OtherStation.GetDashboardStationID();
    m_DashboardStationName = OtherStation.GetDashboardStationName();
    m_ReagentID = OtherStation.GetDashboardReagentID();
    m_ReagentStatus = OtherStation.GetDashboardReagentStatus();
    m_DashboardStationStatus = OtherStation.GetDashboardStationStatus();
    m_DashboardStationType = OtherStation.GetDashboardStationType();
    m_IsParaffinBath = OtherStation.IsParaffinBath();
    m_ReagentExchangeDate = OtherStation.GetDashboardReagentExchangeDate();
    m_ReagentActualCassettes = OtherStation.GetDashboardReagentActualCassettes();
    m_ReagentActualCycles = OtherStation.GetDashboardReagentActualCycles();

}

/****************************************************************************/
/*!
 *  \brief Output Stream Operator which streams data
 *
 *  \iparam OutDataStream = Instance of the QDataStream
 *  \iparam Station = CDashboardStation class object
 *
 *  \return Output Stream
 */
/****************************************************************************/
QDataStream& operator <<(QDataStream& OutDataStream, const CDashboardStation& Station)
{
    CDashboardStation* p_TempStation = const_cast<CDashboardStation*>(&Station);

    QXmlStreamWriter XmlStreamWriter;
    XmlStreamWriter.setDevice(OutDataStream.device());

    if (!p_TempStation->SerializeContent(XmlStreamWriter, true)) {
        qDebug() << "CDashboardStation::Operator Streaming (SerializeContent) failed.";
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
 *  \iparam Station = CDashboardStation class object
 *
 *  \return Input Stream
 */
/****************************************************************************/

QDataStream& operator >>(QDataStream& InDataStream, CDashboardStation& Station)
{
    QXmlStreamReader XmlStreamReader;
    XmlStreamReader.setDevice(InDataStream.device());

    if (!Helper::ReadNode(XmlStreamReader, QString("Station"))) {
        qDebug() << "CDashboardStation::Operator Streaming (DeSerializeContent) Node not found: Reagent";
        THROWARG(Global::EVENT_GLOBAL_UNKNOWN_STRING_ID, Global::tTranslatableStringList() << FILE_LINE);
    }

    if (!Station.DeserializeContent(XmlStreamReader, true)) {
        qDebug() << "CDashboardStation::Operator Streaming (DeSerializeContent) failed.";
        // throws an exception
        THROWARG(Global::EVENT_GLOBAL_UNKNOWN_STRING_ID, Global::tTranslatableStringList() << FILE_LINE);
    }

    return InDataStream;
}


/****************************************************************************/
/*!
 *  \brief Writes the CProgram Data to QIODevice
 *
 *  \iparam XmlStreamWriter = XmlStream to Write
 *  \iparam CompleteData = if true writes Complete data of object into
 *                           the stream
 *
 *  \return true - success ,false - failure
 */
/****************************************************************************/
bool CDashboardStation::SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData)
{
    //QReadLocker Locker(mp_ReadWriteLock);
    bool Result = true;

    XmlStreamWriter.writeStartElement("Station");
    XmlStreamWriter.writeAttribute("ID", GetDashboardStationID());
    XmlStreamWriter.writeAttribute("Name", GetDashboardStationName());
    XmlStreamWriter.writeAttribute("ParaffinBath", (IsParaffinBath() == true ? "true" : "false"));

    if (CompleteData) {

    }

    XmlStreamWriter.writeStartElement("Reagent");
    XmlStreamWriter.writeAttribute("ID", GetDashboardReagentID());
    XmlStreamWriter.writeAttribute("ExchangeDate", GetDashboardReagentExchangeDate().toString("yyyy-MM-dd"));
    XmlStreamWriter.writeAttribute("ActualCassettes", QString::number(GetDashboardReagentActualCassettes()));
    XmlStreamWriter.writeAttribute("ActualCycles", QString::number(GetDashboardReagentActualCycles()));
    XmlStreamWriter.writeAttribute("Status", GetDashboardReagentStatus());
    XmlStreamWriter.writeEndElement(); // Reagent

    XmlStreamWriter.writeEndElement(); // Station

    return Result;
}

/****************************************************************************/
/*!
 *  \brief Reads from the IODevice to CProgram object.
 *
 *  \iparam XmlStreamReader = Xml stream to read from
 *  \iparam CompleteData = bool type if true gives Complete data of object
 *
 *  \return true-success, false - failure
 */
/****************************************************************************/
bool CDashboardStation::DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData)
{
//    QWriteLocker Locker(mp_ReadWriteLock);
    bool Result = true;

    // ID
    if (!XmlStreamReader.attributes().hasAttribute("ID")) {
        qDebug() << "### attribute <ID> is missing => abort reading";
        return false;
    }
    SetDashboardStationID(XmlStreamReader.attributes().value("ID").toString());

    // Name
    if (!XmlStreamReader.attributes().hasAttribute("Name")) {
        qDebug() << "### attribute <Name> is missing => abort reading";
        return false;
    }
    SetDashboardStationName(XmlStreamReader.attributes().value("Name").toString());

    // ParaffinBath
    if (!XmlStreamReader.attributes().hasAttribute("ParaffinBath")) {
        qDebug() << "### attribute <ParaffinBath> is missing => abort reading";
        return false;
    }
    IsParaffinBath(
                XmlStreamReader.attributes().value("ParaffinBath").toString().toLower() == "true" ?
                    true : false);

    if (CompleteData) {

    }

    // Look for node <Reagent>
    if (!Helper::ReadNode(XmlStreamReader, "Reagent")) {
        qDebug() << "DeserializeContent: abort reading. Node not found: Reagent";
        return false;
    }

    // Reagent ID
    if (!XmlStreamReader.attributes().hasAttribute("ID")) {
        qDebug() << "### attribute <ID> is missing => abort reading";
        return false;
    }
    SetDashboardReagentID(XmlStreamReader.attributes().value("ID").toString());

    // Reagent Exchange Date
    if (!XmlStreamReader.attributes().hasAttribute("ExchangeDate")) {
        qDebug() << "### attribute <ExchangeDate> is missing => abort reading";
        return false;
    }
    QString Date = XmlStreamReader.attributes().value("ExchangeDate").toString();
    SetDashboardReagentExchangeDate(QDate::fromString(Date, "yyyy-MM-dd"));

    // Reagent ActualCassettes
    if (!XmlStreamReader.attributes().hasAttribute("ActualCassettes")) {
        qDebug() << "### attribute <ActualCassettes> is missing => abort reading";
        return false;
    }
    SetDashboardReagentActualCassettes(XmlStreamReader.attributes().value("ActualCassettes").toString().toInt());

    // Reagent ActualCycles
    if (!XmlStreamReader.attributes().hasAttribute("ActualCycles")) {
        qDebug() << "### attribute <ActualCycles> is missing => abort reading";
        return false;
    }
    SetDashboardReagentActualCycles(XmlStreamReader.attributes().value("ActualCycles").toString().toInt());

    // Reagent Status
    if (!XmlStreamReader.attributes().hasAttribute("Status")) {
        qDebug() << "### attribute <Status> is missing => abort reading";
        return false;
    }
    SetDashboardReagentStatus(XmlStreamReader.attributes().value("Status").toString());

    return Result;
}

/****************************************************************************/
/*!
 *  \brief Assignment Operator which copies from rhs to lhs.
 *
 *  \iparam SourceStation = CDashboardStation class object
 *
 *  \return CDashboardStation Class Object
 *  \note   Swap idiom is not used since CDashboardStation doesnt manage any resource
 *          and exception wont be thrown
 */
/****************************************************************************/
CDashboardStation& CDashboardStation::operator=(const CDashboardStation& SourceStation)
{
    CopyFromOther(SourceStation);
    return (*this);
}

/****************************************************************************/
/*!
 *  \brief  Resets actual cassettes, actual cycles,
 *          actual days, and exchange date.
 *
 *  \return true, if succeed to reset data;
 *          false, if cannot reset data.
 */
/****************************************************************************/
void CDashboardStation::ResetData(void)
{
    m_ReagentActualCassettes = 0;
    m_ReagentActualCycles    = 0;
    m_ReagentExchangeDate    = QDate::currentDate();
}

/****************************************************************************/
/*!
 *  \brief  Get the status of a reagent
 *
 *  \return Global::ReagentStatusType
 *
 */
/****************************************************************************/

Global::ReagentStatusType CDashboardStation::GetReagentStatus(const DataManager::CReagent &Reagent, const Global::RMSOptions_t Option)
{
    Global::ReagentStatusType ReagentStatus = Global::REAGENT_STATUS_NOT_IN_STATION;
	
    CDashboardStation*  pDashboardStation = this;
    if (pDashboardStation->GetDashboardReagentID() == Reagent.GetReagentID())
    {
		switch ( Option )
		{
		    case Global::RMS_OFF:
                ReagentStatus = Global::REAGENT_STATUS_NORMAL;
				break;
			case Global::RMS_CASSETTES:
			{
                int MaxCassettes = Reagent.GetMaxCassettes();
                int ActualCassettes = pDashboardStation->GetDashboardReagentActualCassettes();
                if ( (MaxCassettes - ActualCassettes) < 0 )
                    ReagentStatus = Global::REAGENT_STATUS_EXPIRED;
				else
                    ReagentStatus = Global::REAGENT_STATUS_NORMAL;
				break;
			}
			case Global::RMS_CYCLES:
			{
                int MaxCycles = Reagent.GetMaxCycles();
                int ActualRecycles = pDashboardStation->GetDashboardReagentActualCycles();
				if ( (MaxCycles - ActualRecycles) < 0 )
                    ReagentStatus = Global::REAGENT_STATUS_EXPIRED;
				else
                    ReagentStatus = Global::REAGENT_STATUS_NORMAL;
				break;
			}
			case Global::RMS_DAYS:
			{
                QDate CurDate;
                QDate ReagentExchangeQDate = pDashboardStation->GetDashboardReagentExchangeDate();
                QDate ReagentExpiryQDate = ReagentExchangeQDate.addDays(Reagent.GetMaxDays());
                if(CurDate.currentDate() > ReagentExpiryQDate)
                    ReagentStatus = Global::REAGENT_STATUS_EXPIRED;
                else
                    ReagentStatus = Global::REAGENT_STATUS_NORMAL;
				break;
			}
			default:
				break;
        }
    }

    return ReagentStatus;
}


} // end of namespace DataManager
