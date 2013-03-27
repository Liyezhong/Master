/****************************************************************************/
/*! \file CmdStationStatus.h
 *
 *  \brief CmdStationStatus command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 27.03.2013
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
  *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_CMDSTATIONSTATUS_H
#define MSGCLASSES_CMDSTATIONSTATUS_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdStationStatus command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdStationStatus : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdStationStatus &);
    friend QDataStream & operator >> (QDataStream &, CmdStationStatus &);
public:
    CmdStationStatus();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdStationStatus(int Timeout, const QString& StationID, DataManager::StationStatusType_t StationStatusType);
    ~CmdStationStatus();
    virtual QString GetName() const;
    inline DataManager::StationStatusType_t StationStatusType() const {return m_StationStatusType;} 
    inline const QString& StationID()const {return m_StationID;}
private:
    CmdStationStatus(const CmdStationStatus &);                     ///< Not implemented.
    const CmdStationStatus & operator = (const CmdStationStatus &); ///< Not implemented.
private:
    DataManager::StationStatusType_t m_StationStatusType;
    QString m_StationID;
    
}; // end class CmdStationStatus

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdStationStatus &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationID;
    Stream << Cmd.m_StationStatusType;
    return Stream;
}

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream from.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator >> (QDataStream &Stream, CmdStationStatus &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationID;
    int temp;
    Stream >> temp;
    Cmd.m_StationStatusType = (DataManager::StationStatusType_t)temp;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONSTATUS_H
