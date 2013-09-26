/****************************************************************************/
/*! \file CmdStationSetAsEmpty.h
 *
 *  \brief Set the station as empty command definition.
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

#ifndef MSGCLASSES_CMDSTATIONSETASEMPTY_H
#define MSGCLASSES_CMDSTATIONSETASEMPTY_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {


class CmdStationSetAsEmpty : public Global::Command
{
    friend QDataStream & operator << (QDataStream &, const CmdStationSetAsEmpty &);
    friend QDataStream & operator >> (QDataStream &, CmdStationSetAsEmpty &);
public:
    CmdStationSetAsEmpty(int timeOut, const QString& stationID);
    CmdStationSetAsEmpty(void);

    ~CmdStationSetAsEmpty(void);

    virtual QString GetName(void) const;

    static QString NAME;
    inline const QString StationID() const { return m_StationID; }
private:
    CmdStationSetAsEmpty(const CmdStationSetAsEmpty &);
    const CmdStationSetAsEmpty &operator = (const CmdStationSetAsEmpty &);
    QString m_StationID;
}; // end class CmdStationSetAsEmpty

inline QDataStream & operator << (QDataStream &Stream, const CmdStationSetAsEmpty &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationID;
    return Stream;
}

inline QDataStream & operator >> (QDataStream &Stream, CmdStationSetAsEmpty &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationID;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONSETASEMPTY_H
