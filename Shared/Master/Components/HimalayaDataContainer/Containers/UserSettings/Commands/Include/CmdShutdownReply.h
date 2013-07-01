/****************************************************************************/
/*! \file CmdShutdownReply.h
 *
 *  \brief After Slave board shutdown, scheduler will notify GUI by this command.
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

#ifndef MSGCLASSES_CMDSHUTDOWNREPLY_H
#define MSGCLASSES_CMDSHUTDOWNREPLY_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

class CmdShutdownReply : public Global::Command
{
    friend QDataStream & operator << (QDataStream &, const CmdShutdownReply &);
    friend QDataStream & operator >> (QDataStream &, CmdShutdownReply &);

public:
    CmdShutdownReply(int TimeOut);
    CmdShutdownReply(void);

    ~CmdShutdownReply(void);

    virtual QString GetName(void) const;

    static QString NAME; ///< Command name.
  
private:
    CmdShutdownReply(const CmdShutdownReply &); ///< Not implemented.
    const CmdShutdownReply &operator = (const CmdShutdownReply &); ///< Not implemented.

 }; // end class CmdShutdownReply

inline QDataStream & operator << (QDataStream &Stream, const CmdShutdownReply &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    return Stream;
}

inline QDataStream & operator >> (QDataStream &Stream, CmdShutdownReply &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSHUTDOWNREPLY_H
