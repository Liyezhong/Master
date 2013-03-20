/****************************************************************************/
/*! \file ProgramStep.h
 *
 *  \brief Definition file for class CProgramStep.
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

#ifndef DATAMANAGER_PROGRAMSTEP_H
#define DATAMANAGER_PROGRAMSTEP_H

#include <QString>
#include <QStringList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "DataManager/Helper/Include/Types.h"

namespace DataManager {
//!< Forward declartion
class CProgram;
/****************************************************************************/
/*!
 *  \brief  This class implements program step
 */
/****************************************************************************/
class CProgramStep
{
    friend class CProgram;

private:
    QString m_StepID;               //!< Step Id
    QString m_ReagentID;            //!< Reagent ID
    int m_MinDuration;              //!< Minimum duration of the program step [s]
    int m_MaxDuration;              //!< Maximum duration of program step [%] related to m_MinDuration
    QString m_MaxDurationString;    //!< Maximum duration string; include "%"
    bool m_Exclusive;               //!< true- step exclusive to program
    int m_NumberOfParallelStations; //!< Number of parallel stations
    int m_Intensity;                //!< Intensity
    int m_NumberOfSteps;            //!< Number of Steps field only for Leica Programs
    int m_RangeLow;                 //!< Range Low only for Leica Programs
    int m_RangeHigh;                //!< Range High only for Leica Programs
    QStringList m_StationIDList;    //!< Stations IDs list for scheduler

    bool DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData);
    bool SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData);
    bool ReadCompleteData(QXmlStreamReader& XmlStreamReader);
    void SetRangeLow(const QString Value);
    void SetRangeHigh(const QString Value);
    int CalculateMinDuration();

    /****************************************************************************/
    /*!
     *  \brief Sets Number OF steps for a Leica program
     *
     *  \iparam Value = Number Of Steps value
     */
    /****************************************************************************/
    void SetNumberOfSteps(const int Value){m_NumberOfSteps = Value;}

public:
    CProgramStep();
    CProgramStep(const QString StepID);
    CProgramStep(const QString StepID, const QString ReagentID, const int MinDuration, const int MaxDuration,
                 const bool Exclusive, const int ParallelStations, const int Intensity);
    CProgramStep(const CProgramStep&);
    friend QDataStream& operator << (QDataStream& OutDataStream, const CProgramStep& ProgramStep);
    friend QDataStream& operator >> (QDataStream& InDataStream, CProgramStep& ProgramStep);
    CProgramStep& operator = (const CProgramStep&);

    int GetRangeLow() const;
    int GetRangeHigh() const;
    QString GetRangeLowInString() const;
    QString GetRangeHighInString() const;

    /****************************************************************************/
    /*!
     *  \brief Retrieves Step ID of the program step
     *
     *  \return Step ID
     */
    /****************************************************************************/
    QString  GetStepID() const {return m_StepID;}
    /****************************************************************************/
    /*!
     *  \brief Sets Step ID
     *
     *  \iparam Value = Step ID
     */
    /****************************************************************************/
    void SetStepID(const QString Value){m_StepID = Value;}

    /****************************************************************************/
    /*!
     *  \brief Retrieves reagent id of the reagent used for the program step
     *
     *  \return Reagent ID
     */
    /****************************************************************************/
    QString  GetReagentID() const {return m_ReagentID;}
    /****************************************************************************/
    /*!
     *  \brief Sets Reagent ID
     *
     *  \iparam Value = Reagent ID
     */
    /****************************************************************************/
    void SetReagentID(const QString Value){m_ReagentID = Value;}


    QString GetMinDuration() ; // {return ConvertSecondsToTimeString(m_MinDuration);}
    /****************************************************************************/
    /*!
     *  \brief Retrieves list of program steps in a program
     *
     *  \return Step list
     */
    /****************************************************************************/
    int GetMinDurationInSeconds() const {return m_MinDuration;}

    void SetMinDuration(const QString Value);  // for time strings like "4d 1h 30m 15s"
    /****************************************************************************/
    /*!
     *  \brief Sets Minimum duration of the program step
     *
     *  \iparam Value = minimum duration value
     */
    /****************************************************************************/
    void SetMinDurationInSeconds(const int Value){m_MinDuration = Value;}

    //bool IsMaxDurationInfinite() {return (m_MaxDuration < 0);}  // value for unlimited max duration not needed (2011/06/07)

    int  GetMaxDurationInSeconds() const ;
    /****************************************************************************/
    /*!
     *  \brief Retrieves value of maximum duration of program step
     *
     *  \return maximum duration
     */
    /****************************************************************************/
    int  GetMaxDurationInPercent() const {return m_MaxDuration;}

    /****************************************************************************/
    /*!
     *  \brief Retrieves the maximum duration string
     *
     *  \return maximum duration
     */
    /****************************************************************************/
    QString GetMaxDurationString() const {return m_MaxDurationString;}

    /****************************************************************************/
    /*!
     *  \brief Sets Maximum duration (in %)of the program step
     *
     *  \iparam Value = maximum duration value
     */
    /****************************************************************************/
    void SetMaxDurationInPercent(const int Value){m_MaxDuration = Value;}
    void SetMaxDurationInPercent(const QString Value);   // expects % at the end of the string

    /****************************************************************************/
    /*!
     *  \brief checks if program step is exclusive
     *
     *  \return true -exclusive ,else false
     */
    /****************************************************************************/
    bool GetExclusive() const {return m_Exclusive;}

    /****************************************************************************/
    /*!
     *  \brief Sets the exclusive status of the program step
     *
     *  \iparam Value = true for exclusive
     */
    /****************************************************************************/
    void SetExclusive(const bool Value){m_Exclusive = Value;}

    /****************************************************************************/
    /*!
     *  \brief Retrieves the parallel stations count
     *
     *  \return Parallel stations count
     */
    /****************************************************************************/
    int GetNumberOfParallelStations() const {return m_NumberOfParallelStations;}
    /****************************************************************************/
    /*!
     *  \brief Sets the number of parallel stations
     *
     *  \iparam Value = no. of parallel stations
     */
    /****************************************************************************/
    void SetNumberOfParallelStations(const int Value){m_NumberOfParallelStations = Value;}

    /****************************************************************************/
    /*!
     *  \brief Retrieves Intensity value
     *
     *  \return Intensity
     */
    /****************************************************************************/
    int GetIntensity() const {return m_Intensity;}
    /****************************************************************************/
    /*!
     *  \brief Sets Intensity of program step for a Leica program
     *
     *  \iparam Value = Itensity value
     */
    /****************************************************************************/
    void SetIntensity(const int Value){m_Intensity = Value;}

    /****************************************************************************/
    /*!
        *  \brief Retrieves Number of Steps of the reagent used for the Leica Program
        *
        *  \return Number of Steps
        */
    /****************************************************************************/
    int GetNumberOfSteps() const {return m_NumberOfSteps;}

    /****************************************************************************/
    /*!
     *  \brief Retrieves the station id ist
     *
     *  \return Station Id list
     */
    /****************************************************************************/
    QStringList GetStationIDList() const { return m_StationIDList; }
    /****************************************************************************/
    /*!
     *  \brief Sets the list of station ID's
     *
     *  \iparam StationIDList = List of Station IDs
     */
    /****************************************************************************/
    void SetStationIDList(const QStringList StationIDList) {
        m_StationIDList = StationIDList;
    }
};

} // namespace DataManager

#endif // DATAMANAGER_PROGRAMSTEP_H
