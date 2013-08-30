/****************************************************************************/
/*! \file CmdProgramUpdate.h
 *
 *  \brief CmdProgramUpdate command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 18.03.2011
 *   $Author:  $ Y.Novak
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_CMDPROGRAMUPDATE_H
#define MSGCLASSES_CMDPROGRAMUPDATE_H

#include <Global/Include/Commands/Command.h>
//lint -e1527
namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdProgramUpdate command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdProgramUpdate : public Global::Command {

    friend QDataStream & operator << (QDataStream &, const CmdProgramUpdate &);
    friend QDataStream & operator >> (QDataStream &, CmdProgramUpdate &);
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdProgramUpdate(int Timeout, const QDataStream &ProgramDataStream);
    CmdProgramUpdate(int Timeout, const QDataStream &ProgramDataStream,
                     const QDataStream &NextProgramDataStream);
    CmdProgramUpdate();
    ~CmdProgramUpdate();
    virtual QString GetName() const;

    void SetProgramColorReplaced(bool ColorReplaced) { m_ProgramColorReplaced = ColorReplaced; }
    bool GetProgramColorReplaced() const { return m_ProgramColorReplaced;}

    /****************************************************************************/
    /**
     * \brief Retrieve Program data
     * \return Reagent Data
     **/
    /****************************************************************************/
    QByteArray const & GetProgramData() const { return m_ProgramByteArray;}
    QByteArray const & GetColorAssignedProgramData() const { return m_ColorAssignedProgramByteArray;}

private:
    CmdProgramUpdate(const CmdProgramUpdate &);                     ///< Not implemented.
    const CmdProgramUpdate & operator = (const CmdProgramUpdate &); ///< Not implemented.
    QByteArray m_ProgramByteArray;//!< Byte Array filled with Program data
    QByteArray m_ColorAssignedProgramByteArray;
    bool m_ProgramColorReplaced;
}; // end class CmdProgramUpdate

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream,const CmdProgramUpdate &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_ProgramByteArray;// << Cmd.m_ColorAssignedProgramByteArray << Cmd.m_ProgramColorReplaced;

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
inline QDataStream & operator >> (QDataStream &Stream,CmdProgramUpdate &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_ProgramByteArray;// >> Cmd.m_ColorAssignedProgramByteArray >> Cmd.m_ProgramColorReplaced;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDPROGRAMUPDATE_H
