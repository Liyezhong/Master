/****************************************************************************/
/*! \file Program.h
 *
 *  \brief Definition file for class CProgram.
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

#ifndef DATAMANAGER_PROGRAM_H
#define DATAMANAGER_PROGRAM_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QDebug>
#include <DataManager/Helper/Include/Types.h>
#include "HimalayaDataContainer/Containers/Programs/Include/ProgramBase.h"


namespace DataManager {

//!< Forward declration
class CDataProgramList;

//!< Hash containing SID step pointer
typedef QHash<QString, CProgramStep *> ProgramSIDHash_t;
//!<  List containing macro steps and normal program steps
typedef QList<CProgramStep*> ListOfExpandedSteps_t;


//!< Self explaining
static const QString SLIDE_ID_SPECIAL_REAGENT = "S3";
//!< Self explaining
static const int MAX_DURATION_PERCENTAGE_SID_STATION_STEP = 5900;
//!< Self explaining
static const QString MAX_DURATION_PERCENTAGE_DRY_STATION_STEP = "11900%";

/****************************************************************************/
/*!
 *  \brief  This class implements  programs
 */
/****************************************************************************/
class CProgram : public CProgramBase
{
    friend class CDataProgramList;
private:
    QString m_Color;                            //!< program color
    bool m_Locked;                              //!< True - Leica program, else user defined program
    ListOfExpandedSteps_t m_ExpandedStepList;   //!< Normal steps + Macro steps
    bool m_ExpandedListUpToDate;                //!< Flag: true- list upto date
    ProgramSIDHash_t    m_ProgramSIDHash;       //!< This hash holds the previously created SID step.
    QStringList m_ReagentIDList;                //!< Holds the reagent ID's present in Program.
    QStringList m_StationList;                  //!< List of Stations used by the program
    CProgramStep* mp_DryStationStep;            //!< Dry Station step created when two steps are in different zones of BathLayout
    CProgramStep* mp_SIDStationStep;            //!< SID step . Both Dry step and SID step are present only in Expanded Steplist
    bool m_Modified;
    ListOfIDs_t m_OrderedListOfExpandedStepIDs; //!< Ordered list of step id's
    bool SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData);
    bool DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData);
    bool RefreshExpandedStepList();
    void Init();
    void UpdateExpandedSteps(const CProgram &Other);
public:
    CProgram();
    CProgram(const QString ID);
    CProgram(const QString Color, const bool IsLocked, const QString ID, const QString ShortName, const QString LongName);
    CProgram(const CProgram&);
    ~CProgram();

    friend QDataStream& operator << (QDataStream& OutDataStream, const CProgram& Program);
    friend QDataStream& operator >> (QDataStream& InDataStream, CProgram& Program);
    CProgram& operator = (const CProgram&);

    /****************************************************************************/
    /*!
     *  \brief Retrieves the color associated with the program
     *
     *  \return Color
     */
    /****************************************************************************/
    QString GetColor()const {return m_Color;}


    /****************************************************************************/
    /*!
     *  \brief Sets the color associated with the program
     *
     *  \iparam Color
     */
    /****************************************************************************/
    void SetColor(const QString Color){m_Color = Color;}

    /****************************************************************************/
    /*!
     *  \brief Checks if program is a Leica program
     *
     *  \return True - Locked, else no
     */
    /****************************************************************************/
    bool IsLocked() const {return m_Locked;}

    /****************************************************************************/
    /*!
     *  \brief  Locks a program
     */
    /****************************************************************************/
    void LockProgram() {m_Locked = true;}   // unlock not possible

    /****************************************************************************/
    /*!
     *  \brief Get the expanded list for the program list
     *
     *  \iparam ExpandedStepList = List of the programs
     *
     *  \return Successful (true) or not (false)
     */
    /****************************************************************************/
    bool GetExpandedStepList(ListOfExpandedSteps_t& ExpandedStepList) const
    {
        ExpandedStepList = m_ExpandedStepList;
        return true;
    }

    /****************************************************************************/
    /*!
     *  \brief Get the expanded list for the program list
     **
     *  \return Expanded step list
     */
    /****************************************************************************/
    const ListOfExpandedSteps_t* GetExpandedStepList() const { return &m_ExpandedStepList;}

    /****************************************************************************/
    /*!
     *  \brief Get the expanded step list for the program
     *
     *  \iparam Index = Index for the expanded program step
     *  \iparam p_ProgStep = Pointer to a program step
     *
     *  \return Successful (true) or not (false)
     */
    /****************************************************************************/
    bool GetProgramStepExpanded(const unsigned int Index, CProgramStep* p_ProgStep) const
    {
        if (IsExpandedListUpToDate()) {
            if (m_ExpandedStepList.value(Index, NULL) == NULL) {
                qDebug() << "#####GetProgramStepExpanded: NULL-pointer or invalid index: " << Index;
                return false;
            } else {
                qDebug() << "GetProgramStepExpanded: return program step with index: " << Index;
                *p_ProgStep = *m_ExpandedStepList.at(Index);
                return true;
            }
        }
        else {
            qDebug()<<"### CProgram::GetExpandedProgramStep() failed. List not updated";
            return false;
        }
    }

    CProgramStep* GetProgramStepExpanded(const unsigned int Index) const
    {
        if (Index < (unsigned int)m_ExpandedStepList.count()) {
            return m_ExpandedStepList.value(Index, NULL);
        }
        else {
            qDebug()<<"### CProgram::GetExpandedProgramStep() failed. Invalid Index";
            return NULL;
        }
    }

    /****************************************************************************/
    /*!
     *  \brief Retrieves the expanded steplist count
     *
     *  \return Expanded steplist count
     *
     */
    /****************************************************************************/
    int GetNumberOfStepsInExpandedList() const {return m_ExpandedStepList.count();}

    /****************************************************************************/
    /*!
     *  \brief Sets the status of expanded step list
     *
     *  \iparam IsUpToDate = true - list up to date , false - list not updated
     */
    /****************************************************************************/
    void SetExpandedListState(const bool IsUpToDate){m_ExpandedListUpToDate = IsUpToDate;}
    /****************************************************************************/
    /*!
     *  \brief Checks if list is upto date
     *
     *  \return true -list upto date , false - list not upto date
     */
    /****************************************************************************/
    bool IsExpandedListUpToDate() const {return m_ExpandedListUpToDate;}

    /****************************************************************************/
    /*!
     *  \brief Retrieve Reagent ID list
     *
     *  \return Reagent ID list
     */
    /****************************************************************************/
    QStringList GetReagentIDList() const {return m_ReagentIDList;}

    QStringList const & GetStationsUsedByProgram();
    CProgramStep* CreateDryStationStep(QString StepID, QStringList StationIDList);
    CProgramStep* CreateSIDStationStep(QString StepID);
    bool DeleteProgramStep(const unsigned int Index);
    bool AddProgramStep(CProgramStep* p_ProgramStep);
    bool UpdateProgramStep(CProgramStep *p_ProgramStep);
    bool MoveProgramStep(qint32 FromIndex, qint32 ToIndex);

    void SetProgramModified(bool Modified) { m_Modified = Modified; }
    bool GetModifiedStatus() const { return m_Modified;}
};


} // namespace DataManager

#endif // DATAMANAGER_PROGRAM_H
