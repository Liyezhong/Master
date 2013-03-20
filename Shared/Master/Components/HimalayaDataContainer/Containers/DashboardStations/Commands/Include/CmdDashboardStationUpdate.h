/****************************************************************************/
/*! \file CmdDashboardStationUpdate.h
 *
 *  \brief CmdDashboardStationUpdate command definition.
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

#ifndef MSGCLASSES_CmdDashboardStationUpdate_H
#define MSGCLASSES_CmdDashboardStationUpdate_H

#include <QByteArray>
#include "Global/Include/Commands/Command.h"

namespace MsgClasses {
/****************************************************************************/
/*!
 *  \brief  This class implements a CmdDashboardStationUpdate command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdDashboardStationUpdate : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdDashboardStationUpdate &);
    friend QDataStream & operator >> (QDataStream &, CmdDashboardStationUpdate &);
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdDashboardStationUpdate(int Timeout, const QDataStream &StationUpdateDataStream);
    CmdDashboardStationUpdate();
    ~CmdDashboardStationUpdate();
    virtual QString GetName() const;
    QByteArray const & GetDashboardStationUpdateData() const { return m_StationUpdateByteArray;}
private:
    CmdDashboardStationUpdate(const CmdDashboardStationUpdate &);                       ///< Not implemented.
    const CmdDashboardStationUpdate & operator = (const CmdDashboardStationUpdate &);   ///< Not implemented.
    QByteArray m_StationUpdateByteArray; //!< Contains station update
}; // end class CmdDashboardStationUpdate

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdDashboardStationUpdate &Cmd)
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
inline QDataStream & operator >> (QDataStream &Stream, CmdDashboardStationUpdate &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationUpdateByteArray;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CmdDashboardStationUpdate_H
