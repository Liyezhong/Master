/****************************************************************************/
/*! \file CmdShutdown.h
 *
 *  \brief Notice Scheduler that System will shutdown.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-06-27
 *  $Author:    $ Abe Yang
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

#ifndef MSGCLASSES_CMDSHUTDOWN_H
#define MSGCLASSES_CMDSHUTDOWN_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

class CmdShutdown : public Global::Command
{
    friend QDataStream & operator << (QDataStream &, const CmdShutdown &);
    friend QDataStream & operator >> (QDataStream &, CmdShutdown &);

public:
    CmdShutdown(int TimeOut);
    CmdShutdown(void);

    ~CmdShutdown(void);

    virtual QString GetName(void) const;

    static QString NAME; ///< Command name.
  
private:
    CmdShutdown(const CmdShutdown &); ///< Not implemented.
    const CmdShutdown &operator = (const CmdShutdown &); ///< Not implemented.

 }; // end class CmdShutdown

inline QDataStream & operator << (QDataStream &Stream, const CmdShutdown &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    return Stream;
}

inline QDataStream & operator >> (QDataStream &Stream, CmdShutdown &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSHUTDOWN_H
