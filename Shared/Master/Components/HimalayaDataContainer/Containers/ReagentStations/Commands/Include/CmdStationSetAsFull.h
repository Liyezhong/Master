/****************************************************************************/
/*! \file CmdStationSetAsFull.h
 *
 *  \brief Set the station as full command definition.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-02
 *  $Author:    $ R. Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_CMDSTATIONSETASFULL_H
#define MSGCLASSES_CMDSTATIONSETASFULL_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdStationSetAsFull
 */
/****************************************************************************/
class CmdStationSetAsFull : public Global::Command
{
    friend QDataStream & operator << (QDataStream &, const CmdStationSetAsFull &);
    friend QDataStream & operator >> (QDataStream &, CmdStationSetAsFull &);
public:
    CmdStationSetAsFull(int timeOut, const QString& stationID);
    CmdStationSetAsFull(void);

    ~CmdStationSetAsFull(void);

    virtual QString GetName(void) const;

    static QString NAME;
    inline const QString StationID() const { return m_StationID; }
private:
    CmdStationSetAsFull(const CmdStationSetAsFull &);
    const CmdStationSetAsFull &operator = (const CmdStationSetAsFull &);
    QString m_StationID;
}; // end class CmdStationSetAsFull

inline QDataStream & operator << (QDataStream &Stream, const CmdStationSetAsFull &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationID;
    return Stream;
}

inline QDataStream & operator >> (QDataStream &Stream, CmdStationSetAsFull &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationID;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONSETASFULL_H
