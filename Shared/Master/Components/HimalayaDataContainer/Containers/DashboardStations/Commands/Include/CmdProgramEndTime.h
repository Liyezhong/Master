/****************************************************************************/
/*! \file CmdProgramEndTime.h
 *
 *  \brief CmdProgramEndTime command definition.
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

#ifndef MSGCLASSES_CMDPROGRAMENDTIME_H
#define MSGCLASSES_CMDPROGRAMENDTIME_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>
#include <QDateTime>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdProgramEndTime command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdProgramEndTime : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdProgramEndTime &);
    friend QDataStream & operator >> (QDataStream &, CmdProgramEndTime &);
public:
    CmdProgramEndTime();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdProgramEndTime(int Timeout, int TimeProposed,
                      int ParaffinWeltCostedtime,
                      int CostedTimeBeforeParaffin,
                      QList<QString>& StationList);
    ~CmdProgramEndTime();
    virtual QString GetName() const;
    inline int TimeProposed()const {return m_TimeProposed;}
    inline int ParaffinWeltCostedTime()const {return m_ParaffinWeltCostedtime;}
    inline int CostedTimeBeforeParaffin()const {return m_CostedTimeBeforeParaffin;}
    inline const QList<QString>& StationList()const {return m_StationList;}

    
private:
    CmdProgramEndTime(const CmdProgramEndTime &);                     ///< Not implemented.
    const CmdProgramEndTime & operator = (const CmdProgramEndTime &); ///< Not implemented.
private:
    int m_TimeProposed;//the costed time for selected program, seconds
    int m_CostedTimeBeforeParaffin; //seconds
    int m_ParaffinWeltCostedtime; //seconds
    QList<QString> m_StationList;
    
}; // end class CmdProgramEndTime

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdProgramEndTime &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_TimeProposed;
    Stream << Cmd.m_ParaffinWeltCostedtime;
    Stream << Cmd.m_CostedTimeBeforeParaffin;
    Stream << Cmd.m_StationList.count();
    for (int i = 0; i < Cmd.m_StationList.count(); i++)
    {
        Stream << Cmd.m_StationList.at(i);
    }
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
inline QDataStream & operator >> (QDataStream &Stream, CmdProgramEndTime &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_TimeProposed;
    Stream >> Cmd.m_ParaffinWeltCostedtime;
    Stream >> Cmd.m_CostedTimeBeforeParaffin;
    int stationCount = 0;
    Stream >> stationCount;
    QString stationID("");
    for (int i = 0; i < stationCount; i++)
    {
        Stream >> stationID;
        Cmd.m_StationList.push_back(stationID);
    }
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDPROGRAMENDDATETIME_H
