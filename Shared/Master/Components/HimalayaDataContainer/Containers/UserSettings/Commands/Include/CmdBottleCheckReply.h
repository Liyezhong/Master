/****************************************************************************/
/*! \file CmdBottleCheckReply.h
 *
 *  \brief Bottle check reply command.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2015-04-30
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

#ifndef MSGCLASSES_CMDBOTTLECHECKREPLY_H
#define MSGCLASSES_CMDBOTTLECHECKREPLY_H

#include "Global/Include/Commands/Command.h"
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdBottleCheckReply
 */
/****************************************************************************/
class CmdBottleCheckReply : public Global::Command
{
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of friend operator<<
     */
    /****************************************************************************/
    friend QDataStream & operator << (QDataStream &, const CmdBottleCheckReply &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of friend operator>>
     */
    /****************************************************************************/
    friend QDataStream & operator >> (QDataStream &, CmdBottleCheckReply &);
public:
    /*!
      * \brief constructor
    */
    CmdBottleCheckReply(int TimeOut);

    /*!
      * \brief constructor
    */
    CmdBottleCheckReply();

    /*!
      * \brief destructor
    */
    ~CmdBottleCheckReply(void);

    /*!
      * \brief function get name
      * \return name
    */
    virtual QString GetName(void) const;
    static QString NAME; ///< Command name.

private:
    /*!
      * \brief copy constructor
    */
    CmdBottleCheckReply(const CmdBottleCheckReply &); ///< Not implemented.
    /*!
      * \brief operator =
      * \return from CmdBottleCheckReply
    */
    const CmdBottleCheckReply &operator = (const CmdBottleCheckReply &); ///< Not implemented.
 }; // end class CmdBottleCheckReply

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function operator<<
 *  \param  Stream stream
 *  \param  Cmd cmd
 *  \return from operator<<
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdBottleCheckReply &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    return Stream;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function operator>>
 *  \param  Stream stream
 *  \param  Cmd cmd
 *  \return from operator>>
 */
/****************************************************************************/
inline QDataStream & operator >> (QDataStream &Stream, CmdBottleCheckReply &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDBOTTLECHECKREPLY_H
