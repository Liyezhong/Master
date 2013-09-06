/****************************************************************************/
/*! \file CmdStationSuckDrain.h
 *
 *  \brief CmdStationSuckDrain command definition.
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

#ifndef MSGCLASSES_CmdStationSuckDrain_H
#define MSGCLASSES_CmdStationSuckDrain_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdStationSuckDrain command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdStationSuckDrain : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdStationSuckDrain &);
    friend QDataStream & operator >> (QDataStream &, CmdStationSuckDrain &);
public:
    CmdStationSuckDrain();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdStationSuckDrain(int timeout, const QString& stationID, bool isStart, bool isSuck);
    ~CmdStationSuckDrain();
    virtual QString GetName() const;
    inline bool IsStart() const {return m_IsStart;}
    inline bool IsSuck() const {return m_IsSuck;}
    inline const QString& StationID()const {return m_StationID;}
private:
    CmdStationSuckDrain(const CmdStationSuckDrain &);                     ///< Not implemented.
    const CmdStationSuckDrain & operator = (const CmdStationSuckDrain &); ///< Not implemented.
private:
    QString m_StationID;
    bool m_IsStart;//true: start, false: completed
    bool m_IsSuck; //true: suck, false:drain
    
}; // end class CmdStationSuckDrain

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdStationSuckDrain &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StationID;
    Stream << Cmd.m_IsStart;
    Stream << Cmd.m_IsSuck;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdStationSuckDrain &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StationID;
    Stream >> Cmd.m_IsStart;
    Stream >> Cmd.m_IsSuck;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CmdStationSuckDrainS_H
