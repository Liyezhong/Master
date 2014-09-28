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

    void SetLastRVPosition(const DeviceControl::RVPosition_t& rvpos);
    DeviceControl::RVPosition_t GetLastRVPosition();
    void SetProgramID(const QString& ProgramID);
    QString GetProgramId();
    void SetStepID(const QString& ProgramID);
    QString GetStepID();
    void SetScenario(const quint32 Scenario);
    quint32 GetScenario();
    bool IsProgramFinished();
    bool IsRetortContaminted();
    quint64 GetOvenHeatingTime(quint32 ParaffinMeltingPoint);
    void UpdateOvenHeatingTime(quint64 Time, bool StartFlag = false, bool ResetFlag = false);



private:
    void FlushFile();
    void SetStatus(const QString& key, const QString& value);
    QString GetStatus(const QString& key);
    QFile m_StatusFile;         //!< File for store program status
    QMap<QString, QString> m_Status;             //!< the pairs for keys and values
};

#endif // PROGRAMSTATUSINFOR_H
