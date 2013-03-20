/****************************************************************************/
/*! \file CmdFillLeicaReagent.h
 *
 *  \brief CmdFillLeicaReagent command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 21.08.2012
 *   $Author:  $ Vikram MK
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

#ifndef MSGCLASSES_CMDFILLLEICAREAGENT_H
#define MSGCLASSES_CMDFILLLEICAREAGENT_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdFillLeicaReagent command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdFillLeicaReagent : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdFillLeicaReagent &);
    friend QDataStream & operator >> (QDataStream &, CmdFillLeicaReagent &);
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdFillLeicaReagent(int Timeout, const QString &StationId);
    CmdFillLeicaReagent();
    ~CmdFillLeicaReagent();
    virtual QString GetName() const;
    QString GetStationId() const;
private:    
    CmdFillLeicaReagent(const CmdFillLeicaReagent &);                     ///< Not implemented.
    const CmdFillLeicaReagent & operator = (const CmdFillLeicaReagent &); ///< Not implemented.
    QString      m_StationId;       ///< Station ID.
}; // end class CmdFillLeicaReagent

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdFillLeicaReagent &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationId;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdFillLeicaReagent &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationId;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDFILLLEICAREAGENT_H
