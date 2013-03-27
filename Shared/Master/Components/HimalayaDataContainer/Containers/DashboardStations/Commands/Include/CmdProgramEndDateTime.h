/****************************************************************************/
/*! \file CmdProgramEndDateTime.h
 *
 *  \brief CmdProgramEndDateTime command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 27.03.2013
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

#ifndef MSGCLASSES_CMDPROGRAMENDDATETIME_H
#define MSGCLASSES_CMDPROGRAMENDDATETIME_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>
#include <QDateTime>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdProgramEndDateTime command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdProgramEndDateTime : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdProgramEndDateTime &);
    friend QDataStream & operator >> (QDataStream &, CmdProgramEndDateTime &);
public:
    CmdProgramEndDateTime();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdProgramEndDateTime(int Timeout, const QDateTime& EndDateTime);
    ~CmdProgramEndDateTime();
    virtual QString GetName() const;
    inline const QDateTime& EndDateTime()const {return m_EndDateTime;}
private:
    CmdProgramEndDateTime(const CmdProgramEndDateTime &);                     ///< Not implemented.
    const CmdProgramEndDateTime & operator = (const CmdProgramEndDateTime &); ///< Not implemented.
private:
    QDateTime m_EndDateTime;
    
}; // end class CmdProgramEndDateTime

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdProgramEndDateTime &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_EndDateTime;

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
inline QDataStream & operator >> (QDataStream &Stream, CmdProgramEndDateTime &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_EndDateTime;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDPROGRAMENDDATETIME_H
