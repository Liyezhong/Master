/****************************************************************************/
/*! \file CmdStationUpdate.h
 *
 *  \brief CmdStationUpdate command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 14.03.2012
 *   $Author:  $ V.Raghunath
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

#ifndef MSGCLASSES_CMDSTATIONUPDATE_H
#define MSGCLASSES_CMDSTATIONUPDATE_H

#include <QByteArray>
#include "Global/Include/Commands/Command.h"

namespace MsgClasses {
/****************************************************************************/
/*!
 *  \brief  This class implements a CmdStationUpdate command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdStationUpdate : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdStationUpdate &);
    friend QDataStream & operator >> (QDataStream &, CmdStationUpdate &);
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdStationUpdate(int Timeout, const QDataStream &StationUpdateDataStream);
    CmdStationUpdate();
    ~CmdStationUpdate();
    virtual QString GetName() const;
    QByteArray const & GetStationUpdateData() const { return m_StationUpdateByteArray;}
private:
    CmdStationUpdate(const CmdStationUpdate &);                       ///< Not implemented.
    const CmdStationUpdate & operator = (const CmdStationUpdate &);   ///< Not implemented.
    QByteArray m_StationUpdateByteArray; //!< Contains station update
}; // end class CmdStationUpdate

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdStationUpdate &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationUpdateByteArray;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdStationUpdate &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationUpdateByteArray;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONUPDATE_H
