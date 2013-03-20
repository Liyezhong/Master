/****************************************************************************/
/*! \file ProgramSequenceList.h
 *
 *  \brief ProgramSequence defination.
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
#ifndef PROGRAMSEQUENCESTEP_H
#define PROGRAMSEQUENCESTEP_H


#include <QHash>
#include <QList>
#include <QMutableHashIterator>
#include <QReadWriteLock>
#include <QBuffer>
#include <QDebug>

#include "DataManager/Helper/Include/Types.h"
#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequence.h"

//lint -sem(DataManager::CProgramSequenceList::AddProgramSequenceStep, custodial(1))
//lint -sem(DataManager::CProgramSequenceList::UpdateProgramSequenceStep, custodial(2))

namespace DataManager {
//!< List of program sequence steps
typedef QHash<QString, CProgramSequence*> ListOfProgramsSequenceSteps_t; //!< List of program sequences
//!< Verifier list

/****************************************************************************/
/*!
 *  \brief  This class contains list of program sequence
 */
/****************************************************************************/
class CProgramSequenceList:public CDataContainerBase
{
    friend class CSpecialVerifierGroupA;
public:
    CProgramSequenceList();
    ~CProgramSequenceList();

    void Init();

    /****************************************************************************/
    /*!
     *  \brief Get the data container type
     *
     *  \return Type of the data container
     */
    /****************************************************************************/
    DataContainerType_t GetDataContainerType() {return PROGRAMSEQUENCE;}

    bool Read(QString Filename);

    CProgramSequenceList(const CProgramSequenceList&);
    friend QDataStream& operator << (QDataStream& OutDataStream, const CProgramSequenceList& ProgramSequenceList);
    friend QDataStream& operator >> (QDataStream& InDataStream, CProgramSequenceList& ProgramSequenceList);
    CProgramSequenceList& operator = (const CProgramSequenceList&);

    /****************************************************************************/
    /*!
     *  \brief Retrieve verification status
     *  \return true - verfication on. false - verification off
     */
    /****************************************************************************/
    bool GetDataVerificationMode() {return m_DataVerificationMode;}

    /****************************************************************************/
    /*!
     *  \brief Sets the data verification status
     *
     *  \iparam Value = true -set ,false - not set
     */
    /****************************************************************************/
    void SetDataVerificationMode(const bool Value){m_DataVerificationMode = Value;}

    /****************************************************************************/
    /*!
     *  \brief Retrieve the version of the xml file
     *  \return version
     */
    /****************************************************************************/
    int  GetVersion() {return m_Version;}

    /****************************************************************************/
    /*!
     *  \brief Retrieve filename of the XML
     *  \return file name
     */
    /****************************************************************************/
    QString GetFilename () {return m_Filename;}

    //SECTION PROGRAMS//
    /****************************************************************************/
    /*!
     *  \brief Retrieve the number of programs in the programsequence list
     *  \return Program count
     */
    /****************************************************************************/
    int  GetNumberOfPrograms() {return m_ProgramSequenceStepsList.count();}

    // the CreateStep functions won't add the created step to the internal step list!
    /****************************************************************************/
    /*!
     *  \brief Create a program sequence step
     *  \return Program sequence step
     */
    /****************************************************************************/
    CProgramSequence* CreateProgramSequenceStep() {return new CProgramSequence();}
    /****************************************************************************/
    /*!
     *  \brief Create program sequence step for the given program id  
     *
     *	\iparam ProgramID = Program ID
     *
     *  \return program sequence step
     */
    /****************************************************************************/
    CProgramSequence* CreateProgramSequenceStep(const QString ProgramID) {return (new CProgramSequence(ProgramID));} // given ID will be used

    /****************************************************************************/
    /*!
     *  \brief Create program sequence step for the given program id
     *
     *	\iparam ProgramID = Program ID
     *
     *  \return program sequence
     */
    /****************************************************************************/
    CProgramSequence* GetProgramSequenceStep(const QString ProgramID) {return m_ProgramSequenceStepsList.value(ProgramID, NULL);}  // uses unique step ID

    bool GetProgramSequenceStep(const unsigned int Index, CProgramSequence& ProgramSequence);
    bool GetProgramSequenceStep(const QString ProgramID, CProgramSequence& ProgramSequence);

    const CProgramSequence* GetProgramSequenceStep(const unsigned int Index); // uses order index

    bool AddProgramSequenceStep(const CProgramSequence* p_ProgramSequenceStep);   // p_ProgramStep is added directly => don't delete outside!
    bool AddLeicaProgramSequenceStep(const CProgramSequence* p_ProgramSequenceStep);   // p_ProgramStep is added directly => don't delete outside! For Leica program
    bool UpdateProgramSequenceStep(const CProgramSequence* p_ProgramSequenceStep);  // content of p_ProgramStep will be copied  => delete outside!

    bool DeleteProgramSequenceStep(const QString StepID);   // uses unique step ID
    bool DeleteProgramSequenceStep(const unsigned int Index);  // uses order index

    bool MoveProgramStep(qint32 FromIndex, qint32 ToIndex); // moves step inside a program step list
    QStringList GetStartableProgramIDList();
    QStringList GetProgramIDList();
    QStringList GetUsedProgramIDList();

private:
    int m_Version;                  //!< version of the xml
    bool m_DataVerificationMode;    //!< Verification status
    QString m_Filename;             //!< XML File name


    ListOfProgramsSequenceSteps_t m_ProgramSequenceStepsList;   //!< A list containg the program sequence steps
    ListOfIDs_t m_OrderedListOfProgramSequenceIDs;  //!< A List in which program sequence ids are stored in order

    QReadWriteLock* mp_ReadWriteLock;//!< File read write protection

    ErrorHash_t m_ErrorHash;    //!< Event List for GUI and for logging purpose. This member is not copied when using copy constructor/Assignment operator

    /****************************************************************************/
    /*!
     *  \brief Sets the version of the xml file
     *
     *  \iparam Value = version of the file
     */
    /****************************************************************************/
    void SetVersion(const int Value){m_Version = Value;}

    /****************************************************************************/
    /*!
     *  \brief Sets the file name of the xml
     *
     *  \iparam Value = file name
     */
    /****************************************************************************/
    void SetFilename (const QString Value) {m_Filename = Value;}

    bool SerializeContent(QIODevice& IODevice, bool CompleteData); // Writes from the CProgramSequence object to a IODevice.
    bool DeserializeContent(QIODevice& IODevice, bool CompleteData); // Reads from the IODevice to CProgramSequence object.
    bool ReadCompleteData(QXmlStreamReader& XmlStreamReader);

    void SetDefaultAttributes();
    bool DeleteAllProgramSequenceSteps();

};

}//End of namespace DataManager
#endif // PROGRAMSEQUENCESTEP_H
