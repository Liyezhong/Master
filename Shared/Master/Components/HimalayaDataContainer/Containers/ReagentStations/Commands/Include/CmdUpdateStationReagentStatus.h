/****************************************************************************/
/*! \file CmdUpdateStationReagentStatus.h
 *
 *  \brief when the running Program finishes with 1 Reagent, the Reagent status should be updated.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-20
 *  $Author:    $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2013 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_CMDUPDATESTATIONREAGENTSTATUS_H
#define MSGCLASSES_CMDUPDATESTATIONREAGENTSTATUS_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

class CmdUpdateStationReagentStatus : public Global::Command
{
    friend QDataStream & operator << (QDataStream &, const CmdUpdateStationReagentStatus &);
    friend QDataStream & operator >> (QDataStream &, CmdUpdateStationReagentStatus &);

public:
    //bUpdateCycle:true----update cycle
    //bUpdateCycle:false----update days
    CmdUpdateStationReagentStatus(int TimeOut, const QString& StationID, int CassetteCount, bool bUpdateCycle);
    CmdUpdateStationReagentStatus(void);

    ~CmdUpdateStationReagentStatus(void);

    virtual QString GetName(void) const;

    static QString NAME; ///< Command name.
    inline bool IsUpdateCycle()const {return m_IsUpdateCycle;}
    inline int CassetteCount()const{return m_CassetteCount;}
    inline const QString& StationID() const {return m_StationID;}
    

private:
    CmdUpdateStationReagentStatus(const CmdUpdateStationReagentStatus &); ///< Not implemented.
    const CmdUpdateStationReagentStatus &operator = (const CmdUpdateStationReagentStatus &); ///< Not implemented.
    bool m_IsUpdateCycle;
    int m_CassetteCount;
    QString m_StationID;
}; // end class CmdUpdateStationReagentStatus

inline QDataStream & operator << (QDataStream &Stream, const CmdUpdateStationReagentStatus &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationID;
    Stream << Cmd.m_IsUpdateCycle;
    Stream << Cmd.m_CassetteCount;
    return Stream;
}

inline QDataStream & operator >> (QDataStream &Stream, CmdUpdateStationReagentStatus &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationID;
    Stream >> Cmd.m_IsUpdateCycle;
    Stream >> Cmd.m_CassetteCount;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDUPDATESTATIONREAGENTSTATUS_H
