/****************************************************************************/
/*! \file CmdRetortLockStatus.h
 *
 *  \brief CmdRetortLockStatus command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 26.03.2013
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
  *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_CMDRETORTLOCKSTATUS_H
#define MSGCLASSES_CMDRETORTLOCKSTATUS_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdRetortLockStatus command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdRetortLockStatus : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdRetortLockStatus &);
    friend QDataStream & operator >> (QDataStream &, CmdRetortLockStatus &);
public:
    CmdRetortLockStatus();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdRetortLockStatus(int Timeout, bool IsLocked);
    ~CmdRetortLockStatus();
    virtual QString GetName() const;
    bool IsLocked() const {return m_Locked;}
    
private:
    CmdRetortLockStatus(const CmdRetortLockStatus &);                     ///< Not implemented.
    const CmdRetortLockStatus & operator = (const CmdRetortLockStatus &); ///< Not implemented.
private:
    bool m_Locked;
    
}; // end class CmdRetortLockStatus

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdRetortLockStatus &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_Locked;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdRetortLockStatus &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_Locked;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDRETORTLOCKSTATUS_H
