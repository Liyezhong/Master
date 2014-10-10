/****************************************************************************/
/*! \file CmdParaffinMeltPointChanged.h
 *
 *  \brief Notice Scheduler that Paraffin melting Point is Changed in GUI.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-09-18
 *  $Author:    $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_CMDPARAFFINMELTPOINTCHANGED_H
#define MSGCLASSES_CMDPARAFFINMELTPOINTCHANGED_H

#include "Global/Include/Commands/Command.h"
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdParaffinMeltPointChanged
 */
/****************************************************************************/
class CmdParaffinMeltPointChanged : public Global::Command
{
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of friend operator<<
     */
    /****************************************************************************/
    friend QDataStream & operator << (QDataStream &, const CmdParaffinMeltPointChanged &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of friend operator>>
     */
    /****************************************************************************/
    friend QDataStream & operator >> (QDataStream &, CmdParaffinMeltPointChanged &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of GetLastMeltPoint
     */
    /****************************************************************************/
    inline int GetLastMeltPoint() const {return m_LastMeltPoint;}
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of GetCurrentMeltPoint
     */
    /****************************************************************************/
    inline int GetCurrentMeltPoint() const {return m_CurrentMeltPoint;}
public:
    CmdParaffinMeltPointChanged(int TimeOut, int lastMeltPoint, int currentMeltPoint);
    CmdParaffinMeltPointChanged(void);

    ~CmdParaffinMeltPointChanged(void);

    virtual QString GetName(void) const;
    static QString NAME; ///< Command name.
private:
    CmdParaffinMeltPointChanged(const CmdParaffinMeltPointChanged &); ///< Not implemented.
    const CmdParaffinMeltPointChanged &operator = (const CmdParaffinMeltPointChanged &); ///< Not implemented.
    int m_LastMeltPoint;
    int m_CurrentMeltPoint;

 }; // end class CmdParaffinMeltPointChanged

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function operator<<
 *
 *  \return from operator<<
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdParaffinMeltPointChanged &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    Stream << Cmd.GetLastMeltPoint();
    Stream << Cmd.GetCurrentMeltPoint();
    return Stream;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function operator>>
 *
 *  \return from operator>>
 */
/****************************************************************************/
inline QDataStream & operator >> (QDataStream &Stream, CmdParaffinMeltPointChanged &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    Stream >> Cmd.m_LastMeltPoint;
    Stream >> Cmd.m_CurrentMeltPoint;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDPARAFFINMELTPOINTCHANGED_H
