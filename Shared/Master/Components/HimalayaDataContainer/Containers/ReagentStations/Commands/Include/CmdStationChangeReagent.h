/****************************************************************************/
/*! \file CmdStationChangeReagent.h
 *
 *  \brief Change reagent in station command definition.
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

#ifndef MSGCLASSES_CMDSTATIONCHANGEREAGENT_H
#define MSGCLASSES_CMDSTATIONCHANGEREAGENT_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

class CmdStationChangeReagent : public Global::Command
{
    friend QDataStream & operator << (QDataStream &, const CmdStationChangeReagent &);
    friend QDataStream & operator >> (QDataStream &, CmdStationChangeReagent &);

public:
    CmdStationChangeReagent(int TimeOut, const QString& StationID, const QString& ReagentID);
    CmdStationChangeReagent(void);

    ~CmdStationChangeReagent(void);

    virtual QString GetName(void) const;

    static QString NAME; ///< Command name.
    const QString& StationID()const;
    const QString& ReagentID()const;

private:
    CmdStationChangeReagent(const CmdStationChangeReagent &); ///< Not implemented.
    const CmdStationChangeReagent &operator = (const CmdStationChangeReagent &); ///< Not implemented.

    QString m_StationID;
    QString m_ReagentID;
}; // end class CmdStationChangeReagent

inline QDataStream & operator << (QDataStream &Stream, const CmdStationChangeReagent &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationID;
    Stream << Cmd.m_ReagentID;
    return Stream;
}

inline QDataStream & operator >> (QDataStream &Stream, CmdStationChangeReagent &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationID;
    Stream >> Cmd.m_ReagentID;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONCHANGEREAGENT_H
