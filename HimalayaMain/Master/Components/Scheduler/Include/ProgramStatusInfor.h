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

namespace Scheduler{
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
     *  \param ParaffinMeltPoint the melting point for paraffin
     *  \return true - program finished, false - not finished
     */
    /****************************************************************************/
    bool InitProgramStatus(quint32 ParaffinMeltPoint);
    /****************************************************************************/
    /*!
     *  \brief clear oven heating time
     *  \param ParaffinMeltPoint the melting point for paraffin
     *  \param Reset true clear Heating time slice, otherwise false
     */
    /****************************************************************************/
    void ResetOvenHeatingTime(quint32 ParaffinMeltPoint, bool Reset = false);
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
     *  \return Qstring from GetLastReagentGroup
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
     *  \brief Set error flag
     *  \param ErrFlag - error flag
     */
    /****************************************************************************/
    void SetErrorFlag(int ErrFlag);
    /****************************************************************************/
    /*!
     *  \brief get error flag
     *  \return int from GetErrorFlag
     */
    /****************************************************************************/
    int GetErrorFlag();

    /****************************************************************************/
    /*!
     *  \brief Set station list
     *  \param List - QList
     */
    /****************************************************************************/
    void SetStationList(const QList<QString>&  List);
    /****************************************************************************/
    /*!
     *  \brief Get step id
     *  \return QString from GetStepID
     */
    /****************************************************************************/
    QList<QString> GetStationList();
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
     *  \brief SetPressureDriftOffset
     *  \param PressureDriftOffset - qreal
     */
    /****************************************************************************/
    void SetPressureDriftOffset(qreal PressureDriftOffset);
    /****************************************************************************/
    /*!
     *  \brief Get GetPressureDriftOffset
     *  \return qreal from GetPressureDriftOffset
     */
    /****************************************************************************/
    qreal GetPressureDriftOffset();
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
     *  \brief Get oven heating time by ms
     *  \return quint64 form seconds that oven has been heated.
     */
    /****************************************************************************/
    quint64 GetOvenHeatingTime();
    /****************************************************************************/
    /*!
     *  \brief Get the remaining time for melting paraffin by ms
     *  \return quint64 form seconds for melting paraffin
     */
    /****************************************************************************/
    quint64 GetRemaingTimeForMeltingParffin();
    /****************************************************************************/
    /*!
     *  \brief update oven heating time
     *  \param Time oven heating time
     *  \param IsHeatingOn oven heating start time
     *  \return true / false
     */
    /****************************************************************************/
    void UpdateOvenHeatingTime(quint64 Time, bool IsHeatingOn);

private:
    bool CalculateTime(quint64& HeatingTime, quint64& UnHeatingTime);
    bool ReadProgramStatusFile();
    quint64 GetShutdownTime();
    void SetShutdownTime(quint64 Time);
    quint64 GetOvenHeatingStopDuration();
    void SetOvenHeatingStopDuration(quint64 Duration);

private:
    void FlushFile();
    void SetStatus(const QString& key, const QString& value);
    QString GetStatus(const QString& key);
    QFile m_StatusFile;         //!< File for store program status
    QMap<QString, QString> m_Status;             //!< the pairs for keys and values
    quint64 m_LastTimeUpdateHeatingTime;        //!< the last time of update oven heating time
    bool m_LastHeatingOn;                       //!< store the last status of heating
    quint64 m_MaxMeltTime;                      //!< max time for melting paraffin
    quint64 m_RemainTime;                       //!< remain time for emlting paraffin
    quint64 m_FirstStopHeatingTime;             //!< the first time for stop oven heating of on stop period
    bool m_IsInit;                              //!< True - m_StatusFile has been read and clear, or not
};
} //end of scheduler name space
#endif // PROGRAMSTATUSINFOR_H
