/****************************************************************************/
/*! \file CmdStationResetData.h
 *
 *  \brief Reset station data command definition.
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

#ifndef MSGCLASSES_CMDSTATIONRESETDATA_H
#define MSGCLASSES_CMDSTATIONRESETDATA_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {


class CmdStationResetData : public Global::Command
{
    friend QDataStream & operator << (QDataStream &, const CmdStationResetData &);
    friend QDataStream & operator >> (QDataStream &, CmdStationResetData &);

public:
    CmdStationResetData(int TimeOut, const QString& StationID);
    CmdStationResetData(void);

    ~CmdStationResetData(void);

    virtual QString GetName(void) const;

    static QString NAME;
    inline const QString StationID() const { return m_StationID; }
private:
    CmdStationResetData(const CmdStationResetData &);
    const CmdStationResetData &operator = (const CmdStationResetData &);
    QString m_StationID;

}; // end class CmdStationResetData

inline QDataStream & operator << (QDataStream &Stream, const CmdStationResetData &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationID;
    return Stream;
}

inline QDataStream & operator >> (QDataStream &Stream, CmdStationResetData &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationID;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONRESETDATA_H
