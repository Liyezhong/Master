/****************************************************************************/
/*! \file CmdRetortLock.h
 *
 *  \brief CmdRetortLock command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 25.03.2013
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

#ifndef MSGCLASSES_CMDRETORTLOCK_H
#define MSGCLASSES_CMDRETORTLOCK_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdRetortLock command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdRetortLock : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdRetortLock &);
    friend QDataStream & operator >> (QDataStream &, CmdRetortLock &);
public:
    CmdRetortLock();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdRetortLock(int Timeout, bool IsLock);
    ~CmdRetortLock();
    virtual QString GetName() const;
    bool IsLock() const {return m_Lock;}
    
private:
    CmdRetortLock(const CmdRetortLock &);                     ///< Not implemented.
    const CmdRetortLock & operator = (const CmdRetortLock &); ///< Not implemented.
private:
    bool m_Lock;
    
}; // end class CmdRetortLock

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdRetortLock &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_Lock;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdRetortLock &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_Lock;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDRETORTLOCK_H
