/****************************************************************************/
/*! \file CmdProgramSelected.h
 *
 *  \brief CmdProgramSelected command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17.04.2013
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

#ifndef MSGCLASSES_CMDPROGRAMSELECTED_H
#define MSGCLASSES_CMDPROGRAMSELECTED_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdProgramSelected command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdProgramSelected : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdProgramSelected &);
    friend QDataStream & operator >> (QDataStream &, CmdProgramSelected &);
public:
    CmdProgramSelected();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdProgramSelected(int Timeout, const QString& ProgramID);
    ~CmdProgramSelected();
    virtual QString GetName() const;
    inline const QString& GetProgramID()const {return m_ProgramID;}


private:
    CmdProgramSelected(const CmdProgramSelected &);                     ///< Not implemented.
    const CmdProgramSelected & operator = (const CmdProgramSelected &); ///< Not implemented.
private:
    QString      m_ProgramID;
}; // end class CmdProgramSelected

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdProgramSelected &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_ProgramID;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdProgramSelected &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_ProgramID;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDPROGRAMSELECTED_H
