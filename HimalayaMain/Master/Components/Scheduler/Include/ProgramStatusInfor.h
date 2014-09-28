/****************************************************************************/
/*! \file ProgramStatusInfro.h
 *
 *  \brief CProgramSelfTest class definition.
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
     */
    /****************************************************************************/
    DeviceControl::RVPosition_t GetLastRVPosition();
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
     */
    /****************************************************************************/
    QString GetProgramId();
    /****************************************************************************/
    /*!
     *  \brief Set step id
     *  \param StepID, step id
     */
    /****************************************************************************/
    void SetStepID(const QString& StepID);
    /****************************************************************************/
    /*!
     *  \brief Get step id
     */
    /****************************************************************************/
    QString GetStepID();
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
     */
    /****************************************************************************/
    quint64 GetOvenHeatingTime(quint32 ParaffinMeltingPoint);
    /****************************************************************************/
    /*!
     *  \brief update oven heating time
     *  \param Time oven heating time
     *  \param StartFlag oven heating start time
     *  \param ResetFlag reset oven heating time to 0
     */
    /****************************************************************************/
    void UpdateOvenHeatingTime(quint64 Time, bool StartFlag = false, bool ResetFlag = false);



private:
    void FlushFile();
    void SetStatus(const QString& key, const QString& value);
    QString GetStatus(const QString& key);
    QFile m_StatusFile;         //!< File for store program status
    QMap<QString, QString> m_Status;             //!< the pairs for keys and values
};

#endif // PROGRAMSTATUSINFOR_H
