/****************************************************************************/
/*! \file CmdLeicaConsumables.h
 *
 *  \brief CmdLeicaConsumables command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 13.08.2012
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

#ifndef MSGCLASSES_CMDLEICACONSUMABLES_H
#define MSGCLASSES_CMDLEICACONSUMABLES_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdLeicaConsumables command.
 */
/****************************************************************************/
class CmdLeicaConsumables : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdLeicaConsumables &);
    friend QDataStream & operator >> (QDataStream &, CmdLeicaConsumables &);
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdLeicaConsumables(int Timeout, const QDataStream &RFIDStream);
    CmdLeicaConsumables();
    ~CmdLeicaConsumables();
    virtual QString GetName() const;
    QByteArray const & GetRFIDData() const { return m_RFIDByteArray;}
private:
    CmdLeicaConsumables(const CmdLeicaConsumables &);                       ///< Not implemented.
    const CmdLeicaConsumables & operator = (const CmdLeicaConsumables &);   ///< Not implemented.
    QByteArray m_RFIDByteArray;                                             ///< Byte Array containing RFID
}; // end class CmdRackUpdate


/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdLeicaConsumables &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_RFIDByteArray;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdLeicaConsumables &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_RFIDByteArray;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDLEICACONSUMABLES_H
