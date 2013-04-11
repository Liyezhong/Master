/****************************************************************************/
/*! \file CmdProgramStartReady.h
 *
 *  \brief CmdProgramStartReady command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 11.04.2013
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
  *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_CMDPROGRAMSTARTREADY_H
#define MSGCLASSES_CMDPROGRAMSTARTREADY_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>
#include <QDateTime>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdProgramStartReady command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdProgramStartReady : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdProgramStartReady &);
    friend QDataStream & operator >> (QDataStream &, CmdProgramStartReady &);
public:
    CmdProgramStartReady();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdProgramStartReady(int Timeout, bool IsReady);
    ~CmdProgramStartReady();
    virtual QString GetName() const;
    inline bool IsReady() const {return m_IsReady;}
 private:
    CmdProgramStartReady(const CmdProgramStartReady &);                     ///< Not implemented.
    const CmdProgramStartReady & operator = (const CmdProgramStartReady &); ///< Not implemented.
private:
    bool m_IsReady;
    
}; // end class CmdProgramStartReady

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdProgramStartReady &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_IsReady;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdProgramStartReady &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_IsReady;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDPROGRAMSTARTREADY_H
