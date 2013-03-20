/****************************************************************************/
/*! \file ProgramSequence.cpp
 *
 *  \brief ProgramSequence implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-12-26
 *   $Author:  $ N.Kamath
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

#include <QDebug>
#include <QBuffer>
#include <QXmlStreamReader>
#include <iostream>

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequence.h"
#include "Global/Include/EventObject.h"
#include <DataManager/Helper/Include/DataManagerEventCodes.h>
#include "HimalayaDataContainer/Helper/Include/HimalayaDataManagerEventCodes.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CProgramSequence::CProgramSequence() :
    m_ProgramID(""), m_Used(false), m_Counter(0), m_StartCapable(false), m_StartCapableResult("-1"),
    m_StartCapableDynamic(true)
{

}

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam ProgramID
 */
/****************************************************************************/
CProgramSequence::CProgramSequence(QString ProgramID) :
    m_ProgramID(ProgramID), m_Used(false), m_Counter(0), m_StartCapable(false), m_StartCapableResult("-1"),
    m_StartCapableDynamic(true)
{
}

/***************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam ProgramID
 *  \iparam Used
 *  \iparam Counter
 *  \iparam StartCapable
 *  \iparam StartCapableResult
 */
/****************************************************************************/
CProgramSequence::CProgramSequence(QString ProgramID, bool Used, quint32 Counter,
                                   bool StartCapable, QString StartCapableResult):
    m_ProgramID(ProgramID), m_Used(Used), m_Counter(Counter), m_StartCapable(StartCapable), m_StartCapableResult(StartCapableResult),
    m_StartCapableDynamic(true)
{
}

/****************************************************************************/
/*!
 *  \brief Copy Constructor
 *
 *  \iparam ProgramSequence = Instance of the CProgramSequence class
 */
/****************************************************************************/
CProgramSequence::CProgramSequence(const CProgramSequence& ProgramSequence)
{
    CProgramSequence* p_TempProgramSequence = const_cast<CProgramSequence*>(&ProgramSequence);
    *this = *p_TempProgramSequence;
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CProgramSequence::~CProgramSequence()
{
}

/****************************************************************************/
/*!
 *  \brief Writes from the CProgramSequence object to a IODevice.
 *
 *  \iparam XmlStreamWriter = Instance of the IODevice might be Buffer or File
 *  \iparam CompleteData = bool type if true writes Complete data of object
 *
 *  \return true - success , false - failure
 */
/****************************************************************************/
bool CProgramSequence::SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData)
{
    QString StringValue;
    bool Result = true;

    XmlStreamWriter.writeStartElement("Program");

    XmlStreamWriter.writeAttribute("ID", GetID());

    if (GetUsedStatus()) {
        XmlStreamWriter.writeAttribute("Used", "true");
    }
    else {
        XmlStreamWriter.writeAttribute("Used", "false");
    }
    (void)StringValue.setNum(GetCounterValue()); // to avoid lint-534

    XmlStreamWriter.writeAttribute("Counter", StringValue );

    if (GetStartCapableStatus()) {
        XmlStreamWriter.writeAttribute("StartCapable", "true");
    }
    else {
        XmlStreamWriter.writeAttribute("StartCapable", "false");
    }

    StringValue=GetStartCapableResult();
    XmlStreamWriter.writeAttribute("StartCapableResult", StringValue);

    //======NODE=======Temporary Data Variables=========================
    if(CompleteData) {
        //
        if (GetStartCapableDynamicStatus()) {
            XmlStreamWriter.writeAttribute("StartCapableDynamic", "true");
        }
        else {
            XmlStreamWriter.writeAttribute("StartCapableDynamic", "false");
        }
    }
    //======NODE=END====Temporary Data Variables=========================

    XmlStreamWriter.writeEndElement(); // Program

    return Result;
}

/****************************************************************************/
/*!
 *  \brief Reads from the IODevice to CProgramSequence object.
 *
 *  \iparam XmlStreamReader = Instance of the IODevice might be Buffer or File.
 *  \iparam CompleteData = bool type if true gives Complete data of object
 *
 *  \return true - success , false - failure
 */
/****************************************************************************/
bool CProgramSequence::DeserializeContent(QXmlStreamReader& XmlStreamReader,bool CompleteData)
{
    // ID
    if (!XmlStreamReader.attributes().hasAttribute("ID")) {
        qDebug() << "### attribute <ID> is missing => abort reading";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_ERROR_XML_ATTRIBUTE_NOT_FOUND,
                                                   Global::tTranslatableStringList() << "ProgramSequence:ID", true);
        return false;
    }
    SetID(XmlStreamReader.attributes().value("ID").toString());

    if (!XmlStreamReader.attributes().hasAttribute("Used")) {
        qDebug() << "### attribute <Used> is missing => abort reading";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_ERROR_XML_ATTRIBUTE_NOT_FOUND,
                                                   Global::tTranslatableStringList() << "ProgramSequence:Used", true);
        return false;
    }
    else {
        bool Value = false;
        if (XmlStreamReader.attributes().value("Used").toString().toUpper() == "TRUE") {
            Value = true;
        }
        SetUsedStatus(Value);
    }

    // Counter
    if (!XmlStreamReader.attributes().hasAttribute("Counter")) {
        qDebug() << "### attribute <Counter> is missing => abort reading";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_ERROR_XML_ATTRIBUTE_NOT_FOUND,
                                                   Global::tTranslatableStringList() << "ProgramSequence:Counter", true);
        return false;
    }
    SetCounterValue(XmlStreamReader.attributes().value("Counter").toString().toInt());

    // StartCapable
    if (!XmlStreamReader.attributes().hasAttribute("StartCapable")) {
        qDebug() << "### attribute <StartCapable> is missing => abort reading";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_ERROR_XML_ATTRIBUTE_NOT_FOUND,
                                                   Global::tTranslatableStringList() << "ProgramSequence:StartCapable", true);
        return false;
    }
    else {
        bool Value = false;
        if (XmlStreamReader.attributes().value("StartCapable").toString().toUpper() == "TRUE") {
            Value = true;
        }
        SetStartCapableStatus(Value);
    }

    if (!XmlStreamReader.attributes().hasAttribute("StartCapableResult")) {
        qDebug() << "### attribute <StartCapableResult> is missing => abort reading";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_ERROR_XML_ATTRIBUTE_NOT_FOUND,
                                                   Global::tTranslatableStringList() << "ProgramSequence:StartCapableResult", true);
        return false;
    }
    SetStartCapableResult(XmlStreamReader.attributes().value("StartCapableResult").toString());

    //======NODE=======Temporary Data Variables=========================
    if(CompleteData) {
        // StartCapable Dynamic
        if (!XmlStreamReader.attributes().hasAttribute("StartCapableDynamic")) {
            qDebug() << "### attribute <StartCapableDynamic> is missing => abort reading";
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_ERROR_XML_ATTRIBUTE_NOT_FOUND,
                                                       Global::tTranslatableStringList() << "ProgramSequence:StartCapableDynamic", true);
            return false;
        }
        else {
            bool Value = false;
            if (XmlStreamReader.attributes().value("StartCapableDynamic").toString().toUpper() == "TRUE") {
                Value = true;
            }
            SetStartCapableDynamicStatus(Value);
        }
    }
    //======NODE=END====Temporary Data Variables=========================
    return true;
}

/****************************************************************************/
/*!
 *  \brief Output Stream Operator which streams data
 *
 *  \iparam OutDataStream = Instance of the QDataStream
 *  \iparam ProgramSequence = CProgramSequence class object
 *
 *  \return Output Stream
 */
/****************************************************************************/
QDataStream& operator <<(QDataStream& OutDataStream, const CProgramSequence& ProgramSequence)
{
    CProgramSequence* p_TempProgramSequence = const_cast<CProgramSequence*>(&ProgramSequence);

    QXmlStreamWriter XmlStreamWriter;
    XmlStreamWriter.setDevice(OutDataStream.device());

    if (!p_TempProgramSequence->SerializeContent(XmlStreamWriter, true)) {
        qDebug() << "CProgramSequence::Operator Streaming (SerializeContent) failed.";

        p_TempProgramSequence->m_ErrorHash.insert(EVENT_DM_STREAMOUT_FAILED,
                                              Global::tTranslatableStringList() << "ProgramSequence");
        p_TempProgramSequence->SetErrorList(&p_TempProgramSequence->m_ErrorHash);
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_STREAMOUT_FAILED,
                                                   Global::tTranslatableStringList() << "ProgramSequence", true);
    }

    return OutDataStream;
}

/****************************************************************************/
/*!
 *  \brief Input stream Operator which reads the data from Input parameter.
 *
 *  \iparam InDataStream = Instance of the DataStream
 *  \iparam ProgramSequence = CProgramSequence class object
 *
 *  \return Input Stream
 */
/****************************************************************************/
QDataStream& operator >>(QDataStream& InDataStream, CProgramSequence& ProgramSequence)
{
    QXmlStreamReader XmlStreamReader;
    XmlStreamReader.setDevice(InDataStream.device());

    if (!Helper::ReadNode(XmlStreamReader, QString("Program"))) {
        qDebug() << "CProgramSequence::Operator Streaming (DeSerializeContent) Node not found: Program";
        ProgramSequence.m_ErrorHash.insert(EVENT_DM_STREAMIN_FAILED,
                                              Global::tTranslatableStringList() << "ProgramSequence");
        ProgramSequence.SetErrorList(&ProgramSequence.m_ErrorHash);
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_STREAMIN_FAILED,
                                                   Global::tTranslatableStringList() << "ProgramSequence", true);
    }
    if (!ProgramSequence.DeserializeContent(XmlStreamReader, true)) {
        qDebug() << "CProgramSequence::Operator Streaming (DeSerializeContent) failed.";
        ProgramSequence.m_ErrorHash.insert(EVENT_DM_STREAMIN_FAILED,
                                              Global::tTranslatableStringList() << "ProgramSequence");
        ProgramSequence.SetErrorList(&ProgramSequence.m_ErrorHash);
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_STREAMIN_FAILED,
                                                   Global::tTranslatableStringList() << "ProgramSequence", true);
    }
    return InDataStream;
}

/****************************************************************************/
/*!
 *  \brief Assignment Operator which copies from rhs to lhs.
 *
 *  \iparam SourceProgramSequence = CProgramSequence class object
 *
 *  \return CProgramSequence Class Object
 */
/****************************************************************************/
CProgramSequence& CProgramSequence::operator=(const CProgramSequence& SourceProgramSequence)
{
    // make sure not same object
    if (this != &SourceProgramSequence)
    {
        QByteArray TempByteArray;
        QDataStream DataStream(&TempByteArray, QIODevice::ReadWrite);
        DataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
        TempByteArray.clear();
        DataStream << SourceProgramSequence;
        (void)DataStream.device()->reset();
        DataStream >> *this;
    }
    return *this;
}

}//End of namespace DataManager
