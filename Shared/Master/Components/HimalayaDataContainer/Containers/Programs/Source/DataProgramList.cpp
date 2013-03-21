/****************************************************************************/
/*! \file DataProgramList.cpp
 *
 *  \brief Implementation file for class CDataProgramList.
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
#include <QFile>
#include <QReadLocker>
#include <QWriteLocker>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Global/Include/Exception.h"
#include "Global/Include/EventObject.h"
#include "Global/Include/Utils.h"
#include "DataManager/Helper/Include/Helper.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"



namespace DataManager {
/****************************************************************************/
/*!
 *  \brief Default Constructor
 */
/****************************************************************************/
CDataProgramList::CDataProgramList() : CDataContainerBase(), m_Version(0), m_DataVerificationMode(true), m_Filename(""), mp_ReadWriteLock(NULL)
{
    mp_ReadWriteLock = new QReadWriteLock(QReadWriteLock::Recursive);

//    m_VerifierList.clear();

    Init();
}

/****************************************************************************/
/*!
 *  \brief Copy Constructor
 *
 *  \iparam ProgramList = Instance of the ProgramList class
 *
 */
/****************************************************************************/
CDataProgramList::CDataProgramList(const CDataProgramList& ProgramList) :CDataContainerBase(),
    m_Version(0), m_DataVerificationMode(true), m_Filename(""), mp_ReadWriteLock(NULL)
{
    mp_ReadWriteLock = new QReadWriteLock(QReadWriteLock::Recursive);
    Init();
    //! \todo catch exceptions
    *this = ProgramList;
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDataProgramList::~CDataProgramList()
{
    //Ignoring return value
    try {
        (void)DeleteAllPrograms(); //to avoid lint warning 534
    }
    catch(...) {
        //to please PClint
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
 *  \brief Initializes the object
 */
/****************************************************************************/
void CDataProgramList::Init()
{
    QWriteLocker Locker(mp_ReadWriteLock);
    m_Version = 0;
    m_Filename = "";

    if (!DeleteAllPrograms()) {
        //throw error ...
        THROWARG(Global::EVENT_GLOBAL_ERROR_NULL_POINTER, Global::tTranslatableStringList() << FILE_LINE);
    }

    m_NextProgramID = "0";   // this has to be a persistent value later (maybe stored in a file)

}

/****************************************************************************/
/*!
 *  \brief Reads an xml file and fill programlist object
 *
 *  \iparam Filename = Filename to read
 *
 *  \return true- read success , false - read failed
 */
/****************************************************************************/
bool CDataProgramList::Read(QString Filename)
{
    //check if file exists
    if (!QFile::exists(Filename)) {
        return false;
    }

    bool Result = true;

    if (m_DataVerificationMode) {
        //To make sure other threads cant write when reading is active
        QWriteLocker locker(mp_ReadWriteLock);

        // create instance of CDataProgramList for verification
        CDataProgramList* p_DPL_Verification = new CDataProgramList();

        // create clone from current state
        *p_DPL_Verification = *this;

        // disable verification in clone
        p_DPL_Verification->SetDataVerificationMode(false);

        // execute required action (Read) in clone
        Result = true;
        if (!p_DPL_Verification->Read(Filename)) {
            Result = false;
        } else {

            // now check new content => call all active verifiers
            if (DoLocalVerification(p_DPL_Verification)) {
                // if content ok, clone backwards
                *this = *p_DPL_Verification;
                Result = true;
            }
            else {
                Result = false;
            }
        }
        // delete test clone
        delete p_DPL_Verification;
    }
    else {
        QWriteLocker locker(mp_ReadWriteLock);

        // clear content
        Init();

        // Initialise the m_Filename to a known string "UNDEFINED"
        m_Filename = "UNDEFINED";

        QFile File(Filename);
        if (!File.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "open file failed in Read: " << Filename;
            return false;
        }

        if (!DeserializeContent(File, false)) {
            qDebug() << "### CDataProgramList::Read failed for file during deserializing: " << Filename;
            return false;
        }

        //Now initialize the filename member since Read is succcess
        m_Filename = Filename;

        File.close();

        return true;
    }
    return Result;
}




/****************************************************************************/
/*!
 *  \brief Function to retrieve program for given index
 *  \iparam Index = Index in program list
 *
 *  \return Program associated with the index
 */
/****************************************************************************/
CProgram* CDataProgramList::GetProgram(const unsigned int Index)
{
    if (Index < (unsigned int)m_OrderedListOfProgramIDs.count()) {
        QString Key = m_OrderedListOfProgramIDs.value(Index);
        CProgram* p_Prog = m_ProgramList.value(Key, NULL);
        return p_Prog;
    }
    return NULL;
}

/****************************************************************************/
/*!
 *  \brief Function to retrieve program for given index. Program passed is
 *         Updated with the program at "Index" in the list
 *
 *  \iparam Index = Index in the program list
 *  \iparam Program = Reference to program
 *
 *  \return true - success , false - failure
 */
/****************************************************************************/
bool CDataProgramList::GetProgram(const unsigned int Index, CProgram& Program)
{
    QReadLocker locker(mp_ReadWriteLock);

    bool Result = true;
    if (Index < (unsigned int)m_OrderedListOfProgramIDs.count()) {
        QString ProgramID = m_OrderedListOfProgramIDs.value(Index);
        if (m_ProgramList.contains(ProgramID)) {
            DataManager::CProgram *p_ProgTemp = m_ProgramList.value(ProgramID, NULL);
            if (p_ProgTemp != NULL) {
                Program = *p_ProgTemp;
            } else {
                Q_ASSERT(p_ProgTemp != NULL);
            }
        }
    }
    else {
        return false;
    }
    return Result;
}

//deep copy
/****************************************************************************/
/*!
 *  \brief Function to retrieve program for given program ID.Program passed is
 *         Updated with a Program having ProgramID in the list
 *
 *  \iparam ProgramID = Id of the program
 *  \iparam Program = Program reference
 *
 *  \return true - success , false - failure
 */
/****************************************************************************/
bool CDataProgramList::GetProgram(const QString ProgramID, CProgram& Program)
{
    QReadLocker locker(mp_ReadWriteLock);

    if (m_ProgramList.contains(ProgramID)) {
        Program = *(m_ProgramList.value(ProgramID));
        return true;
    } else {
        return false;
    }
}

/****************************************************************************/
/*!
 *  \brief Checks program exists in the list
 *  \iparam ProgramID = Program ID
 *
 *  \return true =exists , false - not exists
 */
/****************************************************************************/

bool CDataProgramList::ProgramExists(const QString ProgramID) {
    return m_ProgramList.contains(ProgramID);
}

/****************************************************************************/
/*!
 *  \brief Adds program step to program
 *  \iparam p_Program = Program to add
 *  \warning If a program is added with verification mode false,
 *           then expanded step list is not updated. One has to explicitly
 *           Call UpdateExpandedSteps()
 *  \return true = add success , false - add failed
 */
/****************************************************************************/
bool CDataProgramList::AddProgram(const CProgram* p_Program)
{
    if (p_Program == NULL) return false;

    QString ID = const_cast<CProgram*>(p_Program)->GetID();
    if (m_ProgramList.contains(ID)) {
        // there is already a program with the given ProgramID
        // => use UpdateProgram instead
        //    or call DeleteProgram before AddProgram
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_ID_NOT_UNIQUE,
                                                   Global::tTranslatableStringList() << p_Program->GetID()
                                                   << p_Program->GetLongName());
        m_ErrorHash.insert(EVENT_DM_PROG_ID_NOT_UNIQUE,
                           Global::tTranslatableStringList() << p_Program->GetID()
                           << p_Program->GetLongName());
        SetErrorList(&m_ErrorHash);
        return false;
    }

    bool Result = true;
    // check for the Long name existence in the Program list
    if (m_ProgramListNames.contains(p_Program->GetName().simplified(), Qt::CaseInsensitive)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGRAM_NAME_NOT_UNIQUE
                                                   , Global::tTranslatableStringList() << p_Program->GetName().simplified());
        m_ErrorHash.insert(EVENT_DM_PROGRAM_NAME_NOT_UNIQUE,
                           Global::tTranslatableStringList() << p_Program->GetName().simplified());
        qDebug() << "CDataProgramList::AddProgram - Program long name already exists" <<p_Program->GetName();
        Result = false;
    }

    // If Uniqueness test fails we exit the funtion and store the errors
    if (!Result) {
        SetErrorList(&m_ErrorHash);
        return Result;
    }

    CProgram *p_TempProgram = new CProgram();
    if (p_Program->GetNumberOfStepsInExpandedList() == 0) {
        const_cast<CProgram*>(p_Program)->RefreshExpandedStepList();
    }
    *p_TempProgram = *p_Program;

    Result = true;
    if ((m_DataVerificationMode)) {
        CDataProgramList* p_DPL_Verification = new CDataProgramList();

        // first lock current state for reading
        {   // code block defined for QReadLocker.
            QReadLocker locker(mp_ReadWriteLock);

            // create clone from current state
            *p_DPL_Verification = *this;

            // disable verification in clone
            p_DPL_Verification->SetDataVerificationMode(false);

            // execute required action (AddProgram) in clone
            Result = p_DPL_Verification->AddProgram(p_TempProgram);

            if (Result) {
                // now check new content => call all active verifiers
                Result = DoLocalVerification(p_DPL_Verification);
            }
        }
        if (Result) {
            // if content ok, clone backwards
            *this = *p_DPL_Verification;
        }
        else {
            //Store errors.
            // Since we are going to delete p_DPL_Verification
            // We need to store the errors generated by it.
            // For now Errors are not copied by assignment or copy constructors.
            ListOfErrors_t ErrorList = p_DPL_Verification->GetErrorList();
            if (!ErrorList.isEmpty()) {
                // Control reaches here means Error list is not empty.
                // Considering only the first element in list since
                // verfier can atmost add only one Hash has to the error list
                m_ErrorHash = *(ErrorList.first());
                SetErrorList(&m_ErrorHash);
            }
        }
        delete p_TempProgram;
        // delete test clone
        delete p_DPL_Verification;

    } else {
        QWriteLocker locker(mp_ReadWriteLock);
        m_ProgramList.insert(ID, p_TempProgram);
        m_OrderedListOfProgramIDs.append(ID);
        // White space at begining , end are removed from names. If continuous
        // White space exits between a name, they are replaced by a single white
        //space
        m_ProgramListNames.append(p_TempProgram->GetName().simplified());
        Result = true;
    }

    return Result;
}


/****************************************************************************/
/*!
 *  \brief Adds new Leica program to program list, after the existing Leica programs
 *  \iparam p_Program = Program to add
 *  \warning If a program is added with verification mode false,
 *           then expanded step list is not updated. One has to explicitly
 *           Call UpdateExpandedSteps()
 *  \return true = add success , false - add failed
 */
/****************************************************************************/
bool CDataProgramList::AddLeicaProgram(const CProgram* p_Program)
{
    CHECKPTR(p_Program);

    QString ID = p_Program->GetID();
    if (m_ProgramList.contains(ID)) {
        // there is already a program with the given ProgramID
        // => use UpdateProgram instead
        //    or call DeleteProgram before AddProgram
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_ID_NOT_UNIQUE,
                                                   Global::tTranslatableStringList() << p_Program->GetID()
                                                   << p_Program->GetLongName());
        m_ErrorHash.insert(EVENT_DM_PROG_ID_NOT_UNIQUE,
                           Global::tTranslatableStringList() << p_Program->GetID()
                           << p_Program->GetLongName());
        SetErrorList(&m_ErrorHash);

        return false;
    }
    bool Result = true;
    // check for the Long name existence in the Program list
    if (m_ProgramListNames.contains(p_Program->GetName().simplified(), Qt::CaseInsensitive)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGRAM_NAME_NOT_UNIQUE
                                                   , Global::tTranslatableStringList() << p_Program->GetName().simplified());
        m_ErrorHash.insert(EVENT_DM_PROGRAM_NAME_NOT_UNIQUE,
                           Global::tTranslatableStringList() << p_Program->GetName().simplified());
        qDebug() << "CDataProgramList::AddProgram - Program long name already exists" <<p_Program->GetName();
        Result = false;
    }


    if (!Result) {
        SetErrorList(&m_ErrorHash);
        return Result;
    }
    CProgram *p_TempProgram = new CProgram();
    if (p_Program->GetNumberOfStepsInExpandedList() == 0) {
        const_cast<CProgram*>(p_Program)->RefreshExpandedStepList();
    }
    *p_TempProgram = *p_Program;

    Result = true;
    if ((m_DataVerificationMode)) {
        CDataProgramList* p_DPL_Verification = new CDataProgramList();

        // first lock current state for reading
        {   // code block defined for QReadLocker.
            QReadLocker locker(mp_ReadWriteLock);

            // create clone from current state
            *p_DPL_Verification = *this;

            // disable verification in clone
            p_DPL_Verification->SetDataVerificationMode(false);

            // execute required action (AddProgram) in clone
            Result = p_DPL_Verification->AddLeicaProgram(p_TempProgram);

            if (Result) {
                // now check new content => call all active verifiers
                Result = DoLocalVerification(p_DPL_Verification);
            }
        }
        if (Result) {
            // if content ok, clone backwards
            *this = *p_DPL_Verification;
        }
        else {
            //Store errors.
            // Since we are going to delete p_DPL_Verification
            // We need to store the errors generated by it.
            // For now Errors are not copied by assignment or copy constructors.
            ListOfErrors_t ErrorList = p_DPL_Verification->GetErrorList();
            if (!ErrorList.isEmpty()) {
                // Control reaches here means Error list is not empty.
                // Considering only the first element in list since
                // verfier can atmost add only one Hash has to the error list
                m_ErrorHash = *(ErrorList.first());
                SetErrorList(&m_ErrorHash);
            }
        }
        delete p_TempProgram;
        // delete test clone
        delete p_DPL_Verification;

    } else {
        QWriteLocker locker(mp_ReadWriteLock);
        int ProgramIDCount = 0;
        // check for the last Leica program in the list
        for (ProgramIDCount = 0; ProgramIDCount < m_OrderedListOfProgramIDs.count(); ProgramIDCount++) {
            if (m_OrderedListOfProgramIDs.value(ProgramIDCount).at(0) != 'L') {
                // found the first User program
                break;
            }
        }
        // insert the item in the Hash table
        m_ProgramList.insert(ID, p_TempProgram);

        // If all the programs all Leica then counter value will be last, then it requires to append the list
        if (ProgramIDCount >= m_OrderedListOfProgramIDs.count()) {
            m_OrderedListOfProgramIDs.append(ID);
            // White space at begining , end are removed from names. If continuous
            // White space exits between a name, they are replaced by a single white
            //space
            m_ProgramListNames.append(p_TempProgram->GetName().simplified());
        }
        else {
            m_OrderedListOfProgramIDs.insert(ProgramIDCount, ID);
            // White space at begining , end are removed from names. If continuous
            // White space exits between a name, they are replaced by a single white
            //space
            m_ProgramListNames.insert(ProgramIDCount, p_TempProgram->GetName().simplified());
        }
        Result = true;
    }

    return Result;
}

/****************************************************************************/
/*!
 *  \brief  Updates program
 *  \iparam p_Program = program which needs to be updated
 *  \return true - update success, false - update failed
 */
/****************************************************************************/
bool CDataProgramList::UpdateProgram(const CProgram* p_Program)
{
    CHECKPTR(p_Program);

    QString ID = p_Program->GetID();
    if (!m_ProgramList.contains(ID)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGRAM_ID_NOT_FOUND,
                                                   Global::tTranslatableStringList() << p_Program->GetID()
                                                   << p_Program->GetLongName());
        m_ErrorHash.insert(EVENT_DM_PROGRAM_ID_NOT_FOUND,
                           Global::tTranslatableStringList() << p_Program->GetID()
                           << p_Program->GetLongName());
        SetErrorList(&m_ErrorHash);
        // there is no program with the given ProgramID
        return false;
    }

    // check for the unique long name and short name existence
    if (!CheckForUniqueName(ID, p_Program->GetName().simplified(), p_Program->GetName().simplified())) {
        //Store Errors.
        SetErrorList(&m_ErrorHash);
        return false;
    }

    bool Result = false;
    if (m_DataVerificationMode) {
        CDataProgramList* p_DPL_Verification = new CDataProgramList();
        CProgram const* p_Prog = GetProgram(ID);
        CHECKPTR(p_Prog);
        QString PreviousName = p_Prog->GetName().simplified();
//        QString PreviousLongName  = p_Prog->GetLongName().simplified();
        // first lock current state for reading
        {   // code block defined for QReadLocker.
            QReadLocker locker(mp_ReadWriteLock);

            // create clone from current state
            *p_DPL_Verification = *this;

            // disable verification in clone
            p_DPL_Verification->SetDataVerificationMode(false);

            // execute required action (UpdateProgram) in clone
            Result = p_DPL_Verification->UpdateProgram(p_Program);
            if (Result) {
                // now check new content => call all active verifiers
                Result = DoLocalVerification(p_DPL_Verification);
                if (!Result) {
                    //Store errors.
                    // Since we are going to delete p_DPL_Verification
                    // We need to store the errors generated by it.
                    // For now Errors are not copied by assignment or copy constructors.
                    ListOfErrors_t ErrorList = p_DPL_Verification->GetErrorList();
                    if (!ErrorList.isEmpty()) {
                        // Control reaches here means Error list is not empty.
                        // Considering only the first element in list since
                        // verfier can atmost add only one Hash has to the error list
                        m_ErrorHash = *(ErrorList.first());
                        SetErrorList(&m_ErrorHash);
                    }
                }
            }
        }
        if (Result) {
            // if content ok, clone backwards
            *this = *p_DPL_Verification;
        }

        //Update expanded step list
        p_Prog = m_ProgramList.value(ID, NULL);
        int Index;
        if (PreviousName != p_Prog->GetName() ) {
            //Update short name & long name list
            Index = m_ProgramListNames.indexOf(PreviousName);
            if (Index != -1) {
                m_ProgramListNames.replace(Index, p_Prog->GetName().simplified());
            }
            else {
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGRAM_INVALID_INDEX,
                                                           Global::tTranslatableStringList() << p_Program->GetName());
                m_ErrorHash.insert(EVENT_DM_PROGRAM_INVALID_INDEX,
                                   Global::tTranslatableStringList() << p_Program->GetName());
                Result = false;
            }
        }
        // delete the cloned objects
        delete p_DPL_Verification;

    } else {
        QWriteLocker locker(mp_ReadWriteLock);

        // do a deep copy
        *m_ProgramList.value(ID) = *p_Program;
        Result = true;
    }
    return Result;
}

/****************************************************************************/
/*!
 *  \brief  Deletes one progarm
 *  \iparam ProgramID = Program ID associated with the program to be deleted
 *  \return true - delete success , false - delete failure
 */
/****************************************************************************/
bool CDataProgramList::DeleteProgram(const QString ProgramID)
{
    if (m_ProgramList.contains(ProgramID)) {
        CProgram *p_Program = m_ProgramList.value(ProgramID);
        /* We store Program Name so that we can use it later on for Error reporting */
        QString ProgramName = p_Program->GetName();
        //get Program from ProgramList and free memory
        delete p_Program;

        //remove pointer to Program from ProgramList
        m_ProgramList.remove(ProgramID);

        //remove ProgramID from ID list
        int MatchIndex = -1;
        for (int i=0; i<m_OrderedListOfProgramIDs.count(); i++) {
            if (m_OrderedListOfProgramIDs[i] == ProgramID) {
                MatchIndex = i;
                break;
            }
        }
        // MatchIndex MUST never be -1 !!!
        if (MatchIndex != -1) {
            m_OrderedListOfProgramIDs.removeAt(MatchIndex);
            m_ProgramListLongNames.removeAt(MatchIndex);
            m_ProgramListShortNames.removeAt(MatchIndex);
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGRAM_INVALID_INDEX,
                                                       Global::tTranslatableStringList() << ProgramName);
            m_ErrorHash.insert(EVENT_DM_PROGRAM_INVALID_INDEX,
                               Global::tTranslatableStringList() << ProgramName);
            return false;
        }

        if (!UpdateReagentIDList()) {
//            Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_ID_LIST,
//                                                       Global::tTranslatableStringList() << ProgramName
//                                                       );
//            m_ErrorHash.insert(EVENT_DM_REAGENT_ID_LIST_UPDATE_FAILED,
//                               Global::tTranslatableStringList() << ProgramName
//                               );
            return false;
        }
        return true;
    } else {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_DELETE_FAILED_ID_NOT_FOUND,
                                                   Global::tTranslatableStringList() << ProgramID);
        m_ErrorHash.insert(EVENT_DM_PROG_DELETE_FAILED_ID_NOT_FOUND,
                           Global::tTranslatableStringList() << ProgramID);
        // there is no program with the given ProgramID
        return false;
    }
}

/****************************************************************************/
/*!
 *  \brief  Deletes the progarm
 *  \iparam Index
 *  \return true - delete success , false - delete failure
 */
/****************************************************************************/
bool CDataProgramList::DeleteProgram(const unsigned int Index)
{
    if (Index < (unsigned int)m_OrderedListOfProgramIDs.count()) {
        QString ProgramID = m_OrderedListOfProgramIDs.value(Index);
        return DeleteProgram(ProgramID);
    } else {
        return false;
    }
}

/****************************************************************************/
/*!
 *  \brief  Deletes all the programs in the list
 *  \return true - delete success , false - delete failure
 */
/****************************************************************************/
bool CDataProgramList::DeleteAllPrograms()
{
    bool Result = true;

    while (m_ProgramList.size() > 0) {
        if (DeleteProgram(0) == false) {
            Result = false;
        }
    }

    m_ProgramList.clear();
    m_OrderedListOfProgramIDs.clear();
    m_ProgramListLongNames.clear();
    m_ProgramListShortNames.clear();
    m_ReagentIDList.clear();
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Reads all the programs from the xml stream
 *
 *  \iparam XmlStreamReader = Reference to xml stream to read from
 *  \iparam CompleteData = true to read Complete data of object
 *
 *  \return true - read success or false - read failure
 */
/****************************************************************************/
bool CDataProgramList::ReadAllPrograms(QXmlStreamReader& XmlStreamReader, bool CompleteData)
{
    bool Result = true;
    // Look for node <Program>
    while(!XmlStreamReader.atEnd()) {
        if (static_cast<int>(XmlStreamReader.readNext()) == 1) {
            qDebug() << "Reading " << XmlStreamReader.name() << " at line number: " << XmlStreamReader.lineNumber();
            qDebug() << "Invalid Token. Error: " << XmlStreamReader.errorString();
        }

        if (XmlStreamReader.isStartElement()) {
            if (XmlStreamReader.name() == "Program") {

                DataManager::CProgram Program;

                if (!Program.DeserializeContent(XmlStreamReader, CompleteData)) {
                    qDebug() << "CProgram Deserialize failed!";
                    return false;
                }
                // Now add this Reagent
                if (!AddProgram(&Program)) {
                    qDebug() << "CProgramList::Add Program failed!";
                    return false;
                }
            }
            else {
                qDebug() << "### unknown node name <" << XmlStreamReader.name().toString() << "> at line number: " << XmlStreamReader.lineNumber();
                return false;
            }
        }
        else if (XmlStreamReader.isEndElement() && XmlStreamReader.name().toString() == "ProgramList") {
            //qDebug() << "It has reached the end of the particular Node , Stop reading for this Reagent";
            break; // exit from while loop
        }
    }
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Reads the CDataProgramList Data from QIODevice
 *
 *  \iparam IODevice = Instance of the IODevice - Buffer or File
 *  \iparam CompleteData = bool type if true writes Complete data of object
 *
 *  \return True or False
 */
/****************************************************************************/
// the content of p_Device (xml stream expected) will be
// mirrored into this class/object
bool CDataProgramList::DeserializeContent(QIODevice& IODevice, bool CompleteData)
{
//    QWriteLocker Locker(mp_ReadWriteLock);
    bool Result = true;
    QXmlStreamReader XmlStreamReader;

    XmlStreamReader.setDevice(&IODevice);

    // look for node <StainingPrograms>
    if (!Helper::ReadNode(XmlStreamReader, "StainingPrograms")) {
        qDebug() << "DeserializeContent: abort reading. Node not found: StainingPrograms";
        return false;
    }

    // Read attribute Version
    if (!XmlStreamReader.attributes().hasAttribute("Version")) {
        qDebug() << "### attribute <Version> is missing => abort reading";
        return false;
    }
    SetVersion(XmlStreamReader.attributes().value("Version").toString().toInt());

    //  Now Delete the Program list and ordered list of Program ID's
    if (!DeleteAllPrograms()) {
        //throw error ...
        THROWARG(Global::EVENT_GLOBAL_ERROR_NULL_POINTER, Global::tTranslatableStringList() << FILE_LINE);
    }

    // Look for node <ProgramList>
    if (!Helper::ReadNode(XmlStreamReader, "ProgramList")) {
        qDebug() << "DeserializeContent: abort reading. Node not found: ProgramList";
        return false;
    }

    // NextProgramID
    if (!XmlStreamReader.attributes().hasAttribute("NextProgramID")) {
        qDebug() << "### attribute <NextProgramID> is missing => abort reading";
        return false;
    }

    SetNextFreeProgID(XmlStreamReader.attributes().value("NextProgramID").toString());

    Result = ReadAllPrograms(XmlStreamReader, CompleteData);
    UpdateReagentIDList();
    //if completedata
//    if (CompleteData && Result) {
//        if (!Helper::ReadNode(XmlStreamReader, "NonXmlData")) {
//            return false;
//        }

//        if (!XmlStreamReader.attributes().hasAttribute("VerificationMode")) {
//            qDebug() << "### attribute <VerificationMode> is missing";
//            return false;
//        }

//        if (XmlStreamReader.attributes().value("VerificationMode").toString() == "true") {
//            SetDataVerificationMode(true);
//        }
//        else {
//            SetDataVerificationMode(false);
//        }

//        if (!XmlStreamReader.attributes().hasAttribute("FileName")) {
//            qDebug() << "### attribute <FileName> is missing";
//            return false;
//        }
//        SetFilename(XmlStreamReader.attributes().value("FileName").toString());

//	}
    (void)XmlStreamReader.device()->reset();
//    qDebug()<<"\n\n\n PROGRAM LIST"<<XmlStreamReader.device()->readAll();
    return Result;
}

// the complete content of this class/object will be
// written into p_ByteArray as XML stream
// used e.g. for transfer to GUI
/****************************************************************************/
/*!
 *  \brief Writes the CDataReagentList Data to QIODevice
 *
 *  \iparam IODevice = Instance of the IODevice might be Buffer or File
 *  \iparam CompleteData = bool type if true writes Complete data of object
 *
 *  \return True or False
 */
/****************************************************************************/
bool CDataProgramList::SerializeContent(QIODevice& IODevice, bool CompleteData)
{
    //QReadLocker Locker(mp_ReadWriteLock);
    QString StringValue;
    QXmlStreamWriter XmlStreamWriter;

    XmlStreamWriter.setDevice(&IODevice);
    XmlStreamWriter.setAutoFormatting(true);
    //XmlStreamWriter.setAutoFormattingIndent(4);
    XmlStreamWriter.writeStartDocument();

    XmlStreamWriter.writeDTD("<!DOCTYPE StainingPrograms>");
    XmlStreamWriter.writeStartElement("StainingPrograms");

    // write attribute version
    (void)StringValue.setNum(GetVersion()); //to suppress lint-534
    XmlStreamWriter.writeAttribute("Version", StringValue);

    //=========================================================================================
    // write section ProgramList

    XmlStreamWriter.writeStartElement("ProgramList");

    // write attribute NextProgramID
    XmlStreamWriter.writeAttribute("NextProgramID", GetNextFreeProgID(false));

    // write all programs
    for (int i = 0; i < GetNumberOfPrograms(); i++) {
        CProgram *p_Prog = GetProgram(i);
        if (p_Prog != NULL) {
            if (!p_Prog->SerializeContent(XmlStreamWriter, CompleteData)) {
                qDebug("DataManager::CProgram SerializeContent failed ");
                return false;
            }
        }
    }
    XmlStreamWriter.writeEndElement(); // ProgramList

     //======NODE=======Temporary Data Variables=========================
//    if (CompleteData) {
//        XmlStreamWriter.writeStartElement("NonXmlData");
//        if (GetDataVerificationMode()) {
//            XmlStreamWriter.writeAttribute("VerificationMode", "true");
//        }
//        else {
//            XmlStreamWriter.writeAttribute("VerificationMode", "false");
//        }

//        XmlStreamWriter.writeAttribute("FileName", GetFilename());

////        XmlStreamWriter.writeAttribute("VerifierCount", QString::number(m_VerifierList.count()));
////        for (qint32 I = 0; I < m_VerifierList.count(); I++) {
////            QString VerifierPtr;
////            QTextStream VerifierPointer(&VerifierPtr);
////            VerifierPointer << m_VerifierList.at(I);
////            qDebug()<<"\n\n Real Verifier Pointer"<<m_VerifierList.at(I);
////            XmlStreamWriter.writeAttribute(QString("Verifier%1Pointer").arg(I + 1), VerifierPointer.readAll());
////        }
//        XmlStreamWriter.writeEndElement();
//     }
     //======NODE=END====Temporary Data Variables=========================
    // The above Result has to be true to write the end element
    XmlStreamWriter.writeEndElement(); // for Staining Programss
    XmlStreamWriter.writeEndDocument(); // End of Document

    return true;
}

/****************************************************************************/
/*!
 *  \brief Output Stream Operator which streams data
 *
 *  \iparam OutDataStream = Instance of the QDataStream
 *  \iparam ProgramList = CDataProgramList class object
 *
 *  \return Output Stream
 */
/****************************************************************************/
QDataStream& operator << (QDataStream& OutDataStream, const CDataProgramList& ProgramList) {

    CDataProgramList* p_TempProgramList = const_cast<CDataProgramList*>(&ProgramList);

    if (!p_TempProgramList->SerializeContent(*OutDataStream.device(), true)) {
        qDebug() << "CProgramList::Operator Streaming (SerializeContent) failed.";
    }
    return OutDataStream;
}

/****************************************************************************/
/*!
 *  \brief Input stream Operator which reads the data from Input parameter.
 *
 *  \iparam InDataStream = Instance of the DataStream
 *  \iparam ProgramList = CDataProgramList class object
 *
 *  \return Input Stream
 */
/****************************************************************************/

QDataStream& operator >> (QDataStream& InDataStream, CDataProgramList& ProgramList) {

    if (!ProgramList.DeserializeContent(*InDataStream.device(), true)) {
        qDebug() << "CProgramList::Operator Streaming (DeSerializeContent) failed.";

    }
    return InDataStream;
}

/****************************************************************************/
/*!
 *  \brief Assignment Operator which copies from rhs to lhs.
 *
 *  \iparam SourceProgramList = CDataProgramList class object
 *
 *  \return CDataProgramList Class Object
 */
/****************************************************************************/
CDataProgramList& CDataProgramList::operator = (const CDataProgramList& SourceProgramList) {

    //Read should be blocked since internally Serialization takes place
    //QReadLocker Locker(mp_ReadWriteLock);
    // make sure not same object
    if (this != &SourceProgramList)
    {
        QByteArray TempByteArray;
        QDataStream DataStream(&TempByteArray, QIODevice::ReadWrite);
        DataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
        TempByteArray.clear();
        //Serialize
        DataStream << SourceProgramList;
        (void)DataStream.device()->reset();
        //Deserialize
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
 *  \brief Verifies the Data with respect to Verifiers
 *
 *  \iparam ID = Name of the Program ID
 *  \iparam ShortName = Short name for the Program
 *  \iparam LongName = Long name for the Program
 *
 *  \return On Successful (True) or not (False)
 */
/****************************************************************************/
bool CDataProgramList::CheckForUniqueName(QString ID, QString ShortName, QString LongName)
{
    bool Result = true;

    const CProgram *p_Program = GetProgram(ID);
    if (p_Program) {
        if (p_Program->GetName() != ShortName) {
            // check for the Short name existence in the Program list
            if (m_ProgramListShortNames.contains(ShortName, Qt::CaseInsensitive)) {
                qDebug() << "CDataProgramList::CheckForUniqueName() - Program short name already exists :";
                m_ErrorHash.insert(EVENT_DM_SHORT_NAME_NOT_UNIQUE, Global::tTranslatableStringList() << ShortName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_SHORT_NAME_NOT_UNIQUE,
                                                           Global::tTranslatableStringList() << ShortName);
                Result = false;
            }
        }
//        if (p_Program->GetLongName() != LongName) {
//            // check for the Long name existence in the Program list
//            if (m_ProgramListLongNames.contains(LongName, Qt::CaseInsensitive)) {
//                qDebug() << "CDataProgramList::CheckForUniqueName() - Program long name already exists :";
//                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGRAM_NAME_NOT_UNIQUE
//                                                           , Global::tTranslatableStringList() << LongName);
//                m_ErrorHash.insert(EVENT_DM_PROGRAM_NAME_NOT_UNIQUE, Global::tTranslatableStringList() << LongName);
//                Result = false;
//            }
//        }
    }
    else {
        Result = false;
    }
    return Result;
}

/****************************************************************************/
/*!
 *  \brief Updates The reagent ID's used by the programs in list.
 *         Usually called when Programs are deleted from list.
 *  \return True -Success  or False - Update failed.
 */
/****************************************************************************/
bool CDataProgramList::UpdateReagentIDList()
{

    m_ReagentIDList.clear();
    for (qint32 I = 0; I < m_ProgramList.count(); I++) {
        CProgram const *p_Program = GetProgram(I);
        if (p_Program) {
            m_ReagentIDList.append(p_Program->GetReagentIDList());
        }
        else {
            qDebug()<<"### CDataProgramList::UpdateReagentIDList() failed";
            return false;
        }
    }
   m_ReagentIDList.removeDuplicates();
   return true;
}
} // namespace DataManager
