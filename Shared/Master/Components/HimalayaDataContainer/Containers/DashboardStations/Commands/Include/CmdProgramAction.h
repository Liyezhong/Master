/****************************************************************************/
/*! \file CmdProgramAction.h
 *
 *  \brief CmdProgramAction command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 22.03.2013
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

#ifndef MSGCLASSES_CMDPROGRAMACTION_H
#define MSGCLASSES_CMDPROGRAMACTION_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {
using namespace DataManager::Global;
/****************************************************************************/
/*!
 *  \brief  This class implements a CmdProgramAction command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdProgramAction : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdProgramAction &);
    friend QDataStream & operator >> (QDataStream &, CmdProgramAction &);
public:
    CmdProgramAction();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdProgramAction(int Timeout, int& ProgramID, ProgramActionType_t ActionType);
    ~CmdProgramAction();
    virtual QString GetName() const;
    int GetProgramID() const {return m_ProgramID;}
    inline ProgramActionType_t ProgramActionType() const {return m_ActionType;}

private:
    CmdProgramAction(const CmdProgramAction &);                     ///< Not implemented.
    const CmdProgramAction & operator = (const CmdProgramAction &); ///< Not implemented.
private:
    int      m_ProgramID;
    ProgramActionType_t m_ActionType;
}; // end class CmdProgramAction

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdProgramAction &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_ProgramID;
    Stream << Cmd.m_ActionType;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdProgramAction &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_ProgramID;
    int temp;
    Stream >> temp;
    Cmd.m_ActionType = (ProgramActionType_t)temp;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDPROGRAMACTION_H
