/****************************************************************************/
/*! \file CmdResetStationRms.h
 *
 *  \brief CmdResetStationRms command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17.03.2011, 21.08.2012
 *   $Author:  $ Y.Novak, Vikram MK
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

#ifndef MSGCLASSES_CMDRESETSTATIONRMS_H
#define MSGCLASSES_CMDRESETSTATIONRMS_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdResetStationRms command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdResetStationRms : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdResetStationRms &);
    friend QDataStream & operator >> (QDataStream &, CmdResetStationRms &);
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdResetStationRms(int Timeout, const QString &StationId);
    CmdResetStationRms();                                               ///< Not implemented.
    ~CmdResetStationRms();
    virtual QString GetName() const;
    QString GetStationId() const;
private:   
    CmdResetStationRms(const CmdResetStationRms &);                     ///< Not implemented.
    const CmdResetStationRms & operator = (const CmdResetStationRms &); ///< Not implemented.
    QString      m_StationId;       ///< Station ID.
}; // end class CmdResetStationRms

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdResetStationRms &Cmd)
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
inline QDataStream & operator >> (QDataStream &Stream, CmdResetStationRms &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationId;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDRESETSTATIONRMS_H
