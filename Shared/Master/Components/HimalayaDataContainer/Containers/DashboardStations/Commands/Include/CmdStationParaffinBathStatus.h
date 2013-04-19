/****************************************************************************/
/*! \file CmdStationParaffinBathStatus.h
 *
 *  \brief CmdStationParaffinBathStatus command definition.
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

#ifndef MSGCLASSES_CMDSTATIONPARAFFINBATHSTATUS_H
#define MSGCLASSES_CMDSTATIONPARAFFINBATHSTATUS_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdStationParaffinBathStatus command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdStationParaffinBathStatus : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdStationParaffinBathStatus &);
    friend QDataStream & operator >> (QDataStream &, CmdStationParaffinBathStatus &);
public:
    CmdStationParaffinBathStatus();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdStationParaffinBathStatus(int Timeout, const QString&, DataManager::StationParaffinBathStatusType_t);
    ~CmdStationParaffinBathStatus();
    virtual QString GetName() const;
    inline DataManager::StationParaffinBathStatusType_t StationParaffinBathStatusType() const {return m_StationParaffinBathStatusType;} 
    inline const QString& StationParaffinBathID()const {return m_StationParaffinBathID;}
private:
    CmdStationParaffinBathStatus(const CmdStationParaffinBathStatus &);                     ///< Not implemented.
    const CmdStationParaffinBathStatus & operator = (const CmdStationParaffinBathStatus &); ///< Not implemented.
private:
    QString m_StationParaffinBathID;
    DataManager::StationParaffinBathStatusType_t m_StationParaffinBathStatusType;
    
}; // end class CmdStationParaffinBathStatus

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdStationParaffinBathStatus &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationParaffinBathID;
    Stream << Cmd.m_StationParaffinBathStatusType;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdStationParaffinBathStatus &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationParaffinBathID;
    int temp;
    Stream >> temp;
    Cmd.m_StationParaffinBathStatusType = (DataManager::StationParaffinBathStatusType_t)temp;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONPARAFFINBATHSTATUSS_H
