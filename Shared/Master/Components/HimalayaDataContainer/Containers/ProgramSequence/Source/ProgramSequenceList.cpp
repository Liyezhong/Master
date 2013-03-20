/****************************************************************************/
/*! \file ProgramSequenceList.cpp
 *
 *  \brief ProgramSequence Step implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-12-26 2013-Jan-15
 *   $Author:  $ N.Kamath,Ningu
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
#include <QFile>
#include <QReadLocker>
#include <QWriteLocker>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QDebug>

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Global/Include/EventObject.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequenceList.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequence.h"
#include <DataManager/Helper/Include/DataManagerEventCodes.h>
#include <HimalayaDataContainer/Helper/Include/HimalayaDataManagerEventCodes.h>


namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CProgramSequenceList::CProgramSequenceList() : m_Version(0), m_DataVerificationMode(true), m_Filename(""), mp_ReadWriteLock(NULL)
{
    SetDefaultAttributes();
}

/****************************************************************************/
/*!
 *  \brief Copy Constructor
 *
 *  \iparam ProgramSequenceList = Instance of the CProgramSequence class
 *
 *  \return Programsequence class object
 */
/****************************************************************************/
CProgramSequenceList::CProgramSequenceList(const CProgramSequenceList& ProgramSequenceList) : CDataContainerBase()
{
    CProgramSequenceList* p_TempProgramSequenceList = const_cast<CProgramSequenceList*>(&ProgramSequenceList);

    SetDefaultAttributes();
    m_DataVerificationMode = true;
    *this = *p_TempProgramSequenceList;
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CProgramSequenceList::~CProgramSequenceList()
{
    try {
        (void)DeleteAllProgramSequenceSteps(); // to avoid lint 534
    }
    catch(...) {

    }
    if (mp_ReadWriteLock != NULL) {
        try {
            delete mp_ReadWriteLock;
        }
        catch(...) {
            //to please PClint
        }
        mp_ReadWriteLock = NULL;
    }

}

/****************************************************************************/
/*!
 *  \brief Defaults to the initial state of Attributes of CDataReagentList
 */
/****************************************************************************/
void CProgramSequenceList::SetDefaultAttributes()
{
    mp_ReadWriteLock = new QReadWriteLock(QReadWriteLock::Recursive);
    //    m_VerifierList.clear();
    Init();
}
/****************************************************************************/
/*!
 *  \brief Initialize the Program Sequence list
 */
/****************************************************************************/
void CProgramSequenceList::Init()
{
    m_Version = 1;
    m_Filename = "";
    (void)DeleteAllProgramSequenceSteps(); // to suppress lint-534
}


/****************************************************************************/
/*!
 *  \brief Writes from the CProgramSequenceList object to a IODevice.
 *
 *  \iparam IODevice = Instance of the IODevice might be Buffer or File
 *  \iparam CompleteData = bool type if true writes Complete data of object
 *
 *  \return True or False
 */
/****************************************************************************/
bool CProgramSequenceList::SerializeContent(QIODevice& IODevice, bool CompleteData)
{
    QString StringValue;
    bool Result = true;
    QXmlStreamWriter XmlStreamWriter;

    XmlStreamWriter.setDevice(&IODevice);
    XmlStreamWriter.setAutoFormatting(true);
    //XmlStreamWriter.setAutoFormattingIndent(4);
    XmlStreamWriter.writeDTD("<!DOCTYPE ProgramsSequence>");    
    XmlStreamWriter.writeStartElement("ProgramsSequence");

    // write attribute version
    (void)StringValue.setNum(GetVersion()); //to suppress lint-534
    XmlStreamWriter.writeAttribute("Version", StringValue);

    for(int i=0;i<GetNumberOfPrograms();i++)
    {
        CProgramSequence *p_ProgramSequence= const_cast<CProgramSequence*> (GetProgramSequenceStep(i));
        if (p_ProgramSequence != NULL) {
            if(!p_ProgramSequence->SerializeContent(XmlStreamWriter,CompleteData)){
                return false;
            }
        }
    }

    if (CompleteData) {
        XmlStreamWriter.writeStartElement("NonXmlData");
        if (GetDataVerificationMode()) {
            XmlStreamWriter.writeAttribute("VerificationMode", "true");
        }
        else {
            XmlStreamWriter.writeAttribute("VerificationMode", "false");
        }

        XmlStreamWriter.writeAttribute("FileName", GetFilename());

        //        XmlStreamWriter.writeAttribute("VerifierCount", QString::number(m_VerifierList.count()));
        //        for (qint32 I = 0; I < m_VerifierList.count(); I++) {
        //            QString VerifierPtr;
        //            QTextStream VerifierPointer(&VerifierPtr);
        //            VerifierPointer << m_VerifierList.at(I);
        //            XmlStreamWriter.writeAttribute(QString("Verifier%1Pointer").arg(I + 1), VerifierPointer.readAll());
        //        }

        XmlStreamWriter.writeEndElement();//NonXmlData
    }
    XmlStreamWriter.writeEndElement(); // ProgramSequence
    XmlStreamWriter.writeEndDocument(); // End of Document
//    XmlStreamWriter.device()->reset();
//    qDebug()<<"\nXML"<<XmlStreamWriter.device()->readAll();
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Reads from the IODevice to CProgramSequenceList object.
 *
 *  \iparam IODevice = Instance of the IODevice might be Buffer or File.
 *  \iparam CompleteData = bool type if true gives Complete data of object
 *
 *  \return True or False
 */
/****************************************************************************/
bool CProgramSequenceList::DeserializeContent(QIODevice& IODevice ,bool CompleteData)
{
    QXmlStreamReader XmlStreamReader;
    XmlStreamReader.setDevice(&IODevice);

    // look for node <ProgramsSequence>
    if (!Helper::ReadNode(XmlStreamReader, "ProgramsSequence")) {
        qDebug()<<"ProgramSequence NodeName:"<<XmlStreamReader.name()<<"NodeName LineNumber"<<XmlStreamReader.lineNumber();
        qDebug() << "DeserializeContent: abort reading. Node not found: ProgramsSequence";
        return false;
    }
    // Read attribute Version
    if (!XmlStreamReader.attributes().hasAttribute("Version")) {
        qDebug() << "### attribute <Version> is missing => abort reading";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_ERROR_XML_ATTRIBUTE_NOT_FOUND,
                                                   Global::tTranslatableStringList() << "ProgramSequenceList:Version", true);
        return false;
    }
    SetVersion(XmlStreamReader.attributes().value("Version").toString().toInt());
    //  Now Delete the Program list and ordered list of ProgramSequence ID's
    if(!DeleteAllProgramSequenceSteps()) {
        qDebug() << "DeserializeContent: abort reading. Deleting all the program sequence failed";
        return false;
    }

    while (!XmlStreamReader.atEnd()) {
        if (XmlStreamReader.readNext() == QXmlStreamReader::Invalid) {
            qDebug() << "Reading " << XmlStreamReader.name() << " at line number: " << XmlStreamReader.lineNumber();
            qDebug() << "Invalid Token. Error: " << XmlStreamReader.errorString();
        }
        if (XmlStreamReader.isStartElement()) {
            if (XmlStreamReader.name() == "Program") {
                CProgramSequence ProgramSequence;
                if (!ProgramSequence.DeserializeContent(XmlStreamReader, CompleteData)) {
                    return false;
                }
                // Now add this ProgramSequence
                if (!AddProgramSequenceStep(&ProgramSequence)) {
                    qDebug() << "CProgramSequenceList::Add ProgramSequence failed!";
                    return false;
                }
            }
            else if (CompleteData) {
                if (XmlStreamReader.name() == "NonXmlData") {
                    if (!ReadCompleteData(XmlStreamReader)) {
                        return false;
                    }
                }
            }
            else {
                qDebug() << "### unknown node name <" << XmlStreamReader.name().toString() << "> at line number: " << XmlStreamReader.lineNumber();
                return false;
            }
        }
        else if (XmlStreamReader.isEndElement() && XmlStreamReader.name().toString() == "ProgramsSequence") {
            //qDebug() << "It has reached the end of the particular Node , Stop reading for this Reagent";
            break; // exit from while loop
        }
    } // end of while loop


    //======NODE=END====Temporary Data Variables=========================
    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads the data from the XML stream, the data read is not part of the
 *         XML file;e.g. Flags
 *  \iparam XmlStreamReader = XML stream
 *
 *  \return true - read success , false - read failed
 */
/****************************************************************************/
bool CProgramSequenceList::ReadCompleteData(QXmlStreamReader& XmlStreamReader)
{
    if (!XmlStreamReader.attributes().hasAttribute("VerificationMode")) {
        qDebug() << "### attribute <VerificationMode> is missing";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_ERROR_XML_ATTRIBUTE_NOT_FOUND,
                                                   Global::tTranslatableStringList() << "ProgramSequenceList:VerificationMode", true);
        return false;
    }

    if (XmlStreamReader.attributes().value("VerificationMode").toString() == "true") {
        SetDataVerificationMode(true);
    }
    else {
        SetDataVerificationMode(false);
    }

    if (!XmlStreamReader.attributes().hasAttribute("FileName")) {
        qDebug() << "### attribute <FileName> is missing";
        return false;
    }
    SetFilename(XmlStreamReader.attributes().value("FileName").toString());

    //    if (!XmlStreamReader.attributes().hasAttribute("VerifierCount")) {
    //        qDebug() << "### attribute <VerifierCount> is missing";
    //        return false;
    //    }
    //
    //    int VerifierCount = XmlStreamReader.attributes().value("VerifierCount").toString().toInt();

    //    m_VerifierList.clear();

    //    for (qint32 I = 0; I < VerifierCount; I++) {

    //        if (!XmlStreamReader.attributes().hasAttribute(QString("Verifier%1Pointer").arg(I + 1))) {
    //            qDebug() << "### attribute <VerifierPointer> is missing";
    //            return false;
    //        }

    //        IVerifierInterface* p_VerifierInterface = reinterpret_cast<IVerifierInterface*>
    //                                                  (XmlStreamReader.attributes().
    //                                                   value(QString("Verifier%1Pointer").arg(I + 1))
    //                                                   .toString().toInt(0, 16));
    //        qDebug()<<"\n\n Verifier Pointer"<<p_VerifierInterface;
    //        m_VerifierList.append(p_VerifierInterface);
    //    }
    return true;
}

/****************************************************************************/
/*!
 *  \brief Output Stream Operator which streams data
 *
 *  \iparam OutDataStream = Instance of the QDataStream
 *  \iparam ProgramSequenceList = CProgramSequenceList class object
 *
 *  \return Output Stream
 */
/****************************************************************************/
QDataStream& operator <<(QDataStream& OutDataStream, const CProgramSequenceList& ProgramSequenceList) {

    CProgramSequenceList* p_TempProgramSequenceList = const_cast<CProgramSequenceList*>(&ProgramSequenceList);

    if (!p_TempProgramSequenceList->SerializeContent(*OutDataStream.device(), true)) {
        qDebug() << "CProgramSequence::Operator Streaming (SerializeContent) failed.";

        // raise an event and insert to the error list
        p_TempProgramSequenceList->m_ErrorHash.insert(EVENT_DM_STREAMOUT_FAILED,
                                              Global::tTranslatableStringList() << "ProgramSequenceList");
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_STREAMOUT_FAILED,
                                                   Global::tTranslatableStringList() << "ProgramSequenceList", true);
    }
    return OutDataStream;
}

/****************************************************************************/
/*!
 *  \brief Input stream Operator which reads the data from Input parameter.
 *
 *  \iparam InDataStream = Instance of the DataStream
 *  \iparam ProgramSequenceList = CProgramSequenceList class object
 *
 *  \return Input Stream
 */
/****************************************************************************/
QDataStream& operator >>(QDataStream& InDataStream, CProgramSequenceList& ProgramSequenceList) {

    if (!ProgramSequenceList.DeserializeContent(*InDataStream.device(), true)) {
        qDebug() << "CProgramSequence::Operator Streaming (DeSerializeContent) failed.";
        // raise an event and insert to the error list
        ProgramSequenceList.m_ErrorHash.insert(EVENT_DM_STREAMIN_FAILED,
                                       Global::tTranslatableStringList() << "ProgramSequenceList");
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_STREAMIN_FAILED,
                                                   Global::tTranslatableStringList() << "ProgramSequenceList", true);
    }
    return InDataStream;
}

/****************************************************************************/
/*!
 *  \brief Assignment Operator which copies from rhs to lhs.
 *
 *  \iparam SourceProgramSequenceList = CProgramSequenceList class object
 *
 *  \return CProgramSequence Class Object
 */
/****************************************************************************/
CProgramSequenceList& CProgramSequenceList::operator = (const CProgramSequenceList& SourceProgramSequenceList) {

    // make sure not same object
    if (this != &SourceProgramSequenceList)
    {
        QByteArray TempByteArray;
        QDataStream DataStream(&TempByteArray, QIODevice::ReadWrite);
        (void)DataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0)); //to avoid lint-534
        TempByteArray.clear();
        DataStream << SourceProgramSequenceList;
        (void)DataStream.device()->reset(); //to avoid lint-534
        bool VerificationModeLocal = GetDataVerificationMode();
        if (VerificationModeLocal) {
            SetDataVerificationMode(false);
        }
        DataStream >> *this;
        //Set back verification mode to original state
        SetDataVerificationMode(VerificationModeLocal);
    }
    return *this;
}

/****************************************************************************/
/*!
 *  \brief Reads the data and stores in CProgramSequenceList from the file
 *
 *  \iparam Filename = Name of the Input File
 *
 *  \return True or False
 */
/****************************************************************************/
bool CProgramSequenceList::Read(QString Filename)
{
    //check if file exists
    if (!QFile::exists(Filename)) {
        qDebug("File doesn't exists");
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_XML_FILE_NOT_EXISTS,
                                                   Global::tTranslatableStringList() << Filename, true);
        return false;
    }    
    bool Result = true;

    if (m_DataVerificationMode) {
        QWriteLocker locker(mp_ReadWriteLock);

        // create instance of CProgramSequenceList for verification
        CProgramSequenceList* p_DPSL_Verification = new CProgramSequenceList();

        // disable verification in clone
        p_DPSL_Verification->SetDataVerificationMode(false);

        // execute required action (Read) in clone
        Result = true;
        if (!p_DPSL_Verification->Read(Filename)) {
            Result = false;
        }
        else {
            // now check new content => call all active verifiers
            if (DoLocalVerification(p_DPSL_Verification)) {
                // if content ok, clone backwards
            }
            else {
                Result = false;
            }
        }
        // delete test clone
        delete p_DPSL_Verification;

    } else {
        QWriteLocker locker(mp_ReadWriteLock);

        // clear content
        Init();

        m_Filename = "UNDEFINED";
        QFile File (Filename);
        if (!File.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "open file failed in Read: " << Filename;
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_FILE_OPEN_FAILED,
                                                       Global::tTranslatableStringList() << Filename, true);
            return false;
        }
        if (!DeserializeContent(File, false)) {
            qDebug() << "### CProgramSequenceList::Read failed for file: " << Filename;
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_XML_DESERIALIZE_FAILED,
                                                       Global::tTranslatableStringList() << Filename, true);
            return false;
        }        
        File.close();

        //Now initialize the filename member since Read is succcess
        m_Filename = Filename;

        Result = true;
    }
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Get program Sequence Object
 *
 *  \iparam Index
 *
 *  \return  m_ProgramSequenceStepsList or NULL if not found
 */
/****************************************************************************/
const CProgramSequence* CProgramSequenceList::GetProgramSequenceStep(const unsigned int Index)
{
    if (Index < (unsigned int)m_OrderedListOfProgramSequenceIDs.count()) {
        QString Key = m_OrderedListOfProgramSequenceIDs.value(Index);
        CProgramSequence* p_ProgramSequence = m_ProgramSequenceStepsList.value(Key, NULL);
        return p_ProgramSequence;
    }
    else {
        return NULL;
    }
}
/****************************************************************************/
/*!
 *  \brief Get program Sequence Object
 *
 *  \iparam Index - Index of the program sequence list
 *  \iparam ProgramSequence - Program sequence class
 *
 *  \return  True-success else false
 */
/****************************************************************************/
//deep copy
bool CProgramSequenceList::GetProgramSequenceStep(const unsigned int Index, CProgramSequence& ProgramSequence)
{
    QReadLocker locker(mp_ReadWriteLock);

    if (Index < (unsigned int)m_OrderedListOfProgramSequenceIDs.count()) {
        QString ProgramSequenceID = m_OrderedListOfProgramSequenceIDs.value(Index);
        if (m_ProgramSequenceStepsList.contains(ProgramSequenceID)) {
            ProgramSequence = *m_ProgramSequenceStepsList.value(ProgramSequenceID); // this gives the exception if not copied
            return true;
        }
    }
    return false;
}

/****************************************************************************/
/*!
 *  \brief Deep Copies the ProgramSequence content ProgramSequence Instance by ProgramSequence ID
 *
 *  \iparam ProgramSequenceID = ID of the ProgramSequence
 *  \iparam ProgramSequence =  CProgramSequence Instance
 *
 *  \return True or False
 */
/****************************************************************************/
//deep copy
bool CProgramSequenceList::GetProgramSequenceStep(const QString ProgramSequenceID, CProgramSequence& ProgramSequence)
{
    QReadLocker locker(mp_ReadWriteLock);

    if (m_ProgramSequenceStepsList.contains(ProgramSequenceID)) {
        ProgramSequence = *m_ProgramSequenceStepsList.value(ProgramSequenceID);
        return true;
    }
    else {
        return false;
    }
}

/****************************************************************************/
/*!
 *  \brief Add program Sequence object to the list
 *  \iparam p_ProgramSequenceStep
 *
 *  \return true -Add success else false
 */
/****************************************************************************/
bool CProgramSequenceList::AddProgramSequenceStep(const CProgramSequence* p_ProgramSequenceStep)
{
    try {
        CHECKPTR(p_ProgramSequenceStep);
    }
    catch(Global::Exception &E) {
        Global::EventObject::Instance().RaiseException(E);
        m_ErrorHash.insert(EVENT_DM_PROGSEQ_OPERATION_FAILED,
                           Global::tTranslatableStringList() << "AddProgramSequenceStep");
        SetErrorList(&m_ErrorHash);
        return false;
    }

    QString  ID = const_cast<CProgramSequence*>(p_ProgramSequenceStep)->GetID();
    if (m_ProgramSequenceStepsList.contains(ID)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_ID_REPEATED,
                                                   Global::tTranslatableStringList() << ID, true);
        return false;
    }

    //Don't DELETE !!!!! deleting the cloned program list is suffice
    CProgramSequence *p_TempProgramSequence = new CProgramSequence();
    *p_TempProgramSequence = *p_ProgramSequenceStep;
    bool Result = true;
    if ((m_DataVerificationMode)) {
        CProgramSequenceList* p_DSPL_Verification = new CProgramSequenceList();

        // first lock current state for reading
        {   // code block defined for QReadLocker.
            QReadLocker locker(mp_ReadWriteLock);

            // create clone from current state
            *p_DSPL_Verification = *this;

            // disable verification in clone
            p_DSPL_Verification->SetDataVerificationMode(false);

            // execute required action (AddProgramSequenceStep) in clone
            Result = p_DSPL_Verification->AddProgramSequenceStep(p_TempProgramSequence);
            if (Result) {
                // now check new content => call all active verifiers
                Result = DoLocalVerification(p_DSPL_Verification);
                if (!Result) {
                    //Store errors.
                    // Since we are going to delete p_DSPL_Verification
                    // We need to store the errors generated by it.
                    // For now Errors are not copied by assignment or copy constructors.
                    ListOfErrors_t ErrorList = p_DSPL_Verification->GetErrorList();
                    if (!ErrorList.isEmpty()) {
                        // If the control reaches here means Error hash is empty
                        // Considering only the first element in Hash since
                        // verfier can atmost add only one Hash has to the error list
                        m_ErrorHash = *(ErrorList.first());
                        SetErrorList(&m_ErrorHash);
                    }
                }
            }
        }
        if (Result) {
            // if content ok, clone backwards
            *this = *p_DSPL_Verification;
            Result = true;
        }

        // delete test clone
        delete p_DSPL_Verification;

    }
    else {
        QWriteLocker locker(mp_ReadWriteLock);
        m_ProgramSequenceStepsList.insert(ID, p_TempProgramSequence);
        m_OrderedListOfProgramSequenceIDs.append(ID);
        Result = true;
    }
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Add Leica program Sequence object to the list
 *  \iparam p_ProgramSequenceStep
 *
 *  \return true -Add success else false
 */
/****************************************************************************/
bool CProgramSequenceList::AddLeicaProgramSequenceStep(const CProgramSequence* p_ProgramSequenceStep)
{
    try {
        CHECKPTR(p_ProgramSequenceStep);
    }
    catch(Global::Exception &E) {
        Global::EventObject::Instance().RaiseException(E);
        m_ErrorHash.insert(EVENT_DM_PROGSEQ_OPERATION_FAILED,
                           Global::tTranslatableStringList() << "AddLeicaProgramSequenceStep");
        SetErrorList(&m_ErrorHash);
        return false;
    }

    QString  ID = const_cast<CProgramSequence*>(p_ProgramSequenceStep)->GetID();
    if (m_ProgramSequenceStepsList.contains(ID)) {
        return false;
    }

    //Don't DELETE !!!!! deleting the cloned program list is suffice
    CProgramSequence *p_TempProgramSequence = new CProgramSequence();
    *p_TempProgramSequence = *p_ProgramSequenceStep;
    bool Result = true;
    if ((m_DataVerificationMode)) {
        CProgramSequenceList* p_DSPL_Verification = new CProgramSequenceList();

        // first lock current state for reading
        {   // code block defined for QReadLocker.
            QReadLocker locker(mp_ReadWriteLock);

            // create clone from current state
            *p_DSPL_Verification = *this;

            // disable verification in clone
            p_DSPL_Verification->SetDataVerificationMode(false);

            // execute required action (AddLeicaProgramSequenceStep) in clone
            Result = p_DSPL_Verification->AddLeicaProgramSequenceStep(p_TempProgramSequence);
            if (Result) {
                // now check new content => call all active verifiers
                Result = DoLocalVerification(p_DSPL_Verification);
                if (!Result) {
                    //Store errors.
                    // Since we are going to delete p_DSPL_Verification
                    // We need to store the errors generated by it.
                    // For now Errors are not copied by assignment or copy constructors.
                    ListOfErrors_t ErrorList = p_DSPL_Verification->GetErrorList();
                    if (!ErrorList.isEmpty()) {
                        // If the control reaches here means Error hash is empty
                        // Considering only the first element in Hash since
                        // verfier can atmost add only one Hash has to the error list
                        m_ErrorHash = *(ErrorList.first());
                        SetErrorList(&m_ErrorHash);
                    }
                }
            }
        }
        if (Result) {
            // if content ok, clone backwards
            *this = *p_DSPL_Verification;
            Result = true;
        }

        // delete test clone
        delete p_DSPL_Verification;

    }
    else {
        QWriteLocker locker(mp_ReadWriteLock);
        // counter the program sequence
        int ProgramSeqIDCount = 0;

        // check for the last Leica program sequence step in the list
        for (ProgramSeqIDCount = 0; ProgramSeqIDCount < m_OrderedListOfProgramSequenceIDs.count(); ProgramSeqIDCount++) {
            if (m_OrderedListOfProgramSequenceIDs.value(ProgramSeqIDCount).at(0) != 'L') {
                // found the first User program
                break;
            }
        }
        // insert the item
        m_ProgramSequenceStepsList.insert(ID, p_TempProgramSequence);
        // If all the programs all Leica then counter value will be last, then it requires to append the list
        if (ProgramSeqIDCount >= m_OrderedListOfProgramSequenceIDs.count()) {
            m_OrderedListOfProgramSequenceIDs.append(ID);
        }
        else {
            // insert the item
            m_OrderedListOfProgramSequenceIDs.insert(ProgramSeqIDCount, ID);
        }

        Result = true;
    }
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Update ProgramSequenceStep
 *  \iparam p_ProgramSequence
 *
 *  \return true -Update success else false
 */
/****************************************************************************/
bool CProgramSequenceList::UpdateProgramSequenceStep(const CProgramSequence* p_ProgramSequence)
{
    try {
        CHECKPTR(p_ProgramSequence);
    }
    catch(Global::Exception &E) {
        Global::EventObject::Instance().RaiseException(E);
        m_ErrorHash.insert(EVENT_DM_PROGSEQ_OPERATION_FAILED,
                           Global::tTranslatableStringList() << "UpdateProgramSequenceStep");
        SetErrorList(&m_ErrorHash);
        return false;
    }

    QString ID = const_cast<CProgramSequence*>(p_ProgramSequence)->GetID();
    if (!m_ProgramSequenceStepsList.contains(ID)) {
        // there is no ProgramSequenceStep with the given ProgramSequenceStepID
        m_ErrorHash.insert(EVENT_DM_REAGENT_ID_DOESTNOT_EXISTS,
                           Global::tTranslatableStringList() << ID);
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_ID_DOESTNOT_EXISTS,
                                                   Global::tTranslatableStringList() << ID, true);
        SetErrorList(&m_ErrorHash);
        return false;
    }

    bool Result = false;
    if (m_DataVerificationMode) {
        CProgramSequenceList* p_DRL_Verification = new CProgramSequenceList();

        // first lock current state for reading
        {   // code block defined for QReadLocker.
            QReadLocker locker(mp_ReadWriteLock);

            // create clone from current state
            *p_DRL_Verification = *this;

            // disable verification in clone
            p_DRL_Verification->SetDataVerificationMode(false);

            // execute required action (UpdateProgramSequence) in clone
            Result = p_DRL_Verification->UpdateProgramSequenceStep(p_ProgramSequence);
            if (Result) {
                // now check new content => call all active verifiers
                Result = DoLocalVerification(p_DRL_Verification);
                if (!Result) {
                    //Store errors.
                    // Since we are going to delete p_DRL_Verification
                    // We need to store the errors generated by it.
                    // For now Errors are not copied by assignment or copy constructors.
                    ListOfErrors_t ErrorList = p_DRL_Verification->GetErrorList();
                    if (!ErrorList.isEmpty()) {
                        // If the control reaches here means Error hash is empty
                        // Considering only the first element in Hash since
                        // verfier can atmost add only one Hash has to the error list
                        m_ErrorHash = *(ErrorList.first());
                        SetErrorList(&m_ErrorHash);
                    }
                }
            }
        }
        if (Result) {
            // if content ok, clone backwards
            *this = *p_DRL_Verification;
            Result = true;
        }

        // delete test clone
        delete p_DRL_Verification;


    }
    else {
        QWriteLocker locker(mp_ReadWriteLock);
        // deep copy is happening for the program sequence step
        *m_ProgramSequenceStepsList[ID] = *p_ProgramSequence;
        Result = true;
    }

    return Result;
}

/****************************************************************************/
/*!
 *  \brief Delete Program Sequence Step
 *  \iparam StepID
 *
 *  \return true -Delete success else false
 */
/****************************************************************************/
bool CProgramSequenceList::DeleteProgramSequenceStep(const QString StepID)
{
    try {
        if (m_ProgramSequenceStepsList.contains(StepID)) {
            //get ProgramStep from StepList and free memory
            delete m_ProgramSequenceStepsList.value(StepID);

            //remove pointer to ProgramStep from StepList
            m_ProgramSequenceStepsList.remove(StepID);
           // qDebug("ProgramStep deleted");

            //remove StepID from ID list
            int MatchIndex = -1;
            for (int i=0; i<m_OrderedListOfProgramSequenceIDs.count(); i++) {
                if (m_OrderedListOfProgramSequenceIDs[i] == StepID) {
                    MatchIndex = i;
                    break;
                }
            }
            // MatchIndex MUST never be -1 !!!
            Q_ASSERT(MatchIndex != -1);
            m_OrderedListOfProgramSequenceIDs.removeAt(MatchIndex);

            return true;
        }
        else {
            // there is no program step with the given StepID
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_ID_DOESTNOT_EXISTS,
                                                       Global::tTranslatableStringList() << StepID, true);
            return false;
        }
    }
    catch (...) {
        qDebug() << "### exception in CProgramSequenceList::DeleteProgramSequenceStep";
        m_ErrorHash.insert(EVENT_DM_PROGSEQ_OPERATION_FAILED,
                           Global::tTranslatableStringList() << "DeleteProgramSequenceStep");
        SetErrorList(&m_ErrorHash);
        return false;
    }
}

/****************************************************************************/
/*!
 *  \brief Delete Program Sequence step based on Index
 *  \iparam Index
 *
 *  \return true -Delete succes else false
 */
/****************************************************************************/
bool CProgramSequenceList::DeleteProgramSequenceStep(const unsigned int Index)
{
    if (Index < (unsigned int)m_OrderedListOfProgramSequenceIDs.count()) {
        QString StepID = m_OrderedListOfProgramSequenceIDs.value(Index);
        return DeleteProgramSequenceStep(StepID);
    }
    else {
        return false;
    }
}

/****************************************************************************/
/*!
 *  \brief Delete all program sequence steps
 *
 *  \return true- Delete success else false
 */
/****************************************************************************/
bool CProgramSequenceList::DeleteAllProgramSequenceSteps()
{
    bool Result = true;

    while (m_ProgramSequenceStepsList.size() > 0) {
        if (DeleteProgramSequenceStep(0) == false) {
            Result = false;
        }
    }
    m_ProgramSequenceStepsList.clear();
    m_OrderedListOfProgramSequenceIDs.clear();

    return Result;
}

/****************************************************************************/
/*!
 *  \brief  Move Program Sequence Steps
 *  \iparam FromIndex
 *  \iparam ToIndex
 *  \return Changed something (true) or not (false)
 */
/****************************************************************************/
bool CProgramSequenceList::MoveProgramStep(qint32 FromIndex, qint32 ToIndex)
{
    if (FromIndex >= 0 && FromIndex < m_OrderedListOfProgramSequenceIDs.size() && ToIndex >= 0 && ToIndex < m_OrderedListOfProgramSequenceIDs.size()) {
        m_OrderedListOfProgramSequenceIDs.move(FromIndex, ToIndex);
        return true;
    }
    return false;
}

/****************************************************************************/
/*!
 *  \brief  Retrieves a list of startable program ID's for used programs
 *
 *  \return Startable Program ID List
 */
/****************************************************************************/
QStringList CProgramSequenceList::GetStartableProgramIDList()
{
    QStringList ProgramIDList;
    for (qint32 I = 0; I < GetNumberOfPrograms(); I++) {
        const CProgramSequence *p_ProgramSequence = GetProgramSequenceStep(I);
        if (p_ProgramSequence && (p_ProgramSequence->GetUsedStatus())
                && (p_ProgramSequence->GetStartCapableStatus() && p_ProgramSequence->GetStartCapableDynamicStatus())) {
            ProgramIDList.append(p_ProgramSequence->GetID());
        }
    }
    return ProgramIDList;
}
/****************************************************************************/
/*!
 *  \brief  Retrieves a list of all program ID's
 *
 *  \return Program ID List
 */
/****************************************************************************/
QStringList CProgramSequenceList::GetProgramIDList()
{
    QStringList ProgramIDList;
    for (qint32 I = 0; I < GetNumberOfPrograms(); I++) {
        const CProgramSequence *p_ProgramSequence = GetProgramSequenceStep(I);
        ProgramIDList.append(p_ProgramSequence->GetID());
    }
    return ProgramIDList;
}
/****************************************************************************/
/*!
 *  \brief  Retrieves a list of used program ID's
 *
 *  \return Used Program ID List
 */
/****************************************************************************/
QStringList CProgramSequenceList::GetUsedProgramIDList()
{
    QStringList ProgramIDList;
    CProgramSequence ProgramSequence;
    for (qint32 I = 0; I < GetNumberOfPrograms(); I++) {
        if (GetProgramSequenceStep(I, ProgramSequence) && (ProgramSequence.GetUsedStatus())) {
            ProgramIDList.append(ProgramSequence.GetID());
        }
    }
    return ProgramIDList;
}

}//End of namespace DataManager
