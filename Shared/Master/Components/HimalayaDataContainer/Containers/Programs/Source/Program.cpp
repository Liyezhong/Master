/****************************************************************************/
/*! \file Program.cpp
 *
 *  \brief Implementation file for class CProgram.
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

#include <QDebug>

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "DataManager/Helper/Include/Helper.h"
#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Default Constructor
 */
/****************************************************************************/
CProgram::CProgram() : CProgramBase("0","",""), m_Color(""), m_Locked(false), m_ExpandedListUpToDate(false), mp_DryStationStep(NULL), mp_SIDStationStep(NULL), m_Modified(false)
{
    Init();
}

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam ID = program ID
 */
/****************************************************************************/
CProgram::CProgram(const QString ID) : CProgramBase(ID,"",""), m_Color(""), m_Locked(false), m_ExpandedListUpToDate(false), mp_DryStationStep(NULL), mp_SIDStationStep(NULL), m_Modified(false)
{
    Init();
}

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam Color = Color associated with the program
 *  \iparam ProgramIsLocked = True for Leica program, false for user defined program
 *  \iparam ID = program ID
 *  \iparam ShortName = program short name
 *  \iparam LongName = program long name
 */
/****************************************************************************/
CProgram::CProgram(const QString Color, const bool ProgramIsLocked, const QString ID, const QString ShortName, const QString LongName)
    : CProgramBase(ID, ShortName, LongName), m_Color(Color), m_Locked(ProgramIsLocked), m_ExpandedListUpToDate(false), mp_DryStationStep(NULL), mp_SIDStationStep(NULL), m_Modified(false)
{
    Init();
}

/****************************************************************************/
/*!
 *  \brief Initialize object
 */
/****************************************************************************/
void CProgram::Init()
{
    m_ExpandedStepList.clear();
    m_ReagentIDList.clear();
}

/****************************************************************************/
/*!
 *  \brief Copy Constructor
 *
 *  \iparam SourceProgram
 *
 */
/****************************************************************************/
CProgram::CProgram(const CProgram& SourceProgram):CProgramBase(SourceProgram),
    mp_DryStationStep(NULL),
    mp_SIDStationStep(NULL)
{
    Init();
    try {
        *this = SourceProgram;
    }
    catch(...) {
        qDebug()<<" CProgram::Copy constructor failed";
    }

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CProgram::~CProgram()
{
    try {
        m_ReagentIDList.clear();
        for (qint32 I = 0; I < m_ExpandedStepList.size(); I++) {
            delete m_ExpandedStepList.at(I);
        }
    }
    catch (...){
        //to please lint.
    }

}

/****************************************************************************/
/*!
 *  \brief Refreshes the expanded step list
 *  \return true - refresh success , false - refresh failure
 */
/****************************************************************************/
bool CProgram::RefreshExpandedStepList()
{
    SetExpandedListState(false);
    for (qint32 I = 0; I < m_ExpandedStepList.size(); I++) {
        delete m_ExpandedStepList.at(I);
    }
    m_ExpandedStepList.clear();



    for (qint32 i = 0; i < GetNumberOfSteps(); i++) {
        CProgramStep* p_NewPStep = new CProgramStep();
        *p_NewPStep = *GetProgramStep(i);
         m_ExpandedStepList.append(p_NewPStep);
    }

    return true;
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
bool CProgram::SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData)
{
    //QReadLocker Locker(mp_ReadWriteLock);
    bool Result = true;

    XmlStreamWriter.writeStartElement("Program");
    XmlStreamWriter.writeAttribute("ID", GetID());
    XmlStreamWriter.writeAttribute("ShortName", GetShortName());
    XmlStreamWriter.writeAttribute("LongName", GetLongName());
    XmlStreamWriter.writeAttribute("Color", GetColor());
    if (IsLocked()) {
        XmlStreamWriter.writeAttribute("Locked", "true");
    } else {
        XmlStreamWriter.writeAttribute("Locked", "false");
    }

    XmlStreamWriter.writeStartElement("StepList");
    XmlStreamWriter.writeAttribute("NextStepID", GetNextFreeStepID(false));

    if (CompleteData) {
        if (m_Modified) {
            XmlStreamWriter.writeAttribute("Modified", "true");
        }
        else {
            XmlStreamWriter.writeAttribute("Modified", "false");
        }
    }


    for (int i = 0; i < GetNumberOfStepsInExpandedList() ; i++) {
        CProgramStep *p_ProgStep = GetProgramStepExpanded(i);
        if (p_ProgStep != NULL) {
            if (!p_ProgStep->SerializeContent(XmlStreamWriter, CompleteData)) {
                qDebug("DataManager::CProgramStep SerializeContent failed ");
                return false;
            }
        }
    }
    XmlStreamWriter.writeEndElement(); // StepList
    XmlStreamWriter.writeEndElement(); // Program
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
bool CProgram::DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData)
{
//    QWriteLocker Locker(mp_ReadWriteLock);
    bool Result = true;
    // ID
    if (!XmlStreamReader.attributes().hasAttribute("ID")) {
        qDebug() << "### attribute <ID> is missing => abort reading";
        return false;
    }

    SetID(XmlStreamReader.attributes().value("ID").toString());
    // ShortName
    if (!XmlStreamReader.attributes().hasAttribute("ShortName")) {
        qDebug() << "### attribute <ShortName> is missing => abort reading";
        return false;
    }
    SetShortName(XmlStreamReader.attributes().value("ShortName").toString());

    // LongName
    if (!XmlStreamReader.attributes().hasAttribute("LongName")) {
        qDebug() << "### attribute <LongName> is missing => abort reading";
        return false;
    }
    SetLongName(XmlStreamReader.attributes().value("LongName").toString());

    // Color
    if (!XmlStreamReader.attributes().hasAttribute("Color")) {
        qDebug() << "### attribute <Color> is missing => abort reading";
        return false;
    }
    SetColor(XmlStreamReader.attributes().value("Color").toString());

    // Locked
    if (!XmlStreamReader.attributes().hasAttribute("Locked")) {
        qDebug() << "### attribute <Locked> is missing => abort reading";
        return false;
    }
    if (XmlStreamReader.attributes().value("Locked").toString().toUpper() == "TRUE") {
        LockProgram();
    }
    else {
        m_Locked = false;
    }
    // Look for node <StepList>
    if (!Helper::ReadNode(XmlStreamReader, "StepList")) {
        qDebug() << "DeserializeContent: abort reading. Node not found: StepList";
        return false;
    }

    // get attribute NextStepID
    if (!XmlStreamReader.attributes().hasAttribute("NextStepID")) {
        qDebug() << "NextStepID missing";
        return false;
    } else {
        SetNextFreeStepID(XmlStreamReader.attributes().value("NextStepID").toString());
    }

    if (CompleteData) {
        if (!XmlStreamReader.attributes().hasAttribute("Modified")) {
            qDebug() << "### attribute <Modified> is missing => abort reading";
            return false;
        }
        if (XmlStreamReader.attributes().value("Modified").toString().toUpper() == "TRUE") {
            m_Modified = true;
        }
        else {
            m_Modified = false;
        }
    }

    //Delete program steps
    (void)DeleteAllProgramSteps();
    m_ExpandedStepList.clear();
    m_ReagentIDList.clear();
    while(!XmlStreamReader.atEnd()) {
        if (static_cast<int>(XmlStreamReader.readNext()) == 1) {
            qDebug() << "Reading " << XmlStreamReader.name() << " at line number: " << XmlStreamReader.lineNumber();
            qDebug() << "Invalid Token. Error: " << XmlStreamReader.errorString();
            return false;
        }
        if (XmlStreamReader.isStartElement()) {
            if (XmlStreamReader.name() == "Step") {
                //Create a program step
                CProgramStep* p_NewProgramStep = CreateProgramStep("Dummy");
                if (p_NewProgramStep != NULL) {
                    if (!p_NewProgramStep->DeserializeContent(XmlStreamReader, CompleteData)) {
                        qDebug() << "CProgramStep Deserialize failed!";
                        delete p_NewProgramStep;
                        return false;
                    }
                    //Before adding the step to program , check if its a special step , e.g. SID, DRY station.
                    if ((p_NewProgramStep->GetReagentID() != SLIDE_ID_SPECIAL_REAGENT )
                            && (p_NewProgramStep->GetReagentID() != "S9")) {
                        CProgramStep* p_OriginalProgramStep = new CProgramStep(*p_NewProgramStep);
                        m_ExpandedStepList.append(p_NewProgramStep);
                        if (m_Modified) {
                            if (p_NewProgramStep->GetReagentID() == "S7") {
                                p_OriginalProgramStep->SetReagentID("S8");
                            }
                            else if (p_NewProgramStep->GetReagentID() == "S8") {
                                p_OriginalProgramStep->SetReagentID("S7");
                            }
                        }
                        // Now add this Step
                        if (!CProgramBase::AddProgramStep(p_OriginalProgramStep)) {
                            qDebug() << "CProgram::Add ProgramStep failed!";
                            delete p_NewProgramStep;
                            delete p_OriginalProgramStep;
                            return false;
                        }
                    }
                    else {
                        m_ExpandedStepList.append(p_NewProgramStep);
                    }
                    m_ReagentIDList.append(p_NewProgramStep->GetReagentID());
                }
            }
            else {
                qDebug() << "### unknown node name <" << XmlStreamReader.name().toString() << "> at line number: " << XmlStreamReader.lineNumber();
                return false;
            }
            //Remove Duplicates from reagent ID list
            m_ReagentIDList.removeDuplicates();
        }
        else if (XmlStreamReader.isEndElement() && XmlStreamReader.name().toString() == "StepList") {
            //qDebug() << "It has reached the end of the particular Node , Stop reading for this Reagent";
            break; // exit from while loop
        }
    }
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Output Stream Operator which streams data
 *
 *  \iparam OutDataStream = Instance of the QDataStream
 *  \iparam Program = CProgram class object
 *
 *  \return Output Stream
 */
/****************************************************************************/
QDataStream& operator << (QDataStream& OutDataStream, const CProgram& Program)
{

    CProgram* p_TempProgram = const_cast<CProgram*>(&Program);

    QXmlStreamWriter XmlStreamWriter;
    XmlStreamWriter.setDevice(OutDataStream.device());

    if (!p_TempProgram->SerializeContent(XmlStreamWriter, true)) {
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
 *  \iparam Program = CProgram class object
 *
 *  \return Input Stream
 */
/****************************************************************************/
QDataStream& operator >> (QDataStream& InDataStream, CProgram& Program)
{
    QXmlStreamReader XmlStreamReader;
    XmlStreamReader.setDevice(InDataStream.device());
    (void)XmlStreamReader.readElementText(QXmlStreamReader::IncludeChildElements); //lint -e534
    if (!Helper::ReadNode(XmlStreamReader, QString("Program"))) {
        qDebug() << "CProgram::Operator Streaming (DeSerializeContent) Node not found: Program";
        THROWARG(Global::EVENT_GLOBAL_UNKNOWN_STRING_ID, Global::tTranslatableStringList() << FILE_LINE);
    }

    if (!Program.DeserializeContent(XmlStreamReader, true)) {
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
 *  \iparam SourceProgram  = Program to assign from
 *
 *  \return CProgram Class Object
 */
/****************************************************************************/
CProgram& CProgram::operator = (const CProgram& SourceProgram)
{
    // make sure not same object
    if (this != &SourceProgram) {
        CProgramBase::operator =(SourceProgram);
        QByteArray TempByteArray;
        QDataStream DataStream(&TempByteArray, QIODevice::ReadWrite);
        DataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
        TempByteArray.clear();
        try {
            DataStream << SourceProgram;
            (void)DataStream.device()->reset();
            DataStream >> *this;
            SetExpandedListState(true);
        }
        catch(...) {
            //! \todo re-throw or Just log ? delete memory allocated ?
            (void)DeleteAllProgramSteps();
            delete mp_DryStationStep;
            delete mp_SIDStationStep;
            qDebug()<<"CProgram::Assignment failed";
        }
    }
    return *this;
}

/****************************************************************************/
/*!
 *  \brief  Retrieves all the stations used by the program
 *
 *  \return Station ID list
 */
/****************************************************************************/
QStringList const & CProgram::GetStationsUsedByProgram()
{
    for(qint32 I = 0; I < m_ExpandedStepList.count(); I++) {
        CProgramStep *p_Step = m_ExpandedStepList.value(I, NULL);
        if (p_Step) {
            m_StationList.append(p_Step->GetStationIDList());
        }
        m_StationList.removeDuplicates();
    }
    return m_StationList;
}

/****************************************************************************/
/*!
 *  \brief  Updates Expanded steps
 *
 *  \iparam SourceProgram
 */
/****************************************************************************/
void CProgram::UpdateExpandedSteps(const CProgram& SourceProgram)
{
    try {
        if (RefreshExpandedStepList() && (&SourceProgram != this)) {
            if (!SourceProgram.m_ExpandedStepList.isEmpty()) {
                CProgramStep *p_StepSource = NULL;
                p_StepSource = const_cast<CProgramStep *>(SourceProgram.m_ExpandedStepList.at(0));
                CHECKPTR(p_StepSource);
                //We dont create SID step for the first time. We add SID step only when rack is inserted in the device.
                //Hence , for the first time Expanded step contains only Normal steps , it wont contain
                // SID /dry station steps
                if (p_StepSource->GetReagentID() == SLIDE_ID_SPECIAL_REAGENT) {
                    //Internally added to expanded step list
                    CProgramStep * p_SIDStep = CreateSIDStationStep(p_StepSource->GetStepID());
                    CHECKPTR(p_SIDStep);
                    p_SIDStep->SetStationIDList(p_StepSource->GetStationIDList());
                }
                //Now check if Dry station was inserted. If so, insert Dry steps.
                for (qint32 I = 0; I < m_ExpandedStepList.count(); I++) {
                    CProgramStep *p_ExpandedStepSource = NULL;
                    p_ExpandedStepSource = SourceProgram.GetProgramStepExpanded(I);
                    CHECKPTR(p_ExpandedStepSource);
                    if (p_ExpandedStepSource->GetReagentID() == "S9") {
                        CProgramStep* p_DryStationStep = CreateDryStationStep(p_ExpandedStepSource->GetStepID(),
                                                                              p_ExpandedStepSource->GetStationIDList());
                        m_ExpandedStepList.insert(I, p_DryStationStep);
                    }
                }
            }
        }
    }
    catch (...) {
        qDebug()<<"### Update Expanded Step Failed";
    }

    SetExpandedListState(true);
}

/****************************************************************************/
/*!
 *  \brief  Create Dry Station Step
 *
 *  \iparam StepID
 *  \iparam StationIDList
 *
 *  \return DryStationStep
 */
/****************************************************************************/
CProgramStep* CProgram::CreateDryStationStep(QString StepID, QStringList StationIDList)
{
    mp_DryStationStep = new CProgramStep();
    mp_DryStationStep->SetStepID(StepID);
    mp_DryStationStep->SetReagentID("S9");
    mp_DryStationStep->SetMinDurationInSeconds(1);
    mp_DryStationStep->SetMaxDurationInPercent(MAX_DURATION_PERCENTAGE_DRY_STATION_STEP);
    mp_DryStationStep->SetNumberOfParallelStations(0);
    mp_DryStationStep->SetStationIDList(StationIDList);
    mp_DryStationStep->SetIntensity(0);
    return mp_DryStationStep;
}

/****************************************************************************/
/*!
 *  \brief  Create SID Station Step
 *
 *  \iparam StepID
 *
 *  \return SIDStationStep
 */
/****************************************************************************/
CProgramStep* CProgram::CreateSIDStationStep(QString StepID)
{
    mp_SIDStationStep = new CProgramStep(StepID, SLIDE_ID_SPECIAL_REAGENT, 1,
                                         MAX_DURATION_PERCENTAGE_SID_STATION_STEP,
                                         false, 0, 0);
    m_ExpandedStepList.insert(0, mp_SIDStationStep);
    return mp_SIDStationStep;

}

/****************************************************************************/
/*!
 *  \brief  Deletes the progarm step
 *  \iparam Index
 *  \return true - delete success , false - delete failure
 */
/****************************************************************************/
bool CProgram::DeleteProgramStep(const unsigned int Index)
{
    const CProgramStep *p_ProgramStep = GetProgramStep(Index);
    QString ReagentId = p_ProgramStep->GetReagentID();
    bool Result = CProgramBase::DeleteProgramStep(Index);
    for (qint32 I = 0; I < m_ExpandedStepList.size(); I++) {
        CProgramStep *p_ExpandedStep = m_ExpandedStepList.at(I);
        if (p_ExpandedStep && (p_ExpandedStep->GetReagentID() == ReagentId)) {
            delete m_ExpandedStepList.takeAt(I);
            break;
        }
    }
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Adds program step to program
 *  \iparam p_ProgramStep = Program step to add
 *
 *  \return true = add success , false - add failed
 */
/****************************************************************************/
bool CProgram::AddProgramStep(CProgramStep* p_ProgramStep)
{
    bool Result = CProgramBase::AddProgramStep(p_ProgramStep);
    CProgramStep *p_NewProgramStep = new CProgramStep(*p_ProgramStep);
    //insert step in expanded StepList. Expanded Step list will be having only original steps when this
    // function is called.
    m_ExpandedStepList.append(p_NewProgramStep);
    return Result;
}

bool CProgram::MoveProgramStep(qint32 FromIndex, qint32 ToIndex)
{
    if (FromIndex >= 0 && FromIndex < m_ExpandedStepList.size() && ToIndex >= 0 && ToIndex < m_ExpandedStepList.size()) {
        //First we need to change order in Expanded step list
        CProgramStep const *p_ProgramStepFrom = GetProgramStep(FromIndex);
        CProgramStep const *p_ProgramStepTo = GetProgramStep(ToIndex);
        QString FromReagentId = p_ProgramStepFrom->GetReagentID();
        QString ToReagentId = p_ProgramStepTo->GetReagentID();
        qint32 FromIndexExpanded = -1;
        qint32 ToIndexExpanded = -1;
        CProgramStep *p_ExpandedStep = NULL;
        for (qint32 I = 0 ; I < m_ExpandedStepList.count(); I++) {
            p_ExpandedStep = m_ExpandedStepList.at(I);
            if (p_ExpandedStep) {
                if (p_ExpandedStep->GetReagentID() == FromReagentId) {
                    FromIndexExpanded = I;
                }
                else if (p_ExpandedStep->GetReagentID() == ToReagentId) {
                    ToIndexExpanded = I;
                }
            }
            else {
                return false;
            }
        }
        if (FromIndexExpanded != -1 && ToIndexExpanded != -1) {
            m_ExpandedStepList.move(FromIndexExpanded, ToIndexExpanded);
        }

        return (CProgramBase::MoveProgramStep(FromIndex, ToIndex));
    }
    //Should never come here
    return false;
}

bool CProgram::UpdateProgramStep(CProgramStep *p_ProgramStep)
{
    const CProgramStep *p_ExistingStep = GetProgramStep(p_ProgramStep->GetStepID());
    for (qint32 I = 0; I < m_ExpandedStepList.size(); I++) {
        CProgramStep *p_ExpandedProgramStep = m_ExpandedStepList.at(I);
        if (QString::compare(p_ExpandedProgramStep->GetReagentID(), p_ExistingStep->GetReagentID()) == 0) {
            *p_ExpandedProgramStep = *p_ProgramStep;
        }
    }
    bool Result = CProgramBase::UpdateProgramStep(p_ProgramStep);
    return Result;
}

}  // namespace DataManager

