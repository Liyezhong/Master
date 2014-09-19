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

public:
    CmdParaffinMeltPointChanged(int TimeOut);
    CmdParaffinMeltPointChanged(void);

    ~CmdParaffinMeltPointChanged(void);

    virtual QString GetName(void) const;
    static QString NAME; ///< Command name.
  
private:
    CmdParaffinMeltPointChanged(const CmdParaffinMeltPointChanged &); ///< Not implemented.
    const CmdParaffinMeltPointChanged &operator = (const CmdParaffinMeltPointChanged &); ///< Not implemented.

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
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDPARAFFINMELTPOINTCHANGED_H
