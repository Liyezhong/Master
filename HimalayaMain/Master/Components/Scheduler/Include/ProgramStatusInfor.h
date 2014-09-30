/****************************************************************************/
/*! \file ProgramStatusInfor.h
 *
 *  \brief CProgramStatusInfor class definition.
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

#ifndef PROGRAMSTATUSINFOR_H
#define PROGRAMSTATUSINFOR_H

#include <QFile>
#include <QMap>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

/****************************************************************************/
/*!
 * \brief the class for manage program status.
 */
/****************************************************************************/
class CProgramStatusInfor
{
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of CProgramStatusInfor
     *
     */
    /****************************************************************************/
    CProgramStatusInfor();
    ~CProgramStatusInfor();
    /****************************************************************************/
    /*!
     *  \brief Read program status from file to m_Status;
     *  \return true - program finished, false - not finished
     */
    /****************************************************************************/
    bool ReadProgramStatusFile();
    /****************************************************************************/
    /*!
     *  \brief Set RV position
     *  \param rvpos RV position
     */
    /****************************************************************************/
    void SetLastRVPosition(const DeviceControl::RVPosition_t& rvpos);
    /****************************************************************************/
    /*!
     *  \brief get last rv position
     *  \return DeviceControl::RVPosition_t from GetLastRVPosition
     */
    /****************************************************************************/
    DeviceControl::RVPosition_t GetLastRVPosition();

    /****************************************************************************/
    /*!
     *  \brief Set Last Reagent Group for normal protocol, not involve the cleaning program
     *  \param rgb - reagent group id
     */
    /****************************************************************************/
    void SetLastReagentGroup(QString rgb);
    /****************************************************************************/
    /*!
     *  \brief get last reagent group for normal protocol, not involve the cleaning program
     */
    /****************************************************************************/
    QString GetLastReagentGroup();
    /****************************************************************************/
    /*!
     *  \brief program ID
     *  \param ProgramID program id
     */
    /****************************************************************************/
    void SetProgramID(const QString& ProgramID);
    /****************************************************************************/
    /*!
     *  \brief Get program id
     *  \return QString from GetProgramId
     */
    /****************************************************************************/
    QString GetProgramId();
    /****************************************************************************/
    /*!
     *  \brief Set step id
     *  \param StepID - step id
     */
    /****************************************************************************/
    void SetStepID(int StepID);
    /****************************************************************************/
    /*!
     *  \brief Get step id
     *  \return QString from GetStepID
     */
    /****************************************************************************/
    int GetStepID();
    /****************************************************************************/
    /*!
     *  \brief set scenario
     *  \param Scenario scenario
     */
    /****************************************************************************/
    void SetScenario(const quint32 Scenario);
    /****************************************************************************/
    /*!
     *  \brief Get scenario
     *  \return quint32 from GetScenario
     */
    /****************************************************************************/
    quint32 GetScenario();
    /****************************************************************************/
    /*!
     *  \brief program is finished or not
     *  \return true - program finished, false - not finished
     */
    /****************************************************************************/
    bool IsProgramFinished();
    /****************************************************************************/
    /*!
     *  \brief set program to finished
     */
    /****************************************************************************/
    void SetProgramFinished();
    /****************************************************************************/
    /*!
     *  \brief retort is contaminted or not
     *  \return true- contaminted, false not
     */
    /****************************************************************************/
    bool IsRetortContaminted();
    /****************************************************************************/
    /*!
     *  \brief Get oven heating time
     *  \param ParaffinMeltingPoint  paraffin melting point
     *  \return quint64 form GetOvenHeatingTime
     */
    /****************************************************************************/
    quint64 GetOvenHeatingTime(quint32 ParaffinMeltingPoint);
    /****************************************************************************/
    /*!
     *  \brief update oven heating time
     *  \param Time oven heating time
     *  \param IsHeatingOn oven heating start time
     *  \param ResetFlag reset oven heating time to 0
     */
    /****************************************************************************/
    void UpdateOvenHeatingTime(quint64 Time, bool IsHeatingOn, bool ResetFlag = false);



private:
    void FlushFile();
    void SetStatus(const QString& key, const QString& value);
    QString GetStatus(const QString& key);
    QFile m_StatusFile;         //!< File for store program status
    QMap<QString, QString> m_Status;             //!< the pairs for keys and values
    quint64 m_LastTimeUpdateHeatingTime;        //!< the last time of update oven heating time
    bool m_LastHeatingOn;                       //!< store the last status of heating
};

#endif // PROGRAMSTATUSINFOR_H
